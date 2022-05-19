#ifndef SERVER_HPP
# define SERVER_HPP

# include "global.hpp"

class Server
{   
private:
    int         _port;
    int         _listening;
    int         _count_connects;
    int         _max_fd;
    std::string _password;
    std::vector<int> client_socket;
    std::vector<User> clients;
public:
    static std::string password;

    Server();
    Server(int port, std::string password);
    void start();
    void main_loop(sockaddr_in address);

};

#endif