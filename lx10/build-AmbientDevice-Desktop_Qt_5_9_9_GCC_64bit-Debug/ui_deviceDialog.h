/********************************************************************************
** Form generated from reading UI file 'deviceDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICEDIALOG_H
#define UI_DEVICEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogForm
{
public:
    QPushButton *pushButtonOK;
    QFrame *frame;
    QLabel *labelTitle;

    void setupUi(QWidget *DialogForm)
    {
        if (DialogForm->objectName().isEmpty())
            DialogForm->setObjectName(QStringLiteral("DialogForm"));
        DialogForm->resize(1366, 768);
        pushButtonOK = new QPushButton(DialogForm);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(620, 400, 121, 61));
        QFont font;
        pushButtonOK->setFont(font);
        pushButtonOK->setStyleSheet(QLatin1String("QPushButton{\n"
"border:2px solid #0fbacd;\n"
"border-radius: 15px;\n"
"color:rgb(255,251,240);\n"
"background:rgba(29, 165, 219, 0.3);\n"
"font-size:22px;\n"
"}"));
        frame = new QFrame(DialogForm);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(341, 294, 684, 180));
        frame->setStyleSheet(QLatin1String("QFrame{\n"
"border:2px solid rgba(29,165,219,1.0);\n"
"border-radius: 15px;\n"
"background-color:rgba(0, 0, 0, 0.9);\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        labelTitle = new QLabel(DialogForm);
        labelTitle->setObjectName(QStringLiteral("labelTitle"));
        labelTitle->setGeometry(QRect(340, 300, 684, 81));
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QLatin1String("QLabel{\n"
"font-size:32px;\n"
" color:rgb(255,251,240);\n"
"}"));
        labelTitle->setAlignment(Qt::AlignCenter);
        frame->raise();
        pushButtonOK->raise();
        labelTitle->raise();

        retranslateUi(DialogForm);

        QMetaObject::connectSlotsByName(DialogForm);
    } // setupUi

    void retranslateUi(QWidget *DialogForm)
    {
        DialogForm->setWindowTitle(QApplication::translate("DialogForm", "Form", Q_NULLPTR));
        pushButtonOK->setText(QApplication::translate("DialogForm", "OK", Q_NULLPTR));
        labelTitle->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DialogForm: public Ui_DialogForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICEDIALOG_H
