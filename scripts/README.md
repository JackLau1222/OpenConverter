# Analysis PR  Scripts Introduction

## Overview

In the open source community, we always need to quantify the workload of a new PR and the indicators of changes made to the entire project. This will help us better evaluate the new PR and quantify its contribution from multiple perspectives.

There are two scripts that PR Workload Analysis Script and SonarCloud Metrics Fetcher Script and I will introduce the features and usage methods of these two scripts separately.

# PR Workload Analysis Script Guide

This Python script analyzes the workload of a GitHub pull request (PR) by examining the files changed in the PR, evaluating their complexity, and estimating the overall effort required. It supports both C++ and Python files and calculates workload based on cyclomatic complexity, code level, and changes made in the PR.

---

## Features
1. **Workload Estimation**:
   - Calculates workload based on additions, deletions, and code complexity.
   - Supports configurable complexity levels for different directories.
   
2. **Cyclomatic Complexity Analysis**:
   - Analyzes function complexity using `lizard` for supported file types.

3. **GitHub Integration**:
   - Fetches PR details, file changes, and commit history using the GitHub API.

---

## Prerequisites
1. **Python Environment**:
   - Python 3.6+ is required.
   - Install dependencies:
     ```bash
     pip install requests lizard
     ```

2. **GitHub Personal Access Token**:
   - Generate a GitHub token with `repo` and `read:org` permissions.
   
3. **Environment Variables**:
   
   ：Set the following required variables:
   
   - `PR_NUMBER`: The pull request number for which metrics need to be fetched.
   - `OWNER`: The owner of the repository (e.g., `my-org`).
   - `REPO`: The name of the repository (e.g., `my-repo`).
   - `GITHUB_TOKEN`: The token for authenticating with the Github API.
   
   - Optional variables (with default values):
     - `DEFAULT_CODE_LEVEL`: Default code complexity level (`1`).
     - `DEFAULT_NOT_ANALYSE_COMPLEXITY`: Default complexity for non-analyzed files (`1`).
     - `DEFAULT_CPP_ENDS_WITH_FILE`: File extensions to analyze (`.cpp, .h, .hpp, .m, .mm, .cc` ）for C++
     - `DEFAULT_PYTHON_ENDS_WITH_FILE `：File extensions to analyze (`.py` ）for Python

---

## How to Run the Script

**Set Environment Variables And Run the Script**:

```bash
python.exe workload_analyzation.py --owner JackLau1222 --repo OpenConverter --pr-number 36 --token yourtoken 
```

or you can add more parameters 

````
python.exe workload_analyzation.py --owner JackLau1222 --repo OpenConverter --pr-number 36 --token yourtoken --default-code-level 1 --default-not-analyse-complexity 1 --default-cpp-ends-with-file .cpp,.h,.hpp,.m,.mm,.cc --default-python-ends-with-file .py 
````

## Script Output

The script will output:

- **Estimated workload**: A numerical value representing the estimated effort.
- **Changed lines**: Total number of lines added and deleted in the PR.
- Detailed information on modified files, including function complexity, changed lines, and directory complexity.

Example output:

```
textCopy codeEstimated workload for PR 123: 15.75
Estimated changedLine for PR 123: 150
File path: src/transcoder/video.cpp
File changed lines: 45
Number of functions: 5
Complexity Level: 10
  ——The information of changed function——
  1.Function: processVideo
  2.Complexity: 3
  3.Lines: 25
  4.CodeLevel: 10
```

## Customization

You can customize the script by modifying:

- **Complexity Levels**: Update the `CodeLevel` class to redefine weights.
- **File Extensions**: Adjust `DEFAULT_CPP_ENDS_WITH_FILE` and `DEFAULT_PYTHON_ENDS_WITH_FILE` for additional file types.

------

## Troubleshooting

1. **Error: "No files found in the PR"**:
   - Ensure the `PR_NUMBER`, `OWNER`, and `REPO` environment variables are set correctly.
2. **Authentication Error**:
   - Verify the GitHub token's validity and scope.
3. **API Rate Limits**:
   - Use a token with higher API limits if analyzing large PRs frequently.

------

## Example

### Input:

Environment Variables:

```
PR_NUMBER=456
OWNER=my-org
REPO=my-repo
GITHUB_TOKEN=my-github-token
```

### Output:

```
Origin File: src/converter.cpp
Origin File: src/converter.h
File path: src/OpenConverter_v1.1/widget.cpp
File changed lines : 141
Number of functions: 8
Complexity Level: 1
  ——The information of changed function——
  1.Function: Widget::Widget
  2.Complexity: 1
  3.Lines: 49
  4.CodeLevel : 1
  ——The information of changed function——
  1.Function: Widget::handle_Converter_Result
  2.Complexity: 1
  3.Lines: 12
  4.CodeLevel : 1
  ——The information of changed function——
  1.Function: Transcoder::prepare_Encoder_Video
  2.Complexity: 2
  3.Lines: 108
  4.CodeLevel : 1
Estimated workload for PR 16: 60.58
Estimated changedLine for PR 16: 2375
```

# SonarCloud Metrics Fetcher Script Guide

This guide explains how to use the provided Python script to fetch metrics from the SonarCloud API for a specified pull request (PR) and repository.

## Features

1. **PR Multi-dimensional Analysis**:
   - Multi-dimensional Analyzes PR information by SonarCloud Metrics Fetcher 

2. **Reliable Information Integration**:
   - Fetches PR details, file changes, and commit history using the SonarCloud Metrics API.
3. **Rate Metrics for PR**:
   - Set some rate metrics for PR and give a better way to visualization change that in PR

## Prerequisites

1. **Python Environment**: Ensure you have Python installed on your system.

2. Required Python Libraries

   : Install the necessary libraries by running:

   ```bash
   pip install requests
   ```

3. **SonarCloud API Token**: Obtain a token from your SonarCloud account.

4. Environment Variables

   : Set the following environment variables:

   - `PR_NUMBER`: The pull request number for which metrics need to be fetched.
   - `OWNER`: The owner of the repository (e.g., `my-org`).
   - `REPO`: The name of the repository (e.g., `my-repo`).
   - `SONARCLOUD_API_TOKEN`: The token for authenticating with the SonarCloud API.
   - `OUTPUT_TYPE`： Determines which metrics to fetch. Options are:
     - `RATE`: Fetch only rate metrics.
     - `NON_RATE`: Fetch only non-rate metrics.
     - `ALL` (default): Fetch both rate and non-rate metrics.

## How to Run the Script

**Set Environment Variables And Run the Script**:

```bash
python.exe quality_rate.py --owner JackLau1222 --repo OpenConverter --pr-number 36 --token yourtoken --output-type ALL
```

## Script Output

The script fetches and categorizes the metrics into two groups:

### Rate Metrics

These metrics have a rating scale (e.g., A, B, C, D, E). The script will output them in the following format:

```
Rate Metrics:
- new_reliability_rating: A
- new_security_rating: B
```

### Non-Rate Metrics

These metrics display raw numerical values. The script will output them in the following format:

```
Non-Rate Metrics:
- new_technical_debt: 5
- new_branch_coverage: 85.3
```

## Customization

### Adjust Metrics

You can customize the rate and non-rate metrics in the script by modifying the `rate_metrics` and `non_rate_metrics` lists.

### Modify Output

You can enhance or format the output further by editing the respective `print` statements in the script.

## Troubleshooting

1. **Missing Environment Variables**:
   - Ensure all required environment variables are set.
   - If a variable is missing, the script will raise an error.
2. **Authentication Error**:
   - If the API token is invalid or missing, the script will fail to fetch data.
   - Verify the token and ensure it has the necessary permissions.
3. **Invalid Repository or PR**:
   - Ensure `OWNER`, `REPO`, and `PR_NUMBER` point to a valid repository and pull request.
   - Double-check the formatting of the `OWNER` and `REPO` values.
4. **SonarCloud API Issues**:
   - If the SonarCloud API is down or rate-limiting applies, the script may fail.
   - Retry after some time or check the SonarCloud status.

## Example

### Input:

Environment Variables:

```
python.exe quality_rate.py --owner JackLau1222 --repo OpenConverter --pr-number 36 --token yourtoken --output-type ALL
```

### Output:

```
Metrics:
Rate Metrics:
- new_reliability_rating: A
- new_security_rating: B
Non-Rate Metrics:
(No output as OUTPUT_TYPE is set to RATE)
```

# License

This script is provided under the MIT License. Feel free to modify and distribute it as needed.

For further questions or issues, please open an issue in the repository or contact the script maintainer.# Analysis PR  Scripts Introduction
