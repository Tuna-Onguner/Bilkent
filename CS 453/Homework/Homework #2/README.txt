Bilkent University
Department of Computer Engineering


CS 453 Application Lifecycle Management
Homework #2: Experimenting with Gemini API


Author: Deniz Tuna Onguner
Bilkent ID: 22001788
Section: 1



PROJECT TITLE
    Pull Request Title Generator



CONTENTS
    1. Installation
    2. Gemini API Configuration
    3. Usage
    4. Requirements



INSTALLATION
    Make sure you have the required packages installed by running the following command:
        pip install -r requirements.txt
    or
        pip3 install -r requirements.txt

    This command will install all the necessary dependencies listed in the requirements.txt file unless they are already installed.



GEMINI API CONFIGURATION
    To be able to run the code, you need to have a valid Google Generative AI, or Gemini, API key. You can get your API key from the following link:
        https://aistudio.google.com/app/apikey

    After obtaining your API key, you need to create a file named ".env" in the root directory of the project,
    or you can simply use the provided ".env" file instead. In the ".env" file, you need to add the following line:
        GOOGLE_API_KEY=YOUR_API_KEY

    There already might be a value assigned to the GOOGLE_API_KEY variable in the ".env" file provided.
    If there is, replace it with your very own API key. The provided one will not be valid once the homework is submitted.



USAGE
    Run the code by executing the following command in your terminal:
        python PRTitleGenerator.py
    or
        python3 PRTitleGenerator.py



REQUIREMENTS
    - Python 3.11 or higher
    - Other dependencies are listed in the file `requirements.txt`
