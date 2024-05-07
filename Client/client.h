#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMetaEnum>


class client : public QObject
{
    Q_OBJECT
protected:
    QString role , request , requestflag;
    QVariant serverResponse;
public:
    client();
    virtual void getAccountNumber() = 0;
    virtual void viewAccountBalance() = 0;
    virtual void viewTransactionHistory() = 0;
    virtual void sendRequestToServer() = 0;
    virtual bool login() = 0;
    void cleanScreen();
};

#endif // CLIENT_H
