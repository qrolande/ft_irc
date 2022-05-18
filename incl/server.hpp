#ifndef SERVER_HPP
# define SERVER_HPP

#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <iostream>
#include <vector>

# include "user.hpp"

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
    Server();
    Server(int port, std::string password);
    void start();
    void main_loop(sockaddr_in address);

};

#endif