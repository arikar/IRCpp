#include "ircmessageparser.h"
/*
PASS, NICK, USER, OPER, MODE, SERVICE,QUIT, SQUIT,  // Connection
JOIN, PART, TOPIC, NAMES, LIST, INVITE, KICK,       // Channel
PRIVMSG, NOTICE,                                    // Messages
*/
IRCMessageParser::IRCMessageParser()
{
    translation_table = new std::map<std::string, IRCMessage::IRCCommand>();
    (*translation_table)["PASS"]       = IRCMessage::PASS;
    (*translation_table)["NICK"]       = IRCMessage::NICK;
    (*translation_table)["USER"]       = IRCMessage::USER;
    (*translation_table)["OPER"]       = IRCMessage::OPER;
    (*translation_table)["MODE"]       = IRCMessage::MODE;
    (*translation_table)["SERVICE"]    = IRCMessage::SERVICE;
    (*translation_table)["QUIT"]       = IRCMessage::QUIT;
    (*translation_table)["SQUIT"]      = IRCMessage::SQUIT;
    (*translation_table)["JOIN"]       = IRCMessage::JOIN;
    (*translation_table)["PART"]       = IRCMessage::PART;
    (*translation_table)["NOTICE"]     = IRCMessage::NOTICE;
    (*translation_table)["PRIVMSG"]     = IRCMessage::PRIVMSG;
    reverse_translation_table = new std::map<IRCMessage::IRCCommand, std::string>();
    for (auto const &ent: *translation_table)
    {
        (*reverse_translation_table)[ent.second] = ent.first;
    };

}

IRCMessage* IRCMessageParser::parseIRCMessage(std::string *message)
{
    std::string prefix, command, trailing_param;
    std::vector<std::string> *params = new std::vector<std::string>();
    int offset = 0, p1;
    /* Check if ':' is present,  is present, if yes, parse <prefix>. */
    if (message->at(offset) == ':')
    {
        p1 = offset++;
        while (message->at(offset++) != ' ');
        prefix = message->substr(p1, offset - p1 - 1);
    }

    /* Read <command> type */
    p1 = offset;
    while(message->at(offset++) != ' ');
    offset--;
    command = message->substr(p1, offset - p1);

    Q_ASSERT(message->at(offset) == ' ');
    /* Parse the params */
    while(message->at(offset) != 0x0D) // Run until CR(0x0D)\LF(0x0A)
    {
        // Consume space
        offset++;
        p1 = offset;
        if (message->at(offset) == ':' || params->size() == 14)
        {
            // 'trailing' param either starts with a colon, or when the maximum number of params has been read.
            while(message->at(offset) != 0x0D) offset++;
            trailing_param =message->substr(p1, offset - p1);
        }
        else
        {
            // 'middle' param
            while(message->at(offset) != ' ' && (message->at(offset) != 0x0D)) offset++;
            std::string param  = message->substr(p1, offset - p1);
            params->push_back(param);
        }
    }
    Q_ASSERT(message->at(offset) == 0x0D && message->at(offset + 1) == 0x0A);
    // Check if command or command id
    char *p;
    int converted = std::strtol(command.c_str(), &p, 10);
    if (*p) {
        // conversion failed because the input wasn't a number
        IRCMessage::IRCCommand cmd;
        cmd = (*this->translation_table)[command];
        return new IRCMessage(prefix, cmd, params, trailing_param);
    }
    else {
        // use converted
        return new IRCMessage(prefix, converted, params, trailing_param);
    }


}
