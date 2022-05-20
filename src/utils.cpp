#include "../incl/global.hpp"

void split(std::vector<std::string>& dest, const std::string& str)
{
    int j, i;
    for (i = 0; i < (int)str.length() && str[i] != ' '; i++);
    dest.push_back(str.substr(0, i++));
    for (j = i; j < (int)str.length(); j++);
    if (i < j)
        dest.push_back(str.substr(i, j - i));
}

void split(std::vector<std::string>& dest)
{
    std::string tmp = dest[1];
    dest.clear();
    for (int i = 0, j; i < (int)tmp.length(); i++)
    {
        for (j = i; tmp[j] && tmp[j] != ' '; j++);
        dest.push_back(tmp.substr(i, j - i));
        i = j;
    }
}

int contains_new_line( char buffer[] )
{
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\r' || buffer[i] == '\n')
            return i;
    }
    return -1;
}

void adam_sender(int fd, std::string message)
{
    send(fd, message.c_str(), strlen(message.c_str()), 0);
    send(fd, "\r\n", 2, 0);
}
