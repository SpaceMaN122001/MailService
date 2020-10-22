#ifndef HISTORYOFALLLSHIPMENTS_H
#define HISTORYOFALLLSHIPMENTS_H

#include <QWidget>
#include "mailservice.h"

namespace Ui {
class HistoryOfAlllShipments;
}

class HistoryOfAlllShipments : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryOfAlllShipments(QWidget *parent = nullptr);
    ~HistoryOfAlllShipments();

private slots:
    void on_updateTrackNumbersPushButton_clicked();

    void on_allTrackNumbersComboBox_currentTextChanged(const QString &arg1);

    void on_closePushButton_clicked();

private:
    Ui::HistoryOfAlllShipments *ui;
    VOID updateTracknumbers();
    QString _selectedTracknumber;
};

#endif // HISTORYOFALLLSHIPMENTS_H
