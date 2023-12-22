def main() -> None:

    while True:
        phrase = input("\nEnter a phrase (or 'exit'): ")
        if phrase.lower() == 'exit':
            break
        length = len(phrase)
        for i in range(length // 2 + 1):
            print(' ' * i + phrase[i:length-i])
    print("End of program.\n")


if __name__ == '__main__':
    main()
