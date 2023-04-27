import re


def display_menu() -> chr:
    while True:
        print('Menu:\n'
              '(a) Find email by domain\n'
              '(b) Display users by domain\n'
              '(c) Display numeric userNames by domain\n'
              '(d) EXIT')

        choice = input('Enter choice: ').lower().strip()

        if choice in ['a', 'b', 'c', 'd']:
            return choice
        else:
            print('Invalid Choice!\n')


def get_email_by_domain(domain, filename) -> int:
    pattern = re.compile(fr'\b[\w.-]+@{domain}\b')
    users = set()
    count = 0

    try:
        with open(filename, 'r') as input_file:
            for line in input_file:
                matches = pattern.findall(line)
                for match in matches:
                    i = 0
                    username = ''
                    while i < len(match) and match[i] != '@':
                        username += match[i]
                        i += 1
                    users.add(username)
                    count += 1
        if count > 0:
            with open(f'{domain}.txt', 'w') as output_file:
                for user in users:
                    output_file.write(f'{user}\n')
    except FileNotFoundError:
        print('File Not Found')

    return count


def display_users_by_domain(domain) -> None:
    pattern = re.compile(fr'\b([\w.-]+)\b')
    users = set()

    try:
        with open(f'{domain}.txt', 'r') as input_file:
            for line in input_file:
                matches = pattern.findall(line)
                for match in matches:
                    users.add(match)
        for user in sorted(users):
            print(user)
    except FileNotFoundError:
        print('File Not Found')


def display_users_having_numeric(domain) -> None:
    pattern = re.compile(fr'\b([\w.-]+)\b')
    users = set()

    try:
        with open(f'{domain}.txt', 'r') as input_file:
            for line in input_file:
                matches = pattern.findall(line)
                for match in matches:
                    for i in match:
                        if i.isdigit():
                            users.add(match)
                            break
        for user in sorted(users):
            print(user)
    except FileNotFoundError:
        print('File Not Found')
