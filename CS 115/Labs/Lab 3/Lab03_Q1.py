def swap(sentence) -> str:
    words = sentence.split()
    if len(words) <= 1:
        return sentence
    else:
        words[0], words[-1] = words[-1], words[0]
        return ' '.join(words)


if __name__ == '__main__':
    while True:
        phrase = input('Enter a sentence: ')
        if phrase.lower() == 'exit':
            break
        print(swap(phrase), '\n')
