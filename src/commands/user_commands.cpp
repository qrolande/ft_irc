#include "../../incl/global.hpp"

void User::quit_cmd(std::vector<std::string> cmd)
{
    cmd[0].clear();
    //доделать логику с сообщением
    printf("[FD%d] DISCONNECTED\n", _fd);
    for (unsigned int i = 0; i < server->client_socket.size(); i++)
        if (server->client_socket[i] == _fd)
            server->client_socket[i] = 0;
    close(_fd);
}

void User::privmsg_cmd(std::vector<std::string> cmd)
{
    int i;
    split(cmd, 2);
    if (cmd[0][0] == ':')
        adam_sender(_fd, ERR_NORECIPIENT(_nickname, "PRIVMSG"));
    else if (cmd.size() == 1)
        adam_sender(_fd, ERR_NOTEXTTOSEND(_nickname));
    else if ((i = server->is_nickname_available(cmd[0])) == -1)
        adam_sender(_fd, ERR_NOSUCHNICK(_nickname, cmd[0]));
    else
    {
        adam_sender(server->clients[i]._fd, RPL_PRIVMSG(_nickname,
            cmd[0], cmd[1][0] == ':' ? cmd[1].substr(1) : cmd[1]));
    }
    
}

void User::away_cmd( std::vector<std::string> cmd )
{
    if (cmd.size() == 1)
    {
        adam_sender(_fd, RPL_UNAWAY(_nickname));
        is_away = false;
    }
    else
    {
        adam_sender(_fd, RPL_NOWAWAY(_nickname));
        is_away = true;
        away_message = cmd[1];
    }
}