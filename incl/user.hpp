#ifndef USER_HPP
# define USER_HPP

#include <cstring>
#include <iostream>
#include <vector>

void split(std::vector<std::string>& dest, const std::string& str);

class User
{
private:
    int _fd;
    std::string _nickname;
    std::string _username;

    bool is_password_passed;
    bool is_nickname_passed;
    bool is_username_passed;

public:
    User(int fd);

    bool is_authorized( void );
    void set_nickname( std::string nickname );
    void set_username( std::string username );

    bool get_nickname_passed();
    bool get_username_passed();

    std::string get_nickname( void );
    std::string get_username( void );

    void parse_command( std::string command );
    void authorize( std::vector<std::string> parsed );
};

#endif