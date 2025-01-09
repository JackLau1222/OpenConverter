import requests
import re
import radon.complexity as rc
from radon.visitors import ComplexityVisitor
from radon.raw import analyze
import os

# GitHub Token
GITHUB_TOKEN = "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"
HEADERS = {"Authorization": f"token {GITHUB_TOKEN}"}

# Define the base API URL
BASE_URL = "https://api.github.com"

# File type classification rules
FILE_CLASSIFICATION = {
    "kernel": ["/kernel/", "/core/"],
    "engine": ["/engine/", "/src/"],
    "interface": ["/interface/", "/api/"],
    "documentation": ["/docs/", "/documentation/"],
}


# Step 1: Get files and changes from a PR
def get_pr_files(repo_owner, repo_name, pr_number):
    # https://api.github.com/repos/JackLau1222/OpenConverter/pulls/36/files
    url = f"{BASE_URL}/repos/{repo_owner}/{repo_name}/pulls/{pr_number}/files"
    response = requests.get(url, headers=HEADERS)
    response.raise_for_status()
    files = response.json()
    file_changes = []
    for file in files:
        filename = file["filename"]
        if not file.get("patch"):
            continue
        changes = file['patch']

        line_changes = parse_patch(changes)
        file_changes.append((filename, line_changes))
    return file_changes


# Parse patch to get line changes
def parse_patch(patch):
    ranges = []
    for match in re.finditer(r"@@ -\d+,\d+ \+(\d+),(\d+) @@", patch):
        start, length = int(match.group(1)), int(match.group(2))
        ranges.append((start, start + length - 1))
    return ranges


# Step 2: Get function definition based on line range
def get_function_from_lines(filename, start_line, end_line):
    # Fetch file content from GitHub
    url = f"{BASE_URL}/repos/{repo_owner}/{repo_name}/contents/{filename}"
    response = requests.get(url, headers=HEADERS)
    response.raise_for_status()
    content = response.json()
    decoded_content = requests.utils.unquote(content["content"])
    lines = decoded_content.split("\n")

    # Extract function definition using line range
    function_lines = lines[start_line - 1: end_line]
    function_code = "\n".join(function_lines)
    return function_code


# Step 3: Calculate complexity
def calculate_complexity(code):
    try:
        visitor = ComplexityVisitor.from_code(code)
        cyclomatic_complexity = sum(block.complexity for block in visitor.blocks)
        raw_analysis = analyze(code)
        return cyclomatic_complexity, raw_analysis.loc
    except Exception as e:
        print(f"Error calculating complexity: {e}")
        return None, None


# Step 4: Classify file by level
def classify_file(filename):
    for level, patterns in FILE_CLASSIFICATION.items():
        if any(pattern in filename for pattern in patterns):
            return level
    return "unknown"


# Step 5: Evaluate PR workload
def evaluate_pr_workload(repo_owner, repo_name, pr_number):
    file_changes = get_pr_files(repo_owner, repo_name, pr_number)
    total_workload = 0
    for filename, line_ranges in file_changes:
        level = classify_file(filename)
        for start, end in line_ranges:
            function_code = get_function_from_lines(filename, start, end)
            cyclomatic_complexity, loc = calculate_complexity(function_code)
            if cyclomatic_complexity and loc:
                # Weight calculation: Complexity * LOC * Level Factor
                level_factor = {"kernel": 3, "engine": 2, "interface": 1, "documentation": 0.5}.get(level, 1)
                workload = cyclomatic_complexity * loc * level_factor
                total_workload += workload
    return total_workload


# Example usage
if __name__ == "__main__":
    repo_owner = "JackLau1222"
    repo_name = "OpenConverter"
    pr_number = 36
    workload = evaluate_pr_workload(repo_owner, repo_name, pr_number)
    print(f"Total workload for PR #{pr_number}: {workload}")
