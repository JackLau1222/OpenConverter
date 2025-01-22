import json

import requests
import argparse


def fetch_sonar_metrics(pr_number, repository, sonarcloud_api_token):
    """
    Fetch metrics from SonarCloud API and return metric names with their ratings.

    :param pr_number: Pull Request number
    :param repository: Repository name in the format "owner/repo"
    :param sonarcloud_api_token: SonarCloud API token
    :param output_type: "RATE", "NON_RATE", or "ALL" to determine which metrics to fetch
    :return: A tuple of rate metrics and non-rate metrics
    """

    # Construct the API URL
    base_url = "https://sonarcloud.io/api/measures/component"
    rate_metrics = ['new_reliability_rating', 'new_security_rating', 'new_security_review_rating',
                    'new_maintainability_rating']
    non_rate_metrics = [
        'new_technical_debt', 'new_branch_coverage', 'new_conditions_to_cover', 'new_coverage',
        'new_duplicated_blocks', 'new_duplicated_lines_density', 'new_duplicated_lines', 'new_line_coverage',
        'new_lines_to_cover', 'new_maintainability_rating', 'new_accepted_issues', 'new_blocker_violations',
        'new_bugs', 'new_code_smells', 'new_critical_violations', 'new_info_violations', 'new_violations',
        'new_lines', 'new_major_violations', 'new_minor_violations', 'new_security_hotspots', 'new_vulnerabilities',
        'new_reliability_rating', 'new_reliability_remediation_effort', 'new_security_hotspots_reviewed',
        'new_security_rating', 'new_security_remediation_effort', 'new_security_review_rating', 'new_sqale_debt_ratio',
        'new_uncovered_conditions', 'new_uncovered_lines'
    ]


    selected_metrics = rate_metrics + non_rate_metrics

    # Remove duplicates and join metrics into a comma-separated string
    metric_keys = ",".join(sorted(set(selected_metrics)))

    params = {
        "component": repository,
        "metricKeys": metric_keys,
        "pullRequest": pr_number
    }

    # Make the API request
    response = requests.get(base_url, params=params, auth=(sonarcloud_api_token, ""))

    if response.status_code != 200:
        raise Exception(
            f"Failed to fetch data from SonarCloud API. HTTP Status: {response.status_code}, Response: {response.text}")

    # Parse the response JSON
    data = response.json()

    # Extract and process metrics
    component_measures = data.get("component", {}).get("measures", [])
    if not component_measures:
        print("No measures found for the specified PR and REPOSITORY.")
        return [], []

    # Define rating mappings
    rating_mapping = {
        "1.0": "A",
        "2.0": "B",
        "3.0": "C",
        "4.0": "D",
        "5.0": "E"
    }

    measure_rate_data = []
    measure_non_rate_data = []

    for measure in component_measures:
        metric_name = measure.get("metric")
        metric_value = measure.get("value")
        if "periods" in measure:
            metric_value = measure.get("periods")[0].get("value")

        if metric_name in rate_metrics and metric_value in rating_mapping:
            rating = rating_mapping[metric_value]
            measure_rate_data.append({"metric_name": metric_name, "rating": rating})
        else:
            measure_non_rate_data.append({"metric_name": metric_name, "value": metric_value})

    return measure_rate_data, measure_non_rate_data


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--owner", required=True, help="Repository owner")
    parser.add_argument("--repo", required=True, help="Repository name")
    parser.add_argument("--pr-number", type=int, required=True, help="Pull request number")
    parser.add_argument("--token", required=True, help="SonarCloud API token")
    parser.add_argument("--output-type", choices=["RATE", "NON_RATE", "ALL"], default="ALL",
                        help="Type of metrics to fetch")

    args = parser.parse_args()
    global REPO, OWNER, PR_NUMBER,OUT_TYPE
    REPO = args.repo
    OWNER = args.owner
    PR_NUMBER = args.pr_number
    OUT_TYPE = args.output_type
    repository = f"{args.owner}_{args.repo}"
    try:
        measure_rate_data, measure_non_rate_data = fetch_sonar_metrics(args.pr_number, repository, args.token)
        # Print rating indicator
        print("\n📈 Metrics:")
        rating_icons = {
            'A': '🟢',
            'B': '🟡',
            'C': '🟠',
            'D': '🔴',
            'E': '⚫'
        }

        if OUT_TYPE != "NON_RATE":
            for measure in measure_rate_data:
                icon = rating_icons.get(measure['rating'], '⚪')
                print(f"{icon} {measure['metric_name']}: {measure['rating']}")
        if OUT_TYPE != "RATE":
            for measure in measure_non_rate_data:
                print(f"{measure['metric_name']}: {measure['value']}")

        # Print the Issues section
        print("\n🐛 Issues")
        new_issues_url = f"https://sonarcloud.io/project/issues?id={repository}&pullRequest={args.pr_number}&issueStatuses=OPEN,CONFIRMED&sinceLeakPeriod=true"
        accepted_issues_url = f"https://sonarcloud.io/project/issues?id={repository}&pullRequest={args.pr_number}&issueStatuses=ACCEPTED"

        # Get issues data
        issues_api_url = f"https://sonarcloud.io/api/measures/component?component={repository}&metricKeys=maintainability_issues,accepted_issues&pullRequest={args.pr_number}&additionalFields=periods"
        response = requests.get(issues_api_url, auth=(args.token, ""))
        if response.status_code == 200:
            data = response.json()
            measures = data.get('component', {}).get('measures', [])
            total_issues = 0
            accepted_issues = 0

            for measure in measures:
                if measure.get('metric') == 'maintainability_issues':
                    value = measure.get('value', '')
                    if isinstance(value, str) and '{' in value:
                        import json
                        value_dict = json.loads(value.replace("'", '"'))
                        total_issues = value_dict.get('total', 0)
                elif measure.get('metric') == 'accepted_issues':
                    accepted_issues = int(measure.get('value', 0))

            print(f"<a href='{new_issues_url}'>✅ {total_issues} New issues</a>")
            print(f"<a href='{accepted_issues_url}'>✅ {accepted_issues} Accepted issues</a>")
        else:
            print(f"<a href='{new_issues_url}'>✅ New issues</a>")
            print(f"<a href='{accepted_issues_url}'>✅ Accepted issues</a>")

        # Print the Measures section
        measures_url = f"https://sonarcloud.io/component_measures?id={repository}&pullRequest={args.pr_number}"
        print(f"\n<a href='{measures_url}'>📊 Measures</a>")

    except Exception as e:
        print(f"❌ Error: {e}")


if __name__ == "__main__":
    main()
