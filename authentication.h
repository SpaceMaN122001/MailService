#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QWidget>
#include <QKeyEvent>
#include "mailservice.h"
#include "userpersonalaccount.h"
#include "settings.h"

namespace Ui {
class Authentication;
}

class Authentication : public QWidget
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = nullptr);
    ~Authentication();

private slots:
    void on_authenticationPushButton_clicked();
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::Authentication *ui;
    bool isOk;
};

#endif // AUTHENTICATION_H
