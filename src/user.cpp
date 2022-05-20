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
    for (int j = 0; buffer[j];)
    {
        int i = contains_new_line(this->buffer + j);
        if (i == -1)
            return;
        this->buffer[i + j] = '\0';
        std::vector<std::string> parsed;
        split(parsed, this->buffer + j);
        std::cout << parsed[0] << "|" << parsed[1] << std::endl;
        if (parsed.size() > 0)
        {
            if (!is_authorized())
            {
                authorize(parsed);
                if (is_authorized())
                {
                    printf("NEW USER! [FD%d] NICKNAME: [%s]\n", _fd, _nickname.c_str());
                    send(_fd, ":ircserv 375 nickname : Message of the day -\r\n", strlen(":ircserv 375 nickname : Message of the day -\r\n"), 0);
                    send(_fd, ":ircserv 372 nickname : ???\r\n", strlen(":ircserv 372 nickname : ???\r\n"), 0);
                    send(_fd, ":ircserv 376 nickname : End of /MOTD command\r\n", strlen(":ircserv 376 nickname : End of /MOTD command\r\n"), 0);
                    send(_fd, ":ircserv 001 username!username@localhost :Welcome to the IRCServ, username!username@localhost", strlen(":ircserv 001 asdcxz!asd123@localhost :Welcome to the IRCServ, asdcxz!asd123@localhost"), 0);
                }
            }
            else {
                send(_fd, buffer, strlen(buffer), 0);
                send(_fd, "\r\n", 2, 0);
            }
        }
        for (j = i + j + 1; buffer[j] == '\r' || buffer[j] == '\n'; j++);
        parsed.clear();
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
    send(_fd, "something went wrong\r\n", strlen("something went wrong\r\n"), 0);
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