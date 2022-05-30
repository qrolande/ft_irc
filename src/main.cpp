#include "../incl/global.hpp"

int main(int argc, char **argv)
{
    if ( argc != 3)
    {
        perror("usage: irc port password");
        return -1;
    }
    Server::password = std::string(argv[2]);
    for (int i = 0; argv[1][i] != '\0'; i++){
        if (std::isdigit(argv[1][i]) == 0)
            Error("port must be numeric!");
    }
    if (1024 > std::atoi(argv[1]) || std::atoi(argv[1]) > 65535)
        Error("wrong port number. Please enter the correct port number in range [1024, 65535]");
    Server server = Server(atoi(argv[1]));
    server.start();
}
