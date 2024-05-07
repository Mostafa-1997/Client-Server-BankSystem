#include "AdminManager.h"
#include <QVariant>
#include <conio.h>
#include <iostream>
using namespace std;

AdminManager::AdminManager()
{
    outStream.setDevice(&socket);
    outStream.setVersion(QDataStream::Qt_6_6);
    inStream.setDevice(&socket);
    inStream.setVersion(QDataStream::Qt_6_6);
    role = "admin";
    connect(&socket,&QTcpSocket::connected,this,&AdminManager::connected);
    connect(&socket,&QTcpSocket::disconnected,this,&AdminManager::disconnected);
    connect(&socket,&QTcpSocket::stateChanged,this,&AdminManager::stateChanged);
    connect(&socket,&QTcpSocket::readyRead,this,&AdminManager::readyRead);
    connect(&socket,&QTcpSocket::errorOccurred,this,&AdminManager::errorOccurred);
}

bool AdminManager::login()
{
    qInfo() << "************ WELCOME ************";
    qInfo() << "Username: ";
    request = "Login";
    outStream << requestflag << request << role;
    string cname;
    cin >> cname;
    qadminname = QString::fromStdString(cname);
    qInfo() << "Password: ";
    char ch;
    string cpass;
    while (true)
    {
        ch = _getch();
        if (ch == 13)
        {
            break;
        }
        cout << '*';
        cpass += ch;
    }
    cout<< std::endl;
    QString qpass = QString::fromStdString(cpass);
    bool ok = false;
    if (!qadminname.isEmpty() && !qpass.isEmpty())
    {
        outStream << qadminname << qpass;
        socket.waitForBytesWritten();
        socket.waitForReadyRead();
        ok = serverResponse.toBool();
    }
    cleanScreen();
    return ok;
}

void AdminManager::getAccountNumber()
{
    qInfo() << "Username: ";
    string cusername;
    cin >> cusername;
    QString qusername = QString::fromStdString(cusername);
    outStream << qusername;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qInfo() << "Your AccountNumber is: " << serverResponse.toString();
}

void AdminManager::viewAccountBalance()
{
    cleanScreen();
    qInfo() << "Your AccountNumber: ";
    string cAccountNumber;
    cin >> cAccountNumber;
    QString qAccountNumber = QString::fromStdString(cAccountNumber);
    outStream << qAccountNumber;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qInfo() << "Your AccountBalance is: " << serverResponse.toInt();
}

void AdminManager::viewTransactionHistory()
{
    qInfo() << "Your AccountNumber: ";
    string cAccountNumber;
    cin >> cAccountNumber;
    QString qAccountNumber = QString::fromStdString(cAccountNumber);
    qInfo() << "Number of Transactions:";
    quint16 count;
    cin >> count;
    outStream << qAccountNumber << count;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qInfo().noquote() << serverResponse.toString();
}

void AdminManager::viewBankDatabase()
{
    socket.waitForReadyRead();
    qInfo().noquote() << serverResponse.toString();
}

void AdminManager::createNewUser()
{
    QTextStream cin(stdin);
    QVariantMap map;
    string cusername , cpassword;
    QString quserName , qpassword , fullName , flag = "check";
    quint16 counter = 0;
    qint32 balance = 0 , age = 0;
    bool ok = false;
    requestflag = "CreateUser";
    do
    {
        if (counter > 0)
        {
            qInfo() << "UserName is already existed. \nPlease Enter another UserName: ";
            std::cin >> cusername;
            quserName = QString::fromStdString(cusername);
            outStream << requestflag << flag << quserName;
        }
        else
        {
            qInfo() << "UserName: ";
            std::cin >> cusername;
            quserName = QString::fromStdString(cusername);
            outStream << flag << quserName;
        }
        counter++;
        socket.waitForReadyRead();
        ok = serverResponse.toBool();
    } while (!ok);
    flag = "update";
    qInfo() << "Full Name: ";
    fullName = cin.readLine();
    qInfo() << "Balance: ";
    std::cin >> balance;
    qInfo() << "Age: ";
    std::cin >> age;
    qInfo() << "Password:";
    std::cin >> cpassword;
    map["Fullname"] = fullName;
    map["Balance"] = balance;
    map["Age"] = age;
    map["Username"] = quserName;
    qpassword = QString::fromStdString(cpassword);
    outStream << requestflag << flag << quserName << qpassword << map;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    requestflag = "General";
    if (serverResponse.toBool() == true)
    {
        qInfo() << "Created Successfully";
    }
    else
    {
        qInfo() << "Not Created...";
    }
}

void AdminManager::deleteUser()
{
    qInfo() << "Your AccountNumber: ";
    string cAccountNumber;
    cin >> cAccountNumber;
    QString qAccountNumber = QString::fromStdString(cAccountNumber);
    outStream << qAccountNumber;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    if (serverResponse.toBool() == true)
    {
        qInfo() << "Deleted Done";
    }
    else
    {
        qInfo() << "Not deleted... \nAccountNumber is not existed...";
    }
}

void AdminManager::updateUser()
{
    QTextStream cin(stdin);
    QVariantMap map;
    QString qAccountNumber , fullName , flag = "check";
    string cAccountNumber, cpassword , key;
    quint16 counter = 0 , in = 0;
    qint32 balance = 0  , age = 0;
    bool chflag = true , ok = false;
    requestflag = "UpdateUser";
    do
    {
        if (counter > 0)
        {
            qInfo() << "AccountNumber is not existed... \nPlease Enter another one:";
            std::cin >> cAccountNumber;
            qAccountNumber = QString::fromStdString(cAccountNumber);
            outStream << requestflag << flag << qAccountNumber;
        }
        else
        {
            qInfo() << "Account Number:";
            std::cin >> cAccountNumber;
            qAccountNumber = QString::fromStdString(cAccountNumber);
            outStream << flag << qAccountNumber;
        }
        counter++;
        socket.waitForReadyRead();
        ok = serverResponse.toBool();
    } while (!ok);
    flag = "update";
    do
    {
        qInfo() << "Select Part wanted To Update: ";
        qInfo() << "(1) Full Name \n (2) Balance \n (3)Age \n (4)Password";
        std::cin >> in;
        switch (in)
        {
        case 1:
            qInfo() << "New Full Name:";
            fullName = cin.readLine();
            map["Fullname"] = fullName;
            qInfo() << fullName;
            break;
        case 2:
            qInfo() << "New Balance:";
            std::cin >> balance;
            map["Balance"] = balance;
            break;
        case 3:
            qInfo() << "New Age:";
            std::cin >> age;
            map["Age"] = age;
            break;
        case 4:
            qInfo() << "New Password:";
            std::cin >> cpassword;
            map["password"] = QString::fromStdString(cpassword);
            break;
        default:
            qInfo() << "Invalid Choice...";
            break;
        }
        qInfo() << "*****************************************************";
        qInfo() << "To update another part press 'U' \n But to save and exit press 'E'";
        std::cin >> key;
        if (key != "u" || key != "U")
        {
            chflag = false;
        }
    } while (chflag);
    outStream << requestflag << flag << qAccountNumber << map;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    requestflag = "General";
    if (serverResponse.toBool() == true)
    {
        qInfo() << "Updated Successfully";
    }
    else
    {
        qInfo() << "Not Updated...";
    }
}

void AdminManager::sendRequestToServer()
{
    outStream << requestflag << request << role;
}

void AdminManager::start(bool &IsLoggin)
{
    quint16 input;
    char in;
    qInfo() << "Choose from the list: \n (1) View Account Balance \n (2) View Transaction History \n (3) Get Account Number \n (4) Create New User \n (5) Update User Data \n (6) Delete User \n (7) View Bank DataBase \n (8) exit";
    qInfo() << "Your choose is : ";
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
        request = "GetAccountNumber";
        sendRequestToServer();
        getAccountNumber();
        break;
    case 4:
        request = "Create New User";
        sendRequestToServer();
        createNewUser();
        break;
    case 5:
        request = "Update User";
        sendRequestToServer();
        updateUser();
        break;
    case 6:
        request = "Delete User";
        sendRequestToServer();
        deleteUser();
        break;
    case 7:
        request = "View Bank DataBase";
        sendRequestToServer();
        viewBankDatabase();
        break;
    case 8:
        IsLoggin = false;
        break;
    default:
        qInfo() << "Invalid Choice...";
        break;
    }
    if (input != 8)
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

void AdminManager::connectToHost(QString hostname, quint16 port)
{
    if(socket.isOpen())
    {
        disconnect();
    }
    qInfo() << "Trying to connect to " << hostname << " on port " << port;
    socket.connectToHost(hostname,port);
}

void AdminManager::connected()
{
    QTextStream input(stdin, QIODevice::ReadOnly);
    QString userInput = input.readLine().trimmed();
    socket.write(userInput.toUtf8());
}

void AdminManager::disconnected()
{
    qInfo() << "Connection closed...";
}

void AdminManager::errorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qInfo() << "Error:" << socket.errorString();
}

void AdminManager::stateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString str = metaEnum.valueToKey(socketState);
    qInfo() << "Socket state changed: " << str;
}

void AdminManager::disconnect()
{
    socket.close();
    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

void AdminManager::readyRead()
{
    if (request == "View Bank DataBase")
    {
        QString database;
        inStream >> database;
        serverResponse.setValue(database);
    }
    else if (request == "View Account Balance")
    {
        quint32 AccountMoney;
        inStream >> AccountMoney;
        serverResponse.setValue(AccountMoney);
    }
    else if (request == "Create New User" || (request == "Delete User") || (request == "Update User") || (request == "Login"))
    {
        bool respond;
        inStream >> respond;
        serverResponse.setValue(respond);
    }
    else if (request == "GetAccountNumber")
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
