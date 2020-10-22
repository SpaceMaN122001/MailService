#include "accountinfo.h"
#include "ui_accountinfo.h"

AccountInfo::AccountInfo(QString login, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountInfo)
{
    _login = login;

    ui->setupUi(this);
    ui->accountPhotoPixmap->setPixmap(QPixmap(":/res/account.png").scaled(128, 128));
    ui->accountBranchIDLineEdit->hide();
    ui->label_10->hide();
    ui->accountBranchIDLineEdit->setReadOnly(true);

    setWindowIcon(QIcon(":/res/account.png"));

    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE NumberPhone = :login");
    query.bindValue(":login", _login);
    query.exec();

    while(query.next()) {

        ui->accountNumberPhoneLineEdit->setText(query.value(0).toString());
        ui->accountPasswordLineEdit->setText(query.value(1).toString());
        ui->accountFullNameLineEdit->setText(query.value(2).toString());
        ui->accountHomeAddressLineEdit->setText(query.value(3).toString());
        ui->accountBranchIDLineEdit->setText(query.value(5).toString());

        INT role = query.value(4).toInt();

        if(role == ChiefAdmin) ui->accountRoleLineEdit->setText("Главный администратор");
        if(role == Admin) ui->accountRoleLineEdit->setText("Администратор");

        if(role == PostOffice) {

            ui->accountRoleLineEdit->setText("Сотрудник почтового отделения");
            ui->accountBranchIDLineEdit->show();
            ui->label_10->show();
        }
        if(role == User) ui->accountRoleLineEdit->setText("Пользователь");
    }

    //сохранение данных
    connect(ui->accountSaveDataPushButton, &QPushButton::clicked, this, [this]() {

        QSqlQuery query;
        UserInfo user;
        user.Password = ui->accountPasswordLineEdit->text();
        user.fullName = ui->accountFullNameLineEdit->text();
        user.homeAddress = ui->accountHomeAddressLineEdit->text();

        if(!isPasswordCorrect(user.Password)) {QMessageBox::critical(this, "Внимание!", "Введите корректный пароль!"); return;}

        query.prepare("UPDATE Users SET Password = :password, FullName = :fullName, HomeAddress = :homeAddress WHERE NumberPhone = :login");
        query.bindValue(":fullName", user.fullName);
        query.bindValue(":password", user.Password);
        query.bindValue(":homeAddress", user.homeAddress);
        query.bindValue(":login", _login);
        bool isOk = query.exec();

        if(!isOk) {QMessageBox::critical(this, "Внимание!", "Произошла ошибка при запросе!"); return;}
        else QMessageBox::information(this, "Внимание!", "Данные сохранены!");

        close();
    });

    connect(ui->closePushButton, &QPushButton::clicked, this, [this](){close();});

}

AccountInfo::~AccountInfo()
{
    //delete ui;
}
