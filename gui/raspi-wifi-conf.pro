include(wpalib.pri)

# Set the following option true to include a WiFi-Network Scanning option
# Note that the build-size will increase significantly!
WITH_NETWORK = true


equals(WITH_NETWORK, true) {
QT       += core gui network
DEFINES  += USE_NETWORK
}else{
QT       += core gui
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editheaders.cpp \
    infolabel.cpp \
    main.cpp \
    mainwindow.cpp \
    rpi-header.cpp \
    wifilist.cpp

HEADERS += \
    editheaders.h \
    infolabel.h \
    mainwindow.h \
    rpi-header.h \
    wifilist.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

TRANSLATIONS += languages/lang_de.ts

LIBS += -L$${PWD}/incl -Wl,-rpath=$${PWD}/incl $${PWD}/incl/_libwpa_passphrase.so
