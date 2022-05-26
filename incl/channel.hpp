#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "global.hpp"

enum channel_mode {
    none = 0,               // 0000000000
    oper = 1 << 0,          // 0000000001
    invite_only = 1 << 1,   // 0000000010
    limited = 1 << 2,       // 0000000100
    operatorTopic = 1 << 3  // 0000001000
};

class Channel
{
private:
    std::string _channel_name;
    std::string _topic;
    Server *_server;
    int users_count;
public:
    Channel(std::string channel_name, Server *server);
    std::vector<User *> channel_users;
    int get_users_count( void );
    void add_user( User *user );
    bool user_in_channel(int fd);

    std::string get_channel_name( void );
    std::string get_topic( void );


    void send_all( User *user, std::string message, bool flag );
    void send_all(std::string message);

    void remove_client(int fd);
};

#endif