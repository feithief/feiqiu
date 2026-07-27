#ifndef AMBIENTLINSCHEDULER_H
#define AMBIENTLINSCHEDULER_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QMutex>
#include <QList>
#include <QTimer>
#include "ambientlincomm.h"
#include <application_config.h>


enum PlatformType
{
  EPlatformTypeMQB        = 0x00,
  EPlatformTypeMLBevo     = 0x01,
  EPlatformTypeMLBevoVW   = 0x02,
};

enum SlaveMode
{
  ESlaveModeNormal   = 0x0,
  ESlaveModeCrash    = 0x1,
  ESlaveModeShowroom = 0x2,
  ESlaveModeActuator = 0x3,
};

enum SlaveDimmingMode
{
  ESlaveDimmingModeAbsolute = 0x0,
  ESlaveDimmingModeRelative = 0x1,
};

enum SlaveErrorFlag
{
  ESlaveErrorFlagInit     = 0x0,
  ESlaveErrorFlagReserved = 0x1,
  ESlaveErrorFlagNormal   = 0x2,
  ESlaveErrorFlagError    = 0x3,
};
/*
struct BCMSignal
{
  uint16_t address;
  bool usingGroupAdress;
  uint8_t R;
  uint8_t G;
  uint8_t B;
  uint8_t intensity;
  uint8_t dimmingRamp;
  enum SlaveMode mode;
  bool validMarker;
  SlaveDimmingMode dimmingType;
};
*/
struct BCMSignal
{
  uint16_t address;
  //bool usingGroupAdress;
  uint8_t intensity;
  uint8_t R;
  uint8_t G;
  uint8_t B;
  uint8_t dimmingRamp;
//  enum SlaveMode mode;
//  bool validMarker;
//  SlaveDimmingMode dimmingType;
};

struct SlaveStatus
{
  uint8_t slaveNAD;
//  SlaveErrorFlag ROpenStatus;
//  SlaveErrorFlag GOpenStatus;
//  SlaveErrorFlag BOpenStatus;
//  SlaveErrorFlag RShortStatus;
//  SlaveErrorFlag GShortStatus;
//  SlaveErrorFlag BShortStatus;
//  uint8_t tempState;
//  SlaveErrorFlag ramState;
//  SlaveErrorFlag romState;
//  SlaveErrorFlag nvrmState;
  uint8_t SWVerNum;
  uint8_t HWVerNum;
  uint8_t ErrResp;
  bool isOnLine;
};

enum OperationType
{
  EOperationTypeLock       = 0x0,
  EOperationTypeSingleAddr = 0x1,
  EOperationTypeGroupAddr  = 0x2,
  EOperationTypePlatform   = 0x3,
  EOperationTypeIntensity  = 0x4,
  EOperationTypeRValue     = 0x5,
  EOperationTypeGValue     = 0x6,
  EOperationTypeBValue     = 0x7,
  EOperationTypePartNO     = 0x8,
  EOperationTypeSerialNO   = 0x9,
  EOperationTypeHardwareVer= 0xA,
  EOperationTypeSoftwareVer= 0xB,
  EOperationTypeCalibration= 0xC,
  EOperationTypeID         = 0xD,
};


class AmbientLinScheduler : public QThread
{
  Q_OBJECT

public:
  bool least_one_online;//new add
  //no multiple thread used!
  static AmbientLinScheduler* getInstance(QWidget *parent = 0)
  {
    if (mInstance == NULL)
    {
      mInstance = new AmbientLinScheduler(parent);
    }

    return mInstance;
  };


  void LinSchedulerStart();
  void LinSchedulerStop();

  void setBCMSignal(struct BCMSignal *signalSets);
  struct BCMSignal getBCMSignal();

  uint32_t readServiceValue(unsigned char nad,
                             enum OperationType type,
                             uint8_t* buffer,
                             uint32_t desiredLength);

  bool writeServiceValue(uint8_t nad,
                         enum OperationType type,
                         uint8_t* buffer,
                         uint32_t desiredLength);

  void LinSchedulerSleep(void);
  void colorCoordinate(bool color_fix, bool group_fix);

signals:
  void SlaveStatusChanged(struct SlaveStatus status);
  void timerStart(void);

protected:
  void run();

private:
  struct BCMSignal currentMasterSignal;
  struct SlaveStatus slaveCurrentStatus[DSLAVE_NODE_AMOUNT];
  bool exitThread;
  QMutex BCMMutex;
  QMutex diagnosticMutex;
  AmbientLinComm *commDev;
  static AmbientLinScheduler *mInstance;
  QTimer *specialColorCD;
  int colorIndex, currentgroup;
  bool color_fix, group_fix;

  AmbientLinScheduler(QWidget *parent = 0);

private slots:
  void colorGroupChange(void);
  void timershouldstart(void);
};

#endif // AMBIENTLINSCHEDULER_H
