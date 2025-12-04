#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;
using i32 = int;
using i64 = long long int;
using u32 = unsigned;
using u64 = unsigned long long;

#define v(x) vector<x>
#define m(x) vector<vector<x>>
#define iter(x) x::iterator

// read input as a matrix of chars
m(char) getInput(string fileName) {
    ifstream f(fileName);
    m(char) inp;
    string line;
    while (f >> line) {
        v(char) row;
        for (char c : line) {
            row.push_back(c);
        }
        inp.push_back(row);
    }
    return inp;
}

// helper function to check if a pair of coords are in bounds of a matrix
bool inBounds(int x, int y, int minX, int minY, int maxX, int maxY) {
    return minX <= x && x < maxX && minY <= y && y < maxY;
}

// complete search through all coords and check amount of neighbors.
// For part 2, repeat the above step until no more paper rolls are removed this iteration.
u64 solve(m(char) inp, bool remove = true) {  // remove should be true for part 2, false for part 1
    u32 total = 0;
    bool removedThisIteration;
    do {  // every imperative programming language should have a do-while loop, they are goated.
        removedThisIteration = false;
        for (i32 y = 0; y < inp.size(); ++y) {
            for (i32 x = 0; x < inp[y].size(); ++x) {
                if (inp[y][x] != '@') { continue; }
                u32 neighbors = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int currX = x + dx;
                        int currY = y + dy;
                        if (inBounds(currX, currY, 0, 0, inp.size(), inp[y].size()) && inp[currY][currX] == '@') {
                            neighbors++;
                        }
                    }
                }
                // also allow 4 here, since the algorithm considers a coord to be a neighbour of it self.
                // There for the middle roll in the following example would have 4 neighbors in my algoritm:
                // @..
                // @@@
                // ...
                // but the expected would be 3
                if (neighbors <= 4) {
                    total++;
                    if (remove) {
                        inp[y][x] = 'x';
                        removedThisIteration = true;
                    }
                }
            }
        }
    } while (remove && removedThisIteration);
    return total;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp, false);
    auto result2 = solve(inp, true);
    cout << "part 1: " << result << endl << "part 2: " << result2 << endl;
}