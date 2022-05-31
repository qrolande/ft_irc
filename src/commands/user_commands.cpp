#include "../../incl/global.hpp"

void User::quit_cmd(std::vector<std::string>)
{
	std::vector<std::string> tmp;
	tmp.push_back("PART");
	while (channels.size() != 0)
	{
		tmp.push_back(channels[0]->get_channel_name()); //мб можно проще
		part_cmd(tmp);
		tmp.erase(tmp.begin() + 1);
	}
	printf("[FD%d] DISCONNECTED\n", _fd);
	for (unsigned int i = 0; i < server->client_socket.size(); i++)
		if (server->client_socket[i] == _fd)
			server->client_socket[i] = 0;
	close(_fd);
	server->remove_one_connect();
	for (unsigned int i = 0; i < server->users.size(); i++)
	{
		if (server->users[i] == this)
		{
			server->users.erase(server->users.begin() + i);
			break;
		}
	}
	server->give_operator();
}

void User::privmsg_cmd(std::vector<std::string> cmd)
{
	if (!is_authorized())
	{
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
		return;
	}
	int i;
	std::string command = cmd[0]; // ??
	split(cmd, -1);
	if (cmd[0][0] == ':')
		adam_sender(_fd, ERR_NORECIPIENT(_nickname, command));
	else if (cmd.size() == 1)
		adam_sender(_fd, ERR_NOTEXTTOSEND(_nickname));
	else if ((i = server->is_channel_available(cmd[0])) != -1)
	{
		if (!server->channels[i]->user_in_channel(_fd))
			adam_sender(_fd, ERR_NOTONCHANNEL(_nickname, cmd[0]));
		else if (command == "PRIVMSG")
			server->channels[i]->send_all(this, cmd[1][0] == ':' ? cmd[1].substr(1) : cmd[1], false);
		else
			server->channels[i]->send_all_n(this, cmd[1][0] == ':' ? cmd[1].substr(1) : cmd[1], false);
	}
	else if ((i = server->is_nickname_available(cmd[0])) == -1)
		adam_sender(_fd, ERR_NOSUCHNICK(_nickname, cmd[0]));
	else if (server->clients[i]->is_away)
	{
		if (command == "PRIVMSG")
			adam_sender(_fd, RPL_AWAY(_nickname, cmd[0], server->clients[i]->away_message));
	}
	else if (server->clients[i]->has_mode(silence) && command == "NOTICE")
		return;
	else
	{
		if (command == "PRIVMSG")
			adam_sender(server->clients[i]->_fd, RPL_PRIVMSG(_nickname,
				cmd[0], cmd[1][0] == ':' ? cmd[1].substr(1) : cmd[1]));
		else
			adam_sender(server->clients[i]->_fd, RPL_NOTICE(_nickname,
				cmd[0], cmd[1][0] == ':' ? cmd[1].substr(1) : cmd[1]));
	}
	
}

void User::away_cmd( std::vector<std::string> cmd )
{
	if (!is_authorized())
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
	else if (cmd.size() == 1)
	{
		adam_sender(_fd, RPL_UNAWAY(_nickname));
		is_away = false;
	}
	else
	{
		adam_sender(_fd, RPL_NOWAWAY(_nickname));
		is_away = true;
		away_message = cmd[1][0] == ':' ? cmd[1].substr(1) : cmd[1];
	}
}

void User::mode_cmd( std::vector<std::string> cmd )
{
	int i;
	if (cmd.size() == 1)
		adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
	else
	{
		split(cmd, -1);
		if ((i = server->is_channel_available(cmd[0])) != -1)
			mode_channel(cmd, i);
		else if ((i = server->is_nickname_available(cmd[0])) != -1)
			mode_user(cmd, i);
		else
		{
			if (cmd[0][0] == '#')
				adam_sender(_fd, ERR_NOSUCHCHANNEL(_nickname, cmd[0]));
			else
				adam_sender(_fd, ERR_NOSUCHNICK(_nickname, cmd[0]));
		}
	}
}

void User::mode_channel( std::vector<std::string> cmd, int i )
{
	Channel *channel = server->channels[i];
	bool mode;
	if (!channel->is_operator(_fd))
		adam_sender(_fd, ERR_CHANOPRIVSNEEDED(_nickname, cmd[0]));
	else if (is_valid_keys(cmd[1], true) && (cmd[1][0] == '+' || cmd[1][0] == '-'))
	{
		for (unsigned int i = 0; i < cmd[1].length(); i++)
		{
			mode = cmd[1][i] == '+' ? true : cmd[1][i] == '-' ? false : mode;
			if (cmd[1][i] == 'i')
				mode ? channel->set_mode(invite_only, get_fullname(), "+i")
					: channel->unset_mode(invite_only, get_fullname(), "-i");
			else if (cmd[1][i] == 't')
				mode ? channel->set_mode(protectedTopic, get_fullname(), "+t")
					: channel->unset_mode(protectedTopic, get_fullname(), "-t");
			else if (cmd[1][i] == 'l')
			{
				mode ? channel->set_mode(limited, get_fullname(), "+l:" + cmd[2])
					: channel->unset_mode(limited, get_fullname(), "-l");
				channel->set_limit(atoi(cmd[2].c_str()));
			}
			else if (cmd[1][i] == 'o')
			{
				if (cmd.size() == 2)
				{
					if (mode)
						channel->set_mode(oper, _nickname, "+o");
					else
						channel->unset_mode(oper, _nickname, "-o");
				}
				else
				{
					int l = server->is_nickname_available(cmd[cmd.size() - 1]);
					if (l != -1)
					{
						if (channel->has_mode(oper) && mode && !channel->is_operator(server->client_socket[l]))
							channel->add_operator(server->clients[l], _nickname);
						else if (channel->has_mode(oper) && !mode && channel->is_operator(server->client_socket[l]))
							channel->delete_operator(server->clients[l], _nickname);
						else
							adam_sender(_fd, ERR_CHANOPRIVSNEEDED2(_nickname, cmd[0]));
					}
					else
						adam_sender(_fd, ERR_NOSUCHNICK(_nickname, cmd[cmd.size() - 1]));
				}
			}
		}
	}
	else
		adam_sender(_fd, ERR_UMODEUNKNOWNFLAG(_nickname));

}

void User::mode_user( std::vector<std::string> cmd, int i )
{
	User *user = server->clients[i];
	bool mode;
	if (!has_mode(UserOper))
		adam_sender(_fd, ERR_NOPRIVILEGES(_nickname));
	else if (is_valid_keys(cmd[1], false) && (cmd[1][0] == '+' || cmd[1][0] == '-'))
	{
		for (unsigned int i = 0; i < cmd[1].length(); i++)
		{
			mode = cmd[1][i] == '+' ? true : cmd[1][i] == '-' ? false : mode;
			if (cmd[1][i] == 'i')
				mode ? user->set_mode(invisibility, get_fullname(), "+i")
					: user->unset_mode(invisibility, get_fullname(), "-i");
			else if (cmd[1][i] == 's')
				mode ? user->set_mode(silence, get_fullname(), "+s")
					: user->unset_mode(silence, get_fullname(), "+s");
			else if (cmd[1][i] == 'w')
				mode ? user->set_mode(wallopsOff, get_fullname(), "+w")
					: user->unset_mode(wallopsOff, get_fullname(), "-w");
			else if (cmd[1][i] == 'o')
				mode ? user->set_mode(UserOper, get_fullname(), "+o")
					: user->unset_mode(UserOper, get_fullname(), "-o");
		}
	}
	else
		adam_sender(_fd, ERR_UMODEUNKNOWNFLAG(_nickname));
}
