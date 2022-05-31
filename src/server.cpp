#include "../incl/global.hpp"

Server::Server(int port) : _port(port), _count_connects(0) {}

std::string Server::password = "";

void Server::start()
{
	sockaddr_in address;
 
	if ((_listening = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
		Error("socket failed", this);

	int opt = 1;
	if (setsockopt(_listening, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
		Error("setsockopt failed", this);

	address.sin_family = AF_INET;  
	address.sin_addr.s_addr = INADDR_ANY;  
	address.sin_port = htons(_port);
		  
	if (bind(_listening, (sockaddr *)&address, sizeof(address))<0)  
		Error("bind failed", this);

	if (listen(_listening, SOMAXCONN) < 0)  
		Error("listen failed", this);
	fcntl(_listening, F_SETFL, O_NONBLOCK);
	std::cout << "Server is running..." << std::endl;
	main_loop(address);
}

void Server::main_loop(sockaddr_in address)
{
	int sd, new_socket;
	int addrlen = sizeof(address);
	fd_set readfds;
	char buffer[BUFFER_SIZE];
	while(true)  
	{   
		FD_ZERO(&readfds);  
		FD_SET(_listening, &readfds);  
		_max_fd = _listening;  
		for (unsigned int i = 0, size = client_socket.size() ; i < size; i++)  
		{
			sd = client_socket[i];  
 
			if(sd > 0)  
				FD_SET(sd , &readfds);  

			if(sd > _max_fd)  
				_max_fd = sd;  
		}
		
		sd = select(_max_fd + 1 , &readfds , NULL , NULL , NULL);
		if (sd < 0)
			Error("select failed", this);
		else if (sd == 0)
			continue;
			 
		if (FD_ISSET(_listening, &readfds))  
		{
			if ((new_socket = accept(_listening, 
					(sockaddr *)&address, (socklen_t*)&addrlen))<0)  
				Error("accept failed", this);

			fcntl(new_socket, F_SETFL, O_NONBLOCK);

			printf("NEW CONNECTION [FD%d]\n", new_socket);  
				 
			bool t = false;
			User *user = new User(new_socket, this);
			for (unsigned int i = 0, size = client_socket.size(); i < size; i++)  
			{  
				if( client_socket[i] == 0 )  
				{
					client_socket[i] = new_socket;
					delete clients[i];
					clients[i] = nullptr;
					clients[i] = user;
					t = true;
					break;  
				}
			}
			if (!t)
			{
				client_socket.push_back(new_socket);
				clients.push_back(user);
			}
			_count_connects++;
		}

		for (unsigned int i = 0, valread, size = client_socket.size(); i < size; i++)  
		{
			User *user = clients[i];
			memset(buffer, '\0', BUFFER_SIZE);
			if (FD_ISSET( user->get_fd() , &readfds))  
			{
				if ((valread = recv( user->get_fd() , buffer, BUFFER_SIZE, 0 )) <= 0)
				{
					std::vector<std::string> vec;
					user->quit_cmd(vec);
				}
				else
				{
					std::cout << buffer << std::endl;
					user->parse_command(buffer);
				}
			}
		}
	}
}

int Server::is_nickname_available( std::string nickname )
{
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		if (client_socket[i] != 0)
		{
			if (clients[i]->get_nickname() == nickname)
				return i;
		}
	}
	return -1;
}

int Server::is_channel_available( std::string channel_name )
{
	for (unsigned int i = 0; i < channels.size(); i++)
	{
		if (channels[i] != nullptr)
		{
			if (channels[i]->get_channel_name() == channel_name)
				return i;
		}
	}
	return -1;
}


bool Server::is_username_available( std::string username )
{
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		if (client_socket[i] != 0)
		{
			if (clients[i]->get_username() == username)
				return false;
		}
	}
	return true;
}

int Server::get_count_connects( void )
{
	return _count_connects;
}

void Server::remove_one_connect( void )
{
	--_count_connects;
}

void Server::remove_channel( std::string channel_name )
{
   for (unsigned int i = 0; i < channels.size(); i++)
    {
        if (channels[i]->get_channel_name() == channel_name)
        {
			Channel *tmp = channels[i];
			channels.erase(channels.begin() + i);
            delete tmp;
			break;
        }
    }
}

void Server::give_operator( void )
{
	unsigned int i = 0;
	if (users.size() != 0 && users[0]->get_nickname() == "bot")
		++i;
	if (users.size() == i || users[i]->has_mode(UserOper))
		return;
	users[i]->set_mode(UserOper, "", "");
}

void Server::exiting( void )
{
	for (unsigned int i = 0; i < client_socket.size(); i++)
		if (client_socket[i] != 0)
			close(client_socket[i]);
	for (unsigned int i = 0; i < clients.size(); i++)
		if (clients[i] != nullptr)
			delete clients[i];
	close(_listening);
}