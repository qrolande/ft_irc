#include "../incl/server.hpp"
#include <cstring>
#include <vector>

#define BUFFER_SIZE 4096

static int	wordcount(char const *s, char c)
{
	int	ctr;
	int	i;
	int	check;

	i = 0;
	ctr = 0;
	while (s[i])
	{
		check = 0;
		while (s[i] != c && s[i])
		{
			++check;
			++i;
		}
		if ((s[i] == c || !s[i]) && check)
			++ctr;
		if (s[i] == '\0')
			break ;
		++i;
	}
	return (ctr);
}

static int	symbcount(char const *s, char c, int ptr)
{
	int	ctr;

	ctr = 0;
	while (s[ptr] != c && s[ptr])
	{
		++ctr;
		++ptr;
	}
	return (ctr);
}

static void	*ft_free(char **split, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(split[i]);
		++i;
	}
	free(split);
	return (NULL);
}

static char	**ft_put(char const *s, char c, char **split, int words)
{
	int	wordctr;
	int	ptr;
	int	length;
	int	i;

	ptr = 0;
	wordctr = 0;
	while (wordctr < words)
	{
		while (s[ptr] == c)
			++ptr;
		length = symbcount(s, c, ptr);
		split[wordctr] = (char *)malloc(sizeof(char) * (length + 1));
		i = 0;
		while (s[ptr] != c && s[ptr])
			split[wordctr][i++] = s[ptr++];
		split[wordctr++][i] = '\0';
	}
	split[wordctr] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**split;

	if (!s)
		return (NULL);
	words = wordcount(s, c);
	split = (char **)malloc(sizeof(char *) * (words + 1));
	if (!split)
		return (NULL);
	split = ft_put(s, c, split, words);
	return (split);
}

Server::Server() : _port(8888), _password(""), _count_connects(0) {}
Server::Server(int port, std::string password) : _port(port), _password(password), _count_connects(0) {}

void Server::start()
{
    sockaddr_in address;

    //create a master socket 
    if( (_listening = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
     
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this
    int opt = 1;
    if( setsockopt(_listening, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }

    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(_port);
         
    //bind the socket to localhost port 8888 
    if (bind(_listening, (sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);
    }  
         
    //try to specify maximum of 3 pending connections for the master socket ??? 
    if (listen(_listening, SOMAXCONN) < 0)  
    {  
        perror("listen");
        exit(EXIT_FAILURE);  
    }
    main_loop(address);
}

void Server::main_loop(sockaddr_in address)
{
    int sd, activity, new_socket;
    int addrlen = sizeof(address);
        //set of socket descriptors 
    fd_set readfds;
    char buffer[BUFFER_SIZE];
         
    //a message 
    char *message = "Connection successful\r\n";
    while(true)  
    {  
        //clear the socket set 
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(_listening, &readfds);  
        _max_fd = _listening;  
             
        //add child sockets to set 
        for (int i = 0 ; i < client_socket.size(); i++)  
        {  
            //socket descriptor 
            sd = client_socket.at(i);  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET(sd , &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > _max_fd)  
                _max_fd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select(_max_fd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(_listening, &readfds))  
        {  
            if ((new_socket = accept(_listening, 
                    (sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                  (address.sin_port));  
           
            //send new connection greeting message 
            if(send(new_socket, message, strlen(message), 0) != strlen(message))  
            {  
                perror("send");  
            }
                 
            bool t = false;
            User *user = new User(new_socket);
            for (int i = 0; i < client_socket.size(); i++)  
            {  
                if( client_socket[i] == 0 )  
                {
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);
                    t = true;
                    clients[i] = *user;
                    break;  
                }
            }
            if (!t)
            {
                client_socket.push_back(new_socket);
                clients.push_back(*user);
                printf("Adding to list of sockets as %lu\n" , client_socket.size() - 1);
            }
            delete user;
            _count_connects++;
        }  
             
        //else its some IO operation on some other socket
        for (int i = 0, valread; i < client_socket.size(); i++)  
        {  
            sd = client_socket[i];
            // std::cout << sd << std::endl; 
            memset(buffer, 0, BUFFER_SIZE);
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, BUFFER_SIZE - 1)) == 0)  
                {
                    printf("fd %d is disconnected\n", sd);
                    // delete &clients[sd - 4];
                    //Close the socket and mark as 0 in list for reuse 
                    close(sd);  
                    client_socket[i] = 0;
                    _count_connects--;
                }  
                     
                //Echo back the message that came in 
                else 
                {
                    User *user = &clients[sd - 4];
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';
                    // user->parse_command(buffer);
                    //  else {
                        char **strings = ft_split(buffer, ' ');
                        send(atoi(strings[0]), strings[1], strlen(strings[1]), 0);
                    // }
                    // std::cout << user->get_nickname().substr(0, user->get_nickname().length() - 2) << " send: " << buffer << std::endl;
                    // send(sd , buffer , strlen(buffer) , 0);
                }  
            }  
        }
    }
}  