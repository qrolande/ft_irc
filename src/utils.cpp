#include "../incl/global.hpp"

void split(std::vector<std::string>& dest, const std::string& str)
{
    unsigned int j, i;
    for (i = 0; i < str.length() && str[i] != ' '; i++);
    dest.push_back(str.substr(0, i++));
    for (; str[i] == ' '; i++);
    for (j = i; j < str.length(); j++);
    if (i < j)
        dest.push_back(str.substr(i, j - i));
}

void split(std::vector<std::string>& dest, int max_args)
{
    std::string tmp = dest[1];
    dest.clear();
    for (unsigned int i = 0, j; i < tmp.length() && max_args != 0; max_args--)
    {
        for (j = i; tmp[j] && tmp[j] != ' '; j++);
        if (max_args == 1 || tmp[i] == ':')
        {
            dest.push_back(tmp.substr(i));
            return;
        }
        else
            dest.push_back(tmp.substr(i, j - i));
        for (; tmp[j] == ' '; j++);
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

bool    is_nickname_valid( const std::string& nick )
{
    const std::string spec = "-[]^{}";
    if (nick.length() > 9)
        return false;
    for (unsigned int i = 0; i < nick.length(); ++i) {
        if (!std::isalnum(nick[i]) && spec.find(nick[i]) == std::string::npos)
            return false;
    }
    return true;
}