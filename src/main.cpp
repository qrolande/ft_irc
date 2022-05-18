#include "../incl/server.hpp"

int main(int argc, char **argv)
{
    Server server;
    if (argc != 1 && argc != 3)
    {
        perror("usage: irc port password");
        return -1;
    }
    if (argc == 3)
        server = Server(atoi(argv[1]), std::string(argv[2]));
    server.start();
}
