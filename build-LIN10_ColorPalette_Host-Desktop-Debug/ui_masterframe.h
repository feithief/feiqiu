/********************************************************************************
** Form generated from reading UI file 'masterframe.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MASTERFRAME_H
#define UI_MASTERFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MasterFrame
{
public:
    QLabel *labelTitle;
    QLabel *labelAddressMode;
    QLabel *labelAddress;
    QLabel *labelDimRamp;
    QPushButton *pushButtonSingle;
    QPushButton *pushButtonGroup;
    QSpinBox *spinBoxAdress;
    QSpinBox *spinBoxDimRamp;
    QSlider *horizontalSliderAdress;
    QSlider *horizontalSliderDimRamp;
    QFrame *frameCIE;
    QSlider *verticalSliderRed;
    QSlider *verticalSliderGreen;
    QSlider *verticalSliderBlue;
    QSlider *verticalSliderIntensity;
    QLabel *labelValidMarker;
    QPushButton *pushButtonAccept;
    QPushButton *pushButtonCancel;
    QLabel *labelRunMode;
    QLabel *labelX;
    QLabel *labelY;
    QLabel *labelRed;
    QSpinBox *spinBoxBlue;
    QSpinBox *spinBoxIntensity;
    QSpinBox *spinBoxRed;
    QSpinBox *spinBoxGreen;
    QLabel *labelGreen;
    QLabel *labelBlue;
    QLabel *labelIntensity;
    QLabel *labelTitleColorTabel;
    QPushButton *PredefColor;
    QPushButton *RGBColor;
    QPushButton *pushButtonSleep;
    QPushButton *pushButtonAwake;
    QPushButton *pushButtonNext;
    QPushButton *pushButtonPrev;
    QPushButton *LinStop;
    QPushButton *LinStart;
    QLabel *labelAddress_2;
    QLabel *labelAddress_3;
    QPushButton *LED_Enable;
    QPushButton *LED_Disable;
    QPushButton *Fading_Enable;
    QPushButton *Fading_Disable;
    QGroupBox *groupBox1;
    QPushButton *pushButtonColor8;
    QPushButton *pushButtonColor13;
    QPushButton *pushButtonColor12;
    QPushButton *pushButtonColor5;
    QPushButton *pushButtonColor6;
    QPushButton *pushButtonColor11;
    QPushButton *pushButtonColor7;
    QPushButton *pushButtonColor4;
    QPushButton *pushButtonColor3;
    QPushButton *pushButtonColor1;
    QPushButton *pushButtonColor10;
    QPushButton *pushButtonColor14;
    QPushButton *pushButtonColor2;
    QPushButton *pushButtonColor15;
    QPushButton *pushButtonColor9;
    QGroupBox *groupBox2;
    QPushButton *pushButtonColor16;
    QPushButton *pushButtonColor17;
    QPushButton *pushButtonColor18;
    QPushButton *pushButtonColor20;
    QPushButton *pushButtonColor19;
    QPushButton *pushButtonColor21;
    QPushButton *pushButtonColor24;
    QPushButton *pushButtonColor25;
    QPushButton *pushButtonColor22;
    QPushButton *pushButtonColor23;
    QPushButton *pushButtonColor30;
    QPushButton *pushButtonColor29;
    QPushButton *pushButtonColor28;
    QPushButton *pushButtonColor27;
    QPushButton *pushButtonColor26;

    void setupUi(QWidget *MasterFrame)
    {
        if (MasterFrame->objectName().isEmpty())
            MasterFrame->setObjectName(QStringLiteral("MasterFrame"));
        MasterFrame->resize(1366, 768);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MasterFrame->sizePolicy().hasHeightForWidth());
        MasterFrame->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        MasterFrame->setFont(font);
        MasterFrame->setContextMenuPolicy(Qt::DefaultContextMenu);
        MasterFrame->setWindowOpacity(0.3);
        MasterFrame->setAutoFillBackground(false);
        labelTitle = new QLabel(MasterFrame);
        labelTitle->setObjectName(QStringLiteral("labelTitle"));
        labelTitle->setGeometry(QRect(0, 0, 1361, 71));
        QFont font1;
        labelTitle->setFont(font1);
        labelTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:32px;\n"
"color:rgb(29,165,219);\n"
"}"));
        labelTitle->setAlignment(Qt::AlignCenter);
        labelAddressMode = new QLabel(MasterFrame);
        labelAddressMode->setObjectName(QStringLiteral("labelAddressMode"));
        labelAddressMode->setGeometry(QRect(680, 80, 111, 42));
        labelAddressMode->setFont(font1);
        labelAddressMode->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165, 219);\n"
"}"));
        labelAddressMode->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelAddress = new QLabel(MasterFrame);
        labelAddress->setObjectName(QStringLiteral("labelAddress"));
        labelAddress->setGeometry(QRect(10, 80, 131, 42));
        labelAddress->setFont(font1);
        labelAddress->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165,219);\n"
"}"));
        labelAddress->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelDimRamp = new QLabel(MasterFrame);
        labelDimRamp->setObjectName(QStringLiteral("labelDimRamp"));
        labelDimRamp->setGeometry(QRect(0, 160, 141, 42));
        labelDimRamp->setFont(font1);
        labelDimRamp->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165,219);\n"
"}"));
        labelDimRamp->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonSingle = new QPushButton(MasterFrame);
        pushButtonSingle->setObjectName(QStringLiteral("pushButtonSingle"));
        pushButtonSingle->setGeometry(QRect(810, 80, 90, 42));
        pushButtonGroup = new QPushButton(MasterFrame);
        pushButtonGroup->setObjectName(QStringLiteral("pushButtonGroup"));
        pushButtonGroup->setGeometry(QRect(910, 80, 90, 42));
        spinBoxAdress = new QSpinBox(MasterFrame);
        spinBoxAdress->setObjectName(QStringLiteral("spinBoxAdress"));
        spinBoxAdress->setGeometry(QRect(160, 80, 81, 41));
        spinBoxAdress->setStyleSheet(QLatin1String("QSpinBox{\n"
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
        spinBoxAdress->setAlignment(Qt::AlignCenter);
        spinBoxAdress->setMaximum(32767);
        spinBoxAdress->setValue(0);
        spinBoxDimRamp = new QSpinBox(MasterFrame);
        spinBoxDimRamp->setObjectName(QStringLiteral("spinBoxDimRamp"));
        spinBoxDimRamp->setGeometry(QRect(160, 160, 81, 41));
        spinBoxDimRamp->setStyleSheet(QLatin1String("QSpinBox{\n"
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
        spinBoxDimRamp->setAlignment(Qt::AlignCenter);
        spinBoxDimRamp->setMaximum(250);
        spinBoxDimRamp->setValue(10);
        horizontalSliderAdress = new QSlider(MasterFrame);
        horizontalSliderAdress->setObjectName(QStringLiteral("horizontalSliderAdress"));
        horizontalSliderAdress->setGeometry(QRect(260, 83, 391, 35));
        horizontalSliderAdress->setStyleSheet(QLatin1String("QSlider::add-page:Horizontal\n"
"{\n"
"  background-color: rgba(87, 97, 106,0);\n"
"  height:40px;\n"
"}\n"
"QSlider::sub-page:Horizontal\n"
"{\n"
"background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(29,165,219, 180), stop:1 rgba(29,165,219, 180));\n"
"  height:40px;\n"
"}\n"
"QSlider::groove:Horizontal\n"
"{\n"
"  background:transparent;\n"
"  height:46px;\n"
"}\n"
"QSlider::handle:Horizontal\n"
"{\n"
"  background-color:rgb(29,165,219);\n"
"  height: 40px;\n"
"  width:30px;\n"
"}"));
        horizontalSliderAdress->setMaximum(32767);
        horizontalSliderAdress->setValue(0);
        horizontalSliderAdress->setOrientation(Qt::Horizontal);
        horizontalSliderDimRamp = new QSlider(MasterFrame);
        horizontalSliderDimRamp->setObjectName(QStringLiteral("horizontalSliderDimRamp"));
        horizontalSliderDimRamp->setGeometry(QRect(260, 163, 391, 35));
        horizontalSliderDimRamp->setStyleSheet(QLatin1String("QSlider::add-page:Horizontal\n"
"{\n"
"  background-color: rgba(87, 97, 106,0);\n"
"  height:40px;\n"
"}\n"
"QSlider::sub-page:Horizontal\n"
"{\n"
"background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(29,165,219, 180), stop:1 rgba(29,165,219, 180));\n"
"  height:40px;\n"
"}\n"
"QSlider::groove:Horizontal\n"
"{\n"
"  background:transparent;\n"
"  height:46px;\n"
"}\n"
"QSlider::handle:Horizontal\n"
"{\n"
"  background-color:rgb(29,165,219);\n"
"  height: 40px;\n"
"  width:30px;\n"
"}"));
        horizontalSliderDimRamp->setMaximum(250);
        horizontalSliderDimRamp->setValue(2);
        horizontalSliderDimRamp->setOrientation(Qt::Horizontal);
        frameCIE = new QFrame(MasterFrame);
        frameCIE->setObjectName(QStringLiteral("frameCIE"));
        frameCIE->setGeometry(QRect(30, 410, 290, 325));
        frameCIE->setAutoFillBackground(false);
        frameCIE->setStyleSheet(QLatin1String("QFrame{\n"
"border:2px solid #0fbacd;\n"
"background-image: url(:/PICS/resources/CIE1391.bmp);\n"
"}"));
        frameCIE->setFrameShape(QFrame::StyledPanel);
        frameCIE->setFrameShadow(QFrame::Raised);
        verticalSliderRed = new QSlider(MasterFrame);
        verticalSliderRed->setObjectName(QStringLiteral("verticalSliderRed"));
        verticalSliderRed->setGeometry(QRect(368, 240, 35, 461));
        verticalSliderRed->setStyleSheet(QLatin1String("QSlider::sub-page:Vertical\n"
"{\n"
"  background-color: rgba(87, 97, 106,0);\n"
"  width:40px;\n"
"}\n"
"QSlider::add-page:Vertical\n"
"{\n"
"  background-color:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(254,67,101, 140), stop:1 rgba(254,67,101, 200));\n"
"  width:40px;\n"
"}\n"
"QSlider::groove:Vertical\n"
"{\n"
"  background:transparent;\n"
"  width:46px;\n"
"}\n"
"QSlider::handle:Vertical\n"
"{\n"
"  background-color:rgb(254,67,101);\n"
"  width: 40px;\n"
"  height:30px;\n"
"}"));
        verticalSliderRed->setMaximum(255);
        verticalSliderRed->setValue(255);
        verticalSliderRed->setOrientation(Qt::Vertical);
        verticalSliderGreen = new QSlider(MasterFrame);
        verticalSliderGreen->setObjectName(QStringLiteral("verticalSliderGreen"));
        verticalSliderGreen->setGeometry(QRect(448, 240, 35, 461));
        verticalSliderGreen->setStyleSheet(QLatin1String("QSlider::sub-page:Vertical\n"
"{\n"
"  background-color: rgba(87, 97, 106,0);\n"
"  width:40px;\n"
"}\n"
"QSlider::add-page:Vertical\n"
"{\n"
"  background-color:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(104,194,53, 140), stop:1 rgba(106,194,53, 200));\n"
"  width:40px;\n"
"}\n"
"QSlider::groove:Vertical\n"
"{\n"
"  background:transparent;\n"
"  width:46px;\n"
"}\n"
"QSlider::handle:Vertical\n"
"{\n"
"  background-color:rgb(106,194,53);\n"
"  width: 40px;\n"
"  height:30px;\n"
"}"));
        verticalSliderGreen->setMaximum(255);
        verticalSliderGreen->setValue(255);
        verticalSliderGreen->setOrientation(Qt::Vertical);
        verticalSliderBlue = new QSlider(MasterFrame);
        verticalSliderBlue->setObjectName(QStringLiteral("verticalSliderBlue"));
        verticalSliderBlue->setGeometry(QRect(528, 240, 35, 461));
        verticalSliderBlue->setStyleSheet(QLatin1String("QSlider::sub-page:Vertical\n"
"{\n"
"  background-color: rgba(87, 97, 106,0);\n"
"  width:40px;\n"
"}\n"
"QSlider::add-page:Vertical\n"
"{\n"
"  background-color:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0,95,255, 140), stop:1 rgba(0,95,255, 200));\n"
"  width:40px;\n"
"}\n"
"QSlider::groove:Vertical\n"
"{\n"
"  background:transparent;\n"
"  width:46px;\n"
"}\n"
"QSlider::handle:Vertical\n"
"{\n"
"  background-color:rgb(0,95,255);\n"
"  width: 40px;\n"
"  height:30px;\n"
"}"));
        verticalSliderBlue->setMaximum(255);
        verticalSliderBlue->setValue(255);
        verticalSliderBlue->setOrientation(Qt::Vertical);
        verticalSliderIntensity = new QSlider(MasterFrame);
        verticalSliderIntensity->setObjectName(QStringLiteral("verticalSliderIntensity"));
        verticalSliderIntensity->setGeometry(QRect(608, 240, 35, 461));
        verticalSliderIntensity->setStyleSheet(QLatin1String("QSlider::sub-page:Vertical\n"
"{\n"
"  background-color: rgba(87, 97, 106,0);\n"
"  width:40px;\n"
"}\n"
"QSlider::add-page:Vertical\n"
"{\n"
"  background-color:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(29,165,219, 140), stop:1 rgba(29,165,219, 200));\n"
"  width:40px;\n"
"}\n"
"QSlider::groove:Vertical\n"
"{\n"
"  background:transparent;\n"
"  width:46px;\n"
"}\n"
"QSlider::handle:Vertical\n"
"{\n"
"  background-color:rgb(29,165,219);\n"
"  width: 40px;\n"
"  height:30px;\n"
"}"));
        verticalSliderIntensity->setMaximum(100);
        verticalSliderIntensity->setValue(100);
        verticalSliderIntensity->setOrientation(Qt::Vertical);
        verticalSliderIntensity->setInvertedAppearance(false);
        verticalSliderIntensity->setInvertedControls(false);
        labelValidMarker = new QLabel(MasterFrame);
        labelValidMarker->setObjectName(QStringLiteral("labelValidMarker"));
        labelValidMarker->setGeometry(QRect(1000, 80, 110, 42));
        labelValidMarker->setFont(font1);
        labelValidMarker->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165, 219);\n"
"}"));
        labelValidMarker->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonAccept = new QPushButton(MasterFrame);
        pushButtonAccept->setObjectName(QStringLiteral("pushButtonAccept"));
        pushButtonAccept->setGeometry(QRect(1070, 650, 131, 71));
        pushButtonAccept->setFont(font1);
        pushButtonAccept->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonCancel = new QPushButton(MasterFrame);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(1210, 650, 131, 71));
        pushButtonCancel->setFont(font1);
        pushButtonCancel->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        labelRunMode = new QLabel(MasterFrame);
        labelRunMode->setObjectName(QStringLiteral("labelRunMode"));
        labelRunMode->setGeometry(QRect(1000, 160, 110, 42));
        labelRunMode->setFont(font1);
        labelRunMode->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165, 219);\n"
"}"));
        labelRunMode->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelX = new QLabel(MasterFrame);
        labelX->setObjectName(QStringLiteral("labelX"));
        labelX->setGeometry(QRect(40, 300, 131, 41));
        labelX->setFont(font1);
        labelX->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165,219);\n"
"}"));
        labelY = new QLabel(MasterFrame);
        labelY->setObjectName(QStringLiteral("labelY"));
        labelY->setGeometry(QRect(40, 350, 131, 41));
        labelY->setFont(font1);
        labelY->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165,219);\n"
"}"));
        labelRed = new QLabel(MasterFrame);
        labelRed->setObjectName(QStringLiteral("labelRed"));
        labelRed->setGeometry(QRect(360, 210, 51, 31));
        labelRed->setFont(font1);
        labelRed->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29,165,219);\n"
"}"));
        labelRed->setAlignment(Qt::AlignCenter);
        spinBoxBlue = new QSpinBox(MasterFrame);
        spinBoxBlue->setObjectName(QStringLiteral("spinBoxBlue"));
        spinBoxBlue->setGeometry(QRect(520, 710, 51, 41));
        spinBoxBlue->setStyleSheet(QLatin1String("QSpinBox{\n"
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
        spinBoxBlue->setAlignment(Qt::AlignCenter);
        spinBoxBlue->setMaximum(255);
        spinBoxIntensity = new QSpinBox(MasterFrame);
        spinBoxIntensity->setObjectName(QStringLiteral("spinBoxIntensity"));
        spinBoxIntensity->setGeometry(QRect(600, 710, 51, 41));
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
        spinBoxIntensity->setMaximum(100);
        spinBoxRed = new QSpinBox(MasterFrame);
        spinBoxRed->setObjectName(QStringLiteral("spinBoxRed"));
        spinBoxRed->setGeometry(QRect(360, 710, 51, 41));
        spinBoxRed->setFont(font1);
        spinBoxRed->setStyleSheet(QLatin1String("QSpinBox{\n"
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
        spinBoxRed->setAlignment(Qt::AlignCenter);
        spinBoxRed->setMaximum(255);
        spinBoxRed->setValue(255);
        spinBoxGreen = new QSpinBox(MasterFrame);
        spinBoxGreen->setObjectName(QStringLiteral("spinBoxGreen"));
        spinBoxGreen->setGeometry(QRect(440, 710, 51, 41));
        spinBoxGreen->setFont(font1);
        spinBoxGreen->setStyleSheet(QLatin1String("QSpinBox{\n"
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
        spinBoxGreen->setAlignment(Qt::AlignCenter);
        spinBoxGreen->setMaximum(255);
        labelGreen = new QLabel(MasterFrame);
        labelGreen->setObjectName(QStringLiteral("labelGreen"));
        labelGreen->setGeometry(QRect(440, 210, 51, 31));
        labelGreen->setFont(font1);
        labelGreen->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29,165,219);\n"
"}"));
        labelGreen->setAlignment(Qt::AlignCenter);
        labelBlue = new QLabel(MasterFrame);
        labelBlue->setObjectName(QStringLiteral("labelBlue"));
        labelBlue->setGeometry(QRect(520, 210, 51, 31));
        labelBlue->setFont(font1);
        labelBlue->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29,165,219);\n"
"}"));
        labelBlue->setAlignment(Qt::AlignCenter);
        labelIntensity = new QLabel(MasterFrame);
        labelIntensity->setObjectName(QStringLiteral("labelIntensity"));
        labelIntensity->setGeometry(QRect(580, 210, 91, 31));
        labelIntensity->setFont(font1);
        labelIntensity->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29,165,219);\n"
"}"));
        labelIntensity->setAlignment(Qt::AlignCenter);
        labelTitleColorTabel = new QLabel(MasterFrame);
        labelTitleColorTabel->setObjectName(QStringLiteral("labelTitleColorTabel"));
        labelTitleColorTabel->setGeometry(QRect(680, 140, 110, 42));
        labelTitleColorTabel->setFont(font1);
        labelTitleColorTabel->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165, 219);\n"
"}"));
        labelTitleColorTabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PredefColor = new QPushButton(MasterFrame);
        PredefColor->setObjectName(QStringLiteral("PredefColor"));
        PredefColor->setGeometry(QRect(810, 130, 141, 41));
        PredefColor->setFont(font1);
        PredefColor->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        RGBColor = new QPushButton(MasterFrame);
        RGBColor->setObjectName(QStringLiteral("RGBColor"));
        RGBColor->setEnabled(false);
        RGBColor->setGeometry(QRect(810, 170, 141, 41));
        RGBColor->setFont(font1);
        RGBColor->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonSleep = new QPushButton(MasterFrame);
        pushButtonSleep->setObjectName(QStringLiteral("pushButtonSleep"));
        pushButtonSleep->setGeometry(QRect(170, 220, 131, 71));
        pushButtonSleep->setFont(font1);
        pushButtonSleep->setCursor(QCursor(Qt::PointingHandCursor));
        pushButtonSleep->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonAwake = new QPushButton(MasterFrame);
        pushButtonAwake->setObjectName(QStringLiteral("pushButtonAwake"));
        pushButtonAwake->setGeometry(QRect(170, 220, 131, 71));
        pushButtonAwake->setFont(font1);
        pushButtonAwake->setCursor(QCursor(Qt::PointingHandCursor));
        pushButtonAwake->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonNext = new QPushButton(MasterFrame);
        pushButtonNext->setObjectName(QStringLiteral("pushButtonNext"));
        pushButtonNext->setVisible(false);
        pushButtonNext->setGeometry(QRect(810, 640, 101, 81));
        QFont font2;
        font2.setFamily(QStringLiteral("Noto Sans CJK SC"));
        font2.setPointSize(50);
        font2.setBold(true);
        font2.setWeight(75);
        pushButtonNext->setFont(font2);
        pushButtonNext->setCursor(QCursor(Qt::PointingHandCursor));
        pushButtonNext->setVisible(false);
        pushButtonNext->setStyleSheet(QLatin1String("QPushButton{\n"
"border:none;\n"
"border-top-right-radius: 0;\n"
"border-bottom-left-radius: 0;\n"
"color:rgb(24,255,27);\n"
"ont-size:50px;\n"
"background:rgba(29, 165, 219, 0);\n"
"}"));
        pushButtonPrev = new QPushButton(MasterFrame);
        pushButtonPrev->setObjectName(QStringLiteral("pushButtonPrev"));
        pushButtonPrev->setVisible(false);
        pushButtonPrev->setGeometry(QRect(690, 640, 101, 81));
        QFont font3;
        font3.setFamily(QStringLiteral("Noto Sans CJK SC"));
        font3.setPointSize(50);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setWeight(75);
        font3.setKerning(false);
        pushButtonPrev->setFont(font3);
        pushButtonPrev->setCursor(QCursor(Qt::PointingHandCursor));
        pushButtonPrev->setVisible(false);
        pushButtonPrev->setStyleSheet(QLatin1String("QPushButton{\n"
"border:none;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(24,255,27);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0);\n"
"}"));
        pushButtonPrev->setCheckable(false);
        pushButtonPrev->setAutoRepeat(false);
        pushButtonPrev->setAutoExclusive(false);
        LinStop = new QPushButton(MasterFrame);
        LinStop->setObjectName(QStringLiteral("LinStop"));
        LinStop->setGeometry(QRect(140, 0, 131, 71));
        QFont font4;
        font4.setFamily(QStringLiteral("Noto Sans CJK SC"));
        LinStop->setFont(font4);
        LinStop->setCursor(QCursor(Qt::PointingHandCursor));
        LinStop->setStyleSheet(QLatin1String("QPushButton{\n"
"border:none;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,0,0);\n"
"background:rgba(29, 165, 219, 0);\n"
"font-size:50px;\n"
"}"));
        LinStart = new QPushButton(MasterFrame);
        LinStart->setObjectName(QStringLiteral("LinStart"));
        LinStart->setGeometry(QRect(140, -10, 141, 81));
        LinStart->setFont(font4);
        LinStart->setCursor(QCursor(Qt::PointingHandCursor));
        LinStart->setStyleSheet(QLatin1String("QPushButton{\n"
"border:none;\n"
"border-top-right-radius: 0px;\n"
"border-bottom-left-radius: 0px;\n"
"color:rgb(0,255,0);\n"
"background:rgba(29, 165, 219, 0);\n"
"font-size:85px;\n"
"}"));
        labelAddress_2 = new QLabel(MasterFrame);
        labelAddress_2->setObjectName(QStringLiteral("labelAddress_2"));
        labelAddress_2->setGeometry(QRect(10, 20, 131, 42));
        labelAddress_2->setFont(font1);
        labelAddress_2->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165,219);\n"
"}"));
        labelAddress_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelAddress_3 = new QLabel(MasterFrame);
        labelAddress_3->setObjectName(QStringLiteral("labelAddress_3"));
        labelAddress_3->setGeometry(QRect(0, 230, 131, 42));
        labelAddress_3->setFont(font1);
        labelAddress_3->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:22px;\n"
"color:rgb(29, 165,219);\n"
"}"));
        labelAddress_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        LED_Enable = new QPushButton(MasterFrame);
        LED_Enable->setObjectName(QStringLiteral("LED_Enable"));
        LED_Enable->setGeometry(QRect(1150, 70, 141, 41));
        LED_Enable->setFont(font1);
        LED_Enable->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        LED_Disable = new QPushButton(MasterFrame);
        LED_Disable->setObjectName(QStringLiteral("LED_Disable"));
        LED_Disable->setGeometry(QRect(1150, 110, 141, 41));
        LED_Disable->setFont(font1);
        LED_Disable->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        Fading_Enable = new QPushButton(MasterFrame);
        Fading_Enable->setObjectName(QStringLiteral("Fading_Enable"));
        Fading_Enable->setGeometry(QRect(1150, 170, 141, 41));
        Fading_Enable->setFont(font1);
        Fading_Enable->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        Fading_Disable = new QPushButton(MasterFrame);
        Fading_Disable->setObjectName(QStringLiteral("Fading_Disable"));
        Fading_Disable->setGeometry(QRect(1150, 210, 141, 41));
        Fading_Disable->setFont(font1);
        Fading_Disable->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        groupBox1 = new QGroupBox(MasterFrame);
        groupBox1->setObjectName(QStringLiteral("groupBox1"));
        groupBox1->setGeometry(QRect(690, 280, 661, 241));
        pushButtonColor8 = new QPushButton(groupBox1);
        pushButtonColor8->setObjectName(QStringLiteral("pushButtonColor8"));
        pushButtonColor8->setGeometry(QRect(260, 95, 111, 61));
        QFont font5;
        font5.setFamily(QStringLiteral("Agency FB"));
        font5.setPointSize(22);
        pushButtonColor8->setFont(font5);
        pushButtonColor8->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,0);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor13 = new QPushButton(groupBox1);
        pushButtonColor13->setObjectName(QStringLiteral("pushButtonColor13"));
        pushButtonColor13->setGeometry(QRect(260, 170, 111, 61));
        pushButtonColor13->setFont(font5);
        pushButtonColor13->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(252, 233, 79);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor12 = new QPushButton(groupBox1);
        pushButtonColor12->setObjectName(QStringLiteral("pushButtonColor12"));
        pushButtonColor12->setGeometry(QRect(135, 170, 111, 61));
        pushButtonColor12->setFont(font5);
        pushButtonColor12->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(252, 233, 79);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor5 = new QPushButton(groupBox1);
        pushButtonColor5->setObjectName(QStringLiteral("pushButtonColor5"));
        pushButtonColor5->setGeometry(QRect(510, 20, 111, 61));
        pushButtonColor5->setFont(font5);
        pushButtonColor5->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,0);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor6 = new QPushButton(groupBox1);
        pushButtonColor6->setObjectName(QStringLiteral("pushButtonColor6"));
        pushButtonColor6->setGeometry(QRect(10, 95, 111, 61));
        pushButtonColor6->setFont(font5);
        pushButtonColor6->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,0);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor11 = new QPushButton(groupBox1);
        pushButtonColor11->setObjectName(QStringLiteral("pushButtonColor11"));
        pushButtonColor11->setGeometry(QRect(10, 170, 111, 61));
        pushButtonColor11->setFont(font5);
        pushButtonColor11->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(252, 233, 79);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor7 = new QPushButton(groupBox1);
        pushButtonColor7->setObjectName(QStringLiteral("pushButtonColor7"));
        pushButtonColor7->setGeometry(QRect(135, 95, 111, 61));
        pushButtonColor7->setFont(font5);
        pushButtonColor7->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(115, 210, 22);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor4 = new QPushButton(groupBox1);
        pushButtonColor4->setObjectName(QStringLiteral("pushButtonColor4"));
        pushButtonColor4->setGeometry(QRect(385, 20, 111, 61));
        pushButtonColor4->setFont(font5);
        pushButtonColor4->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,69,0);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor3 = new QPushButton(groupBox1);
        pushButtonColor3->setObjectName(QStringLiteral("pushButtonColor3"));
        pushButtonColor3->setGeometry(QRect(260, 20, 111, 61));
        pushButtonColor3->setFont(font5);
        pushButtonColor3->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(177,17,22);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor1 = new QPushButton(groupBox1);
        pushButtonColor1->setObjectName(QStringLiteral("pushButtonColor1"));
        pushButtonColor1->setGeometry(QRect(10, 20, 111, 61));
        pushButtonColor1->setFont(font5);
        pushButtonColor1->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(238, 238, 236);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor10 = new QPushButton(groupBox1);
        pushButtonColor10->setObjectName(QStringLiteral("pushButtonColor10"));
        pushButtonColor10->setGeometry(QRect(510, 95, 111, 61));
        pushButtonColor10->setFont(font5);
        pushButtonColor10->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(239,215,0);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor14 = new QPushButton(groupBox1);
        pushButtonColor14->setObjectName(QStringLiteral("pushButtonColor14"));
        pushButtonColor14->setGeometry(QRect(385, 170, 111, 61));
        pushButtonColor14->setFont(font5);
        pushButtonColor14->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(252, 233, 79);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor2 = new QPushButton(groupBox1);
        pushButtonColor2->setObjectName(QStringLiteral("pushButtonColor2"));
        pushButtonColor2->setGeometry(QRect(135, 20, 111, 61));
        pushButtonColor2->setFont(font5);
        pushButtonColor2->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(196, 160, 0);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor15 = new QPushButton(groupBox1);
        pushButtonColor15->setObjectName(QStringLiteral("pushButtonColor15"));
        pushButtonColor15->setGeometry(QRect(510, 170, 111, 61));
        pushButtonColor15->setFont(font5);
        pushButtonColor15->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(138,226,24);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor9 = new QPushButton(groupBox1);
        pushButtonColor9->setObjectName(QStringLiteral("pushButtonColor9"));
        pushButtonColor9->setGeometry(QRect(385, 95, 111, 61));
        pushButtonColor9->setFont(font5);
        pushButtonColor9->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(250,128,10);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        groupBox2 = new QGroupBox(MasterFrame);
        groupBox2->setObjectName(QStringLiteral("groupBox2"));
        groupBox2->setGeometry(QRect(690, 520, 661, 91));
        pushButtonColor16 = new QPushButton(groupBox2);
        pushButtonColor16->setObjectName(QStringLiteral("pushButtonColor16"));
        pushButtonColor16->setGeometry(QRect(10, 20, 111, 61));
        QFont font6;
        font6.setFamily(QStringLiteral("Agency FB"));
        font6.setPointSize(22);
        font6.setBold(false);
        font6.setWeight(50);
        pushButtonColor16->setFont(font6);
        pushButtonColor16->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(18, 250, 235);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor17 = new QPushButton(groupBox2);
        pushButtonColor17->setObjectName(QStringLiteral("pushButtonColor17"));
        pushButtonColor17->setGeometry(QRect(135, 20, 111, 61));
        pushButtonColor17->setFont(font6);
        pushButtonColor17->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(18, 250, 235);\n"
"ont-size:16px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor18 = new QPushButton(groupBox2);
        pushButtonColor18->setObjectName(QStringLiteral("pushButtonColor18"));
        pushButtonColor18->setGeometry(QRect(260, 20, 111, 61));
        pushButtonColor18->setFont(font6);
        pushButtonColor18->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(18, 250, 235);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor20 = new QPushButton(groupBox2);
        pushButtonColor20->setObjectName(QStringLiteral("pushButtonColor20"));
        pushButtonColor20->setGeometry(QRect(510, 20, 111, 61));
        pushButtonColor20->setFont(font6);
        pushButtonColor20->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(18, 250, 235);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor19 = new QPushButton(groupBox2);
        pushButtonColor19->setObjectName(QStringLiteral("pushButtonColor19"));
        pushButtonColor19->setGeometry(QRect(385, 20, 111, 61));
        pushButtonColor19->setFont(font6);
        pushButtonColor19->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(18, 250, 235);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor21 = new QPushButton(groupBox2);
        pushButtonColor21->setObjectName(QStringLiteral("pushButtonColor21"));
        pushButtonColor21->setGeometry(QRect(0, 130, 121, 81));
        pushButtonColor21->setFont(font6);
        pushButtonColor21->setVisible(false);
        pushButtonColor21->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(114,255,3);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor24 = new QPushButton(groupBox2);
        pushButtonColor24->setObjectName(QStringLiteral("pushButtonColor24"));
        pushButtonColor24->setGeometry(QRect(380, 130, 101, 81));
        pushButtonColor24->setFont(font6);
        pushButtonColor24->setVisible(false);
        pushButtonColor24->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,32,244);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor25 = new QPushButton(groupBox2);
        pushButtonColor25->setObjectName(QStringLiteral("pushButtonColor25"));
        pushButtonColor25->setGeometry(QRect(500, 130, 101, 81));
        pushButtonColor25->setFont(font6);
        pushButtonColor25->setVisible(false);
        pushButtonColor25->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,32,244);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor22 = new QPushButton(groupBox2);
        pushButtonColor22->setObjectName(QStringLiteral("pushButtonColor22"));
        pushButtonColor22->setGeometry(QRect(140, 130, 101, 81));
        pushButtonColor22->setFont(font6);
        pushButtonColor22->setVisible(false);
        pushButtonColor22->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(142,60,255);\n"
"ont-size:16px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor23 = new QPushButton(groupBox2);
        pushButtonColor23->setObjectName(QStringLiteral("pushButtonColor23"));
        pushButtonColor23->setGeometry(QRect(260, 130, 101, 81));
        QFont font7;
        font7.setFamily(QStringLiteral("Agency FB"));
        font7.setPointSize(21);
        font7.setBold(false);
        font7.setWeight(50);
        pushButtonColor23->setFont(font7);
        pushButtonColor23->setVisible(false);
        pushButtonColor23->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(188,30,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor30 = new QPushButton(groupBox2);
        pushButtonColor30->setObjectName(QStringLiteral("pushButtonColor30"));
        pushButtonColor30->setGeometry(QRect(500, 220, 101, 81));
        QFont font8;
        font8.setFamily(QStringLiteral("Agency FB"));
        font8.setPointSize(20);
        font8.setBold(false);
        font8.setWeight(50);
        pushButtonColor30->setFont(font8);
        pushButtonColor30->setVisible(false);
        pushButtonColor30->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(252, 175, 62);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor29 = new QPushButton(groupBox2);
        pushButtonColor29->setObjectName(QStringLiteral("pushButtonColor29"));
        pushButtonColor29->setGeometry(QRect(380, 220, 101, 81));
        pushButtonColor29->setFont(font6);
        pushButtonColor29->setVisible(false);
        pushButtonColor29->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(238, 114, 114);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor28 = new QPushButton(groupBox2);
        pushButtonColor28->setObjectName(QStringLiteral("pushButtonColor28"));
        pushButtonColor28->setGeometry(QRect(260, 220, 101, 81));
        pushButtonColor28->setFont(font8);
        pushButtonColor28->setVisible(false);
        pushButtonColor28->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,138,42);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor27 = new QPushButton(groupBox2);
        pushButtonColor27->setObjectName(QStringLiteral("pushButtonColor27"));
        pushButtonColor27->setGeometry(QRect(140, 220, 101, 81));
        pushButtonColor27->setFont(font6);
        pushButtonColor27->setVisible(false);
        pushButtonColor27->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(238, 114, 114);\n"
"ont-size:16px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonColor26 = new QPushButton(groupBox2);
        pushButtonColor26->setObjectName(QStringLiteral("pushButtonColor26"));
        pushButtonColor26->setGeometry(QRect(0, 220, 121, 81));
        pushButtonColor26->setFont(font6);
        pushButtonColor26->setVisible(false);
        pushButtonColor26->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,32,244);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));

        retranslateUi(MasterFrame);

        QMetaObject::connectSlotsByName(MasterFrame);
    } // setupUi

    void retranslateUi(QWidget *MasterFrame)
    {
        MasterFrame->setWindowTitle(QApplication::translate("MasterFrame", "Form", nullptr));
        labelTitle->setText(QApplication::translate("MasterFrame", "BCM Master Control Panel", nullptr));
        labelAddressMode->setText(QApplication::translate("MasterFrame", "Target Mode:", nullptr));
        labelAddress->setText(QApplication::translate("MasterFrame", "Target Mask:", nullptr));
        labelDimRamp->setText(QApplication::translate("MasterFrame", "Fading Time:", nullptr));
        pushButtonSingle->setText(QApplication::translate("MasterFrame", "Single", nullptr));
        pushButtonGroup->setText(QApplication::translate("MasterFrame", "Group", nullptr));
        labelValidMarker->setText(QApplication::translate("MasterFrame", "LED_Enable", nullptr));
        pushButtonAccept->setText(QApplication::translate("MasterFrame", "Apply", nullptr));
        pushButtonCancel->setText(QApplication::translate("MasterFrame", "EXIT", nullptr));
        labelRunMode->setText(QApplication::translate("MasterFrame", "Fading_Enable", nullptr));
        labelX->setText(QApplication::translate("MasterFrame", "X:", nullptr));
        labelY->setText(QApplication::translate("MasterFrame", "Y:", nullptr));
        labelRed->setText(QApplication::translate("MasterFrame", "RED", nullptr));
        labelGreen->setText(QApplication::translate("MasterFrame", "GREEN", nullptr));
        labelBlue->setText(QApplication::translate("MasterFrame", "BLUE", nullptr));
        labelIntensity->setText(QApplication::translate("MasterFrame", "INTENSITY", nullptr));
        labelTitleColorTabel->setText(QApplication::translate("MasterFrame", "Color Table:", nullptr));
        PredefColor->setText(QApplication::translate("MasterFrame", "Color #0  M0", nullptr));
#ifndef QT_NO_TOOLTIP
        RGBColor->setToolTip(QApplication::translate("MasterFrame", "The current slave firmware supports predefined colors only.", nullptr));
#endif // QT_NO_TOOLTIP
        RGBColor->setText(QApplication::translate("MasterFrame", "RGB Disabled", nullptr));
        pushButtonSleep->setText(QApplication::translate("MasterFrame", "Sleep", nullptr));
        pushButtonAwake->setText(QApplication::translate("MasterFrame", "Awake", nullptr));
        pushButtonNext->setText(QApplication::translate("MasterFrame", "\342\236\241", nullptr));
        pushButtonPrev->setText(QApplication::translate("MasterFrame", "\342\254\205", nullptr));
        LinStop->setText(QApplication::translate("MasterFrame", "\342\227\217", nullptr));
        LinStart->setText(QApplication::translate("MasterFrame", "\342\226\270", nullptr));
        labelAddress_2->setText(QApplication::translate("MasterFrame", "LinSignal:", nullptr));
        labelAddress_3->setText(QApplication::translate("MasterFrame", "State:", nullptr));
        LED_Enable->setText(QApplication::translate("MasterFrame", "Enable", nullptr));
        LED_Disable->setText(QApplication::translate("MasterFrame", "Disable", nullptr));
        Fading_Enable->setText(QApplication::translate("MasterFrame", "Enable", nullptr));
        Fading_Disable->setText(QApplication::translate("MasterFrame", "Disable", nullptr));
        groupBox1->setTitle(QApplication::translate("MasterFrame", "Predefined Colors #0 - #14", nullptr));
        pushButtonColor8->setText(QApplication::translate("MasterFrame", "#7", nullptr));
        pushButtonColor13->setText(QApplication::translate("MasterFrame", "#12", nullptr));
        pushButtonColor12->setText(QApplication::translate("MasterFrame", "#11", nullptr));
        pushButtonColor5->setText(QApplication::translate("MasterFrame", "#4", nullptr));
        pushButtonColor6->setText(QApplication::translate("MasterFrame", "#5", nullptr));
        pushButtonColor11->setText(QApplication::translate("MasterFrame", "#10", nullptr));
        pushButtonColor7->setText(QApplication::translate("MasterFrame", "#6", nullptr));
        pushButtonColor4->setText(QApplication::translate("MasterFrame", "#3", nullptr));
        pushButtonColor3->setText(QApplication::translate("MasterFrame", "#2", nullptr));
        pushButtonColor1->setText(QApplication::translate("MasterFrame", "#0", nullptr));
        pushButtonColor10->setText(QApplication::translate("MasterFrame", "#9", nullptr));
        pushButtonColor14->setText(QApplication::translate("MasterFrame", "#13", nullptr));
        pushButtonColor2->setText(QApplication::translate("MasterFrame", "#1", nullptr));
        pushButtonColor15->setText(QApplication::translate("MasterFrame", "#14", nullptr));
        pushButtonColor9->setText(QApplication::translate("MasterFrame", "#8", nullptr));
        groupBox2->setTitle(QApplication::translate("MasterFrame", "Predefined Colors #15 - #19", nullptr));
        pushButtonColor16->setText(QApplication::translate("MasterFrame", "#15", nullptr));
        pushButtonColor17->setText(QApplication::translate("MasterFrame", "#16", nullptr));
        pushButtonColor18->setText(QApplication::translate("MasterFrame", "#17", nullptr));
        pushButtonColor20->setText(QApplication::translate("MasterFrame", "#19", nullptr));
        pushButtonColor19->setText(QApplication::translate("MasterFrame", "#18", nullptr));
        pushButtonColor21->setText(QApplication::translate("MasterFrame", "Reserved #20", nullptr));
        pushButtonColor24->setText(QApplication::translate("MasterFrame", "Reserved #23", nullptr));
        pushButtonColor25->setText(QApplication::translate("MasterFrame", "Reserved #24", nullptr));
        pushButtonColor22->setText(QApplication::translate("MasterFrame", "Reserved #21", nullptr));
        pushButtonColor23->setText(QApplication::translate("MasterFrame", "Reserved #22", nullptr));
        pushButtonColor30->setText(QApplication::translate("MasterFrame", "Reserved #29", nullptr));
        pushButtonColor29->setText(QApplication::translate("MasterFrame", "Reserved #28", nullptr));
        pushButtonColor28->setText(QApplication::translate("MasterFrame", "Reserved #27", nullptr));
        pushButtonColor27->setText(QApplication::translate("MasterFrame", "Reserved #26", nullptr));
        pushButtonColor26->setText(QApplication::translate("MasterFrame", "Reserved #25", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MasterFrame: public Ui_MasterFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MASTERFRAME_H
