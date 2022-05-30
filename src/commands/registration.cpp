#include "../../incl/global.hpp"

void User::password_cmd(std::vector<std::string> cmd)
{
    if (is_password_passed)
        adam_sender(_fd, ERR_ALREADYREGISTRED(_nickname));
    else if (cmd.size() == 1)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else if (cmd.size() != 2 || !(cmd[1] == Server::password || (cmd[1][0] == ':' && cmd[1].substr(1) == Server::password)))
        adam_sender(_fd, ERR_PASSWDMISMATCH(_nickname));
    else
        is_password_passed = true;
}

void User::nickname_cmd(std::vector<std::string> cmd)
{
    split(cmd, -1);
    if (!is_password_passed)
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
    else if (cmd.size() == 0)
        adam_sender(_fd, ERR_NONICKNAMEGIVEN(_nickname));
    else if (!is_nickname_valid(cmd[0]))
        adam_sender(_fd, ERR_ERRONEUSNICKNAME(_nickname, cmd[0]));
    else if (server->is_nickname_available(cmd[0]) != -1)
        adam_sender(_fd, ERR_NICKNAMEINUSE(_nickname, cmd[0]));
    else if (cmd[0] == "bot")
    {
       if (cmd.size() == 2)
       {
            struct timeval tp;
            gettimeofday(&tp, NULL);
            long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
            long bot_time = atol(cmd[1].c_str());
            if (ms - bot_time <= 100)
            {
                this->_nickname = cmd[0];
                this->is_nickname_passed = true;
            }
            else
                adam_sender(_fd, ERR_ERRONEUSNICKNAME(_nickname, cmd[0]));
       }
        else
                adam_sender(_fd, ERR_ERRONEUSNICKNAME(_nickname, cmd[0]));
    }
    else
    {
        this->_nickname = cmd[0];
        this->is_nickname_passed = true;
    }
}

void User::username_cmd(std::vector<std::string> cmd)
{
    if (!is_password_passed)
    {
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
        return;
    }
    split(cmd, 4);
    if (cmd.size() != 4)
        adam_sender(_fd, ERR_NEEDMOREPARAMS(_nickname, cmd[0]));
    else if (!server->is_username_available(cmd[0]))
        adam_sender(_fd, ERR_ALREADYREGISTRED(cmd[0]));
    else
    {
        this->_username = cmd[0];
        this->_realname = cmd[3][0] == ':' ? cmd[3].substr(1) : cmd[3];
        is_username_passed = true;
    }
}

void User::welcome( void )
{
    printf("NEW USER! [FD%d] NICKNAME: [%s]\n", _fd, _nickname.c_str());
    adam_sender(_fd, RPL_MOTDSTART(_nickname));
    adam_sender(_fd, RPL_MOTD(_nickname, "⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠏⢀⣀⣤⣤⣤⣤⣤⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⣿⣿⣿⣿⣿⣿⡿⣿⣴⢶⣶⣿⣟⣶⣿⣭⠿⠦⠤⠽⣷⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⣿⣿⣿⣿⡿⢫⣿⢋⣠⣿⣿⡶⢻⡏⠀⠀⠀⠀⠀⠀⠀⠉⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⣿⣿⡿⠋⠈⣸⣿⣿⣿⡿⠿⠀⠈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⢯⠋⠈⠀⣴⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠒⠀⠀⢰⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⢼⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⢀⡀⠤⠤⠤⣀⠀⢀⡀⠤⠤⠤⣀⣱⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⢸⣿⣿⣿⣿⣿⠀⠀⠀⢀⡖⠁⠀⠀⠀⠀⠀⠱⡏⠀⠀⠀⠀⠈⠱⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠈⣿⣿⣿⣿⣿⡆⠀⠀⢸⠀⠀⠴⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠶⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⢀⡴⠋⠉⢹⡶⠶⢤"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣸⢻⡜⡄⠀⠀⠀⠀⠀⠀⢀⠶⠒⠒⠀⠐⣄⡼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⠟⠛⠓⠶⣏⠀⠀⣀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⢸⢧⣿⣿⣿⣿⣿⡿⠀⠷⠙⠲⠄⠀⡀⠠⠔⠁⠀⠀⠀⢀⣠⡇⡧⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⡇⠀⡀⠀⠀⠈⢦⠞⠁"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⢸⡈⢻⣿⣿⣿⡿⠧⣄⠀⠀⠀⢀⡴⠖⠒⠚⠛⠛⠛⠛⠉⠀⠈⠙⠦⣀⣠⣀⠀⠀⠀⠀⢰⣿⣿⣿⣿⠄⠀⠈⢢⡀⠠⢾⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠀⣴⣾⣿⣿⣿⢲⡶⡄⠀⢀⡶⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢁⠀⢸⠀⠀⠀⠀⣸⣿⣿⠟⠋⠀⠀⠀⡎⠀⠀⠈⠉⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⡀⠀⠀⠘⣿⣿⣿⣿⣿⣦⣤⡴⠀⣾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡷⢰⠃⠀⠀⣠⣾⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠙⢦⡀⠀⠈⠛⢻⣿⣿⣿⣿⡇⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢦⣀⣠⠎⢀⣤⣾⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠞⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠈⠳⢄⠀⢸⣿⣿⣿⣿⡇⠀⠙⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠃⠀⣠⣴⣿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡰⠁⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠀⠈⠙⢾⣿⣿⣿⣿⡇⠀⠀⠈⠳⠤⣀⡀⠀⠀⢀⣀⠤⡖⠋⢀⡤⠾⠿⣏⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠎⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠀⠀⠀⠀⢨⠇⠙⢿⠷⠖⠒⠛⠓⠒⠚⠛⠯⡉⠉⠀⠀⡷⠶⠯⡁⠀⠀⠀⠙⠢⡀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠜⠁⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠀⠀⠀⢀⡌⠀⠀⠀⠳⡀⠀⠀⠀⠀⠀⡌⠀⠙⢆⠀⠀⡧⠂⠀⢡⠀⠀⠀⠀⠀⠈⠢⡀⠀⠀⠀⠀⢀⡔⠁⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠀⠀⢀⠌⠀⠀⠀⠀⠀⠰⡀⠀⠀⠀⢰⠃⠀⠀⠈⠣⡴⠉⠡⡀⠈⡆⠀⠀⠀⠀⠀⠀⠘⠄⠀⢀⡴⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠀⠀⠀⠀⡠⠊⠀⠀⠀⠀⠀⠀⠀⢩⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠱⡀⠁⠀⠀⠀⠀⠀⠀⠀⠈⣶⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⣷⣄⣠⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢡⠀⠀⠀⠀⠀⠀⠀⠀⢠⠎⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⣿⣎⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡆⠀⠀⠀⠀⢀⡠⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⣿⣿⣿⣶⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⢤⡤⠴⠒⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_MOTD(_nickname, "⠻⠿⢿⣿⣿⣿⣿⠏⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"));
    adam_sender(_fd, RPL_ENDOFMOTD(_nickname));
    adam_sender(_fd, RPL_WELCOME(_username));
    welcomed = true;
    server->users.push_back(this);
    server->give_operator();
}
