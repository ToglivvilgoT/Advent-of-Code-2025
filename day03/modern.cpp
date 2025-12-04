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
#include <ranges>
#include <numeric>

using namespace std;
using u64 = unsigned long long;

// format the input as a matrix of digits
vector<vector<char>> getInput(string fileName) {
    ifstream f(fileName);
    string srow;
    vector<vector<char>> inp;
    vector<char> row;
    while (f >> srow) {
        for (char c : srow) {
            row.push_back(c);
        }
        inp.push_back(row);
        row = vector<char>();
    }
    return inp;
}

// find and return the biggest digit from start to end;
vector<char>::iterator getMax(vector<char>::iterator start, vector<char>::iterator end) {
    char maxVal = 0;
    vector<char>::iterator maxPoint;
    for (;start != end; ++start) {
        if (*start > maxVal) {
            maxVal = *start;
            maxPoint = start;
        }
    }
    return maxPoint;
}

// "unsigned long long" is my new favorite type to write in a contest, it's so fast.
unsigned long long intPow(unsigned long long b, unsigned long long e) {
    if (e == 0) {
        return 1;
    } else {
        return b * intPow(b, e - 1);
    }
}

void solveRow(vector<char>::iterator start, vector<char>::iterator end, vector<char>::iterator stop, string& output) {
    auto batteryPos = getMax(start, end);
    output.push_back(*batteryPos);
    if (end == stop) {
        return;
    }
    solveRow(batteryPos + 1, end + 1, stop, output);
}

// a greedy algorithm that loops over each battery row.
// For each row, the biggest digit is found repeatedly until enough (batteriesPerRow) are found.
// Each consecutive battery search only looks to the right of the previous found battery.
unsigned long long solve(vector<vector<char>> inp, int batteriesPerRow) {
    return ranges::fold_left(
        inp
      | views::transform([batteriesPerRow](vector<char> row) {
            string joltage;
            solveRow(row.begin(), row.end() - batteriesPerRow + 1, row.end(), joltage);
            return ranges::fold_left(joltage, static_cast<u64>(0), [](u64 acum, char joltage){
                return acum * 10 + joltage - '0';
            });
        }),
        static_cast<u64>(0),
        plus{}
    );
}

void main() {
    auto inp = getInput(1 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp, 2);
    auto result2 = solve(inp, 12);
    cout << "part 1: " << result << "\npart 2: " << result2 << endl;
}