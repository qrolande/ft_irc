#include "../../incl/global.hpp"

void User::ping_cmd( std::vector<std::string> cmd )
{
    if (!is_authorized())
        adam_sender(_fd, ERR_NOTREGISTERED(_nickname));
    else if (cmd.size() == 1)
        adam_sender(_fd, ERR_NOORIGIN(_nickname));
    else
        adam_sender(_fd, "PONG " + cmd[1] + " " + _nickname);
}
