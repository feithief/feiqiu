/********************************************************************************
** Form generated from reading UI file 'productionverify.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTIONVERIFY_H
#define UI_PRODUCTIONVERIFY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProductionVerify
{
public:
    QSpinBox *spinBoxIntensity;
    QPushButton *pushButtonNode5;
    QPushButton *pushButtonNode6;
    QPushButton *pushButtonNode7;
    QPushButton *pushButtonNode1;
    QPushButton *pushButtonNode2;
    QPushButton *pushButtonNode3;
    QPushButton *pushButtonNode4;
    QLabel *labelIntensity;
    QSlider *horizontalSliderIntensity;
    QPushButton *pushButtonCancel;
    QLabel *labelTitle;
    QPushButton *pushButtonChangeColorManual;
    QPushButton *pushButtonChangeColorAuto;
    QPushButton *pushButtonNode12;
    QPushButton *pushButtonNode11;
    QFrame *frame;

    void setupUi(QWidget *ProductionVerify)
    {
        if (ProductionVerify->objectName().isEmpty())
            ProductionVerify->setObjectName(QStringLiteral("ProductionVerify"));
        ProductionVerify->resize(1366, 768);
        QFont font;
        font.setFamily(QStringLiteral("WenYue HouXianDaiTi (Non-Commercial Use)"));
        font.setPointSize(28);
        ProductionVerify->setFont(font);
        spinBoxIntensity = new QSpinBox(ProductionVerify);
        spinBoxIntensity->setObjectName(QStringLiteral("spinBoxIntensity"));
        spinBoxIntensity->setGeometry(QRect(120, 370, 91, 41));
        QFont font1;
        font1.setPointSize(28);
        spinBoxIntensity->setFont(font1);
        spinBoxIntensity->setStyleSheet(QLatin1String("QSpinBox{\n"
"border:1px solid rgb(29,165,219);\n"
"color:rgb(255,251,240);\n"
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
        pushButtonNode5 = new QPushButton(ProductionVerify);
        pushButtonNode5->setObjectName(QStringLiteral("pushButtonNode5"));
        pushButtonNode5->setGeometry(QRect(30, 110, 171, 91));
        QFont font2;
        font2.setFamily(QStringLiteral("Agency FB"));
        font2.setPointSize(36);
        pushButtonNode5->setFont(font2);
        pushButtonNode5->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode6 = new QPushButton(ProductionVerify);
        pushButtonNode6->setObjectName(QStringLiteral("pushButtonNode6"));
        pushButtonNode6->setGeometry(QRect(220, 110, 171, 91));
        pushButtonNode6->setFont(font2);
        pushButtonNode6->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode7 = new QPushButton(ProductionVerify);
        pushButtonNode7->setObjectName(QStringLiteral("pushButtonNode7"));
        pushButtonNode7->setGeometry(QRect(410, 110, 171, 91));
        pushButtonNode7->setFont(font2);
        pushButtonNode7->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode1 = new QPushButton(ProductionVerify);
        pushButtonNode1->setObjectName(QStringLiteral("pushButtonNode1"));
        pushButtonNode1->setGeometry(QRect(600, 110, 171, 91));
        pushButtonNode1->setFont(font2);
        pushButtonNode1->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode2 = new QPushButton(ProductionVerify);
        pushButtonNode2->setObjectName(QStringLiteral("pushButtonNode2"));
        pushButtonNode2->setGeometry(QRect(790, 110, 171, 91));
        pushButtonNode2->setFont(font2);
        pushButtonNode2->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode3 = new QPushButton(ProductionVerify);
        pushButtonNode3->setObjectName(QStringLiteral("pushButtonNode3"));
        pushButtonNode3->setGeometry(QRect(980, 110, 171, 91));
        pushButtonNode3->setFont(font2);
        pushButtonNode3->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode4 = new QPushButton(ProductionVerify);
        pushButtonNode4->setObjectName(QStringLiteral("pushButtonNode4"));
        pushButtonNode4->setGeometry(QRect(1170, 110, 171, 91));
        pushButtonNode4->setFont(font2);
        pushButtonNode4->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        labelIntensity = new QLabel(ProductionVerify);
        labelIntensity->setObjectName(QStringLiteral("labelIntensity"));
        labelIntensity->setGeometry(QRect(10, 370, 91, 41));
        labelIntensity->setFont(font1);
        labelIntensity->setStyleSheet(QLatin1String("QLabel{\n"
"color:rgb(29,165,219);\n"
"}"));
        labelIntensity->setAlignment(Qt::AlignCenter);
        horizontalSliderIntensity = new QSlider(ProductionVerify);
        horizontalSliderIntensity->setObjectName(QStringLiteral("horizontalSliderIntensity"));
        horizontalSliderIntensity->setGeometry(QRect(230, 370, 1111, 41));
        horizontalSliderIntensity->setStyleSheet(QLatin1String("QSlider::add-page:Horizontal\n"
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
        horizontalSliderIntensity->setMaximum(100);
        horizontalSliderIntensity->setValue(100);
        horizontalSliderIntensity->setOrientation(Qt::Horizontal);
        pushButtonCancel = new QPushButton(ProductionVerify);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(1170, 640, 161, 91));
        pushButtonCancel->setFont(font1);
        pushButtonCancel->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        labelTitle = new QLabel(ProductionVerify);
        labelTitle->setObjectName(QStringLiteral("labelTitle"));
        labelTitle->setGeometry(QRect(0, 0, 1361, 71));
        QFont font3;
        font3.setPointSize(36);
        labelTitle->setFont(font3);
        labelTitle->setStyleSheet(QLatin1String("QLabel{\n"
"color:rgb(29,165,219);\n"
"}"));
        labelTitle->setAlignment(Qt::AlignCenter);
        pushButtonChangeColorManual = new QPushButton(ProductionVerify);
        pushButtonChangeColorManual->setObjectName(QStringLiteral("pushButtonChangeColorManual"));
        pushButtonChangeColorManual->setGeometry(QRect(70, 460, 551, 121));
        QFont font4;
        font4.setFamily(QStringLiteral("Agency FB"));
        font4.setPointSize(28);
        pushButtonChangeColorManual->setFont(font4);
        pushButtonChangeColorManual->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonChangeColorAuto = new QPushButton(ProductionVerify);
        pushButtonChangeColorAuto->setObjectName(QStringLiteral("pushButtonChangeColorAuto"));
        pushButtonChangeColorAuto->setGeometry(QRect(750, 460, 551, 121));
        pushButtonChangeColorAuto->setFont(font4);
        pushButtonChangeColorAuto->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode12 = new QPushButton(ProductionVerify);
        pushButtonNode12->setObjectName(QStringLiteral("pushButtonNode12"));
        pushButtonNode12->setGeometry(QRect(710, 230, 631, 91));
        pushButtonNode12->setFont(font2);
        pushButtonNode12->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        pushButtonNode11 = new QPushButton(ProductionVerify);
        pushButtonNode11->setObjectName(QStringLiteral("pushButtonNode11"));
        pushButtonNode11->setGeometry(QRect(30, 230, 631, 91));
        pushButtonNode11->setFont(font2);
        pushButtonNode11->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(0,255,255);\n"
"ont-size:22px;\n"
"background:rgba(29, 165, 219, 0.3);\n"
"}"));
        frame = new QFrame(ProductionVerify);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(30, 600, 681, 151));
        frame->setStyleSheet(QStringLiteral("background-image: url(:/PICS/resources/logoNeicon.jpg);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        retranslateUi(ProductionVerify);

        QMetaObject::connectSlotsByName(ProductionVerify);
    } // setupUi

    void retranslateUi(QWidget *ProductionVerify)
    {
        ProductionVerify->setWindowTitle(QApplication::translate("ProductionVerify", "Form", nullptr));
        pushButtonNode5->setText(QApplication::translate("ProductionVerify", "\345\217\270\346\234\272\344\276\247", nullptr));
        pushButtonNode6->setText(QApplication::translate("ProductionVerify", "\345\211\257\351\251\276A", nullptr));
        pushButtonNode7->setText(QApplication::translate("ProductionVerify", "\345\211\257\351\251\276B", nullptr));
        pushButtonNode1->setText(QApplication::translate("ProductionVerify", "\345\267\246\345\211\215\351\227\250", nullptr));
        pushButtonNode2->setText(QApplication::translate("ProductionVerify", "\345\217\263\345\211\215\351\227\250", nullptr));
        pushButtonNode3->setText(QApplication::translate("ProductionVerify", "\345\267\246\345\220\216\351\227\250", nullptr));
        pushButtonNode4->setText(QApplication::translate("ProductionVerify", "\345\217\263\345\220\216\351\227\250", nullptr));
        labelIntensity->setText(QApplication::translate("ProductionVerify", "\344\272\256\345\272\246:", nullptr));
        pushButtonCancel->setText(QApplication::translate("ProductionVerify", "\351\200\200\345\207\272", nullptr));
        labelTitle->setText(QApplication::translate("ProductionVerify", "\346\260\233\345\233\264\347\201\257\347\256\200\345\215\225\346\216\247\345\210\266\347\233\222", nullptr));
        pushButtonChangeColorManual->setText(QApplication::translate("ProductionVerify", "\346\211\213\345\212\250\346\224\271\345\217\230\351\242\234\350\211\262", nullptr));
        pushButtonChangeColorAuto->setText(QApplication::translate("ProductionVerify", "\345\274\200\345\247\213\350\207\252\345\212\250\346\224\271\345\217\230\351\242\234\350\211\262", nullptr));
        pushButtonNode12->setText(QApplication::translate("ProductionVerify", "\345\244\251\347\252\227\345\217\263", nullptr));
        pushButtonNode11->setText(QApplication::translate("ProductionVerify", "\345\244\251\347\252\227\345\267\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProductionVerify: public Ui_ProductionVerify {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTIONVERIFY_H
