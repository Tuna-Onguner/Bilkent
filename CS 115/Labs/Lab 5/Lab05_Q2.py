def sum_t(tup):
    return tuple(filter(lambda x: type(x) in (float, int), tup))


tup = ((3, 5), 5, False, 2.5, 'cs115', [1, 2, 3], 7.8)
print(f'Original tuple:\n{tup}')

tup = sum_t(tup)
print(f'New tuple:\n{tup}\nSum of new tuple is {sum(tup)}')
