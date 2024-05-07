#ifndef ADMINMANAGER_H
#define ADMINMANAGER_H

#include "client.h"

class AdminManager : public client
{
    Q_OBJECT

private:
    QTcpSocket socket;
    QDataStream inStream , outStream;
    QString qadminname;

public:
    AdminManager();
    bool login();
    void getAccountNumber();
    void viewAccountBalance();
    void viewTransactionHistory();
    void viewBankDatabase();
    void createNewUser();
    void deleteUser();
    void updateUser();
    void sendRequestToServer();
    void start(bool &isLogged);

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

#endif // ADMINMANAGER_H
