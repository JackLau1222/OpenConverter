import os
import tempfile
import re
import bisect
import requests
import lizard
import time
import argparse
import logging

def setup_logging(verbose_level):
    """Sets up logging based on the verbosity level"""
    log_levels = {
        0: logging.CRITICAL,   # Only critical logs
        1: logging.ERROR,       # Warnings and errors
        2: logging.INFO,      # Info, warnings, and errors
    }

    log_level = log_levels.get(verbose_level, logging.INFO)  # Default to INFO if invalid level

    logging.basicConfig(
        level=log_level,
        format='%(message)s',  # Print only the message content
        handlers=[
            logging.StreamHandler(),  # If you still want console output
            logging.FileHandler("workload_output.txt", mode='w')  # Logs also written to file
        ]
    )


# Define the complexity weights (you may need to adjust this to your enum or constants)
class CodeLevel:
    TRANSCODER = 10
    ENGINE = 8
    BUILDER = 5
    COMMON = 3
    SCRIPT = 2
    DEFAULT = 1
# Assume GITHUB_GITHUB_TOKEN, OWNER, REPO, and PR_NUMBER are already defined globally
# Get the complexity level based on the file's directory
def get_code_level(filename):
    # Take the path of file
    file_dir = os.path.dirname(filename)

    # Set complexity weight by file path
    if 'transcoder' in file_dir:
        return CodeLevel.TRANSCODER
    elif 'engine' in file_dir:
        return CodeLevel.ENGINE
    elif 'builder' in file_dir:
        return CodeLevel.BUILDER
    elif 'common' in file_dir:
        return CodeLevel.COMMON
    elif 'scripts' or 'CMakeLists.txt' or 'yaml' in file_dir:
        return CodeLevel.SCRIPT
    else:
        return CodeLevel.DEFAULT

# Send request by GitHub API
def send_request(url, headers):
    response = requests.get(url, headers=headers)
    return response

# get basic information of PR that contains SHA in the origin branch
def get_pr_info(owner, repo, pr_number):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/pulls/{pr_number}"
    headers = {"Authorization": f"Bearer {GITHUB_TOKEN}", "Accept": "application/vnd.github.v3+json"}
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            return response.json()
        elif response.status_code == 404:
            logging.error(f"PR {pr_number} not found.")
            return None
        else:
            logging.error(f"Error fetching PR info: {response.status_code}, {response.text}\n Error url: {url}")
            return None
    except requests.exceptions.RequestException as e:
        logging.error(f"Error during request: {e}")
        return None


# Get changed files in PR
def get_pr_files(owner, repo, pr_number):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/pulls/{pr_number}/files"
    headers = {"Authorization": f"Bearer {GITHUB_TOKEN}", "Accept": "application/vnd.github.v3+json"}
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            return response.json()
        else:
            logging.error(f"Error fetching PR files: {response.status_code}, {response.text}\n Error url: {url}")
            return None
    except requests.exceptions.RequestException as e:
        logging.error(f"Error during request: {e}")
        return None


# Get the history records of a file
def get_file_commits(owner, repo, sha, file_path):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/commits?sha={sha}&path={file_path}"
    headers = {"Authorization": f"Bearer {GITHUB_TOKEN}", "Accept": "application/vnd.github.v3+json"}
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            # stop a little time to provide time to GitHub
            time.sleep(SLEEP_TIME)
            return response.json()
        else:
            logging.error(f"Error fetching commit history for {file_path} and branch {sha}\n Error url: {url}")
            return None
    except requests.exceptions.RequestException as e:
        logging.error(f"Error during request: {e}")
        return None


# Get content in file
def get_file_content(owner, repo, file_path, sha):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/contents/{file_path}?ref={sha}"
    headers = {
        "Authorization": f"Bearer {GITHUB_TOKEN}",
        "Accept": "application/vnd.github.raw+json"  # use this 'Accept' when getting origin file content
    }
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            # stop a little time to provide time to GitHub
            time.sleep(SLEEP_TIME)
            return response.text
        else:
            logging.error(f"Error fetching file content for {file_path}: {response.status_code}, {response.text}\n Error url: {url}")
            return None
    except requests.exceptions.RequestException as e:
        logging.error(f"Error during request: {e}")
        return None

# get origin files from pr
def get_original_files_from_pr(owner, repo, pr_number):
    # get data from PR
    pr_info = get_pr_info(owner, repo, pr_number)
    if not pr_info:
        return []
    global PR_TITLE
    PR_TITLE = pr_info["title"]
    # get the sha of head from PR
    head_sha = pr_info["head"]["sha"]
    head_ref = pr_info["head"]["ref"]

    # get the sha of base from PR
    base_sha = pr_info["base"]["sha"]
    base_ref = pr_info["base"]["ref"]

    # get changed file in PR
    pr_files = get_pr_files(owner, repo, pr_number)
    if not pr_files:
        return []

    original_files = []

    for file_info in pr_files:
        file_path = file_info["filename"]

        # get the earliest history record records of file in source branch and find the earliest commit
        commits = get_file_commits(owner, repo, head_ref, file_path)
        if not commits or len(commits) == 0:
            # new file in source branch but not merge
            original_files.append({
                "file_path": file_path,
                "original_content": ''
            })
            continue
        earliest_commit_sha = commits[-1]['sha']
        # get the content of file in source branch
        file_content = get_file_content(owner, repo, file_path, earliest_commit_sha)
        if file_content:
            original_files.append({
                "file_path": file_path,
                "original_content": file_content
            })
            logging.info(f"Origin File: {file_path}")
    return original_files


# Analyze complexity by file content
def analyse_cpp_file(file_content):
    # Create a temporary file to store the file content
    with tempfile.NamedTemporaryFile(delete=False, mode='w', suffix='.cpp') as temp_file:
        temp_file.write(file_content)
        temp_file_path = temp_file.name
    # Use lizard to analyze the file
    result = lizard.analyze_file(temp_file_path)
    # Delete the temporary file
    os.remove(temp_file_path)
    return result


def analyse_python_file(file_content):
    # Create a temporary file to store the file content
    with tempfile.NamedTemporaryFile(delete=False, mode='w', suffix='.py') as temp_file:
        temp_file.write(file_content)
        temp_file_path = temp_file.name
    # Use lizard to analyze the file
    result = lizard.analyze_file(temp_file_path)
    # Delete the temporary file
    os.remove(temp_file_path)
    return result


def get_function_complexity(file_content, level, patch, file_name):
    try:
        complexity_data = []

        changes = []
        # Match diff lines
        diff_line_pattern = r'@@ -(\d+),(\d+) \+(\d+),(\d+) @@'
        lines = patch.split('\n')
        # Match the range of lines
        match = re.search(diff_line_pattern, patch)
        if not match:
            return changes
        # Extract data of lines
        old_start, old_count, new_start, new_count = map(int, match.groups())
        # Record changed lines
        modified_lines = []
        count = old_start - 1
        # Analyze the changed lines
        for line in lines:
            count += 1
            if line.startswith('@@'):
                # Step into when have more changed result in a file
                match = re.search(diff_line_pattern, line)
                if not match:
                    return changes
                # Extract data of lines
                a, b, c, d = map(int, match.groups())
                count = a - 1

            if line.startswith('-') or line.startswith('+'):
                modified_lines.append(count)

        # Record changed lines
        if modified_lines:
            changes.append({
                'changed': modified_lines,
            })

        if os.path.splitext(file_name)[1] in DEFAULT_CPP_ENDS_WITH_FILE:
            result = analyse_cpp_file(file_content)
        elif os.path.splitext(file_name)[1] in DEFAULT_PYTHON_ENDS_WITH_FILE:
            result = analyse_python_file(file_content)
        else:
            return []

        # Analyze the result of file
        logging.info(f"File path: {file_name}")
        logging.info(f"File changed lines : {len(modified_lines)}")
        logging.info(f"Number of functions: {len(result.function_list)}")
        logging.info(f"Complexity Level: {level}")

        # Analyze the result of each function
        for function in result.function_list:
            for change in changes:
                start = function.start_line
                end = function.end_line
                c = change['changed']
                start_pos = bisect.bisect_left(c, start)
                if start_pos < len(c) and c[start_pos] <= end:
                    # analyse different level according to different function cyclomatic complexity
                    categorized_complexity = 1
                    if function.cyclomatic_complexity <= 5:
                        categorized_complexity = 1
                    elif function.cyclomatic_complexity <= 10:
                        categorized_complexity = 2
                    elif function.cyclomatic_complexity <= 20:
                        categorized_complexity = 3
                    elif function.cyclomatic_complexity <= 50:
                        categorized_complexity = 4
                    else:
                        categorized_complexity = 5
                    complexity_data.append({
                        "function_name": function.name,
                        "complexity": categorized_complexity,
                        "lines": function.length,
                        'start_line': function.start_line,
                        'end_line': function.end_line,
                    })
                    logging.info(f"  ——The information of changed function——")
                    logging.info(f"  1.Function: {function.name}")
                    logging.info(f"  2.Complexity: {function.cyclomatic_complexity}")
                    logging.info(f"  3.Lines: {function.length}")
                    logging.info(f"  4.CodeLevel : {level}")

        return complexity_data

    except Exception as e:
        logging.info(f"Error analyzing complexity: {e}")
        return []


# Calculate the changes of files
def calculate_changes(files):
    changes = []

    original_files = get_original_files_from_pr(OWNER, REPO, PR_NUMBER)
    for file in files:
        filename = file['filename']
        patch = file.get('patch', '')
        additions = file['additions']
        deletions = file['deletions']
        contents_url = file['contents_url']
        sha = file['sha']  # We need to use the SHA to get the original content
        # Get the complexity weight of the file
        code_level = get_code_level(filename)
        # determine whether there are corresponding attributes in the object
        # if 'previous_filename' in file:
        #     filename = file['previous_filename']
        for original_file in original_files:
            if original_file['file_path'] == filename:
                changes.append({
                    'filename': filename,
                    'additions': additions,
                    'deletions': deletions,
                    'contents_url': contents_url,
                    'patch': patch,
                    'code_level': code_level,
                    'sha': sha,
                    'file_content': original_file['original_content']
                })
                original_files.remove(original_file)

    return changes


# Calculate the workload of PR
def calculate_pr_workload(changes):
    workload = 0
    changedLine = 0
    for change in changes:
        filename = change['filename']
        patch = change['patch']
        additions = change['additions']
        deletions = change['deletions']
        code_level = change['code_level']
        file_content = change['file_content']

        changedLine += additions + deletions
        if not os.path.splitext(filename)[1] in DEFAULT_CPP_ENDS_WITH_FILE and not os.path.splitext(filename)[1] in DEFAULT_PYTHON_ENDS_WITH_FILE:
            workload += (additions + deletions) * DEFAULT_NOT_ANALYSE_COMPLEXITY * 1 * 0.01
            continue

        if not file_content :
            logging.info(f"This PR add a new file but not merged:{filename}")
            workload += (additions + deletions) * DEFAULT_NOT_ANALYSE_COMPLEXITY * code_level * 0.01
            continue
        function_complexity = get_function_complexity(file_content, code_level, patch, filename)

        # Calculate workload
        for function in function_complexity:
            complexity = function["complexity"]
            # Workload calculation: (additions + deletions) * complexity * code_level
            workload += (additions + deletions) * complexity * code_level * 0.01

    return workload, changedLine


# Main function to execute the calculation
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--owner", required=True, help="Repository owner")
    parser.add_argument("--repo", required=True, help="Repository name")
    parser.add_argument("--pr-number", type=int, required=True, help="Pull request number")
    parser.add_argument("--token", required=True, help="GitHub API token")
    parser.add_argument('--verbose', type=int, choices=[0, 1, 2], default=1,
                        help='Set verbosity level: 0=critical, 1=info, 2=error (default: 1)')
    parser.add_argument("--default-code-level", type=int, default=1, help="Default code level")
    parser.add_argument("--default-not-analyse-complexity", type=int, default=1, help="Default not analyse complexity")
    parser.add_argument("--default-cpp-ends-with-file", default=".cpp,.h,.hpp,.m,.mm,.cc",
                        help="Default C++ ends with file")
    parser.add_argument("--default-python-ends-with-file", default=".py,", help="Default Python ends with file")
    parser.add_argument("--api-base-url", default="https://api.github.com", help="API base URL")
    parser.add_argument("--sleep-time", type=int, default=0, help="Sleep time between requests")

    args = parser.parse_args()

    setup_logging(args.verbose)

    global  GITHUB_TOKEN, OWNER, REPO, PR_NUMBER, \
        DEFAULT_CODE_LEVEL, DEFAULT_NOT_ANALYSE_COMPLEXITY, DEFAULT_CPP_ENDS_WITH_FILE, DEFAULT_PYTHON_ENDS_WITH_FILE,\
        API_BASE_URL,SLEEP_TIME
    GITHUB_TOKEN = args.token
    OWNER = args.owner
    REPO = args.repo
    PR_NUMBER = args.pr_number
    DEFAULT_CODE_LEVEL = args.default_code_level
    DEFAULT_NOT_ANALYSE_COMPLEXITY = args.default_not_analyse_complexity
    DEFAULT_CPP_ENDS_WITH_FILE =  args.default_cpp_ends_with_file.split(',')
    DEFAULT_PYTHON_ENDS_WITH_FILE = args.default_python_ends_with_file.split(',')
    API_BASE_URL = args.api_base_url
    SLEEP_TIME = args.sleep_time

    # Get the files of the PR
    files = get_pr_files(OWNER, REPO, PR_NUMBER)
    if not files:
        logging.error("No files found in the PR.")
        return
    # Calculate the changes of files
    changes = calculate_changes(files)

    # Calculate the workload of PR
    workload, changedLine = calculate_pr_workload(changes)
    logging.critical(f"Estimated workload for PR {PR_NUMBER}: {workload:.2f}")
    logging.critical(f"Estimated changedLine for PR {PR_NUMBER}: {changedLine}")


if __name__ == '__main__':
    try:
        start_time = time.time()  # Record the start tim
        main()
        end_time = time.time()  # Record the end time
        execution_time = end_time - start_time  # Calculate the elapsed time
        logging.info(f"Script executed in {execution_time:.2f} seconds")
    except Exception as e:
        logging.error(f"Error: {e}")

