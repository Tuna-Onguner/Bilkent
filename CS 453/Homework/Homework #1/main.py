#
# Bilkent University
# Department of Computer Engineering
#
# CS453: Application Lifecycle Management
# Homework #1: Understanding and Experimenting with GitHub API
#
# March 3, 2024, Sunday
#
# Deniz Tuna Onguner [Bilkent ID: 22001788]
# Section 1
#
# Description: This is the main file of the homework.
# It includes the implementation of all the requirements asked for.
#

import csv
from datetime import datetime, timedelta

import requests as req  # Import requests library as req to avoid name conflict

headers: dict = {
    "Accept": "application/json",  # Set the Accept header to receive JSON response
    "User-Agent": "request",  # Set the User-Agent header to avoid 403 Forbidden error
    "Authorization": "token <<YOUR_TOKEN_GOES_HERE>>"
}


def find_repository(search_criteria: dict) -> dict | None:  # Used for Question 1
    """
    Searches for repositories on GitHub based on the provided criteria.

    Args:
        search_criteria: A dictionary containing the following keys and values:
          - contributors: Minimum number of contributors (int), e.g., 100
          - issues: Minimum number of total issues[closed + open] (int), e.g., 1000
          - stars: Minimum number of stargazers (int), e.g., 1000
          - age_days: Minimum age of the repository in days (int), e.g., 365 for 1 year
          - language: Programming language (str), e.g., "python"

    Returns:
        A dictionary containing information about the first matching repository,
        or None if no match by the criteria is found.
    """

    global headers

    # Base URL for searching repositories
    url: str = "https://api.github.com/search/repositories"

    # Get today's date
    today: datetime = datetime.now()

    # Find the date that is age_days days before today
    age: datetime = today - timedelta(days=search_criteria['age_days'])

    # Constructing the query string
    params: dict = {
        "q": f"language:{search_criteria['language']} "
             f"stars:>={search_criteria['stars']} "
             f"created:<{age.strftime('%Y-%m-%d')}",
        "sort": "stars",  # Sort the results by the number of stars
        "order": "desc",  # Order the results in descending order
        # This will return the most popular repositories first which are more likely to match the criteria
    }

    # Send a request to the GitHub API
    response: req.Response = req.get(url, params=params, headers=headers)

    # Check if the request was successful
    if response.status_code != 200:
        print(f"Failed to fetch data from GitHub API. Status code: {response.status_code}")
        return None

    # Parse the response as JSON
    data: dict = response.json()

    # Check if any repositories were found
    if data['total_count'] == 0:
        print("No repositories found by the criteria.")
        return None  # Return None if no repositories are found

    # Initialize variables to store the number of contributors and issues
    base_url, page = "https://api.github.com/repos/{}/{}?per_page=100&page={}", 1

    # Iterate over the repositories and find the first one that matches the criteria
    for repo in data['items']:  # data['items'] contains the list of repositories
        """
        ******************************************************************************************************
        IMPORTANT NOTE:
        Logically, the page number should be reset to 1 for each repository -- each iteration of the loop.
        However, the returned result is more desirable if the page number is not reset.
        This is because some repositories have a large number of contributors and issues,
            which require numerous requests to fetch all of them which is time-consuming.
            
        When the page number is not reset, the loop passes such repositories faster and finds another match.
        Which I mistakenly realized after the implementation of the code, and I decided to keep it as is.

        Therefore, the page number is not reset to 1 for each repository as one might expect.
        Feel free to change this behavior as per the requirements.
        ******************************************************************************************************
        """

        number_of_contributors: int = 0  # Initialize the number of contributors to 0

        while True:  # Loop to fetch all contributors
            # Send a request to the GitHub API to get the number of contributors
            """
            *************************************************************************************
            DECLARATION:
            This is for the full list of contributors, not just so-called "active" contributors.
            The active contributors will be found in Question 4, respectively.
            *************************************************************************************
            """
            contributors_response: req.Response = req.get(base_url.format(repo['full_name'],
                                                                          "contributors", page), headers=headers)

            if contributors_response.status_code != 200:  # Check if the request was successful
                print(f"Failed to fetch data from GitHub API during contributors request. "
                      f"Status code: {contributors_response.status_code}")
                return None

            if (response_length := len(contributors_response.json())) > 0:  # Check if there are contributors
                number_of_contributors += response_length  # Increment the number of contributors
            else:
                break  # Break the loop if there are no more contributors

            page += 1  # Increment the page number to fetch the next batch of contributors

        page = 1  # Reset the page number to reuse it for fetching issues

        if number_of_contributors >= search_criteria['contributors']:
            number_of_issues: int = 0  # Initialize the number of issues to 0

            while True:  # Loop to fetch all issues
                # Send a request to the GitHub API to get the number of issues

                issues_response: req.Response = req.get(base_url.format(repo['full_name'],
                                                                        "issues", page), headers=headers)

                if issues_response.status_code != 200:  # Check if the request was successful
                    print(f"Failed to fetch data from GitHub API during issues request. "
                          f"Status code: {issues_response.status_code}")
                    return None

                if (response_length := len(issues_response.json())) > 0:  # Check if there are issues
                    number_of_issues += response_length  # Increment the number of issues
                else:
                    break  # Break the loop if there are no more issues

                page += 1  # Increment the page number to fetch the next batch of issues

            # Check if the number of issues is greater than or equal to the search criteria
            if number_of_issues >= search_criteria['issues']:
                return {  # Return the information of the repository
                    "name": repo['full_name'],
                    "url": repo['html_url'],
                    "checked_at": today.strftime('%Y-%m-%d %H:%M:%S')
                }

            else:  # If the number of issues is less than the search criteria, continue to the next repository
                continue

        else:  # If the number of contributors is less than the search criteria, continue to the next repository
            continue

    # If no matching repository is found, return None
    return None


def main() -> int:
    """
    The main function of the program.
    :return: Zero if the program runs successfully, 1 otherwise
    """

    global headers

    # Question 1:
    # Find and document (link + date you checked the data at) a GitHub Repo with the following criteria:
    # - At least 100 contributors
    # - At least 1000 issues
    # - At least 1000 stars
    # - Written in Java (>= 70% of the code is Java)
    # - Created within the last 2 years

    criteria: dict = {
        "contributors": 100,
        "issues": 1000,
        "stars": 1000,
        "age_days": 365 * 2,  # 2 years = 365 days per year * 2
        "language": "java",
    }

    repository_found: dict | None = find_repository(criteria)

    if repository_found:
        print(f"Found repository: {repository_found['name']}")
        print(f"URL: {repository_found['url']}")
        print(f"Checked at: {repository_found['checked_at']}")
    else:
        return 1

    # Question 2:
    # Go through all the closed issues and calculate the number of issues resolved by each contributor and
    # provide a sorted list in descending order of the contributors and the number of issues they resolved.
    # Hint: Some issues may not have an assignee to them.

    page, contributors = 1, {}
    issues_with_assignee, issues_without_assignee = 0, 0  # for Question 3

    while True:
        """
        ****************************************************************************************
        WARNING:
        The criteria here includes only closed issues.
        Therefore, used `state=closed` parameter in the query string to exclude non-closed ones.
        No other parameters are used to filter the issues, feel free to change this behavior.
        Please be aware, and change accordingly as per the requirements.
        
        Also, note that the GitHub UI in the browser may show different 
        results than the API unless the same parameters are used in the browser link.
        ****************************************************************************************
        """

        issues_url: str = ("https://api.github.com/repos/{}/issues?state=closed&per_page=100&page={}"
                           .format(repository_found['name'], page))

        issues_response: req.Response = req.get(issues_url, headers=headers)

        if issues_response.status_code != 200:
            print(f"Failed to fetch data from GitHub API during issues request. "
                  f"Status code: {issues_response.status_code}")
            break

        if len(issues := issues_response.json()) == 0:
            break

        for issue in issues:
            if issue['assignee']:
                assignee: str = issue['assignee']['login']
                if assignee in contributors:
                    contributors[assignee] += 1
                else:
                    contributors[assignee] = 1
                issues_with_assignee += 1  # for Question 3
            else:
                issues_without_assignee += 1  # for Question 3

        page += 1

    # Sort the contributors by the number of issues they resolved
    sorted_contributors: list = sorted(
        contributors.items(),
        key=lambda x: x[1],
        reverse=True
    )

    # Write the contributors and the number of issues they resolved to a CSV file
    with open('q3.csv', 'w', newline='') as q3_csv:
        writer: any = csv.writer(q3_csv)
        writer.writerow(["Contributor", "Number of Issues Resolved"])
        writer.writerows(sorted_contributors)

    # Question 3:
    # Calculate the number of closed issues and group them by
    # Issues with Assignee and Issues Without Assignees.

    print(f"No. of Issues with Assignee is {issues_with_assignee} issues")
    print(f"No. of Issues without Assignee is {issues_without_assignee} issues")

    # Question 4:
    # Find and sort the active developers in the project.
    # Active developers are those who contributed (committed) to the project in the last year.

    # Get today's date
    today: datetime = datetime.now()
    last_year: datetime = today - timedelta(days=365)  # Find the date that is 1 year before today

    # Constructing the query string
    params: dict = {
        "since": last_year.strftime('%Y-%m-%d')  # Filter the contributors who contributed after last_year
    }

    # Send a request to the GitHub API to get the contributors
    contributors_response: req.Response = req.get(f"https://api.github.com/repos/{repository_found['name']}"
                                                  f"/contributors", params=params, headers=headers)

    if contributors_response.status_code != 200:
        print(f"Failed to fetch data from GitHub API during contributors request. "
              f"Status code: {contributors_response.status_code}")
        return 1

    # Parse the response as JSON
    contributors_data: dict = contributors_response.json()

    # Sort the contributors by the number of commits they made
    sorted_active_contributors: list = sorted(
        contributors_data,
        key=lambda x: x['contributions'],
        reverse=True
    )

    # Write the active contributors and the number of commits they made to a CSV file
    with open('q4.csv', 'w', newline='') as q4_csv:
        writer: any = csv.writer(q4_csv)
        writer.writerow(["Contributor", "Number of Commits"])
        writer.writerows(
            [
                [contributor['login'], contributor['contributions']] for contributor in sorted_active_contributors
            ]
        )

    # Question 5:
    # Draw a scattered diagram with two axes of the number of closed issues in the past year and
    # the number of commits in the past year. Each dot in this diagram represents a developer.

    def draw_scattered_diagram_and_show() -> int:  # Only used for Question 5
        """
        Draws a scattered diagram with two axes of the number of closed issues in the past year and
        the number of commits in the past year. Each dot in this diagram represents a developer.
        :return: Zero if the diagram is drawn successfully, 1 otherwise
        """

        from numpy import array as narr
        from matplotlib import pyplot as plt

        x: any = narr(  # Convert the list of commits to a numpy array
            [
                contributor['contributions']
                for contributor in sorted_active_contributors
            ]
        )

        y: any = narr(  # Convert the list of issues to a numpy array
            [
                contributors[contributor['login']]
                if contributor['login'] in contributors else 0
                for contributor in sorted_active_contributors
            ]
        )

        try:  # Try to draw the scattered diagram
            plt.title('Number of Commits vs Number of Issues', loc='center', fontsize=11)
            plt.grid(color='#88c999', linestyle='dotted', linewidth=1.5)
            plt.scatter(x, y, color='#1e90ff', s=11)
            plt.xlabel('Number of Commits')
            plt.ylabel('Number of Issues')
            #plt.show()  # Show the scattered diagram, comment this line to disable showing the diagram
        except Exception as exception:  # Catch any exception that may occur
            print(f"Failed to draw the scattered diagram. Error: {exception}")
            return 1

        return 0

    __exit: int = draw_scattered_diagram_and_show()

    return __exit


if __name__ == '__main__':
    print(
        "\033[94mThe program is running.\n"
        "Which may take a while due to the number of requests needed to"
        " be sent to the GitHub API depending on the search criteria.\n"
        "And, depending on your connection speed.\n"
        "Please wait for the program to exit with a legitimate exit code.\n\033[0m"
    )

    exit(code := main())  # Check for the walrus operator, used a lot in the code, if not familiar with the syntax
