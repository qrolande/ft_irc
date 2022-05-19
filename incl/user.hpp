#ifndef USER_HPP
# define USER_HPP

# include "global.hpp"
#define BUFFER_SIZE 4096

class User
{
private:
    int _fd;
    std::string _nickname;
    std::string _username;

    bool is_password_passed;
    bool is_nickname_passed;
    bool is_username_passed;

    bool check_password( std::vector<std::string> cmd );
    bool check_nickname( std::vector<std::string> cmd );
    bool check_username( std::vector<std::string> cmd );

    void buffer_copy( char command[] );

public:
    char buffer[BUFFER_SIZE];

    User(int fd);

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