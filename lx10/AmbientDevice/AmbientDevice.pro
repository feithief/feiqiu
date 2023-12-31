#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T10:11:24
#
#-------------------------------------------------

QT       += core gui
QT += testlib

# QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AmbientDevice
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ambientlincomm.cpp \
    ambientlinscheduler.cpp \
    bcmmasterbutton.cpp \
    slavebutton.cpp \
    bcmmasterframe.cpp \
    slaveframeconfig.cpp \
    adialog.cpp \
    keyboard.cpp \
    productionverify.cpp \
    unlockfrom.cpp

HEADERS += \
        mainwindow.h \
    application_config.h \
    ambientlincomm.h \
    ambientlinscheduler.h \
    bcmmasterbutton.h \
    slavebutton.h \
    bcmmasterframe.h \
    slaveframeconfig.h \
    adialog.h \
    keyboard.h \
    productionverify.h \
    stdint-uintn.h \
    types.h \
    unlockfrom.h

FORMS += \
        mainwindow.ui \
    masterframe.ui \
    slavenodeframe.ui \
    deviceDialog.ui \
    amkeyboard.ui \
    productionverify.ui \
    unlockfrom.ui

RESOURCES += \
    ambinetdevideres.qrc
