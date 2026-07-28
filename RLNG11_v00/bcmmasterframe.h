#ifndef BCMMASTERFRAME_H
#define BCMMASTERFRAME_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QFrame>
#include <QSignalMapper>
#include <QList>
#include "ui_masterframe.h"
#include "ambientlinscheduler.h"
#include "keyboard.h"

class BCMMasterFrame : public QWidget
{
  Q_OBJECT
public:
  explicit BCMMasterFrame(AmbientLinScheduler *scheduler,
                          QWidget *parent = 0);
  ~BCMMasterFrame();
  void init();

private:
  Ui::MasterFrame *ui;
  double colorX, colorY;
  QFrame *backgroundframe;
  AmbientLinScheduler *linScheduler;
  BCMSignal masterSignals;
  bool groupTargetMode;
  bool signalPresetMode;
  QSignalMapper *colorMapper;
  QList<QPushButton *> colorButtons;
  int colorPage;
  int colorPageSize;
  KeyBoard *keys;
  void updateSignals();
  void setDirectRgbControlsEnabled(bool enabled);
  void setDirectRgbControlsVisible(bool visible);
  void setPredefinedColorControlsVisible(bool visible);
  void updateColorPage();
  bool eventFilter(QObject *watched, QEvent *event);
  void showInputKeyBoard(bool show);
private slots:
  void changeColor(int i);
  void showPreviousColorPage();
  void showNextColorPage();
  void drawCIE();
  void setGroup();
  void setSingle();
  void changeAddr(int value);
  void changeR(int r);
  void changeG(int g);
  void changeB(int b);
  void changeI(int i);
  void changeDimRamp(int dimramp);
  void Accept();
  void Cancel();
  void on_pushButtonSleep_clicked();
  void on_pushButtonAwake_clicked();
  void on_LinStart_clicked();
  void on_LinStop_clicked();
  void on_PredefColor_clicked();
  void on_RGBColor_clicked();
  void on_LED_Enable_clicked();
  void on_LED_Disable_clicked();
  void on_Fading_Enable_clicked();
  void on_Fading_Disable_clicked();
};

#endif // BCMMASTERFRAME_H
