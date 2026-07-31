#ifndef SLAVEFRAMECONFIG_H
#define SLAVEFRAMECONFIG_H

#include <QWidget>

#include "lin_types.h"

class ADialog;
class LinRuntime;
class KeyBoard;
class QFrame;
class QTimer;
struct LinNodeLayout;

namespace Ui {
  class SlaveNodeFrame;
}

class SlaveFrameConfig : public QWidget
{
  Q_OBJECT

public:
  explicit SlaveFrameConfig(LinRuntime *runtime,
                            QWidget *parent = 0);
  ~SlaveFrameConfig();

  void SlaveFrameConfigInit(int slaveNode);

signals:
  void configurationReady(int nodeAddress);

private slots:
  void buttonCalibrateNormal();
  void buttonCalibrateR();
  void buttonCalibrateG();
  void buttonCalibrateB();
  void updateNodeState(SlaveStatus status);
  void exitSlaveConfig();
  void changeConfigs();
  void singleAddressChanged(int value);
  void requestLock();
  void requestUnlock();
  void handleNodeResponse(quint8 node);
  void handleFeedbackTimeout();
  void retryConfigurationRead();

  void handleReadResult(quint32 requestId,
                        SlaveConfigInfo info,
                        bool success,
                        QString errorMessage);
  void handleWriteResult(quint32 requestId,
                         quint8 node,
                         bool success,
                         QString errorMessage);
  void handleCalibrationResult(quint32 requestId,
                               quint8 node,
                               bool success,
                               QString errorMessage);
  void handleLockStateResult(quint32 requestId,
                             quint8 node,
                             bool locked,
                             bool success,
                             QString errorMessage);

  void on_SleepButton_clicked();
  void on_AwakeButton_clicked();

private:
  LinRuntime *linRuntime;
  int currentNode;
  ENodeType nodeType;
  bool configurationAvailable;
  Ui::SlaveNodeFrame *ui;
  ADialog *dialog;
  QFrame *backgroundframe;
  KeyBoard *keys;

  quint32 readRequestId;
  quint32 writeRequestId;
  quint32 calibrationRequestId;
  QTimer *feedbackWatchdog;
  QTimer *configurationRetryTimer;
  quint32 lockRequestId;
  quint32 unlockRequestId;
  bool statusUsesRawFrame;

  bool eventFilter(QObject *watched, QEvent *event);
  void showInputKeyBoard(bool show);
  void resetForm();
  void setConfigurationControlsEnabled(bool enabled);
  void configureStatusRows(const LinNodeLayout &node);
  void setLockButtonsBusy(bool busy);
  void showLockState(bool locked);
  void displayConfiguration(const SlaveConfigInfo &info);
  void showConfigurationLoading();
  void showReadWriteOk();
  void requestCalibration(quint8 mode);
};

#endif // SLAVEFRAMECONFIG_H
