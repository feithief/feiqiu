#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "lin_types.h"

class AmbientLinScheduler;
class BCMMasterButton;
class BCMMasterFrame;
class QCloseEvent;
class DebugPanel;
class DebugStore;
class ProductionVerify;
class QPushButton;
class SlaveButton;
class SlaveFrameConfig;
class unlockFrom;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(DebugStore *debugStore, QWidget *parent = 0);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void unlockFeature();
  void slaveConfig(int nodeAddress);
  void enterMasterConfigMode();
  void slaveStatusHandle(SlaveStatus status);

private:
  Ui::MainWindow *ui;
  DebugStore *debug;
  AmbientLinScheduler *linScheduler;

  BCMMasterButton *masterButton;
  BCMMasterButton *exitButton;
  BCMMasterButton *settingButton;
  QPushButton *debugQuickButton;
  QMap<quint8, SlaveButton *> slaveButtons;

  SlaveFrameConfig *slaveFrame;
  ProductionVerify *productVerifyFrame;
  BCMMasterFrame *masterFrame;
  unlockFrom *lockFrame;
  DebugPanel *debugPanel;

  void keepDebugAccessVisible();
};

#endif // MAINWINDOW_H
