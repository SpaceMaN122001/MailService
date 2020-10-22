#include "historyofalllshipments.h"
#include "ui_historyofalllshipments.h"

HistoryOfAlllShipments::HistoryOfAlllShipments(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryOfAlllShipments)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/russiamail.png"));

    ui->parcelsHistoriesTableWidget->setRowCount(1);
    ui->parcelsHistoriesTableWidget->setColumnCount(4);
    ui->parcelsHistoriesTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Статус"));
    ui->parcelsHistoriesTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Отделение"));
    ui->parcelsHistoriesTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Дата"));
    ui->parcelsHistoriesTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Время"));

    ui->parcelsHistoriesTableWidget->verticalHeader()->setVisible(false);
    ui->parcelsHistoriesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->parcelsHistoriesTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->parcelsHistoriesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->parcelsHistoriesTableWidget->horizontalHeader()->setStyleSheet("font-weight: bold;");

    updateTracknumbers();
}

HistoryOfAlllShipments::~HistoryOfAlllShipments()
{
    delete ui;
}

VOID HistoryOfAlllShipments::updateTracknumbers()
{
    QSqlQuery query;
    query.exec("SELECT * FROM Parcels");
    ui->allTrackNumbersComboBox->clear();

    while(query.next()) ui->allTrackNumbersComboBox->addItem(query.value(0).toString());
}

void HistoryOfAlllShipments::on_updateTrackNumbersPushButton_clicked()
{
    updateTracknumbers();
}

//выбираем трэк номер из списка
void HistoryOfAlllShipments::on_allTrackNumbersComboBox_currentTextChanged(const QString &arg1)
{
    //чистить таблицу
    for(int row = 0; row < ui->parcelsHistoriesTableWidget->rowCount(); row++) {

        for(int column = 0; column < ui->parcelsHistoriesTableWidget->columnCount(); column++) {

            ui->parcelsHistoriesTableWidget->setItem(row, column, new QTableWidgetItem(""));
        }
    }

    _selectedTracknumber = ui->allTrackNumbersComboBox->currentText();
    INT rowCount = 0;

    //Считаем количество строк для истории
    QSqlQuery query;
    query.prepare("SELECT * FROM ShipmentHistory WHERE TrackNumber = :tracknumber");
    query.bindValue(":tracknumber", _selectedTracknumber);
    query.exec();
    while(query.next()) rowCount++;

    ui->parcelsHistoriesTableWidget->setRowCount(rowCount);

    query.prepare("SELECT * FROM ShipmentHistory WHERE TrackNumber = :tracknumber");
    query.bindValue(":tracknumber", _selectedTracknumber);
    query.exec();

    int row = 0;
    int i = 2;
    while(query.next()) {

        for(int column = 0; column < ui->parcelsHistoriesTableWidget->columnCount(); column++) {

            ui->parcelsHistoriesTableWidget->setItem(row, column, new QTableWidgetItem(query.value(i).toString()));

            i++;
        }

        qDebug() << "" << "\n";
        i = 2;
        row++;
    }
}

void HistoryOfAlllShipments::on_closePushButton_clicked()
{
    close();
}
