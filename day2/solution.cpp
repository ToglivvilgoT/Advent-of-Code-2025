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

// the fact I needed long long took me way to long to figure out.
// Im to used to python's magic ints resizing to never overflow...
vector<tuple<long long, long long>> getInput(string fileName) {
    ifstream f(fileName);
    vector<tuple<long long, long long>> inp;
    long long start, end;
    char c;
    while (f >> start >> c >> end) {
        inp.push_back(make_tuple(start, end));
        f >> c;
    }
    return inp;
}

bool is_invalid_number(long long num, int part = 2) {
    string str = to_string(num);
    int length = str.length();
    if (part == 1) {
        // this is pretty simple, just check if the number has even length and
        // that the first and second half are equal.
        return (length % 2 == 0) && str.substr(0, length / 2) == str.substr(length / 2, length / 2);
    }
    else {
        // for part 2, we must check for a repetition of any size (div_size).
        // E.g. 202020 would have a repetition of div_size = 2 since "20" has length 2.
        for (int div_size = 1; div_size < length; ++div_size) {
            if (length % div_size) {
                continue;
            }
            string prev = "";
            // partition the number-string into substrings of length div_size and
            // check that they are all equal to each other.
            for (int div = 0; div < length / div_size; ++div) {
                string curr = str.substr(div * div_size, div_size);
                if (prev == "" || prev == curr) {
                    prev = curr;
                }
                else {
                    // If the solution doesn't have a goto,
                    // is it truly a c++ solution?
                    goto not_found;
                }
            }
            return true;
            not_found:
            int a = 0; // had to add this dummy line so "goto not_found:" would work.
        }
        return false;
    }
}

                          //|vvvvvvvvvvvvvvvvvvvv| very long
long long solve(vector<tuple<long long, long long>> inp) {
    long long total = 0;
    for (auto it = inp.begin(); it != inp.end(); ++it) {
        auto [start, end] = *it;
        // complete search brute force check all numbers [start, end] and add to total if they are invalid.
        for (auto [start, end] = *it; start <= end; ++start) {
            total += start * is_invalid_number(start);
        }
    }
    return total;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    cout << result << endl;
}