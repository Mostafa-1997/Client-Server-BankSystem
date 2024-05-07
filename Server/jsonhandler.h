#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QTextStream>

class JsonHandler : public QObject
{
    Q_OBJECT
public:
    explicit JsonHandler(QObject *parent = nullptr);
    QString GetAccNo(QString);
    qint32 ViewAccountBalance(QString);
    bool GetField(QString,QString,QString&);
    bool UpdateField(QString,QString,QString);
    bool MakeTransaction(qint32);
    bool TransferAmount(QString,quint32);
    QString ViewTransactionHistory(QString,quint16);
    bool checkAccNo(QString);
    bool checkLogin(QString ,QString);
    bool checkField(QString ,QString);
    bool checkUsername(QString);
    bool UpdateUser(QString ,QVariantMap);
    void SaveTransaction(QString,QString&);
    bool CreateUser(QString,QString,QVariantMap);
    bool DeleteUser(QString);
    QString ViewBankDataBase();


    QString role() const;
    void setRole(const QString &newRole);
    QString username() const;
    QString accountnumber() const;

signals:
private:
    QFile Login_DB;
    QFile Users_DB;
    QString m_accountnumber;
    QString m_username;
    QString m_role;
    QJsonObject m_login;
};




#endif // JSONHANDLER_H
