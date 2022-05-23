#include "../incl/global.hpp"

Channel::Channel(std::string channel_name, Server *server) : _channel_name(channel_name) , _server(server) {}

std::string Channel::get_channel_name( void )
{
    return _channel_name;
}

void Channel::add_user( int fd )
{
    channel_users.push_back(fd);
}

void Channel::send_all( User *user, std::string message, bool flag )
{
    std::list<int>::iterator it;
    for (it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (*it != user->get_fd())
        {
            if (!flag)
                adam_sender(*it, RPL_PRIVMSG(user->get_nickname(), _channel_name, message));
            else
                adam_sender(*it, message);
        }
    }
}