#include "addinguser.h"
#include "ui_addinguser.h"

AddingUser::AddingUser(QString login, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddingUser)
{
    ui->setupUi(this);
    ui->accountBranchIDLineEdit->hide();
    ui->label_9->hide();
    setWindowTitle("Главный администратор");
    setWindowIcon(QIcon(":/res/adduser.ico"));

    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE NumberPhone = :login");
    query.bindValue(":login", login);
    query.exec();

    while(query.next()) {

        if(query.value(4).toInt() == ChiefAdmin) _role = 2;
        if(query.value(4).toInt() == Admin) {_role = 3; ui->accountBranchIDLineEdit->show();ui->label_9->show();}
    }

    //Регистрация пользователя
    connect(ui->accountAddDataPushButton, &QPushButton::clicked, this, [this]() {

        userInfo user;
        QSqlQuery query;

        user.numberPhone = ui->accountNumberPhoneLineEdit->text();
        user.Password = ui->accountPasswordLineEdit->text();
        user.fullName = ui->accountFullNameLineEdit->text();
        user.homeAddress = ui->accountHomeAddressLineEdit->text();
        user.officeID = ui->accountBranchIDLineEdit->text();

        if(!isPasswordCorrect(user.Password)) {QMessageBox::critical(this, "Внимание!", "Введите корректный пароль!"); return;}

        query.exec("SELECT * FROM Users");

        while(query.next()) {

            if(query.value(0).toString() == user.numberPhone) {QMessageBox::critical(this, "Внимание!", "Такой номер телефона уже зарегестрирован!"); return;}

            if(query.value(1).toString() == user.Password) {QMessageBox::critical(this, "Внимание!", "Такой пароль уже существует!"); return;}

            if(user.officeID != nullptr && query.value(5).toString() == user.officeID) {QMessageBox::critical(this, "Внимание!", "Сотрудник с таким идентификатором уже существует!"); return;}
        }

        QString numberPhone = user.numberPhone;
        QString password = user.Password;
        QString fullName = user.fullName;
        QString homeAddress = user.homeAddress;
        QString officeID = user.officeID;

        if(user.officeID != nullptr) {

            if(numberPhone.remove(' ') == nullptr || password.remove(' ') == nullptr || fullName.remove(' ') == nullptr || homeAddress.remove(' ') == nullptr || officeID.remove(' ') == nullptr) {

                QMessageBox::critical(this, "Внимание!", "Не все поля заполнены!");

                return;
            }

        } else if(user.officeID == nullptr) {

            if(numberPhone.remove(' ') == nullptr || password.remove(' ') == nullptr || fullName.remove(' ') == nullptr || homeAddress.remove(' ') == nullptr) {

                QMessageBox::critical(this, "Внимание!", "Не все поля заполнены!");

                return;
            }
        }

        QString sqlQuery = "INSERT INTO Users (NumberPhone, Password, FullName, HomeAddress, Role, OfficeID) VALUES (:number, :password, :fullName, :homeAddress, :role, :officeId)";

        if(user.officeID == nullptr) sqlQuery = "INSERT INTO Users (NumberPhone, Password, FullName, HomeAddress, Role) VALUES (:number, :password, :fullName, :homeAddress, :role)";

        query.prepare(sqlQuery);
        query.bindValue(":number", user.numberPhone);
        query.bindValue(":password", user.Password);
        query.bindValue(":fullName", user.fullName);
        query.bindValue(":homeAddress", user.homeAddress);
        query.bindValue(":role", _role);
        if(user.officeID != nullptr) query.bindValue(":officeId", user.officeID);
        bool isOk = query.exec();

        if(!isOk) {QMessageBox::critical(this, "Внимание!", "Ошибка запроса!"); return;}

        QMessageBox::information(this, "Внимание!", "Вы успешно добавлены в систему!");

    });

    connect(ui->closePushButton, &QPushButton::clicked, this, [this]() {close();});

}

AddingUser::~AddingUser()
{
    delete ui;
}
