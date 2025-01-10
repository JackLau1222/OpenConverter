import requests
import lizard
import os
import bisect
import tempfile
from enum import Enum
import re

# GitHub API configuration
GITHUB_TOKEN = os.getenv('GITHUB_TOKEN') or ''
REPO_OWNER = os.getenv('REPO_OWNER') or 'JackLau1222'
REPO_NAME = os.getenv('REPO_NAME') or 'OpenConverter'
PR_NUMBER = os.getenv('PR_NUMBER') or 36
DEFAULT_COMPLEXITY_WEIGHT = os.getenv('DEFAULT_COMPLEXITY_WEIGHT') or 1
DEFAULT_ENDS_WITH_FILE = os.getenv('DEFAULT_ENDS_WITH_FILE') or ('.cpp', '.h', '.hpp', '.m', '.mm', '.cc')

# declare some enum classes to represent complexity weight
class ComplexityWeight(Enum):
    TRANSCODER = 5
    ENGINE = 4
    BUILDER = 3
    COMMON = 2
    DEFAULT = DEFAULT_COMPLEXITY_WEIGHT


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
        return ComplexityWeight.DEFAULT.value


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


# analyze the complexity of file by its content
def get_function_complexity(file_content, level, patch, file_name):
    try:
        complexity_data = []

        changes = []
        # match diff lines
        diff_line_pattern = r'@@ -(\d+),(\d+) \+(\d+),(\d+) @@'
        lines = patch.split('\n')
        # match the range of lines
        match = re.search(diff_line_pattern, patch)
        if not match:
            return changes
        # extract data of lines
        old_start, old_count, new_start, new_count = map(int, match.groups())
        # record changed lines
        modified_lines = []
        count = old_start - 1
        # analyse the  changed lines
        for line in lines:
            count += 1
            if line.startswith('@@'):
                # step into when have more changed result in a file
                match = re.search(diff_line_pattern, line)
                if not match:
                    return changes
                # extract data of lines
                a, b, c, d = map(int, match.groups())
                count = a - 1

            if line.startswith('-') or line.startswith('+'):
                modified_lines.append(count)

        # record changed lines
        if modified_lines:
            changes.append({
                'changed': modified_lines,
            })

        # create a temporary file to store the file content
        with tempfile.NamedTemporaryFile(delete=False, mode='w', suffix='.cpp') as temp_file:
            temp_file.write(file_content)
            temp_file_path = temp_file.name

        # use lizard to analyze the file
        result = lizard.analyze_file(temp_file_path)

        # analyse the result of file
        print(f"File path: {file_name}")
        print(f"File changed lines : {len(modified_lines)}")
        print(f"Number of functions: {len(result.function_list)}")
        print(f"Complexity Level: {level}")

        # analyse the result of each function
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
                    print(f"  4.ComplexityWeight : {level}")

        # delete the temporary file
        os.remove(temp_file_path)

        return complexity_data

    except Exception as e:
        print(f"Error analyzing complexity: {e}")
        return []


# calculate the changes of files
def calculate_changes(files):
    changes = []

    for file in files:
        filename = file['filename']
        patch = file.get('patch', '')
        additions = file['additions']
        deletions = file['deletions']
        contents_url = file['contents_url']

        # get the  complexity weight of the file
        complexity_weight = get_complexity_weight(filename)
        # skip non-C++ files
        if not filename.endswith(DEFAULT_ENDS_WITH_FILE):
            complexity_weight = ComplexityWeight.DEFAULT.value
        changes.append({
            'filename': filename,
            'additions': additions,
            'deletions': deletions,
            'contents_url': contents_url,
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

        # get the content of file by its url
        file_content = get_file_content(change['contents_url'])
        if not file_content:
            continue

        function_complexity = []
        if filename.endswith(DEFAULT_ENDS_WITH_FILE):
            function_complexity = get_function_complexity(file_content, get_complexity_weight(filename), patch,
                                                          filename)
        else:
            function_complexity.append({'complexity': 1})
        # set the workload
        for function in function_complexity:
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
