#include "../incl/global.hpp"

void split(std::vector<std::string>& dest, const std::string& str)
{
    int i, j;
    for (i = 0; i < str.length() && str[i] != ' '; i++);
    dest.push_back(str.substr(0, i++));
    for (j = i; j < str.length(); j++);
    if (i < j)
        dest.push_back(str.substr(i, j));
}

int contains_new_line( char buffer[] )
{
    // printf("!%c!\n", buffer[0]);
    for (int i = 0; buffer[i] != '\0'; i++) {
        // std::cout << "!" << std::endl;
        if (buffer[i] == '\r' || buffer[i] == '\n')
            return i;
    }
    return -1;
}