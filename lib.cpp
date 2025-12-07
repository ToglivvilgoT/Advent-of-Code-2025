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

template<typename T, typename S>
ostream& operator <<(ostream& stream, pair<T, S> pair) {
    return stream << '(' << pair.first << ", " << pair.second << ')';
}

static int vectorSettingsIndex = std::ios_base::xalloc();

struct vectorSettings {
    string start;
    string separator;
    string end;
};

static vectorSettings defaultVectorSettings = {"[", ", ", "]"};

inline
std::ostream& operator<<(std::ostream& os, const vectorSettings& settings) {
    os.pword(vectorSettingsIndex) = (void*)&settings;  
    return os;
}

template<typename T>
ostream& operator <<(ostream& os, v(T) const& vec) {
    vectorSettings* settings = static_cast<vectorSettings*>(os.pword(vectorSettingsIndex));
    if (!settings) settings = &defaultVectorSettings;

    os << settings->start;
    for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
        os << *it;
        if (it + 1 != vec.cend()) {
            os << settings->separator;
        }
    }
    return os << settings->end;
}

template<typename T>
ostream& operator <<(ostream& os, m(T) const& matrix) {
    os << '[';
    for (auto it = matrix.cbegin(); it != matrix.cend(); ++it) {
        os << *it;
        if (it + 1 != matrix.cend()) {
            os << "\n ";
        }
    }
    return os << ']';
}

template<typename T, typename S>
bool inBounds(T i, v(S) const& vec) {
    return 0 <= i && i < vec.size();
}

template<typename T, typename S>
bool inBounds(T x, T y, m(S) const& matrix) {
    return inBounds(y, matrix) && inBounds(x, matrix[y]);
}