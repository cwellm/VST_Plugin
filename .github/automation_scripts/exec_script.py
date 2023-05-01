import requests
import json
import argparse
import xml.etree.ElementTree as ET

from graphql_api import request_api

REPO_USER = "cwellm"
REPO_NAME = "VST_Plugin"

# -------------------------- Implemented queries --------------------------

# First, extend this list when implementing a new query. Then implement it as new method below.
implemented_queries = [
    "get_latest_release",
    "publish_latest_release_draft",
    "get_workflow_runs",
    "get_latest_main_commit_sha,"
    "get_workflow_runs_for_latest_main_commit",
    "check_latest_workflow_runs",
    "get_current_plugin_version",
    "increase_plugin_version"
]

# -------------------------- Methods --------------------------

def execute(token: str, script_name: str, is_mutation: bool = False):

    if script_name == "get_latest_release":
        return _get_latest_release(token)

    elif script_name == "publish_latest_release_draft":
        return _publish_latest_release_draft(token)
    
    elif script_name == "get_workflow_runs":
        return _get_workflow_runs(token)
    
    elif script_name == "get_latest_main_commit":
        return _get_latest_main_commit_sha(token)
    
    elif script_name == "get_workflow_runs_for_latest_main_commit":
        return _get_workflow_runs_for_latest_main_commit(token)
    
    elif script_name == "check_latest_workflow_runs":
        return _check_latest_workflow_runs(token)
    
    elif script_name == "get_current_plugin_version":
        return _get_current_plugin_version()
    
    elif script_name == "increase_plugin_version":
        return _increase_plugin_version()

    else:
        implemented = "\n".join(implemented_queries)
        err_msg = f"This script is not yet implemented! Available scripts:\n{implemented}"
        raise ValueError(err_msg)


def _get_latest_release(token):
    """
    Return value:
        {"code": , "id": , "is_draft": }
    """

    print("Getting Latest Release...")

    query_string = (
        f'repository(owner: "{REPO_USER}", name:"{REPO_NAME}") {{ '
            'releases(first:1) { ' 
                'nodes { '
                    'databaseId '
                    'isDraft'
                '}'
            '}'
        '}'   
    )
    code, content = request_api(token, query_string)
    results = content.get("data").get("repository").get("releases").get("nodes")[0] 
    return {"code": code, "id": results.get("databaseId"), "is_draft": results.get("isDraft")}


def _publish_latest_release_draft(token):
    """
    Needs to be implemented as http request, since GraphQL endpoint not available.

    Return value:
        {"code": , "content": }
    """

    print("Publishing Latest Release...")

    latest_release = _get_latest_release(token)
    print(f'Latest Release found with ID: {latest_release.get("id")}')

    if not latest_release.get("is_draft"):
        print("The latest release is not a draft: Cannot publish! Returning...")
        return 

    headers = {"Accept": "application/vnd.github+json", "Authorization": f"Bearer {token}",
               "X-GitHub-Api-Version": "2022-11-28"}

    url = (f'https://api.github.com/repos/{REPO_USER}/{REPO_NAME}/releases/'
           f'{latest_release.get("id")}')
    
    data_dict = {"draft": "false", "prerelease": "false"}

    response = requests.patch(url=url, headers=headers, data=json.dumps(data_dict))

    code, content = response.status_code, response.content
    print("HTTP Status Code: ", code)
    print("Request Content: ", content)

    return {"code": code, "content": response.json()}

def _get_workflow_runs(token):
    """
    Needs to be implemented as http request, since GraphQL endpoint not available.

    Return value:
        {"code": , "runs": ["name": , "node_id": , "commit_sha": , "status": , "conclusion": }]}
    """

    print("Get latest workflow run...")

    headers = {"Accept": "application/vnd.github+json", "Authorization": f"Bearer {token}",
               "X-GitHub-Api-Version": "2022-11-28"}

    url = f'https://api.github.com/repos/{REPO_USER}/{REPO_NAME}/actions/runs'
    
    response = requests.get(url=url, headers=headers)

    code, content_json = response.status_code, response.json()
    print("HTTP Status Code: ", code)
    print("Request Content...may be a little long here :-D ")

    result = []
    for item in content_json.get("workflow_runs"):
        result.append({"name": item.get("name"), "node_id": item.get("node_id"), 
            "commit_sha": item.get("head_sha"), "status": item.get("status"), 
            "conclusion": item.get("conclusion")})
    return {"code": code, "runs": result}

def _get_latest_main_commit_sha(token):
    """
    Return value:
        {"code": , "sha": }
    """

    print("Getting Latest Main Commit...")

    query_string = (
        f'repository(owner: "{REPO_USER}", name: "{REPO_NAME}") {{ '
            'refs(refPrefix: "refs/heads/", first: 1, query: "main") { '
                'edges { '
                    'node { '
                    '... on Ref { '
                        'name '
                        'target {'
                        '... on Commit {'
                            'oid'
                            '}'
                        '}'
                        '}'
                    '}'
                '}'
            '}'
        '}'
    )

    code, content = request_api(token, query_string)
    results = content.get("data").get("repository").get("refs").get("edges")[0].get("node")\
        .get("target").get("oid")
    return {"code": code, "sha": results}

def _get_workflow_runs_for_latest_main_commit(token):
    """
    Return value:
        {"code": , "runs": ["name": , "node_id": , "commit_sha": , "status": , "conclusion": }] }
    """   
    
    print("Filtering workflow runs for the latest commit...")

    workflow_runs = _get_workflow_runs(token)
    latest_commit = _get_latest_main_commit_sha(token)

    results = []
    for run in workflow_runs.get("runs"):
        if latest_commit.get("sha") == run.get("commit_sha"):
            print("Found run: ", run)
            results.append(run)

    return {"code": 200, "runs": results}

def _check_latest_workflow_runs(token) -> bool:
    """
    Return value:
        True
        exit(1) on error
    """   
    
    print("Checking latest workflow runs...")

    latest_runs = _get_workflow_runs_for_latest_main_commit(token)

    for run in latest_runs.get("runs"):
        if run.get("name") != "Release Next Version":
            if (run.get("status") != "completed") or (run.get("conclusion") != "success"):
                print("Not all latest (relevant) runs have concluded successfully. Aborting...")
                exit(1)

    return True

def _get_current_plugin_version(config_file_path="config/pluginfo.xml"):
    """
    This script is designed to be used in a bash(like) shell. The 'print' corresponds to an 'echo',
    while the 'return' will give the script exit status.
    """
    try:
        with open(config_file_path, "r") as file:
            xml_tree = ET.parse(file)
            root = xml_tree.getroot()
        version = root.find("version").text
        print(version)
        return 0
    except:
        return 1

def _increase_plugin_version(config_file_path="config/pluginfo.xml", version_type="minor"):
    """
    Only supports X.Y or X.Y.Z types of versions
    """
    try:
        with open(config_file_path, "r") as file:
            xml_tree = ET.parse(file)
            root = xml_tree.getroot()
            version = root.find("version").text

            version_numbers = version.split(".")
            if version_type == "minor":
                version_numbers[1] = str(int(version_numbers[1]) + 1)
                new_version = ".".join(version_numbers)
                root.find("version").text = new_version

        with open(config_file_path, "wb") as file:
            xml_tree.write(file)

        return 0
    
    except:
        return 1

# -------------------------- Script Call --------------------------

if __name__ == "__main__":

    parser = argparse.ArgumentParser(
                        prog='Automation Scripts for GitHub repositories',
                        description='Helps doing some GitHub workflows automatically.'
                        )
    parser.add_argument('-n', '--name', required=True)
    parser.add_argument('-t', '--token', required=True)
    args = parser.parse_args()

    execute(args.token, args.name)
