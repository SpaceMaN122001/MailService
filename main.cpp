#include "authentication.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SpaceMan(Nikita Kravchenko)");
    QCoreApplication::setApplicationName("MailService");
    QCoreApplication::setApplicationVersion("0.1");
    QApplication a(argc, argv);
    Authentication w;
    w.show();
    return a.exec();
}
