import os
import tempfile
import re
import bisect
import requests
import lizard

# GitHub API configuration
TOKEN = os.getenv('GITHUB_TOKEN') or ''
OWNER = os.getenv('OWNER') or 'JackLau1222'
REPO = os.getenv('REPO') or 'OpenConverter'
PR_NUMBER = os.getenv('PR_NUMBER') or 16
DEFAULT_CODE_LEVEL = os.getenv('DEFAULT_CODE_LEVEL') or 1
DEFAULT_ENDS_WITH_FILE = os.getenv('DEFAULT_ENDS_WITH_FILE') or ('.cpp', '.h', '.hpp', '.m', '.mm', '.cc')
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


# send request by GitHub API
def send_request(url, headers):
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response
    else:
        print(f"Error: {response.status_code}, {response.text}")
        return None


# get basic information of PR that contain SHA in origin branch
def get_pr_info(owner, repo, pr_number):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/pulls/{pr_number}"
    return send_request(url, {"Authorization": f"token {TOKEN}", "Accept": "application/vnd.github.v3+json"})


# get changed file in PR
def get_pr_files(owner, repo, pr_number):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/pulls/{pr_number}/files"
    return send_request(url, {"Authorization": f"token {TOKEN}", "Accept": "application/vnd.github.v3+json"})


# get the history records of file
def get_file_commits(owner, repo, sha, file_path):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/commits?sha={sha}&path={file_path}"
    return send_request(url, {"Authorization": f"token {TOKEN}", "Accept": "application/vnd.github.v3+json"})


# get content in file
def get_file_content(owner, repo, file_path, sha):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/contents/{file_path}?ref={sha}"
    headers = {
        "Authorization": f"token {TOKEN}",
        "Accept": "application/vnd.github.raw+json"  # use this 'Accept' when you get origin file content
    }
    return send_request(url, headers)
# get file content (origin content)
def get_file_content(owner, repo, file_path, sha):
    url = f"{API_BASE_URL}/repos/{owner}/{repo}/contents/{file_path}?ref={sha}"
    headers = {
        "Authorization": f"token {TOKEN}",
        "Accept": "application/vnd.github.raw+json"  # use this 'Accept' when you get origin file content
    }
    return send_request(url, headers)


# get origin files from pr
def get_original_files_from_pr(owner, repo, pr_number):
    # get data from PR
    pr_info = get_pr_info(owner, repo, pr_number).json()
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

    for file_info in pr_files.json():
        file_path = file_info["filename"]

        # get the earliest history record records of file in source branch and find the earliest commit
        commits = get_file_commits(owner, repo, head_sha, file_path).json()
        if not commits or len(commits) == 0:
            continue

        earliest_commit_sha = commits[-1]['sha']
        # get the content of file in source branch
        file_content = get_file_content(owner, repo, file_path, earliest_commit_sha).text
        if file_content:
            original_files.append({
                "file_path": file_path,
                "original_content": file_content
            })
            print(f"Origin File: {file_path}")
    return original_files


# Analyze complexity by file content
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

        # Create a temporary file to store the file content
        with tempfile.NamedTemporaryFile(delete=False, mode='w', suffix='.cpp') as temp_file:
            temp_file.write(file_content)
            temp_file_path = temp_file.name

        # Use lizard to analyze the file
        result = lizard.analyze_file(temp_file_path)

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

        # Delete the temporary file
        os.remove(temp_file_path)

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
        # Skip non-C++ files
        if not filename.endswith(DEFAULT_ENDS_WITH_FILE):
            code_level = CodeLevel.DEFAULT
        # determine whether there are corresponding attributes in the object
        if 'previous_filename' in file :
            filename = file['previous_filename']
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

    return changes


# Calculate the workload of PR
def calculate_pr_workload(changes):
    workload = 0

    for change in changes:
        filename = change['filename']
        patch = change['patch']
        additions = change['additions']
        deletions = change['deletions']
        code_level = change['code_level']
        file_content = change['file_content']

        function_complexity = []

        if not file_content:
            print(f"Error: Could not fetch the original content of {filename}")
            function_complexity.append({'complexity': 1})
        # Analyze the complexity using the original content
        if filename.endswith(DEFAULT_ENDS_WITH_FILE):
            function_complexity = get_function_complexity(file_content, code_level, patch, filename)
        else:
            function_complexity.append({'complexity': 1})
        # Calculate workload
        for function in function_complexity:
            complexity = function["complexity"]

            # Workload calculation: (additions + deletions) * complexity * code_level
            workload += (additions + deletions) * complexity * code_level

    return workload


# Main function to execute the calculation
def main():
    # Get the files of the PR
    files = get_pr_files(OWNER, REPO, PR_NUMBER).json()
    if not files:
        print("No files found in the PR.")
        return

    # Calculate the changes of files
    changes = calculate_changes(files)

    # Calculate the workload of PR
    workload = calculate_pr_workload(changes)
    print(f"Estimated workload for PR {PR_NUMBER}: {workload}")


if __name__ == '__main__':
    main()
