#include "../incl/global.hpp"

int main(int argc, char **argv)
{
    Server::password = std::string(argv[2]);
    if (argc != 1 && argc != 3)
    {
        perror("usage: irc port password");
        return -1;
    }
    Server server;
    if (argc == 3)
        server = Server(atoi(argv[1]), std::string(argv[2]));
    server.start();
}
