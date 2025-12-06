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

using namespace std;
using i32 = int;
using i64 = long long int;
using u32 = unsigned;
using u64 = unsigned long long;

#define v(x) vector<x>
#define m(x) vector<vector<x>>
#define iter(x) x::iterator

using inpType = pair<m(u64), v(bool)>;
inpType getInput(string fileName) {
    ifstream f(fileName);
    m(u64) numbers;
    v(u64) row;
    v(bool) isAddition;
    string currentNumber;
    for (int character = f.get(); character != -1; character = f.get()) {
        if ('0' <= character && character <= '9') {
            currentNumber.push_back(character);
        }
        else if (character == ' ' && currentNumber != "") {
            row.push_back(stoi(currentNumber));
            currentNumber.clear();
        }
        else if (character == '+' or character == '*') {
            isAddition.push_back(character == '+');
        }
        else if (character == '\n') {
            if (currentNumber != "") {
                row.push_back(stoi(currentNumber));
                currentNumber.clear();
            }
            numbers.push_back(row);
            row.clear();
        }
    }
    return {numbers, isAddition};
}

inpType getInput2(string const& fileName) {
    ifstream f(fileName);
    v(string) numbersBuilder;
    v(bool) isAddition;
    size_t i = 0;
    for (int character = f.get(); character != -1; character = f.get()) {
        if ('0' <= character && character <= '9') {
            if (i >= numbersBuilder.size()) {
                numbersBuilder.push_back("");
            }
            numbersBuilder[i].push_back(character);
            ++i;
        }
        else if (character == ' ') {
            if (i >= numbersBuilder.size()) {
                numbersBuilder.push_back("");
            }
            ++i;
        }
        else if (character == '+' or character == '*') {
            isAddition.push_back(character == '+');
        }
        else if (character == '\n') {
            i = 0;
        }
    }
    m(u64) numbers;
    numbers.push_back(v(u64)());
    for (string numberString : numbersBuilder) {
        if (numberString == "") {
            numbers.push_back(v(u64)());
        }
        else {
            numbers.back().push_back(stoi(numberString));
        }
    }
    return {numbers, isAddition};
}

i64 solve2(inpType inp) {
    auto [numbers, isAddition] = inp;
    u64 total = 0;
    for (size_t column = 0; column < numbers.size(); ++column) {
        u64 rowTotal = !isAddition[column];
        for (size_t row = 0; row < numbers[column].size(); ++row) {
            //cout << row << " " << column << endl;
            //cout << numbers[row][column] << endl;
            if (isAddition[column]) {
                rowTotal += numbers[column][row];
            }
            else {
                rowTotal *= numbers[column][row];
            }
        }
        total += rowTotal;
    }
    return total;
}

i64 solve(inpType inp) {
    auto [numbers, isAddition] = inp;
    u64 total = 0;
    for (size_t column = 0; column < numbers[0].size(); ++column) {
        u64 rowTotal = !isAddition[column];
        for (size_t row = 0; row < numbers.size(); ++row) {
            //cout << row << " " << column << endl;
            //cout << numbers[row][column] << endl;
            if (isAddition[column]) {
                rowTotal += numbers[row][column];
            }
            else {
                rowTotal *= numbers[row][column];
            }
        }
        total += rowTotal;
    }
    return total;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    cout << result << endl;
    auto inp2 = getInput2(0 ? "../example-input.txt" : "../input.txt");
    auto result2 = solve2(inp2);
    cout << result2 << endl;
}