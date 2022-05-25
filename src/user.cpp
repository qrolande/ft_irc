#include "../incl/global.hpp"

User::User(int fd, Server *_server) : _fd(fd), server(_server) {
    _nickname = "user" + std::to_string(fd);
    is_password_passed = false;
    is_nickname_passed = false;
    is_username_passed = false;
    memset(this->buffer, '\0', BUFFER_SIZE);
}

User & User::operator=(const User & src)
{
    if (this != &src)
    {
        this->server = src.server;
        this->_fd = src._fd; 
    }
    return *this;
}

bool User::is_authorized( void )
{
    return is_nickname_passed && is_password_passed && is_username_passed ? true : false;
}

void User::set_nickname(std::string nickname)
{
    this->_nickname = nickname;
    this->is_nickname_passed = true;
}

void User::set_username(std::string username)
{
    this->_username = username;
    this->is_username_passed = true;
}

bool User::get_nickname_passed()
{
    return this->is_nickname_passed;
}

bool User::get_username_passed()
{
    return this->is_username_passed;
}

std::string User::get_nickname( void )
{
    return this->_nickname;
}

std::string User::get_username( void )
{
    return this->_username;
}

void User::buffer_copy( char command[] )
{
    int i = 0;
    for (; this->buffer[i] != '\0'; i++);
    for (int j = 0; command[j] != '\0'; j++, i++)
        this->buffer[i] = command[j];
    this->buffer[i] = '\0';
}

void User::parse_command( char command[] )
{
    buffer_copy(command);
    for (int j = 0; buffer[j];)
    {
        int i = contains_new_line(this->buffer + j);
        if (i == -1)
            return;
        this->buffer[i + j] = '\0';
        std::vector<std::string> parsed;
        split(parsed, this->buffer + j);
        if (parsed.size() > 0)
            work_with_command(parsed);
        for (j = i + j + 1; buffer[j] == '\r' || buffer[j] == '\n'; j++);
        parsed.clear();
    }
    memset(this->buffer, '\0', BUFFER_SIZE);
}

void User::work_with_command( std::vector<std::string> parsed )
{
    if (!is_authorized())
    {
        authorize(parsed);
        if (is_authorized())
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
        }
    }
    else
    {
        if (parsed[0] == "QUIT")
            quit_cmd(parsed);
        else if (parsed[0] == "PRIVMSG")
            privmsg_cmd(parsed);
        else if (parsed[0] == "NICK")
            nickname_cmd(parsed);
        else if (parsed[0] == "PING")
            ping_cmd(parsed);
        else if (parsed[0] == "AWAY")
            away_cmd(parsed);
        else if (parsed[0] == "JOIN")
            join_cmd(parsed);



        else
            adam_sender(_fd, ERR_UNKNOWNCOMMAND(_nickname, parsed[0]));
    }
}

//добавить ошибки
void User::authorize( std::vector<std::string> parsed )
{
    if (parsed[0] == "PASS")
        password_cmd(parsed);
    else if (parsed[0] == "NICK")
        nickname_cmd(parsed);
    else if (parsed[0] == "USER")
        username_cmd(parsed);
    else
        adam_sender(_fd, ERR_UNKNOWNCOMMAND(_nickname, parsed[0]));
}

int User::get_fd( void )
{
    return this->_fd;
}
