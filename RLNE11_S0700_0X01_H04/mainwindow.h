#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "lin_types.h"

class LinRuntime;
class BCMMasterButton;
class BCMMasterFrame;
class QCloseEvent;
class DebugPanel;
class DebugSink;
class DebugSnapshotSource;
class ProductionVerify;
class QPushButton;
class SlaveButton;
class SlaveFrameConfig;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(LinRuntime *runtime,
                      DebugSink *debugSink,
                      DebugSnapshotSource *debugSource,
                      QWidget *parent = 0);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void slaveConfig(int nodeAddress);
  void showSlaveConfig(int nodeAddress);
  void enterMasterConfigMode();
  void slaveStatusHandle(SlaveStatus status);

private:
  Ui::MainWindow *ui;
  DebugSink *debug;
  LinRuntime *linRuntime;

  BCMMasterButton *masterButton;
  BCMMasterButton *exitButton;
  QPushButton *debugQuickButton;
  QMap<quint8, SlaveButton *> slaveButtons;

  SlaveFrameConfig *slaveFrame;
  ProductionVerify *productVerifyFrame;
  BCMMasterFrame *masterFrame;
  DebugPanel *debugPanel;

  void keepDebugAccessVisible();
};

#endif // MAINWINDOW_H
