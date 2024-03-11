Bilkent University
Department of Computer Engineering


CS 453 Application Lifecycle Management
Homework #1: Understanding and Experimenting with GitHub API


Author: Deniz Tuna Onguner
Bilkent ID: 22001788
Section: 1


CONTENTS
    1. Installation
    2. Token
    3. Usage
    4. Requirements


INSTALLATION
    Make sure you have the required packages installed by running the following command:
        pip install -r requirements.txt

    This command will install all the necessary dependencies listed in the requirements.txt file unless they are already installed.


TOKEN
    To run the code, you need to have a GitHub token. You can create a token by following the instructions in the link below:
        https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token

    Then, place your token in the header of the main.py file on line 25 as follows:
        headers = {
            "Accept": "application/json",
            "User-Agent": "request",
            "Authorization": "token <<YOUR_TOKEN_GOES_HERE>>"
        }

    Replace `<<YOUR_TOKEN_GOES_HERE>>` with your actual token.
    Usage of a token is required to avoid running into the rate limit of the GitHub API.


USAGE
    Run the code by executing the following command in your terminal:
        python main.py
    or
        python3 main.py


REQUIREMENTS
    - Python 3.10 or higher
    - Other dependencies listed in requirements.txt
