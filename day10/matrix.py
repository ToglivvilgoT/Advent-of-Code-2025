"""Library for matrix and different operations on it"""
# I created this lib before aoc and just copied it in here for solving part 2.
# That is the main reason I switch away from c++,
# did not feel like re-implementing matrix solver for c++, it's a bit tedious.
from fractions import Fraction
F = Fraction
from typing import TypeVar, Generic

T = TypeVar('T')

class Row(Generic[T]):
    row: list[T]

    def __init__(self, elements: list[T]):
        self.row = [element for element in elements]

    def __mul__(self, scalar: T, from_left = True):
        if from_left:
            return Row([scalar * element for element in self.row])
        else:
            return Row([element * scalar for element in self.row])

    def __add__(self, other: 'Row[T]'):
        return Row(map(lambda tup: tup[0] + tup[1], zip(self.row, other.row)))

    def __iter__(self):
        for t in self.row:
            yield t
    
    def __repr__(self):
        return 'Row: ' + str(self.row)

class Matrix(Generic[T]):
    rows: list[Row[T]]

    def __init__(self, elements: list[list[T]]):
        self.rows = [Row(row) for row in elements]

    def multiply_matrix(self, other: 'Matrix[T]') -> 'Matrix[T]':
        m, n = len(self.rows), len(self.rows[0].row)
        n_b, p = len(other.rows), len(other.rows[0].row)
        if n != n_b:
            raise ValueError("Incompatible dimensions for matrix multiplication")

        result: Matrix[Generic[T]] = [[0 for _ in range(p)] for _ in range(m)]

        for i in range(m):
            for j in range(p):
                s = 0
                for k in range(n):
                    s += self.rows[i].row[k] * other.rows[k].row[j]
                result[i][j] = s

        return Matrix(result)

    def multiply_row(self, row: int, k: T):
        self.rows[row] *= k

    def __mul__(self, other: 'Matrix[T] | tuple[int, T]'):
        if isinstance(other, Matrix):
            return self.multiply_matrix(other)
        else:
            self.multiply_row(*other)

    def __add__(self, other: 'Matrix[T]'):
        return Matrix(((self.rows[y].row[x] + other.rows[y].row[x] for x in range(len(self.rows[y].row))) for y in range(len(self.rows))))

    def eliminate_row(self, row_to: int, row_from: int, k: Fraction):
        self.rows[row_to] += self.rows[row_from] * k

    def staircase(self):
        x = y = 0
        while y < len(self.rows) and x < len(self.rows[0].row):
            if self.rows[y].row[x] == 0:
                for y2 in range(y + 1, len(self.rows)):
                    if self.rows[y2].row[x] != 0:
                        self.eliminate_row(y, y2, 1)
                        break
                else:
                    x += 1
                    continue
                
            self.multiply_row(y, F(1, self.rows[y].row[x]))

            for y2 in range(y + 1, len(self.rows)):
                self.eliminate_row(y2, y, -self.rows[y2].row[x])

            x += 1
            y += 1

    def reduced_staircase(self):
        x = y = 0
        while y < len(self.rows) and x < len(self.rows[0].row):
            if self.rows[y].row[x] == 0:
                x += 1
                continue
                
            for y2 in range(y):
                self.eliminate_row(y2, y, -self.rows[y2].row[x])

            x += 1
            y += 1

    def transpose(self):
        return Matrix(zip(*self.rows))

    def concat(self, other: 'Matrix[T]'):
        return Matrix(map(lambda tup: tup[0].row + tup[1].row, zip(self.rows, other.rows)))

    def least_squares(self, other: 'Matrix'):
        self_t = self.transpose()
        left = self_t * self
        right = self_t * other
        matrix = left.concat(right)
        matrix.staircase()
        matrix.reduced_staircase()
        return matrix

    def get_rows(self):
        return [row.row for row in self.rows]
    
    def get_columns(self):
        return zip(*self.get_rows())

    def __repr__(self):
        matrix = [[str(n) for n in row.row] for row in self.rows]
        biggest = 0
        for row in matrix:
            biggest = max(biggest, max(map(len, row)))
        
        for row in matrix:
            for i in range(len(row)):
                row[i] = " " * (biggest - len(row[i])) + row[i]

        name = " " + "_" * ((biggest + 1) * len(row) - 1) + '\n'
        for row in matrix:
            name += '|'
            name += ' '.join(row)
            name += '|\n'
        name += " " + "\u203E" * ((biggest + 1) * len(row) - 1)

        return name

if __name__ == '__main__':
    f = Matrix((
        (1, 0, 0, 2),
        (1, 1, 1, -1),
        (1, 1, -1, -1),
        (0, 1, 0, 2),
    ))
    i = Matrix((
        (1, 0, 0, 0),
        (0, 1, 0, 0),
        (0, 0, 1, 0),
        (0, 0, 0, 1),
    ))
    total = f.concat(i)
    total.staircase()
    total.reduced_staircase()
    total = Matrix([row.row[4:] for row in total.rows])
    print(total)
    u = Matrix((
        (1, 0, 0, 0),
        (1, 1, 0, 0),
        (1, 1, 0, 0),
        (0, 1, 0, 0),
    )) * total
    uc = Matrix((
        (0, 0, 0, 2),
        (0, 0, 1, -1),
        (0, 0, -1, -1),
        (0, 0, 0, 2),
    )) * total
    print(u * uc)
    print(u + uc)

