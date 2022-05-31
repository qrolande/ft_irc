#include "../incl/global.hpp"

void Error(std::string msg)
{
    std::cout << "Error: " << msg << "\n";
    exit(1);
}

void Error(std::string msg, Server *sever)
{
    std::cout << "Error: " << msg << "\n";
    sever->exiting();
    exit(1);
}

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
            dest.push_back(tmp.substr(tmp[i] == ':' ? i + 1 : i));
            return;
        }
        else
            dest.push_back(tmp.substr(i, j - i));
        for (; tmp[j] == ' '; j++);
        i = j;
    }
}

std::vector<std::string> split(std::string &str, char sep)
{
    std::vector<std::string> result;
    if (str.find(sep) == std::string::npos)
        result.push_back(str);
    while (str.find(sep) != std::string::npos) {
        if (str.find(sep) == 0){
            str.erase(0);
            continue;
        }
        else
            result.push_back(str.substr(0, str.find(sep)));
        str.erase(0, str.find(sep) + 1);
        if ((str.find(sep) == std::string::npos)){
            result.push_back(str);
            break;
        }
    }
    return result;
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
    std::cout << message << std::endl;
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

bool    is_valid_keys(const std::string& keys, bool mode)
{
    if (mode)
    {
        for (unsigned int i = 0; i < keys.length(); i++)
        {
            if (keys[i] != '+' && keys[i] != '-' && keys[i] != 'o' && keys[i] != 'i'
                && keys[i] != 'l' && keys[i] != 't')
                return false;
        }
    }
    else
    {
        for (unsigned int i = 0; i < keys.length(); i++)
        {
            if (keys[i] != '+' && keys[i] != '-' && keys[i] != 'i' && keys[i] != 's'
                && keys[i] != 'o' && keys[i] != 'w')
                return false;
        }
    }
    return true;
}

std::string to_lower(std::string str){
    std::string::iterator start, end;

    start = str.begin();
    end = str.end();

    while (start != end) {
        if ((*start) >= 'A' && (*start) <= 'Z')
            (*start) += 32;
        start++;
    }
    return str;
}