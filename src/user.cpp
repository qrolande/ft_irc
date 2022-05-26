#include "../incl/global.hpp"

User::User(int fd, Server *_server) : _fd(fd), server(_server) {
    _nickname = "user" + std::to_string(fd);
    is_password_passed = false;
    is_nickname_passed = false;
    is_username_passed = false;
    welcomed = false;
    memset(this->buffer, '\0', BUFFER_SIZE);
    functions["PASS"] = &User::password_cmd;
    functions["USER"] = &User::username_cmd;
    functions["NICK"] = &User::nickname_cmd;
    functions["PRIVMSG"] = &User::privmsg_cmd;
    functions["QUIT"] = &User::quit_cmd;
    functions["AWAY"] = &User::away_cmd;
    functions["JOIN"] = &User::join_cmd;
    functions["PING"] = &User::ping_cmd;
}

User & User::operator=(const User & src)
{
    if (this != &src)
    {
        this->server = src.server;
        this->_fd = src._fd; 
    }
    return *this;
}

bool User::is_authorized( void )
{
    return is_nickname_passed && is_password_passed && is_username_passed ? true : false;
}

void User::set_nickname(std::string nickname)
{
    this->_nickname = nickname;
    this->is_nickname_passed = true;
}

void User::set_username(std::string username)
{
    this->_username = username;
    this->is_username_passed = true;
}

bool User::get_nickname_passed()
{
    return this->is_nickname_passed;
}

bool User::get_username_passed()
{
    return this->is_username_passed;
}

std::string User::get_nickname( void )
{
    return this->_nickname;
}

std::string User::get_username( void )
{
    return this->_username;
}

void User::buffer_copy( char command[] )
{
    int i = 0;
    for (; this->buffer[i] != '\0'; i++);
    for (int j = 0; command[j] != '\0' && i < BUFFER_SIZE; j++, i++)
        this->buffer[i] = command[j];
    this->buffer[i] = '\0';
}

void User::parse_command( char command[] )
{
    buffer_copy(command);
    for (int j = 0; buffer[j];)
    {
        int i = contains_new_line(this->buffer + j);
        if (i == -1)
            return;
        this->buffer[i + j] = '\0';
        std::vector<std::string> parsed;
        split(parsed, this->buffer + j);
        if (parsed.size() > 0 && parsed[0].size() > 0)
            work_with_command(parsed);
        for (j = i + j + 1; buffer[j] == '\r' || buffer[j] == '\n'; j++);
        parsed.clear();
    }
    memset(this->buffer, '\0', BUFFER_SIZE);
}

void User::work_with_command( std::vector<std::string> parsed )
{
    if (functions.find(parsed[0]) != functions.end())
    {
        (this->*functions[parsed[0]])(parsed);
        if (!welcomed && is_authorized())
            welcome();
    }
    else
        adam_sender(_fd, ERR_UNKNOWNCOMMAND(_nickname, parsed[0]));
}

int User::get_fd( void )
{
    return this->_fd;
}
