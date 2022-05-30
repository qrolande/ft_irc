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
            };
            if (server->channels[i]->has_mode(invite_only))
                adam_sender(_fd, ERR_INVITEONLYCHAN(_nickname, cmd[0]));
            else if (server->channels[i]->has_mode(limited) && !server->channels[i]->has_empty_place())
                adam_sender(_fd, ERR_CHANNELISFULL(_nickname, cmd[0]));
            else if (!server->channels[i]->user_in_channel(_fd))
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
        for (unsigned int l = 0; l < channel->channel_users.size(); l++)
            adam_sender(channel->channel_users[i]->get_fd(), RPL_NAMREPLY(channel->channel_users[i]->_nickname, channel->get_channel_name(), server->clients[l]->get_fullname()));
    for (unsigned int l = 0; l < channel->channel_users.size(); l++)
            adam_sender(channel->channel_users[l]->get_fd(), RPL_ENDOFNAMES(_nickname, channel->get_channel_name()));
    channels.push_back(channel);
    channel->give_operator();
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
            if (server->channels[i]->has_mode(protectedTopic))
            {
                if (!server->channels[i]->is_operator(_fd))
                {
                    adam_sender(_fd, ERR_CHANOPRIVSNEEDED(_nickname, cmd[0]));
                    return;
                }
            }
            server->channels[i]->set_topic(cmd[1]);
            server->channels[i]->send_all(RPL_TOPIC(_nickname, cmd[0], server->channels[i]->get_topic()));
        }
    }
}

void User::kick_cmd( std::vector<std::string> cmd )
{
    int i, u = 0;
    split(cmd, -1);
    if (cmd.size() != 2 && cmd.size() != 3)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else if ((i = server->is_channel_available(cmd[0])) == -1)
        adam_sender(_fd, ERR_NOSUCHCHANNEL(_nickname, cmd[0]));
    else if (!server->channels[i]->user_in_channel(_fd))
        adam_sender(_fd, ERR_NOTONCHANNEL(_nickname, cmd[0]));
    else if (!server->channels[i]->is_operator(_fd))
        adam_sender(_fd, ERR_CHANOPRIVSNEEDED(_nickname, cmd[0]));
    else if ((u = server->is_nickname_available(cmd[1])) == -1
        || !server->channels[i]->user_in_channel(server->client_socket[u]))
        adam_sender(_fd, ERR_NOSUCHNICK(_nickname, cmd[1]));
    else
    {
        User *user = server->clients[server->is_nickname_available(cmd[1])];
        server->channels[i]->send_all(RPL_KICK(_nickname, cmd[0], cmd[1], cmd.size() == 3 ? cmd[2] : ""));
        server->channels[i]->remove_client(user->get_fd());
    }
}

void User::invite_cmd( std::vector<std::string> cmd )
{
    int i, u = 0;
    split(cmd, -1);
    if (cmd.size() != 2)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, "INVITE"));
    else if ((u = server->is_nickname_available(cmd[0])) == -1)
        adam_sender(_fd, ERR_NOSUCHNICK(_nickname, cmd[0]));
    else if ((i = server->is_channel_available(cmd[1])) == -1)
        adam_sender(_fd, ERR_NOSUCHCHANNEL(_nickname, cmd[1]));
    else if (server->channels[i]->user_in_channel(server->clients[u]->get_fd()))
        adam_sender(_fd, ERR_USERONCHANNEL(_nickname, cmd[0], cmd[1]));
    else if (server->channels[i]->has_mode(invite_only)
        && !server->channels[i]->is_operator(_fd))
        adam_sender(_fd, ERR_CHANOPRIVSNEEDED(_nickname, cmd[1]));
    else
    {
        User *user = server->clients[u];
        if (user->is_away)
            adam_sender(_fd, RPL_AWAY(user->get_nickname(), _nickname, user->away_message));
        else
            adam_sender(user->get_fd(), RPL_INVITING(get_fullname(), user->get_nickname(), cmd[1]));
    }
}

