def convert_char(sentence) -> str:
    result = ''
    for char in sentence:
        if char.islower():
            result += chr(ord(char) - 3)
        elif char.isupper():
            result += chr(ord(char) + 3)
        elif char.isalnum():
            result += char
        else:
            result += '#'
    return result


if __name__ == '__main__':
    phrase = input('Enter a phrase for conversion: ')
    print(f'Text you entered: {phrase}\nAfter conversion: {convert_char(phrase)}')
