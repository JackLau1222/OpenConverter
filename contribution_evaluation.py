
import requests
import lizard
import os
import tempfile
from enum import Enum
import re
# GitHub API configuration
GITHUB_TOKEN = ''
REPO_OWNER = 'JackLau1222'
REPO_NAME = 'OpenConverter'
PR_NUMBER = 36  # Your PR number here

# declare some enum classes to represent complexity weight
class ComplexityWeight(Enum):
    TRANSCODER = 5
    ENGINE = 4
    BUILDER = 3
    COMMON = 2
    SIMPLE = 1

# analyse the complexity weight of file by its path
def get_complexity_weight(filename):
    # take the path of file
    file_dir = os.path.dirname(filename)

    # set complexity weight by file path
    if 'transcoder' in file_dir:
        return ComplexityWeight.TRANSCODER.value
    elif 'engine' in file_dir:
        return ComplexityWeight.ENGINE.value
    elif 'builder' in file_dir:
        return ComplexityWeight.BUILDER.value
    elif 'common' in file_dir:
        return ComplexityWeight.COMMON.value
    else:
        return ComplexityWeight.SIMPLE.value


# get some information of changed file
def get_pr_files(owner, repo, pr_number):
    url = f'https://api.github.com/repos/{owner}/{repo}/pulls/{pr_number}/files'
    headers = {'Authorization': f'token {GITHUB_TOKEN}'}
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response.json()
    else:
        print(f"Error fetching PR files: {response.status_code}, {response.text}")
        return []


# analyze the complexity of file by its content
def get_function_complexity(file_content,level):
    try:
        # create a temporary file to store the file content
        with tempfile.NamedTemporaryFile(delete=False, mode='w', suffix='.cpp') as temp_file:
            temp_file.write(file_content)
            temp_file_path = temp_file.name

        # use lizard to analyze the file
        result = lizard.analyze_file(temp_file_path)

        # analyse the result of file
        print(f"File analyzed: {temp_file_path}")
        print(f"Number of functions: {len(result.function_list)}")
        print(f"Complexity Level: {level}")

        complexity_data = []
        # analyse the result of each function
        for function in result.function_list:
            complexity_data.append({
                "function_name": function.name,
                "complexity": function.cyclomatic_complexity,
                "lines": function.length,
                "start_line": function.start,  # function start line
                "end_line": function.end,  # function end line
            })

            print(f"Function: {function.name}")
            print(f"  Complexity: {function.cyclomatic_complexity}")
            print(f"  Lines: {function.length}")
            print(f"  ComplexityWeight : {level}")

        # delete the temporary file
        os.remove(temp_file_path)

        return complexity_data

    except Exception as e:
        print(f"Error analyzing complexity: {e}")
        return []

# get the content of file by its url
def get_file_content(contents_url):
    headers = {
        "Authorization": f'token {GITHUB_TOKEN}',
        "Accept": "application/vnd.github.raw+json",
    }

    # post request to get the file content

    url = f'{contents_url}'
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response.text
    else:
        print(f"Error fetching file content: {response.status_code}, {response.text}")
        return ""
# extract changed functions from patch
def extract_changed_functions(patch, file_content,file_name):
    added_functions = []
    modified_functions = []

    added_lines = []
    removed_lines = []

    # analyse the patch
    lines = patch.splitlines()

    for line in lines:
        if line.startswith('+'):
            added_lines.append(line[1:])
        elif line.startswith('-'):
            removed_lines.append(line[1:])

    # get the added and removed lines
    added_lines_set = set(added_lines)
    removed_lines_set = set(removed_lines)

    # get the function complexity
    function_complexity = get_function_complexity(file_content,file_name)

    # confirm that the function is changed
    for function in function_complexity:
        start_line = function["start_line"]
        end_line = function["end_line"]

        # if the function is changed
        if any(line in added_lines_set or line in removed_lines_set for line in range(start_line, end_line + 1)):
            # analyse the function
            if any(line in added_lines_set for line in range(start_line, end_line + 1)):
                added_functions.append(function)
            else:
                modified_functions.append(function)

    return added_functions, modified_functions


# calculate the changes of files
def calculate_changes(files):
    changes = []

    for file in files:
        filename = file['filename']
        patch = file.get('patch', '')
        additions = file['additions']
        deletions = file['deletions']
        contents_url = file['contents_url']

        # 获取该文件的复杂度权重
        complexity_weight = get_complexity_weight(filename)

        changes.append({
            'filename': filename,
            'additions': additions,
            'deletions': deletions,
            'contents_url':contents_url,
            'patch': patch,
            'complexity_weight': complexity_weight,
        })

    return changes

# calculate the workload of PR
def calculate_pr_workload(changes):
    workload = 0

    for change in changes:
        filename = change['filename']
        patch = change['patch']
        additions = change['additions']
        deletions = change['deletions']
        complexity_weight = change['complexity_weight']
        # skip non-C++ files
        if not filename.endswith(('.cpp', '.h')):
            continue
        # get the content of file by its url
        file_content = get_file_content(change['contents_url'])
        if not file_content:
            continue
        # extract changed functions from patch
        added_functions, modified_functions = extract_changed_functions(patch, file_content,filename)

        # set the workload
        for function in added_functions + modified_functions:
            function_name = function["function_name"]
            complexity = function["complexity"]

            # workload calculation: (additions + deletions) * complexity * complexity_weight
            workload += (additions + deletions) * complexity * complexity_weight

    return workload

def main():
    # get the files of PR
    files = get_pr_files(REPO_OWNER, REPO_NAME, PR_NUMBER)

    if not files:
        print("No files found in the PR.")
        return

    # calculate the changes of files
    changes = calculate_changes(files)

    # calculate the workload of PR
    workload = calculate_pr_workload(changes)

    print(f"Estimated workload for PR {PR_NUMBER}: {workload}")

if __name__ == '__main__':
    main()
