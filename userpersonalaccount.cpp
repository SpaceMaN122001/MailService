#include "userpersonalaccount.h"
#include "ui_userpersonalaccount.h"

UserPersonalAccount::UserPersonalAccount(QString login, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserPersonalAccount)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/russiamail.png"));
    ui->chiefAdminIcoPixmap->setPixmap(QPixmap(":/res/chiefAdmin.png").scaled(64, 64));
    QCustomButton::setCustomButtonStyle(*ui->accountInfoToolButton, ":/res/account.png", "Информация об аккаунте", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->addUserToolButton, ":/res/adduser.ico", "Добавить администратора", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->historyOfAllShimpmentsToolButton, ":/res/history.ico", "История всех отправлений", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->authenticationFormToolButton, ":/res/authmenu.png", "Открыть меню аунтефикации", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->changePostOfficeEmployeeIDToolButton, ":/res/id.ico", "Изменить идентификатор сотрудника почтового отделения", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->mailManagmentToolButton, ":/res/mail.ico", "Управление посылками", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->userParcelsToolButton, ":/res/parcels.ico", "Мои посылки", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->removeAllParcelsToolButton, ":/res/server.ico", "Удалить все посылки", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->removeAllUsersToolButton, ":/res/user.ico", "Удалить всех пользователей", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->clearAllHistoriesParcelsToolButton, ":/res/clearhistory.png", "Очистить историю отправлений", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->shrinkDataBaseToolButton, ":/res/databasereset.png", "Сжать базу данных", 32, 32);

    ui->removeAllParcelsToolButton->hide();
    ui->label_9->hide();
    ui->removeAllUsersToolButton->hide();
    ui->label_10->hide();
    ui->clearAllHistoriesParcelsToolButton->hide();
    ui->label_11->hide();

    _login = login;
    INT role = -1;

    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE NumberPhone = :login");
    query.bindValue(":login", login);
    query.exec();

    while(query.next()) role = query.value(4).toInt();

    if(role == ChiefAdmin) {

        setWindowTitle("Главный администратор");
        ui->roleNameLabel->setText("Главный администратор");
        ui->mailManagmentToolButton->hide();
        ui->label_6->hide();
        ui->changePostOfficeEmployeeIDToolButton->hide();
        ui->label_7->hide();
        ui->userParcelsToolButton->hide();
        ui->label_8->hide();
        ui->removeAllParcelsToolButton->show();
        ui->label_9->show();
        ui->removeAllUsersToolButton->show();
        ui->label_10->show();
        ui->clearAllHistoriesParcelsToolButton->show();
        ui->label_11->show();

    } else if(role == Admin) {

        setWindowTitle("Администратор");
        ui->roleNameLabel->setText("Администратор");
        ui->mailManagmentToolButton->hide();
        ui->userParcelsToolButton->hide();
        ui->label_8->hide();
        ui->mailManagmentToolButton->hide();
        ui->label_7->hide();

    } else if(role == PostOffice) {

        setWindowTitle("Сотрудник почтового отделения");
        ui->roleNameLabel->setText("Сотрудник почтового отделения");
        ui->changePostOfficeEmployeeIDToolButton->hide();
        ui->label_6->hide();
        ui->userParcelsToolButton->hide();
        ui->label_8->hide();
        ui->addUserToolButton->hide();
        ui->label_3->hide();

    } else if(role == User) {

        setWindowTitle("Пользователь");
        ui->roleNameLabel->setText("Пользователь");
        ui->changePostOfficeEmployeeIDToolButton->hide();
        ui->label_7->hide();
        ui->mailManagmentToolButton->hide();
        ui->label_6->hide();
        ui->addUserToolButton->hide();
        ui->label_3->hide();
        ui->historyOfAllShimpmentsToolButton->hide();
        ui->label_4->hide();
    }

    connect(ui->accountInfoToolButton, &QToolButton::clicked, this, [this]()
    {

        AccountInfo* info = new AccountInfo(_login);
        info->setAttribute(Qt::WA_DeleteOnClose);
        info->show();
    });

    connect(ui->addUserToolButton, &QToolButton::clicked, this, [this]()
    {

        AddingUser* user = new AddingUser(_login);
        user->setAttribute(Qt::WA_DeleteOnClose);
        user->show();
    });

    connect(ui->authenticationFormToolButton, &QToolButton::clicked, this, [this]()
    {
        Authentication* auth = new Authentication;
        auth->setAttribute(Qt::WA_DeleteOnClose);
        close();
        auth->show();
    });

    connect(ui->mailManagmentToolButton, &QToolButton::clicked, this, [this]()
    {
        ParcelManagement* parcel = new ParcelManagement(_login);
        parcel->setAttribute(Qt::WA_DeleteOnClose);
        parcel->show();
    });
}

UserPersonalAccount::~UserPersonalAccount()
{
    delete ui;
}

void UserPersonalAccount::on_removeAllParcelsToolButton_clicked()
{
    int result = QMessageBox::question(this, "Внимание!", "Вы хотите удалить все посылки?", QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes) {

        QSqlQuery query;
        bool isOK = query.exec("DELETE FROM Parcels");

        if(isOK) QMessageBox::information(this, "Внимание!", "Все посылки успешно удалены!");

        if(!isOK) QMessageBox::critical(this, "Внимание!", "Посылки не были удалены!");
    }
}

void UserPersonalAccount::on_removeAllUsersToolButton_clicked()
{
    int result = QMessageBox::question(this, "Внимание!", "Вы хотите удалить всех пользователей?", QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes) {

        QSqlQuery query;
        query.prepare("DELETE FROM Users WHERE NumberPhone != :login");
        query.bindValue(":login", _login);
        bool isOK = query.exec();

        if(isOK) QMessageBox::information(this, "Внимание!", "Все пользователи удалены!");

        if(!isOK) QMessageBox::critical(this, "Внимание!", "Пользователине были удалены!");
    }
}

//История всех отправлений
void UserPersonalAccount::on_historyOfAllShimpmentsToolButton_clicked()
{
    HistoryOfAlllShipments* history = new HistoryOfAlllShipments;
    history->setAttribute(Qt::WA_DeleteOnClose);
    history->show();
}

void UserPersonalAccount::on_changePostOfficeEmployeeIDToolButton_clicked()
{
    ChangePostOfficeEmployeeID* change = new ChangePostOfficeEmployeeID;
    change->setAttribute(Qt::WA_DeleteOnClose);
    change->show();
}

void UserPersonalAccount::on_clearAllHistoriesParcelsToolButton_clicked()
{
    int result = QMessageBox::question(this, "Внимание!", "Вы очистить историю всех отправлений?", QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes) {

    QSqlQuery query;
    bool isOK = query.exec("TRUNCATE TABLE ShipmentHistory");

    if(isOK) QMessageBox::information(this, "Внимание!", "История посылок очищена!");

    if(!isOK) QMessageBox::critical(this, "Внимание!", "История не была очищена!");

    }
}

void UserPersonalAccount::on_shrinkDataBaseToolButton_clicked()
{
    int result = QMessageBox::question(this, "Внимание!", "Вы хотите удалить все посылки?", QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes) {

    QSqlQuery query;
    bool isOK = query.exec("DBCC SHRINKDATABASE('MailService')");

    if(isOK) QMessageBox::information(this, "Внимание!", "База данных сжата!");

    if(!isOK) QMessageBox::critical(this, "Внимание!", "База данных не была сжата!");

    }
}
