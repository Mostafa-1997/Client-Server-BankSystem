#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "UserManager.h"
#include "AdminManager.h"
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    bool IsLoggin = false;
    qInfo() << "Welcome to our Banking System..." ;
    qInfo() << "Please choose Who are you? " ;
    qInfo() << "\"User\" or \"Admin\" or \"Exit\" " ;
    string cselection;
    cin >> cselection;
    QString qselection = QString::fromStdString(cselection);
    if(qselection.toUpper() == "USER")
    {
        //qInfo() << "AsmaaMohamed";
        UserManage user;
        user.connectToHost("127.0.0.1" , 1234);
        while(!IsLoggin)
        {
            IsLoggin = user.login();
            user.cleanScreen();
            if(!IsLoggin)
            {
                qInfo() << "Username or Password is uncorrect, Please try again..." ;
            }
        }
        while(IsLoggin)
        {
            user.start(IsLoggin);
        }
    }
    else if(qselection.toUpper() == "ADMIN")
    {
        AdminManager admin;
        admin.connectToHost("127.0.0.1" , 1234);
        while(!IsLoggin)
        {
            IsLoggin = admin.login();
            admin.cleanScreen();
            if(!IsLoggin)
            {
                qInfo() << "Username or Password is uncorrect, Please try again..." ;
            }
        }
        while(IsLoggin)
        {
            admin.start(IsLoggin);
        }
    }
    else if (qselection.toUpper() == "EXIT")
    {
        qInfo() << "Banking System is Exiting now...";
        QCoreApplication::exit(0);
    }
    else
    {
        qInfo() << "Invalid Username...";
    }
    QCoreApplication::exit(0);
    return a.exec();
}
