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
	fcntl(_listening, F_SETFL, O_NONBLOCK);
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
			printf("select error\n"); //ERROR
		else if (sd == 0)
			continue;
			 
		if (FD_ISSET(_listening, &readfds))  
		{
			if ((new_socket = accept(_listening, 
					(sockaddr *)&address, (socklen_t*)&addrlen))<0)  
			{
				perror("accept");   //ERROR
				exit(EXIT_FAILURE);
			}

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
		if (client_socket[i] != 0)
		{
			if (clients[i]->get_nickname() == nickname)
				return i;
		}
	return -1;
}

int Server::is_channel_available( std::string channel_name )
{
	for (unsigned int i = 0; i < channels.size(); i++)
		if (channels[i] != nullptr)
		{
			if (channels[i]->get_channel_name() == channel_name)
				return i;
		}
	return -1;
}


bool Server::is_username_available( std::string username )
{
	for (unsigned int i = 0; i < clients.size(); i++)
		if (client_socket[i] != 0)
		{
			if (clients[i]->get_username() == username)
				return false;
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
	std::vector<Channel *>::iterator start = channels.begin();
    while (start != channels.end())
    {
        if ((*start)->get_channel_name() == channel_name)
        {
			Channel *tmp = *start;
			channels.erase(start);
            delete tmp;
			break;
        }
    }
}

void Server::give_operator( void )
{
	if (users.size() == 0 || users[0]->has_mode(UserOper))
		return;
	users[0]->set_mode(UserOper);
	adam_sender(users[0]->get_fd(), RPL_YOUREOPER(users[0]->get_nickname()));
}
