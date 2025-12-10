"""Solution for part 2. (see solution.cpp for part 1)."""
from matrix import Matrix
from itertools import product
from fractions import Fraction

Lights = str  # not used for part 2
Button = list[int]
Joltages = list[int]
Machine = tuple[Lights, list[Button], Joltages]


def get_input(file_name: str) -> list[Machine]:
    with open(file_name) as f:
        return [
            (
                line.split()[0][1:-1],
                [[int(num) for num in button[1:-1].split(',')] for button in line.split()[1:-1]],
                [int(num) for num in line.split()[-1][1:-1].split(',')]
            ) for line in f.readlines()
        ]


def button_to_column(button, length):
    """Turns a button like [0, 2] into a button like [1, 0, 1, 0],
    where the output has a "1" at index i if i is in input button.
    """
    return [1 if n in button else 0 for n in range(length)]


def get_button_max_presses(button: list[int], jolts: list[int]):
    """Get the maximum amount of clicks allowed for a button before the joltage would go above target.
    This is used to limit the search space by upper bound."""
    return min(filter(lambda tup: tup[0] != 0, zip(button_to_column(button, len(jolts)), jolts)), key=lambda tup: tup[1])[1] + 1


def solve_machine(machine):
    """Solves one instance of the problem.
    
    The problem can be set up as a set of linear equations.
    Each input button is one column in a matrix.
    The solution to the problem is the vector X,
    where each element in X is the amount of presses for that button.
    The Joltages is the expected result.
    So the problem is to find X such that: MX = J,
    where M is the matrix of buttons and J the joltages.
    This can be solved by gauss elimination.
    The problem that remains is that this gives all solutions,
    we must then find the shortest of them.
    
    In the case of multiple solutions, we have some free parameters that we search through.
    The free parameters are at least 0 (since negative button presses aren't allowed)
    and they are upper bounded by get_button_max_presses().
    We then just have to search through all those possibilities,
    filter out the negative and non-integer solutions,
    and finally return the best one.
    """
    _, buttons, jolts = machine
    # create the matrix of buttons
    matrix = Matrix(zip(*(button_to_column(button, len(jolts)) for button in buttons)))
    # append joltages to create a total matrix
    matrix = matrix.concat(Matrix([n] for n in jolts))
    # solve the matrix using gauss elimination
    matrix.staircase()
    matrix.reduced_staircase()
    # find the free columns. All other columns depend on these columns.
    # They are the columns in the matrix not containing a pivot element.
    free_columns = list(range(len(list(matrix.get_columns()))))
    for row in matrix.get_rows():
        for column, element in enumerate(row):
            if element == 1:
                free_columns.remove(column)
                break
            elif element != 0:
                break

    # remove the last free column, since it is always the joltage column and we don't care about it.
    free_columns = free_columns[:-1]
        
    print(matrix)
    print(free_columns)
    
    search_space = [get_button_max_presses(buttons[n], jolts) for n in free_columns]
    # split up into the case when there is one solution and multiple ones:
    if not search_space:
        print(sum(list(matrix.get_columns())[-1]))
        return sum(list(matrix.get_columns())[-1])
    else:
        matrix = matrix.get_rows()  # convert to a list[list[Fraction]]
        best_score = 999999999999
        best_values = []  # used for debug
        # search through the entire search space:
        for candidate in product(*(range(n) for n in search_space)):
            candidate_score = 0  # amount of button presses for this candidate
            row = 0
            values = []  # used for debug
            for column in range(len(matrix[0]) - 1):
                if column in free_columns:
                    # if the current column is a free column.
                    # look up how many times it is pressed from the current candidate
                    candidate_score += candidate[free_columns.index(column)]
                    values.append(candidate[free_columns.index(column)])  # for debug
                else:
                    # if the column is not free, it depends on the other free columns.
                    presses: Fraction = matrix[row][-1] - sum([candidate[free_columns.index(x)] * matrix[row][x] for x in free_columns])
                    # filter out non integer and negative solutions
                    if presses.numerator % presses.denominator != 0 or presses < 0:
                        break 
                    candidate_score += presses
                    row += 1
                    values.append(presses)
            else:
                if best_score > candidate_score:
                    best_values = values
                best_score = min(best_score, candidate_score)
        print(best_values)
        print(best_score)
        return best_score


def solve(inp):
    """Solves part 2."""
    return sum((solve_machine(machine) for machine in inp))


if __name__ == '__main__':
    print(solve(get_input('input.txt')))