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
    else if ((i = server->is_nickname_available(cmd[0])) == -1)
        adam_sender(_fd, ERR_NOSUCHNICK(_nickname, cmd[0]));
    else
    {
        int fd = server->clients[i]._fd;
        send(fd, ":", 1, 0);
        send(fd, _nickname.c_str(), strlen(_nickname.c_str()), 0);
        send(fd, " PRIVMSG ", 9, 0);
        send(fd, server->clients[i]._nickname.c_str(), strlen(server->clients[i]._nickname.c_str()), 0);
        adam_sender(fd, " :" + cmd[1].substr(1));
    }
    
}