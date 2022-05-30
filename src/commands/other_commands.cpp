#include "../../incl/global.hpp"

void User::ping_cmd( std::vector<std::string> cmd )
{
    if (!is_authorized())
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
    else if (cmd.size() == 1)
        adam_sender(_fd, ERR_NOORIGIN(_nickname));
    else
        adam_sender(_fd, "PONG " + cmd[1] + " " + _nickname);
}

void User::wallops_cmd( std::vector<std::string> cmd )
{
    if (!has_mode(UserOper))
        adam_sender(_fd, ERR_NOPRIVILEGES(_nickname));
    else if (cmd.size() != 2)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else
    {
        for (unsigned int i = 0; i < server->clients.size(); i++)
        {
            User *user = server->clients[i];
            if (user->_fd != _fd && user->has_mode(UserOper) && !user->has_mode(wallopsOff))
                adam_sender(user->_fd, RPL_PRIVMSG(_nickname, user->_nickname, cmd[1]));
        }
    }
}