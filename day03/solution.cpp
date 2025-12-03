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

// a greedy algorithm that loops over each battery row.
// For each row, the biggest digit is found repeatedly until enough (batteriesPerRow) are found.
// Each consecutive battery search only looks to the right of the previous found battery.
unsigned long long solve(vector<vector<char>> inp, int batteriesPerRow) {
    unsigned long long totalJoltage = 0;
    unsigned long long prevJoltage = 0;  // ops forgot to remove this line, used it for debugging :)
    for (auto row : inp) {
        vector<char>::iterator currentBattery = row.begin();
        for (int batteriesLeft = batteriesPerRow; batteriesLeft; --batteriesLeft) {
            currentBattery = getMax(currentBattery, row.end() - batteriesLeft + 1);
            totalJoltage += (*currentBattery - '0') * intPow(10, batteriesLeft - 1);
            ++currentBattery;
        }
    }
    return totalJoltage;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp, 12);
    cout << result << endl;
}