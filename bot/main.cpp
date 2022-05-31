#include "Bot.hpp"

void Error(std::string msg)
{
    std::cout << "Error: " << msg << "\n";
    exit(1);
}

int main(int argc, char *argv[]){
    if (argc != 3){
        std::cerr << "Usage: <" << argv[0] << "> <port> <pass>\n";
        return (1);
    }
    for (int i = 0; argv[1][i] != '\0'; i++){
        if (std::isdigit(argv[1][i]) == 0)
            Error("port must be numeric!");
    }
    if (1024 > std::atoi(argv[1]) || std::atoi(argv[1]) > 65535)
        Error("wrong port number. Please enter the correct port number in range [1024, 65535]");
    Bot newBot(argv[1], argv[2]);
    newBot.start();
    return (0);
}
