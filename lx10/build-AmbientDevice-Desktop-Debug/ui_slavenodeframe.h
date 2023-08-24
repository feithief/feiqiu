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
#include <QtWidgets/QHBoxLayout>
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
    QFrame *VersionFrame;
    QLabel *labelDimRamp_8;
    QLabel *labelSuppTitle;
    QLabel *labelSWTitle;
    QLabel *labelHWTitle;
    QLabel *labelFuncTitle;
    QLabel *labelVarTitle;
    QLabel *labelFuncVer;
    QLabel *labelVariantVer;
    QLabel *labelSWVer;
    QLabel *labelSuppVer;
    QFrame *Configurationframe;
    QFrame *StatusFrame;
    QLabel *labelTitleStatus;
    QLabel *labelSWVerNumTitle;
    QLabel *labelSWVerNum;
    QLabel *labelErrRespTitle;
    QLabel *labelErrResp;
    QLabel *labelHWVerNumTitle;
    QLabel *labelHWVerNum;
    QLabel *labelLuminous;
    QLabel *labelBlueAttr;
    QDoubleSpinBox *doubleSpinBoxRX;
    QDoubleSpinBox *doubleSpinBoxBX;
    QLabel *labelRedAttr;
    QLabel *labelYCoordinate;
    QDoubleSpinBox *doubleSpinBoxBY;
    QLabel *labelGteenAttr;
    QDoubleSpinBox *doubleSpinBoxGL;
    QDoubleSpinBox *doubleSpinBoxRL;
    QDoubleSpinBox *doubleSpinBoxGX;
    QDoubleSpinBox *doubleSpinBoxGY;
    QLabel *labelXCoordinate;
    QDoubleSpinBox *doubleSpinBoxBL;
    QDoubleSpinBox *doubleSpinBoxRY;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonAccept;
    QPushButton *pushButtonUnlock;
    QLabel *labelSerialNO;
    QLineEdit *lineEditPN;
    QLabel *labelLockState;
    QLabel *labelPartNO;
    QLineEdit *lineEditSerial;
    QPushButton *pushButtonLock;
    QLabel *labelGroupAddr;
    QSpinBox *spinBoxGA;
    QSpinBox *spinBoxSA;
    QLabel *labelSingleAddr;
    QSpinBox *spinBoxPlatform;
    QLabel *labelPlatform;
    QSpinBox *spinBoxIntensity;
    QLabel *labelIntensity;
    QLabel *labelTitle_2;
    QFrame *CalibrationFrame;
    QLabel *labelTitleCalibration;
    QPushButton *pushButtonCalibrateR;
    QPushButton *pushButtonCalibrateG;
    QPushButton *pushButtonCalibrateB;
    QPushButton *pushButtonNoCalibrate;
    QLineEdit *lineEditHWVer;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;

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
        labelFuncVer = new QLabel(SlaveNodeFrame);
        labelFuncVer->setObjectName(QStringLiteral("labelFuncVer"));
        labelFuncVer->setGeometry(QRect(360, 670, 101, 42));
        labelFuncVer->setFont(font);
        labelFuncVer->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(255, 251, 240);\n"
"};"));
        labelFuncVer->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
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
        Configurationframe->setGeometry(QRect(60, 80, 1241, 481));
        Configurationframe->setStyleSheet(QLatin1String("QFrame{\n"
"border:2px solid rgba(29,165,219,1.0);\n"
"border-bottom-right-radius: 30px;\n"
"background-color:rgba(0, 0, 0, 0.3);\n"
"}"));
        Configurationframe->setFrameShape(QFrame::StyledPanel);
        Configurationframe->setFrameShadow(QFrame::Raised);
        StatusFrame = new QFrame(Configurationframe);
        StatusFrame->setObjectName(QStringLiteral("StatusFrame"));
        StatusFrame->setGeometry(QRect(70, 360, 721, 101));
        StatusFrame->setStyleSheet(QLatin1String("QFrame{\n"
"border:1px solid rgba(29,165,219,1.0);\n"
"border-bottom-right-radius: 30px;\n"
"background-color:rgba(0, 0, 0, 0);\n"
"}"));
        StatusFrame->setFrameShape(QFrame::StyledPanel);
        StatusFrame->setFrameShadow(QFrame::Raised);
        labelTitleStatus = new QLabel(StatusFrame);
        labelTitleStatus->setObjectName(QStringLiteral("labelTitleStatus"));
        labelTitleStatus->setGeometry(QRect(10, 10, 161, 31));
        labelTitleStatus->setFont(font);
        labelTitleStatus->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"border:none;\n"
"};"));
        labelTitleStatus->setAlignment(Qt::AlignCenter);
        labelSWVerNumTitle = new QLabel(StatusFrame);
        labelSWVerNumTitle->setObjectName(QStringLiteral("labelSWVerNumTitle"));
        labelSWVerNumTitle->setGeometry(QRect(20, 50, 101, 42));
        labelSWVerNumTitle->setFont(font);
        labelSWVerNumTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"border:none;\n"
"};"));
        labelSWVerNumTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelSWVerNum = new QLabel(StatusFrame);
        labelSWVerNum->setObjectName(QStringLiteral("labelSWVerNum"));
        labelSWVerNum->setGeometry(QRect(130, 50, 81, 42));
        labelSWVerNum->setFont(font);
        labelSWVerNum->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"border:none;\n"
"};"));
        labelSWVerNum->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelErrRespTitle = new QLabel(StatusFrame);
        labelErrRespTitle->setObjectName(QStringLiteral("labelErrRespTitle"));
        labelErrRespTitle->setGeometry(QRect(490, 50, 101, 42));
        labelErrRespTitle->setFont(font);
        labelErrRespTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"border:none;\n"
"};"));
        labelErrRespTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelErrResp = new QLabel(StatusFrame);
        labelErrResp->setObjectName(QStringLiteral("labelErrResp"));
        labelErrResp->setGeometry(QRect(600, 50, 81, 42));
        labelErrResp->setFont(font);
        labelErrResp->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"border:none;\n"
"};"));
        labelErrResp->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelHWVerNumTitle = new QLabel(StatusFrame);
        labelHWVerNumTitle->setObjectName(QStringLiteral("labelHWVerNumTitle"));
        labelHWVerNumTitle->setGeometry(QRect(260, 50, 101, 42));
        labelHWVerNumTitle->setFont(font);
        labelHWVerNumTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"border:none;\n"
"};"));
        labelHWVerNumTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelHWVerNum = new QLabel(StatusFrame);
        labelHWVerNum->setObjectName(QStringLiteral("labelHWVerNum"));
        labelHWVerNum->setGeometry(QRect(370, 50, 81, 42));
        labelHWVerNum->setFont(font);
        labelHWVerNum->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219);\n"
"border:none;\n"
"};"));
        labelHWVerNum->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelLuminous = new QLabel(Configurationframe);
        labelLuminous->setObjectName(QStringLiteral("labelLuminous"));
        labelLuminous->setGeometry(QRect(480, 120, 121, 42));
        labelLuminous->setFont(font);
        labelLuminous->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelLuminous->setAlignment(Qt::AlignCenter);
        labelBlueAttr = new QLabel(Configurationframe);
        labelBlueAttr->setObjectName(QStringLiteral("labelBlueAttr"));
        labelBlueAttr->setGeometry(QRect(70, 290, 121, 42));
        labelBlueAttr->setFont(font);
        labelBlueAttr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelBlueAttr->setAlignment(Qt::AlignCenter);
        doubleSpinBoxRX = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxRX->setObjectName(QStringLiteral("doubleSpinBoxRX"));
        doubleSpinBoxRX->setGeometry(QRect(210, 170, 101, 41));
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
        doubleSpinBoxBX = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxBX->setObjectName(QStringLiteral("doubleSpinBoxBX"));
        doubleSpinBoxBX->setGeometry(QRect(210, 290, 101, 41));
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
        labelRedAttr = new QLabel(Configurationframe);
        labelRedAttr->setObjectName(QStringLiteral("labelRedAttr"));
        labelRedAttr->setGeometry(QRect(70, 170, 121, 42));
        labelRedAttr->setFont(font);
        labelRedAttr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelRedAttr->setAlignment(Qt::AlignCenter);
        labelYCoordinate = new QLabel(Configurationframe);
        labelYCoordinate->setObjectName(QStringLiteral("labelYCoordinate"));
        labelYCoordinate->setGeometry(QRect(340, 120, 121, 42));
        labelYCoordinate->setFont(font);
        labelYCoordinate->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelYCoordinate->setAlignment(Qt::AlignCenter);
        doubleSpinBoxBY = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxBY->setObjectName(QStringLiteral("doubleSpinBoxBY"));
        doubleSpinBoxBY->setGeometry(QRect(350, 290, 101, 41));
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
        labelGteenAttr = new QLabel(Configurationframe);
        labelGteenAttr->setObjectName(QStringLiteral("labelGteenAttr"));
        labelGteenAttr->setGeometry(QRect(70, 230, 121, 42));
        labelGteenAttr->setFont(font);
        labelGteenAttr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelGteenAttr->setAlignment(Qt::AlignCenter);
        doubleSpinBoxGL = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxGL->setObjectName(QStringLiteral("doubleSpinBoxGL"));
        doubleSpinBoxGL->setGeometry(QRect(490, 230, 101, 41));
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
        doubleSpinBoxRL = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxRL->setObjectName(QStringLiteral("doubleSpinBoxRL"));
        doubleSpinBoxRL->setGeometry(QRect(490, 170, 101, 41));
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
        doubleSpinBoxGX = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxGX->setObjectName(QStringLiteral("doubleSpinBoxGX"));
        doubleSpinBoxGX->setGeometry(QRect(210, 230, 101, 41));
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
        doubleSpinBoxGY = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxGY->setObjectName(QStringLiteral("doubleSpinBoxGY"));
        doubleSpinBoxGY->setGeometry(QRect(350, 230, 101, 41));
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
        labelXCoordinate = new QLabel(Configurationframe);
        labelXCoordinate->setObjectName(QStringLiteral("labelXCoordinate"));
        labelXCoordinate->setGeometry(QRect(200, 120, 121, 42));
        labelXCoordinate->setFont(font);
        labelXCoordinate->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelXCoordinate->setAlignment(Qt::AlignCenter);
        doubleSpinBoxBL = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxBL->setObjectName(QStringLiteral("doubleSpinBoxBL"));
        doubleSpinBoxBL->setGeometry(QRect(490, 290, 101, 41));
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
        doubleSpinBoxRY = new QDoubleSpinBox(Configurationframe);
        doubleSpinBoxRY->setObjectName(QStringLiteral("doubleSpinBoxRY"));
        doubleSpinBoxRY->setGeometry(QRect(350, 170, 101, 41));
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
        pushButtonCancel = new QPushButton(Configurationframe);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(1020, 380, 131, 71));
        pushButtonCancel->setFont(font);
        pushButtonCancel->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonAccept = new QPushButton(Configurationframe);
        pushButtonAccept->setObjectName(QStringLiteral("pushButtonAccept"));
        pushButtonAccept->setGeometry(QRect(820, 380, 131, 71));
        pushButtonAccept->setFont(font);
        pushButtonAccept->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonUnlock = new QPushButton(Configurationframe);
        pushButtonUnlock->setObjectName(QStringLiteral("pushButtonUnlock"));
        pushButtonUnlock->setGeometry(QRect(1061, 290, 90, 42));
        pushButtonUnlock->setFont(font);
        pushButtonUnlock->setStyleSheet(QLatin1String("QPushButton{\n"
"border:0px;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgba(255,251,240,0.5);\n"
"font-size:22px;\n"
"background:rgba(29, 165, 219, 0.1);\n"
"}"));
        labelSerialNO = new QLabel(Configurationframe);
        labelSerialNO->setObjectName(QStringLiteral("labelSerialNO"));
        labelSerialNO->setGeometry(QRect(820, 150, 101, 41));
        labelSerialNO->setFont(font);
        labelSerialNO->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelSerialNO->setAlignment(Qt::AlignCenter);
        lineEditPN = new QLineEdit(Configurationframe);
        lineEditPN->setObjectName(QStringLiteral("lineEditPN"));
        lineEditPN->setGeometry(QRect(930, 220, 221, 41));
        lineEditPN->setStyleSheet(QLatin1String("QLineEdit{\n"
"font-size:22px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(0,0,0,0);\n"
"border:1px solid rgb(29,165,219);\n"
"};"));
        lineEditPN->setMaxLength(12);
        lineEditPN->setAlignment(Qt::AlignCenter);
        labelLockState = new QLabel(Configurationframe);
        labelLockState->setObjectName(QStringLiteral("labelLockState"));
        labelLockState->setGeometry(QRect(820, 290, 101, 41));
        labelLockState->setFont(font);
        labelLockState->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelLockState->setAlignment(Qt::AlignCenter);
        labelPartNO = new QLabel(Configurationframe);
        labelPartNO->setObjectName(QStringLiteral("labelPartNO"));
        labelPartNO->setGeometry(QRect(820, 220, 101, 41));
        labelPartNO->setFont(font);
        labelPartNO->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelPartNO->setAlignment(Qt::AlignCenter);
        lineEditSerial = new QLineEdit(Configurationframe);
        lineEditSerial->setObjectName(QStringLiteral("lineEditSerial"));
        lineEditSerial->setGeometry(QRect(930, 150, 221, 41));
        lineEditSerial->setStyleSheet(QLatin1String("QLineEdit{\n"
"font-size:22px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(0,0,0,0);\n"
"border:1px solid rgb(29,165,219);\n"
"};"));
        lineEditSerial->setMaxLength(21);
        lineEditSerial->setAlignment(Qt::AlignCenter);
        pushButtonLock = new QPushButton(Configurationframe);
        pushButtonLock->setObjectName(QStringLiteral("pushButtonLock"));
        pushButtonLock->setGeometry(QRect(930, 290, 90, 42));
        pushButtonLock->setFont(font);
        pushButtonLock->setStyleSheet(QLatin1String("QPushButton{\n"
"border:0px;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgba(255,251,240,0.5);\n"
"font-size:22px;\n"
"background:rgba(29, 165, 219, 0.1);\n"
"}"));
        labelGroupAddr = new QLabel(Configurationframe);
        labelGroupAddr->setObjectName(QStringLiteral("labelGroupAddr"));
        labelGroupAddr->setGeometry(QRect(350, 60, 121, 42));
        labelGroupAddr->setFont(font);
        labelGroupAddr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelGroupAddr->setAlignment(Qt::AlignCenter);
        spinBoxGA = new QSpinBox(Configurationframe);
        spinBoxGA->setObjectName(QStringLiteral("spinBoxGA"));
        spinBoxGA->setGeometry(QRect(480, 60, 81, 41));
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
        spinBoxSA = new QSpinBox(Configurationframe);
        spinBoxSA->setObjectName(QStringLiteral("spinBoxSA"));
        spinBoxSA->setGeometry(QRect(200, 60, 81, 41));
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
        labelSingleAddr = new QLabel(Configurationframe);
        labelSingleAddr->setObjectName(QStringLiteral("labelSingleAddr"));
        labelSingleAddr->setGeometry(QRect(70, 60, 121, 42));
        labelSingleAddr->setFont(font);
        labelSingleAddr->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelSingleAddr->setAlignment(Qt::AlignCenter);
        spinBoxPlatform = new QSpinBox(Configurationframe);
        spinBoxPlatform->setObjectName(QStringLiteral("spinBoxPlatform"));
        spinBoxPlatform->setGeometry(QRect(770, 60, 81, 41));
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
        labelPlatform = new QLabel(Configurationframe);
        labelPlatform->setObjectName(QStringLiteral("labelPlatform"));
        labelPlatform->setGeometry(QRect(640, 60, 121, 42));
        labelPlatform->setFont(font);
        labelPlatform->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelPlatform->setAlignment(Qt::AlignCenter);
        spinBoxIntensity = new QSpinBox(Configurationframe);
        spinBoxIntensity->setObjectName(QStringLiteral("spinBoxIntensity"));
        spinBoxIntensity->setGeometry(QRect(1070, 60, 81, 41));
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
        labelIntensity = new QLabel(Configurationframe);
        labelIntensity->setObjectName(QStringLiteral("labelIntensity"));
        labelIntensity->setGeometry(QRect(940, 60, 121, 42));
        labelIntensity->setFont(font);
        labelIntensity->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgba(29,165,219,1.0);\n"
"background-color:rgba(0, 0, 0, 0);\n"
"border:none;\n"
"};"));
        labelIntensity->setAlignment(Qt::AlignCenter);
        labelTitle_2 = new QLabel(SlaveNodeFrame);
        labelTitle_2->setObjectName(QStringLiteral("labelTitle_2"));
        labelTitle_2->setGeometry(QRect(60, 80, 1241, 41));
        labelTitle_2->setFont(font);
        labelTitle_2->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
" color:rgba(29,165,219,1.0);\n"
"}"));
        labelTitle_2->setAlignment(Qt::AlignCenter);
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
        lineEditHWVer = new QLineEdit(SlaveNodeFrame);
        lineEditHWVer->setObjectName(QStringLiteral("lineEditHWVer"));
        lineEditHWVer->setGeometry(QRect(160, 670, 101, 42));
        lineEditHWVer->setStyleSheet(QLatin1String("QLineEdit{\n"
"font-size:22px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(0,0,0,0);\n"
"border:none;\n"
"};"));
        lineEditHWVer->setMaxLength(12);
        lineEditHWVer->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        layoutWidget = new QWidget(SlaveNodeFrame);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 2, 2));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(SlaveNodeFrame);

        QMetaObject::connectSlotsByName(SlaveNodeFrame);
    } // setupUi

    void retranslateUi(QWidget *SlaveNodeFrame)
    {
        SlaveNodeFrame->setWindowTitle(QApplication::translate("SlaveNodeFrame", "Form", nullptr));
        labelTitle->setText(QApplication::translate("SlaveNodeFrame", "Slave Node Status And  Configuration", nullptr));
        labelDimRamp_8->setText(QApplication::translate("SlaveNodeFrame", "Version Report", nullptr));
        labelSuppTitle->setText(QApplication::translate("SlaveNodeFrame", "Supplier ID:", nullptr));
        labelSWTitle->setText(QApplication::translate("SlaveNodeFrame", "SW Ver:", nullptr));
        labelHWTitle->setText(QApplication::translate("SlaveNodeFrame", "HW Ver:", nullptr));
        labelFuncTitle->setText(QApplication::translate("SlaveNodeFrame", "Function ID:", nullptr));
        labelVarTitle->setText(QApplication::translate("SlaveNodeFrame", "Varaint ID:", nullptr));
        labelFuncVer->setText(QString());
        labelVariantVer->setText(QString());
        labelSWVer->setText(QString());
        labelSuppVer->setText(QString());
        labelTitleStatus->setText(QApplication::translate("SlaveNodeFrame", "Status Report:", nullptr));
        labelSWVerNumTitle->setText(QApplication::translate("SlaveNodeFrame", "SWVerNum:", nullptr));
        labelSWVerNum->setText(QString());
        labelErrRespTitle->setText(QApplication::translate("SlaveNodeFrame", "ErrResp:", nullptr));
        labelErrResp->setText(QString());
        labelHWVerNumTitle->setText(QApplication::translate("SlaveNodeFrame", "HWVerNum:", nullptr));
        labelHWVerNum->setText(QString());
        labelLuminous->setText(QApplication::translate("SlaveNodeFrame", "Luminous", nullptr));
        labelBlueAttr->setText(QApplication::translate("SlaveNodeFrame", "Blue Color Attr:", nullptr));
        labelRedAttr->setText(QApplication::translate("SlaveNodeFrame", "Red Color Attr:", nullptr));
        labelYCoordinate->setText(QApplication::translate("SlaveNodeFrame", "Y Coordinate", nullptr));
        labelGteenAttr->setText(QApplication::translate("SlaveNodeFrame", "Green Color Attr:", nullptr));
        labelXCoordinate->setText(QApplication::translate("SlaveNodeFrame", "X Coordinate", nullptr));
        pushButtonCancel->setText(QApplication::translate("SlaveNodeFrame", "Exit", nullptr));
        pushButtonAccept->setText(QApplication::translate("SlaveNodeFrame", "Apply", nullptr));
        pushButtonUnlock->setText(QApplication::translate("SlaveNodeFrame", "Unlocked", nullptr));
        labelSerialNO->setText(QApplication::translate("SlaveNodeFrame", "Serial NO.:", nullptr));
        lineEditPN->setText(QString());
        labelLockState->setText(QApplication::translate("SlaveNodeFrame", "Lock State:", nullptr));
        labelPartNO->setText(QApplication::translate("SlaveNodeFrame", "Part NO.:", nullptr));
        lineEditSerial->setText(QString());
        pushButtonLock->setText(QApplication::translate("SlaveNodeFrame", "Locked", nullptr));
        labelGroupAddr->setText(QApplication::translate("SlaveNodeFrame", "CtrlList:", nullptr));
        labelSingleAddr->setText(QApplication::translate("SlaveNodeFrame", "Frame ID:", nullptr));
        labelPlatform->setText(QApplication::translate("SlaveNodeFrame", "Platform:", nullptr));
        labelIntensity->setText(QApplication::translate("SlaveNodeFrame", "Intensity:", nullptr));
        labelTitle_2->setText(QApplication::translate("SlaveNodeFrame", "Slave Node Configuration", nullptr));
        labelTitleCalibration->setText(QApplication::translate("SlaveNodeFrame", "Calibration Function", nullptr));
        pushButtonCalibrateR->setText(QApplication::translate("SlaveNodeFrame", "RED", nullptr));
        pushButtonCalibrateG->setText(QApplication::translate("SlaveNodeFrame", "GREEN", nullptr));
        pushButtonCalibrateB->setText(QApplication::translate("SlaveNodeFrame", "BLUE", nullptr));
        pushButtonNoCalibrate->setText(QApplication::translate("SlaveNodeFrame", "Normal", nullptr));
        lineEditHWVer->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SlaveNodeFrame: public Ui_SlaveNodeFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAVENODEFRAME_H
