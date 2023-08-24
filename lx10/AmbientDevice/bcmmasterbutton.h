#ifndef BCMMASTERBUTTON_H
#define BCMMASTERBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QTimer>

enum buttonMode
{
  EButtonModeNormal = 0x0,
  EButtonModeExit = 0x1,
  EButtonModeSetting = 0x2,
};

class BCMMasterButton : public QPushButton
{
  Q_OBJECT

public:
  BCMMasterButton(QWidget *parent = 0);
  void setButtonMode(enum buttonMode btnMode);

private:
  int innerCircleCounter, outerCircleCounter;
  int innerBreathingControl;
  bool isBreathingUp;
  enum buttonMode currentMode;
  QTimer *arcTimer;
  bool eventFilter(QObject *watched, QEvent *evt);

private slots:
  void arcTimeOut(void);
};

#endif // BCMMASTERBUTTON_H
