#include "ambientlinscheduler.h"

AmbientLinScheduler::AmbientLinScheduler(QWidget *parent):
                  QThread(parent)
{
  exitThread = false;
  this->setPriority(QThread::TimeCriticalPriority);
  least_one_online = false;//new add
  currentMasterSignal.address = 65535;
  //currentMasterSignal.usingGroupAdress = true;
  currentMasterSignal.R = 0xFF;
  currentMasterSignal.G = 0xFF;
  currentMasterSignal.B = 0xFF;
  currentMasterSignal.intensity = 200;
  currentMasterSignal.dimmingRamp = 0;
//  currentMasterSignal.validMarker = false;
//  currentMasterSignal.mode = ESlaveModeNormal;
//  currentMasterSignal.dimmingType = ESlaveDimmingModeAbsolute;

  commDev = AmbientLinComm::getInstance(this);

  specialColorCD = new QTimer(this);
  specialColorCD->setInterval(1250);
  specialColorCD->setSingleShot(true);
  connect(specialColorCD, SIGNAL(timeout()), this, SLOT(colorGroupChange()));

  connect(this, SIGNAL(timerStart()), this, SLOT(timershouldstart()));

  colorIndex = 0;

  qRegisterMetaType<struct SlaveStatus>("struct SlaveStatus");
}

void AmbientLinScheduler::LinSchedulerStart()
{
  this->start(QThread::TimeCriticalPriority);
}

void AmbientLinScheduler::LinSchedulerStop()
{
  exitThread = true;
}

void AmbientLinScheduler::setBCMSignal(struct BCMSignal *signalSets)
{
  BCMMutex.lock();
  ::memcpy(&this->currentMasterSignal, signalSets, sizeof(struct BCMSignal));
  BCMMutex.unlock();
}

struct BCMSignal AmbientLinScheduler::getBCMSignal()
{
  struct BCMSignal ret;

  BCMMutex.lock();
  ::memcpy(&ret, &this->currentMasterSignal, sizeof(struct BCMSignal));
  BCMMutex.unlock();

  return ret;
}

static void convertConfigToBuffer(uint8_t* buffer, struct BCMSignal *signal)
{
  memset(buffer ,0x0, 8);

  buffer[0] = (uint8_t)(signal->address & 0xff);
  buffer[1] = (uint8_t)((signal->address>>8) & 0xff);
//  buffer[1] |= (uint8_t)((signal->usingGroupAdress == true)?0x80:0x0);
  buffer[3] = signal->R;
  buffer[4] = signal->G;
  buffer[5] = signal->B;
  buffer[2] = (signal->intensity >= 200)?200:signal->intensity;
  //buffer[6] = ((signal->dimmingRamp >= 0xf)?0xf:signal->dimmingRamp)&0x0f;
  buffer[6] = ((signal->dimmingRamp >= 0xff)?0xff:signal->dimmingRamp)&0xff;
//  buffer[6] |= (uint8_t)(((signal->mode&0x03)<<5)&0x70);
//  buffer[7] = (uint8_t)signal->dimmingType;
//  buffer[7] |= (signal->validMarker == true)?0x2:0x0;
}

static void convertBufferToStatus(struct SlaveStatus *status, uint8_t* buffer)
{
//  status->RShortStatus = (enum SlaveErrorFlag)(buffer[3]&0x3);
//  status->GShortStatus = (enum SlaveErrorFlag)((buffer[3]>>2)&0x3);
//  status->BShortStatus = (enum SlaveErrorFlag)((buffer[3]>>4)&0x3);
//  status->ROpenStatus = (enum SlaveErrorFlag)((buffer[3]>>6)&0x3);

//  status->GOpenStatus = (enum SlaveErrorFlag)(buffer[4]&0x3);
//  status->BOpenStatus = (enum SlaveErrorFlag)((buffer[4]>>2)&0x3);
//  status->tempState = (buffer[4]>>4)&0x7;

//  status->ramState = (enum SlaveErrorFlag)(buffer[5]&0x3);
//  status->romState = (enum SlaveErrorFlag)((buffer[5]>>2)&0x3);
//  status->nvrmState = (enum SlaveErrorFlag)((buffer[5]>>4)&0x3);
    status->ErrResp = buffer[0] & 0x01;
    status->HWVerNum = buffer[1];
    status->SWVerNum = buffer[2];
}

struct ServiceAttribute
{
  uint16_t serviceID;
  bool needChangeA0;
  bool isWriteable;
  uint32_t serviceLength;
};

static const struct ServiceAttribute services[13] =
{
  {0x0002, true, true, 2},//EOperationTypeLock
  {0x0003, true, true, 2},//EOperationTypeSingleAddr
  {0x0004, true, true, 2},//EOperationTypeGroupAddr
  {0x0005, true, true, 2},//EOperationTypePlatform
  {0x0006, true, true, 2},//EOperationTypeIntensity
  {0x0007, true, true, 8},//EOperationTypeRValue
  {0x0008, true, true, 8},//EOperationTypeGValue
  {0x0009, true, true, 8},//EOperationTypeBValue
  {0x6269, true, true, 12},//EOperationTypePartNO
  {0x6A69, true, true, 20},//EOperationTypeSerialNO
  {0x6869, true, true, 6},//EOperationTypeHardwareVer
  {0x6469, true, false, 6},//EOperationTypeSoftwareVer
  {0x000A, true, true, 1},//EOperationTypeCalibration

};

typedef enum
{
  EcolorTypeInit = 0x0,
  EColorTypeTarget = 0x1,
} EcolorType;

struct ColorGroupFVW
{
  uint16_t nad;
  EcolorType type;
  uint8_t R;
  uint8_t G;
  uint8_t B;
  bool inDimming;
} SColorGroupFVW;

#define DMAX_COLOR (6*15)
static volatile int  currentColor = 0;
static const struct ColorGroupFVW colorSpecial[DMAX_COLOR] =
{
  /*Initial group 1-1*/
  {2, EcolorTypeInit  , 255,  3,  1, false},
  {3, EcolorTypeInit  , 255,  8, 42, false},
  {4, EcolorTypeInit  , 188, 30,255, false},

  /*Target group 1-2*/
  {2, EcolorTypeInit  , 255,  8, 42, true},
  {3, EcolorTypeInit  , 188, 30,255, true},
  {4, EColorTypeTarget, 255,  8, 42, true},

  /*Target group 1-3*/
  {2, EcolorTypeInit  , 188, 30,255, true},
  {3, EcolorTypeInit  , 255,  8, 42, true},
  {4, EColorTypeTarget, 255,  3,  1, true},

  /*Target group 1-4*/
  {2, EcolorTypeInit  , 255,  8, 42, true},
  {3, EcolorTypeInit  , 255,  3,  1, true},
  {4, EColorTypeTarget, 255,  8, 42, true},

  /*Target group 1-5*/
  {2, EcolorTypeInit  , 255,  3,  1, true},
  {3, EcolorTypeInit  , 255,  8, 42, true},
  {4, EColorTypeTarget, 188, 30,255, true},

  /*Initial group 2-1*/
  {2, EcolorTypeInit  ,  19,255, 60, false},
  {3, EcolorTypeInit  ,   1,255,125, false},
  {4, EcolorTypeInit  ,   1, 20,255, false},

  /*Target group 2-2*/
  {2, EcolorTypeInit  ,   1,255,125, true},
  {3, EcolorTypeInit  ,   1, 20,255, true},
  {4, EColorTypeTarget,   1,255,125, true},

  /*Target group 2-3*/
  {2, EcolorTypeInit  ,   1, 20,255, true},
  {3, EcolorTypeInit  ,   1,255,125, true},
  {4, EColorTypeTarget,  19,255, 60, true},

  /*Target group 2-4*/
  {2, EcolorTypeInit  ,   1,255,125, true},
  {3, EcolorTypeInit  ,  19,255, 60, true},
  {4, EColorTypeTarget,   1,255,125, true},

  /*Target group 2-5*/
  {2, EcolorTypeInit  ,  19,255, 60, true},
  {3, EcolorTypeInit  ,   1,255,125, true},
  {4, EColorTypeTarget,   1, 20,255, true},

  /*Initial group 3-1*/
  {2, EcolorTypeInit  ,  19,255, 60, false},
  {3, EcolorTypeInit  ,  78,255,  3, false},
  {4, EcolorTypeInit  , 114,255,  3, false},

  /*Target group 3-2*/
  {2, EcolorTypeInit  ,  78,255,  3, true},
  {3, EcolorTypeInit  , 114,255,  3, true},
  {4, EColorTypeTarget,  78,255,  3, true},

  /*Target group 3-3*/
  {2, EcolorTypeInit  , 114,255,  3, true},
  {3, EcolorTypeInit  ,  78,255,  3, true},
  {4, EColorTypeTarget,  19,255, 60, true},

  /*Target group 3-4*/
  {2, EcolorTypeInit  ,  78,255,  3, true},
  {3, EcolorTypeInit  ,  19,255, 60, true},
  {4, EColorTypeTarget,  78,255,  3, true},

  /*Target group 3-5*/
  {2, EcolorTypeInit  ,  19,255, 60, true},
  {3, EcolorTypeInit  ,  78,255,  3, true},
  {4, EColorTypeTarget, 114,255,  3, true},

  /*Initial group 4-1*/
  {2, EcolorTypeInit  , 142, 60,255, false},
  {3, EcolorTypeInit  ,   1,255,125, false},
  {4, EcolorTypeInit  , 175,255, 64, false},

  /*Target group 4-2*/
  {2, EcolorTypeInit  ,   1,255,125, true},
  {3, EcolorTypeInit  , 175,255, 64, true},
  {4, EColorTypeTarget,   1,255,125, true},

  /*Target group 4-3*/
  {2, EcolorTypeInit  , 175,255, 64, true},
  {3, EcolorTypeInit  ,   1,255,125, true},
  {4, EColorTypeTarget, 142, 60,255, true},

  /*Target group 4-4*/
  {2, EcolorTypeInit  ,   1,255,125, true},
  {3, EcolorTypeInit  , 142, 60,255, true},
  {4, EColorTypeTarget,   1,255,125, true},

  /*Target group 4-5*/
  {2, EcolorTypeInit  , 142, 60,255, true},
  {3, EcolorTypeInit  ,   1,255,125, true},
  {4, EColorTypeTarget, 175,255, 64, true},

  /*Initial group 5-1*/
  {2, EcolorTypeInit  , 255, 79, 19, false},
  {3, EcolorTypeInit  ,   1,255,125, false},
  {4, EcolorTypeInit  , 175,255, 64, false},

  /*Target group 5-2*/
  {2, EcolorTypeInit  ,   1,255,125, true},
  {3, EcolorTypeInit  , 175,255, 64, true},
  {4, EColorTypeTarget,   1,255,125, true},

  /*Target group 5-3*/
  {2, EcolorTypeInit  , 175,255, 64, true},
  {3, EcolorTypeInit  ,   1,255,125, true},
  {4, EColorTypeTarget, 255, 79, 19, true},

  /*Target group 5-4*/
  {2, EcolorTypeInit  ,   1,255,125, true},
  {3, EcolorTypeInit  , 255, 79, 19, true},
  {4, EColorTypeTarget,   1,255,125, true},

  /*Target group 5-5*/
  {2, EcolorTypeInit  , 255, 79, 19, true},
  {3, EcolorTypeInit  ,   1,255,125, true},
  {4, EColorTypeTarget, 175,255, 64, true},

  /*Initial group 6-1*/
  {2, EcolorTypeInit  , 255,  8, 42, false},
  {3, EcolorTypeInit  ,  44, 25,255, false},
  {4, EcolorTypeInit  ,   1,255,125, false},

  /*Target group 6-2*/
  {2, EcolorTypeInit  ,  44, 25,255, true},
  {3, EcolorTypeInit  ,   1,255,125, true},
  {4, EColorTypeTarget,  44, 25,255, true},

  /*Target group 6-3*/
  {2, EcolorTypeInit  ,  1,255,125, true},
  {3, EcolorTypeInit  ,  44, 25,255, true},
  {4, EColorTypeTarget, 255,  8, 42, true},

  /*Target group 6-4*/
  {2, EcolorTypeInit  ,  44, 25,255, true},
  {3, EcolorTypeInit  , 255,  8, 42, true},
  {4, EColorTypeTarget,  44, 25,255, true},

  /*Target group 6-5*/
  {2, EcolorTypeInit  , 255,  8, 42, true},
  {3, EcolorTypeInit  ,  44, 25,255, true},
  {4, EColorTypeTarget,   1,255,125, true},
};

void AmbientLinScheduler::colorCoordinate(bool color_fix, bool group_fix)
{
  BCMMutex.lock();
  this->color_fix = color_fix;
  this->group_fix = group_fix;
  if (group_fix == true)
    currentgroup = currentColor/9;
  BCMMutex.unlock();
}

void AmbientLinScheduler::colorGroupChange(void)
{
  colorIndex=2;
}

void AmbientLinScheduler::timershouldstart(void)
{
  specialColorCD->start();
}


void AmbientLinScheduler::run()
{
  uint8_t BCMBuffer[8];
  uint8_t recvBuffer[8];
  uint32_t slaveIndex = 0;
  uint8_t ret = 0;

  struct SlaveStatus currentNodeStatus;
  struct BCMSignal specialMaster;

  while(exitThread == false)
  {
    uint8_t online_count = DSLAVE_NODE_AMOUNT-6;//new add
    for (slaveIndex = 6; slaveIndex < DSLAVE_NODE_AMOUNT; slaveIndex++)//slaveIndex = 0
    {
      if (exitThread == true)
        break;

      diagnosticMutex.lock();
#if 0
      if (this->currentMasterSignal.mode == ESlaveModeShowroom)
      {
        if ((colorIndex == 2) && (this->color_fix == false))
        {
          currentColor++;
          if (this->group_fix == false)
          {
            currentColor = currentColor%DMAX_COLOR;
          }
          else
          {
            currentColor = currentgroup*9 + currentColor%9;
          }
          colorIndex = 0;
        }

        if (colorSpecial[currentColor].type == EcolorTypeInit)
        {
          specialMaster.address = colorSpecial[currentColor].nad;
          specialMaster.usingGroupAdress = false;
          specialMaster.R = colorSpecial[currentColor].R;
          specialMaster.G = colorSpecial[currentColor].G;
          specialMaster.B = colorSpecial[currentColor].B;
          specialMaster.intensity = 100;
          if ((colorSpecial[currentColor].inDimming == true) &&
              (this->color_fix == false))
            specialMaster.dimmingRamp = 5;
          else
            specialMaster.dimmingRamp = 0;
          specialMaster.validMarker = true;
          specialMaster.mode = ESlaveModeNormal;
          specialMaster.dimmingType = ESlaveDimmingModeAbsolute;

          BCMMutex.lock();
          ::convertConfigToBuffer(BCMBuffer, &specialMaster);
          BCMMutex.unlock();

          if (this->color_fix == false)
          {
            currentColor++;
            if (this->group_fix == false)
            {
              currentColor = currentColor%DMAX_COLOR;
            }
            else
            {
              currentColor = currentgroup*9 + currentColor%9;
            }
            colorIndex = 0;
          }
          commDev->transferDataToLin(0x10, BCMBuffer, 8);
          //this->msleep(10);
        }
        else
        {
          specialMaster.address = colorSpecial[currentColor].nad;
          specialMaster.usingGroupAdress = false;
          specialMaster.R = colorSpecial[currentColor].R;
          specialMaster.G = colorSpecial[currentColor].G;
          specialMaster.B = colorSpecial[currentColor].B;
          specialMaster.intensity = 100;
          if ((colorSpecial[currentColor].inDimming == true) &&
              (this->color_fix == false))
            specialMaster.dimmingRamp = 5;
          else
            specialMaster.dimmingRamp = 0;
          specialMaster.validMarker = true;
          specialMaster.mode = ESlaveModeNormal;
          specialMaster.dimmingType = ESlaveDimmingModeAbsolute;

          if ((colorIndex == 0) && (this->color_fix == false))
          {
            colorIndex++;
            emit timerStart();
          }

          BCMMutex.lock();
          ::convertConfigToBuffer(BCMBuffer, &specialMaster);
          BCMMutex.unlock();

          commDev->transferDataToLin(0x10, BCMBuffer, 8);
          //this->msleep(10);
        }
      }
#endif
      if(1)
      {
        colorIndex = 0;
        currentColor = 0;

        if (specialColorCD->isActive() == true)
        {
          specialColorCD->stop();
        }

        BCMMutex.lock();
        ::convertConfigToBuffer(BCMBuffer, &this->currentMasterSignal);
        BCMMutex.unlock();

        commDev->transferDataToLin(0x10, BCMBuffer, 8);
        this->msleep(10);
        commDev->transferHeaderToLin(slaveIndex+1);
        this->msleep(2);

        ret = commDev->retrieveDataFromLin(recvBuffer, 8);
        ::memset(&currentNodeStatus, 0x0, sizeof(struct SlaveStatus));
        currentNodeStatus.slaveNAD = slaveIndex+1;
        if (ret == 8)
        {
          ::convertBufferToStatus(&currentNodeStatus, recvBuffer);
          currentNodeStatus.isOnLine = true;
          online_count++;//new add
        }
        else
        {
          currentNodeStatus.isOnLine = false;
          online_count--;
        }

        emit SlaveStatusChanged(currentNodeStatus);
      }

      diagnosticMutex.unlock();

      this->msleep(2);
    }

    if(online_count != 0)//new add
    {
        least_one_online = true;
    }
    else
    {
        least_one_online = false;
    }
  }

  this->exit(0);
}

static void initialBufferAssignNAD(uint8_t nad, uint8_t AssignNAD, uint8_t *buffer)
{
  buffer[0] = nad;
  buffer[1] = 0x6;
  buffer[2] = 0xB0;
  buffer[3] = 0xFF;
  buffer[4] = 0x7F;
  buffer[5] = 0xFF;
  buffer[6] = 0xFF;
  buffer[7] = AssignNAD;
}

static void initialBufferReadSID(uint8_t nad, uint16_t SID, uint8_t *buffer)
{
  buffer[0] = nad;
  buffer[1] = 0x3;
  buffer[2] = 0x22;
  buffer[3] = (uint8_t)((SID >> 8) & 0xFF);
  buffer[4] = (uint8_t)(SID & 0xFF);
  buffer[5] = 0;
  buffer[6] = 0;
  buffer[7] = 0;
}

uint32_t AmbientLinScheduler::readServiceValue(uint8_t nad,
                                                enum OperationType type,
                                                uint8_t* buffer,
                                                uint32_t desiredLength)
{
  uint8_t sendBuffer[8], recvBuffer[8];
  uint8_t requestNAD;
  uint8_t receivedCount = 0;
  //uint8_t resultBuffer[20];

  ::memset(buffer, 0, desiredLength);

  diagnosticMutex.lock();

  if (type < EOperationTypeID)
  {
    desiredLength = services[type].serviceLength;
    if (services[type].serviceID == EOperationTypeCalibration)
      goto failed;

    //Change NAD
    if (services[type].needChangeA0 == true)
    {
      requestNAD = 0xA0;
      initialBufferAssignNAD(nad , 0xA0, sendBuffer);
    }
    else
    {
      requestNAD = nad;
      initialBufferAssignNAD(nad , nad, sendBuffer);
    }
    commDev->transferDataToLin(0x3C, sendBuffer, 8);
    this->msleep(10);

    //Send read request;
    initialBufferReadSID(requestNAD, services[type].serviceID, sendBuffer);
    commDev->transferDataToLin(0x3C, sendBuffer, 8);
    this->msleep(10);

    //receiveData
    receivedCount = 0;
    do
    {
      ::memset(recvBuffer, 0, 8);
      commDev->transferHeaderToLin(0x3D);
      this->msleep(10);
      if (commDev->retrieveDataFromLin(recvBuffer, 8) == 8)
      {
        uint8_t PCI;
        PCI = recvBuffer[1] & 0xf0;

        if (PCI == 0x0)
        {
          receivedCount = 3;
          ::memcpy(buffer, &recvBuffer[5], receivedCount);
          break;
        }

        if (PCI == 0x10)
        {
          ::memcpy(buffer, &recvBuffer[6], 2);
          receivedCount += 2;
        }

        if (PCI == 0x20)
        {
          if ((desiredLength - receivedCount) >6)
          {
            ::memcpy(buffer + receivedCount, &recvBuffer[2], 6);
            receivedCount += 6;
          }
          else
          {
            ::memcpy(buffer + receivedCount, &recvBuffer[2], desiredLength - receivedCount);
            break;
          }
        }
      }
      else
        goto failed;

      this->msleep(10);
    }while(receivedCount < desiredLength);
  }
  else
  {
    if (desiredLength < 5)
      return 0;

    desiredLength = 5;

    initialBufferAssignNAD(nad , nad, sendBuffer);

    commDev->transferDataToLin(0x3C, sendBuffer, 8);
    this->msleep(10);

    sendBuffer[0] = nad;
    sendBuffer[1] = 0x6;
    sendBuffer[2] = 0xB2;
    sendBuffer[3] = 0x0;
    sendBuffer[4] = 0xFF;
    sendBuffer[5] = 0x7F;
    sendBuffer[6] = 0xFF;
    sendBuffer[7] = 0xFF;

    commDev->transferDataToLin(0x3C, sendBuffer, 8);
    this->msleep(10);

    commDev->transferHeaderToLin(0x3D);
    this->msleep(10);

    ::memset(recvBuffer, 0x0, 8);
    if (commDev->retrieveDataFromLin(recvBuffer, 8) == 8)
    {
      ::memcpy(buffer, &recvBuffer[3], 5);
      receivedCount =5;
    }
    else
      goto failed;
  }

  initialBufferAssignNAD(nad , nad, sendBuffer);
  commDev->transferDataToLin(0x3C, sendBuffer, 8);
  this->msleep(10);

  diagnosticMutex.unlock();
  return receivedCount;

failed:
  initialBufferAssignNAD(nad , nad, sendBuffer);
  commDev->transferDataToLin(0x3C, sendBuffer, 8);
  this->msleep(10);

  diagnosticMutex.unlock();
  return 0;
}

bool AmbientLinScheduler::writeServiceValue(uint8_t nad,
                                            enum OperationType type,
                                            uint8_t* buffer,
                                            uint32_t desiredLength)
{
  bool mutiplueFrames = false;
  uint8_t writeIndex = 0;
  uint8_t sendBuffer[8];

  if (services[type].serviceID == EOperationTypeID)
    return false;

  if (services[type].serviceLength > 3)
    mutiplueFrames = true;

  if (desiredLength < services[type].serviceLength)
    return false;

  desiredLength = services[type].serviceLength;

  diagnosticMutex.lock();

  initialBufferAssignNAD(nad , 0xA0, sendBuffer);
  commDev->transferDataToLin(0x3C, sendBuffer, 8);
  this->msleep(10);

  do
  {
    ::memset(sendBuffer, 0xFF, 8);
    sendBuffer[0] = 0xA0;

    if (mutiplueFrames == false)
    {
      writeIndex = desiredLength;

      sendBuffer[1] = desiredLength + 3;
      sendBuffer[2] = 0x2E;
      sendBuffer[3] = (uint8_t)((services[type].serviceID >> 8)&0xff);
      sendBuffer[4] = (uint8_t)(services[type].serviceID&0xff);
      ::memcpy(&sendBuffer[5], buffer, desiredLength);

      commDev->transferDataToLin(0x3C, sendBuffer, 8);
    }
    else
    {
      if (writeIndex == 0)
      {
        sendBuffer[1] = 0x10;
        sendBuffer[2] = desiredLength + 3;
        sendBuffer[3] = 0x2E;
        sendBuffer[4] = (uint8_t)((services[type].serviceID >> 8)&0xff);
        sendBuffer[5] = (uint8_t)(services[type].serviceID&0xff);
        sendBuffer[6] = buffer[0];
        sendBuffer[7] = buffer[1];

        commDev->transferDataToLin(0x3C, sendBuffer, 8);
        writeIndex = 2;
      }
      else
      {
        sendBuffer[1] = 0x20|((writeIndex-2)/6+1);
        ::memcpy(&sendBuffer[2], buffer + writeIndex, (desiredLength-writeIndex)>6?6:(desiredLength-writeIndex));
        commDev->transferDataToLin(0x3C, sendBuffer, 8);
        writeIndex += (desiredLength-writeIndex)>6?6:(desiredLength-writeIndex);
      }
    }

    this->msleep(10);
  }while(writeIndex < desiredLength);

  initialBufferAssignNAD(nad , nad, sendBuffer);
  commDev->transferDataToLin(0x3C, sendBuffer, 8);
  this->msleep(10);

  diagnosticMutex.unlock();
  return true;
}

void AmbientLinScheduler::LinSchedulerSleep(void)
{
  uint8_t sendBuffer[8];

  ::memset(sendBuffer, 0, 8);

  diagnosticMutex.lock();
  commDev->transferDataToLin(0x3C, sendBuffer, 8);
  this->msleep(50);
  diagnosticMutex.unlock();
}
