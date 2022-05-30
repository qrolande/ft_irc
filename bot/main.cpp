#include "../incl/global.hpp"
#include "Bot.hpp"

int main(int argc, char *argv[]){
    if (argc != 3){
        std::cerr << "Usage: <" << argv[0] << "> <port> <pass>\n";
        return (1);
    }
    Bot newBot(argv[1], argv[2]);
    newBot.start();
    return (0);
}
