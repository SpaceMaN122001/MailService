#include "parcelmanagement.h"
#include "ui_parcelmanagement.h"

ParcelManagement::ParcelManagement(QString login, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParcelManagement)
{
    _login = login;

    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/russiamail.png"));
    ui->receivedParcelsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->senderAddressIconPixmap->setPixmap(QPixmap(":/res/homeaddress.png").scaled(16, 16));
    ui->recipientAddressIconPixmap->setPixmap(QPixmap(":/res/homeaddress.png").scaled(16, 16));
    ui->typeIconPixmap->setPixmap(QPixmap(":/res/type.png").scaled(16, 16));
    ui->departureClassIconPixmap->setPixmap(QPixmap(":/res/class.png").scaled(16, 16));
    ui->costOfDeliveryIconPixmap->setPixmap(QPixmap(":/res/price.png").scaled(16, 16));
    ui->weightIconPixmap->setPixmap(QPixmap(":/res/weight.png").scaled(16, 16));
    ui->senderIndexIconPixmap->setPixmap(QPixmap(":/res/index.png").scaled(16, 16));
    ui->recipientIndexIconPixmap->setPixmap(QPixmap(":/res/index.png").scaled(16, 16));
    ui->totalColstIconPixmap->setPixmap(QPixmap(":/res/resprice.png").scaled(16, 16));
    ui->tacknumberIconPixmap->setPixmap(QPixmap(":/res/tracknumber.png").scaled(16, 16));
    ui->parcelWeightIconPixmap->setPixmap(QPixmap(":/res/weight.png").scaled(16, 16));
    ui->selectParceliconPixmap->setPixmap(QPixmap(":/res/parcel.png").scaled(16, 16));
    ui->declaredValueIconPixmap->setPixmap(QPixmap(":/res/declaredvalue.png").scaled(16, 16));
    ui->deliveryPeriodIconPixmap->setPixmap(QPixmap(":/res/deliveryperiod.png").scaled(16, 16));
    ui->receiverParceliconPixmap->setPixmap(QPixmap(":/res/parcel.png").scaled(16, 16));
    ui->senderIndexLineEdit->setMaxLength(6);
    ui->senderIndexLineEdit->setInputMask("999999");
    ui->recipienIndextLineEdit->setMaxLength(6);
    ui->recipienIndextLineEdit->setInputMask("999999");
    ui->deliveryPeriodLineEdit->setText("10");
    ui->costOfDeliveryLineEdit->setText("0.5");
    ui->declaredValueLineEdit->setText("0");
    ui->senderIndexLineEdit->setText("347935");
    ui->recipienIndextLineEdit->setText("346783");
    ui->mainTabWidget->setTabIcon(0, QIcon(":/res/send.png"));
    ui->mainTabWidget->setTabIcon(1, QIcon(":/res/accept.png"));
    ui->receivedParcelsListWidget->setIconSize(QSize(24, 24));
    ui->acceptParcelPushButton->setDisabled(true);
    ui->parcelInfoPushButton->setDisabled(true);
    ui->parcelWeightLineEdit->setDisabled(true);
    ui->sendNextPushButton->setDisabled(true);

    if(ui->allParcelsComboBox->count() > 0) {

        ui->acceptParcelPushButton->setDisabled(false);
        ui->parcelInfoPushButton->setDisabled(false);
        ui->parcelWeightLineEdit->setDisabled(false);
    }

    if(ui->receivedParcelsListWidget->count() == 0) {

        ui->receivedParcelsListWidget->setDisabled(true);
    }

    _postOffices.push_back("344000");
    _postOffices.push_back("347900");
    _postOffices.push_back("347935");
    _postOffices.push_back("347913");
    _postOffices.push_back("347928");
    _postOffices.push_back("346400");
    _postOffices.push_back("346424");
    _postOffices.push_back("346414");
    _postOffices.push_back("346448");
    _postOffices.push_back("346330");
    _postOffices.push_back("346333");
    _postOffices.push_back("346335");
    _postOffices.push_back("346342");
    _postOffices.push_back("346780");
    _postOffices.push_back("346783");
    _postOffices.push_back("346789");
    _postOffices.push_back("346781");

    QCustomButton::setCustomButtonStyle(*ui->calculateTotalCostToolButton, ":/res/calculate.png", "Рассчитать итоговую стоимость", 32, 32);
    QCustomButton::setCustomButtonStyle(*ui->generateTrackNumberToolButton, ":/res/tracknumber.png", "Сгенерировать трэк номер", 32, 32);

    //получаем текущий идентификатор
    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE NumberPhone = :login");
    query.bindValue(":login", _login);
    query.exec();

    while(query.next()) _identifier = query.value(5).toString().remove('R');

    updateParcelsList();
    updateListOfReceivedParcels();
}

VOID ParcelManagement::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Return) {

        qDebug () << ui->mainTabWidget->currentIndex() << "\n";
    }
}

ParcelManagement::~ParcelManagement()
{
    delete ui;
}

VOID ParcelManagement::updateParcelsList()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Parcels WHERE NextBranch = :identifier AND Status = :status");
    query.bindValue(":identifier", _identifier);
    query.bindValue(":status", 0);
    query.exec();
    ui->allParcelsComboBox->clear();

    while(query.next()) {

        ui->allParcelsComboBox->addItem(query.value(0).toString());
        ui->parcelWeightLineEdit->setText(query.value(7).toString());
    }
}

VOID ParcelManagement::updateListOfReceivedParcels()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Parcels WHERE NextBranch = :identifier AND Status = :status");
    query.bindValue(":identifier", _identifier);
    query.bindValue(":status", 1);
    query.exec();
    ui->receivedParcelsListWidget->clear();

    while(query.next()) {

        if(query.value(11).toString() == _identifier) {

            ui->receivedParcelsListWidget->addItem(new QListWidgetItem(QIcon(":/res/address.png"), query.value(0).toString()));

        } else {

            ui->receivedParcelsListWidget->addItem(new QListWidgetItem(QIcon(":/res/package.png"), query.value(0).toString()));
        }
    }
}

QString ParcelManagement::getFirstFourDigits(QString s)
{
    QString s1 = "";
    int i = 0;

    for(QChar c : s) {

        s1 += c;

        i++;

        if(i == 4) return s1;
    }

    return "";
}

void ParcelManagement::on_receivedParcelsListWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->receivedParcelsListWidget->count() != 0) {

        _SelectedAcceptedParcel = ui->receivedParcelsListWidget->currentItem()->text();
    }

    QPoint globalPos = ui->receivedParcelsListWidget->mapToGlobal(pos);

    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    QAction* parcelInfoAction = new QAction(QPixmap(":/res/tracknumber.png"), "&Информация о посылке", this);
    QAction* returnParcel = new QAction(QPixmap(":/res/tracknumber.png"), "&Вернуть посылку", this);

    connect(parcelInfoAction, &QAction::triggered, this, [this]()
    {
        ParcelInfo* info = new ParcelInfo(_SelectedAcceptedParcel);
        info->setAttribute(Qt::WA_DeleteOnClose);
        info->resize(QApplication::desktop()->width(), 600);
        info->show();
    });

    connect(returnParcel, &QAction::triggered, this, [this]()
    {
        //являемся ли мы конечным отделением
        QSqlQuery query;
        query.prepare("SELECT * FROM Parcels WHERE NextBranch = :identifier AND Status = :status");
        query.bindValue(":identifier", _identifier);
        query.bindValue(":status", 1);
        query.exec();

        while(query.next()) {

            if(query.value(11).toString() == _identifier) {

                //ui->sendNextPushButton->setDisabled(true);

                int result = QMessageBox::information(this, "Внимание!", "Вы хотите отдать посылку?", QMessageBox::Yes | QMessageBox::No);

                if(result == QMessageBox::Yes) {

                    query.prepare("UPDATE Parcels SET Status = :status WHERE TrackNumber = :tracknumber");
                    query.bindValue(":status", Delivered);
                    query.bindValue(":tracknumber", _SelectedAcceptedParcel);
                    query.exec();

                    updateListOfReceivedParcels();

                    //История: мы достигли пункта назначения и ставим статус завершенно
                    query.prepare("INSERT INTO ShipmentHistory (TrackNumber, Status, BranchId, Date, Time) VALUES (:tracknumber, :status, :branchId,  :date, :time)");
                    query.bindValue(":tracknumber", _SelectedAcceptedParcel);
                    query.bindValue(":status", "Получено в");
                    query.bindValue(":branchId", _identifier);
                    query.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
                    query.bindValue(":time", QTime::currentTime().toString("hh:mm:ss"));
                    query.exec();

                    if(ui->receivedParcelsListWidget->count() == 0) {

                        ui->receivedParcelsListWidget->setDisabled(true);
                    }
                }

            } else {

                QMessageBox::critical(this, "Внимание!", "Посылка еще не достигла назначителя!");
                //ui->sendNextPushButton->setDisabled(false);
            }
        }
    });

    menu->addAction(parcelInfoAction);
    menu->addAction(returnParcel);
    menu->exec(globalPos);
}

//генерация трэк номера
void ParcelManagement::on_generateTrackNumberToolButton_clicked()
{
    QString senderIndex = ui->senderIndexLineEdit->text();
    QString recipientIndex = ui->recipienIndextLineEdit->text();

    if(senderIndex.isEmpty()) {

        QMessageBox::critical(this, "Внимание!", "Вы не указали индекс отправителя!");

        return;
    }

    if(recipientIndex.isEmpty()) {

        QMessageBox::critical(this, "Внимание!", "Вы не указали индекс получателя!");

        return;
    }

    if(senderIndex.remove(' ') == nullptr || recipientIndex.remove(' ') == nullptr) {QMessageBox::critical(this, "Внимание!", "Вы не указали индекс получателя или отправителя!"); return;}

    parcel.senderIndex = ui->senderIndexLineEdit->text();
    parcel.recipientIndex = ui->recipienIndextLineEdit->text();
    parcel.trackNumber = "RR" + QDate::currentDate().toString("ddMMyyyy") + "1" + parcel.senderIndex + parcel.recipientIndex;

    ui->tracknumberLineEdit->setText(parcel.trackNumber);
}

//итоговая стоимость
void ParcelManagement::on_calculateTotalCostToolButton_clicked()
{
    if(ui->weightLineEdit->text().isEmpty()) {

        QMessageBox::critical(this, "Внимание!", "Вы не указали вес посылки!");

        return;
    }

    if(ui->declaredValueLineEdit->text().isEmpty()) {

        QMessageBox::critical(this, "Внимание!", "Вы не указали заявленную ценность!");

        return;
    }

    int costOfDelivery = parcel.costOfDelivery.toInt();
    int weight = ui->weightLineEdit->text().toInt();
    int declaredValue = ui->declaredValueLineEdit->text().toInt();

    DOUBLE result = costOfDelivery * weight + declaredValue * 0.2;

    parcel.totalCost = QString::number(result);

    ui->totalCostLineEdit->setText(parcel.totalCost);
}

//отправить посылку
void ParcelManagement::on_sendParcelPushButton_clicked()
{
    QString senderAddress = ui->senderAddressLineEdit->text();
    QString recipientAddress = ui->recipientAddressLineEdit->text();
    QString weight = ui->weightLineEdit->text();
    QString declaredValue = ui->declaredValueLineEdit->text();

    if(senderAddress.remove(' ') == nullptr || recipientAddress.remove(' ') == nullptr || weight.remove(' ') == nullptr || declaredValue.remove(' ') == nullptr) {

        QMessageBox::critical(this, "Внимание!", "Вы указали не все данные!");

        return;
    }

    if(ui->weightLineEdit->text().toInt() > 10) {

        QMessageBox::critical(this, "Внимание!", "Вес не может привышать 10 кг!");

        return;
    }

    parcel.senderAddress = ui->senderAddressLineEdit->text();
    parcel.recipientAddress = ui->recipientAddressLineEdit->text();
    parcel.type = ui->parcelTypeComboBox->currentText();
    parcel.departureClass = ui->deparrtureClassComboBox->currentText();
    parcel.deliveryPeriod = ui->deliveryPeriodLineEdit->text();
    parcel.costOfDelivery = ui->costOfDeliveryLineEdit->text();
    parcel.weight = ui->weightLineEdit->text();
    parcel.declaredValue = ui->declaredValueLineEdit->text();

    /*qDebug () << "Трэк номер: " << parcel.trackNumber << "\n";
    qDebug () << "Адрес отправителя: " << parcel.senderAddress << "\n";
    qDebug () << "Адрес получателя: " << parcel.recipientAddress << "\n";
    qDebug () << "Тип посылки: " << parcel.type << "\n";
    qDebug () << "Класс отправления: " << parcel.departureClass << "\n";
    qDebug () << "Стоимость за 1 кг: " << parcel.costOfDelivery << "\n";
    qDebug () << "Вес: " << parcel.weight << "\n";
    qDebug () << "Индекс отправителя: " << parcel.senderIndex << "\n";
    qDebug () << "Индекс получателя: " << parcel.recipientIndex << "\n";
    qDebug () << "Объявленная ценность: " << parcel.declaredValue << "\n";
    qDebug () << "Срок доставки: " << parcel.deliveryPeriod << "\n";
    qDebug () << "Итоговая стоимость: " << parcel.totalCost << "\n";*/

    QString index = parcel.senderIndex;
    QString recipientIndex = parcel.recipientIndex;
    INT maxLenght = 5;

    if(index[maxLenght] != '0') {

        qDebug() << "Не главное!" << "\n";

        for(int i = 0; i < _postOffices.size(); i++) {

            if(getFirstFourDigits(index) == getFirstFourDigits(_postOffices[i])) {

                QString currentIndex = _postOffices[i];

                if(currentIndex[maxLenght] == '0') parcel.nextBranch = currentIndex;
            }
        }

    } else if(index[maxLenght] == '0' && index != "344000") {

        qDebug() << "Главное!" << "\n";

        parcel.nextBranch = "344000";

    } else if (index == "344000") {

        for(int i = 0; i < _postOffices.size(); i++) {

            if(getFirstFourDigits(recipientIndex) == getFirstFourDigits(_postOffices[i])) {

                QString currentIndex = _postOffices[i];

                if(currentIndex[5] == '0') parcel.nextBranch = currentIndex;
            }
        }

        qDebug() << "ИЗ ростова в Главное получателя!" << "\n";
    }

    parcel.status = InTransit;

    QSqlQuery query;
    query.prepare("INSERT INTO Parcels (TrackNumber, SenderAddress, RecipientAddress, Type, DepartureClass, DeliveryPeriod, CostOfDelivery, Weight, DeclaredValue, TotalCost, SenderIndex, RecipientIndex, "
                  "Status, NextBranch) VALUES (:tracknumber, :senderAddress, :recipientAddress, :type, :departureClass, :deliveryPeriod, :costOfDelivery, :weight, :declaredValue, :totalCost, :senderIndex, "
                  ":recipientIndex, :status , :nextBranch)");
    query.bindValue(":tracknumber", parcel.trackNumber);
    query.bindValue(":senderAddress", parcel.senderAddress);
    query.bindValue(":recipientAddress", parcel.recipientAddress);
    query.bindValue(":type", parcel.type);
    query.bindValue(":departureClass", parcel.departureClass);
    query.bindValue(":deliveryPeriod", parcel.deliveryPeriod);
    query.bindValue(":costOfDelivery", parcel.costOfDelivery);
    query.bindValue(":weight", parcel.weight);
    query.bindValue(":declaredValue", parcel.declaredValue);
    query.bindValue(":totalCost", parcel.totalCost);
    query.bindValue(":senderIndex", parcel.senderIndex);
    query.bindValue(":recipientIndex", parcel.recipientIndex);
    query.bindValue(":status", parcel.status);
    query.bindValue(":nextBranch", parcel.nextBranch);
    bool isOk = query.exec();

    if(isOk) QMessageBox::information(this, "Внимание!", "Посылка отправлена!");

    else if (!isOk) QMessageBox::critical(this, "Внимание!", "Возникла ошибка при запросе!");

    //История
    query.prepare("INSERT INTO ShipmentHistory (TrackNumber, Status, BranchId, Date, Time) VALUES (:tracknumber, :status, :branchId,  :date, :time)");
    query.bindValue(":tracknumber", parcel.trackNumber);
    query.bindValue(":status", "Отправлено из");
    query.bindValue(":branchId", parcel.senderIndex);
    query.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
    query.bindValue(":time", QTime::currentTime().toString("hh:mm:ss"));
    isOk = query.exec();

    if(isOk) QMessageBox::information(this, "Внимание!", "История Записалась!");

    else if (!isOk) QMessageBox::critical(this, "Внимание!", "История не записалась!");
}

//выбор класса доставки
void ParcelManagement::on_deparrtureClassComboBox_currentTextChanged(const QString &arg1)
{
    arg1.isEmpty();

    int departureClass = ui->deparrtureClassComboBox->currentText().toInt();

    if(departureClass == 1) {

        ui->deliveryPeriodLineEdit->setText("10");
        ui->costOfDeliveryLineEdit->setText("0.5");

    } else if(departureClass == 2) {

        ui->deliveryPeriodLineEdit->setText("15");
        ui->costOfDeliveryLineEdit->setText("0.3");

    } else if(departureClass == 3) {

        ui->deliveryPeriodLineEdit->setText("20");
        ui->costOfDeliveryLineEdit->setText("0.1");
    }
}

//выбор трэк номера из списка всех трэк номеров
void ParcelManagement::on_allParcelsComboBox_currentTextChanged(const QString &arg1)
{
    arg1.isEmpty();

    if(ui->allParcelsComboBox->count() > 0) {

        ui->acceptParcelPushButton->setDisabled(false);
        ui->parcelInfoPushButton->setDisabled(false);
        ui->parcelWeightLineEdit->setDisabled(false);

    } else {

        ui->acceptParcelPushButton->setDisabled(true);
        ui->parcelInfoPushButton->setDisabled(true);
        ui->parcelWeightLineEdit->setDisabled(true);
    }

    //устанавливаем вес
    _selectedTracknumber = ui->allParcelsComboBox->currentText();
    QSqlQuery query;
    query.prepare("SELECT * FROM Parcels WHERE TrackNumber = :tracknumber");
    query.bindValue(":tracknumber", ui->allParcelsComboBox->currentText());
    query.exec();

    while(query.next()) {

        ui->parcelWeightLineEdit->setText(query.value(7).toString());
    }
}

//Принятие посылки
void ParcelManagement::on_acceptParcelPushButton_clicked()
{
    //меняем статус и вес
    QSqlQuery query;
    query.prepare("UPDATE Parcels SET Status = :status, Weight = :weight WHERE TrackNumber = :tracknumber");
    query.bindValue(":weight", ui->parcelWeightLineEdit->text());
    query.bindValue(":status", 1);
    query.bindValue(":tracknumber", _selectedTracknumber);
    query.exec();

    query.prepare("SELECT * FROM Parcels WHERE TrackNumber = :tracknumber");
    query.bindValue(":tracknumber", _selectedTracknumber);
    query.exec();

    while(query.next()) {

        if(query.value(11).toString() == _identifier) {


            //История: Принято в
            query.prepare("INSERT INTO ShipmentHistory (TrackNumber, Status, BranchId, Date, Time) VALUES (:tracknumber, :status, :branchId,  :date, :time)");
            query.bindValue(":tracknumber", _selectedTracknumber);
            query.bindValue(":status", "Принято в");
            query.bindValue(":branchId", _identifier);
            query.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
            query.bindValue(":time", QTime::currentTime().toString("hh:mm:ss"));
            query.exec();

            //История: Ожидает получения
            query.prepare("INSERT INTO ShipmentHistory (TrackNumber, Status, BranchId, Date, Time) VALUES (:tracknumber, :status, :branchId,  :date, :time)");
            query.bindValue(":tracknumber", _selectedTracknumber);
            query.bindValue(":status", "Ожидает получения");
            query.bindValue(":branchId", _identifier);
            query.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
            query.bindValue(":time", QTime::currentTime().toString("hh:mm:ss"));
            query.exec();

        } else {

            //История: Принято в
            query.prepare("INSERT INTO ShipmentHistory (TrackNumber, Status, BranchId, Date, Time) VALUES (:tracknumber, :status, :branchId,  :date, :time)");
            query.bindValue(":tracknumber", _selectedTracknumber);
            query.bindValue(":status", "Принято в");
            query.bindValue(":branchId", _identifier);
            query.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
            query.bindValue(":time", QTime::currentTime().toString("hh:mm:ss"));
            query.exec();
        }
    }

    ui->parcelWeightLineEdit->setText("");
    updateListOfReceivedParcels();
    updateParcelsList();

    ui->receivedParcelsListWidget->setDisabled(false);
}

//обновляем все посылки
void ParcelManagement::on_updateParcelsPushButton_clicked()
{
    updateListOfReceivedParcels();
    updateParcelsList();
}

//Выбираем принятую посылку из списка
void ParcelManagement::on_receivedParcelsListWidget_itemClicked(QListWidgetItem *item)
{
    item->text().isEmpty();

    _SelectedAcceptedParcel = ui->receivedParcelsListWidget->currentItem()->text();

    //Отключаем кнопку если посылка пришла к нам на выдачу назначителю
    QSqlQuery query;
    query.prepare("SELECT * FROM Parcels WHERE NextBranch = :identifier AND Status = :status");
    query.bindValue(":identifier", _identifier);
    query.bindValue(":status", 1);
    query.exec();

    while(query.next()) {

        if(query.value(11).toString() == _identifier) {

             ui->sendNextPushButton->setDisabled(true);

        } else {

            ui->sendNextPushButton->setDisabled(false);
        }
    }
}

//Отправляем дальше
void ParcelManagement::on_sendNextPushButton_clicked()
{
    //Берем индекс получателя
    QSqlQuery query;
    QString nextIndex = "";
    QString recipientIndex = "";
    query.prepare("SELECT * FROM Parcels WHERE TrackNumber = :selectedAcceptedParcel");
    query.bindValue(":selectedAcceptedParcel", _SelectedAcceptedParcel);
    query.exec();

    while(query.next()) {

        recipientIndex = query.value(11).toString();
    }

    //Отправляем на Ростов
    if(_identifier[5] == '0' && _identifier != "344000" && getFirstFourDigits(_identifier) != getFirstFourDigits(recipientIndex)) {

        qDebug () << "На Ростов!" << "\n";
        nextIndex = "344000";

        //Ищем главное отделение получателя
    } else if(_identifier == "344000") {

        qDebug () << "Ищем главное отделение получателя" << "\n";

        for(int i = 0; i < _postOffices.size(); i++) {

            if(getFirstFourDigits(recipientIndex) == getFirstFourDigits(_postOffices[i])) {

                QString currentIndex = _postOffices[i];

                if(currentIndex[5] == '0') nextIndex = currentIndex;
            }
        }

        //Отправить не на главное
    } else if(_identifier[5] == '0' && _identifier != "344000" && getFirstFourDigits(_identifier) == getFirstFourDigits(recipientIndex)) {

        qDebug () << "Отправить не на главное!" << "\n";

        nextIndex = recipientIndex;
    }

    query.prepare("UPDATE Parcels SET Status = :status, NextBranch = :nextBranch WHERE TrackNumber = :tracknumber");
    query.bindValue(":nextBranch", nextIndex);
    query.bindValue(":status", InTransit);
    query.bindValue(":tracknumber", _SelectedAcceptedParcel);
    query.exec();

    updateListOfReceivedParcels();
    updateParcelsList();

    //История: Отправлено транзитом из
    query.prepare("INSERT INTO ShipmentHistory (TrackNumber, Status, BranchId, Date, Time) VALUES (:tracknumber, :status, :branchId,  :date, :time)");
    query.bindValue(":tracknumber", _SelectedAcceptedParcel);
    query.bindValue(":status", "Отправлено транзитом из");
    query.bindValue(":branchId", _identifier);
    query.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
    query.bindValue(":time", QTime::currentTime().toString("hh:mm:ss"));
    query.exec();

    if(ui->receivedParcelsListWidget->count() == 0) {

        ui->receivedParcelsListWidget->setDisabled(true);
    }
}

void ParcelManagement::on_closePushButton_clicked()
{
    close();
}

void ParcelManagement::on_closePushButton2_clicked()
{
    close();
}

void ParcelManagement::on_parcelInfoPushButton_clicked()
{
    ParcelInfo* info = new ParcelInfo(ui->allParcelsComboBox->currentText());
    info->setAttribute(Qt::WA_DeleteOnClose);
    info->resize(QApplication::desktop()->width(), 600);
    info->show();
}
