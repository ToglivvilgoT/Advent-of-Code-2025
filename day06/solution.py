from math import prod
from functools import reduce

with open('day06\example-input.txt', 'r') as f:
    lines = f.readlines()
    nums = lines[:-1]
    ops = lines[-1].split()
    nums = [[int(n) for n in row.split()] for row in nums]
    nums = zip(*nums)

    print(sum((sum(line) if op == '+' else prod(line) for (line, op) in zip(nums, ops))))

def add_number(lst, number):
    number = ''.join(number).strip()
    if number:
        lst[-1].append(int(number))
    else:
        lst.append([])
    return lst

with open('day06\example-input.txt', 'r') as f:
    lines = f.readlines()
    nums = lines[:-1]
    ops = lines[-1].split()
    nums = zip(*nums)
    nums = reduce(add_number, nums, [[]])

    print(sum((sum(line) if op == '+' else prod(line) for (line, op) in zip(nums, ops))))
    