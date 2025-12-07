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

template<typename T, typename S>
ostream& operator <<(ostream& stream, pair<T, S> pair) {
    return stream << '(' << pair.first << ", " << pair.second << ')';
}

using inpType = pair<m(char), pair<u64, u64>>;
inpType getInput(string fileName) {
    m(char) manifold;
    pair<size_t, size_t> start;
    ifstream f(fileName);
    size_t x = 0, y = 0;
    v(char) row;
    for (int c = f.get(); c != -1; c = f.get()) {
        if (c == '\n') {
            manifold.push_back(row);
            row = v(char)();
            x = 0;
            ++y;
        }
        else if (c == 'S') {
            start = {x, y};
            ++x;
            row.push_back('.');
        }
        else {
            ++x;
            row.push_back(c);
        }
    }
    manifold.push_back(row);
    return {manifold, start};
}

template<typename T>
bool inBounds(u64 x, u64 y, m(T) const& matrix) {
    return 0 <= x < matrix.size() &&
           0 <= y < matrix[0].size();
}

/* 
 * a simple BFS searching through all beams,
 * queuing one step down if we are on a '.',
 * queuing one point left and right if we are on a '^'
 */
i64 solve(inpType inp) {
    auto [manifold, start] = inp;
    vector<pair<u64, u64>> queue;
    set<pair<u64, u64>> visited;
    queue.push_back(start);
    visited.insert(start);
    u64 maxY = manifold.size();

    u64 amountOfSplits = 0;
    while (!queue.empty()) {
        auto [x, y] = queue.back();
        queue.pop_back();
        if (y >= maxY) {
            continue;
        }
        else if (manifold[y][x] == '.' && !visited.contains({x, y + 1})) {
            queue.push_back({x, y + 1});
            visited.insert({x, y + 1});
        }
        else if (manifold[y][x] == '^') {
            ++amountOfSplits;
            if (!visited.contains({x - 1, y})) {
                queue.push_back({x - 1, y});
                visited.insert({x - 1, y});
            }
            if (!visited.contains({x + 1, y})) {
                queue.push_back({x + 1, y});
                visited.insert({x + 1, y});
            }
        }
    }
    return amountOfSplits;
}

/*
 * Recursively search to find all time lines of beams.
 * Using memory to avoid exploring the same beam-path multiple times (dynamic programming with memoization)
 */
u64 recursiveSearch(m(char) const& manifold, map<pair<u64, u64>, u64>& memory, pair<u64, u64> coord) {
    if (memory.contains(coord)) {
        return memory[coord];
    }
    auto [x, y] = coord;

    // base case, we hit end of manifold,
    // the amount of time lines here is simply one
    // (the one we are currently on).
    if (y >= manifold.size()) {
        return 1;
    }
    // if we hit a dot we simply continue down,
    // when we get back we also cache the result in memory,
    // to avoid exploring this path again.
    else if (manifold[y][x] == '.') {
        auto timeLines = recursiveSearch(manifold, memory, {x, y + 1});
        memory[coord] = timeLines;
        return timeLines;
    }
    // If we hit a splitter,
    // the amount of timelines of this beam is simply the sum
    // of the beam going to the left and the beam going to the right.
    // Once again we also cache the result in memory before returning.
    else if (manifold[y][x] == '^') {
        auto timeLines = recursiveSearch(manifold, memory, {x - 1, y}) +
                         recursiveSearch(manifold, memory, {x + 1, y});
        memory[coord] = timeLines;
        return timeLines;
    }

}

u64 solve2(inpType inp) {
    auto [manifold, start] = inp;
    map<pair<u64, u64>, u64> memory;
    return recursiveSearch(manifold, memory, start);
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    auto result2 = solve2(inp);
    cout << result << endl;
    cout << result2 << endl;
}