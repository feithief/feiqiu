#include "slaveframeconfig.h"
#include <QTest>
#include <QtMath>

static struct CalibrationInfo getCalibration(uint8_t *buffer)
{
  uint32_t tmp;
  struct CalibrationInfo ret;

  tmp = (uint32_t)buffer[0];
  tmp |= ((uint32_t)(buffer[1]<<8))&0x0000ff00;
  ret.x = ((double)tmp)/10000.0;
  tmp = (uint32_t)buffer[2];
  tmp |= ((uint32_t)(buffer[3]<<8))&0x0000ff00;
  ret.y = ((double)tmp)/10000.0;
  tmp = (uint32_t)buffer[4];
  tmp |= ((uint32_t)(buffer[5]<<8))&0x0000ff00;
  tmp |= ((uint32_t)(buffer[6]<<16))&0x00ff0000;
  tmp |= ((uint32_t)(buffer[7]<<24))&0xff000000;
  ret.Y = ((double)tmp)/10000.0;

  return ret;
}
SlaveWriteThread::SlaveWriteThread(QWidget *parent):
     QThread(parent)
{
  linScheduler = AmbientLinScheduler::getInstance();
}

void SlaveWriteThread::threadWriteValue(struct slaveConfigInfo *info)
{
  ::memcpy(&writtingInfo, info, sizeof(struct slaveConfigInfo));
}

void SlaveWriteThread::run()
{
  uint8_t buffer[22];
  bool needWrite;

  enum OperationType index;

  for (index = EOperationTypeGroupAddr; index <= EOperationTypeHardwareVer;)
  {
    this->msleep(50);
    ::memset(buffer, 0x0, 22);
    needWrite = true;
    switch(index)
    {
      case EOperationTypeGroupAddr:
      {
        int value = writtingInfo.GA;

        buffer[0] = (uint8_t)(value&0xff);
        buffer[1] = (uint8_t)((value>>8)&0xff);
      }
      break;
      case EOperationTypePlatform:
      {
        int value = writtingInfo.platform;

        buffer[0] = (uint8_t)(value&0xff);
        buffer[1] = (uint8_t)((value>>8)&0xff);
      }
      break;
      case EOperationTypeIntensity:
      {
        int value = writtingInfo.intensity;

        buffer[0] = (uint8_t)(value&0xff);
        buffer[1] = (uint8_t)((value>>8)&0xff);
      }
      break;
      case EOperationTypeRValue:
      {
        uint32_t temp_val;
        double tmp;

        tmp= writtingInfo.r.x;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[0] = (uint8_t)(temp_val&0xff);
        buffer[1] = (uint8_t)((temp_val>>8)&0xff);

        tmp= writtingInfo.r.y;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[2] = (uint8_t)(temp_val&0xff);
        buffer[3] = (uint8_t)((temp_val>>8)&0xff);

        tmp= writtingInfo.r.Y;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[4] = (uint8_t)(temp_val&0xff);
        buffer[5] = (uint8_t)((temp_val>>8)&0xff);
        buffer[6] = (uint8_t)((temp_val>>16)&0xff);
        buffer[7] = (uint8_t)((temp_val>>24)&0xff);
      }
      break;
      case EOperationTypeGValue:
      {
        uint32_t temp_val;
        double tmp;

        tmp= writtingInfo.g.x;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[0] = (uint8_t)(temp_val&0xff);
        buffer[1] = (uint8_t)((temp_val>>8)&0xff);

        tmp= writtingInfo.g.y;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[2] = (uint8_t)(temp_val&0xff);
        buffer[3] = (uint8_t)((temp_val>>8)&0xff);

        tmp= writtingInfo.g.Y;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[4] = (uint8_t)(temp_val&0xff);
        buffer[5] = (uint8_t)((temp_val>>8)&0xff);
        buffer[6] = (uint8_t)((temp_val>>16)&0xff);
        buffer[7] = (uint8_t)((temp_val>>24)&0xff);

        if (writtingInfo.nodeType == ENodeTypeWhite)
          needWrite = false;
      }
      break;
      case EOperationTypeBValue:
      {
        uint32_t temp_val;
        double tmp;

        tmp= writtingInfo.b.x;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[0] = (uint8_t)(temp_val&0xff);
        buffer[1] = (uint8_t)((temp_val>>8)&0xff);

        tmp= writtingInfo.b.y;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[2] = (uint8_t)(temp_val&0xff);
        buffer[3] = (uint8_t)((temp_val>>8)&0xff);

        tmp= writtingInfo.b.Y;
        tmp = tmp*10000;
        temp_val = floor(tmp);
        buffer[4] = (uint8_t)(temp_val&0xff);
        buffer[5] = (uint8_t)((temp_val>>8)&0xff);
        buffer[6] = (uint8_t)((temp_val>>16)&0xff);
        buffer[7] = (uint8_t)((temp_val>>24)&0xff);

        if (writtingInfo.nodeType == ENodeTypeWhite)
          needWrite = false;
      }
      break;
      case EOperationTypePartNO:
      {
        QString value = writtingInfo.PN;
        int charIndex;
        ::memset(buffer, 0x0, 22);
        ::memset(buffer, 0x20, 11);
        for (charIndex = 0; charIndex < value.length();charIndex++)
        {
          if (charIndex >= 12)
            break;

          buffer[charIndex] = (uint8_t)value.at(charIndex).toLatin1();
        }
      }
      break;
      case EOperationTypeSerialNO:
      {
        QString value = writtingInfo.SN;
        int charIndex;
        ::memset(buffer, 0x0, 22);
        for (charIndex = 0; charIndex < value.length();charIndex++)
        {
          if (charIndex >= 20)
            break;

          buffer[charIndex] = (uint8_t)value.at(charIndex).toLatin1();
        }
      }
      break;
    case EOperationTypeHardwareVer:
    {
      QString value = writtingInfo.HardwareVer;
      int charIndex;
      ::memset(buffer, 0x0, 22);
      for (charIndex = 0; charIndex < value.length();charIndex++)
      {
        if (charIndex >= 20)
          break;

        buffer[charIndex] = (uint8_t)value.at(charIndex).toLatin1();
      }
    }
    break;
      default:
        needWrite = false;
      break;
    }

    if (needWrite == true)
      linScheduler->writeServiceValue(writtingInfo.slaveNode, index, buffer, 22);

    index = (enum OperationType)((uint8_t)index + 1);
  }

  ::memset(buffer, 0x0, 22);

  buffer[0] = (uint8_t)(writtingInfo.SA&0xff);
  buffer[1] = (uint8_t)((writtingInfo.SA>>8)&0xff);

  linScheduler->writeServiceValue(writtingInfo.slaveNode, EOperationTypeSingleAddr, buffer, 22);

  emit WrittingFinished(true);
}

SlaveInitThread::SlaveInitThread(QWidget *parent):
  QThread(parent)
{
  linScheduler = AmbientLinScheduler::getInstance();
  isExit = false;
}

void SlaveInitThread::threadSetSlave(int slaveNode)
{
  currentNode = slaveNode;
  nodeType = ENodeTypeRGB;
  isExit = false;
}

void SlaveInitThread::stopFetching()
{
  isExit = true;
}

void SlaveInitThread::run()
{
  enum OperationType index;
  uint8_t buffer[22];
  uint32_t retLen;

  for (index = EOperationTypeLock; index <= EOperationTypeID;)
  {
    if (isExit == true)
      break;

    ::memset(buffer, 0x0, 22);
    retLen = linScheduler->readServiceValue(currentNode, index, buffer, 22);
    if (retLen != 0)
    {
      switch(index)
      {
        case EOperationTypeLock:
        {
          if ((buffer[0] & 0x80) != 0)
            emit LockFetched(true);
          else
            emit LockFetched(false);
        };
        break;
        case EOperationTypeSingleAddr:
        {
          int ret;
          ret = (int)buffer[0];
          emit SAFetched(ret);
        }
        break;
        case EOperationTypeGroupAddr:
        {
          int ret;
          ret = (int)buffer[0];
//          ret = (int)buffer[0];
          ret |= (int)(buffer[1]<<8)&0x0000ff00;
          emit GAFetched(ret);
        }
        break;
        case EOperationTypePlatform:
        {
          int ret;
          ret = (int)buffer[0];
          emit PlatformFetched(ret);
        }
        break;
        case EOperationTypeIntensity:
        {
          int ret;
          ret = (int)buffer[0];
          ret |= (int)(buffer[1]<<8)&0x0000ff00;
          emit IntensityFetched(ret);
        }
        break;
        case EOperationTypeRValue:
        {
          struct CalibrationInfo ret;

          ret = getCalibration(buffer);

          emit RFetched(ret);
        }
        break;
        case EOperationTypeGValue:
        {
          struct CalibrationInfo ret;

          ret = getCalibration(buffer);

          if (retLen == 3)
            this->nodeType = ENodeTypeWhite;

          emit GFetched(ret);
        }
        break;
        case EOperationTypeBValue:
        {
          struct CalibrationInfo ret;

          ret = getCalibration(buffer);

          if (retLen == 3)
            this->nodeType = ENodeTypeWhite;

          emit BFetched(ret);
        }
        break;
        case EOperationTypeSoftwareVer:
        {
          QString ret;
          ret = ret.fromLocal8Bit((const char*)buffer);
          emit SWFetched(ret);
        }
        break;
        case EOperationTypeHardwareVer:
        {
          QString ret;
          ret = ret.fromLocal8Bit((const char*)buffer);
          emit HWFetched(ret);
        }
        break;
        case EOperationTypePartNO:
        {
          QString ret;
          ret = ret.fromLocal8Bit((const char*)buffer);
          emit PNFetched(ret);
        }
        break;
        case EOperationTypeSerialNO:
        {
          QString ret;
          ret = ret.fromLocal8Bit((const char*)buffer);
          emit SerialFetched(ret);
        }
        break;
        case EOperationTypeID:
        {
          QString ret;
          unsigned short retNum;

          retNum = (unsigned short)buffer[0];
          retNum |= ((unsigned short)buffer[1]<<8)&0xff00;
          ret = ret.asprintf("0x%X", retNum);
          emit SuppFetched(ret);

          retNum = (unsigned short)buffer[2];
          retNum |= ((unsigned short)buffer[3]<<8)&0xff00;
          ret = ret.asprintf("0x%X", retNum);
          emit FuncFetched(ret);

          ret = ret.asprintf("0x%X", buffer[4]);
          emit VariantFetched(ret);
        }
        break;
        default:
        break;
      }
    }

    this->msleep(10);

    index = (enum OperationType)((uint8_t)index + 1);
  }

  emit fechingFinished(true, this->nodeType);


  this->exit(0);
}

static const QString QFrameBasicQSS = "QFrame{\
                                        border:2px solid #0fbacd;\
                                        border-radius: 15px;\
                                        background-color:rgba(0, 0, 0, 0.7);\
                                        }";


static const QString PushButtonEnabledQSS = "QPushButton{\
                                              border:2px solid #0fbacd;\
                                              border-top-right-radius: 15px;\
                                              border-bottom-left-radius: 15px;\
                                              color:rgb(255,251,240);\
                                              font-size:22px;\
                                              background:rgba(29, 165, 219, 0.3);\
                                              }";

static const QString PushButtonDisabledQSS = "QPushButton{\
                                                border:0px;\
                                                border-top-right-radius: 15px;\
                                                border-bottom-left-radius: 15px;\
                                                color:rgba(255,251,240,0.5);\
                                                font-size:22px;\
                                                background:rgba(29, 165, 219, 0.1);\
                                                }";

SlaveFrameConfig::SlaveFrameConfig(QWidget *parent)
  : QWidget(parent),
  ui(new Ui::SlaveNodeFrame)
{
  backgroundframe  = new QFrame(this);
  backgroundframe->setStyleSheet(QFrameBasicQSS);
  backgroundframe->setGeometry(0,0,1366,768);
  backgroundframe->show();

  ui->setupUi(this);

  dialog = new ADialog(this);
  dialog->hide();

  qRegisterMetaType<struct CalibrationInfo>("struct CalibrationInfo");
  qRegisterMetaType<enum ENodeType>("enum ENodeType");

  initThread = new SlaveInitThread(this);
  writeThread = new SlaveWriteThread(this);

  connect(initThread, SIGNAL(fechingFinished(bool, enum ENodeType)), this, SLOT(processReadDone(bool, enum ENodeType)));
  connect(writeThread, SIGNAL(WrittingFinished(bool)), this, SLOT(processWriteDone(bool)));

  connect(initThread, SIGNAL(SWFetched(QString)), ui->labelSWVer, SLOT(setText(QString)));
  connect(initThread, SIGNAL(HWFetched(QString)), ui->lineEditHWVer, SLOT(setText(QString)));
  connect(initThread, SIGNAL(SuppFetched(QString)), ui->labelSuppVer, SLOT(setText(QString)));
  connect(initThread, SIGNAL(FuncFetched(QString)), ui->labelFuncVer, SLOT(setText(QString)));
  connect(initThread, SIGNAL(VariantFetched(QString)), ui->labelVariantVer, SLOT(setText(QString)));

  connect(initThread, SIGNAL(SAFetched(int)), ui->spinBoxSA, SLOT(setValue(int)));
  connect(initThread, SIGNAL(GAFetched(int)), ui->spinBoxGA, SLOT(setValue(int)));
  connect(initThread, SIGNAL(PlatformFetched(int)), ui->spinBoxPlatform, SLOT(setValue(int)));
  connect(initThread, SIGNAL(IntensityFetched(int)), ui->spinBoxIntensity, SLOT(setValue(int)));

  connect(initThread, SIGNAL(PNFetched(QString)), ui->lineEditPN, SLOT(setText(QString)));
  connect(initThread, SIGNAL(SerialFetched(QString)), ui->lineEditSerial, SLOT(setText(QString)));

  connect(initThread, SIGNAL(LockFetched(bool)), this, SLOT(getLockStatus(bool)));
  connect(initThread, SIGNAL(RFetched(struct CalibrationInfo)), this, SLOT(getRValue(struct CalibrationInfo)));
  connect(initThread, SIGNAL(GFetched(struct CalibrationInfo)), this, SLOT(getGValue(struct CalibrationInfo)));
  connect(initThread, SIGNAL(BFetched(struct CalibrationInfo)), this, SLOT(getBValue(struct CalibrationInfo)));

  connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(exitSlaveConfig()));

  linScheduler = AmbientLinScheduler::getInstance();
  connect(linScheduler, SIGNAL(SlaveStatusChanged(struct SlaveStatus)), this, SLOT(updateNodeState(struct SlaveStatus)));

  this->hide();

  connect(ui->pushButtonNoCalibrate, SIGNAL(clicked()), this, SLOT(buttonCalibrateNormal()));
  connect(ui->pushButtonCalibrateR, SIGNAL(clicked()), this, SLOT(buttonCalibrateR()));
  connect(ui->pushButtonCalibrateG, SIGNAL(clicked()), this, SLOT(buttonCalibrateG()));
  connect(ui->pushButtonCalibrateB, SIGNAL(clicked()), this, SLOT(buttonCalibrateB()));

  currentNode = 0;

  connect(ui->pushButtonAccept, SIGNAL(clicked()), this, SLOT(changeConfigs()));




  ui->spinBoxGA->installEventFilter(this);
  ui->spinBoxSA->installEventFilter(this);
  ui->spinBoxIntensity->installEventFilter(this);
  ui->spinBoxPlatform->installEventFilter(this);

  ui->lineEditPN->installEventFilter(this);
  ui->lineEditSerial->installEventFilter(this);
  ui->lineEditHWVer->installEventFilter(this);

  ui->doubleSpinBoxRX->installEventFilter(this);
  ui->doubleSpinBoxRY->installEventFilter(this);
  ui->doubleSpinBoxRL->installEventFilter(this);
  ui->doubleSpinBoxGX->installEventFilter(this);
  ui->doubleSpinBoxGY->installEventFilter(this);
  ui->doubleSpinBoxGL->installEventFilter(this);
  ui->doubleSpinBoxBX->installEventFilter(this);
  ui->doubleSpinBoxBY->installEventFilter(this);
  ui->doubleSpinBoxBL->installEventFilter(this);

  keys = new KeyBoard(this);
  keys->hide();
}

static const QString stateMap[4] = {"Init", "Reserved", "Normal", "Error"};

static const QString NormalStatusQSS = "QLabel{\
                                                font-size:22px;\
                                                color:rgb(104, 194, 53);\
                                              }";
static const QString ErrorStatusQSS = "QLabel{\
                                                font-size:22px;\
                                                color:rgb(254, 67, 101);\
                                              }";

void SlaveFrameConfig::updateNodeState(struct SlaveStatus status)
{
  if (currentNode == 0)
    return;

  if (status.slaveNAD == currentNode)
  {
    if (status.isOnLine == false)
      exitSlaveConfig();
    else
    {
#if 0
      ui->labelRedOpen->setText(stateMap[(uint8_t)status.ROpenStatus]);
      if (status.ROpenStatus == ESlaveErrorFlagError)
        ui->labelRedOpen->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelRedOpen->setStyleSheet(NormalStatusQSS);

      ui->labelGreenOpen->setText(stateMap[(uint8_t)status.GOpenStatus]);
      if (status.GOpenStatus == ESlaveErrorFlagError)
        ui->labelGreenOpen->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelGreenOpen->setStyleSheet(NormalStatusQSS);

      ui->labelBlueOpen->setText(stateMap[(uint8_t)status.BOpenStatus]);
      if (status.BOpenStatus == ESlaveErrorFlagError)
        ui->labelBlueOpen->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelBlueOpen->setStyleSheet(NormalStatusQSS);

      ui->labelRedShort->setText(stateMap[(uint8_t)status.RShortStatus]);
      if (status.RShortStatus == ESlaveErrorFlagError)
        ui->labelRedShort->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelRedShort->setStyleSheet(NormalStatusQSS);

      ui->labelGreenShort->setText(stateMap[(uint8_t)status.GShortStatus]);
      if (status.GShortStatus == ESlaveErrorFlagError)
        ui->labelGreenShort->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelGreenShort->setStyleSheet(NormalStatusQSS);

      ui->labelBlueShort->setText(stateMap[(uint8_t)status.BShortStatus]);
      if (status.BShortStatus == ESlaveErrorFlagError)
        ui->labelBlueShort->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelBlueShort->setStyleSheet(NormalStatusQSS);

      ui->labelRomStatus->setText(stateMap[(uint8_t)status.romState]);
      if (status.romState == ESlaveErrorFlagError)
        ui->labelRomStatus->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelRomStatus->setStyleSheet(NormalStatusQSS);

      ui->labelRamStatus->setText(stateMap[(uint8_t)status.ramState]);
      if (status.ramState == ESlaveErrorFlagError)
        ui->labelRamStatus->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelRamStatus->setStyleSheet(NormalStatusQSS);

      ui->labelNvrmStatus->setText(stateMap[(uint8_t)status.nvrmState]);
      if (status.nvrmState == ESlaveErrorFlagError)
        ui->labelNvrmStatus->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelNvrmStatus->setStyleSheet(NormalStatusQSS);

      ui->labelTempState->setText(QString::number(status.tempState));
      if (status.tempState >= 5)
        ui->labelTempState->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelTempState->setStyleSheet(NormalStatusQSS);


      ui->labelTempState->setText(QString::number(status.tempState));
      if (status.tempState >= 5)
        ui->labelTempState->setStyleSheet(ErrorStatusQSS);
      else
        ui->labelTempState->setStyleSheet(NormalStatusQSS);
#else
    ui->labelErrResp->setText(QString::number(status.ErrResp));
    ui->labelHWVerNum->setText(QString::number(status.HWVerNum));
    ui->labelSWVerNum->setText(QString::number(status.SWVerNum));
#endif
    }
  }
}

void SlaveFrameConfig::exitSlaveConfig()
{
  initThread->stopFetching();
  while(initThread->isRunning());
#if 0
  ui->labelRedOpen->setText("");
  ui->labelGreenOpen->setText("");
  ui->labelBlueOpen->setText("");
  ui->labelRedShort->setText("");
  ui->labelGreenShort->setText("");
  ui->labelBlueShort->setText("");
  ui->labelRomStatus->setText("");
  ui->labelRamStatus->setText("");
  ui->labelNvrmStatus->setText("");
  ui->labelTempState->setText("");
#endif
  ui->labelSWVerNum->setText("");
  ui->labelHWVerNum->setText("");
  ui->labelErrResp->setText("");

  ui->labelSWVer->setText("");
  ui->lineEditHWVer->setText("");
  ui->labelSuppVer->setText("");
  ui->labelFuncVer->setText("");
  ui->labelVariantVer->setText("");

  ui->lineEditPN->setText("");
  ui->lineEditSerial->setText("");

  ui->spinBoxSA->setValue(0);
  ui->spinBoxGA->setValue(0);
  ui->spinBoxPlatform->setValue(0);
  ui->spinBoxIntensity->setValue(0);

  ui->doubleSpinBoxRX->setValue(0);
  ui->doubleSpinBoxRY->setValue(0);
  ui->doubleSpinBoxRL->setValue(0);

  ui->doubleSpinBoxGX->setValue(0);
  ui->doubleSpinBoxGY->setValue(0);
  ui->doubleSpinBoxGL->setValue(0);

  ui->doubleSpinBoxBX->setValue(0);
  ui->doubleSpinBoxBY->setValue(0);
  ui->doubleSpinBoxBL->setValue(0);

  this->hide();
  currentNode = 0;
}

void SlaveFrameConfig::SlaveFrameConfigInit(int slaveNode)
{
  if (initThread != NULL)
  {
    currentNode = slaveNode;
    while(initThread->isRunning());
    initThread->threadSetSlave(slaveNode);
    dialog->setMode(EDialogTypeWaitting);
    dialog->setContent("Reading Attributes...");
    dialog->show();
    initThread->start();
  }
}

void SlaveFrameConfig::getLockStatus(bool islocked)
{
  if (islocked == true)
  {
    ui->pushButtonLock->setStyleSheet(PushButtonEnabledQSS);
    ui->pushButtonUnlock->setStyleSheet(PushButtonDisabledQSS);
  }
  else
  {
    ui->pushButtonLock->setStyleSheet(PushButtonDisabledQSS);
    ui->pushButtonUnlock->setStyleSheet(PushButtonEnabledQSS);
  }
}

void SlaveFrameConfig::getRValue(struct CalibrationInfo r)
{
  ui->doubleSpinBoxRX->setValue(r.x);
  ui->doubleSpinBoxRY->setValue(r.y);
  ui->doubleSpinBoxRL->setValue(r.Y);

}

void SlaveFrameConfig::getGValue(struct CalibrationInfo g)
{
  ui->doubleSpinBoxGX->setValue(g.x);
  ui->doubleSpinBoxGY->setValue(g.y);
  ui->doubleSpinBoxGL->setValue(g.Y);
}

void SlaveFrameConfig::getBValue(struct CalibrationInfo b)
{
  ui->doubleSpinBoxBX->setValue(b.x);
  ui->doubleSpinBoxBY->setValue(b.y);
  ui->doubleSpinBoxBL->setValue(b.Y);
}

void SlaveFrameConfig::buttonCalibrateNormal()
{
  uint8_t buffer[8];
  ::memset(buffer, 0x0, 8);
  buffer[0] = 0;
  linScheduler->writeServiceValue(currentNode, EOperationTypeCalibration, buffer, 8);
}

void SlaveFrameConfig::buttonCalibrateR()
{
  uint8_t buffer[8];
  ::memset(buffer, 0x0, 8);
  buffer[0] = 1;
  linScheduler->writeServiceValue(currentNode, EOperationTypeCalibration, buffer, 8);
}

void SlaveFrameConfig::buttonCalibrateG()
{
  uint8_t buffer[8];
  ::memset(buffer, 0x0, 8);
  buffer[0] = 2;
  linScheduler->writeServiceValue(currentNode, EOperationTypeCalibration, buffer, 8);
}

void SlaveFrameConfig::buttonCalibrateB()
{
  uint8_t buffer[8];
  ::memset(buffer, 0x0, 8);
  buffer[0] = 3;
  linScheduler->writeServiceValue(currentNode, EOperationTypeCalibration, buffer, 8);
}

void SlaveFrameConfig::changeConfigs()
{
  struct slaveConfigInfo info;

  info.slaveNode = currentNode;
  info.nodeType = this->nodeType;

  info.SA = ui->spinBoxSA->value();
  info.GA = ui->spinBoxGA->value();
  info.platform = ui->spinBoxPlatform->value();
  info.intensity = ui->spinBoxIntensity->value();
  info.PN = ui->lineEditPN->text();
  info.SN = ui->lineEditSerial->text();
  info.HardwareVer = ui->lineEditHWVer->text();
  info.r.x = ui->doubleSpinBoxRX->value();
  info.r.y = ui->doubleSpinBoxRY->value();
  info.r.Y = ui->doubleSpinBoxRL->value();
  info.g.x = ui->doubleSpinBoxGX->value();
  info.g.y = ui->doubleSpinBoxGY->value();
  info.g.Y = ui->doubleSpinBoxGL->value();
  info.b.x = ui->doubleSpinBoxBX->value();
  info.b.y = ui->doubleSpinBoxBY->value();
  info.b.Y = ui->doubleSpinBoxBL->value();

  writeThread->threadWriteValue(&info);

  dialog->setMode(EDialogTypeWaitting);
  dialog->setContent("Writting Attributes...");
  dialog->show();

  writeThread->start();
}

bool SlaveFrameConfig::eventFilter(QObject *watched, QEvent *event)
{
  if (event->type() == QEvent::FocusIn)
  {
    if ((watched->inherits("QSpinBox") == true) ||
        (watched->inherits("QDoubleSpinBox") == true) ||
        (watched->inherits("QLineEdit") == true))
      showInputKeyBoard(true);
  }

  if (event->type() == QEvent::FocusOut)
  {
    if ((watched->inherits("QSpinBox") == true) ||
        (watched->inherits("QDoubleSpinBox") == true) ||
        (watched->inherits("QLineEdit") == true))
      showInputKeyBoard(false);
  }

  return QWidget::eventFilter(watched, event);
}

void SlaveFrameConfig::showInputKeyBoard(bool show)
{
  QWidget *widget = QApplication::focusWidget();
  if (show == true)
  {
    if (widget->y() > this->height()/2)
      keys->setGeometry(68,0, 1230, 353);
    else
      keys->setGeometry(68,this->height() - 353 - 5, 1230, 353);
    keys->setCurrentObject(widget);
    keys->show();
  }
  else
    keys->hide();
}

void SlaveFrameConfig::processReadDone(bool result, enum ENodeType type)
{
  nodeType = type;
  dialog->changeToDoneMode();
  if (result == false)
    dialog->setContent("Error Occured!");

}

void SlaveFrameConfig::processWriteDone(bool result)
{
  dialog->changeToDoneMode();
  if (result == false)
    dialog->setContent("Error Occured!");
}

