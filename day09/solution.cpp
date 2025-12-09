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

#include "lib.cpp"

using namespace std;
using i32 = int;
using i64 = long long int;
using u32 = unsigned;
using u64 = unsigned long long;

#define v(x) vector<x>
#define m(x) vector<vector<x>>
#define iter(x) x::iterator

using Point = pair<u64, u64>;
using Segment = pair<Point, Point>;

using inpType = vector<Point>;
inpType getInput(string fileName) {
    vector<Point> points;
    u64 x, y;
    char c;
    ifstream f(fileName);
    while (f >> x >> c >> y) {
        points.push_back({x, y});
    }
    return points;
}

/*
 * complete search all pairs of point and calculate their area,
 * return the biggest area found (part 1)
 */
i64 solve(inpType inp) {
    u64 biggest = 0;
    for (auto a_it = inp.begin(); a_it != inp.end(); ++a_it) {
        for (auto b_it = a_it + 1; b_it != inp.end(); ++b_it) {
            auto [ax, ay] = *a_it;
            auto [bx, by] = *b_it;
            auto x = max(ax, bx) - min(ax, bx) + 1;
            auto y = max(ay, by) - min(ay, by) + 1;
            auto area = x * y;
            biggest = max(biggest, area);
        }
    }
    return biggest;
}

/*
 * Return a vector of all segments in the input (for part 2).
 * A segment is the line from 2 consecutive points in the input,
 * as well as the line from the first point to the last one in the input.
 */
vector<Segment> getSegments(inpType const& points) {
    vector<Segment> segments;
    for (auto it = points.begin() + 1; it != points.end(); ++it) {
        segments.push_back({*(it - 1), *(it)});
    }
    segments.push_back({points.back(), points.front()});
    return segments;
}

/**
 * Checks if any segments intersect the box bounded by minX, minY, maxX, maxY.
 * The assumption here is that if a box intersects with a segment,
 * only one side of the segment will have green/valid/inside tiles,
 * so the box must lie over some non-valid tiles and can there for be disregarded (for part 2).
 * 
 * All checks are done with strict "<" and ">",
 * since a segment on the edge of our box is ok.
 */
bool noOverlap(u64 minX, u64 minY, u64 maxX, u64 maxY, v(Segment)& segments) {
    for (auto& segment : segments) {
        auto [a, b] = segment;
        auto [ax, ay] = a;
        auto [bx, by] = b;
        if (ax == bx) {  // vertical segment
            if (minX < ax && ax < maxX) {  // check that segment is horizontally inside box.
                auto lowY = min(ay, by);
                auto highY = max(ay, by);
                if (lowY < maxY && highY > minY) {  // check that the segment overlaps the box vertically.
                    return false;
                }
            }
        }
        else {  // horizontal segment.
            if (minY < ay && ay < maxY) {
                auto lowX = min(ax, bx);
                auto highX = max(ax, bx);
                if (lowX < maxX && highX > minX) {
                    return false;
                }
            }
        }
    }
    return true;
}

/**
 * Checks that the box is inside the shape and not outside the shape.
 * "The shape" refers to the shape created by all segments. 
 * The check is done by a ray-cast upwards from the middle of the top segment of the box we want to check.
 * If this ray intersects and odd number of segments, we must lie inside the shape,
 * if it intersects and even number of segments, the box must be outside of the shape.
 */
bool checkInsideVertical(u64 x, u64 y, v(Segment)& segments) {
    u64 segmentCollisions = 0;
    for (auto& segment : segments) {
        auto [a, b] = segment;
        auto [ax, ay] = a;
        auto [bx, by] = b;
        if (ay == by && ay >= y) {
            auto lowX = min(ax, bx);
            auto highX = max(ax, bx);
            if (lowX <= x && x < highX) {
                segmentCollisions++;
            }
        }
    }
    return segmentCollisions % 2 == 1;
}

/**
 * Same as checkInsideVertical, but does the raycast to the right instead.
 * Not sure if both are needed, but I added it to help with the edge-case
 * when a box is just one tile wide or one tile high.
 */
bool checkInsideHorizontal(u64 x, u64 y, v(Segment)& segments) {
    u64 segmentCollisions = 0;
    for (auto& segment : segments) {
        auto [a, b] = segment;
        auto [ax, ay] = a;
        auto [bx, by] = b;
        if (ax == bx && ax >= x) {
            auto lowY = min(ay, by);
            auto highY = max(ay, by);
            if (lowY <= y && y < highY) {
                segmentCollisions++;
            }
        }
    }
    return segmentCollisions % 2 == 1;
}

/**
 * If a box is confined it is a valid candidate for part 2, and true is returned.
 * Being confined is checked with the three helper-functions noOverlap, checkInsideVertical and checkInsideHorizontal.
 */
bool isConfined(u64 minX, u64 minY, u64 maxX, u64 maxY, v(Segment)& segments) {
    if (noOverlap(minX, minY, maxX, maxY, segments)) {
        if (maxX - minX > maxY - minY) {
            return checkInsideVertical((minX + maxX) / 2, maxY, segments);
        }
        else {
            return checkInsideHorizontal(maxX, (minY + maxY) / 2, segments);
        }
    }
    return false;
}

/**
 * Same as solve(), but only considers boxes that are confined to constraints of part 2.
 */
i64 solve2(inpType& inp) {
    auto segments = getSegments(inp);
    u64 biggest = 0;
    for (auto a_it = inp.begin(); a_it != inp.end(); ++a_it) {
        for (auto b_it = a_it + 1; b_it != inp.end(); ++b_it) {
            auto [ax, ay] = *a_it;
            auto [bx, by] = *b_it;
            auto minX = min(ax, bx);
            auto maxX = max(ax, bx);
            auto minY = min(ay, by);
            auto maxY = max(ay, by);
            auto x = maxX - minX + 1;
            auto y = maxY - minY + 1;
            auto area = x * y;
            if (area > biggest && isConfined(minX, minY, maxX, maxY, segments)) {
                biggest = area;
            }
        }
    }
    return biggest;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    auto result2 = solve2(inp);
    cout << "Part 1: " << result << endl;
    cout << "Part 2: " << result2 << endl;
}