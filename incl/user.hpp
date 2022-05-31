#ifndef USER_HPP
# define USER_HPP

#include "global.hpp"

class Server;
class Channel;

enum UserMode {
	UserNone = 0,					// 0000000000
	invisibility = 1 << 0,          // 0000000001
	silence = 1 << 1,          		// 0000000010
	UserOper = 1 << 2,				// 0000000100
	wallopsOff = 1 << 3,       		// 0000001000
};

class User
{
private:
	int									_fd;

	int									_modes;

	std::string							_nickname;
	std::string							_username;
	std::string							_realname;

	std::string							away_message;

	bool								is_password_passed;
	bool								is_nickname_passed;
	bool								is_username_passed;
	bool								is_away;
	bool								welcomed;

	void 								work_with_command(std::vector<std::string> parsed);

	void 								welcome();

	std::vector<Channel *>				channels;

	void 								buffer_copy( char command[] );

	Server								*server;

	typedef void						(User::*commandPtr)(std::vector<std::string>);
	std::map<std::string, commandPtr>	functions;

public:

	User(int fd, Server *_server);

	User							& operator=(const User &);

	char							buffer[BUFFER_SIZE];

	bool							is_authorized( void );
	void 							set_nickname( std::string nickname );
	void 							set_username( std::string username );

	bool							get_nickname_passed( void );
	bool							get_username_passed( void );

	std::string						get_nickname( void );
	std::string						get_username( void );
	std::string						get_fullname( void );
	int								get_fd( void );

	int								is_on_channel( std::string channel_name );

	void 							parse_command( char command[] );
	void 							authorize( std::vector<std::string> parsed );

	
	void 							password_cmd( std::vector<std::string> cmd );
	void 							nickname_cmd( std::vector<std::string> cmd );
	void 							username_cmd( std::vector<std::string> cmd );
	void 							quit_cmd( std::vector<std::string> );
	void 							privmsg_cmd( std::vector<std::string> cmd );
	void 							ping_cmd( std::vector<std::string> cmd );
	void 							away_cmd( std::vector<std::string> cmd );
	void 							who_cmd( std::vector<std::string> ) {}
	void 							part_cmd( std::vector<std::string> cmd );
	void 							topic_cmd( std::vector<std::string> cmd );
	void 							kick_cmd( std::vector<std::string> cmd );
	void 							invite_cmd( std::vector<std::string> cmd );
	void							wallops_cmd( std::vector<std::string> cmd );

	void							mode_cmd( std::vector<std::string> cmd );
	void							mode_channel( std::vector<std::string> cmd, int i );
	void							mode_user( std::vector<std::string> cmd, int i );

	void							join_cmd( std::vector<std::string> cmd );
	void							joining(Channel *channel);


	void 							set_mode(UserMode, std::string nick, std::string change);
	void 							unset_mode(UserMode, std::string nick, std::string change);
	bool							has_mode(UserMode) const;
	std::string						show_mode() const;
};

#endif
