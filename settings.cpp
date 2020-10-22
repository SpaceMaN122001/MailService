#include "settings.h"
#include "ui_settings.h"

Settings::Settings(bool dbConnectStatus, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    _dbConnectStatus = dbConnectStatus;

    ui->setupUi(this);
    ui->serverIconPixmap->setPixmap(QPixmap(":/res/server.ico").scaled(16, 16));
    ui->userNameIconPixMap->setPixmap(QPixmap(":/res/user.ico").scaled(16, 16));
    ui->passwordIconPixMap->setPixmap(QPixmap(":/res/password.ico").scaled(16, 16));
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    setWindowIcon(QIcon(":/res/settings.png"));
    setWindowTitle("Настройки");

    _pathToSettingsFile = "C:\\Users\\" + QDir::home().dirName() + "\\AppData\\Local\\MailService\\settings.ini";

    QFile file(_pathToSettingsFile);

    if(!file.exists()) {QMessageBox::critical(this, "Внимание!", "Отстуствует файл конфигурации!"); QApplication::quit();}

    QSettings settings(_pathToSettingsFile, QSettings::IniFormat);
    ui->serverNameLineEdit->setText(settings.value("DataBase/Server").toString());
    ui->userNameLineEdit->setText(settings.value("DataBase/UserName").toString());
    ui->passwordLineEdit->setText(settings.value("DataBase/Password").toString());

    connect(ui->applyPushButton, &QPushButton::clicked, this, [this]()
    {

        QSettings settings(_pathToSettingsFile, QSettings::IniFormat);
        settings.beginGroup("DataBase");
        settings.setValue("Server", ui->serverNameLineEdit->text());
        settings.setValue("UserName", ui->userNameLineEdit->text());
        settings.setValue("Password", ui->passwordLineEdit->text());
        settings.endGroup();

        QMessageBox::information(this, "Внимание!", "Настройки сохранены!");

        close();

        if(!_dbConnectStatus) QApplication::quit();
    });

    connect(ui->cancelPushButton, &QPushButton::clicked, this, [this]()
    {
        if(!_dbConnectStatus) QApplication::quit();

        if(_dbConnectStatus) close();
    });
}

Settings::~Settings()
{
    delete ui;
}
