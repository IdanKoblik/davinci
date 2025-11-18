#include "trim.h"
#include <algorithm>
#include <cctype>

std::string trim(const std::string& s) {
    auto start = std::find_if(s.begin(), s.end(),
                              [](unsigned char ch) { return !std::isspace(ch); });
    auto end = std::find_if(s.rbegin(), s.rend(),
                            [](unsigned char ch) { return !std::isspace(ch); }).base();
    if (start >= end) return "";
    return std::string(start, end);
}


