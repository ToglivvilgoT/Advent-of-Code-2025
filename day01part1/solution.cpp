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

// this function was suppose to only take input,
// but in the heat of it all I ended up solving the task here as well :)
int getInput(string fileName) {
    ifstream f(fileName);
    char c;
    int spin;
    int curr = 50;
    int answer = 0;
    while (f >> c >> spin) {
        if (c == 'L') {
            curr -= spin;
        }
        else {
            curr += spin;
        }
        curr %= 100;
        answer += curr == 0;
    }
    return answer;
}

int solve(int inp) {
    return inp;
}

void main() {
    auto inp = getInput(0 ? "../example-input.txt" : "../input.txt");
    auto result = solve(inp);
    cout << result << endl;
}