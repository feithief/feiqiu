/********************************************************************************
** Form generated from reading UI file 'slavenodeframe.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
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
    QLabel *labelTitleStatus;
    QLabel *labelBlueOpenTitle;
    QLabel *labelRedOpenTitle;
    QLabel *labelGreenOpenTitle;
    QFrame *VersionFrame;
    QLabel *labelDimRamp_8;
    QLabel *labelSuppTitle;
    QLabel *labelSWTitle;
    QLabel *labelHWTitle;
    QLabel *labelFuncTitle;
    QLabel *labelVarTitle;
    QLabel *ROutput_Err;
    QLabel *GOutput_Err;
    QLabel *BOutput_Err;
    QLabel *labelFuncVer;
    QLabel *labelHWVer;
    QLabel *labelVariantVer;
    QLabel *labelSWVer;
    QLabel *labelSuppVer;
    QFrame *Configurationframe;
    QPushButton *SleepButton;
    QPushButton *AwakeButton;
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
    QLabel *labelBlueShortTitle;
    QLabel *Voltage_Err;
    QLabel *Lin_Err;
    QLabel *labelRedShortTitle;
    QLabel *Temp_Err;
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
        ROutput_Err = new QLabel(SlaveNodeFrame);
        ROutput_Err->setObjectName(QStringLiteral("ROutput_Err"));
        ROutput_Err->setGeometry(QRect(190, 140, 81, 42));
        ROutput_Err->setFont(font);
        ROutput_Err->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        ROutput_Err->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        GOutput_Err = new QLabel(SlaveNodeFrame);
        GOutput_Err->setObjectName(QStringLiteral("GOutput_Err"));
        GOutput_Err->setGeometry(QRect(190, 180, 81, 42));
        GOutput_Err->setFont(font);
        GOutput_Err->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        GOutput_Err->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        BOutput_Err = new QLabel(SlaveNodeFrame);
        BOutput_Err->setObjectName(QStringLiteral("BOutput_Err"));
        BOutput_Err->setGeometry(QRect(190, 220, 81, 42));
        BOutput_Err->setFont(font);
        BOutput_Err->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        BOutput_Err->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
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
        SleepButton = new QPushButton(Configurationframe);
        SleepButton->setObjectName(QStringLiteral("SleepButton"));
        SleepButton->setGeometry(QRect(160, 360, 131, 71));
        SleepButton->setFont(font);
        SleepButton->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        AwakeButton = new QPushButton(Configurationframe);
        AwakeButton->setObjectName(QStringLiteral("AwakeButton"));
        AwakeButton->setGeometry(QRect(320, 360, 131, 71));
        AwakeButton->setFont(font);
        AwakeButton->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
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
        spinBoxSA->setMaximum(127);
        spinBoxSA->setValue(0);
        spinBoxSA->setDisplayIntegerBase(10);
        spinBoxGA = new QSpinBox(SlaveNodeFrame);
        spinBoxGA->setObjectName(QStringLiteral("spinBoxGA"));
        spinBoxGA->setReadOnly(true);
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
        spinBoxGA->setMaximum(32767);
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
        spinBoxPlatform->setMaximum(2);
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
        spinBoxIntensity->setMaximum(65535);
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
        lineEditSerial->setMaxLength(23);
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
        doubleSpinBoxRX->setSingleStep(1e-5);
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
        doubleSpinBoxRY->setSingleStep(1e-5);
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
        doubleSpinBoxRL->setSingleStep(1e-5);
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
        doubleSpinBoxGL->setSingleStep(1e-5);
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
        doubleSpinBoxGX->setSingleStep(1e-5);
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
        doubleSpinBoxGY->setSingleStep(1e-5);
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
        doubleSpinBoxBL->setSingleStep(1e-5);
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
        doubleSpinBoxBX->setSingleStep(1e-5);
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
        doubleSpinBoxBY->setSingleStep(1e-5);
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
        labelBlueShortTitle = new QLabel(SlaveNodeFrame);
        labelBlueShortTitle->setObjectName(QStringLiteral("labelBlueShortTitle"));
        labelBlueShortTitle->setGeometry(QRect(80, 340, 101, 42));
        labelBlueShortTitle->setFont(font);
        labelBlueShortTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelBlueShortTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        Voltage_Err = new QLabel(SlaveNodeFrame);
        Voltage_Err->setObjectName(QStringLiteral("Voltage_Err"));
        Voltage_Err->setGeometry(QRect(190, 300, 81, 42));
        Voltage_Err->setFont(font);
        Voltage_Err->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        Voltage_Err->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        Lin_Err = new QLabel(SlaveNodeFrame);
        Lin_Err->setObjectName(QStringLiteral("Lin_Err"));
        Lin_Err->setGeometry(QRect(190, 340, 81, 42));
        Lin_Err->setFont(font);
        Lin_Err->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        Lin_Err->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelRedShortTitle = new QLabel(SlaveNodeFrame);
        labelRedShortTitle->setObjectName(QStringLiteral("labelRedShortTitle"));
        labelRedShortTitle->setGeometry(QRect(80, 260, 101, 42));
        labelRedShortTitle->setFont(font);
        labelRedShortTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 127, 0);\n"
"};"));
        labelRedShortTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        Temp_Err = new QLabel(SlaveNodeFrame);
        Temp_Err->setObjectName(QStringLiteral("Temp_Err"));
        Temp_Err->setGeometry(QRect(190, 260, 81, 42));
        Temp_Err->setFont(font);
        Temp_Err->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        Temp_Err->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
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
        SlaveNodeFrame->setWindowTitle(QApplication::translate("SlaveNodeFrame", "Form", nullptr));
        labelTitle->setText(QApplication::translate("SlaveNodeFrame", "Slave Node Status And  Configuration", nullptr));
        labelTitleStatus->setText(QApplication::translate("SlaveNodeFrame", "Status Report", nullptr));
        labelBlueOpenTitle->setText(QApplication::translate("SlaveNodeFrame", "BOutput_Err", nullptr));
        labelRedOpenTitle->setText(QApplication::translate("SlaveNodeFrame", "ROutput_Err", nullptr));
        labelGreenOpenTitle->setText(QApplication::translate("SlaveNodeFrame", "GOutput_Err", nullptr));
        labelDimRamp_8->setText(QApplication::translate("SlaveNodeFrame", "Version Report", nullptr));
        labelSuppTitle->setText(QApplication::translate("SlaveNodeFrame", "Supplier ID:", nullptr));
        labelSWTitle->setText(QApplication::translate("SlaveNodeFrame", "SW Ver:", nullptr));
        labelHWTitle->setText(QApplication::translate("SlaveNodeFrame", "HW Ver:", nullptr));
        labelFuncTitle->setText(QApplication::translate("SlaveNodeFrame", "Function ID:", nullptr));
        labelVarTitle->setText(QApplication::translate("SlaveNodeFrame", "Varaint ID:", nullptr));
        ROutput_Err->setText(QString());
        GOutput_Err->setText(QString());
        BOutput_Err->setText(QString());
        labelFuncVer->setText(QString());
        labelHWVer->setText(QString());
        labelVariantVer->setText(QString());
        labelSWVer->setText(QString());
        labelSuppVer->setText(QString());
        SleepButton->setText(QApplication::translate("SlaveNodeFrame", "Sleep", nullptr));
        AwakeButton->setText(QApplication::translate("SlaveNodeFrame", "Awake", nullptr));
        labelTitle_2->setText(QApplication::translate("SlaveNodeFrame", "Slave Node Configuration", nullptr));
        labelSingleAddr->setText(QApplication::translate("SlaveNodeFrame", "Single Address:", nullptr));
        labelGroupAddr->setText(QApplication::translate("SlaveNodeFrame", "Group Address:", nullptr));
        labelPlatform->setText(QApplication::translate("SlaveNodeFrame", "Platform:", nullptr));
        labelIntensity->setText(QApplication::translate("SlaveNodeFrame", "Factor:", nullptr));
        labelPartNO->setText(QApplication::translate("SlaveNodeFrame", "Part NO.:", nullptr));
        labelSerialNO->setText(QApplication::translate("SlaveNodeFrame", "Serial NO.:", nullptr));
        labelLockState->setText(QApplication::translate("SlaveNodeFrame", "Lock State:", nullptr));
#ifndef QT_NO_TOOLTIP
        spinBoxGA->setToolTip(QApplication::translate("SlaveNodeFrame", "Automatically derived from the SA/control-mask table in the active profile.", nullptr));
#endif // QT_NO_TOOLTIP
        lineEditPN->setText(QString());
        lineEditSerial->setText(QString());
        labelXCoordinate->setText(QApplication::translate("SlaveNodeFrame", "X Coordinate", nullptr));
        labelYCoordinate->setText(QApplication::translate("SlaveNodeFrame", "Y Coordinate", nullptr));
        labelLuminous->setText(QApplication::translate("SlaveNodeFrame", "Luminous", nullptr));
        labelRedAttr->setText(QApplication::translate("SlaveNodeFrame", "Red Color Attr:", nullptr));
        labelGteenAttr->setText(QApplication::translate("SlaveNodeFrame", "Green Color Attr:", nullptr));
        labelBlueAttr->setText(QApplication::translate("SlaveNodeFrame", "Blue Color Attr:", nullptr));
        pushButtonAccept->setText(QApplication::translate("SlaveNodeFrame", "Apply", nullptr));
        pushButtonCancel->setText(QApplication::translate("SlaveNodeFrame", "Exit", nullptr));
        pushButtonUnlock->setText(QApplication::translate("SlaveNodeFrame", "Unlocked", nullptr));
        pushButtonLock->setText(QApplication::translate("SlaveNodeFrame", "Locked", nullptr));
        labelTitleCalibration->setText(QApplication::translate("SlaveNodeFrame", "Calibration Function", nullptr));
        pushButtonCalibrateR->setText(QApplication::translate("SlaveNodeFrame", "RED", nullptr));
        pushButtonCalibrateG->setText(QApplication::translate("SlaveNodeFrame", "GREEN", nullptr));
        pushButtonCalibrateB->setText(QApplication::translate("SlaveNodeFrame", "BLUE", nullptr));
        labelBlueShortTitle->setText(QApplication::translate("SlaveNodeFrame", "Lin_Err", nullptr));
        Voltage_Err->setText(QString());
        Lin_Err->setText(QString());
        labelRedShortTitle->setText(QApplication::translate("SlaveNodeFrame", "Temp_Err", nullptr));
        Temp_Err->setText(QString());
        labelGreenShortTitle->setText(QApplication::translate("SlaveNodeFrame", "Voltage_Err", nullptr));
        pushButtonNoCalibrate->setText(QApplication::translate("SlaveNodeFrame", "Normal", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SlaveNodeFrame: public Ui_SlaveNodeFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAVENODEFRAME_H
