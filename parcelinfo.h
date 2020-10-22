#ifndef PARCELINFO_H
#define PARCELINFO_H

#include <QWidget>
#include "mailservice.h"

namespace Ui {
class ParcelInfo;
}

class ParcelInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ParcelInfo(QString trackNumber, QWidget *parent = nullptr);
    ~ParcelInfo();

private:
    Ui::ParcelInfo *ui;
};

#endif // PARCELINFO_H
