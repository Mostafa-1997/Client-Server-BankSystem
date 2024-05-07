#include "UserManager.h"
#include <QVariant>
#include <conio.h>
#include <iostream>
using namespace std;

UserManage::UserManage() : client()
{
    socket = new QTcpSocket;
    outStream.setDevice(socket);
    outStream.setVersion(QDataStream::Qt_6_6);
    inStream.setDevice(socket);
    inStream.setVersion(QDataStream::Qt_6_6);
    role = "user";
    request = "Login";
    requestflag = "General";
    connect(socket,&QAbstractSocket::connected,this,&UserManage::connected);
    connect(socket,&QAbstractSocket::disconnected,this,&UserManage::disconnected);
    connect(socket,&QAbstractSocket::errorOccurred,this,&UserManage::errorOccurred);
    connect(socket,&QAbstractSocket::stateChanged,this,&UserManage::stateChanged);
    connect(socket,&QAbstractSocket::readyRead,this,&UserManage::readyRead);
}

bool UserManage::login()
{
    qInfo() << "************ WELCOME ************";
    qInfo() << "Username: ";
    request = "Login";
    outStream << requestflag << request << role;
    string cname;
    cin >> cname;
    qusername = QString::fromStdString(cname);
    qInfo() << "Password: ";
    char ch;
    string cpass;
    while (true)
    {
        ch = _getch();
        if (ch == 13) // Enter key
        {
            break;
        }
        cout << '*' ;
        cpass += ch;
    }
    cout << endl;
    QString qpass = QString::fromStdString(cpass);
    bool ok = false;
    if (!qusername.isEmpty() && !qpass.isEmpty())
    {
        outStream << qusername << qpass;
        socket->waitForBytesWritten();
        socket->waitForReadyRead();
        ok = serverResponse.toBool();
    }
    cleanScreen();
    return ok;
}

void UserManage::getAccountNumber()
{
    outStream << qusername;
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    qInfo() << "Your AccountNumber is: " << serverResponse.toString();
}

void UserManage::viewAccountBalance()
{
    socket->waitForReadyRead();
    qInfo() << "Your AccountBalance is: " << serverResponse.toInt();
}

void UserManage::viewTransactionHistory()
{
    qInfo() << "Your AccountNumber :";
    quint16 count;
    cin >> count;
    outStream << count;
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    qInfo().noquote() << serverResponse.toString();
}

void UserManage::makeTransaction()
{
    qint32 transactionAmount = 0;
    qInfo() << "TransactionAmount: ";
    cin >> transactionAmount;
    outStream << transactionAmount;
    socket->waitForReadyRead();
    qInfo() << "Transaction is: " << serverResponse.toBool();
}

void UserManage::transferAccount()
{
    qInfo() << "To AccountNumber will transfer:";
    string ctoAccountNumber;
    cin >> ctoAccountNumber;
    qInfo() << "TransferAmount:";
    quint32 transferAmount;
    cin >> transferAmount;
    QString qtoAccountNumber = QString::fromStdString(ctoAccountNumber);
    outStream << qtoAccountNumber << transferAmount;
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    qInfo() << "Transaction is: " << serverResponse.toBool();
}

void UserManage::sendRequestToServer()
{
    outStream << requestflag << request << role;
}


void UserManage::start(bool &IsLoggin)
{
    quint16 input;
    char in;
    qInfo() << "Select which Action you need to make it: \n (1) View Account Balance \n (2) View Transaction History \n (3) Get Account Number \n (4) Transfer Account \n (5) MakeTransaction \n (6) exit ";
    qInfo() << "Enter Your Selection: ";
    cin >> input;
    cin.ignore();
    cleanScreen();
    switch (input)
    {
    case 1:
        request = "View Account Balance";
        sendRequestToServer();
        viewAccountBalance();
        break;
    case 2:
        request = "View Transaction History";
        sendRequestToServer();
        viewTransactionHistory();
        break;
    case 3:
        request = "Get Account Number";
        sendRequestToServer();
        getAccountNumber();
        break;
    case 4:
        request = "Transfer Account";
        sendRequestToServer();
        transferAccount();
        break;
    case 5:
        request = "Make Transaction";
        sendRequestToServer();
        makeTransaction();
        break;
    case 6:
        IsLoggin = false;
        break;
    default:
        qInfo() << "Invalid choice";
        break;
    }
    if (input == 1 || input == 2 || input == 3 || input == 4 || input == 5)
    {
        qInfo() << "Another request press 'R' But to exit press 'E' : ";
        cin >> in;
        if (in == 'E' || in == 'e')
        {
            IsLoggin = false;
        }
        cleanScreen();
    }
}

UserManage::~UserManage()
{
    delete socket;
}

void UserManage::connectToHost(QString hostname, quint16 port)
{
    if(socket->isOpen())
    {
        disconnect();
    }
    qInfo() << "Trying to connect to " << hostname << " on port " << port;
    socket->connectToHost(hostname,port);
}

void UserManage::connected()
{
    QTextStream input(stdin, QIODevice::ReadOnly);
    QString userInput = input.readLine().trimmed();
    socket->write(userInput.toUtf8());
}

void UserManage::disconnected()
{
    qInfo() << "Connection closed...";
}

void UserManage::errorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qInfo() << "Error:" << socket->errorString();
}

void UserManage::stateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString str = metaEnum.valueToKey(socketState);
    qInfo() << "Socket state changed: " << str;
}

void UserManage::disconnect()
{
    socket->close();
    socket->disconnectFromHost();
    socket->waitForDisconnected();
}

void UserManage::readyRead()
{
    if (request == "View Account Balance")
    {
        quint32 AccountMoney;
        inStream >> AccountMoney;
        serverResponse.setValue(AccountMoney);
    }
    else if (request == "Transfer Account" || request == "Make Transaction" || request == "Login")
    {
        bool respond;
        inStream >> respond;
        serverResponse.setValue(respond);
    }
    else if (request == "Get Account Number")
    {
        QString AccountNumber;
        inStream >> AccountNumber;
        serverResponse.setValue(AccountNumber);
    }
    else if (request == "View Transaction History")
    {
        QString TranactionHistory;
        inStream >> TranactionHistory;
        serverResponse.setValue(TranactionHistory);
    }
}
