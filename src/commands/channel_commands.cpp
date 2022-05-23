#include "../../incl/global.hpp"

void User::join_cmd( std::vector<std::string> cmd )
{
    split(cmd, 2);
    int i = server->is_channel_available(cmd[0]);
    std::cout << "available ? :" << i << std::endl;
    if (i == -1)
    {
        Channel *channel = new Channel(cmd[0], server);
        server->channels.push_back(channel);
        channel->add_user(_fd);
        i = 0;
    }
    else
    {
        server->channels[i]->add_user(_fd);
    }
    adam_sender(_fd, RPL_TOPIC(_nickname, cmd[0], "topic"));
    server->channels[i]->send_all(this, RPL_JOIN(_nickname, cmd[0]), true);
}