#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QWidget>
#include "mailservice.h"

namespace Ui {
class AccountInfo;
}

class AccountInfo : public QWidget
{
    Q_OBJECT

public:
    explicit AccountInfo(QString login, QWidget *parent = nullptr);
    ~AccountInfo();

private:
    Ui::AccountInfo *ui;
    QString _login;
};

#endif // ACCOUNTINFO_H
