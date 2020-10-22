QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accountinfo.cpp \
    addinguser.cpp \
    authentication.cpp \
    changepostofficeemployeeid.cpp \
    historyofalllshipments.cpp \
    main.cpp \
    mailservice.cpp \
    parcelinfo.cpp \
    parcelmanagement.cpp \
    settings.cpp \
    userpersonalaccount.cpp

HEADERS += \
    accountinfo.h \
    addinguser.h \
    authentication.h \
    changepostofficeemployeeid.h \
    historyofalllshipments.h \
    mailservice.h \
    parcelinfo.h \
    parcelmanagement.h \
    settings.h \
    userpersonalaccount.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    accountinfo.ui \
    addinguser.ui \
    authentication.ui \
    changepostofficeemployeeid.ui \
    historyofalllshipments.ui \
    parcelinfo.ui \
    parcelmanagement.ui \
    settings.ui \
    userpersonalaccount.ui

RESOURCES += \
    res.qrc
