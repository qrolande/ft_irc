#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "global.hpp"

class Channel
{
private:
    std::string _channel_name;
    Server *_server;
    std::list<int> channel_users;
    int users_count;
public:
    Channel(std::string channel_name, Server *server);
    int get_users_count( void );
    void add_user(int fd);

    std::string get_channel_name( void );
    
    void send_all( User *user, std::string message, bool flag );
};

#endif