#ifndef SERVER_H
#define SERVER_H

#include <QTextStream>
#include <QObject>
#include <QDebug>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include "jsonhandler.h"


class Server : public JsonHandler
{
    Q_OBJECT

public:

    explicit Server(JsonHandler *parent = nullptr);
    void ServerResponse(QString,QString);

signals:

public slots:

    void start();
    void quit();
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer server;

};
#endif // SERVER_H
