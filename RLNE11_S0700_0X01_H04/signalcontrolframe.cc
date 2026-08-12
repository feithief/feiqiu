#include "signalcontrolframe.h"

#include "linlayout.h"
#include "linruntime.h"
#include "ui_signalcontrolframe.h"

#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

namespace {

quint64 maximumValue(quint8 bitLength)
{
  if (bitLength >= 32)
    return 0xFFFFFFFFULL;
  return (static_cast<quint64>(1) << bitLength) - 1;
}

quint64 logicalValue(const BCMSignal &values,
                     const LinSignalLayout &layout)
{
  const LinLogicalSignal signal = layout.signal;
  switch (signal)
  {
    case ELinSignalTargetMask:           return values.targetMask;
    case ELinSignalDirectRgbEnable:      return values.directRgbEnabled ? 1 : 0;
    case ELinSignalLedEnable:            return values.ledEnabled ? 1 : 0;
    case ELinSignalFadingEnable:         return values.fadingEnabled ? 1 : 0;
    case ELinSignalFadingTime:           return values.fadingTime;
    case ELinSignalIntensity:            return values.intensity;
    case ELinSignalRedOrPredefinedColor: return values.redOrPredefinedColor;
    case ELinSignalGreen:                return values.green;
    case ELinSignalBlue:                 return values.blue;
    case ELinSignalGroupAddressing:      return values.groupAddressing ? 1 : 0;
    case ELinSignalIntensityNormalization:
      return values.intensityNormalization ? 1 : 0;
    case ELinSignalDimmingTimeRelative:
      return values.dimmingTimeRelative ? 1 : 0;
    case ELinSignalDimmingTimeBase250ms:
      return values.dimmingTimeBase250ms ? 1 : 0;
    case ELinSignalSpecialFunction:      return values.specialFunction;
    case ELinSignalCommandValidity:      return values.commandValidity ? 1 : 0;
    case ELinSignalRawValue:
      return values.rawSignalValues.value(
        QString::fromLatin1(layout.name), layout.defaultValue);
  }
  return 0;
}

bool assignLogicalValue(BCMSignal *values,
                        const LinSignalLayout &layout,
                        quint64 value)
{
  if (values == 0)
    return false;

  switch (layout.signal)
  {
    case ELinSignalTargetMask:
      values->targetMask = static_cast<quint16>(value);
      break;
    case ELinSignalDirectRgbEnable:
      values->directRgbEnabled = (value != 0);
      break;
    case ELinSignalLedEnable:
      values->ledEnabled = (value != 0);
      break;
    case ELinSignalFadingEnable:
      values->fadingEnabled = (value != 0);
      break;
    case ELinSignalFadingTime:
      values->fadingTime = static_cast<quint8>(value);
      break;
    case ELinSignalIntensity:
      values->intensity = static_cast<quint8>(value);
      break;
    case ELinSignalRedOrPredefinedColor:
      values->redOrPredefinedColor = static_cast<quint8>(value);
      break;
    case ELinSignalGreen:
      values->green = static_cast<quint8>(value);
      break;
    case ELinSignalBlue:
      values->blue = static_cast<quint8>(value);
      break;
    case ELinSignalGroupAddressing:
      values->groupAddressing = (value != 0);
      break;
    case ELinSignalIntensityNormalization:
      values->intensityNormalization = (value != 0);
      break;
    case ELinSignalDimmingTimeRelative:
      values->dimmingTimeRelative = (value != 0);
      break;
    case ELinSignalDimmingTimeBase250ms:
      values->dimmingTimeBase250ms = (value != 0);
      break;
    case ELinSignalSpecialFunction:
      values->specialFunction = static_cast<quint8>(value);
      break;
    case ELinSignalCommandValidity:
      values->commandValidity = (value != 0);
      break;
    case ELinSignalRawValue:
      values->rawSignalValues.insert(
        QString::fromLatin1(layout.name), static_cast<quint32>(value));
      break;
    default:
      return false;
  }
  return true;
}

QLabel *columnLabel(const QString &text, int width, QWidget *parent)
{
  QLabel *label = new QLabel(text, parent);
  label->setFixedWidth(width);
  label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
  return label;
}

} // namespace

SignalControlFrame::SignalControlFrame(LinRuntime *runtime,
                                       QWidget *parent)
  : QWidget(parent),
    ui(new Ui::SignalControlFrame),
    linRuntime(runtime),
    frameLayout(0),
    selectedFrameIndex(-1)
{
  Q_ASSERT(linRuntime != 0);

  ui->setupUi(this);
  connect(ui->frameSelector,
          static_cast<void (QComboBox::*)(int)>(
            &QComboBox::currentIndexChanged),
          this,
          &SignalControlFrame::selectFrame);
  populateFrameSelector();
  buildSignalRows();

  connect(ui->reloadButton, &QPushButton::clicked,
          this, &SignalControlFrame::loadCurrentValues);
  connect(ui->applyButton, &QPushButton::clicked,
          this, &SignalControlFrame::applyValues);
  connect(ui->exitButton, &QPushButton::clicked,
          this, &SignalControlFrame::closePage);

  hide();
}

SignalControlFrame::~SignalControlFrame()
{
  delete ui;
}

void SignalControlFrame::populateFrameSelector()
{
  const LinLayout &layout = linRuntime->layout();
  const bool wasBlocked = ui->frameSelector->blockSignals(true);
  ui->frameSelector->clear();

  for (int frameIndex = 0;
       frameIndex < layout.publishedFrameCount;
       ++frameIndex)
  {
    const LinFrameLayout &frame = layout.publishedFrames[frameIndex];
    ui->frameSelector->addItem(
      QString::fromUtf8("%1  |  ID %2  |  %3 字节  |  %4 个信号")
        .arg(QString::fromLatin1(frame.name))
        .arg(static_cast<int>(frame.frameId))
        .arg(static_cast<int>(frame.length))
        .arg(frame.signalCount),
      frameIndex);
  }

  selectedFrameIndex = -1;
  if (layout.publishedFrameCount > 0)
  {
    const int preferredFrame =
      (layout.primaryControlFrameIndex >= 0) &&
      (layout.primaryControlFrameIndex < layout.publishedFrameCount)
        ? layout.primaryControlFrameIndex
        : 0;
    const int comboIndex = ui->frameSelector->findData(preferredFrame);
    ui->frameSelector->setCurrentIndex(comboIndex >= 0 ? comboIndex : 0);
    selectedFrameIndex = ui->frameSelector->currentData().toInt();
  }

  ui->frameSelector->blockSignals(wasBlocked);
}

void SignalControlFrame::selectFrame(int comboIndex)
{
  bool validIndex = false;
  const int frameIndex = ui->frameSelector->itemData(comboIndex)
                           .toInt(&validIndex);
  const LinLayout &layout = linRuntime->layout();
  selectedFrameIndex = validIndex &&
                       (frameIndex >= 0) &&
                       (frameIndex < layout.publishedFrameCount)
                         ? frameIndex
                         : -1;
  buildSignalRows();
}

void SignalControlFrame::buildSignalRows()
{
  const LinLayout &layout = linRuntime->layout();
  frameLayout = (selectedFrameIndex >= 0) &&
                (selectedFrameIndex < layout.publishedFrameCount)
                  ? &layout.publishedFrames[selectedFrameIndex]
                  : 0;
  controlledSignals.clear();
  valueEditors.clear();

  while (QLayoutItem *item = ui->verticalLayoutSignals->takeAt(0))
  {
    delete item->widget();
    delete item;
  }

  if (frameLayout != 0)
  {
    ui->frameInformation->setText(
      QString::fromUtf8("当前报文：%1    信号：%2")
        .arg(QString::fromLatin1(frameLayout->name))
        .arg(frameLayout->signalCount));
  }
  else
  {
    ui->frameInformation->setText(
      QString::fromUtf8("当前布局没有可控制的发送报文"));
  }

  if ((frameLayout == 0) || (frameLayout->signalCount <= 0))
  {
    QLabel *empty = new QLabel(
      QString::fromUtf8("当前报文中没有可控制信号。"),
      ui->signalScrollContent);
    empty->setAlignment(Qt::AlignCenter);
    ui->verticalLayoutSignals->addWidget(empty);
    ui->verticalLayoutSignals->addStretch();
    return;
  }

  QWidget *gridHost = new QWidget(ui->signalScrollContent);
  QGridLayout *gridLayout = new QGridLayout(gridHost);
  gridLayout->setContentsMargins(2, 2, 2, 2);
  gridLayout->setHorizontalSpacing(7);
  gridLayout->setVerticalSpacing(10);
  const int columnCount = qMax(1, qMin(11, frameLayout->signalCount));

  for (int signalIndex = 0;
       signalIndex < frameLayout->signalCount;
       ++signalIndex)
  {
    const LinSignalLayout &signal = frameLayout->signalLayouts[signalIndex];
    const quint64 signalMaximum = maximumValue(signal.bitLength);

    QFrame *card = new QFrame(gridHost);
    card->setMinimumWidth(100);
    card->setFixedHeight(390);
    card->setStyleSheet(
      "QFrame{border:1px solid rgba(15,186,205,165);"
      "border-radius:8px;background:rgba(2,22,31,235);}"
      "QLabel{border:0;background:transparent;color:rgb(166,231,247);"
      "font-size:13px;}"
      "QSpinBox,QLineEdit{border:1px solid #0fbacd;"
      "background:rgb(2,22,31);color:white;font-size:18px;}");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(4, 5, 4, 5);
    cardLayout->setSpacing(3);

    QLabel *name = new QLabel(QString::fromLatin1(signal.name), card);
    name->setToolTip(QString::fromLatin1(signal.name));
    name->setWordWrap(true);
    name->setFixedHeight(60);
    name->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(name);

    QLabel *details = new QLabel(
      QString::fromUtf8("范围 0 - %1").arg(signalMaximum), card);
    details->setToolTip(
      QString::fromUtf8("起始位 %1 / 长度 %2 bit / 十进制范围 0 - %3")
        .arg(static_cast<int>(signal.startBit))
        .arg(static_cast<int>(signal.bitLength))
        .arg(signalMaximum));
    details->setFixedHeight(24);
    details->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(details);

    QWidget *editor = 0;
    QSlider *slider = new QSlider(Qt::Vertical, card);
    slider->setObjectName(
      QString("signalSlider_%1").arg(valueEditors.size()));
    slider->setFixedSize(46, 245);
    slider->setSingleStep(1);
    slider->setStyleSheet(
      "QSlider{border:0;background:transparent;}"
      "QSlider::sub-page:vertical{background-color:rgba(87,97,106,0);"
      "width:40px;}"
      "QSlider::add-page:vertical{"
      "background-color:qlineargradient(spread:pad,x1:0,y1:1,x2:0,y2:0,"
      "stop:0 rgba(29,165,219,140),stop:1 rgba(29,165,219,210));"
      "width:40px;}"
      "QSlider::groove:vertical{background:transparent;width:46px;}"
      "QSlider::handle:vertical{background-color:rgb(29,165,219);"
      "width:40px;height:30px;}");
    cardLayout->addWidget(slider, 0, Qt::AlignHCenter);

    if (signal.bitLength <= 30)
    {
      const int maximum = static_cast<int>(signalMaximum);
      QSpinBox *spinBox = new QSpinBox(card);
      spinBox->setFixedSize(92, 38);
      spinBox->setRange(0, maximum);
      spinBox->setDisplayIntegerBase(10);
      spinBox->setAlignment(Qt::AlignCenter);
      spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
      slider->setRange(0, maximum);
      slider->setPageStep(qMax(1, maximum / 20));
      connect(slider, &QSlider::valueChanged,
              spinBox, &QSpinBox::setValue);
      connect(spinBox,
              static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
              slider, &QSlider::setValue);
      editor = spinBox;
    }
    else
    {
      QLineEdit *lineEdit = new QLineEdit(card);
      lineEdit->setFixedSize(92, 38);
      lineEdit->setAlignment(Qt::AlignCenter);
      lineEdit->setMaxLength(10);
      lineEdit->setValidator(
        new QRegExpValidator(QRegExp("[0-9]{1,10}"), lineEdit));
      lineEdit->setToolTip(
        QString("Decimal range: 0 - %1").arg(signalMaximum));
      slider->setRange(0, 1000);
      slider->setPageStep(50);
      connect(slider, &QSlider::sliderMoved, lineEdit,
              [lineEdit, signalMaximum](int position) {
        const quint64 value =
          (signalMaximum * static_cast<quint64>(position)) / 1000ULL;
        lineEdit->setText(QString::number(value));
      });
      connect(lineEdit, &QLineEdit::textChanged, slider,
              [slider, signalMaximum](const QString &text) {
        bool valid = false;
        quint64 value = text.toULongLong(&valid, 10);
        if (!valid)
          return;
        if (value > signalMaximum)
          value = signalMaximum;
        const int position = signalMaximum == 0
          ? 0
          : static_cast<int>((value * 1000ULL) / signalMaximum);
        const bool wasBlocked = slider->blockSignals(true);
        slider->setValue(position);
        slider->blockSignals(wasBlocked);
      });
      editor = lineEdit;
    }

    editor->setObjectName(
      QString("signalValue_%1").arg(valueEditors.size()));
    cardLayout->addWidget(editor, 0, Qt::AlignHCenter);

    controlledSignals.append(&signal);
    valueEditors.append(editor);
    gridLayout->addWidget(card,
                          signalIndex / columnCount,
                          signalIndex % columnCount);
  }

  ui->verticalLayoutSignals->addWidget(gridHost);
  ui->verticalLayoutSignals->addStretch();
  loadCurrentValues();
}
void SignalControlFrame::init()
{
  loadCurrentValues();
}

void SignalControlFrame::applyCurrentFrame()
{
  applyValues();
}

void SignalControlFrame::loadCurrentValues()
{
  if ((frameLayout == 0) ||
      (valueEditors.size() != controlledSignals.size()))
  {
    ui->statusLabel->setText(
      QString::fromUtf8("读取失败：当前报文布局无效"));
    return;
  }

  const BCMSignal values = linRuntime->getBCMSignal();
  for (int index = 0; index < controlledSignals.size(); ++index)
  {
    const quint64 value = logicalValue(values, *controlledSignals[index]);
    QWidget *editor = valueEditors[index];
    if (QCheckBox *checkBox = qobject_cast<QCheckBox *>(editor))
      checkBox->setChecked(value != 0);
    else if (QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor))
      spinBox->setValue(static_cast<int>(value));
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
      lineEdit->setText(QString::number(value));
  }

  ui->statusLabel->setText(
    QString::fromUtf8("已读取当前报文的 %1 个信号状态")
      .arg(controlledSignals.size()));
}

void SignalControlFrame::applyValues()
{
  if ((frameLayout == 0) ||
      (valueEditors.size() != controlledSignals.size()))
  {
    ui->statusLabel->setText(
      QString::fromUtf8("应用失败：当前报文布局无效"));
    return;
  }

  BCMSignal nextValues = linRuntime->getBCMSignal();
  for (int index = 0; index < controlledSignals.size(); ++index)
  {
    const LinSignalLayout &signal = *controlledSignals[index];
    QWidget *editor = valueEditors[index];
    quint64 value = 0;
    bool valid = true;

    if (QCheckBox *checkBox = qobject_cast<QCheckBox *>(editor))
    {
      value = checkBox->isChecked() ? 1 : 0;
    }
    else if (QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor))
    {
      spinBox->interpretText();
      value = static_cast<quint64>(spinBox->value());
    }
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
    {
      value = lineEdit->text().toULongLong(&valid, 10);
    }
    else
    {
      valid = false;
    }

    if (!valid || (value > maximumValue(signal.bitLength)) ||
        !assignLogicalValue(&nextValues, signal, value))
    {
      ui->statusLabel->setText(
        QString::fromUtf8("应用失败：信号 %1 的值无效")
          .arg(QString::fromLatin1(signal.name)));
      return;
    }
  }

  linRuntime->setPublishedFrameSignal(selectedFrameIndex, nextValues);
  emit valuesApplied();
  ui->statusLabel->setText(
    QString::fromUtf8("已应用报文 %1 的 %2 个信号")
      .arg(QString::fromLatin1(frameLayout->name))
      .arg(controlledSignals.size()));
}
void SignalControlFrame::closePage()
{
  hide();
}
