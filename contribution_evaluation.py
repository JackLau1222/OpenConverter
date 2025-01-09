from collections import defaultdict

import requests
import lizard
import os
import tempfile
from enum import Enum

# GitHub API 配置
GITHUB_TOKEN = ''
REPO_OWNER = 'JackLau1222'
REPO_NAME = 'OpenConverter'
PR_NUMBER = 36  # PR号码




# 定义枚举类型，表示不同的难度等级
class ComplexityLevel(Enum):
    TRANSCODER = 5
    ENGINE = 4
    BUILDER = 3
    COMMON = 2
    SIMPLE = 1
# 根据文件路径判断文件所属的目录层级
def get_complexity_weight(filename):
    # 提取文件所在的目录
    file_dir = os.path.dirname(filename)

    # 根据目录结构设置复杂度
    if 'transcoder' in file_dir:
        return ComplexityLevel.TRANSCODER.value
    elif 'engine' in file_dir:
        return ComplexityLevel.ENGINE.value
    elif 'builder' in file_dir:
        return ComplexityLevel.BUILDER.value
    elif 'common' in file_dir:
        return ComplexityLevel.COMMON.value
    else:
        return ComplexityLevel.SIMPLE.value


# 请求 GitHub PR 文件变更信息
def get_pr_files(owner, repo, pr_number):
    url = f'https://api.github.com/repos/{owner}/{repo}/pulls/{pr_number}/files'
    headers = {'Authorization': f'token {GITHUB_TOKEN}'}
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response.json()
    else:
        print(f"Error fetching PR files: {response.status_code}, {response.text}")
        return []
def analyze_complexity_from_string(file_content,level):
    try:
        # 创建一个临时文件并将内容写入该文件
        with tempfile.NamedTemporaryFile(delete=False, mode='w', suffix='.cpp') as temp_file:
            temp_file.write(file_content)
            temp_file_path = temp_file.name

        # 使用 lizard 分析临时文件
        result = lizard.analyze_file(temp_file_path)

        # 输出文件分析结果
        print(f"File analyzed: {temp_file_path}")
        print(f"Number of functions: {len(result.function_list)}")
        print(f"Complexity Level: {level}")

        complexity_data = []
        for function in result.function_list:
            complexity_data.append({
                "function_name": function.name,
                "complexity": function.cyclomatic_complexity,
                "lines": function.length,
            })

            print(f"Function: {function.name}")
            print(f"  Complexity: {function.cyclomatic_complexity}")
            print(f"  Lines: {function.length}")
            print(f"ComplexityLevel : {level}")

        # 删除临时文件
        os.remove(temp_file_path)

        return complexity_data

    except Exception as e:
        print(f"Error analyzing complexity: {e}")
        return []

# 获取文件内容
def get_file_content(contents_url):
    headers = {
        "Authorization": f'token {GITHUB_TOKEN}',
        "Accept": "application/vnd.github.raw+json",
    }

    # 发起 GET 请求

    url = f'{contents_url}'
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response.text
    else:
        print(f"Error fetching file content: {response.status_code}, {response.text}")
        return ""

# 计算每个文件的改动行数
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
            'complexity_weight': complexity_weight , # 添加复杂度权重
        })

    return changes

# 计算 PR 的工作量评估
def calculate_pr_workload(changes, owner, repo):
    workload = 0

    for change in changes:
        filename = change['filename']
        patch = change['patch']
        additions = change['additions']
        deletions = change['deletions']
        complexity_weight = change['complexity_weight']
        # 跳过非 C++ 文件
        if not filename.endswith(('.cpp', '.h')):
            continue
        # 获取文件内容
        file_content = get_file_content(change['contents_url'])

        if not file_content:
            continue

        # 分析文件中函数的复杂度
        function_complexity = analyze_complexity_from_string(file_content, get_complexity_weight(filename))


        # 对函数的复杂度进行评估，这里简单地计算函数的复杂度值
        for func_data in function_complexity:
            complexity_value = func_data['complexity']
            # 假设为文件的层级、代码行数等设定权重
            workload += (additions + deletions) * complexity_value * complexity_weight

    return workload

def main():
    # 获取 PR 文件变更信息
    files = get_pr_files(REPO_OWNER, REPO_NAME, PR_NUMBER)

    if not files:
        print("No files found in the PR.")
        return

    # 计算文件的改动行数
    changes = calculate_changes(files)

    # 计算 PR 的工作量
    workload = calculate_pr_workload(changes, REPO_OWNER, REPO_NAME)

    print(f"Estimated workload for PR {PR_NUMBER}: {workload}")

if __name__ == '__main__':
    main()