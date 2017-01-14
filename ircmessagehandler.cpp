#include "ircmessagehandler.h"

IRCMessageHandler::IRCMessageHandler()
{

}

void IRCMessageHandler::HandleMessage(IRCMessage *msg, QTabWidget *window, std::string *own_nick)
{
    QTextBrowser *status_window = dynamic_cast<QTextBrowser *>(window->widget(0));
    if (msg->_command_id != -1)
    {
        // Status window always zero.
        QString out_string(QString("%1, %2, %3").arg(QString::fromStdString(msg->_prefix), QString::number(msg->_command_id), QString::fromStdString(msg->_trailing_param)));
        status_window->append(out_string);
    }
    else
    {
        switch(msg->_command_type)
        {
            case IRCMessage::NOTICE:
             {
                QString out_string(QString("%1, %2, %3").arg(QString::fromStdString(msg->_prefix), QString("NOTICE"), QString::fromStdString(msg->_trailing_param)));
                status_window->append(out_string);
                break;
             }
            case IRCMessage::JOIN:
               handleJoinMessage(msg, window, own_nick);
               break;
            case IRCMessage::PRIVMSG:
               handlePrivmsgMessage(msg, window, own_nick);
               break;
            default:
                    break;
        }
    }
}

void IRCMessageHandler::handleJoinMessage(IRCMessage *msg, QTabWidget *window, std::string *own_nick)
{
    std::string nick = extractNickname(&msg->_prefix);
    if (nick.compare(*own_nick) == 0)
    {
        // If joined nick belongs to client, that means we need to open new tab for channel.
        QTextBrowser *new_browser = new QTextBrowser();
        window->addTab(new_browser, QString::fromStdString(msg->_params.at(0)));
    }
    else
    {
        // Search correct tab
        for (int i = 0 ; i < window->count() ; i++)
        {
            if ((QString::fromStdString(msg->_params.at(0)).compare(window->tabText(i)) == 0))
            {
                QTextBrowser *browser = dynamic_cast<QTextBrowser *>(window->widget(i));
                browser->append(QString("%1 has joined the channel.").arg(QString::fromStdString(nick)));
            }
        }
    }
}

void IRCMessageHandler::handlePrivmsgMessage(IRCMessage *msg, QTabWidget *window, std::string *own_nick)
{
    std::string nick = extractNickname(&msg->_prefix);
    bool found = false;
    std::string tab_name;
    if (msg->_params.at(0).at(0) == '#')
    {
         tab_name = std::string(msg->_params.at(0));
    }
    else
    {
         tab_name = std::string(nick);
    }

    // Search correct tab
    for (int i = 0 ; i < window->count() ; i++)
    {
        if ((QString::fromStdString(tab_name).compare(window->tabText(i)) == 0))
        {
            QTextBrowser *browser = dynamic_cast<QTextBrowser *>(window->widget(i));
            browser->append(QString("<%1> %2").arg(QString::fromStdString(nick), QString::fromStdString(msg->_trailing_param)));
            found = true;
            break;
        }
    }
    if (!found)
    {
        // If joined nick belongs to client, that means we need to open new tab for channel.
        QTextBrowser *new_browser = new QTextBrowser();
        window->addTab(new_browser, QString::fromStdString(nick));
        new_browser->append(QString("<%1> %2").arg(QString::fromStdString(nick), QString::fromStdString(msg->_trailing_param)));
    }

}

std::string IRCMessageHandler::extractNickname(std::string *prefix)
{
    return prefix->substr(1, prefix->find_first_of("!") -1);
}
