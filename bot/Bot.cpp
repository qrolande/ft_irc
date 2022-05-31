#include "Bot.hpp"

void adam_sender(int fd, std::string message)
{
    send(fd, message.c_str(), strlen(message.c_str()), 0);
    send(fd, "\r\n", 2, 0);
}

Bot::Bot(std::string port, std::string pass):
        _name("bot"), _port(port), _pass(pass) { 
	sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(std::atoi(_port.c_str()));
    if (inet_aton("127.0.0.1", &address.sin_addr) == 0)
        Error("Incorrect ip address has been passed");
    if ((_BotSocket = socket(address.sin_family, SOCK_STREAM, 0)) == -1){
        Error("Socket troubles");
    }
    if (connect(_BotSocket, (sockaddr*)&address, sizeof(address)) == -1){
        std::cout << errno << " ";
        Error("cannot connect to server");
    }
    fcntl(_BotSocket, F_SETFL, O_NONBLOCK);
    _auth_confirmed = false;
}

void Bot::start(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    _message = "PASS " + _pass + "\r\n"\
    "NICK " + _name + " " + std::to_string(ms) + "\r\n"\
    "USER bot bot bot :work work \r\n";
    while (true){
        message_sendler();
		get_buff();
        request_parser();
        execute();
    }     
}

void Bot::message_sendler(){
    if (!_message.empty()){
        adam_sender(_BotSocket, _message.c_str());
        _message.clear();
    }
}

void Bot::get_buff(){
    char buff[BUFFER_SIZE];
	Bytes = 0;
    bzero(&buff, sizeof(buff));
	if ((Bytes = recv(_BotSocket, &buff, BUFFER_SIZE - 1, 0)) >= 0)
		_new_buff += buff;
    if (Bytes == 0)
        Error("server is not available!");
}

std::string Bot::getRequest() {
    std::string::size_type index = _new_buff.find("\n") + 1;
    std::string request = _new_buff.substr(0, index);
    _new_buff.erase(0, index);
    return request;
}

void Bot::request_parser() {
	request = getRequest();
    if (request.find("PRIVMSG") != std::string::npos || request.find("NOTICE") != std::string::npos){
        _request_flag = "PRIVMSG";
    } else if (request.find("Welcome to the IRCServ, bot") != std::string::npos){
        _auth_confirmed = true;
        std::cout << "Connection to server is successful" << std::endl;
        _request_flag = "SKIP";
	} else if (request.find(":ircserv 464") != std::string::npos || request.find(":ircserv 451") != std::string::npos || request.find("You have not registered") != std::string::npos){
        Error("incorrect password!");
    } else
        _request_flag = "SKIP";
}

void Bot::execute() {
	if (_request_flag == "SKIP")
        return;
    else {
        request.erase(0, request.find(":") + 1);
        std::string name = request.substr(0, request.find(" "));
        request.erase(0, request.find(":") + 1);
        std::string req = request.substr(0, request.find("\r"));
        if (req.size() == 0) {
            _message = "PRIVMSG " + name + " :available commands 'time', 'banana', '?'";
        }
        else if (req == "banana") {
            struct timeval tp;
            gettimeofday(&tp, NULL);
            long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
            unsigned int res = ms % 35;
            _message = "PRIVMSG " + name + " :your banana size is " + std::to_string(res);
            if (res < 10)
                _message += ", oh poor guy!";
            else if (res > 28)
                _message += ", baseball bat killer!";
            else
                _message += ", regular normal guy!";
        }
        else if (req == "time") {
            time_t now = time(0);
            char* dt = ctime(&now);
            _message = "PRIVMSG " + name + " :time now is " + dt;
        }
        else if (req.find("?")) {
            struct timeval tp;
            gettimeofday(&tp, NULL);
            long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
            unsigned int res = ms % 3;
            std::cout << res << std::endl;
            if (res > 1)
                _message = "PRIVMSG " + name + " :Yes";
            else
                _message = "PRIVMSG " + name + " :No";
        }
    }
}