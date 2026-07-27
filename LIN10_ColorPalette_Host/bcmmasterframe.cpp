#include "bcmmasterframe.h"
#include "application_config.h"
#include "linlayout.h"
#include <QString>
#include <QPainter>
#include <QPen>
#include <QPushButton>

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

namespace {

quint16 allowedTargetMask(const LinLayout &layout)
{
  quint16 mask = 0;
  for (int index = 0; index < layout.nodeCount; ++index)
    mask = static_cast<quint16>(mask |
      layout.nodes[index].controlAddressMask);
  return mask;
}

quint16 firstSelectedNodeMask(const LinLayout &layout, quint16 targetMask)
{
  for (int index = 0; index < layout.nodeCount; ++index)
  {
    const quint16 nodeMask = layout.nodes[index].controlAddressMask;
    if ((targetMask & nodeMask) != 0)
      return nodeMask;
  }
  return 0;
}

int selectedTargetCount(const LinLayout &layout, quint16 targetMask)
{
  int count = 0;
  for (int index = 0; index < layout.nodeCount; ++index)
  {
    if ((targetMask & layout.nodes[index].controlAddressMask) != 0)
      ++count;
  }
  return count;
}

int primaryAddressMaximum(const LinLayout &layout)
{
  const LinSignalLayout *signal = findPrimaryControlSignal(
    layout, ELinSignalTargetMask);
  if ((signal == 0) || (signal->bitLength == 0))
    return 0;
  if (signal->bitLength >= 16)
    return 0xFFFF;
  return (1 << signal->bitLength) - 1;
}

} // namespace

BCMMasterFrame::BCMMasterFrame(AmbientLinScheduler *scheduler,
                               QWidget *parent):
  QWidget(parent),
  ui(new Ui::MasterFrame),
  linScheduler(scheduler),
  groupTargetMode(false),
  signalPresetMode(false)
{
  colorX = 0;
  colorY = 0;

  backgroundframe  = new QFrame(this);
  backgroundframe->setStyleSheet(QFrameBasicQSS);
  backgroundframe->setGeometry(0,0,1366,768);
  backgroundframe->show();

  ui->setupUi(this);
  ui->frameCIE->installEventFilter(this);

  const LinLayout &layout = linScheduler->layout();

  const bool fixedTargetMask =
    (layout.addressingModel == ELinAddressingModelFixedTargetMask);
  const int maximumAddress = fixedTargetMask
    ? allowedTargetMask(layout)
    : primaryAddressMaximum(layout);
  ui->spinBoxAdress->setRange(0, maximumAddress);
  ui->spinBoxAdress->setDisplayIntegerBase(16);
  ui->spinBoxAdress->setPrefix("0x");
  ui->horizontalSliderAdress->setRange(0, maximumAddress);
  ui->labelAddress->setText(fixedTargetMask
    ? QString("Target Mask:")
    : QString("Address Value:"));
  ui->labelAddressMode->setText(fixedTargetMask
    ? QString("Target Mode:")
    : QString("Address Mode:"));

  const bool fadeEnableAndTime =
    (layout.dimmingModel == ELinDimmingModelFadeEnableAndTime);
  const int maximumDimmingValue =
    static_cast<int>(layout.dimmingMaximum);
  ui->spinBoxDimRamp->setRange(0, maximumDimmingValue);
  ui->horizontalSliderDimRamp->setRange(0, maximumDimmingValue);
  ui->labelDimRamp->setText(fadeEnableAndTime
    ? QString("Fading Time:")
    : QString("Dimming Ramp:"));
  const int maximumIntensity =
    static_cast<int>(layout.intensityMaximum);
  ui->spinBoxIntensity->setRange(0, maximumIntensity);
  ui->verticalSliderIntensity->setRange(0, maximumIntensity);

  const bool directRgbSupported = supportsDirectRgb(layout);
  const bool predefinedColorsSupported = supportsPredefinedColors(layout);
  signalPresetMode = layout.signalPresetCount > 0;
  ui->RGBColor->setVisible(directRgbSupported);
  ui->RGBColor->setEnabled(directRgbSupported);
  ui->RGBColor->setText(QString("Direct RGB"));
  ui->RGBColor->setToolTip(directRgbSupported
    ? QString("Use byte 5..7 as direct RGB values.")
    : QString("Direct RGB is not supported by the active profile."));
  ui->PredefColor->setVisible(signalPresetMode || predefinedColorsSupported);
  ui->PredefColor->setToolTip(signalPresetMode
    ? QString("Apply a named combination of LDF signal values.")
    : QString("Use the predefined-color table from the active profile."));
  setDirectRgbControlsVisible(directRgbSupported);
  setDirectRgbControlsEnabled(false);
  setPredefinedColorControlsVisible(signalPresetMode ||
                                    predefinedColorsSupported);

  const bool hasLedEnable = primaryControlHasSignal(
    layout, ELinSignalLedEnable);
  const bool hasCommandValidity = primaryControlHasSignal(
    layout, ELinSignalCommandValidity);
  const bool hasOutputEnable = hasLedEnable || hasCommandValidity;
  ui->labelValidMarker->setVisible(hasOutputEnable);
  ui->labelValidMarker->setText(hasLedEnable
    ? QString("LED Enable")
    : QString("Command Valid"));
  ui->LED_Enable->setVisible(hasOutputEnable);
  ui->LED_Disable->setVisible(hasOutputEnable);

  const bool hasFadingEnable = primaryControlHasSignal(
    layout, ELinSignalFadingEnable);
  ui->labelRunMode->setVisible(hasFadingEnable);
  ui->Fading_Enable->setVisible(hasFadingEnable);
  ui->Fading_Disable->setVisible(hasFadingEnable);

  ui->pushButtonAwake->hide();
  ui->pushButtonSleep->show();
  ui->LinStart->hide();
  ui->LinStop->show();
  colorMapper = new QSignalMapper((QObject*)this);
  QPushButton *colorButtons[] = {
    ui->pushButtonColor1,  ui->pushButtonColor2,
    ui->pushButtonColor3,  ui->pushButtonColor4,
    ui->pushButtonColor5,  ui->pushButtonColor6,
    ui->pushButtonColor7,  ui->pushButtonColor8,
    ui->pushButtonColor9,  ui->pushButtonColor10,
    ui->pushButtonColor11, ui->pushButtonColor12,
    ui->pushButtonColor13, ui->pushButtonColor14,
    ui->pushButtonColor15, ui->pushButtonColor16,
    ui->pushButtonColor17, ui->pushButtonColor18,
    ui->pushButtonColor19, ui->pushButtonColor20,
    ui->pushButtonColor21, ui->pushButtonColor22,
    ui->pushButtonColor23, ui->pushButtonColor24,
    ui->pushButtonColor25, ui->pushButtonColor26,
    ui->pushButtonColor27, ui->pushButtonColor28,
    ui->pushButtonColor29, ui->pushButtonColor30
  };
  const int colorButtonCount = static_cast<int>(
    sizeof(colorButtons) / sizeof(colorButtons[0]));
  const int shortcutCount = signalPresetMode
    ? layout.signalPresetCount
    : layout.predefinedColorCount;
  const int visibleColorCount = qMin(colorButtonCount,
                                     qMax(0, shortcutCount));

  /*
   * Keep one deterministic, row-major palette.  Five columns preserve the
   * large 0..19 seed layout; a 21..30 color profile automatically switches
   * to six columns without requiring a .ui edit or paging buttons.
   */
  ui->groupBox1->setGeometry(690, 280, 661, 331);
  ui->groupBox2->hide();
  ui->pushButtonPrev->hide();
  ui->pushButtonNext->hide();
  const int paletteColumns = visibleColorCount > 20 ? 6 : 5;
  const int paletteRows = qMax(
    1, (visibleColorCount + paletteColumns - 1) / paletteColumns);
  const int paletteGap = 8;
  const int paletteLeft = 10;
  const int paletteTop = 20;
  const int paletteWidth = 641;
  const int paletteHeight = 301;
  const int colorButtonWidth =
    (paletteWidth - ((paletteColumns - 1) * paletteGap)) /
    paletteColumns;
  const int colorButtonHeight =
    (paletteHeight - ((paletteRows - 1) * paletteGap)) /
    paletteRows;
  for (int colorIndex = 0; colorIndex < colorButtonCount; ++colorIndex)
  {
    const bool shortcutIsVisible = (signalPresetMode ||
                                    predefinedColorsSupported) &&
                                   (colorIndex < shortcutCount);
    colorButtons[colorIndex]->setParent(ui->groupBox1);
    colorButtons[colorIndex]->setGeometry(
      paletteLeft +
        (colorIndex % paletteColumns) *
        (colorButtonWidth + paletteGap),
      paletteTop +
        (colorIndex / paletteColumns) *
        (colorButtonHeight + paletteGap),
      colorButtonWidth,
      colorButtonHeight);
    colorButtons[colorIndex]->setVisible(shortcutIsVisible);
    if (shortcutIsVisible)
    {
      colorMapper->setMapping(colorButtons[colorIndex], colorIndex);
      connect(colorButtons[colorIndex], SIGNAL(clicked()),
              colorMapper, SLOT(map()));
      if (signalPresetMode)
      {
        const LinSignalPreset &preset = layout.signalPresets[colorIndex];
        colorButtons[colorIndex]->setText(QString::fromUtf8(preset.name));
        colorButtons[colorIndex]->setToolTip(
          QString("Apply %1 signal values")
          .arg(preset.assignmentCount));
      }
      else
      {
        colorButtons[colorIndex]->setText(
          QString("#%1").arg(colorIndex));
        colorButtons[colorIndex]->setToolTip(
          QString("Predefined color %1").arg(colorIndex));
      }
    }
  }

  if (signalPresetMode)
  {
    ui->labelTitleColorTabel->setText(
      QString::fromUtf8(layout.signalPresetGroupName));
  }

  connect(colorMapper, SIGNAL(mapped(int)), this, SLOT(changeColor(int)));

  connect(ui->pushButtonAccept, SIGNAL(clicked()), this , SLOT(Accept()));
  connect(ui->pushButtonCancel, SIGNAL(clicked()), this , SLOT(Cancel()));
  connect(ui->spinBoxAdress, SIGNAL(valueChanged(int)), ui->horizontalSliderAdress, SLOT(setValue(int)));
  connect(ui->spinBoxAdress, SIGNAL(valueChanged(int)), this, SLOT(changeAddr(int)));
  connect(ui->horizontalSliderAdress, SIGNAL(valueChanged(int)), ui->spinBoxAdress, SLOT(setValue(int)));
  ui->spinBoxAdress->installEventFilter(this);

  connect(ui->spinBoxRed, SIGNAL(valueChanged(int)), ui->verticalSliderRed, SLOT(setValue(int)));
  connect(ui->spinBoxRed, SIGNAL(valueChanged(int)), this, SLOT(changeR(int)));
  connect(ui->spinBoxRed, SIGNAL(valueChanged(int)), this, SLOT(drawCIE()));
  connect(ui->verticalSliderRed, SIGNAL(valueChanged(int)), ui->spinBoxRed, SLOT(setValue(int)));
  ui->spinBoxRed->installEventFilter(this);

  connect(ui->spinBoxGreen, SIGNAL(valueChanged(int)), ui->verticalSliderGreen, SLOT(setValue(int)));
  connect(ui->spinBoxGreen, SIGNAL(valueChanged(int)), this, SLOT(changeG(int)));
  connect(ui->spinBoxGreen, SIGNAL(valueChanged(int)), this, SLOT(drawCIE()));
  connect(ui->verticalSliderGreen, SIGNAL(valueChanged(int)), ui->spinBoxGreen, SLOT(setValue(int)));
  ui->spinBoxGreen->installEventFilter(this);

  connect(ui->spinBoxBlue, SIGNAL(valueChanged(int)), ui->verticalSliderBlue, SLOT(setValue(int)));
  connect(ui->spinBoxBlue, SIGNAL(valueChanged(int)), this, SLOT(changeB(int)));
  connect(ui->spinBoxBlue, SIGNAL(valueChanged(int)), this, SLOT(drawCIE()));
  connect(ui->verticalSliderBlue, SIGNAL(valueChanged(int)), ui->spinBoxBlue, SLOT(setValue(int)));
  ui->spinBoxBlue->installEventFilter(this);

  connect(ui->spinBoxIntensity, SIGNAL(valueChanged(int)), ui->verticalSliderIntensity, SLOT(setValue(int)));
  connect(ui->spinBoxIntensity, SIGNAL(valueChanged(int)), this, SLOT(changeI(int)));
  connect(ui->verticalSliderIntensity, SIGNAL(valueChanged(int)), ui->spinBoxIntensity, SLOT(setValue(int)));
  ui->spinBoxIntensity->installEventFilter(this);

  connect(ui->spinBoxDimRamp, SIGNAL(valueChanged(int)), ui->horizontalSliderDimRamp, SLOT(setValue(int)));
  connect(ui->spinBoxDimRamp, SIGNAL(valueChanged(int)), this, SLOT(changeDimRamp(int)));
  connect(ui->horizontalSliderDimRamp, SIGNAL(valueChanged(int)), ui->spinBoxDimRamp, SLOT(setValue(int)));
  ui->spinBoxDimRamp->installEventFilter(this);

  connect(ui->pushButtonGroup, SIGNAL(clicked()), this, SLOT(setGroup()));
  connect(ui->pushButtonSingle, SIGNAL(clicked()), this, SLOT(setSingle()));

  ui->pushButtonAccept->setStyleSheet(PushButtonEnabledQSS);

  this->init();
  this->hide();

  keys = new KeyBoard(this);
  keys->hide();
}

BCMMasterFrame::~BCMMasterFrame()
{
  delete ui;
}

void BCMMasterFrame::init()
{
  BCMSignal currentSig = linScheduler->getBCMSignal();
  masterSignals = currentSig;

  const LinLayout &layout = linScheduler->layout();
  if (layout.addressingModel == ELinAddressingModelFixedTargetMask)
  {
    masterSignals.targetMask = static_cast<quint16>(
      masterSignals.targetMask & allowedTargetMask(layout));
  }
  else
  {
    masterSignals.targetMask = static_cast<quint16>(
      masterSignals.targetMask & primaryAddressMaximum(layout));
  }

  if (layout.colorModel == ELinColorModelDirectRgbOnly)
    masterSignals.directRgbEnabled = true;
  else if (layout.colorModel == ELinColorModelPredefinedOnly)
    masterSignals.directRgbEnabled = false;
  else if (masterSignals.directRgbEnabled && !supportsDirectRgb(layout))
    masterSignals.directRgbEnabled = false;
  else if (!masterSignals.directRgbEnabled &&
           !supportsPredefinedColors(layout))
    masterSignals.directRgbEnabled = true;

  if (!masterSignals.directRgbEnabled &&
      (masterSignals.redOrPredefinedColor >= layout.predefinedColorCount))
    masterSignals.redOrPredefinedColor = 0;

  if (layout.addressingModel == ELinAddressingModelAddressValueAndGroupBit)
  {
    if (masterSignals.groupAddressing)
      setGroup();
    else
      setSingle();
  }
  else if (selectedTargetCount(layout, masterSignals.targetMask) > 1)
    setGroup();
  else
    setSingle();

  ui->spinBoxAdress->setValue(masterSignals.targetMask);
  if (masterSignals.directRgbEnabled)
  {
    ui->spinBoxRed->setValue(masterSignals.redOrPredefinedColor);
    ui->spinBoxGreen->setValue(masterSignals.green);
    ui->spinBoxBlue->setValue(masterSignals.blue);
  }
  else if ((layout.predefinedColors != 0) &&
           (layout.predefinedColorCount > 0))
  {
    const LinPredefinedColor &color =
      layout.predefinedColors[masterSignals.redOrPredefinedColor];
    ui->spinBoxRed->setValue(color.red);
    ui->spinBoxGreen->setValue(color.green);
    ui->spinBoxBlue->setValue(color.blue);
  }
  setDirectRgbControlsEnabled(masterSignals.directRgbEnabled &&
                               supportsDirectRgb(layout));
  ui->spinBoxIntensity->setValue(masterSignals.intensity);
  ui->spinBoxDimRamp->setValue(masterSignals.fadingTime);
  if (supportsPredefinedColors(layout))
  {
    ui->PredefColor->setText(
      QString("Color #%1  M0")
        .arg(masterSignals.redOrPredefinedColor));
  }
}

static const double  DoubleRedx = 0.69;//0.69;
static const double  DoubleRedy = 0.31;//0.31;
static const double  DoubleRedz = (1 - DoubleRedx - DoubleRedy);
static const double  DoubleRedY = 1.4;//1.4;

static const double  DoubleGreenx = 0.17;//0.17;
static const double  DoubleGreeny = 0.72;//0.72;
static const double  DoubleGreenz = (1 - DoubleGreenx - DoubleGreeny);
static const double  DoubleGreenY = 2.2;//2.2;

static const double  DoubleBluex = 0.15;//0.15;
static const double  DoubleBluey = 0.03;//0.03;
static const double  DoubleBluez = (1 - DoubleBluex - DoubleBluey);
static const double  DoubleBlueY = 0.5;

void BCMMasterFrame::drawCIE()
{
  int r,g,b;
  QString content;

  double m1 = 0, m2 = 0, m3 = 0, sum_m = 0;
  double x1m1, x2m2, x3m3, sum_xm;
  double y1m1, y2m2, y3m3, sum_ym;
  double mY1, mY2, mY3;

  r = ui->spinBoxRed->value();
  g = ui->spinBoxGreen->value();
  b = ui->spinBoxBlue->value();

  if ((r == 0) && (g == 0) && (b == 0))
  {
    colorX = 0;
    colorY = 0;
  }
  else
  {
    mY1 = DoubleRedY*(double)r;
    mY2 = DoubleGreenY*(double)g;
    mY3 = DoubleBlueY*(double)b;

    m1 = mY1 / DoubleRedy;
    m2 = mY2 / DoubleGreeny;
    m3 = mY3 / DoubleBluey;

    sum_m = m1 + m2 + m3;

    x1m1 = m1*DoubleRedx;
    x2m2 = m2*DoubleGreenx;
    x3m3 = m3*DoubleBluex;

    sum_xm = x1m1 + x2m2 + x3m3;

    y1m1 = m1*DoubleRedy;
    y2m2 = m2*DoubleGreeny;
    y3m3 = m3*DoubleBluey;

    sum_ym = y1m1 + y2m2 + y3m3;

    colorX = sum_xm / sum_m;
    colorY = sum_ym / sum_m;
  }

  content = "X: ";
  content.append(QString::number(colorX, 'f', 4));
  ui->labelX->setText(content);
  content = "Y: ";
  content.append(QString::number(colorY, 'f', 4));
  ui->labelY->setText(content);

  ui->frameCIE->repaint();
}

bool BCMMasterFrame::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == ui->frameCIE && event->type() == QEvent::Paint)
  {
    QPainter painter(ui->frameCIE);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.drawEllipse(18 + (int)(colorX / 0.003053) - 4, 301 - (int)(colorY/0.003053) - 4, 8, 8);
  }

  if (event->type() == QEvent::FocusIn)
  {
    if (watched->inherits("QSpinBox"))
      showInputKeyBoard(true);
  }

  return QWidget::eventFilter(watched, event);
}


void BCMMasterFrame::updateSignals()
{
  linScheduler->setBCMSignal(masterSignals);
}

void BCMMasterFrame::setDirectRgbControlsEnabled(bool enabled)
{
  ui->spinBoxRed->setEnabled(enabled);
  ui->spinBoxGreen->setEnabled(enabled);
  ui->spinBoxBlue->setEnabled(enabled);
  ui->verticalSliderRed->setEnabled(enabled);
  ui->verticalSliderGreen->setEnabled(enabled);
  ui->verticalSliderBlue->setEnabled(enabled);
}

void BCMMasterFrame::setDirectRgbControlsVisible(bool visible)
{
  ui->frameCIE->setVisible(visible);
  ui->labelX->setVisible(visible);
  ui->labelY->setVisible(visible);
  ui->labelRed->setVisible(visible);
  ui->labelGreen->setVisible(visible);
  ui->labelBlue->setVisible(visible);
  ui->spinBoxRed->setVisible(visible);
  ui->spinBoxGreen->setVisible(visible);
  ui->spinBoxBlue->setVisible(visible);
  ui->verticalSliderRed->setVisible(visible);
  ui->verticalSliderGreen->setVisible(visible);
  ui->verticalSliderBlue->setVisible(visible);
}

void BCMMasterFrame::setPredefinedColorControlsVisible(bool visible)
{
  ui->labelTitleColorTabel->setVisible(visible);
  ui->groupBox1->setVisible(visible &&
    ((linScheduler->layout().signalPresetCount > 0) ||
     (linScheduler->layout().predefinedColorCount > 0)));
  ui->groupBox2->hide();
}

void BCMMasterFrame::setGroup()
{
  ui->pushButtonGroup->setStyleSheet(PushButtonEnabledQSS);
  ui->pushButtonSingle->setStyleSheet(PushButtonDisabledQSS);
  groupTargetMode = true;
  masterSignals.groupAddressing = true;
  updateSignals();
}

void BCMMasterFrame::setSingle()
{
  ui->pushButtonGroup->setStyleSheet(PushButtonDisabledQSS);
  ui->pushButtonSingle->setStyleSheet(PushButtonEnabledQSS);
  groupTargetMode = false;
  masterSignals.groupAddressing = false;

  const LinLayout &layout = linScheduler->layout();
  if (layout.addressingModel == ELinAddressingModelFixedTargetMask)
  {
    const quint16 singleMask = firstSelectedNodeMask(
      layout, masterSignals.targetMask);
    if (singleMask != masterSignals.targetMask)
    {
      masterSignals.targetMask = singleMask;
      ui->spinBoxAdress->setValue(singleMask);
    }
  }
  updateSignals();
}

void BCMMasterFrame::changeAddr(int value)
{
  const LinLayout &layout = linScheduler->layout();
  quint16 targetMask = static_cast<quint16>(value);
  if (layout.addressingModel == ELinAddressingModelFixedTargetMask)
  {
    targetMask = static_cast<quint16>(targetMask &
                                      allowedTargetMask(layout));
    if (!groupTargetMode)
      targetMask = firstSelectedNodeMask(layout, targetMask);
  }
  else
  {
    targetMask = static_cast<quint16>(
      targetMask & primaryAddressMaximum(layout));
  }

  if (targetMask != static_cast<quint16>(value))
  {
    ui->spinBoxAdress->setValue(targetMask);
    return;
  }

  masterSignals.targetMask = targetMask;
  updateSignals();
}

void BCMMasterFrame::changeR(int r)
{
    if(masterSignals.directRgbEnabled)
    {
        masterSignals.redOrPredefinedColor = static_cast<quint8>(r);
        updateSignals();
    }

}

void BCMMasterFrame::changeG(int g)
{
    if(masterSignals.directRgbEnabled)
    {
        masterSignals.green = static_cast<quint8>(g);
        updateSignals();
    }

}

void BCMMasterFrame::changeB(int b)
{
  if(masterSignals.directRgbEnabled)
  {
      masterSignals.blue = static_cast<quint8>(b);
      updateSignals();
  }
}

void BCMMasterFrame::changeI(int i)
{
  masterSignals.intensity = static_cast<quint8>(i);
  updateSignals();
}

void BCMMasterFrame::changeDimRamp(int dimramp)
{
  masterSignals.fadingTime = static_cast<quint8>(dimramp);
  updateSignals();
}

void BCMMasterFrame::Accept()
{
  updateSignals();
}

void BCMMasterFrame::Cancel()
{
  this->hide();
}

void BCMMasterFrame::showInputKeyBoard(bool show)
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

void BCMMasterFrame::changeColor(int i)
{
  const LinLayout &layout = linScheduler->layout();
  if (signalPresetMode)
  {
    if ((i < 0) || (i >= layout.signalPresetCount) ||
        (layout.signalPresets == 0))
      return;

    ui->PredefColor->setText(
      QString("Preset: %1")
      .arg(QString::fromUtf8(layout.signalPresets[i].name)));
    linScheduler->applySignalPreset(i);
    return;
  }

  if (!supportsPredefinedColors(layout) ||
      (layout.colorModel == ELinColorModelDirectRgbOnly) ||
      (i < 0) || (i >= layout.predefinedColorCount) ||
      (layout.predefinedColors == 0))
    return;

  masterSignals.directRgbEnabled = false;
  masterSignals.redOrPredefinedColor = static_cast<quint8>(i);
  ui->spinBoxRed->setValue(layout.predefinedColors[i].red);
  ui->spinBoxGreen->setValue(layout.predefinedColors[i].green);
  ui->spinBoxBlue->setValue(layout.predefinedColors[i].blue);
  setDirectRgbControlsEnabled(false);
  ui->PredefColor->setText(QString("Color #%1  M0").arg(i));
  updateSignals();
}

void BCMMasterFrame::on_pushButtonSleep_clicked()
{

        linScheduler->sleepBus();
        ui->pushButtonSleep->hide();
        ui->pushButtonAwake->show();

}

void BCMMasterFrame::on_pushButtonAwake_clicked()
{
    linScheduler->wakeBus();
    ui->pushButtonAwake->hide();
    ui->pushButtonSleep->show();
}



void BCMMasterFrame::on_LinStart_clicked()
{
    linScheduler->setBusEnabled(true);
    ui->LinStart->hide();
    ui->LinStop->show();
}

void BCMMasterFrame::on_LinStop_clicked()
{
    linScheduler->setBusEnabled(false);
    ui->LinStop->hide();
    ui->LinStart->show();
}



void BCMMasterFrame::on_PredefColor_clicked()
{
    const LinLayout &layout = linScheduler->layout();
    if (layout.colorModel == ELinColorModelDirectRgbOnly)
    {
      masterSignals.directRgbEnabled = true;
    }
    else
    {
      const bool wasDirectRgb = masterSignals.directRgbEnabled;
      masterSignals.directRgbEnabled = false;
      if (wasDirectRgb ||
          (masterSignals.redOrPredefinedColor >=
           layout.predefinedColorCount))
      {
        masterSignals.redOrPredefinedColor = 0;
      }

      if ((layout.predefinedColors != 0) &&
          (layout.predefinedColorCount > 0))
      {
        const int colorIndex = masterSignals.redOrPredefinedColor;
        const LinPredefinedColor &color =
          layout.predefinedColors[colorIndex];
        ui->spinBoxRed->setValue(color.red);
        ui->spinBoxGreen->setValue(color.green);
        ui->spinBoxBlue->setValue(color.blue);
        ui->PredefColor->setText(
          QString("Color #%1  M0").arg(colorIndex));
      }
    }
    setDirectRgbControlsEnabled(masterSignals.directRgbEnabled &&
                                supportsDirectRgb(layout));
    updateSignals();
}

void BCMMasterFrame::on_RGBColor_clicked()
{
    const LinLayout &layout = linScheduler->layout();
    const bool useDirectRgb =
      (layout.colorModel != ELinColorModelPredefinedOnly) &&
      supportsDirectRgb(layout);
    if (useDirectRgb && !masterSignals.directRgbEnabled)
    {
      /* Preserve the color currently shown when changing representation. */
      masterSignals.redOrPredefinedColor =
        static_cast<quint8>(ui->spinBoxRed->value());
      masterSignals.green =
        static_cast<quint8>(ui->spinBoxGreen->value());
      masterSignals.blue =
        static_cast<quint8>(ui->spinBoxBlue->value());
    }
    masterSignals.directRgbEnabled = useDirectRgb;
    setDirectRgbControlsEnabled(masterSignals.directRgbEnabled);
    updateSignals();
}

void BCMMasterFrame::on_LED_Enable_clicked()
{
    const LinLayout &layout = linScheduler->layout();
    const bool hasLedEnable = primaryControlHasSignal(
      layout, ELinSignalLedEnable);
    const bool hasCommandValidity = primaryControlHasSignal(
      layout, ELinSignalCommandValidity);
    if (!hasLedEnable && !hasCommandValidity)
      return;

    if (hasLedEnable)
      masterSignals.ledEnabled = true;
    if (hasCommandValidity)
      masterSignals.commandValidity = true;

    /*
     * Intensity 0 is a valid protocol value, but it necessarily produces a
     * black output.  Use the layout default when the user asks to turn the
     * lamp on without first setting a brightness.
     */
    if (hasLedEnable && (masterSignals.intensity == 0))
    {
      quint8 visibleIntensity =
        createDefaultBCMSignal(linScheduler->layout()).intensity;
      if (visibleIntensity == 0)
      {
        visibleIntensity = layout.intensityMaximum < 100
          ? static_cast<quint8>(layout.intensityMaximum)
          : static_cast<quint8>(100);
      }

      masterSignals.intensity = visibleIntensity;

      /* Update both controls without producing an intermediate LIN update. */
      const bool spinSignalsBlocked = ui->spinBoxIntensity->blockSignals(true);
      const bool sliderSignalsBlocked =
        ui->verticalSliderIntensity->blockSignals(true);
      ui->spinBoxIntensity->setValue(visibleIntensity);
      ui->verticalSliderIntensity->setValue(visibleIntensity);
      ui->spinBoxIntensity->blockSignals(spinSignalsBlocked);
      ui->verticalSliderIntensity->blockSignals(sliderSignalsBlocked);
    }

    updateSignals();
}

void BCMMasterFrame::on_LED_Disable_clicked()
{
    const LinLayout &layout = linScheduler->layout();
    const bool hasLedEnable = primaryControlHasSignal(
      layout, ELinSignalLedEnable);
    const bool hasCommandValidity = primaryControlHasSignal(
      layout, ELinSignalCommandValidity);
    if (!hasLedEnable && !hasCommandValidity)
      return;
    if (hasLedEnable)
      masterSignals.ledEnabled = false;
    if (hasCommandValidity)
      masterSignals.commandValidity = false;
    updateSignals();
}

void BCMMasterFrame::on_Fading_Enable_clicked()
{
    masterSignals.fadingEnabled = true;
    linScheduler->setBCMSignal(masterSignals);
}

void BCMMasterFrame::on_Fading_Disable_clicked()
{
    masterSignals.fadingEnabled = false;
    linScheduler->setBCMSignal(masterSignals);
}
