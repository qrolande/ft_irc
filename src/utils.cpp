#include "../incl/server.hpp"

void split(std::vector<std::string>& dest, const std::string& str)
{
    int i, j;
    for (i = 0; i < str.length() - 1 && str[i] != ' '; i++);
    dest.push_back(str.substr(0, i++));
    for (j = i; j < str.length() && str[j] != '\r' && str[j] != '\n'; j++);
    if (i < j)
        dest.push_back(str.substr(i, j));
}