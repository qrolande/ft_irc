#ifndef USER_HPP
# define USER_HPP

#include "global.hpp"

class Server;

class User
{
private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;

    std::string away_message;

    bool is_password_passed;
    bool is_nickname_passed;
    bool is_username_passed;
    bool is_away;
    bool welcomed;

    void work_with_command(std::vector<std::string> parsed);

    //
    void password_cmd( std::vector<std::string> cmd );
    void nickname_cmd( std::vector<std::string> cmd );
    void username_cmd( std::vector<std::string> cmd );
    void quit_cmd( std::vector<std::string> cmd );
    void privmsg_cmd( std::vector<std::string> cmd );
    void ping_cmd( std::vector<std::string> cmd );
    void away_cmd( std::vector<std::string> cmd );

    void join_cmd( std::vector<std::string> cmd );

    void welcome();


    void buffer_copy( char command[] );

    Server *server;

    typedef void (User::*commandPtr)(std::vector<std::string>);
    std::map<std::string, commandPtr> functions;

public:
    char buffer[BUFFER_SIZE];

    User(int fd, Server *_server);
    User & operator=(const User &);


    bool is_authorized( void );
    void set_nickname( std::string nickname );
    void set_username( std::string username );

    bool get_nickname_passed();
    bool get_username_passed();

    std::string get_nickname( void );
    std::string get_username( void );
    int get_fd( void );

    void parse_command( char command[] );
    void authorize( std::vector<std::string> parsed );
};

#endif