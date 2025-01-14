import os
import requests
import json


pr_number = os.getenv("PR_NUMBER") or 37
repository = os.getenv("REPOSITORY") or "JackLau1222_OpenConverter"
api_token = os.getenv("SONARCLOUD_API_TOKEN") or ""
OUTPUT_TYPE = "ALL"  # Options: "RATE", "NON_RATE", "ALL"

def fetch_sonar_metrics():
    """
    Fetch metrics from SonarCloud API and print metric names with their ratings.
    """

    if not all([pr_number, repository, api_token]):
        raise EnvironmentError("Please ensure PR_NUMBER, REPOSITORY, and SONARCLOUD_API_TOKEN are set as environment variables.")

    # Construct the API URL
    base_url = "https://sonarcloud.io/api/measures/component"
    component = repository  # Convert "owner/repo" to "owner_repo"
    # rate_metrics, non_rate_metrics = fetch_all_metrics()
    rate_metrics = ['new_reliability_rating','new_security_rating', 'new_security_review_rating', 'new_maintainability_rating']
    non_rate_metrics = ['new_technical_debt', 'new_branch_coverage', 'new_conditions_to_cover', 'new_coverage', 'new_duplicated_blocks', 'new_duplicated_lines_density', 'new_duplicated_lines', 'new_line_coverage', 'new_lines_to_cover', 'new_maintainability_rating', 'new_accepted_issues', 'new_blocker_violations', 'new_bugs', 'new_code_smells', 'new_critical_violations', 'new_info_violations', 'new_violations', 'new_lines', 'new_major_violations', 'new_minor_violations', 'new_security_hotspots', 'new_vulnerabilities', 'new_reliability_rating', 'new_reliability_remediation_effort', 'new_security_hotspots_reviewed', 'new_security_rating', 'new_security_remediation_effort', 'new_security_review_rating', 'new_sqale_debt_ratio', 'new_uncovered_conditions', 'new_uncovered_lines']
    # Determine metrics to query based on global OUTPUT_TYPE
    if OUTPUT_TYPE == "RATE":
        selected_metrics = rate_metrics
    elif OUTPUT_TYPE == "NON_RATE":
        selected_metrics = non_rate_metrics
    else:
        selected_metrics = rate_metrics + non_rate_metrics

    # Remove duplicates and join metrics into a comma-separated string
    metric_keys = ",".join(sorted(set(selected_metrics)))

    params = {
        "component": component,
        "metricKeys": metric_keys,
        "pullRequest": pr_number
    }

    # Make the API request
    response = requests.get(base_url, params=params, auth=(api_token, ""))

    if response.status_code != 200:
        raise Exception(f"Failed to fetch data from SonarCloud API. HTTP Status: {response.status_code}, Response: {response.text}")

    # Parse the response JSON
    data = response.json()

    # Extract and display metrics
    component_measures = data.get("component", {}).get("measures", [])

    if not component_measures:
        print("No measures found for the specified PR and repository.")
        return

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
        if  "periods" in measure:
            metric_value = measure.get("periods")[0].get("value")

        if metric_name in rate_metrics and metric_value in rating_mapping:
            rating = rating_mapping[metric_value]
            measure_rate_data.append({"metric_name": metric_name, "rating": rating})
        else:
            measure_non_rate_data.append({"metric_name": metric_name, "value": metric_value})

    return measure_rate_data, measure_non_rate_data
if __name__ == "__main__":
    try:
        measure_rate_data,measure_non_rate_data=fetch_sonar_metrics()

        print("Metrics:")
        print("Rate Metrics:")
        for measure in measure_rate_data:
            print(f"- {measure['metric_name']}: {measure['rating']}")
        print("Non-Rate Metrics:")
        for measure in measure_non_rate_data:
            print(f"- {measure['metric_name']}: {measure['value']}")
    except Exception as e:
        print(f"Error: {e}")
