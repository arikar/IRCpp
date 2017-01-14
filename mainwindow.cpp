#include "mainwindow.h"
#include "ui_mainwindow.h"


QByteArray irc_commands_str[] {
    "PASS",
    "NICK",
    "USER",
    "JOIN",
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    buffered_data(),
    parser(),
    handler()
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(connectToIRC()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(sendRaw()));
    QTextBrowser *browser = new QTextBrowser(nullptr);
    this->ui->tabWidget->insertTab(0, browser, QString("Status"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ChangeText()
{
}

void MainWindow::connectToIRC()
{
    QHostInfo addresses = QHostInfo::fromName("irc.freenode.com");
    QHostAddress address = addresses.addresses().first();
    this->connection = new QTcpSocket();
    connect(this->connection, SIGNAL(readyRead()), SLOT(readSocketData()), Qt::UniqueConnection);
    connect(this->connection, SIGNAL(connected()), SLOT(onConnectionEstablished()), Qt::UniqueConnection);
    connect(this->connection, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(tcpSocketState(QAbstractSocket::SocketState)), Qt::UniqueConnection);
    this->connection->connectToHost(address, 6667);
    QTcpSocket::SocketState sstate = this->connection->state();

}

void MainWindow::readSocketData()
{
    while (this->connection->bytesAvailable() != 0)
    {
        if (buffered_data.endsWith("\r\n") || buffered_data.isEmpty())
        {
            buffered_data = this->connection->readLine();
        }
        else
        {
            qDebug("Bytes available: %d", this->connection->bytesAvailable());
            buffered_data.append(this->connection->readLine());
        }
        if (buffered_data.endsWith("\r\n"))
        {
            qDebug("Data: %s", buffered_data.constData());
            QTextBrowser *browser = dynamic_cast<QTextBrowser *>(this->ui->tabWidget->widget(0));
            IRCMessage *msg = parser.parseIRCMessage(&buffered_data.toStdString());
            this->handler.HandleMessage(msg, this->ui->tabWidget, this->nickname);
        }
    }
}

void MainWindow::onConnectionEstablished()
{
    qDebug("Connection established, setting nick.");
    std::string nickname = this->ui->lineEdit->text().toStdString();
    this->nickname = new std::string(nickname);
    std::string real_name = this->ui->lineEdit_2->text().toStdString();
    std::string mode("*");
    QTextBrowser *browser = dynamic_cast<QTextBrowser *>(this->ui->tabWidget->widget(0));
    sendSetNickMessage(this->connection, browser, &nickname);
    sendUserMessage(this->connection,browser, &nickname, &mode, &real_name);
}


void MainWindow::tcpSocketState(QAbstractSocket::SocketState state)
{
    qDebug("state changed... %d", state);
}

std::string* MainWindow::buildIRCMessage(IRCMessage::IRCCommand command, std::vector<std::string> *params, std::string *trailing_param)
{
    std::string *message = new std::string();
    // Command type
    std::string command_ = (*parser.reverse_translation_table)[command];
    message->append(command_);


    // Command params
    if (params != nullptr)
    {
        message->append(" ");
        std::for_each(params->begin(), params->end(), [&](std::string &param) {
           message->append(param);
           message->append(" ");
        });
    }

    if (trailing_param != nullptr)
    {
        message->append(" ");
        message->append(":");
        message->append(*trailing_param);
    }

    // CLRF
    message->append("\r\n");
    return message;
}

void MainWindow::sendRaw()
{
    std::vector<std::string> *params = new std::vector<std::string>();
    std::string channel("#test321");
    params->push_back(channel);
    QTextBrowser *browser = dynamic_cast<QTextBrowser *>(this->ui->tabWidget->widget(0));

    sendIRCMessage(this->connection, browser, IRCMessage::JOIN, params, nullptr);
}

void MainWindow::sendIRCMessage(QTcpSocket *session, QTextBrowser *output_window, IRCMessage::IRCCommand command, std::vector<std::string> *params, std::string *trailing_param)
{
    std::string *command_data = buildIRCMessage(command, params, trailing_param);
    output_window->append(QString(">> Outgoing Command: %1").arg(QString::fromStdString(*command_data)));
    session->write(command_data->c_str(), command_data->size());
}

void MainWindow::sendSetNickMessage(QTcpSocket *session, QTextBrowser *output_window, std::string *nickname)
{
    std::vector<std::string> *params = new std::vector<std::string>();
    params->push_back(*nickname);
    sendIRCMessage(session, output_window, IRCMessage::NICK, params, nullptr);
}
void MainWindow::sendUserMessage(QTcpSocket *session, QTextBrowser *output_window, std::string *nickname, std::string *mode, std::string *real_name)
{
    /*
        Command: USER
        Parameters: <user> <mode> <unused> <realname>
     */
    std::string unused("*");
    std::vector<std::string> *params = new std::vector<std::string>();
    params->push_back(*nickname);
    params->push_back(*mode);
    params->push_back(unused);
    sendIRCMessage(session, output_window, IRCMessage::USER, params, real_name);
}
