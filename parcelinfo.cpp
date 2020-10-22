#include "parcelinfo.h"
#include "ui_parcelinfo.h"

ParcelInfo::ParcelInfo(QString trackNumber, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParcelInfo)
{
    ui->setupUi(this);
    setWindowTitle("Информация о посылке");
    setWindowIcon(QIcon(":/res/package.png"));
    ui->parcelIconPixmap->setPixmap(QPixmap(":/res/package.png").scaled(16, 16));

    ui->parcelInfoTableWidget->setRowCount(1);
    ui->parcelInfoTableWidget->setColumnCount(11);
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Трэк номер"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Адрес отправителя"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Адрес получателя"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Тип посылки"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Класс отправки"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Период доставки"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Цена за 1 кг"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Вес"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Заявленная ценность"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("Итоговая стоимость"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(10, new QTableWidgetItem("Индекс отправителя"));
    ui->parcelInfoTableWidget->setHorizontalHeaderItem(11, new QTableWidgetItem("Индекс получателя"));

    ui->parcelInfoTableWidget->verticalHeader()->setVisible(false);
    ui->parcelInfoTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->parcelInfoTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->parcelInfoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->parcelInfoTableWidget->horizontalHeader()->setStyleSheet("font-weight: bold;");

    QSqlQuery query;
    query.prepare("SELECT * FROM Parcels WHERE TrackNumber = :tracknumber");
    query.bindValue(":tracknumber", trackNumber);
    query.exec();

        while(query.next()) {

            for(int k = 0; k < ui->parcelInfoTableWidget->columnCount(); k++) {

                ui->parcelInfoTableWidget->setItem(0, k, new QTableWidgetItem(query.value(k).toString()));
            }
        }

    connect(ui->closePushButton, &QPushButton::clicked, this, [this]()
    {
        close();
    });
}

ParcelInfo::~ParcelInfo()
{
    delete ui;
}
