#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QTextBrowser>
#include <ircmessageparser.h>
#include <ircmessage.h>
#include <ircmessagehandler.h>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void MainWindow::parseIRCMessage(QByteArray *message, QTextBrowser *output_window);
    std::string* MainWindow::buildIRCMessage(IRCMessage::IRCCommand command, std::vector<std::string> *params, std::string *trailing_param);
    void MainWindow::sendIRCMessage(QTcpSocket *session, QTextBrowser *output_window, IRCMessage::IRCCommand command, std::vector<std::string> *params, std::string *trailing_param);
    void MainWindow::sendSetNickMessage(QTcpSocket *session, QTextBrowser *output_window, std::string *nickname);
    void MainWindow::sendUserMessage(QTcpSocket *session, QTextBrowser *output_window, std::string *nickname, std::string *mode, std::string *real_name);

private slots:
    void ChangeText();
    void connectToIRC();
    void sendRaw();
    void readSocketData();
    void onConnectionEstablished();
    void tcpSocketState(QAbstractSocket::SocketState state);


private:
    Ui::MainWindow *ui;
    QTcpSocket *connection;
    QByteArray buffered_data;
    IRCMessageParser parser;
    IRCMessageHandler handler;
    std::string *nickname;
};

#endif // MAINWINDOW_H
