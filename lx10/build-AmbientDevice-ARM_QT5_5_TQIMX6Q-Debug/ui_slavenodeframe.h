/********************************************************************************
** Form generated from reading UI file 'slavenodeframe.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLAVENODEFRAME_H
#define UI_SLAVENODEFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SlaveNodeFrame
{
public:
    QLabel *labelTitle;
    QFrame *StatusFrame;
    QLabel *labelRamStatusTitle;
    QLabel *labelTitleStatus;
    QLabel *labelBlueOpenTitle;
    QLabel *labelRomStatusTitle;
    QLabel *labelNvrmStatusTitle;
    QLabel *labelRedOpenTitle;
    QLabel *labelGreenOpenTitle;
    QFrame *VersionFrame;
    QLabel *labelDimRamp_8;
    QLabel *labelSuppTitle;
    QLabel *labelSWTitle;
    QLabel *labelHWTitle;
    QLabel *labelFuncTitle;
    QLabel *labelVarTitle;
    QLabel *labelRedOpen;
    QLabel *labelGreenOpen;
    QLabel *labelBlueOpen;
    QLabel *labelRamStatus;
    QLabel *labelRomStatus;
    QLabel *labelNvrmStatus;
    QLabel *labelFuncVer;
    QLabel *labelHWVer;
    QLabel *labelVariantVer;
    QLabel *labelSWVer;
    QLabel *labelSuppVer;
    QFrame *Configurationframe;
    QLabel *labelTitle_2;
    QLabel *labelSingleAddr;
    QLabel *labelGroupAddr;
    QLabel *labelPlatform;
    QLabel *labelIntensity;
    QLabel *labelPartNO;
    QLabel *labelSerialNO;
    QLabel *labelLockState;
    QSpinBox *spinBoxSA;
    QSpinBox *spinBoxGA;
    QSpinBox *spinBoxPlatform;
    QSpinBox *spinBoxIntensity;
    QLineEdit *lineEditPN;
    QLineEdit *lineEditSerial;
    QLabel *labelXCoordinate;
    QLabel *labelYCoordinate;
    QLabel *labelLuminous;
    QDoubleSpinBox *doubleSpinBoxRX;
    QDoubleSpinBox *doubleSpinBoxRY;
    QDoubleSpinBox *doubleSpinBoxRL;
    QDoubleSpinBox *doubleSpinBoxGL;
    QDoubleSpinBox *doubleSpinBoxGX;
    QDoubleSpinBox *doubleSpinBoxGY;
    QDoubleSpinBox *doubleSpinBoxBL;
    QDoubleSpinBox *doubleSpinBoxBX;
    QDoubleSpinBox *doubleSpinBoxBY;
    QLabel *labelRedAttr;
    QLabel *labelGteenAttr;
    QLabel *labelBlueAttr;
    QPushButton *pushButtonAccept;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonUnlock;
    QPushButton *pushButtonLock;
    QFrame *CalibrationFrame;
    QLabel *labelTitleCalibration;
    QPushButton *pushButtonCalibrateR;
    QPushButton *pushButtonCalibrateG;
    QPushButton *pushButtonCalibrateB;
    QLabel *labelTempStateTitle;
    QLabel *labelTempState;
    QLabel *labelBlueShortTitle;
    QLabel *labelGreenShort;
    QLabel *labelBlueShort;
    QLabel *labelRedShortTitle;
    QLabel *labelRedShort;
    QLabel *labelGreenShortTitle;
    QPushButton *pushButtonNoCalibrate;

    void setupUi(QWidget *SlaveNodeFrame)
    {
        if (SlaveNodeFrame->objectName().isEmpty())
            SlaveNodeFrame->setObjectName(QStringLiteral("SlaveNodeFrame"));
        SlaveNodeFrame->resize(1366, 768);
        labelTitle = new QLabel(SlaveNodeFrame);
        labelTitle->setObjectName(QStringLiteral("labelTitle"));
        labelTitle->setGeometry(QRect(0, 0, 1366, 71));
        QFont font;
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:32px;\n"
" color:rgb(178,34,34);\n"
"}"));
        labelTitle->setAlignment(Qt::AlignCenter);
        StatusFrame = new QFrame(SlaveNodeFrame);
        StatusFrame->setObjectName(QStringLiteral("StatusFrame"));
        StatusFrame->setGeometry(QRect(60, 80, 241, 481));
        StatusFrame->setStyleSheet(QLatin1String("QFrame{\n"
"border:2px solid rgba(255,127,0,1.0);\n"
"border-bottom-right-radius: 30px;\n"
"background-color:rgba(0, 0, 0, 0.3);\n"
"}"));
        StatusFrame->setFrameShape(QFrame::StyledPanel);
        StatusFrame->setFrameShadow(QFrame::Raised);
        labelRamStatusTitle = new QLabel(SlaveNodeFrame);
        labelRamStatusTitle->setObjectName(QStringLiteral("labelRamStatusTitle"));
        labelRamStatusTitle->setGeometry(QRect(80, 420, 101, 42));
        labelRamStatusTitle->setFont(font);
        labelRamStatusTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelRamStatusTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelTitleStatus = new QLabel(SlaveNodeFrame);
        labelTitleStatus->setObjectName(QStringLiteral("labelTitleStatus"));
        labelTitleStatus->setGeometry(QRect(60, 80, 241, 51));
        labelTitleStatus->setFont(font);
        labelTitleStatus->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelTitleStatus->setAlignment(Qt::AlignCenter);
        labelBlueOpenTitle = new QLabel(SlaveNodeFrame);
        labelBlueOpenTitle->setObjectName(QStringLiteral("labelBlueOpenTitle"));
        labelBlueOpenTitle->setGeometry(QRect(80, 220, 101, 42));
        labelBlueOpenTitle->setFont(font);
        labelBlueOpenTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelBlueOpenTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelRomStatusTitle = new QLabel(SlaveNodeFrame);
        labelRomStatusTitle->setObjectName(QStringLiteral("labelRomStatusTitle"));
        labelRomStatusTitle->setGeometry(QRect(80, 380, 101, 42));
        labelRomStatusTitle->setFont(font);
        labelRomStatusTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelRomStatusTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelNvrmStatusTitle = new QLabel(SlaveNodeFrame);
        labelNvrmStatusTitle->setObjectName(QStringLiteral("labelNvrmStatusTitle"));
        labelNvrmStatusTitle->setGeometry(QRect(80, 460, 101, 42));
        labelNvrmStatusTitle->setFont(font);
        labelNvrmStatusTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelNvrmStatusTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelRedOpenTitle = new QLabel(SlaveNodeFrame);
        labelRedOpenTitle->setObjectName(QStringLiteral("labelRedOpenTitle"));
        labelRedOpenTitle->setGeometry(QRect(80, 140, 101, 42));
        labelRedOpenTitle->setFont(font);
        labelRedOpenTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelRedOpenTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelGreenOpenTitle = new QLabel(SlaveNodeFrame);
        labelGreenOpenTitle->setObjectName(QStringLiteral("labelGreenOpenTitle"));
        labelGreenOpenTitle->setGeometry(QRect(80, 180, 101, 42));
        labelGreenOpenTitle->setFont(font);
        labelGreenOpenTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelGreenOpenTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        VersionFrame = new QFrame(SlaveNodeFrame);
        VersionFrame->setObjectName(QStringLiteral("VersionFrame"));
        VersionFrame->setGeometry(QRect(60, 580, 601, 161));
        VersionFrame->setStyleSheet(QLatin1String("QFrame{\n"
"border:2px solid rgb(244,96,108);\n"
"border-bottom-right-radius: 30px;\n"
"background-color:rgba(0, 0, 0, 0.3);\n"
"}"));
        VersionFrame->setFrameShape(QFrame::StyledPanel);
        VersionFrame->setFrameShadow(QFrame::Raised);
        labelDimRamp_8 = new QLabel(SlaveNodeFrame);
        labelDimRamp_8->setObjectName(QStringLiteral("labelDimRamp_8"));
        labelDimRamp_8->setGeometry(QRect(60, 580, 601, 42));
        labelDimRamp_8->setFont(font);
        labelDimRamp_8->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(244, 96, 108);\n"
"};"));
        labelDimRamp_8->setAlignment(Qt::AlignCenter);
        labelSuppTitle = new QLabel(SlaveNodeFrame);
        labelSuppTitle->setObjectName(QStringLiteral("labelSuppTitle"));
        labelSuppTitle->setGeometry(QRect(260, 620, 91, 42));
        labelSuppTitle->setFont(font);
        labelSuppTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(244, 96, 108);\n"
"};"));
        labelSuppTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelSWTitle = new QLabel(SlaveNodeFrame);
        labelSWTitle->setObjectName(QStringLiteral("labelSWTitle"));
        labelSWTitle->setGeometry(QRect(60, 620, 91, 42));
        labelSWTitle->setFont(font);
        labelSWTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(244, 96, 108);\n"
"};"));
        labelSWTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelHWTitle = new QLabel(SlaveNodeFrame);
        labelHWTitle->setObjectName(QStringLiteral("labelHWTitle"));
        labelHWTitle->setGeometry(QRect(60, 670, 91, 42));
        labelHWTitle->setFont(font);
        labelHWTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(244, 96, 108);\n"
"};"));
        labelHWTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelFuncTitle = new QLabel(SlaveNodeFrame);
        labelFuncTitle->setObjectName(QStringLiteral("labelFuncTitle"));
        labelFuncTitle->setGeometry(QRect(260, 670, 91, 42));
        labelFuncTitle->setFont(font);
        labelFuncTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(244, 96, 108);\n"
"};"));
        labelFuncTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelVarTitle = new QLabel(SlaveNodeFrame);
        labelVarTitle->setObjectName(QStringLiteral("labelVarTitle"));
        labelVarTitle->setGeometry(QRect(460, 620, 91, 42));
        labelVarTitle->setFont(font);
        labelVarTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(244, 96, 108);\n"
"};"));
        labelVarTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelRedOpen = new QLabel(SlaveNodeFrame);
        labelRedOpen->setObjectName(QStringLiteral("labelRedOpen"));
        labelRedOpen->setGeometry(QRect(190, 140, 81, 42));
        labelRedOpen->setFont(font);
        labelRedOpen->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelRedOpen->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelGreenOpen = new QLabel(SlaveNodeFrame);
        labelGreenOpen->setObjectName(QStringLiteral("labelGreenOpen"));
        labelGreenOpen->setGeometry(QRect(190, 180, 81, 42));
        labelGreenOpen->setFont(font);
        labelGreenOpen->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelGreenOpen->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelBlueOpen = new QLabel(SlaveNodeFrame);
        labelBlueOpen->setObjectName(QStringLiteral("labelBlueOpen"));
        labelBlueOpen->setGeometry(QRect(190, 220, 81, 42));
        labelBlueOpen->setFont(font);
        labelBlueOpen->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelBlueOpen->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelRamStatus = new QLabel(SlaveNodeFrame);
        labelRamStatus->setObjectName(QStringLiteral("labelRamStatus"));
        labelRamStatus->setGeometry(QRect(190, 420, 81, 42));
        labelRamStatus->setFont(font);
        labelRamStatus->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelRamStatus->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelRomStatus = new QLabel(SlaveNodeFrame);
        labelRomStatus->setObjectName(QStringLiteral("labelRomStatus"));
        labelRomStatus->setGeometry(QRect(190, 380, 81, 42));
        labelRomStatus->setFont(font);
        labelRomStatus->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelRomStatus->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelNvrmStatus = new QLabel(SlaveNodeFrame);
        labelNvrmStatus->setObjectName(QStringLiteral("labelNvrmStatus"));
        labelNvrmStatus->setGeometry(QRect(190, 460, 81, 42));
        labelNvrmStatus->setFont(font);
        labelNvrmStatus->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelNvrmStatus->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelFuncVer = new QLabel(SlaveNodeFrame);
        labelFuncVer->setObjectName(QStringLiteral("labelFuncVer"));
        labelFuncVer->setGeometry(QRect(360, 670, 101, 42));
        labelFuncVer->setFont(font);
        labelFuncVer->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelFuncVer->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelHWVer = new QLabel(SlaveNodeFrame);
        labelHWVer->setObjectName(QStringLiteral("labelHWVer"));
        labelHWVer->setGeometry(QRect(160, 670, 101, 42));
        labelHWVer->setFont(font);
        labelHWVer->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelHWVer->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelVariantVer = new QLabel(SlaveNodeFrame);
        labelVariantVer->setObjectName(QStringLiteral("labelVariantVer"));
        labelVariantVer->setGeometry(QRect(560, 620, 101, 42));
        labelVariantVer->setFont(font);
        labelVariantVer->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelVariantVer->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelSWVer = new QLabel(SlaveNodeFrame);
        labelSWVer->setObjectName(QStringLiteral("labelSWVer"));
        labelSWVer->setGeometry(QRect(160, 620, 101, 42));
        labelSWVer->setFont(font);
        labelSWVer->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelSWVer->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelSuppVer = new QLabel(SlaveNodeFrame);
        labelSuppVer->setObjectName(QStringLiteral("labelSuppVer"));
        labelSuppVer->setGeometry(QRect(360, 620, 101, 42));
        labelSuppVer->setFont(font);
        labelSuppVer->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelSuppVer->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        Configurationframe = new QFrame(SlaveNodeFrame);
        Configurationframe->setObjectName(QStringLiteral("Configurationframe"));
        Configurationframe->setGeometry(QRect(340, 80, 961, 481));
        Configurationframe->setStyleSheet(QLatin1String("QFrame{\n"
"border:2px solid rgba(29,165,219,1.0);\n"
"border-bottom-right-radius: 30px;\n"
"background-color:rgba(0, 0, 0, 0.3);\n"
"}"));
        Configurationframe->setFrameShape(QFrame::StyledPanel);
        Configurationframe->setFrameShadow(QFrame::Raised);
        labelTitle_2 = new QLabel(SlaveNodeFrame);
        labelTitle_2->setObjectName(QStringLiteral("labelTitle_2"));
        labelTitle_2->setGeometry(QRect(340, 80, 961, 41));
        labelTitle_2->setFont(font);
        labelTitle_2->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
" color:rgba(29,165,219,1.0);\n"
"}"));
        labelTitle_2->setAlignment(Qt::AlignCenter);
        labelSingleAddr = new QLabel(SlaveNodeFrame);
        labelSingleAddr->setObjectName(QStringLiteral("labelSingleAddr"));
        labelSingleAddr->setGeometry(QRect(370, 140, 121, 42));
        labelSingleAddr->setFont(font);
        labelSingleAddr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelSingleAddr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelGroupAddr = new QLabel(SlaveNodeFrame);
        labelGroupAddr->setObjectName(QStringLiteral("labelGroupAddr"));
        labelGroupAddr->setGeometry(QRect(620, 140, 111, 42));
        labelGroupAddr->setFont(font);
        labelGroupAddr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelGroupAddr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelPlatform = new QLabel(SlaveNodeFrame);
        labelPlatform->setObjectName(QStringLiteral("labelPlatform"));
        labelPlatform->setGeometry(QRect(860, 140, 71, 42));
        labelPlatform->setFont(font);
        labelPlatform->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelPlatform->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelIntensity = new QLabel(SlaveNodeFrame);
        labelIntensity->setObjectName(QStringLiteral("labelIntensity"));
        labelIntensity->setGeometry(QRect(1050, 140, 81, 42));
        labelIntensity->setFont(font);
        labelIntensity->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelIntensity->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelPartNO = new QLabel(SlaveNodeFrame);
        labelPartNO->setObjectName(QStringLiteral("labelPartNO"));
        labelPartNO->setGeometry(QRect(960, 310, 71, 42));
        labelPartNO->setFont(font);
        labelPartNO->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelPartNO->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelSerialNO = new QLabel(SlaveNodeFrame);
        labelSerialNO->setObjectName(QStringLiteral("labelSerialNO"));
        labelSerialNO->setGeometry(QRect(950, 250, 81, 42));
        labelSerialNO->setFont(font);
        labelSerialNO->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelSerialNO->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelLockState = new QLabel(SlaveNodeFrame);
        labelLockState->setObjectName(QStringLiteral("labelLockState"));
        labelLockState->setGeometry(QRect(930, 370, 101, 42));
        labelLockState->setFont(font);
        labelLockState->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelLockState->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxSA = new QSpinBox(SlaveNodeFrame);
        spinBoxSA->setObjectName(QStringLiteral("spinBoxSA"));
        spinBoxSA->setGeometry(QRect(500, 140, 81, 41));
        spinBoxSA->setStyleSheet(QLatin1String("QSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        spinBoxSA->setAlignment(Qt::AlignCenter);
        spinBoxSA->setMaximum(32767);
        spinBoxSA->setValue(0);
        spinBoxSA->setDisplayIntegerBase(10);
        spinBoxGA = new QSpinBox(SlaveNodeFrame);
        spinBoxGA->setObjectName(QStringLiteral("spinBoxGA"));
        spinBoxGA->setGeometry(QRect(740, 140, 81, 41));
        spinBoxGA->setStyleSheet(QLatin1String("QSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        spinBoxGA->setAlignment(Qt::AlignCenter);
        spinBoxGA->setMaximum(65536);
        spinBoxGA->setValue(0);
        spinBoxPlatform = new QSpinBox(SlaveNodeFrame);
        spinBoxPlatform->setObjectName(QStringLiteral("spinBoxPlatform"));
        spinBoxPlatform->setGeometry(QRect(940, 140, 81, 41));
        spinBoxPlatform->setStyleSheet(QLatin1String("QSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        spinBoxPlatform->setAlignment(Qt::AlignCenter);
        spinBoxPlatform->setMaximum(15);
        spinBoxPlatform->setValue(0);
        spinBoxIntensity = new QSpinBox(SlaveNodeFrame);
        spinBoxIntensity->setObjectName(QStringLiteral("spinBoxIntensity"));
        spinBoxIntensity->setGeometry(QRect(1140, 140, 81, 41));
        spinBoxIntensity->setStyleSheet(QLatin1String("QSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        spinBoxIntensity->setAlignment(Qt::AlignCenter);
        spinBoxIntensity->setMaximum(99999);
        spinBoxIntensity->setValue(0);
        lineEditPN = new QLineEdit(SlaveNodeFrame);
        lineEditPN->setObjectName(QStringLiteral("lineEditPN"));
        lineEditPN->setGeometry(QRect(1040, 310, 221, 41));
        lineEditPN->setStyleSheet(QLatin1String("QLineEdit{\n"
"font-size:22px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(0,0,0,0);\n"
"border:1px solid rgb(29,165,219);\n"
"};"));
        lineEditPN->setMaxLength(11);
        lineEditPN->setAlignment(Qt::AlignCenter);
        lineEditSerial = new QLineEdit(SlaveNodeFrame);
        lineEditSerial->setObjectName(QStringLiteral("lineEditSerial"));
        lineEditSerial->setGeometry(QRect(1040, 250, 221, 41));
        lineEditSerial->setStyleSheet(QLatin1String("QLineEdit{\n"
"font-size:22px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(0,0,0,0);\n"
"border:1px solid rgb(29,165,219);\n"
"};"));
        lineEditSerial->setMaxLength(21);
        lineEditSerial->setAlignment(Qt::AlignCenter);
        labelXCoordinate = new QLabel(SlaveNodeFrame);
        labelXCoordinate->setObjectName(QStringLiteral("labelXCoordinate"));
        labelXCoordinate->setGeometry(QRect(500, 200, 121, 42));
        labelXCoordinate->setFont(font);
        labelXCoordinate->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelXCoordinate->setAlignment(Qt::AlignCenter);
        labelYCoordinate = new QLabel(SlaveNodeFrame);
        labelYCoordinate->setObjectName(QStringLiteral("labelYCoordinate"));
        labelYCoordinate->setGeometry(QRect(640, 200, 121, 42));
        labelYCoordinate->setFont(font);
        labelYCoordinate->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelYCoordinate->setAlignment(Qt::AlignCenter);
        labelLuminous = new QLabel(SlaveNodeFrame);
        labelLuminous->setObjectName(QStringLiteral("labelLuminous"));
        labelLuminous->setGeometry(QRect(790, 200, 121, 42));
        labelLuminous->setFont(font);
        labelLuminous->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelLuminous->setAlignment(Qt::AlignCenter);
        doubleSpinBoxRX = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxRX->setObjectName(QStringLiteral("doubleSpinBoxRX"));
        doubleSpinBoxRX->setGeometry(QRect(510, 250, 101, 41));
        doubleSpinBoxRX->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxRX->setAlignment(Qt::AlignCenter);
        doubleSpinBoxRX->setDecimals(4);
        doubleSpinBoxRX->setMaximum(1);
        doubleSpinBoxRX->setSingleStep(1e-05);
        doubleSpinBoxRY = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxRY->setObjectName(QStringLiteral("doubleSpinBoxRY"));
        doubleSpinBoxRY->setGeometry(QRect(650, 250, 101, 41));
        doubleSpinBoxRY->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxRY->setAlignment(Qt::AlignCenter);
        doubleSpinBoxRY->setDecimals(4);
        doubleSpinBoxRY->setMaximum(1);
        doubleSpinBoxRY->setSingleStep(1e-05);
        doubleSpinBoxRL = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxRL->setObjectName(QStringLiteral("doubleSpinBoxRL"));
        doubleSpinBoxRL->setGeometry(QRect(800, 250, 101, 41));
        doubleSpinBoxRL->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxRL->setAlignment(Qt::AlignCenter);
        doubleSpinBoxRL->setDecimals(4);
        doubleSpinBoxRL->setMaximum(10);
        doubleSpinBoxRL->setSingleStep(1e-05);
        doubleSpinBoxGL = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxGL->setObjectName(QStringLiteral("doubleSpinBoxGL"));
        doubleSpinBoxGL->setGeometry(QRect(800, 310, 101, 41));
        doubleSpinBoxGL->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxGL->setAlignment(Qt::AlignCenter);
        doubleSpinBoxGL->setDecimals(4);
        doubleSpinBoxGL->setMaximum(10);
        doubleSpinBoxGL->setSingleStep(1e-05);
        doubleSpinBoxGX = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxGX->setObjectName(QStringLiteral("doubleSpinBoxGX"));
        doubleSpinBoxGX->setGeometry(QRect(510, 310, 101, 41));
        doubleSpinBoxGX->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxGX->setAlignment(Qt::AlignCenter);
        doubleSpinBoxGX->setDecimals(4);
        doubleSpinBoxGX->setMaximum(1);
        doubleSpinBoxGX->setSingleStep(1e-05);
        doubleSpinBoxGY = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxGY->setObjectName(QStringLiteral("doubleSpinBoxGY"));
        doubleSpinBoxGY->setGeometry(QRect(650, 310, 101, 41));
        doubleSpinBoxGY->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxGY->setAlignment(Qt::AlignCenter);
        doubleSpinBoxGY->setDecimals(4);
        doubleSpinBoxGY->setMaximum(1);
        doubleSpinBoxGY->setSingleStep(1e-05);
        doubleSpinBoxBL = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxBL->setObjectName(QStringLiteral("doubleSpinBoxBL"));
        doubleSpinBoxBL->setGeometry(QRect(800, 370, 101, 41));
        doubleSpinBoxBL->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxBL->setAlignment(Qt::AlignCenter);
        doubleSpinBoxBL->setDecimals(4);
        doubleSpinBoxBL->setMaximum(10);
        doubleSpinBoxBL->setSingleStep(1e-05);
        doubleSpinBoxBX = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxBX->setObjectName(QStringLiteral("doubleSpinBoxBX"));
        doubleSpinBoxBX->setGeometry(QRect(510, 370, 101, 41));
        doubleSpinBoxBX->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxBX->setAlignment(Qt::AlignCenter);
        doubleSpinBoxBX->setDecimals(4);
        doubleSpinBoxBX->setMaximum(1);
        doubleSpinBoxBX->setSingleStep(1e-05);
        doubleSpinBoxBY = new QDoubleSpinBox(SlaveNodeFrame);
        doubleSpinBoxBY->setObjectName(QStringLiteral("doubleSpinBoxBY"));
        doubleSpinBoxBY->setGeometry(QRect(650, 370, 101, 41));
        doubleSpinBoxBY->setStyleSheet(QLatin1String("QDoubleSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
"font-size:22px;\n"
"background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}\n"
"QDoubleSpinBox::down-button\n"
"{\n"
"  width:0px;\n"
"  height:0px;\n"
"}"));
        doubleSpinBoxBY->setAlignment(Qt::AlignCenter);
        doubleSpinBoxBY->setDecimals(4);
        doubleSpinBoxBY->setMaximum(1);
        doubleSpinBoxBY->setSingleStep(1e-05);
        labelRedAttr = new QLabel(SlaveNodeFrame);
        labelRedAttr->setObjectName(QStringLiteral("labelRedAttr"));
        labelRedAttr->setGeometry(QRect(370, 250, 121, 42));
        labelRedAttr->setFont(font);
        labelRedAttr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelRedAttr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelGteenAttr = new QLabel(SlaveNodeFrame);
        labelGteenAttr->setObjectName(QStringLiteral("labelGteenAttr"));
        labelGteenAttr->setGeometry(QRect(370, 310, 121, 42));
        labelGteenAttr->setFont(font);
        labelGteenAttr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelGteenAttr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelBlueAttr = new QLabel(SlaveNodeFrame);
        labelBlueAttr->setObjectName(QStringLiteral("labelBlueAttr"));
        labelBlueAttr->setGeometry(QRect(370, 370, 121, 42));
        labelBlueAttr->setFont(font);
        labelBlueAttr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelBlueAttr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonAccept = new QPushButton(SlaveNodeFrame);
        pushButtonAccept->setObjectName(QStringLiteral("pushButtonAccept"));
        pushButtonAccept->setGeometry(QRect(960, 470, 131, 71));
        pushButtonAccept->setFont(font);
        pushButtonAccept->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonCancel = new QPushButton(SlaveNodeFrame);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(1130, 470, 131, 71));
        pushButtonCancel->setFont(font);
        pushButtonCancel->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonUnlock = new QPushButton(SlaveNodeFrame);
        pushButtonUnlock->setObjectName(QStringLiteral("pushButtonUnlock"));
        pushButtonUnlock->setGeometry(QRect(1170, 370, 90, 42));
        pushButtonUnlock->setFont(font);
        pushButtonUnlock->setStyleSheet(QLatin1String("QPushButton{\n"
"border:0px;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgba(255,251,240,0.5);\n"
"font-size:22px;\n"
"background:rgba(29, 165, 219, 0.1);\n"
"}"));
        pushButtonLock = new QPushButton(SlaveNodeFrame);
        pushButtonLock->setObjectName(QStringLiteral("pushButtonLock"));
        pushButtonLock->setGeometry(QRect(1050, 370, 90, 42));
        pushButtonLock->setFont(font);
        pushButtonLock->setStyleSheet(QLatin1String("QPushButton{\n"
"border:0px;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgba(255,251,240,0.5);\n"
"font-size:22px;\n"
"background:rgba(29, 165, 219, 0.1);\n"
"}"));
        CalibrationFrame = new QFrame(SlaveNodeFrame);
        CalibrationFrame->setObjectName(QStringLiteral("CalibrationFrame"));
        CalibrationFrame->setGeometry(QRect(700, 580, 601, 161));
        CalibrationFrame->setStyleSheet(QLatin1String("QFrame{\n"
"border:2px solid rgba(210,180,240,1.0);\n"
"border-bottom-right-radius: 30px;\n"
"background-color:rgba(0, 0, 0, 0.3);\n"
"}"));
        CalibrationFrame->setFrameShape(QFrame::StyledPanel);
        CalibrationFrame->setFrameShadow(QFrame::Raised);
        labelTitleCalibration = new QLabel(SlaveNodeFrame);
        labelTitleCalibration->setObjectName(QStringLiteral("labelTitleCalibration"));
        labelTitleCalibration->setGeometry(QRect(700, 580, 601, 51));
        labelTitleCalibration->setFont(font);
        labelTitleCalibration->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
" color:rgba(210,180,240,1.0);\n"
"}"));
        labelTitleCalibration->setAlignment(Qt::AlignCenter);
        pushButtonCalibrateR = new QPushButton(SlaveNodeFrame);
        pushButtonCalibrateR->setObjectName(QStringLiteral("pushButtonCalibrateR"));
        pushButtonCalibrateR->setGeometry(QRect(870, 660, 121, 61));
        pushButtonCalibrateR->setFont(font);
        pushButtonCalibrateR->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid rgb(254,67,101);\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonCalibrateG = new QPushButton(SlaveNodeFrame);
        pushButtonCalibrateG->setObjectName(QStringLiteral("pushButtonCalibrateG"));
        pushButtonCalibrateG->setGeometry(QRect(1010, 660, 121, 61));
        pushButtonCalibrateG->setFont(font);
        pushButtonCalibrateG->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid rgb(104,194,53);\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonCalibrateB = new QPushButton(SlaveNodeFrame);
        pushButtonCalibrateB->setObjectName(QStringLiteral("pushButtonCalibrateB"));
        pushButtonCalibrateB->setGeometry(QRect(1150, 660, 121, 61));
        pushButtonCalibrateB->setFont(font);
        pushButtonCalibrateB->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        labelTempStateTitle = new QLabel(SlaveNodeFrame);
        labelTempStateTitle->setObjectName(QStringLiteral("labelTempStateTitle"));
        labelTempStateTitle->setGeometry(QRect(80, 500, 101, 42));
        labelTempStateTitle->setFont(font);
        labelTempStateTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelTempStateTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelTempState = new QLabel(SlaveNodeFrame);
        labelTempState->setObjectName(QStringLiteral("labelTempState"));
        labelTempState->setGeometry(QRect(190, 500, 81, 42));
        labelTempState->setFont(font);
        labelTempState->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelTempState->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelBlueShortTitle = new QLabel(SlaveNodeFrame);
        labelBlueShortTitle->setObjectName(QStringLiteral("labelBlueShortTitle"));
        labelBlueShortTitle->setGeometry(QRect(80, 340, 101, 42));
        labelBlueShortTitle->setFont(font);
        labelBlueShortTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelBlueShortTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelGreenShort = new QLabel(SlaveNodeFrame);
        labelGreenShort->setObjectName(QStringLiteral("labelGreenShort"));
        labelGreenShort->setGeometry(QRect(190, 300, 81, 42));
        labelGreenShort->setFont(font);
        labelGreenShort->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelGreenShort->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelBlueShort = new QLabel(SlaveNodeFrame);
        labelBlueShort->setObjectName(QStringLiteral("labelBlueShort"));
        labelBlueShort->setGeometry(QRect(190, 340, 81, 42));
        labelBlueShort->setFont(font);
        labelBlueShort->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelBlueShort->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelRedShortTitle = new QLabel(SlaveNodeFrame);
        labelRedShortTitle->setObjectName(QStringLiteral("labelRedShortTitle"));
        labelRedShortTitle->setGeometry(QRect(80, 260, 101, 42));
        labelRedShortTitle->setFont(font);
        labelRedShortTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelRedShortTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelRedShort = new QLabel(SlaveNodeFrame);
        labelRedShort->setObjectName(QStringLiteral("labelRedShort"));
        labelRedShort->setGeometry(QRect(190, 260, 81, 42));
        labelRedShort->setFont(font);
        labelRedShort->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelRedShort->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelGreenShortTitle = new QLabel(SlaveNodeFrame);
        labelGreenShortTitle->setObjectName(QStringLiteral("labelGreenShortTitle"));
        labelGreenShortTitle->setGeometry(QRect(80, 300, 101, 42));
        labelGreenShortTitle->setFont(font);
        labelGreenShortTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelGreenShortTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonNoCalibrate = new QPushButton(SlaveNodeFrame);
        pushButtonNoCalibrate->setObjectName(QStringLiteral("pushButtonNoCalibrate"));
        pushButtonNoCalibrate->setGeometry(QRect(730, 660, 121, 61));
        pushButtonNoCalibrate->setFont(font);
        pushButtonNoCalibrate->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid rgb()255,251,240;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));

        retranslateUi(SlaveNodeFrame);

        QMetaObject::connectSlotsByName(SlaveNodeFrame);
    } // setupUi

    void retranslateUi(QWidget *SlaveNodeFrame)
    {
        SlaveNodeFrame->setWindowTitle(QApplication::translate("SlaveNodeFrame", "Form", 0));
        labelTitle->setText(QApplication::translate("SlaveNodeFrame", "Slave Node Status And  Configuration", 0));
        labelRamStatusTitle->setText(QApplication::translate("SlaveNodeFrame", "Ram Status:", 0));
        labelTitleStatus->setText(QApplication::translate("SlaveNodeFrame", "Status Report", 0));
        labelBlueOpenTitle->setText(QApplication::translate("SlaveNodeFrame", "Blue Open:", 0));
        labelRomStatusTitle->setText(QApplication::translate("SlaveNodeFrame", "Rom Status:", 0));
        labelNvrmStatusTitle->setText(QApplication::translate("SlaveNodeFrame", "Nvrm Status:", 0));
        labelRedOpenTitle->setText(QApplication::translate("SlaveNodeFrame", "Red Open:", 0));
        labelGreenOpenTitle->setText(QApplication::translate("SlaveNodeFrame", "Green Open:", 0));
        labelDimRamp_8->setText(QApplication::translate("SlaveNodeFrame", "Version Report", 0));
        labelSuppTitle->setText(QApplication::translate("SlaveNodeFrame", "Supplier ID:", 0));
        labelSWTitle->setText(QApplication::translate("SlaveNodeFrame", "SW Ver:", 0));
        labelHWTitle->setText(QApplication::translate("SlaveNodeFrame", "HW Ver:", 0));
        labelFuncTitle->setText(QApplication::translate("SlaveNodeFrame", "Function ID:", 0));
        labelVarTitle->setText(QApplication::translate("SlaveNodeFrame", "Varaint ID:", 0));
        labelRedOpen->setText(QString());
        labelGreenOpen->setText(QString());
        labelBlueOpen->setText(QString());
        labelRamStatus->setText(QString());
        labelRomStatus->setText(QString());
        labelNvrmStatus->setText(QString());
        labelFuncVer->setText(QString());
        labelHWVer->setText(QString());
        labelVariantVer->setText(QString());
        labelSWVer->setText(QString());
        labelSuppVer->setText(QString());
        labelTitle_2->setText(QApplication::translate("SlaveNodeFrame", "Slave Node Configuration", 0));
        labelSingleAddr->setText(QApplication::translate("SlaveNodeFrame", "Frame ID:", 0));
        labelGroupAddr->setText(QApplication::translate("SlaveNodeFrame", "CtrlList:", 0));
        labelPlatform->setText(QApplication::translate("SlaveNodeFrame", "Platform:", 0));
        labelIntensity->setText(QApplication::translate("SlaveNodeFrame", "Intensity:", 0));
        labelPartNO->setText(QApplication::translate("SlaveNodeFrame", "Part NO.:", 0));
        labelSerialNO->setText(QApplication::translate("SlaveNodeFrame", "Serial NO.:", 0));
        labelLockState->setText(QApplication::translate("SlaveNodeFrame", "Lock State:", 0));
        lineEditPN->setText(QString());
        lineEditSerial->setText(QString());
        labelXCoordinate->setText(QApplication::translate("SlaveNodeFrame", "X Coordinate", 0));
        labelYCoordinate->setText(QApplication::translate("SlaveNodeFrame", "Y Coordinate", 0));
        labelLuminous->setText(QApplication::translate("SlaveNodeFrame", "Luminous", 0));
        labelRedAttr->setText(QApplication::translate("SlaveNodeFrame", "Red Color Attr:", 0));
        labelGteenAttr->setText(QApplication::translate("SlaveNodeFrame", "Green Color Attr:", 0));
        labelBlueAttr->setText(QApplication::translate("SlaveNodeFrame", "Blue Color Attr:", 0));
        pushButtonAccept->setText(QApplication::translate("SlaveNodeFrame", "Apply", 0));
        pushButtonCancel->setText(QApplication::translate("SlaveNodeFrame", "Exit", 0));
        pushButtonUnlock->setText(QApplication::translate("SlaveNodeFrame", "Unlocked", 0));
        pushButtonLock->setText(QApplication::translate("SlaveNodeFrame", "Locked", 0));
        labelTitleCalibration->setText(QApplication::translate("SlaveNodeFrame", "Calibration Function", 0));
        pushButtonCalibrateR->setText(QApplication::translate("SlaveNodeFrame", "RED", 0));
        pushButtonCalibrateG->setText(QApplication::translate("SlaveNodeFrame", "GREEN", 0));
        pushButtonCalibrateB->setText(QApplication::translate("SlaveNodeFrame", "BLUE", 0));
        labelTempStateTitle->setText(QApplication::translate("SlaveNodeFrame", "Temp State:", 0));
        labelTempState->setText(QString());
        labelBlueShortTitle->setText(QApplication::translate("SlaveNodeFrame", "Blue Short:", 0));
        labelGreenShort->setText(QString());
        labelBlueShort->setText(QString());
        labelRedShortTitle->setText(QApplication::translate("SlaveNodeFrame", "Red Short:", 0));
        labelRedShort->setText(QString());
        labelGreenShortTitle->setText(QApplication::translate("SlaveNodeFrame", "Green Short:", 0));
        pushButtonNoCalibrate->setText(QApplication::translate("SlaveNodeFrame", "Normal", 0));
    } // retranslateUi

};

namespace Ui {
    class SlaveNodeFrame: public Ui_SlaveNodeFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAVENODEFRAME_H
