#ifndef MAILSERVICE_H
#define MAILSERVICE_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <string>
#include <cctype>
#include <Windows.h>
#include "qcustombutton.h"

enum Users
{
    ChiefAdmin = 1,
    Admin = 2,
    PostOffice = 3,
    User = 4
};

enum Status
{
    InTransit = 0,
    ReceivedAndReadyToShip = 1,
    Delivered = 2
};

enum HistoryStatus
{
    SentFrom = 0, //отправлен из
    AdoptedIn = 1, //принят в
    TransitedTo = 2, //отправлено транзитом
    AwaitingReceipt = 3, //ожидает получения
    Received = 4 //получено
};

typedef struct UserInfo
{
    QString numberPhone;
    QString Password;
    QString fullName;
    QString homeAddress;
    INT role;
    QString officeID; //RR378987

} userInfo;

struct Parcel
{
    QString trackNumber;
    QString senderAddress;
    QString recipientAddress;
    QString type;
    QString departureClass;
    QString deliveryPeriod;
    QString costOfDelivery;
    QString weight;
    QString declaredValue;
    QString totalCost;
    QString senderIndex;
    QString recipientIndex;
    INT status;
    QString nextBranch;

};

bool isPasswordCorrect(QString password);

class MailService
{
public:
    MailService();
    ~MailService();
};
#endif // MAILSERVICE_H
