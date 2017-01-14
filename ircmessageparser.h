#ifndef IRCMESSAGEPARSER_H
#define IRCMESSAGEPARSER_H
#include <ircmessage.h>
#include <QByteArray>
#include <map>
#include <string>

class IRCMessageParser
{
public:
    IRCMessage* parseIRCMessage(std::string *message);
    IRCMessageParser();

    std::map<std::string, IRCMessage::IRCCommand> *translation_table;
    std::map<IRCMessage::IRCCommand, std::string> *reverse_translation_table;
private:

};

#endif // IRCMESSAGEPARSER_H
