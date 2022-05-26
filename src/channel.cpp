#include "../incl/global.hpp"

Channel::Channel(std::string channel_name, Server *server) : _channel_name(channel_name) , _server(server) {}

std::string Channel::get_channel_name( void )
{
    return _channel_name;
}

void Channel::add_user( User *user )
{
    channel_users.push_back(user);
}

int Channel::get_users_count(void) 
{
    users_count = channel_users.size();
    return (users_count);
}

void Channel::send_all( User *user, std::string message, bool flag )
{
    std::vector<User *>::iterator it;
    for (it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (*it != user)
        {
            if (!flag)
                adam_sender((*it)->get_fd(), RPL_PRIVMSG(user->get_nickname(), _channel_name, message));
            else
                adam_sender((*it)->get_fd(), message);
        }
    }
}

void Channel::send_all( std::string message )
{
    std::vector<User *>::iterator it;
    for (it = channel_users.begin(); it != channel_users.end(); ++it)
            adam_sender((*it)->get_fd(), message);
}

bool Channel::user_in_channel(int fd) {
    std::vector<User *>::iterator start = channel_users.begin();
    while (start != channel_users.end())
    {
        if ((*start)->get_fd() == fd)
            return true;
        start++;
    }
    return false;
}

void Channel::remove_client(int fd)
{
    std::vector<User *>::iterator start = channel_users.begin();
    std::vector<Channel *>::iterator start_c;
    while (start != channel_users.end())
    {
        if ((*start)->get_fd() == fd)
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

std::string Channel::get_topic( void )
{
    return this->_topic;
}