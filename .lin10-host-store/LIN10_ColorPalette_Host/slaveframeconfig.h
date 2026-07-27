#ifndef SLAVEFRAMECONFIG_H
#define SLAVEFRAMECONFIG_H

#include <QWidget>

#include "lin_types.h"

class ADialog;
class AmbientLinScheduler;
class KeyBoard;
class QFrame;

namespace Ui {
  class SlaveNodeFrame;
}

class SlaveFrameConfig : public QWidget
{
  Q_OBJECT

public:
  explicit SlaveFrameConfig(AmbientLinScheduler *scheduler,
                            QWidget *parent = 0);
  ~SlaveFrameConfig();

  void SlaveFrameConfigInit(int slaveNode);

private slots:
  void buttonCalibrateNormal();
  void buttonCalibrateR();
  void buttonCalibrateG();
  void buttonCalibrateB();
  void updateNodeState(SlaveStatus status);
  void exitSlaveConfig();
  void changeConfigs();
  void singleAddressChanged(int value);

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

  void on_SleepButton_clicked();
  void on_AwakeButton_clicked();

private:
  AmbientLinScheduler *linScheduler;
  int currentNode;
  ENodeType nodeType;
  Ui::SlaveNodeFrame *ui;
  ADialog *dialog;
  QFrame *backgroundframe;
  KeyBoard *keys;

  quint32 readRequestId;
  quint32 writeRequestId;
  quint32 calibrationRequestId;

  bool eventFilter(QObject *watched, QEvent *event);
  void showInputKeyBoard(bool show);
  void resetForm();
  void displayConfiguration(const SlaveConfigInfo &info);
  void showReadWriteOk();
  void requestCalibration(quint8 mode);
};

#endif // SLAVEFRAMECONFIG_H
