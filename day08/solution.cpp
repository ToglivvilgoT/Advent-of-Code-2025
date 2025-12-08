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
#include <queue>

#include "lib.cpp"

using namespace std;
using i32 = int;
using i64 = long long int;
using u32 = unsigned;
using u64 = unsigned long long;

#define v(x) vector<x>
#define m(x) vector<vector<x>>
#define iter(x) x::iterator

/*
 * A point in 3d space.
 */
struct Point {
    u64 x;
    u64 y;
    u64 z;
    /* A unique id, starting from 0, used for union/find as index. */
    u64 id;

    Point(u64 x, u64 y, u64 z) {
        static u64 nextId = 0;
        this->id = nextId++;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /*
     * Calculate and return the distance squared between this point and other.
     */
    i64 distanceTo(Point const& other) const {
        i64 dx = static_cast<i64>(x) - static_cast<i64>(other.x);
        i64 dy = static_cast<i64>(y) - static_cast<i64>(other.y);
        i64 dz = static_cast<i64>(z) - static_cast<i64>(other.z);
        return dx * dx + dy * dy + dz * dz;
    }
};

ostream& operator <<(ostream& os, Point const& p) {
    return os << '(' << p.x << ", " << p.y << ", " << p.z << ". " << p.id << ')';
}

/*
 * Represents the edge between two points and the length between them.
 */
struct Edge {
    Point a;
    Point b;
    double length;

    Edge(Point& a, Point& b) : a(a), b(b), length(a.distanceTo(b)) {}

    bool operator <(Edge const& other) const {
        return this->length >= other.length;
    }

};

ostream& operator <<(ostream& os, Edge const& edge) {
    return os << make_pair(edge.a, edge.b);
}

struct UnionFind {
    /*
     * keeps track of the different sets.
     * tree[i] is the parent of i.
     */
    v(u64) tree;

    UnionFind(u64 size) {
        for (u64 parent = 0; parent < size; ++parent) {
            tree.push_back(parent);
        }
    }

    /*
     * returns the ancestor of a child.
     * the ancestor is the child itself if tree[child] == child.
     * else it is the ancestor of it's parent.
     * can be used to check if two nodes are in the same set,
     * (if so they must have the same ancestor).
     */
    u64 getAncestor(u64 child) {
        if (tree[child] == child) {
            return child;
        }
        u64 ancestor = getAncestor(tree[child]);
        tree[child] = ancestor;
        return ancestor;
    }

    /*
     * combines the set containing child with the set containing parent.
     */
    void combine(u64 child, u64 parent) {
        tree[getAncestor(child)] = getAncestor(parent);
    }

    /*
     * returns true if a and b are in the same set.
     */
    bool same(u64 a, u64 b) {
        return getAncestor(a) == getAncestor(b);
    }

    /*
     * memory used for isUnionRange
     * basically makes isUnionRange O(log(n)) instead of O(n)
     * using memoization / dynamic programming.
     */
    set<pair<u64, u64>> sameMemory;

    /*
     * returns true if all nodes from start to end are in the same set
     * (including both endpoints).
     */
    bool isUnionRange(u64 start, u64 end) {
        // check memory to avoid repeated work.
        if (sameMemory.contains({start, end})) {
            return true;
        }

        // base case: the range is just 2 nodes so check if they are in the same set
        if (start + 1 == end) {
            bool areSame = same(start, end);
            if (areSame) {
                // save to memory to avoid repeated work.
                sameMemory.insert({start, end});
            }
            return areSame;
        }

        // recursive case: split this range into 2 halves and check that both of them are in a union
        u64 midPoint = (start + end) / 2;
        return isUnionRange(start, midPoint) && isUnionRange(midPoint, end);
    }

    /*
     * returns true if all nodes are in the same set.
     */
    bool isCompleteUnion() {
        return isUnionRange(0, tree.size() - 1);
    }

    /*
     * return the product of the 3 largest sets (answer for part 1).
     */
    u64 threeLargest() {
        map<u64, u64> sizes;
        for (u64 node = 0; node < tree.size(); ++node) {
            u64 ancestor = getAncestor(node);
            if (sizes.contains(ancestor)) {
                ++sizes[ancestor];
            }
            else {
                sizes[ancestor] = 1;
            }
        }
        v(u64) values;
        for (auto entry : sizes) {
            values.push_back(entry.second);
        }
        sort(values.begin(), values.end(), [](auto a, auto b) { return b < a; });
        return values[0] * values[1] * values[2];
    }
};

using inpType = priority_queue<Edge>;
inpType getInput(string fileName) {
    // first read in all points
    v(Point) points;
    u64 x, y, z;
    char c;
    ifstream f(fileName);
    while (f >> x >> c >> y >> c >> z) {
        points.push_back(Point{x, y, z});
    }
    // then create a priority queue of all possible edges based on their distance.
    priority_queue<Edge> edges;
    for (auto a_it = points.begin(); a_it != points.end(); ++a_it) {
        for (auto b_it = a_it + 1; b_it != points.end(); ++b_it) {
            edges.push(Edge(*a_it, *b_it));
        }
    }
    return edges;
}

i64 solve(inpType edges, u64 nodes, u64 connections) {
    auto tree = UnionFind(nodes);
    auto& edge = edges.top();
    for (; connections > 0; --connections) {
        tree.combine(edge.a.id, edge.b.id);
        edges.pop();
    }
    auto answer1 = tree.threeLargest();
    while (!tree.isCompleteUnion()) {
        tree.combine(edge.a.id, edge.b.id);
        if (tree.isCompleteUnion()) {
            break;
        }
        edges.pop();
    }
    return edge.a.x * edge.b.x;
}

void main() {
    const bool isExample = false;
    auto inp = getInput(isExample ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp, isExample ? 20 : 1000, isExample ? 10 : 1000);
    cout << result << endl;
}