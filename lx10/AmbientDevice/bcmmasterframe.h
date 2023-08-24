#ifndef BCMMASTERFRAME_H
#define BCMMASTERFRAME_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QFrame>
#include <QSignalMapper>
#include "ui_masterframe.h"
#include "ambientlinscheduler.h"
#include "keyboard.h"
#include <QScroller>

class BCMMasterFrame : public QWidget
{
  Q_OBJECT
public:
  explicit BCMMasterFrame(QWidget *parent = 0);
  void init();
  bool product_check_is_on;//new add
  bool TenColor_Mode_is_on;//new add
  QTimer *change_color_Timer;//new add

private:
  int index;
  Ui::MasterFrame *ui;
  double colorX, colorY;
  QFrame *backgroundframe;
  AmbientLinScheduler *linScheduler;
  struct BCMSignal masterSignals, oldSignals;
  QSignalMapper *colorMapper;
  KeyBoard *keys;
  void updateSignals();
  bool eventFilter(QObject *watched, QEvent *event);
  void showInputKeyBoard(bool show);
  bool updateSianglImmediate;

  bool groupPaused, colorPaused;

  QScroller *scroller;

private slots:
  void changeColor(int i);
  void drawCIE();
  void change_RGBW_color(void);//new add
#if 0
  void setGroup();
  void setSingle();

  void setDimmingAbsolute();
  void setDimmingRelative();
  void setValid();
  void setInvalid();
  void setRunModeNormal();
  void setRunModeCrash();
  void setRunModeShowroom();
  void setRunModeActuator();
 #endif
  void syncModeChanged();
  void CheckModeChanged();
  void TenColorModeChanged();
  void changeAddr(int value);
  void changeR(int r);
  void changeG(int g);
  void changeB(int b);
  void changeI(int i);
  void changeDimRamp(int dimramp);
  void Accept();
  void Cancel();

  //void groupPauseClicked();
  //void colorPauseClicked();
  void on_pushButtonCancel_2_clicked();
  void on_pushButtonCancel_3_clicked();
};

#endif // BCMMASTERFRAME_H
