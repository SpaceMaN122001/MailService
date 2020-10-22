#ifndef UserPersonalAccount_H
#define UserPersonalAccount_H

#include <QMainWindow>
#include "mailservice.h"
#include "accountinfo.h"
#include "addinguser.h"
#include "authentication.h"
#include "parcelmanagement.h"
#include "historyofalllshipments.h"
#include "changepostofficeemployeeid.h"

namespace Ui {
class UserPersonalAccount;
}

class UserPersonalAccount : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserPersonalAccount(QString login,  QWidget *parent = nullptr);
    ~UserPersonalAccount();

private slots:
    void on_removeAllParcelsToolButton_clicked();

    void on_removeAllUsersToolButton_clicked();

    void on_historyOfAllShimpmentsToolButton_clicked();

    void on_changePostOfficeEmployeeIDToolButton_clicked();

    void on_clearAllHistoriesParcelsToolButton_clicked();

    void on_shrinkDataBaseToolButton_clicked();

private:
    Ui::UserPersonalAccount *ui;
    QString _login;
};

#endif // UserPersonalAccount_H
