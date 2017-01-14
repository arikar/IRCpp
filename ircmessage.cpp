#include "ircmessage.h"

IRCMessage::IRCMessage(std::string prefix, IRCCommand type, std::vector<std::string> *params, std::string trailing_param) :
    _params(*params)
{
    _prefix = prefix;
    _command_type = type;
    _trailing_param = trailing_param;
    _command_id = -1;
}

IRCMessage::IRCMessage(std::string prefix, int type, std::vector<std::string> *params, std::string trailing_param) :
    _params(*params)
{
    _prefix = prefix;
    _command_id = type;
    _trailing_param = trailing_param;
}
