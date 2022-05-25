#include "../../incl/global.hpp"

bool    is_valid_chanell(const std::string& channel) {
    if (channel[0] != '#')
        return false;

    if (channel.length() < 200) {
        for (unsigned long i = 1; i < channel.length(); ++i) {
            if (!std::isalnum(channel[i]))
                return false;
        }
    }
    return true;
}

void User::join_cmd( std::vector<std::string> cmd )
{
    if (cmd.size() != 2)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else {
        split(cmd, 2);
        if (!is_valid_chanell(cmd[0]))
            adam_sender(_fd, ERR_NOSUCHCHANNEL(_nickname, cmd[0]));
        else {
            int i = server->is_channel_available(cmd[0]);
            if (i == -1)
            {
                Channel *channel = new Channel(cmd[0], server);
                server->channels.push_back(channel);
                channel->add_user(_fd);
                i = 0;
            }
            if (server->channels[i]->user_in_channel(_fd)){
                adam_sender(_fd, ERR_USERONCHANNEL(_nickname, _username, server->channels[i]->get_channel_name()));
            }
            else
            {
                server->channels[i]->add_user(_fd);
            }
            adam_sender(_fd, RPL_TOPIC(_nickname, cmd[0], "topic"));
            server->channels[i]->send_all(this, RPL_JOIN(_nickname, cmd[0]), true);
            std::cout << server->channels[i]->get_users_count() << std::endl;
        }
    }
}