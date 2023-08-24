#ifndef SLAVEFRAMECONFIG_H
#define SLAVEFRAMECONFIG_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QThread>
#include "ui_slavenodeframe.h"
#include "ambientlinscheduler.h"
#include "adialog.h"
#include "keyboard.h"


struct CalibrationInfo
{
  double x,y,Y;
};

enum ENodeType
{
  ENodeTypeRGB = 0x0,
  ENodeTypeWhite = 0x1,
};

struct slaveConfigInfo
{
  int slaveNode;
  int SA;
  int GA;//int GA;
  int platform;
  int intensity;
  QString PN;
  QString SN;
  QString HardwareVer;
  struct CalibrationInfo r,g,b;
  enum ENodeType nodeType;
};

class SlaveWriteThread : public QThread
{
  Q_OBJECT

public:
  explicit SlaveWriteThread(QWidget *parent = 0);
  void threadWriteValue(struct slaveConfigInfo *info);

signals:
  void WrittingFinished(bool);

protected:
  void run();

private:
  struct slaveConfigInfo writtingInfo;
  AmbientLinScheduler *linScheduler;
};

class SlaveInitThread : public QThread
{
  Q_OBJECT

public:
  explicit SlaveInitThread(QWidget *parent = 0);
  void threadSetSlave(int slaveNode);
  void stopFetching();

signals:
  void SWFetched(QString);
  void HWFetched(QString);
  void SuppFetched(QString);
  void FuncFetched(QString);
  void VariantFetched(QString);
  void SAFetched(int);
  void GAFetched(int);
  void PlatformFetched(int);
  void IntensityFetched(int);
  void LockFetched(bool);
  void RFetched(struct CalibrationInfo);
  void GFetched(struct CalibrationInfo);
  void BFetched(struct CalibrationInfo);
  void PNFetched(QString);
  void SerialFetched(QString);
  void fechingFinished(bool, enum ENodeType);

protected:
  void run();

private:
  AmbientLinScheduler *linScheduler;
  int currentNode;
  bool isExit;
  enum ENodeType nodeType;
};

class SlaveFrameConfig : public QWidget
{
  Q_OBJECT
public:
  explicit SlaveFrameConfig(QWidget *parent = NULL);
  void SlaveFrameConfigInit(int slaveNode);

private slots:
  void buttonCalibrateNormal();
  void buttonCalibrateR();
  void buttonCalibrateG();
  void buttonCalibrateB();
  void updateNodeState(struct SlaveStatus status);
  void exitSlaveConfig();
  void getLockStatus(bool);
  void getRValue(struct CalibrationInfo);
  void getGValue(struct CalibrationInfo);
  void getBValue(struct CalibrationInfo);
  void changeConfigs();
  void processReadDone(bool, enum ENodeType);
  void processWriteDone(bool);



private:
  int currentNode;
  enum ENodeType nodeType;
  Ui::SlaveNodeFrame *ui;
  ADialog *dialog;
  QFrame *backgroundframe;
  SlaveInitThread *initThread;
  SlaveWriteThread *writeThread;
  AmbientLinScheduler *linScheduler;
  KeyBoard *keys;
  bool eventFilter(QObject *watched, QEvent *event);
  void showInputKeyBoard(bool show);
};

#endif // SLAVEFRAMECONFIG_H
