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

struct Node {
    u64 id;
    v(u64) neighbors;
};

ostream& operator <<(ostream& os, Node const& node) {
    return os << '(' << node.id << ": " << node.neighbors << ')';
}

using inpType = pair<vector<Node>, map<string, u64>>;
inpType getInput(string fileName) {
    vector<Node> nodes;
    map<string, u64> nameToNode;  // map from node name like "aaa" to id like 37
    string line;
    ifstream f(fileName);
    u64 nextId = 0;

    while (getline(f, line)) {
        stringstream s(line);
        string nodeName;
        u64 currentNode;
        while(getline(s, nodeName, ' ')) {
            bool newNode = false;
            if (nodeName.size() == 4) {
                nodeName.pop_back();
                newNode = true;
            }
            if (!nameToNode.contains(nodeName)) {
                nameToNode[nodeName] = nextId++;
                nodes.push_back(Node{nameToNode[nodeName], v(u64)()});
            }
            if (newNode) {
                currentNode = nameToNode[nodeName];
            }
            else {
                auto& neighbors = nodes[currentNode].neighbors;
                neighbors.push_back(nameToNode[nodeName]);
            }
        }
    }
    return {nodes, nameToNode};
}

/**
 * Solve part 1 by depth first search.
 */
u64 dfs(v(Node) const& nodes, u64 current) {
    // using memoization for optimization.
    // forgot to implement it fully here for part 1, so it doesn't quiet work, ops
    static map<u64, u64> memory;
    if (memory.contains(current)) {
        return memory[current];
    }
    // base case: reached the output node "out"
    else if (nodes[current].neighbors.empty()) {
        memory[current] = 1;
        return 1;
    }
    // recursive case: search through all connected nodes.
    else {
        u64 sum = 0;
        for (auto& node : nodes[current].neighbors) {
            sum += dfs(nodes, node);
        }
        return sum;
    }
}

/**
 * Solves part 1 with dfs.
 */
u64 solve(inpType inp) {
    auto [nodes, names] = inp;
    return dfs(nodes, names["you"]);
}

/**
 * Solves part 2 with dfs, but now also keeps track off if dac and fft have been visited.
 */
u64 dfs2(v(Node) const& nodes, u64 current, bool visitedFft, bool visitedDac, u64 fftId, u64 dacId) {
    // uses memoization with memory to avoid duplicate work.
    // the memory stores a state as a tuple of nodeId,
    // and two bools to indicate if fft and dac have been visited.
    static map<tuple<u64, bool, bool>, u64> memory;
    if (memory.contains({current, visitedFft, visitedDac})) {
        return memory[{current, visitedFft, visitedDac}];
    }
    // base case: "out" node has been reached.
    else if (nodes[current].neighbors.empty()) {
        // only if fft and dac have been visited should this count towards the path count.
        if (visitedFft && visitedDac) {
            memory[{current, visitedFft, visitedDac}] = 1;
            return 1;
        }
        else {
            memory[{current, visitedFft, visitedDac}] = 0;
            return 0;
        }
    }
    // recursive case: search through all neighboring nodes.
    else {
        u64 sum = 0;
        for (auto& node : nodes[current].neighbors) {
            sum += dfs2(nodes, node, visitedFft || node == fftId, visitedDac || node == dacId, fftId, dacId);
        }
        // update memory to avoid computing paths from this node again. (memoization)
        memory[{current, visitedFft, visitedDac}] = sum;
        return sum;
    }
}

/**
 * Solves part 2.
 */
u64 solve2(inpType inp) {
    auto [nodes, names] = inp;
    return dfs2(nodes, names["svr"], false, false, names["fft"], names["dac"]);
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    cout << "Part 1: " << result << endl;
    auto result2 = solve2(inp);
    cout << "Part 2: " << result2 << endl;
}