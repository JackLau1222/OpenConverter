from collections import defaultdict

import requests
import lizard
import os



# GitHub API 配置
GITHUB_TOKEN = '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'
REPO_OWNER = 'JackLau1222'
REPO_NAME = 'OpenConverter'
PR_NUMBER = 36  # PR号码
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
def get_function_complexity(file_content):
    try:
        # 使用 lizard 库计算函数的圈复杂度
        result = lizard.analyze_file(file_content)

        if result.function_list is None:
            return []

        # 存储结果的列表
        complexity_data = []
        for function in result.function_list:
            complexity_data.append({
                "function_name": function.name,
                "complexity": function.cyclomatic_complexity,
                "lines": function.length
            })

        return complexity_data  # 确保返回完整列表

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
        changes.append({
            'filename': filename,
            'additions': additions,
            'deletions': deletions,
            'contents_url':contents_url,
            'patch': patch
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

        # 跳过非 C++ 文件
        if not filename.endswith(('.cpp', '.h')):
            continue
        # 获取文件内容
        file_content = get_file_content(change['contents_url'])

        if not file_content:
            continue

        # 获取文件函数的复杂度
        function_complexity = get_function_complexity(file_content)

        # 假设修改涉及函数复杂度与文件层级
        for func_data in function_complexity:
            complexity_value = func_data['complexity']
            # 假设为文件的层级、代码行数等设定权重
            complexity_weight = 1  # 假设权重为1，可以根据实际情况调整
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