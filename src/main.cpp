#include "../incl/global.hpp"

int main(int argc, char **argv)
{
    if (argc != 1 && argc != 3)
    {
        perror("usage: irc port password");
        return -1;
    }
    Server::password = std::string(argv[2]);
    Server server;
    if (argc == 3)
        server = Server(atoi(argv[1]));
    server.start();
}
