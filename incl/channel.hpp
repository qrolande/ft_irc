#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "global.hpp"

enum Mode {
    none = 0,               // 0000000000
    oper = 1 << 0,          // 0000000001
    invite_only = 1 << 1,   // 0000000010
    limited = 1 << 2,       // 0000000100
    protectedTopic = 1 << 3,         // 0000001000
};

class Channel
{
private:
    std::string _channel_name;
    std::string _topic;
    Server *_server;
    int                                     _modes;
    int                                     _limit;

    std::vector<User *> operators;
public:
    Channel(std::string channel_name, Server *server);
    std::vector<User *> channel_users;
    int get_users_count( void );
    void add_user( User *user );
    bool user_in_channel(int fd);

    std::string get_channel_name( void );
    std::string get_topic( void );
    void set_topic( std::string topic );


    void send_all( User *user, std::string message, bool flag );
    void send_all(std::string message);
    void send_all_n( User *user, std::string message, bool flag );

    void remove_client(int fd);

    void add_operator(User *user, std::string nick);
    void delete_operator(User *user, std::string nick);

    bool is_operator( int fd );

    void give_operator( void );


    void set_limit( int limit );
    int get_limit( void );
    bool has_empty_place( void );

    void                            set_mode(Mode, std::string nick, std::string change);
    void                            unset_mode(Mode, std::string nick, std::string change);
    bool                            has_mode(Mode) const;
    std::string                     show_mode() const;
};

#endif