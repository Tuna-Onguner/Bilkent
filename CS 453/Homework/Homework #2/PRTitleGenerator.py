#
# Bilkent University
# Department of Computer Engineering
#
# CS453: Application Lifecycle Management
# Homework #2: Experimenting with Gemini API
#
# March 24, 2024, Sunday
#
# Author: Deniz Tuna Onguner [Bilkent ID: 22001788]
# Section 1
#
# Description: This is the main file of the homework 2.
# It includes the implementation of all the requirements asked for.
#


import csv  # The `csv` module is used to write the PRs and the generated titles to a CSV file
from os import getenv  # The `getenv` function is used to get the environment variables
from typing import Any  # The `Any` type is used to specify the type of the parameters and the return type

import google.generativeai as genai  # The `google.generativeai` module is used to generate titles for the pull requests
from dotenv import load_dotenv  # This function is used to load the environment variables from the .env file
from requests import Response, get  # The `Response` and `get` classes are used to send requests to the GitHub API


def get_pull_requests_from_repo(repo_url: str,
                                headers: dict[str, Any],
                                params: dict[str, Any],
                                number_of_prs: int) -> list[dict[str, Any]]:
    """
    This function gets the pull requests from the given repository.
    :param repo_url:  The url to the repository
    :param headers:  The headers to be used
    :param params:  The parameters to be used
    :param number_of_prs:  The desired number of pull requests to be fetched
    :return:  The list of the pull requests fetched

    Note:  This function is recursive.
           This is not to speed up the runtime of the program, but to handle the case.
           The maximum number of pull requests that can be fetched at once is 100.
           Therefore, the function makes recursive calls to get the rest of the pull requests.
    """

    def raise_exception_from_response(__response: Response) -> None:
        """
        This function raises an exception with the given response.
        :param __response:  The response to be used
        :return:  None
        :raises:  Exception with the given response
        """
        print(__response.text)
        raise Exception(f"Error: {__response.status_code}")

    if number_of_prs <= 0:
        # Primary base case of the recursive function -- less likely to be hit
        # If the number of PRs is less than or equal to 0, return an empty list to terminate the recursion
        return []

    elif number_of_prs <= 100:
        # Secondary base case of the recursive function
        # If the number of PRs is less than or equal to 100, return the PRs
        # No more recursive calls are needed
        params['per_page'] = number_of_prs
        if (response := get(repo_url, headers=headers, params=params)).status_code != 200:
            raise_exception_from_response(response)

        response_json: dict[str, Any] = response.json()
        return response_json['items'] if response_json['total_count'] > 0 else []

    else:
        # Recursive case of the function
        # If the number of PRs is greater than 100, make a recursive call chaining the PRs
        # This is required since the maximum number of PRs that can be fetched at once is 100
        params['per_page'] = 100  # Set the number of PRs to be fetched to 100
        params['page'] = 1 if 'page' not in params else params['page'] + 1  # Set the page number to be fetched
        if (response := get(repo_url, headers=headers, params=params)).status_code != 200:
            raise_exception_from_response(response)

        # Get the PRs from the response and make a recursive call to get the rest of the PRs
        response_json: dict[str, Any] = response.json()
        return ((response_json['items'] if response_json['total_count'] > 0 else [])
                + get_pull_requests_from_repo(repo_url, headers, params, number_of_prs - 100))


def generate_titles_from_diffs(diffs: list[str]) -> list[str] | int:
    """
    This function generates titles for the pull requests from the given diffs.
    :param diffs:  The list of diffs
    :return:  The titles generated for the pull requests, or 1 if an exception occurs indicating an error
    """

    try:
        """
        NOTE:
        The function loads the environment variables from the .env file
        Then, it gets the Google API Key from the environment variables
        Finally, it configures the Google API with the Google API Key loaded

        Create a .env file in the same directory as the program and add the following line:
        ```GOOGLE_API_KEY=YOUR_GOOGLE_API_KEY```
        Replace YOUR_GOOGLE_API_KEY with your actual Google API Key

        GitHub API Token might have been implemented in the same way using the .env file
        However, it is specified to be entered manually during runtime
        Feel free to change this implementation and put the GitHub API Token in the .env file if you like
        If you do so, do NOT forget to change the rest of the program accordingly
        """

        load_dotenv()  # Load the environment variables from the .env file
        google_api_key: str = getenv("GOOGLE_API_KEY")  # Get the Google API Key from the environment variables
        genai.configure(api_key=google_api_key)  # Configure the Google API

        # Create a new generative model
        # 'gemini-pro' is preferred, feel free to change it as you like
        model: genai.GenerativeModel = genai.GenerativeModel('gemini-pro')
    except Exception as exception:
        # If an exception occurs, print the exception and raise an exception
        print(exception)
        return 1

    titles_generated: list[str] = []

    for diff in diffs:  # Generate the titles for each diff
        response: genai.types.generation_types.GenerateContentResponse = model.generate_content(
            "Can you please generate a title for the following GitHub pull request?\n\n" +
            str(diff) + "\n\nNote: Please only return the title and nothing else.",
        )

        titles_generated.append(response.text)  # Append the generated title to the list of titles

    return titles_generated  # Return the list of titles


def prs_to_diffs(prs: list[dict[str, Any]],
                 headers: dict[str, Any]) -> list[dict[str, Any]] | int:
    """
    This function gets the diffs of the given pull requests.
    :param prs:  The list of pull requests
    :param headers:  The headers to be used
    :return:  The list for diffs of the pull requests, or 1 if an exception occurs indicating an error
    """
    diffs: list[dict[str, Any]] = []  # The list of diffs to be returned

    for pr in prs:  # Get the diffs of the pull requests
        diff_url: str = pr['html_url'] + ".diff"  # Construct the URL for the diff of the pull request

        if (response := get(diff_url, headers=headers)).status_code != 200:
            print(response.text)  # Print the response text
            return 1  # Return 1 if an exception occurs indicating an error

        diffs.append(  # Append the diff to the list of diffs
            {
                "diff": {
                    "Additions": [str(line) for line in response.text.split("\n") if line.startswith("+")],
                    "Deletions": [str(line) for line in response.text.split("\n") if line.startswith("-")],
                },
                "title": pr['title']  # Save the original title of the pull request for comparison
            }
        )

    return diffs


def main() -> int:
    """
    This is the main function of the program.
    :return:  Zero if the program is successfully executed, otherwise 1
    """
    try:
        # Get the URL of the repository
        url_link: str = input("Enter the Github Repo URL: ").strip()
        # Parse the URL to get the owner and the repository name
        the_repo: dict[str, str] = {key: val for key, val in zip(["owner", "repo"], url_link.split("/")[-2:])}
        # Construct the URL of the repository for pull requests
        repo_url: str = (f"https://api.github.com/search/issues?q=repo:{the_repo['owner']}/{the_repo['repo']}"
                         f"+is:pr is:merged")  # The URL of the repository for pull requests

        headers: dict[str, str] = {
            "accept": "application/vnd.github+json",  # recommended in the documentation
            "user-agent": "request",  # The user-agent to be used
            "authorization": f"token {input('Enter Your Github Token: ').strip()}"  # The authorization token to be used
        }

        params: dict[str, str] = {
            "sort": "updated",  # The sort of the pull requests to be fetched
            "direction": "desc",  # The direction of the pull requests to be fetched
        }

        number_of_prs: int = int(input("Enter the Number of PRs to generate Title for: ").strip())
    except Exception as exception:
        # If an exception occurs, print the exception and return 1
        print("An exception occurred during the input process:", exception)
        return 1

    # Get the pull requests from the repository
    prs: list[dict[str, Any]] = get_pull_requests_from_repo(repo_url, headers, params, number_of_prs)

    if not prs:  # If no pull requests are found, print a message and return 1
        print("No pull requests merged found on the repository:", url_link)
        return 1

    # Get the diffs of the pull requests
    diffs: list[dict[str, Any]] = prs_to_diffs(prs, headers)

    if diffs == 1:  # If an exception occurs, return 1
        return 1

    # Generate the titles of the diffs
    titles_generated: list[str] = generate_titles_from_diffs(
        [
            diff['diff']
            for diff in diffs
        ]
    )

    if titles_generated == 1:  # If an exception occurs,
        return 1  # return 1

    try:
        # Write the PRs and the generated titles to a CSV file
        with open('PR_Generated_Titles.csv', mode='w', newline='') as csv_file:
            writer: csv.writer = csv.writer(csv_file)
            writer.writerow(["PR #", "Original PR Title", " Generated PR Title"])

            # Write the PRs and the generated titles to the CSV file
            for index in range(len(prs)):
                writer.writerow(
                    [
                        prs[index]['number'],  # The number of the pull request
                        diffs[index]['title'],  # The original title of the pull request
                        titles_generated[index]  # The generated title of the pull request
                    ]
                )
    except Exception as exception:
        # If an exception occurs, print the exception and return 1
        print("An exception occurred during the writing process:", exception)
        return 1

    return 0  # Return 0 if the program is successfully executed


if __name__ == '__main__':
    print(
        "\033[94mThe program is now running.\n"
        "Which may take a while due to the number of requests needed to"
        " be sent to GitHub API and Google Generative API, i.e. Gemini.\n"
        "And, as anticipated, depending on your active connection speed.\n"
        "Please, wait for the program to exit with a legitimate exit code.\n\033[0m"
    )

    code: int = main()
    print(f"\nProcess finished with exit code {code}")
# End of PRTitleGenerator.py
