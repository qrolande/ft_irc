#include "user.hpp"

User::User(int fd) : _fd(fd), is_password_passed(true) {}

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

void User::parse_command( std::string command )
{
    std::vector<std::string> parsed;
    split(parsed, command);
    if (parsed.size() > 0)
    {
        if (!is_authorized())
            authorize(parsed);
    }
}

void User::authorize( std::vector<std::string> parsed )
{
    if (parsed.size() != 2 || parsed[1].length() == 0)
        return;
    std::cout << parsed.size() << std::endl;
}