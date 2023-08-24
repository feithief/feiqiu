#include "productionverify.h"
#include "ui_productionverify.h"

static const QString QFrameBasicQSS = "QFrame{\
                                        border:2px solid #0fbacd;\
                                        border-radius: 15px;\
                                        background-color:rgba(0, 0, 0, 0.7);\
                                        }";

static const QString buttonEnabled = "QPushButton{\
                                        border:2px solid #0fbacd;\
                                        border-top-right-radius: 15px;\
                                        border-bottom-left-radius: 15px;\
                                        color:rgb(0,255,255);\
                                        background:rgba(29, 165, 219, 0.3);}";

static const QString buttonDisabled = "QPushButton{\
                                        border:0px solid #0fbacd;\
                                        border-top-right-radius: 15px;\
                                        border-bottom-left-radius: 15px;\
                                        color:rgba(0,255,255,0.2);\
                                        background:rgba(14, 82, 109, 0.3);}";

static const int color[10][3] = {
  {32 ,255,200},
  {175,255,64 },
  {255,0  ,3  },
  {255,88 ,4  },
  {255,198,0  },
  {149,255,3  },
  {19 ,255,49 },
  {3  ,86 ,255},
  {26 ,71 ,255},
  {255,8  ,42 },
};

ProductionVerify::ProductionVerify(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ProductionVerify)
{
  backgroundframe  = new QFrame(this);
  backgroundframe->setStyleSheet(QFrameBasicQSS);
  backgroundframe->setGeometry(0,0,1366,768);
  backgroundframe->show();
  ui->setupUi(this);

  colorIndex = 0;
  colorIndexRecord = 0;

  colorChangeTimer = new QTimer(this);
  colorChangeTimer->setInterval(3000);
  linScheduler = AmbientLinScheduler::getInstance(this);

  masterSignals.address = 65535;
  //masterSignals.usingGroupAdress = false;
  masterSignals.R = color[0][0];
  masterSignals.G = color[0][1];
  masterSignals.B = color[0][2];
  masterSignals.intensity = 200;
 // masterSignals.mode = ESlaveModeNormal;
  masterSignals.dimmingRamp = 0;
 // masterSignals.validMarker = false;
  //masterSignals.dimmingType = ESlaveDimmingModeAbsolute;
  linScheduler->setBCMSignal(&masterSignals);

  connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(hide()));

  ui->pushButtonNode1->setStyleSheet(buttonDisabled);
  ui->pushButtonNode2->setStyleSheet(buttonDisabled);
  ui->pushButtonNode3->setStyleSheet(buttonDisabled);
  ui->pushButtonNode4->setStyleSheet(buttonDisabled);
  ui->pushButtonNode5->setStyleSheet(buttonDisabled);
  ui->pushButtonNode6->setStyleSheet(buttonDisabled);
  ui->pushButtonNode7->setStyleSheet(buttonDisabled);
  ui->pushButtonNode11->setStyleSheet(buttonDisabled);
  ui->pushButtonNode12->setStyleSheet(buttonDisabled);

  nodeMapper = new QSignalMapper((QObject*)this);
  nodeMapper->setMapping(ui->pushButtonNode1, 1);
  nodeMapper->setMapping(ui->pushButtonNode2, 2);
  nodeMapper->setMapping(ui->pushButtonNode3, 3);
  nodeMapper->setMapping(ui->pushButtonNode4, 4);
  nodeMapper->setMapping(ui->pushButtonNode5, 5);
  nodeMapper->setMapping(ui->pushButtonNode6, 6);
  nodeMapper->setMapping(ui->pushButtonNode7, 7);
  nodeMapper->setMapping(ui->pushButtonNode11, 11);
  nodeMapper->setMapping(ui->pushButtonNode12, 12);

  connect(ui->pushButtonNode1, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode2, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode3, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode4, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode5, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode6, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode7, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode11, SIGNAL(clicked()), nodeMapper, SLOT(map()));
  connect(ui->pushButtonNode12, SIGNAL(clicked()), nodeMapper, SLOT(map()));

  connect(nodeMapper, SIGNAL(mapped(int)), this, SLOT(changeAddress(int)));
  connect(ui->pushButtonChangeColorManual,SIGNAL(clicked()), this, SLOT(changeColorManual()));
  connect(ui->pushButtonChangeColorAuto,SIGNAL(clicked()), this, SLOT(changeColorAuto()));
  connect(colorChangeTimer, SIGNAL(timeout()), this, SLOT(changeColor()));

  connect(ui->horizontalSliderIntensity,SIGNAL(valueChanged(int)), ui->spinBoxIntensity, SLOT(setValue(int)));
  connect(ui->horizontalSliderIntensity,SIGNAL(valueChanged(int)), this, SLOT(intensityChanged(int)));
  connect(ui->spinBoxIntensity,SIGNAL(valueChanged(int)), ui->horizontalSliderIntensity, SLOT(setValue(int)));

  ui->spinBoxIntensity->installEventFilter(this);
  keys = new KeyBoard(this);
  keys->hide();
}

ProductionVerify::~ProductionVerify()
{
  delete backgroundframe;
  delete ui;
}

void ProductionVerify::init(void)
{
  masterSignals = linScheduler->getBCMSignal();

  ui->pushButtonNode1->setStyleSheet(buttonDisabled);
  ui->pushButtonNode2->setStyleSheet(buttonDisabled);
  ui->pushButtonNode3->setStyleSheet(buttonDisabled);
  ui->pushButtonNode4->setStyleSheet(buttonDisabled);
  ui->pushButtonNode5->setStyleSheet(buttonDisabled);
  ui->pushButtonNode6->setStyleSheet(buttonDisabled);
  ui->pushButtonNode7->setStyleSheet(buttonDisabled);
  ui->pushButtonNode11->setStyleSheet(buttonDisabled);
  ui->pushButtonNode12->setStyleSheet(buttonDisabled);

//  if (masterSignals.usingGroupAdress == false)
//  {
//    changeAddress(masterSignals.address);
//  }
  if (0)
  {}
  else
  {
    masterSignals.address = 65535;
    //masterSignals.usingGroupAdress = false;
    masterSignals.R = color[0][0];
    masterSignals.G = color[0][1];
    masterSignals.B = color[0][2];
    masterSignals.intensity = 100;
   // masterSignals.mode = ESlaveModeNormal;
    masterSignals.dimmingRamp = 0;
   // masterSignals.validMarker = false;
    //masterSignals.dimmingType = ESlaveDimmingModeAbsolute;

    ui->pushButtonNode5->setStyleSheet(buttonEnabled);
  }

  ui->spinBoxIntensity->setValue(masterSignals.intensity);

  linScheduler->setBCMSignal(&masterSignals);
}

void ProductionVerify::intensityChanged(int intensity)
{
  masterSignals.intensity = intensity;
  linScheduler->setBCMSignal(&masterSignals);
}

void ProductionVerify::changeColorManual(void)
{
  if (colorChangeTimer->isActive())
  {
    colorChangeTimer->stop();
    ui->pushButtonChangeColorAuto->setText("开始自动改变颜色");
  }

  changeColor();
}

void ProductionVerify::changeColorAuto(void)
{
  if (colorChangeTimer->isActive())
  {
    colorChangeTimer->stop();
    ui->pushButtonChangeColorAuto->setText("开始自动改变颜色");
  }
  else
  {
    colorChangeTimer->start();
    ui->pushButtonChangeColorAuto->setText("停止自动改变颜色 ");
  }
}

void ProductionVerify::changeColor(void)
{
  colorIndex = colorIndex % 10;

  masterSignals.R = color[colorIndex][0];
  masterSignals.G = color[colorIndex][1];
  masterSignals.B = color[colorIndex][2];

  linScheduler->setBCMSignal(&masterSignals);
  colorIndex++;
}

void ProductionVerify::changeAddress(int index)
{
  ui->pushButtonNode1->setStyleSheet(buttonDisabled);
  ui->pushButtonNode2->setStyleSheet(buttonDisabled);
  ui->pushButtonNode3->setStyleSheet(buttonDisabled);
  ui->pushButtonNode4->setStyleSheet(buttonDisabled);
  ui->pushButtonNode5->setStyleSheet(buttonDisabled);
  ui->pushButtonNode6->setStyleSheet(buttonDisabled);
  ui->pushButtonNode7->setStyleSheet(buttonDisabled);
  ui->pushButtonNode11->setStyleSheet(buttonDisabled);
  ui->pushButtonNode12->setStyleSheet(buttonDisabled);


  if (index == 1)
    ui->pushButtonNode1->setStyleSheet(buttonEnabled);

  if (index == 2)
    ui->pushButtonNode2->setStyleSheet(buttonEnabled);

  if (index == 3)
    ui->pushButtonNode3->setStyleSheet(buttonEnabled);

  if (index == 4)
    ui->pushButtonNode4->setStyleSheet(buttonEnabled);

  if (index == 5)
    ui->pushButtonNode5->setStyleSheet(buttonEnabled);

  if (index == 6)
    ui->pushButtonNode6->setStyleSheet(buttonEnabled);

  if (index == 7)
    ui->pushButtonNode7->setStyleSheet(buttonEnabled);

  if (index == 11)
    ui->pushButtonNode11->setStyleSheet(buttonEnabled);

  if (index == 12)
    ui->pushButtonNode12->setStyleSheet(buttonEnabled);

  if ((index <= 0) || (index > 7))
  {
    if ((index < 11) || (index > 12))
    {
        index = 21;
        ui->pushButtonNode5->setStyleSheet(buttonEnabled);
    }
  }

  masterSignals.address = index;
  //masterSignals.usingGroupAdress = false;

  if ((colorIndexRecord != index) &&
      (colorIndexRecord != 0))
    linScheduler->LinSchedulerSleep();

  colorIndexRecord = index;

  linScheduler->setBCMSignal(&masterSignals);
}

bool ProductionVerify::eventFilter(QObject *watched, QEvent *event)
{
  if (event->type() == QEvent::FocusIn)
  {
    if (watched->inherits("QSpinBox") == true)
      showInputKeyBoard(true);
  }

  return QWidget::eventFilter(watched, event);
}

void ProductionVerify::showInputKeyBoard(bool show)
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
