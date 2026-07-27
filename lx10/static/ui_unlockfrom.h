/********************************************************************************
** Form generated from reading UI file 'unlockfrom.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNLOCKFROM_H
#define UI_UNLOCKFROM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_unlockFrom
{
public:
    QPushButton *pushButtonCancel;
    QLabel *labelTitle;
    QLabel *labelUnlockCode;
    QLabel *labelCode;
    QLineEdit *lineEditCode;
    QPushButton *pushButtonConfirm;
    QPushButton *pushButtonSelMaster;
    QPushButton *pushButtonSelFactory;
    QLabel *labelUnlockSelection;
    QPushButton *pushButtonSelSlave;

    void setupUi(QWidget *unlockFrom)
    {
        if (unlockFrom->objectName().isEmpty())
            unlockFrom->setObjectName(QStringLiteral("unlockFrom"));
        unlockFrom->resize(1366, 768);
        pushButtonCancel = new QPushButton(unlockFrom);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(1180, 650, 131, 71));
        QFont font;
        pushButtonCancel->setFont(font);
        pushButtonCancel->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        labelTitle = new QLabel(unlockFrom);
        labelTitle->setObjectName(QStringLiteral("labelTitle"));
        labelTitle->setGeometry(QRect(0, 0, 1361, 71));
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:32px;\n"
"color:rgb(29,165,219);\n"
"}"));
        labelTitle->setAlignment(Qt::AlignCenter);
        labelUnlockCode = new QLabel(unlockFrom);
        labelUnlockCode->setObjectName(QStringLiteral("labelUnlockCode"));
        labelUnlockCode->setGeometry(QRect(450, 240, 141, 51));
        labelUnlockCode->setFont(font);
        labelUnlockCode->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:26px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelUnlockCode->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelCode = new QLabel(unlockFrom);
        labelCode->setObjectName(QStringLiteral("labelCode"));
        labelCode->setGeometry(QRect(620, 240, 241, 51));
        labelCode->setFont(font);
        labelCode->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:26px;\n"
"color:rgba(29,165,219,1.0);\n"
"};"));
        labelCode->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEditCode = new QLineEdit(unlockFrom);
        lineEditCode->setObjectName(QStringLiteral("lineEditCode"));
        lineEditCode->setGeometry(QRect(450, 320, 521, 51));
        lineEditCode->setStyleSheet(QLatin1String("QLineEdit{\n"
"font-size:22px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(0,0,0,0);\n"
"border:1px solid rgb(29,165,219);\n"
"};"));
        lineEditCode->setMaxLength(22);
        lineEditCode->setAlignment(Qt::AlignCenter);
        pushButtonConfirm = new QPushButton(unlockFrom);
        pushButtonConfirm->setObjectName(QStringLiteral("pushButtonConfirm"));
        pushButtonConfirm->setGeometry(QRect(620, 400, 171, 71));
        pushButtonConfirm->setFont(font);
        pushButtonConfirm->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-top-right-radius: 15px;\n"
"border-bottom-left-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        pushButtonSelMaster = new QPushButton(unlockFrom);
        pushButtonSelMaster->setObjectName(QStringLiteral("pushButtonSelMaster"));
        pushButtonSelMaster->setGeometry(QRect(610, 171, 101, 51));
        pushButtonSelFactory = new QPushButton(unlockFrom);
        pushButtonSelFactory->setObjectName(QStringLiteral("pushButtonSelFactory"));
        pushButtonSelFactory->setGeometry(QRect(870, 171, 101, 51));
        labelUnlockSelection = new QLabel(unlockFrom);
        labelUnlockSelection->setObjectName(QStringLiteral("labelUnlockSelection"));
        labelUnlockSelection->setGeometry(QRect(449, 180, 141, 42));
        labelUnlockSelection->setFont(font);
        labelUnlockSelection->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:28px;\n"
"color:rgb(29, 165, 219);\n"
"}"));
        labelUnlockSelection->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonSelSlave = new QPushButton(unlockFrom);
        pushButtonSelSlave->setObjectName(QStringLiteral("pushButtonSelSlave"));
        pushButtonSelSlave->setGeometry(QRect(740, 171, 101, 51));

        retranslateUi(unlockFrom);

        QMetaObject::connectSlotsByName(unlockFrom);
    } // setupUi

    void retranslateUi(QWidget *unlockFrom)
    {
        unlockFrom->setWindowTitle(QApplication::translate("unlockFrom", "Form", 0));
        pushButtonCancel->setText(QApplication::translate("unlockFrom", "EXIT", 0));
        labelTitle->setText(QApplication::translate("unlockFrom", "Unlock Advanced feature", 0));
        labelUnlockCode->setText(QApplication::translate("unlockFrom", "Unlock Code:", 0));
        labelCode->setText(QString());
        lineEditCode->setText(QString());
        pushButtonConfirm->setText(QApplication::translate("unlockFrom", "Confirm", 0));
        pushButtonSelMaster->setText(QApplication::translate("unlockFrom", "Master", 0));
        pushButtonSelFactory->setText(QApplication::translate("unlockFrom", "Factory", 0));
        labelUnlockSelection->setText(QApplication::translate("unlockFrom", "Unlock Function:", 0));
        pushButtonSelSlave->setText(QApplication::translate("unlockFrom", "Slave", 0));
    } // retranslateUi

};

namespace Ui {
    class unlockFrom: public Ui_unlockFrom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNLOCKFROM_H
