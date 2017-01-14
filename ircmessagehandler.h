#ifndef IRCMESSAGEHANDLER_H
#define IRCMESSAGEHANDLER_H
#include <ircmessage.h>
#include <QtWidgets/QTabWidget>
#include <QTextBrowser>
#include <QString>

class IRCMessageHandler
{
public:
    IRCMessageHandler();
    void HandleMessage(IRCMessage *msg, QTabWidget *window, std::string *own_nick);

    void IRCMessageHandler::handleJoinMessage(IRCMessage *msg, QTabWidget *window, std::string *own_nick);
    void IRCMessageHandler::handlePrivmsgMessage(IRCMessage *msg, QTabWidget *window, std::string *own_nick);
    std::string IRCMessageHandler::extractNickname(std::string *prefix);
};

#endif // IRCMESSAGEHANDLER_H
