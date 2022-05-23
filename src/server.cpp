#include "../incl/global.hpp"

Server::Server(int port) : _port(port), _count_connects(0) {}

std::string Server::password = "";

void Server::start()
{
	sockaddr_in address;
 
	if( (_listening = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
	{  
		perror("socket failed");  //ERROR
		exit(EXIT_FAILURE);  
	}

	int opt = 1;
	if( setsockopt(_listening, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		  sizeof(opt)) < 0 )  
	{
		perror("setsockopt");  //ERROR
		exit(EXIT_FAILURE);  
	}

	address.sin_family = AF_INET;  
	address.sin_addr.s_addr = INADDR_ANY;  
	address.sin_port = htons(_port);
		  
	if (bind(_listening, (sockaddr *)&address, sizeof(address))<0)  
	{  
		perror("bind failed");  //ERROR
		exit(EXIT_FAILURE);
	}

	if (listen(_listening, SOMAXCONN) < 0)  
	{  
		perror("listen");   //ERROR
		exit(EXIT_FAILURE);  
	}
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
	   
		if (select(_max_fd + 1 , &readfds , NULL , NULL , NULL) < 0)
		{  
			printf("select error"); //ERROR
		}  
			 
		if (FD_ISSET(_listening, &readfds))  
		{
			if ((new_socket = accept(_listening, 
					(sockaddr *)&address, (socklen_t*)&addrlen))<0)  
			{
				perror("accept");   //ERROR
				exit(EXIT_FAILURE);
			}

			printf("NEW CONNECTION [FD%d]\n", new_socket);  
				 
			bool t = false;
			User *user = new User(new_socket, this);
			for (unsigned int i = 0, size = client_socket.size(); i < size; i++)  
			{  
				if( client_socket[i] == 0 )  
				{
					client_socket[i] = new_socket;  
					t = true;
					clients.insert(clients.begin() + i, *user);
					break;  
				}
			}
			if (!t)
			{
				client_socket.push_back(new_socket);
				clients.push_back(*user);
			}
			delete user;
			_count_connects++;
		}

		for (unsigned int i = 0, valread, size = client_socket.size(); i < size; i++)  
		{
			User *user = &clients[i];
			memset(buffer, 0, BUFFER_SIZE);
			if (FD_ISSET( user->get_fd() , &readfds))  
			{
				if ((valread = recv( user->get_fd() , buffer, BUFFER_SIZE, 0 )) <= 0)  
				{
					printf("[FD%d] DISCONNECTED\n", user->get_fd());
					close(user->get_fd());  
					client_socket[i] = 0;
					_count_connects--;
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
		if (client_socket[i] != 0)
		{
			if (clients[i].get_nickname() == nickname)
				return i;
		}
	return -1;
}

bool Server::is_username_available( std::string username )
{
	for (unsigned int i = 0; i < clients.size(); i++)
		if (client_socket[i] != 0)
		{
			if (clients[i].get_username() == username)
				return false;
		}
	return true;
}

int Server::get_count_connects( void )
{
	return _count_connects;
}
