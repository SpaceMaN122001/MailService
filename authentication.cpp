#include "authentication.h"
#include "ui_authentication.h"

Authentication::Authentication(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Authentication)
{
    ui->setupUi(this);
    ui->mailServiceIconPixmap->setPixmap(QPixmap(":/res/russiamail.png").scaled(64, 64));
    ui->mainTabWidget->setTabIcon(0, QIcon(":/res/authentication.png"));
    ui->mainTabWidget->setTabIcon(1, QIcon(":/res/registration.png"));
    ui->loginIconPixmap->setPixmap(QPixmap(":/res/login.ico").scaled(16, 16));
    ui->passwordIconPixmap->setPixmap(QPixmap(":/res/password.ico").scaled(16, 16));
    //ui->authenticationNumberPhoneLineEdit->setText("901");
    //ui->authenticationPasswordLineEdit->setText("Mail1337");
    ui->passwordIconPixmap_2->setPixmap(QPixmap(":/res/password.ico").scaled(16, 16));
    ui->numberPhoneIconPixmap->setPixmap(QPixmap(":/res/phone.ico").scaled(16, 16));
    ui->fullNameIconPixmap->setPixmap(QPixmap(":/res/user.ico").scaled(16, 16));
    ui->homeAddressIconPixmap->setPixmap(QPixmap(":/res/address.png").scaled(16, 16));
    ui->authenticationNumberPhoneLineEdit->setText("900");
    ui->authenticationPasswordLineEdit->setText("admin");

    setWindowTitle("Аунтефикация/Регистрация");
    setWindowIcon(QIcon(":/res/russiamail.png"));
    setWindowFlags(Qt::Dialog);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMaximumSize(1280, 400);

    connect(ui->quitPushButton, &QPushButton::clicked, this, []() { QApplication::quit(); });

    QString pathToAppData = "C:\\Users\\" + QDir::home().dirName() + "\\AppData\\Local\\";

        auto inifileinitialization = [](QString path)
        {
            QSettings settings(path + "MailService\\settings.ini", QSettings::IniFormat);
            settings.beginGroup("DataBase");
            settings.setValue("Server", "");
            settings.setValue("UserName", "");
            settings.setValue("Password", "");
            settings.endGroup();
        };

        QDir dir;
        QFile file;

        file.setFileName(pathToAppData + "MailService\\settings.ini");

        if(!dir.exists(pathToAppData + "MailService")) {

            dir.setPath(pathToAppData);
            dir.mkdir("MailService");

            file.open(QFile::WriteOnly);
            file.close();

            inifileinitialization(pathToAppData);
        }

        if(dir.exists(pathToAppData + "MailService") && !file.exists()) {

            file.open(QFile::WriteOnly);
            file.close();

            inifileinitialization(pathToAppData);
        }

    /*QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=DESKTOP-U7OPH21\\SQLEXPRESS;DATABASE=MailService;Trusted_Connection=yes;");
    db.setUserName("DESKTOP-U7OPH21\\Spaceman");
    db.setPassword("");*/

    QString server;
    QString userName;
    QString password;
    QSettings settings(pathToAppData + "MailService\\settings.ini", QSettings::IniFormat);

    server = settings.value("DataBase/Server").toString() + ";";
    userName = settings.value("DataBase/UserName").toString();
    password = settings.value("DataBase/Password").toString();

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=" + server + "DATABASE=MailService;Trusted_Connection=yes;");
    db.setUserName(userName);
    db.setPassword(password);

    if(!db.open()) {

        QMessageBox::critical(this, "Внимание!","Базза данных не подключена!");

        Settings* settings = new Settings(false);
        settings->setAttribute(Qt::WA_DeleteOnClose);
        settings->show();
    }

    //Регистрация пользователя
    connect(ui->registrationPushButton, &QPushButton::clicked, this, [this]() {

        userInfo user;
        QSqlQuery query;

        user.numberPhone = ui->registrationNumberPhoneLineEdit->text();
        user.Password = ui->registrationPasswordLineEdit->text();
        user.fullName = ui->registrationFullnameLineEdit->text();
        user.homeAddress = ui->registrationHomeAddressLineEdit->text();

        if(!isPasswordCorrect(user.Password)) {QMessageBox::critical(this, "Внимание!", "Введите корректный пароль!"); return;}

        query.exec("SELECT * FROM Users");

        while(query.next()) {

            if(query.value(0).toString() == user.numberPhone) {QMessageBox::critical(this, "Внимание!", "Такой номер телефона уже зарегестрирован!"); return;}

            if(query.value(1).toString() == user.Password) {QMessageBox::critical(this, "Внимание!", "Такой пароль уже существует!"); return;}
        }

        QString numberPhone = user.numberPhone;
        QString password = user.Password;
        QString fullName = user.fullName;
        QString homeAddress = user.homeAddress;

        if(numberPhone.remove(' ') == nullptr || password.remove(' ') == nullptr || fullName.remove(' ') == nullptr || homeAddress.remove(' ') == nullptr) {QMessageBox::critical(this, "Внимание!", "Не все поля заполнены!"); return;}

        query.prepare("INSERT INTO Users (NumberPhone, Password, FullName, HomeAddress, Role) VALUES (:number, :password, :fullName, :homeAddress, 4)");
        query.bindValue(":number", user.numberPhone);
        query.bindValue(":password", user.Password);
        query.bindValue(":fullName", user.fullName);
        query.bindValue(":homeAddress", user.homeAddress);
        isOk = query.exec();

        if(!isOk) {QMessageBox::critical(this, "Внимание!", "Ошибка запроса!"); return;}

        QMessageBox::information(this, "Внимание!", "Вы успешно добавлены в систему!");

    });

    connect(ui->settingsPushButton, &QPushButton::clicked, this, []()
    {
        Settings* settings = new Settings(true);
        settings->setAttribute(Qt::WA_DeleteOnClose);
        settings->show();
    });

    auto textChanged = [this]()
    {
        if(ui->authenticationNumberPhoneLineEdit->text().isEmpty() || ui->authenticationPasswordLineEdit->text().isEmpty()) {

            ui->authenticationPushButton->setDisabled(true);

        } else {

            ui->authenticationPushButton->setDisabled(false);
        }
    };

    connect(ui->authenticationNumberPhoneLineEdit, &QLineEdit::textChanged, this, textChanged);
    connect(ui->authenticationPasswordLineEdit, &QLineEdit::textChanged, this, textChanged);

}

Authentication::~Authentication()
{
    delete ui;
}

//Авторизация в аккаунт
void Authentication::on_authenticationPushButton_clicked()
{
    userInfo user;
    QSqlQuery query;

    isOk = query.exec("SELECT * FROM Users");
    bool flag = false;

    if(!isOk) {QMessageBox::critical(this, "Внимание!","Ошибка запроса!"); return;}

    user.numberPhone = ui->authenticationNumberPhoneLineEdit->text();
    user.Password = ui->authenticationPasswordLineEdit->text();

    while(query.next()) {

        if(query.value(0).toString() == user.numberPhone && query.value(1).toString() == user.Password){

            QMessageBox::information(this, "Внимание!", "Вход выполнен!");

            user.role = query.value(4).toInt();
            flag = true;

            break;

        }
    }

    if(!flag) {QMessageBox::critical(this, "Внимание!", "Неправильный логин или пароль!"); return;}

    UserPersonalAccount* admin = new UserPersonalAccount(user.numberPhone);
    admin->setAttribute(Qt::WA_DeleteOnClose);
    close();
    admin->show();
}

void Authentication::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Return) on_authenticationPushButton_clicked();
}
