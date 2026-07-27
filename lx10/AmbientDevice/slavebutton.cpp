#include "slavebutton.h"

static const QString slavePushButtonEnabledQSS = "QPushButton{\
                                                  border:2px solid #0fbacd;\
                                                  border-top-right-radius: 15px;\
                                                  border-bottom-left-radius: 15px;\
                                                  color:rgb(255,251,240);\
                                                  background:rgba(29, 165, 219, 0.3);\
                                                  }";

static const QString slavePushButtonDisabledQSS = "QPushButton{\
                                                  border:0px;\
                                                  border-top-right-radius: 15px;\
                                                  border-bottom-left-radius: 15px;\
                                                  color:rgba(255,251,240, 0.15);\
                                                  background:rgba(29, 165, 219, 0.15);\
                                                  }";

SlaveButton::SlaveButton(QWidget *parent, int index):
  QPushButton(parent)
{
  buttonIndex = index;
  onLineState = false;
  this->setStyleSheet(slavePushButtonDisabledQSS);
  connect(this, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

void SlaveButton::setButtonEnabled(int index, bool enabled)
{
  this->setEnabled(enabled);

  QString context = "SLAVE ";
  context.append(QString::number(index+1));
  context.append("\n");

  if (enabled == true)
  {
    this->setStyleSheet(slavePushButtonEnabledQSS);
    context.append("ON-Line");
    onLineState = true;
  }
  else
  {
    this->setStyleSheet(slavePushButtonDisabledQSS);
    context.append("OFF-Line");
    onLineState = false;
  }

  this->setText(context);
}

bool SlaveButton::isOnLine()
{
  return onLineState;
}

void SlaveButton::buttonClicked()
{
  emit slaveClicked(buttonIndex);
}
