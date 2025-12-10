#include "seen_object.h"
#include <regex>
#include <iostream>

bool SeenObject::parse_message(const std::string &s)
{
    std::string pattern_str =
        R"(\(\()" + name + R"(\)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)(?:\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+))?\))";

    std::regex regex(pattern_str);
    std::smatch match;

    if (std::regex_search(s, match, regex))
    {
        if (match[1].matched && match[2].matched)
        {
            dist  = std::stod(match[1].str());
            angle = std::stod(match[2].str());
        }
        return true;
    }
    return false;
}
