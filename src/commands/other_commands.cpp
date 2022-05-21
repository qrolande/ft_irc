#include "../../incl/global.hpp"

void User::ping_cmd( std::vector<std::string> cmd )
{
    if (cmd.size() == 1)
        adam_sender(_fd, ERR_NOORIGIN(_nickname));
    else
        adam_sender(_fd, "PONG " + cmd[1] + " " + _nickname);
}
