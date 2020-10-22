#ifndef ADDINGUSER_H
#define ADDINGUSER_H

#include <QWidget>
#include "mailservice.h"

namespace Ui {
class AddingUser;
}

class AddingUser : public QWidget
{
    Q_OBJECT

public:
    explicit AddingUser(QString login, QWidget *parent = nullptr);
    ~AddingUser();

private:
    Ui::AddingUser *ui;
    QString _login;
    INT _role;
};

#endif // ADDINGUSER_H
