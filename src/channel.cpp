#include "../incl/global.hpp"

Channel::Channel(std::string channel_name, Server *server) : _channel_name(channel_name) , _server(server), _modes(0) {}

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
    return (channel_users.size());
}

void Channel::send_all( User *user, std::string message, bool flag )
{
    std::vector<User *>::iterator it;
    for (it = channel_users.begin(); it != channel_users.end(); ++it)
    {
        if (*it != user)
        {
            if (!flag)
                adam_sender((*it)->get_fd(), RPL_NOTICE(user->get_nickname(), _channel_name, message));
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
    // std::vector<Channel *>::iterator start_c;
    while (start != channel_users.end())
    {
        if ((*start)->get_fd() == fd)
        {
            channel_users.erase(start);
            break;
        }
        start++;
    }
    start = operators.begin();
    while (start != operators.end())
    {
        if ((*start)->get_fd() == fd)
        {
            operators.erase(start);
            break;
        }
        start++;
    }
    give_operator();
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

void Channel::set_topic( std::string topic )
{
    _topic = topic;
}

void Channel::set_mode(Mode flag, std::string nick, std::string change) {
    _modes |= flag;
    send_all(RPL_MODE(nick, _channel_name, change));
}

void Channel::unset_mode(Mode flag, std::string nick, std::string change) {
    _modes &= (~flag);
    send_all(RPL_MODE(nick, _channel_name, change));
}

bool Channel::has_mode(Mode flag) const {
    return ((_modes & flag) == flag);
}

// {[+|-]|o|p|s|i|t|n|b|v}
std::string Channel::show_mode() const {
    std::string show;
    if (has_mode(oper))
        show += 'o';
    if (has_mode(invite_only))
        show += 'i';
    if (has_mode(protectedTopic))
        show += 't';
    if (has_mode(limited))
        show += "l " + std::to_string(_limit);

    return show.empty() ? "" : '+' + show;
}

void Channel::add_operator(User *user)
{
    for (unsigned int i = 0; i < operators.size(); i++)
        if (operators[i] == user)
            return;
    operators.push_back(user);
}

bool Channel::is_operator( int fd )
{
    std::vector<User *>::iterator start = operators.begin();
    while (start != operators.end())
    {
        if ((*start)->get_fd() == fd)
            return true;
        start++;
    }
    return false;
}

void Channel::give_operator( void )
{
	if (is_operator(channel_users[0]->get_fd()))
		return;
	add_operator(channel_users[0]);
}

void Channel::set_limit( int limit )
{
    _limit = limit;
}

int Channel::get_limit( void )
{
    return _limit;
}

bool Channel::has_empty_place( void )
{
    if (has_mode(limited) && _limit - get_users_count() <= 0)
        return false;
    return true;
}