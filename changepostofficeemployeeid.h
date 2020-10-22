#ifndef CHANGEPOSTOFFICEEMPLOYEEID_H
#define CHANGEPOSTOFFICEEMPLOYEEID_H

#include <QWidget>
#include "mailservice.h"

namespace Ui {
class ChangePostOfficeEmployeeID;
}

class ChangePostOfficeEmployeeID : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePostOfficeEmployeeID(QWidget *parent = nullptr);
    ~ChangePostOfficeEmployeeID();

private slots:
    void on_allUsersComboBox_currentTextChanged(const QString &arg1);

    void on_saveDataPushButton_clicked();

    void on_closePushButton_clicked();

private:
    Ui::ChangePostOfficeEmployeeID *ui;
};

#endif // CHANGEPOSTOFFICEEMPLOYEEID_H
