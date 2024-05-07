#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "client.h"

class UserManage : public client
{
    Q_OBJECT

private:
    QTcpSocket *socket;
    QDataStream inStream , outStream;
    QString qusername , accountNumber;

public:
    UserManage();
    bool login();
    void getAccountNumber();
    void viewAccountBalance();
    void viewTransactionHistory();
    void makeTransaction();
    void transferAccount();
    void sendRequestToServer();
    void start(bool &isLogged);
    ~UserManage();

signals:

public slots:
    void connectToHost(QString hostname , quint16 port);


protected slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void disconnect();
    void readyRead();
};

#endif // USERMANAGER_H
