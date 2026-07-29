#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T10:11:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = RLNG11_v00
TEMPLATE = app
CONFIG += c++11

target.path = /root/ControlBox/test
INSTALLS += target

QMAKE_LFLAGS += -Wl,-rpath,/usr/local/Qt-5.10.1/lib

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
        main.cc \
        mainwindow.cc \
    ambientlincomm.cc \
    ambientlinscheduler.cc \
    bcmmasterbutton.cc \
    slavebutton.cc \
    bcmmasterframe.cc \
    slaveframeconfig.cc \
    adialog.cc \
    keyboard.cc \
    productionverify.cc \
     unlockfrom.cc \
     linlayout.cc \
     generated/linprofile_generated.cc \
     linbusworker.cc \
    debugstore.cc \
    debugpanel.cc

HEADERS += \
        mainwindow.h \
    application_config.h \
    ambientlincomm.h \
    ambientlinscheduler.h \
    linruntime.h \
    lintransport.h \
    debugsink.h \
    debugsnapshot.h \
    bcmmasterbutton.h \
    slavebutton.h \
    bcmmasterframe.h \
    slaveframeconfig.h \
    adialog.h \
    keyboard.h \
    productionverify.h \
    unlockfrom.h \
     lin_types.h \
     linlayout.h \
     generated/linprofile_generated.h \
     linbusworker.h \
    debugstore.h \
    debugpanel.h

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
