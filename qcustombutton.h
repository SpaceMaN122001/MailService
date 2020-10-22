#ifndef QCUSTOMBUTTON_H
#define QCUSTOMBUTTON_H
#include <QWidget>
#include <QToolButton>

class QCustomButton
{
public:
    static void setCustomButtonStyle(QToolButton& button, QString imagePath, QString toolTip, int width, int heigth)
    {
        QSize iconSize(width, heigth);
        QPixmap map(imagePath);
        QIcon icon(map);
        button.setIcon(icon);
        button.setIconSize(iconSize);
        button.setStyleSheet("QToolButton { background-color: transparent; }");
        button.setToolTip(toolTip);
    }
};

#endif // QCUSTOMBUTTON_H
