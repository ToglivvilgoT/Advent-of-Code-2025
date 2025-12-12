#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <ranges>
#include <algorithm>
#include <sstream>
#include <queue>

#include "lib.cpp"

using namespace std;
using i32 = int;
using i64 = long long int;
using u32 = unsigned;
using u64 = unsigned long long;

#define v(x) vector<x>
#define m(x) vector<vector<x>>
#define p(x,y) pair<x, y>

using Shape = array<array<bool, 3>, 3>;
using Size = p(u64,u64);
using TestCase = p(Size, v(u64));
using inpType = p(v(Shape), v(TestCase));

inpType getInput(string fileName) {
    v(Shape) shapes;
    v(TestCase) testCases;
    string line;
    ifstream f(fileName);
    while (getline(f, line)) {
        if (line.size() <= 3) {
            Shape shape;
            for (size_t y = 0; y < 3; ++y) {
                for (size_t x = 0; x < 3; ++x) {
                    char c;
                    f >> c;
                    shape[y][x] = c == '#';
                }
            }
            shapes.push_back(shape);
            getline(f, line);
            getline(f, line);
        }
        else {
            stringstream s(line);
            u64 width;
            u64 height;
            char c;
            s >> width >> c >> height >> c;
            v(u64) testCaseShapes;
            u64 shapeAmount;
            while (s >> shapeAmount) {
                testCaseShapes.push_back(shapeAmount);
            }
            testCases.push_back({{width, height}, testCaseShapes});
        }
    }
    return {shapes, testCases};
}

/**
 * Returns the area of a shape i.e. number of '#'
 */
u64 getArea(Shape const& shape) {
    u64 area = 0;
    for (auto row : shape) {
        for (bool val : row) {
            area += val;
        }
    }
    return area;
}

/**
 * Calculates the total area of all shapes,
 * if it is larger than allowed area, return 0, else return 1.
 */
u64 solveCase(v(Shape) const& shapes, TestCase const& test) {
    auto [size, shapeAmounts] = test;
    u64 answer = 0;
    for (size_t i = 0; i < shapeAmounts.size(); ++i) {
        answer += getArea(shapes[i]) * shapeAmounts[i];
    }
    return size.first * size.second > answer;
}

/**
 * Iterates over all testcases and sums up the ones that are completable.
 */
u64 solve(inpType inp) {
    auto [shapes, testCases] = inp;
    u64 total = 0;
    for (auto testCase : testCases) {
        total += solveCase(shapes, testCase);
    }
    return total;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    cout << result << endl;
}