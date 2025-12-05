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

using namespace std;
using i32 = int;
using i64 = long long int;
using u32 = unsigned;
using u64 = unsigned long long;

#define v(x) vector<x>
#define m(x) vector<vector<x>>
#define iter(x) x::iterator

using inpType = pair<vector<pair<u64, u64>>, vector<u64>>;
inpType getInput(string fileName) {
    vector<pair<u64, u64>> ranges;
    ifstream f(fileName);
    string row;
    u64 start, end;
    char c;
    u64 id;
    vector<u64> ids;
    while (f >> row) {
        stringstream rows(row);
        if (row.contains('-')) {
            rows >> start >> c >> end;
            ranges.push_back({start, end});
        } else {
            rows >> id;
            ids.push_back(id);
        }
    }
    return {ranges, ids};
}

u64 solve(inpType inp) {
    auto [ranges, ids] = inp;

    // filter out only the id's that are in range
    auto res = ids
    | views::filter([&ranges](u64 id){
        return ranges::any_of(ranges, [id](pair<u64, u64> range){
            return range.first <= id && id <= range.second;
        });
        cout << endl;
    });

    // return total count of id's in range
    return ranges::fold_left(res, static_cast<u64>(0), [](auto tot, auto next){ return ++tot; });
}

u64 solve2(inpType inp) {
    auto [ranges, _] = inp;

    // sort lexicographically
    sort(ranges.begin(), ranges.end(), [](auto l, auto r) {
        return l.first < r.first || l.first == r.first && l.second < r.second;
    });

    // iterate over all intervals and sum up their coverage
    u64 previousMax = 0;
    return ranges::fold_left(ranges, static_cast<u64>(0), [&previousMax](auto total, auto range) {
        // calculate the size of the current interval
        auto increment = range.second - range.first + 1;
        total += increment;

        // if the previous interval overlaps this one:
        if (previousMax >= range.first) {
            // calculate the overlap and subtract it
            // min() is needed for the cases where previous interval was e.g.
            // {3, 5} and current is {4, 4}, then
            // increment will be 1 (the size of {4, 4}) but
            // overlap will be 2
            auto overlap = previousMax - range.first + 1; 
            total -= min(increment, overlap);
        }

        previousMax = max(previousMax, range.second);
        return total;
    });
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    auto result2 = solve2(inp);
    cout << result << endl;
    cout << result2 << endl;
}