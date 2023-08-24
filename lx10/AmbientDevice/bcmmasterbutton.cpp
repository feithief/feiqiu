#include "bcmmasterbutton.h"
#include <QString>
#include <QPainter>
#include <QEvent>
#include <QPen>

static const char BCMStyleSheetFormat[] = "QPushButton{"
                                          "border:0px;"
                                          "color:rgb(255,251,240);"
                                          "text-align:center;"
                                          "background-color: qradialgradient(spread:pad, cx:0.489796, cy:0.494, radius:0.45, fx:0.489796, fy:0.494, stop:0.75 rgba(29, 165, 219, %d), stop:0.95 rgba(0, 0, 0, 0));"
                                          "}";

static const char ExitFormat[] = "QPushButton{\
                                    border:0px;\
                                    color:rgb(255,251,240);\
                                    text-align:center;\
                                    background-color: qradialgradient(spread:pad, cx:0.489796, cy:0.494, radius:0.45, fx:0.489796, fy:0.494, stop:0.65 rgba(177, 17, 22, %d), stop:0.85 rgba(0, 0, 0, 0));\
                                    }";

static const char settingFormat[] = "QPushButton{\
                                    border:0px;\
                                    color:rgb(255,251,240);\
                                    text-align:center;\
                                    background-color: qradialgradient(spread:pad, cx:0.489796, cy:0.494, radius:0.45, fx:0.489796, fy:0.494, stop:0.65 rgba(17, 200, 22, %d), stop:0.85 rgba(0, 0, 0, 0));\
                                    }";

BCMMasterButton::BCMMasterButton(QWidget * parent):
  QPushButton(parent)
{
  this->installEventFilter(this);

  arcTimer = new QTimer(this);
  arcTimer->setInterval(50);
  connect(arcTimer, SIGNAL(timeout()), this, SLOT(arcTimeOut()));
  innerCircleCounter = 0;
  outerCircleCounter = 360*16;
  innerBreathingControl = 0;
  isBreathingUp = true;
  arcTimer->start();
  currentMode = EButtonModeNormal;
}

void BCMMasterButton::arcTimeOut(void)
{
  QString qssFormat;

  innerCircleCounter += 3*16;
  outerCircleCounter -= 2*16;

  if (innerCircleCounter >= 360*16)
    innerCircleCounter = 0;

  if (outerCircleCounter <= 0)
    outerCircleCounter = 360*16;

  if (isBreathingUp == true)
  {
    innerBreathingControl += 5;
    if (innerBreathingControl >= 255)
    {
      innerBreathingControl = 255;
      isBreathingUp = false;
    }
  }
  else
  {
    innerBreathingControl -= 5;
    if (innerBreathingControl <= 100)
    {
      innerBreathingControl = 100;
      isBreathingUp = true;
    }
  }

  switch (currentMode)
  {
    case EButtonModeNormal:
      qssFormat = qssFormat.asprintf(BCMStyleSheetFormat, innerBreathingControl);
    break;
    case EButtonModeExit:
      qssFormat = qssFormat.asprintf(ExitFormat, innerBreathingControl);
    break;
    case EButtonModeSetting:
      qssFormat = qssFormat.asprintf(settingFormat, innerBreathingControl);
    break;
  }

  this->setStyleSheet(qssFormat);

  this->repaint();
}

bool BCMMasterButton::eventFilter(QObject *watched, QEvent *evt)
{
  if ((watched == this) && (evt->type() == QEvent::Paint))
  {
    QPainter painter(this);
    QPen pen;

    switch (currentMode)
    {
      case EButtonModeNormal:
        pen.setColor(QColor(29, 165, 219, 180));
      break;
      case EButtonModeExit:
        pen.setColor(QColor(177, 17, 22, 180));
      break;
      case EButtonModeSetting:
        pen.setColor(QColor(17, 200, 22, 180));
      break;
    }


    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawArc(4,6, this->width()-12, this->height()-12, 0 + outerCircleCounter, 100*16);
    painter.drawArc(4,6, this->width()-12, this->height()-12, 120*16 + outerCircleCounter, 100*16);
    painter.drawArc(4,6, this->width()-12, this->height()-12, 240*16 + outerCircleCounter, 100*16);

    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawArc(12,14, this->width()-28, this->height()-28, 0 + innerCircleCounter, 100*16);
    painter.drawArc(12,14, this->width()-28, this->height()-28, 120*16 + innerCircleCounter, 100*16);
    painter.drawArc(12,14, this->width()-28, this->height()-28, 240*16 + innerCircleCounter, 100*16);
  }

  return QWidget::eventFilter(watched, evt);
}

void BCMMasterButton::setButtonMode(enum buttonMode btnMode)
{
  currentMode = btnMode;
}
