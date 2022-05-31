#pragma once

# include "../incl/global.hpp"

class Bot {
private:
	std::string 	_name;
    std::string 	_port;
    std::string 	_pass;
	std::string 	_message;
	std::string 	_new_buff;
	std::string 	_request_flag;
	std::string 	request;
	bool        	_auth_confirmed;
	int 			_BotSocket;
	int 			Bytes;
public:
	Bot(std::string, std::string);

	void			start();
	void			message_sendler();
	void			get_buff();
	void			request_parser();
	void			execute();
	std::string		getRequest();
};