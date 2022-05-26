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

int Channel::get_users_count(void) 
{
    users_count = channel_users.size();
    return (users_count);
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

bool Channel::user_in_channel(int fd) {
    std::list<int>::iterator start = channel_users.begin();
    while (start != channel_users.end())
    {
        if ((*start) == fd)
            return true;
        start++;
    }
    return false;
}

void Channel::remove_client(int fd)
{
    std::list<int>::iterator start = channel_users.begin();
    while (start != channel_users.end())
    {
        if ((*start) == fd)
        {
            channel_users.erase(start);
            break;
        }
        start++;
    }
    printf("[FD%d] removed from %s\n", fd, this->_channel_name.c_str());
    if (channel_users.size() == 0)
    {
        printf("%s is deleted\n", _channel_name.c_str());
        _server->remove_channel(_channel_name);
    }
}