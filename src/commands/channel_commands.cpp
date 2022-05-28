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
    if (!is_authorized())
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
    else if (cmd.size() != 2)
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
                i = server->channels.size() - 1;
                std::cout << "Channel: " << server->channels[i]->get_channel_name() << " created" << std::endl;
            }
            if (!server->channels[i]->user_in_channel(_fd))
                joining(server->channels[i]);
            else 
                adam_sender(_fd, ERR_USERONCHANNEL(_nickname, _username, server->channels[i]->get_channel_name()));
        }
    }
}

void User::joining(Channel *channel)
{
    channel->add_user(this);
    channel->send_all(RPL_JOIN(get_fullname(), channel->get_channel_name()));
    adam_sender(_fd, RPL_TOPIC(_nickname, channel->get_channel_name(), channel->get_topic()));
    for (unsigned int i = 0; i < channel->channel_users.size(); i++)
        channel->send_all(RPL_NAMREPLY(_nickname, channel->get_channel_name(), server->clients[i]->get_fullname())); //need to rework
    channel->send_all(RPL_ENDOFNAMES(_nickname, channel->get_channel_name()));
    channels.push_back(channel);
}

void User::part_cmd( std::vector<std::string> cmd )
{
    int i = 0;
    if (!is_authorized())
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
    else if (cmd.size() != 2)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else
    {
        if (server->is_channel_available(cmd[1]) == -1)
            adam_sender(_fd, ERR_NOSUCHCHANNEL(_nickname, cmd[1]));
        else if ((i = is_on_channel(cmd[1])) == -1)
            adam_sender(_fd, ERR_NOTONCHANNEL(_nickname, cmd[1]));
        else
        {
            channels[i]->send_all(this, RPL_PART(_nickname, channels[i]->get_channel_name(), "leaving."), true);
            channels[i]->remove_client(_fd);
            channels.erase(channels.begin() + i);
        }
    }
}

void User::topic_cmd( std::vector<std::string> cmd )
{
    int i;
    if (!is_authorized())
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
    else if (cmd.size() == 1)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else
    {
        split(cmd, 2);
        if ((i = server->is_channel_available(cmd[0])) == -1)
            adam_sender(_fd, ERR_NOSUCHCHANNEL(_nickname, cmd[0]));
        else if (!server->channels[i]->user_in_channel(_fd))
            adam_sender(_fd, ERR_NOTONCHANNEL(_nickname, cmd[0]));
        else if (cmd.size() == 1)
        {
            if (server->channels[i]->get_topic().length() == 0)
                adam_sender(_fd, RPL_NOTOPIC(_nickname, cmd[0]));
            else
                adam_sender(_fd, RPL_TOPIC(_nickname, cmd[0], server->channels[i]->get_topic()));
        }
        else
        {
            //if (server->channels[i] ) <- проверка на мод
            //{
            //  if (server->channels[i].operators) <- проверка на оператора
            //  {
            //      adam_sender(_fd, ERR_CHANOPRIVSNEEDED();
            //      return;
            //  }
            //}
            server->channels[i]->set_topic(cmd[1]);
            adam_sender(_fd, RPL_TOPIC(_nickname, cmd[0], server->channels[i]->get_topic()));
        }
    }
}