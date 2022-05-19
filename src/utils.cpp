#include "../incl/global.hpp"

void split(std::vector<std::string>& dest, const std::string& str)
{
    int i, j;
    for (i = 0; i < (int)str.length() && str[i] != ' '; i++);
    dest.push_back(str.substr(0, i++));
    for (j = i; j < (int)str.length(); j++);
    if (i < j)
        dest.push_back(str.substr(i, j));
}

int contains_new_line( char buffer[] )
{
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\r' || buffer[i] == '\n')
            return i;
    }
    return -1;
}