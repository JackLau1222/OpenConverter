import os
import tempfile
import re
import bisect
import requests
import lizard

# GitHub API configuration
TOKEN = os.getenv('TOKEN') or ''
OWNER = os.getenv('OWNER') or 'JackLau1222'
REPO = os.getenv('REPO') or 'OpenConverter'
PR_NUMBER = os.getenv('PR_NUMBER') or 37
DEFAULT_CODE_LEVEL = os.getenv('DEFAULT_CODE_LEVEL') or 1
DEFAULT_CPP_ENDS_WITH_FILE = os.getenv('DEFAULT_ENDS_WITH_FILE') or ('.cpp', '.h', '.hpp', '.m', '.mm', '.cc')
DEFAULT_PYTHON_ENDS_WITH_FILE = os.getenv('DEFAULT_ENDS_WITH_FILE') or ('.py',)
DEFAULT_NOT_ANALYSE_COMPLEXITY = os.getenv('DEFAULT_NOT_ANALYSE_COMPLEXITY') or 1
API_BASE_URL = "https://api.github.com"


# Define the complexity weights (you may need to adjust this to your enum or constants)
class CodeLevel:
    TRANSCODER = 10
    ENGINE = 8
    BUILDER = 5
    COMMON = 3
    DEFAULT = 1


# Assume GITHUB_TOKEN, OWNER, REPO, and PR_NUMBER are already defined globally

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
    else:
        return CodeLevel.DEFAULT

# Send request by GitHub API
def send_request(url, headers):
    response = requests.get(url, headers=headers)
    return response


# get basic information of PR that contains SHA in the origin branch
def get_pr_info(owner, repo, pr_number):
    url = f"https://api.github.com/repos/{owner}/{repo}/pulls/{pr_number}"
    headers = {"Authorization": f"TOKEN {TOKEN}", "Accept": "application/vnd.github.v3+json"}
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            return response.json()
        elif response.status_code == 404:
            print(f"PR {pr_number} not found.")
            return None
        else:
            print(f"Error fetching PR info: {response.status_code}, {response.text}")
            return None
    except requests.exceptions.RequestException as e:
        print(f"Error during request: {e}")
        return None


# Get changed files in PR
def get_pr_files(owner, repo, pr_number):
    url = f"https://api.github.com/repos/{owner}/{repo}/pulls/{pr_number}/files"
    headers = {"Authorization": f"TOKEN {TOKEN}", "Accept": "application/vnd.github.v3+json"}
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            return response.json()
        else:
            print(f"Error fetching PR files: {response.status_code}, {response.text}")
            return None
    except requests.exceptions.RequestException as e:
        print(f"Error during request: {e}")
        return None


# Get the history records of a file
def get_file_commits(owner, repo, sha, file_path):
    url = f"https://api.github.com/repos/{owner}/{repo}/commits?sha={sha}&path={file_path}"
    headers = {"Authorization": f"TOKEN {TOKEN}", "Accept": "application/vnd.github.v3+json"}
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            return response.json()
        else:
            print(f"Error fetching commit history for {file_path} and branch {sha}")
            return None
    except requests.exceptions.RequestException as e:
        print(f"Error during request: {e}")
        return None


# Get content in file
def get_file_content(owner, repo, file_path, sha):
    url = f"https://api.github.com/repos/{owner}/{repo}/contents/{file_path}?ref={sha}"
    headers = {
        "Authorization": f"TOKEN {TOKEN}",
        "Accept": "application/vnd.github.raw+json"  # use this 'Accept' when getting origin file content
    }
    try:
        response = send_request(url, headers)
        if response.status_code == 200:
            return response.text
        else:
            print(f"Error fetching file content for {file_path}: {response.status_code}, {response.text}")
            return None
    except requests.exceptions.RequestException as e:
        print(f"Error during request: {e}")
        return None


# get origin files from pr
def get_original_files_from_pr(owner, repo, pr_number):
    # get data from PR
    pr_info = get_pr_info(owner, repo, pr_number)
    if not pr_info:
        return []

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
            print(f"Origin File: {file_path}")
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

        if file_name.endswith(DEFAULT_CPP_ENDS_WITH_FILE):
            result = analyse_cpp_file(file_content)
        elif file_name.endswith(DEFAULT_PYTHON_ENDS_WITH_FILE):
            result = analyse_python_file(file_content)
        else:
            return []

        # Analyze the result of file
        print(f"File path: {file_name}")
        print(f"File changed lines : {len(modified_lines)}")
        print(f"Number of functions: {len(result.function_list)}")
        print(f"Complexity Level: {level}")

        # Analyze the result of each function
        for function in result.function_list:
            for change in changes:
                start = function.start_line
                end = function.end_line
                c = change['changed']
                start_pos = bisect.bisect_left(c, start)
                if start_pos < len(c) and c[start_pos] <= end:
                    # analyse different level according to different function cyclomatic complexity
                    if function.cyclomatic_complexity <= 5:
                        function.cyclomatic_complexity = 1
                    elif function.cyclomatic_complexity <= 10:
                        function.cyclomatic_complexity = 2
                    elif function.cyclomatic_complexity <= 20:
                        function.cyclomatic_complexity = 3
                    elif function.cyclomatic_complexity <= 50:
                        function.cyclomatic_complexity = 4
                    else:
                        function.cyclomatic_complexity = 5
                    complexity_data.append({
                        "function_name": function.name,
                        "complexity": function.cyclomatic_complexity,
                        "lines": function.length,
                        'start_line': function.start_line,
                        'end_line': function.end_line,
                    })
                    print(f"  ——The information of changed function——")
                    print(f"  1.Function: {function.name}")
                    print(f"  2.Complexity: {function.cyclomatic_complexity}")
                    print(f"  3.Lines: {function.length}")
                    print(f"  4.CodeLevel : {level}")

        return complexity_data

    except Exception as e:
        print(f"Error analyzing complexity: {e}")
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
        if  not filename.endswith(DEFAULT_CPP_ENDS_WITH_FILE) and  not filename.endswith(DEFAULT_PYTHON_ENDS_WITH_FILE):
            workload += (additions + deletions) * DEFAULT_NOT_ANALYSE_COMPLEXITY * 1 * 0.01
            continue

        if not file_content :
            print(f"This PR add a new file but not merged:{filename}")
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
    # Get the files of the PR
    files = get_pr_files(OWNER, REPO, PR_NUMBER)
    if not files:
        print("No files found in the PR.")
        return

    # Calculate the changes of files
    changes = calculate_changes(files)

    # Calculate the workload of PR
    workload, changedLine = calculate_pr_workload(changes)
    print(f"Estimated workload for PR {PR_NUMBER}: {workload:.2f}")
    print(f"Estimated changedLine for PR {PR_NUMBER}: {changedLine}")


if __name__ == '__main__':
    main()
