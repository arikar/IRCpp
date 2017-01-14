#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H
#include <string>
#include <vector>

class IRCMessage
{
public:
    enum IRCCommand {
        PASS, NICK, USER, OPER, MODE, SERVICE,QUIT, SQUIT,  // Connection
        JOIN, PART, TOPIC, NAMES, LIST, INVITE, KICK,       // Channel
        PRIVMSG, NOTICE,                                    // Messages
    };

    IRCMessage(std::string prefix, IRCCommand type, std::vector<std::string> *params, std::string _trailing_param);
    IRCMessage(std::string prefix, int id, std::vector<std::string> *params, std::string _trailing_param);

    std::string                 _prefix;
    IRCCommand                  _command_type;
    int                         _command_id;
    std::vector<std::string>    _params;
    std::string                 _trailing_param;
private:

};

#endif // IRCMESSAGE_H
