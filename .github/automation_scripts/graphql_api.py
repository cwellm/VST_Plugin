import requests
import json

GQL_ENDPOINT = "https://api.github.com/graphql"

def request_api(token: str, query: dict, is_mutation: bool = False) -> tuple:
    headers = {"Authorization": f"bearer {token}", "Content-Type": "application/json"}

    query_type = "mutation" if is_mutation else "query"

    full_query_json = f"""\
    {query_type} {{ \
        {query} \
    }} \
    """

    data = json.dumps({"query": f"{full_query_json}"})

    print("The following query was done:")
    print(data)
    response = requests.post(headers=headers, data=data, url=GQL_ENDPOINT)

    code, content = response.status_code, response.content
    print("HTTP Status Code: ", code)
    print("Request Content: ", content)

    return code, response.json()
