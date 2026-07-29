#include "productionverify.h"
#include "linlayout.h"
#include "ui_productionverify.h"

#include <QGridLayout>
#include <QHideEvent>
#include <QList>
#include <QPushButton>
#include <QScrollArea>

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

namespace {

static const LinPredefinedColor kDirectRgbTestColors[] = {
  {0xFF, 0x00, 0x00},
  {0x00, 0xFF, 0x00},
  {0x00, 0x00, 0xFF},
  {0xFF, 0xFF, 0xFF}
};

int productionColorCount(const LinLayout &layout)
{
  if (layout.colorModel == ELinColorModelDirectRgbOnly)
  {
    return static_cast<int>(sizeof(kDirectRgbTestColors) /
                            sizeof(kDirectRgbTestColors[0]));
  }

  if (supportsPredefinedColors(layout) &&
      (layout.predefinedColors != 0))
    return layout.predefinedColorCount;

  return 0;
}

void assignProductionColor(const LinLayout &layout,
                           int colorIndex,
                           BCMSignal *signalValues)
{
  if (signalValues == 0)
    return;

  const int colorCount = productionColorCount(layout);
  if ((colorIndex < 0) || (colorIndex >= colorCount))
    return;

  if (layout.colorModel == ELinColorModelDirectRgbOnly)
  {
    const LinPredefinedColor &color = kDirectRgbTestColors[colorIndex];
    signalValues->directRgbEnabled = true;
    signalValues->redOrPredefinedColor = color.red;
    signalValues->green = color.green;
    signalValues->blue = color.blue;
  }
  else
  {
    signalValues->directRgbEnabled = false;
    signalValues->redOrPredefinedColor = static_cast<quint8>(colorIndex);
  }
}

} // namespace

ProductionVerify::ProductionVerify(LinRuntime *runtime,
                                   QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ProductionVerify),
  linRuntime(runtime)
{
  Q_ASSERT(linRuntime != 0);
  backgroundframe  = new QFrame(this);
  backgroundframe->setStyleSheet(QFrameBasicQSS);
  backgroundframe->setGeometry(0,0,1366,768);
  backgroundframe->show();
  ui->setupUi(this);

  colorIndex = 0;
  selectedTargetMaskRecord = 0;

  colorChangeTimer = new QTimer(this);
  colorChangeTimer->setInterval(3000);
  /* A hidden optional page must not overwrite the active control command. */
  masterSignals = linRuntime->getBCMSignal();

  connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(closePage()));

  nodeMapper = new QSignalMapper((QObject*)this);
  const QList<QPushButton *> designedButtons = QList<QPushButton *>()
    << ui->pushButtonNode1 << ui->pushButtonNode2 << ui->pushButtonNode3
    << ui->pushButtonNode4 << ui->pushButtonNode5 << ui->pushButtonNode6
    << ui->pushButtonNode7 << ui->pushButtonNode11 << ui->pushButtonNode12;

  for (int index = 0; index < designedButtons.size(); ++index)
    designedButtons.at(index)->hide();

  QScrollArea *nodeArea = new QScrollArea(this);
  nodeArea->setGeometry(20, 90, 1326, 245);
  nodeArea->setWidgetResizable(true);
  nodeArea->setFrameShape(QFrame::NoFrame);
  nodeArea->setStyleSheet("QScrollArea{background:transparent;border:0px;}"
                          "QWidget{background:transparent;}");

  QWidget *nodeContainer = new QWidget(nodeArea);
  QGridLayout *nodeGrid = new QGridLayout(nodeContainer);
  nodeGrid->setContentsMargins(10, 8, 10, 8);
  nodeGrid->setHorizontalSpacing(12);
  nodeGrid->setVerticalSpacing(12);

  const LinLayout &profile = linRuntime->layout();
  const int testColorCount = productionColorCount(profile);
  if (profile.colorModel == ELinColorModelDirectRgbOnly)
  {
    ui->labelTitle->setText(
      "Production Verify (Direct RGB: Red/Green/Blue/White)");
  }
  else if (testColorCount > 0)
  {
    ui->labelTitle->setText(
      QString("Production Verify (Predefined colors 0-%1)")
        .arg(testColorCount - 1));
  }
  else
  {
    ui->labelTitle->setText("Production Verify (No test colors)");
  }

  const int maximumIntensity =
    static_cast<int>(profile.intensityMaximum);
  ui->spinBoxIntensity->setRange(0, maximumIntensity);
  ui->horizontalSliderIntensity->setRange(0, maximumIntensity);
  const bool fixedTargetMask =
    (profile.addressingModel == ELinAddressingModelFixedTargetMask);
  if (linRuntime->isLayoutValid())
  {
    const int columns = 7;
    for (int index = 0; index < profile.nodeCount; ++index)
    {
      const LinNodeLayout &node = profile.nodes[index];
      QPushButton *button = new QPushButton(nodeContainer);
      button->setText(QString("%1\nNAD 0x%2 / %3 0x%4")
        .arg(QString::fromLatin1(node.name))
        .arg(node.diagnosticNad, 2, 16, QChar('0'))
        .arg(fixedTargetMask ? QString("Mask") : QString("Address"))
        .arg(node.controlAddressMask, 4, 16, QChar('0'))
        .toUpper());
      button->setStyleSheet(buttonDisabled);
      button->setMinimumSize(160, 78);
      nodeMapper->setMapping(button, node.controlAddressMask);
      nodeButtons.insert(node.controlAddressMask, button);
      connect(button, SIGNAL(clicked()), nodeMapper, SLOT(map()));
      nodeGrid->addWidget(button,
                          node.uiSlot / columns,
                          node.uiSlot % columns);
    }
  }
  nodeArea->setWidget(nodeContainer);

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
  hide();
}

ProductionVerify::~ProductionVerify()
{
  delete backgroundframe;
  delete ui;
}

void ProductionVerify::init(void)
{
  masterSignals = linRuntime->getBCMSignal();
  const LinLayout &profile = linRuntime->layout();

  QMap<int, QPushButton *>::const_iterator button = nodeButtons.constBegin();
  while (button != nodeButtons.constEnd())
  {
    button.value()->setStyleSheet(buttonDisabled);
    ++button;
  }

  int selectedMask = masterSignals.targetMask;
  if (!nodeButtons.contains(selectedMask) && !nodeButtons.isEmpty())
  {
    selectedMask = profile.nodes[0].controlAddressMask;
    if (profile.addressingModel == ELinAddressingModelFixedTargetMask)
    {
      for (int index = 0; index < profile.nodeCount; ++index)
      {
        const quint16 nodeMask = profile.nodes[index].controlAddressMask;
        if ((masterSignals.targetMask & nodeMask) != 0)
        {
          selectedMask = nodeMask;
          break;
        }
      }
    }
  }

  const int availableColorCount = productionColorCount(profile);
  int initialColorIndex = 0;
  if ((profile.colorModel != ELinColorModelDirectRgbOnly) &&
      (availableColorCount > 0) &&
      (masterSignals.redOrPredefinedColor < availableColorCount))
  {
    initialColorIndex = masterSignals.redOrPredefinedColor;
  }
  if (availableColorCount > 0)
  {
    assignProductionColor(profile, initialColorIndex, &masterSignals);
    colorIndex = (initialColorIndex + 1) % availableColorCount;
  }

  if (primaryControlHasSignal(profile, ELinSignalLedEnable))
    masterSignals.ledEnabled = true;
  if (primaryControlHasSignal(profile, ELinSignalCommandValidity))
    masterSignals.commandValidity = true;
  if (primaryControlHasSignal(profile, ELinSignalFadingEnable))
    masterSignals.fadingEnabled = false;
  masterSignals.fadingTime = 0;
  masterSignals.groupAddressing = false;
  const quint32 requestedIntensity = profile.intensityMaximum < 100
    ? profile.intensityMaximum
    : 100;
  masterSignals.intensity = static_cast<quint8>(requestedIntensity);

  if (!nodeButtons.isEmpty())
    changeAddress(selectedMask);

  const bool spinSignalsBlocked = ui->spinBoxIntensity->blockSignals(true);
  const bool sliderSignalsBlocked =
    ui->horizontalSliderIntensity->blockSignals(true);
  ui->spinBoxIntensity->setValue(masterSignals.intensity);
  ui->horizontalSliderIntensity->setValue(masterSignals.intensity);
  ui->spinBoxIntensity->blockSignals(spinSignalsBlocked);
  ui->horizontalSliderIntensity->blockSignals(sliderSignalsBlocked);

  if (nodeButtons.isEmpty())
    linRuntime->setBCMSignal(masterSignals);
}

void ProductionVerify::intensityChanged(int intensity)
{
  const int maximumIntensity = static_cast<int>(
    linRuntime->layout().intensityMaximum);
  if (intensity < 0)
    intensity = 0;
  else if (intensity > maximumIntensity)
    intensity = maximumIntensity;
  masterSignals.intensity = static_cast<quint8>(intensity);
  linRuntime->setBCMSignal(masterSignals);
}

void ProductionVerify::changeColorManual(void)
{
  if (colorChangeTimer->isActive())
  {
    colorChangeTimer->stop();
    ui->pushButtonChangeColorAuto->setText("开始自动循环颜色");
  }

  changeColor();
}

void ProductionVerify::changeColorAuto(void)
{
  if (productionColorCount(linRuntime->layout()) <= 0)
    return;

  if (colorChangeTimer->isActive())
  {
    colorChangeTimer->stop();
    ui->pushButtonChangeColorAuto->setText("开始自动循环颜色");
  }
  else
  {
    colorChangeTimer->start();
    ui->pushButtonChangeColorAuto->setText("停止自动循环颜色");
  }
}

void ProductionVerify::changeColor(void)
{
  const LinLayout &profile = linRuntime->layout();
  const int availableColorCount = productionColorCount(profile);
  if (availableColorCount <= 0)
    return;

  colorIndex = colorIndex % availableColorCount;
  assignProductionColor(profile, colorIndex, &masterSignals);

  linRuntime->setBCMSignal(masterSignals);
  colorIndex++;
}

void ProductionVerify::changeAddress(int targetMask)
{
  if (nodeButtons.isEmpty())
    return;

  QMap<int, QPushButton *>::const_iterator button = nodeButtons.constBegin();
  while (button != nodeButtons.constEnd())
  {
    button.value()->setStyleSheet(buttonDisabled);
    ++button;
  }

  if (!nodeButtons.contains(targetMask))
    targetMask = linRuntime->layout().nodes[0].controlAddressMask;
  nodeButtons.value(targetMask)->setStyleSheet(buttonEnabled);

  masterSignals.targetMask = static_cast<quint16>(targetMask);
  masterSignals.groupAddressing = false;
  if (linRuntime->layout().colorModel == ELinColorModelDirectRgbOnly)
    masterSignals.directRgbEnabled = true;
  else
    masterSignals.directRgbEnabled = false;

  const bool requiresAtomicSwitch =
    (selectedTargetMaskRecord != targetMask) &&
    (selectedTargetMaskRecord != 0);

  selectedTargetMaskRecord = targetMask;

  if (requiresAtomicSwitch)
    linRuntime->switchBCMSignal(masterSignals);
  else
    linRuntime->setBCMSignal(masterSignals);
}

void ProductionVerify::closePage()
{
  colorChangeTimer->stop();
  hide();
}

void ProductionVerify::hideEvent(QHideEvent *event)
{
  colorChangeTimer->stop();
  ui->pushButtonChangeColorAuto->setText("开始自动循环颜色");
  QWidget::hideEvent(event);
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
