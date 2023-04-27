from random import randint


def fill_list(n):
    return [randint(2, 50) for _ in range(n)]


def eliminate_values(lst):
    uniques = set(lst)
    lst.clear()
    lst.extend(uniques)
    del uniques


lst = fill_list(int(input('Enter the number of elements: ')))
print(f'Original list:\n{lst}')

eliminate_values(lst)
print(f'List with multiples removed:\n{lst}')
