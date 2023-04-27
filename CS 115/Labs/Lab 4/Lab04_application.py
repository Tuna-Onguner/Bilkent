from Lab04_module import *


def main() -> None:
    filename = 'input.txt'
    
    while True:
        choice = display_menu()

        if choice == 'd':
            break

        domain = input('Enter domain to search: ').lower().strip()

        if choice == 'a':
            count = get_email_by_domain(domain, filename)
            print(f'{count} users exist in {domain} domain' if count > 0 else 'No Users Found!')

        elif choice == 'b':
            display_users_by_domain(domain)

        elif choice == 'c':
            display_users_having_numeric(domain)

        print()


if __name__ == '__main__':
    main()
