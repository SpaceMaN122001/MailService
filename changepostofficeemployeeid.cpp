#include "changepostofficeemployeeid.h"
#include "ui_changepostofficeemployeeid.h"

ChangePostOfficeEmployeeID::ChangePostOfficeEmployeeID(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePostOfficeEmployeeID)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/russiamail.png"));

    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE Role = :role");
    query.bindValue(":role", PostOffice);
    query.exec();

    while(query.next()) ui->allUsersComboBox->addItem(query.value(0).toString());
}

ChangePostOfficeEmployeeID::~ChangePostOfficeEmployeeID()
{
    delete ui;
}

void ChangePostOfficeEmployeeID::on_allUsersComboBox_currentTextChanged(const QString &arg1)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE NumberPhone = :numberPhone");
    query.bindValue(":numberPhone", ui->allUsersComboBox->currentText());
    query.exec();

    while(query.next()) ui->identifierLineEdit->setText(query.value(5).toString());
}

void ChangePostOfficeEmployeeID::on_saveDataPushButton_clicked()
{
    QSqlQuery query;
    query.prepare("UPDATE Users SET OfficeID = :officeID");
    query.bindValue(":officeID", ui->identifierLineEdit->text());
    query.exec();

    QMessageBox::information(this, "Внимание!", "Данные сохранены!");

    close();
}

void ChangePostOfficeEmployeeID::on_closePushButton_clicked()
{
    close();
}
