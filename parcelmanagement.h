#ifndef PARCELMANAGEMENT_H
#define PARCELMANAGEMENT_H

#include <QWidget>
#include <QDate>
#include <QVector>
#include <QKeyEvent>
#include <QMenu>
#include <QContextMenuEvent>
#include <QListWidgetItem>
#include <QDesktopWidget>
#include "mailservice.h"
#include "parcelinfo.h"

namespace Ui {
class ParcelManagement;
}

class ParcelManagement : public QWidget
{
    Q_OBJECT

public:
    explicit ParcelManagement(QString login, QWidget *parent = nullptr);
    ~ParcelManagement();

private:
    Ui::ParcelManagement *ui;
    QString _login;
    Parcel parcel;
    QString _identifier;
    QString _selectedTracknumber;
    QString _SelectedAcceptedParcel;
    QVector<QString> _postOffices;
    QString getFirstFourDigits(QString s);
    VOID updateParcelsList();
    VOID updateListOfReceivedParcels();

private slots:
    VOID keyPressEvent(QKeyEvent* event);
    void on_receivedParcelsListWidget_customContextMenuRequested(const QPoint &pos);
    void on_generateTrackNumberToolButton_clicked();
    void on_sendParcelPushButton_clicked();
    void on_calculateTotalCostToolButton_clicked();
    void on_deparrtureClassComboBox_currentTextChanged(const QString &arg1);
    void on_allParcelsComboBox_currentTextChanged(const QString &arg1);
    void on_acceptParcelPushButton_clicked();
    void on_updateParcelsPushButton_clicked();
    void on_receivedParcelsListWidget_itemClicked(QListWidgetItem *item);
    void on_sendNextPushButton_clicked();
    void on_closePushButton_clicked();
    void on_closePushButton2_clicked();
    void on_parcelInfoPushButton_clicked();
};

#endif // PARCELMANAGEMENT_H
