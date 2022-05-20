#include "../../incl/global.hpp"

void adam_sender(int fd, std::string message);
void split(std::vector<std::string>& dest);

void User::check_password(std::vector<std::string> cmd)
{
    if (is_password_passed)
        adam_sender(_fd, ERR_ALREADYREGISTRED(_nickname));
    else if (cmd.size() == 1)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else if (cmd.size() != 2 || cmd[1] != Server::password)
        adam_sender(_fd, ERR_PASSWDMISMATCH(_nickname));
    else
        is_password_passed = true;
}

void User::check_nickname(std::vector<std::string> cmd)
{
    if (cmd.size() == 1)
        adam_sender(_fd, ERR_NONICKNAMEGIVEN(_nickname));
    else if (0) //nick parser needed
        adam_sender(_fd, ERR_ERRONEUSNICKNAME(_nickname, cmd[1]));
    else if (!server->is_nickname_available(cmd[1]))
        adam_sender(_fd, ERR_ALREADYREGISTRED(_nickname));
    else
    {
        this->_nickname = cmd[1];
        this->is_nickname_passed = true;
    }
}

void User::check_username(std::vector<std::string> cmd)
{
    split(cmd);
    if (cmd.size() != 4)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else if (!server->is_username_available(cmd[0]))
        adam_sender(_fd, ERR_ALREADYREGISTRED(cmd[0]));
    else
    {
        this->_username = cmd[0];
        this->_realname = cmd[3];
        is_username_passed = true;
    }
}