#include "keyboard.h"
#include <QKeyEvent>

static const QString QFrameBasicQSS = "QFrame{\
                                        border:4px solid #0fbacd;\
                                        border-radius: 15px;\
                                        background-color:rgba(0, 0, 0, 0.9);\
                                        }";

KeyBoard::KeyBoard(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AMKeyboard)
{
  backgroundFrame = new QFrame(this);
  backgroundFrame->setGeometry(0,0,1230,353);
  backgroundFrame->setStyleSheet(QFrameBasicQSS);
  backgroundFrame->setFocusPolicy(Qt::NoFocus);
  backgroundFrame->show();

  currentWidget = NULL;

  ui->setupUi(this);

  QList<QPushButton*> buttonList = this->findChildren<QPushButton*>();
  for (int index = 0; index < buttonList.count(); index++)
  {
    QPushButton* temp = buttonList.at(index);
    if (ui->pushButtonOK != temp)
      temp->installEventFilter(this);
  }

  connect(ui->pushButtonOK, SIGNAL(clicked()), this ,SLOT(hideKeyboard()));
}

bool KeyBoard::eventFilter(QObject *watched, QEvent *event)
{
  int forwardKey = 0;
  Qt::KeyboardModifier modifier = Qt::NoModifier;
  QString text;
  QEvent::Type type = event->type();

  if ((type == QEvent::MouseButtonPress) || (type == QEvent::MouseButtonRelease))
  {
    if (watched->inherits("QPushButton") == true)
    {
      QPushButton *button = qobject_cast<QPushButton*>(watched);

      text = button->text();

      if (text.compare("LEFT", Qt::CaseSensitive) == 0)
      {
        text = "";
        forwardKey = Qt::Key_Left;
      }

      if (text.compare("RIGHT", Qt::CaseSensitive) == 0)
      {
        text = "";
        forwardKey = Qt::Key_Right;
      }

      if (text.compare("SPACE", Qt::CaseSensitive) == 0)
      {
        text = " ";
        forwardKey = Qt::Key_Space;
      }

      if (text.compare("BACK\nSPACE", Qt::CaseSensitive) == 0)
      {
        text = "";
        forwardKey = Qt::Key_Backspace;
      }

      if (text.length() == 1)
      {
        forwardKey = text.at(0).toLatin1();
      }

      QWidget *widget = QApplication::focusWidget();
      if (widget)
      {
        if (type == QEvent::MouseButtonPress)
          type = QEvent::KeyPress;
        else if (type == QEvent::MouseButtonRelease)
          type = QEvent::KeyRelease;
        else
          type = QEvent::None;

        QKeyEvent e(type, forwardKey, modifier, text);
        QApplication::sendEvent(widget, &e);
        return true;
      }
    }
  }

  return QObject::eventFilter(watched, event);
}

void KeyBoard::setCurrentObject(QWidget* object)
{
  currentWidget = object;
}

void KeyBoard::hideKeyboard()
{
  if (currentWidget != NULL)
  {
    currentWidget->clearFocus();
    currentWidget = NULL;
  }
  this->hide();
}
