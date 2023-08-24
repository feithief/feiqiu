#include "unlockfrom.h"
#include "ui_unlockfrom.h"
#include <QTime>

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
                                                font-size:28px;\
                                                background:rgba(29, 165, 219, 0.3);\
                                                }";

static const QString PushButtonDisabledQSS = "QPushButton{\
                                                border:0px;\
                                                border-top-right-radius: 15px;\
                                                border-bottom-left-radius: 15px;\
                                                color:rgba(255,251,240,0.5);\
                                                font-size:28px;\
                                                background:rgba(29, 165, 219, 0.1);\
                                                }";

unlockFrom::unlockFrom(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::unlockFrom)
{
  backgroundframe  = new QFrame(this);
  backgroundframe->setStyleSheet(QFrameBasicQSS);
  backgroundframe->setGeometry(0,0,1366,768);
  backgroundframe->show();
  ui->setupUi(this);

  isMasterLocked = true;
  isSlaveLocked = true;
  isFactoryLocked = true;

  dialog = new ADialog(this);
  dialog->setMode(EDialogTypeDone);
  dialog->hide();

  keys = new KeyBoard(this);
  keys->hide();

  ui->lineEditCode->installEventFilter(this);

  countDownTimer = new QTimer(this);
  connect(countDownTimer, SIGNAL(timeout()),this,SLOT(unlockTimeOut()));

  connect(ui->pushButtonSelMaster, SIGNAL(clicked()), this , SLOT(selectMasterClicked()));
  connect(ui->pushButtonSelSlave, SIGNAL(clicked()), this , SLOT(selectSlaveClicked()));
  connect(ui->pushButtonSelFactory, SIGNAL(clicked()), this , SLOT(selectFactoryClicked()));

  connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(hide()));
  connect(ui->pushButtonConfirm,SIGNAL(clicked()),this,SLOT(checkReusult()));
}

unlockFrom::~unlockFrom()
{
  delete ui;
}

void unlockFrom::init()
{
  QTime time;

  time = QTime::currentTime();
  qsrand(time.msec()+time.second()*1000);

  generatedCode = qrand() % 100000000;

  if (generatedCode < 10000000)
    generatedCode += (qrand()%10)*10000000;

  ui->labelCode->setText(QString::number(generatedCode,10));

  isMasterSelected = false;
  isSlaveSelected = false;
  isFactorySelected = false;

  ui->pushButtonSelMaster->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonSelSlave->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonSelFactory->setStyleSheet(PushButtonDisabledQSS);

  ui->lineEditCode->setText("");
}

void unlockFrom::selectMasterClicked()
{
  if (isMasterSelected == false)
  {
    isMasterSelected = true;
    ui->pushButtonSelMaster->setStyleSheet(PushButtonEnabledQSS);
  }
  else
  {
    isMasterSelected = false;
    ui->pushButtonSelMaster->setStyleSheet(PushButtonDisabledQSS);
  }
}

void unlockFrom::selectSlaveClicked()
{
  if (isSlaveSelected == false)
  {
    isSlaveSelected = true;
    ui->pushButtonSelSlave->setStyleSheet(PushButtonEnabledQSS);
  }
  else
  {
    isSlaveSelected = false;
    ui->pushButtonSelSlave->setStyleSheet(PushButtonDisabledQSS);
  }
}

void unlockFrom::selectFactoryClicked()
{
  if (isFactorySelected == false)
  {
    isFactorySelected = true;
    ui->pushButtonSelFactory->setStyleSheet(PushButtonEnabledQSS);
  }
  else
  {
    isFactorySelected = false;
    ui->pushButtonSelFactory->setStyleSheet(PushButtonDisabledQSS);
  }
}

void unlockFrom::unlockTimeOut()
{
  isMasterLocked = true;
  isSlaveLocked = true;
  isFactoryLocked = true;
}

bool unlockFrom::getMasterLockStatus()
{
  return isMasterLocked;
}

bool unlockFrom::getSlaveLockStatus()
{
  return isSlaveLocked;
}

bool unlockFrom::getFactoryLockStatus()
{
  return isFactoryLocked;
}

void unlockFrom::checkReusult()
{
  QString compareString;
  unsigned short int unlockType = 0;

  if (isMasterSelected == true)
  {
    unlockType |= ELockTypeMaster;
  }

  if (isSlaveSelected == true)
  {
    unlockType |= ELockTypeSlave;
  }

  if (isFactorySelected == true)
  {
    unlockType |= ElockTypeFactory;
  }

  if (unlockType == 0)
  {
    dialog->setContent("Please Select One Unlock Function Atleast!");
    dialog->show();
    return;
  }

  compareString = decode(generatedCode, unlockType);

  if (ui->lineEditCode->text().compare(compareString) == 0)
  {
    countDownTimer->stop();
    countDownTimer->setInterval(3600*1000);
    countDownTimer->setSingleShot(true);
    countDownTimer->start();

    if (unlockType & ELockTypeMaster)
      isMasterLocked = false;

    if (unlockType & ELockTypeSlave)
      isSlaveLocked = false;

    if (unlockType & ElockTypeFactory)
      isFactoryLocked = false;

    dialog->setContent("Unlock Success!");
    dialog->show();
  }
  else
  {
    countDownTimer->stop();
    unlockTimeOut();
    dialog->setContent("Wrong Pass Code!");
    dialog->show();
  }
}

QString unlockFrom::decode(int value, unsigned short int type)
{
  QString result;
  unsigned char xorTmp = (unsigned char)(type&0xff);
  static const unsigned char xorMap[4] = {0x55, 0xAA, 0xA5, 0x5A};
  int singleValue[8];

  result.clear();

  for (int index = 8; index >= 1; index--)
  {
    int divided = 1;

    for (int tmp = 0; tmp < index; tmp++)
    {
      divided = divided*10;
    }

    singleValue[index -1] = value / divided;
    value = value - singleValue[index -1]*divided;
  }

  for (int index = 0; index < 8; index++)
  {
    xorTmp = xorTmp^singleValue[index];
  }

  result = result.append(QString::number(xorTmp));

  for (int index = 0 ; index < 8; index++)
  {
    singleValue[index] = singleValue[index]^xorMap[xorTmp%4];
  }

  singleValue[0] = singleValue[0]^(type&0xff);
  singleValue[1] = singleValue[0]^singleValue[1];
  singleValue[2] = singleValue[1]^singleValue[2];
  singleValue[3] = singleValue[2]^singleValue[3];
  singleValue[4] = singleValue[3]^singleValue[4];
  singleValue[5] = singleValue[4]^singleValue[5];
  singleValue[6] = singleValue[5]^singleValue[6];
  singleValue[7] = singleValue[6]^singleValue[7];

  for (int index = 0 ; index < 8; index++)
  {
    result = result.append(QString::number(singleValue[index]));
  }

  return result;
}

void unlockFrom::showInputKeyBoard(bool show)
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

bool unlockFrom::eventFilter(QObject *watched, QEvent *event)
{
  if (event->type() == QEvent::FocusIn)
  {
    if (watched->inherits("QLineEdit") == true)
      showInputKeyBoard(true);
  }

  return QWidget::eventFilter(watched, event);
}
