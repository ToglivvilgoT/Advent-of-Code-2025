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
#define iter(x) x::iterator

struct Machine {
    u64 lights;
    v(u64) buttons;

    // didn't end up using these, solved part 2 with python instead.
    v(i64) joltages;
    v(v(u64)) buttons2;
};

ostream& operator <<(ostream& os, Machine const& machine) {
    return os << '(' << machine.lights << " " << machine.buttons << ')';
}

/**
 * Parse the lights as a u64 where a bit of 1 means it should be on and a bit of 0 means it should be off.
 */
u64 parseLights(string::iterator firstLight) {
    char currentLight = *firstLight;
    switch (currentLight) {
    case ']':
        return 0;
    case '#':
        return (parseLights(++firstLight) << 1) + 1;
    case '.':
        return (parseLights(++firstLight) << 1);
    default:
        cout << "Parse lights went wrong." << endl;
    }
}

/**
 * Parse a button as a u64 where a bit is one if it toggles that light and zero otherwise.
 */
u64 parseButton(string& button) {
    u64 result = 0;
    u64 num;
    char c;
    stringstream buttonStream(button);
    while (buttonStream >> c >> num) {
        result |= 1 << num;
    }
    return result;
}

/**
 * Not used, see solution.py for part 2
 */
v(u64) parseButton2(string& button) {
    v(u64) result;
    u64 num;
    char c;
    stringstream buttonStream(button);
    while (buttonStream >> c >> num) {
        result.push_back(num);
    }
    return result;
}

/**
 * Not used, see solution.py for part 2
 */
v(i64) parseJoltages(string& joltages) {
    v(i64) result;
    u64 num;
    char c;
    stringstream joltagesStream(joltages);
    while (joltagesStream >> c >> num) {
        result.push_back(num);
    }
    return result;
}

using inpType = v(Machine);
inpType getInput(string fileName) {
    inpType machines = inpType();
    string line;
    ifstream f(fileName);
    while (getline(f, line)) {
        Machine machine;
        machine.buttons = v(u64)();
        stringstream lineStream(line);
        string part;
        while (getline(lineStream, part, ' ')) {
            if (part[0] == '[') {
                machine.lights = parseLights(part.begin() + 1);
            }
            else if (part[0] == '(') {
                machine.buttons.push_back(parseButton(part));
                machine.buttons2.push_back(parseButton2(part));
            }
            else if (part[0] == '{') {
                machine.joltages = parseJoltages(part);
            }
        }
        machines.push_back(machine);
    }
    return machines;
}

/**
 * Solves one instance of the problem for part 1.
 * Solves it using BFS where each node is a light configuration,
 * the start node is all lights off,
 * the target node is the light configuration from input,
 * and the edges are a button press.
 */
u64 solveMachine(Machine machine) {
    u64 currLights = 0;
    set<u64> visited;
    visited.insert(currLights);
    queue<u64> queue;
    queue.push(currLights);
    map<u64, u64> distanceToLight;
    distanceToLight[currLights] = 0;

    while (!queue.empty()) {
        currLights = queue.front();
        queue.pop();
        for (auto button : machine.buttons) {
            u64 neighbour = currLights ^ button;
            if (neighbour == machine.lights) {
                return distanceToLight[currLights] + 1;
            }
            if (!visited.contains(neighbour)) {
                visited.insert(neighbour);
                queue.push(neighbour);
                distanceToLight[neighbour] = distanceToLight[currLights] + 1;
            }
        }
    }
    cout << "search failed, something went wrong" << endl;
}

/**
 * Solves each instance of the problem and returns the sum (part 1)
 */
u64 solve(inpType inp) {
    u64 total = 0;
    for (auto machine : inp) {
        total += solveMachine(machine);
    }
    return total;
}

/**
 * Not used, see solution.py for part 2
 */
u64 calculateHeuristic(v(i64)& joltages, u64 biggestButton) {
    i64 sum = 0;
    for (i64 joltage : joltages) {
        sum += joltage;
    }
    return sum / biggestButton;
}

/**
 * Not used, see solution.py for part 2
 */
u64 solveMachine2(Machine& machine) {
    u64 biggestButton = 0;
    for (auto button : machine.buttons2) {
        biggestButton = max(biggestButton, button.size());
    }

    v(i64) currLights = machine.joltages;
    set<v(i64)> visited;
    visited.insert(currLights);
    priority_queue<pair<i64, v(i64)>> queue;
    queue.push({calculateHeuristic(currLights, biggestButton), currLights});
    map<v(i64), u64> distanceToLight;
    distanceToLight[currLights] = 0;

    while (!queue.empty()) {
        auto [_, top] = queue.top();
        auto currLights = top;
        cout << currLights << endl;
        queue.pop();
        for (auto button : machine.buttons2) {
            auto neighbour = currLights;
            for (auto affectedJoltage : button) {
                --neighbour[affectedJoltage];
            }
            bool done = true;
            for (auto joltage : neighbour) {
                if (joltage < 0) {
                    goto nextButton;
                }
                else if (joltage > 0) {
                    done = false;
                }
            }
            if (!visited.contains(neighbour)) {
                visited.insert(neighbour);
                distanceToLight[neighbour] = distanceToLight[currLights] + 1;
                queue.push({-distanceToLight[neighbour] - calculateHeuristic(neighbour, biggestButton), neighbour});
            }
            if (done) {
                return distanceToLight[neighbour];
            }
            nextButton:
            int a;
        }
    }
    cout << "search failed, something went wrong" << endl;
}

/**
 * Not used, see solution.py for part 2
 */
u64 solve2(inpType inp) {
    u64 total = 0;
    for (auto machine : inp) {
        cout << solveMachine2(machine) << endl;
        total += solveMachine2(machine);
    }
    return total;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    cout << result << endl;
    // uncomment and wait 9999999 years for answer 2, or look at solution.py if you are short on time :)
    //auto result2 = solve2(inp);
    //cout << result2 << endl;
}