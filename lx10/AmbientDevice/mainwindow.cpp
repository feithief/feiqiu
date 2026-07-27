#include "mainwindow.h"
#include "ui_mainwindow.h"

const QSize appWindowSize(DAPP_CONFIG_WINDOW_WIDTH, DAPP_CONFIG_WINDOW_HEIGHT);

AmbientLinScheduler *AmbientLinScheduler::mInstance = NULL;
AmbientLinComm *AmbientLinComm::mInstance = NULL;

struct myRect
{
  int x,y,w,h;
};
#if 0
static const struct myRect slaveButtonRectMap[DSLAVE_NODE_AMOUNT]
{
  {36, 45 ,200, 100},{36, 190, 200, 100},{36, 335, 200, 100},{36, 480, 200, 100},{36, 625, 200, 100},
  {309, 45 ,200, 100},{309, 190, 200, 100},{309, 335, 200, 100},{309, 480, 200, 100},{309, 625, 200, 100},
  {856, 45 ,200, 100},{856, 190, 200, 100},{856, 335, 200, 100},{856, 480, 200, 100},{856, 625, 200, 100},
  {1128, 45 ,200, 100},{1128, 190, 200, 100},{1128, 335, 200, 100},{1128, 480, 200, 100},{1128, 625, 200, 100},
};
#else
static const struct myRect slaveButtonRectMap[DSLAVE_NODE_AMOUNT]=
{
  {36, 45 ,200, 100},{36, 160, 200, 100},{36, 275, 200, 100},{36, 390, 200, 100},{36, 505, 200, 100},{36, 620, 200, 100},
  {309, 45 ,200, 100},{309, 160, 200, 100},{309, 275, 200, 100},{309, 390, 200, 100},{309, 505, 200, 100},{309, 620, 200, 100},
  {856, 45 ,200, 100},{856, 160, 200, 100},{856, 275, 200, 100},{856, 390, 200, 100},{856, 505, 200, 100},{856, 620, 200, 100},
  {1128, 45 ,200, 100},{1128, 160, 200, 100},{1128, 275, 200, 100},{1128, 390, 200, 100},{1128, 505, 200, 100},{1128, 620, 200, 100}
};
#endif
static const QString basicQSS = "QMainWindow{\
                         border:2px solid #0fbacd;\
                         border-radius: 15px;\
                         background-color:rgba(0, 0, 0, 0.5);\
                         }";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  QFont fontSet;
  //QApplication::setOverrideCursor(Qt::BlankCursor);
  ui->setupUi(this);


  this->setMinimumSize(appWindowSize);
  this->setMaximumSize(appWindowSize);

  this->setStyleSheet(basicQSS);
  this->setWindowFlags(Qt::FramelessWindowHint);

  this->showFullScreen();

  masterButton = new BCMMasterButton(this);
  masterButton->setText("BCM\nMaster Control");
  masterButton->setGeometry(546,248,274,274);
  fontSet.setPointSize(24);
  masterButton->setFont(fontSet);
  connect(masterButton, SIGNAL(clicked()), this, SLOT(enterMasterConfigMode()));
  masterButton->show();

  exitButton = new BCMMasterButton(this);
  exitButton->setText("Exit");
  exitButton->setGeometry(518, 568, 160, 160);
  exitButton->setFont(fontSet);
  connect(exitButton, SIGNAL(clicked()), this , SLOT(close()));
  exitButton->setButtonMode(EButtonModeExit);
  exitButton->show();

  settingButton = new BCMMasterButton(this);
  settingButton->setText("Unlock\nFeature");
  settingButton->setGeometry(693, 568, 160, 160);
  settingButton->setFont(fontSet);
  connect(settingButton, SIGNAL(clicked()), this , SLOT(unlockFeature()));
  settingButton->setButtonMode(EButtonModeSetting);
  settingButton->show();

  for (int index = 6; index < DSLAVE_NODE_AMOUNT; index++)
  {
    slaveButton[index] = new SlaveButton(this, index);
    slaveButton[index]->setFont(fontSet);
    slaveButton[index]->setGeometry(slaveButtonRectMap[index-6].x,
                                    slaveButtonRectMap[index-6].y,
                                    slaveButtonRectMap[index-6].w,
                                    slaveButtonRectMap[index-6].h);

    slaveButton[index]->setButtonEnabled(index, false);
    connect(slaveButton[index], SIGNAL(slaveClicked(int)), this, SLOT(slaveConfig(int)));
    slaveButton[index]->show();
  }

  masterFrame = new BCMMasterFrame(this);
  slaveFrame = new SlaveFrameConfig(this);
  lockFrame = new unlockFrom(this);
  productVerifyFrame = new ProductionVerify(this);

  linScheduler = AmbientLinScheduler::getInstance(this);

  connect(linScheduler, SIGNAL(SlaveStatusChanged(struct SlaveStatus)), this, SLOT(slaveStatusHandle(struct SlaveStatus)));
  linScheduler->LinSchedulerStart();

  enterMasterConfigMode();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slaveStatusHandle(struct SlaveStatus status)
{
  if (status.isOnLine == true)
    slaveButton[status.slaveNAD -1]->setButtonEnabled(status.slaveNAD -1, true);
  else
    slaveButton[status.slaveNAD -1]->setButtonEnabled(status.slaveNAD -1, false);
}

void MainWindow::enterMasterConfigMode()
{
//  if (lockFrame->getMasterLockStatus() == false)
//  {
    if ((masterFrame != NULL) && (masterFrame->isHidden() == true))
    {
      masterFrame->setGeometry(0,0,1366,768);
      masterFrame->init();
      masterFrame->show();
    }
//  }
//  else//for zhongshan Demup
//  {
//    if ((productVerifyFrame != NULL) && (productVerifyFrame->isHidden() == true))
//    {
//      productVerifyFrame->setGeometry(0,0,1366,768);
//      productVerifyFrame->init();
//      productVerifyFrame->show();
//    }
//  }
}

void MainWindow::slaveConfig(int index)
{

  if (slaveButton[index]->isOnLine() == false)
    return;
#if 0
  if (lockFrame->getSlaveLockStatus() == true)
    return;
#endif
  if ((slaveFrame != NULL) && (slaveFrame->isHidden() == true))
  {
    slaveFrame->setGeometry(0,0,1366,768);
    slaveFrame->show();
    slaveFrame->SlaveFrameConfigInit(index+1);
  }
  //slaveFrame->SetValue();

}

void MainWindow::closeEvent(QCloseEvent *evt)
{
  this->closeApp();
}

void MainWindow::closeApp()
{
  linScheduler->LinSchedulerStop();
  while(linScheduler->isRunning() == true);
}

void MainWindow::unlockFeature()
{
  if ((lockFrame != NULL) && (lockFrame->isHidden() == true))
  {
    lockFrame->setGeometry(0,0,1366,768);
    lockFrame->init();
    lockFrame->show();
  }
}
