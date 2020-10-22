#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "mailservice.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(bool dbConnectStatus = true, QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    QString _pathToSettingsFile;
    bool _dbConnectStatus;
};

#endif // SETTINGS_H
