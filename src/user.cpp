#include "../incl/global.hpp"

int contains_new_line(char buffer[]);
void split(std::vector<std::string>& dest, const std::string& str);

User::User(int fd) : _fd(fd) {}

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
    for (int j = 0; command[j] != '\0'; j++, i++)
        this->buffer[i] = command[j];
    this->buffer[i] = '\0';
}

void User::parse_command( char command[] )
{
    buffer_copy(command);
    int i = contains_new_line(this->buffer);
    if (i == -1)
        return;
    this->buffer[i] = '\0';
    std::vector<std::string> parsed;
    split(parsed, this->buffer);
    if (parsed.size() > 0)
    {
        if (!is_authorized())
        {
            authorize(parsed);
            if (is_authorized())
            {
                printf("NEW USER! [FD%d] NICKNAME: [%s]\n", _fd, _nickname.c_str());
                send(_fd, "Authorization succesful\r\n", strlen("Authorization succesful\r\n"), 0);
            }
        }
        else {
            send(_fd, buffer, strlen(buffer), 0);
            send(_fd, "\n\r", 2, 0);
        }
    }
    memset(this->buffer, 0, 4096); //BUFFERSIZE
}

//добавить ошибки
void User::authorize( std::vector<std::string> parsed )
{
    if (!is_password_passed)
    {
        if (check_password(parsed))
            return;
    }
    else if (!is_nickname_passed || !is_username_passed)
    {
        if (check_nickname(parsed))
            return;
        if (check_username(parsed))
            return;
    }
    send(_fd, "something went wrong\n\r", strlen("something went wrong\n\r"), 0);
}

bool User::check_password(std::vector<std::string> cmd)
{
    if (cmd.size() != 2 || cmd[1].length() == 0)
        return false;
    if (cmd[0] != "PASS" || cmd[1] != Server::password)
        return false;
    is_password_passed = true;
    return true;
}

bool User::check_nickname(std::vector<std::string> cmd)
{
    if (cmd.size() != 2 || cmd[1].length() == 0)
        return false;
    if (cmd[0] != "NICK" || is_nickname_passed)
        return false;
    this->_nickname = cmd[1];
    this->is_nickname_passed = true;
    return true;
}

bool User::check_username(std::vector<std::string> cmd)
{
    if (cmd.size() != 2 || cmd[1].length() == 0)
        return false;
    if (cmd[0] != "USER" || is_username_passed)
        return false;
    this->_username = cmd[1];
    this->is_username_passed = true;
    return true;
}

int User::get_fd( void )
{
    return this->_fd;
}