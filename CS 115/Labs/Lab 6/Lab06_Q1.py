def values_before(arr: list, search_value: int) -> list:
    counts: list = []

    for col in range(len(arr[0])):
        count: int = 0
        found: bool = False

        for row in range(len(arr)):
            if arr[row][col] == search_value:
                found = True
                break

            if not found:
                count += 1

        if not found:
            count = -1

        counts.append(count)

    return counts


def main() -> None:
    arr: list = [
        [2, 4, 8, 6],
        [6, 2, 2, 5],
        [8, 7, 7, 9],
        [4, 6, 2, 3],
        [8, 9, 3, 8],
    ]

    search_value: int = int(input('Enter the number to search: '))

    counts: list = values_before(arr, search_value)

    for i, count in enumerate(counts):
        if count < 0:
            print(f'There is no {search_value} in column {i}')
        else:
            print(f'There are {count} values before {search_value} in column {i}')


if __name__ == '__main__':
    main()
