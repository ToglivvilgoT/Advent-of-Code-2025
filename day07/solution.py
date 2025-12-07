from functools import cache


Point = tuple[int, int]


class Node:
    def paths(self) -> int: pass


class BinaryNode:
    left_child: Node
    right_child: Node

    def __init__(self, left_child: Node, right_child: Node):
        self.left_child = left_child
        self.right_child = right_child

    @cache
    def paths(self) -> int:
        return self.left_child.paths() + self.right_child.paths()


class EmptyNode:
    def paths(self): return 1


class Tree:
    nodes: dict[Point, Node] = {}

    def add_node(self, point: Point, manifold: list[str]):
        if point in self.nodes:
            return self.nodes[point]

        x, y = point
        while y < len(manifold) and manifold[y][x] != '^':
            y += 1
        
        if y >= len(manifold):
            node = EmptyNode()

        else:
            left = self.add_node((x - 1, y), manifold)
            right = self.add_node((x + 1, y), manifold)
            node = BinaryNode(left, right)

        self.nodes[point] = node
        return node

    def paths(self, start: Point):
        return self.nodes[start].paths()


def solve():
    with open('input.txt') as f:
        lines = f.readlines()
        start = (lines[0].index('S'), 0)
        tree = Tree()
        tree.add_node(start, lines)
        print(tree.paths(start))


if __name__ == '__main__':
    solve()
