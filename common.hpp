#pragma once
#include <string>
#include <algorithm>
#include <cctype>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

inline std::string upper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

inline std::string lower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

inline bool startswith(std::string str, std::string start)
{
    size_t pos = str.find(start);
    return pos == 0;
}

inline bool endswith(std::string str, std::string end)
{
    size_t pos = str.rfind(end);
    return pos == str.size() - end.size();
}
