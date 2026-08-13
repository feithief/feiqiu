#include "signalcontrolframe.h"

#include "linlayout.h"
#include "linruntime.h"
#include "ui_signalcontrolframe.h"

#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSlider>
#include <QSpinBox>
#include <QTimer>

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
    selectedFrameIndex(-1),
    immediateApplyTimer(new QTimer(this)),
    loadingValues(false)
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
  immediateApplyTimer->setSingleShot(true);
  immediateApplyTimer->setInterval(20);
  connect(immediateApplyTimer, &QTimer::timeout,
          this, &SignalControlFrame::applyValuesWithoutNotification);

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

QString SignalControlFrame::shortSignalName(const char *sourceName) const
{
  const QString fullName = QString::fromLatin1(sourceName != 0
                                                ? sourceName : "");
  QStringList parts = fullName.split('_', QString::SkipEmptyParts);
  while (parts.size() > 3)
    parts.removeFirst();
  if ((parts.size() > 1) &&
      (parts.first().compare(QString("RGB"), Qt::CaseInsensitive) == 0))
  {
    parts.removeFirst();
  }

  QString displayName = parts.join(QString(" "));
  displayName.replace(QString("Dimmrampe"), QString("Dimming"),
                      Qt::CaseInsensitive);
  displayName.replace(QString("Sonderfunktion"), QString("Special"),
                      Qt::CaseInsensitive);
  displayName.replace(QString("Intensitaet"), QString("Intensity"),
                      Qt::CaseInsensitive);
  displayName.replace(QString("GlobalSync"), QString("Global Sync"),
                      Qt::CaseInsensitive);
  if (displayName.length() > 18)
    displayName = displayName.left(16) + QString("..");
  return displayName;
}
void SignalControlFrame::buildSignalRows()
{
  immediateApplyTimer->stop();
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

  for (int signalIndex = 0;
       signalIndex < frameLayout->signalCount;
       ++signalIndex)
  {
    const LinSignalLayout &signal = frameLayout->signalLayouts[signalIndex];
    const quint64 signalMaximum = maximumValue(signal.bitLength);

    QFrame *row = new QFrame(ui->signalScrollContent);
    row->setFixedHeight(54);
    row->setStyleSheet(
      "QFrame{border:1px solid rgba(15,186,205,150);"
      "border-radius:7px;background:rgba(2,22,31,235);}"
      "QLabel{border:0;background:transparent;color:rgb(166,231,247);"
      "font-size:14px;}"
      "QSpinBox,QLineEdit{border:1px solid #0fbacd;border-radius:4px;"
      "background:rgb(2,22,31);color:white;font-size:17px;}");

    QHBoxLayout *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(5, 4, 5, 4);
    rowLayout->setSpacing(8);

    QLabel *name = new QLabel(shortSignalName(signal.name), row);
    name->setToolTip(QString::fromLatin1(signal.name));
    name->setFixedWidth(145);
    name->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rowLayout->addWidget(name);

    QWidget *editor = 0;
    QSlider *slider = new QSlider(Qt::Horizontal, row);
    slider->setObjectName(
      QString("signalSlider_%1").arg(valueEditors.size()));
    slider->setFixedSize(325, 22);
    slider->setSingleStep(1);
    slider->setStyleSheet(
      "QSlider{border:0;background:transparent;}"
      "QSlider::groove:horizontal{height:4px;background:rgba(42,78,92,210);"
      "border-radius:2px;}"
      "QSlider::sub-page:horizontal{height:4px;background:rgb(29,165,219);"
      "border-radius:2px;}"
      "QSlider::add-page:horizontal{height:4px;background:rgba(42,78,92,210);"
      "border-radius:2px;}"
      "QSlider::handle:horizontal{width:14px;height:14px;margin:-5px 0;"
      "border-radius:7px;background:rgb(38,211,235);}");

    if (signal.bitLength <= 30)
    {
      const int maximum = static_cast<int>(signalMaximum);
      QSpinBox *spinBox = new QSpinBox(row);
      spinBox->setFixedSize(80, 32);
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
      connect(spinBox,
              static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
              this, [this](int) { scheduleImmediateApply(); });
      editor = spinBox;
    }
    else
    {
      QLineEdit *lineEdit = new QLineEdit(row);
      lineEdit->setFixedSize(80, 32);
      lineEdit->setAlignment(Qt::AlignCenter);
      lineEdit->setMaxLength(10);
      lineEdit->setValidator(
        new QRegExpValidator(QRegExp("[0-9]{1,10}"), lineEdit));
      slider->setRange(0, 1000);
      slider->setPageStep(50);
      connect(slider, &QSlider::sliderMoved, lineEdit,
              [lineEdit, signalMaximum](int position) {
        const quint64 value =
          (signalMaximum * static_cast<quint64>(position)) / 1000ULL;
        lineEdit->setText(QString::number(value));
      });
      connect(lineEdit, &QLineEdit::textChanged, slider,
              [slider, signalMaximum](const QString &valueText) {
        bool valid = false;
        quint64 value = valueText.toULongLong(&valid, 10);
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
      connect(lineEdit, &QLineEdit::textChanged,
              this, [this](const QString &) { scheduleImmediateApply(); });
      editor = lineEdit;
    }

    editor->setObjectName(
      QString("signalValue_%1").arg(valueEditors.size()));
    rowLayout->addWidget(editor);
    rowLayout->addWidget(slider);

    controlledSignals.append(&signal);
    valueEditors.append(editor);
    ui->verticalLayoutSignals->addWidget(row);
  }

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

bool SignalControlFrame::applyPresetToCurrentFrame(int presetIndex)
{
  const LinLayout &layout = linRuntime->layout();
  if ((frameLayout == 0) ||
      (selectedFrameIndex < 0) ||
      (presetIndex < 0) ||
      (presetIndex >= layout.signalPresetCount) ||
      (layout.signalPresets == 0))
  {
    return false;
  }

  BCMSignal nextValues = linRuntime->getBCMSignal();
  const LinSignalPreset &preset = layout.signalPresets[presetIndex];
  int appliedCount = 0;
  for (int assignmentIndex = 0;
       assignmentIndex < preset.assignmentCount;
       ++assignmentIndex)
  {
    const LinSignalPresetAssignment &assignment =
      preset.assignments[assignmentIndex];
    if (assignment.frameIndex != selectedFrameIndex)
      continue;

    for (int signalIndex = 0;
         signalIndex < frameLayout->signalCount;
         ++signalIndex)
    {
      const LinSignalLayout &signal = frameLayout->signalLayouts[signalIndex];
      if ((QString::fromLatin1(signal.name) ==
           QString::fromLatin1(assignment.signalName)) &&
          (signal.startBit == assignment.startBit) &&
          (signal.bitLength == assignment.bitLength) &&
          assignLogicalValue(&nextValues, signal, assignment.value))
      {
        ++appliedCount;
        break;
      }
    }
  }

  if (appliedCount <= 0)
  {
    ui->statusLabel->setText(
      QString::fromUtf8("当前颜色在所选报文中没有对应信号"));
    return false;
  }

  linRuntime->setPublishedFrameSignal(selectedFrameIndex, nextValues);
  loadCurrentValues();
  ui->statusLabel->setText(
    QString::fromUtf8("颜色 %1 已更新当前报文的 %2 个信号")
      .arg(QString::fromUtf8(preset.name))
      .arg(appliedCount));
  emit valuesApplied();
  return true;
}
void SignalControlFrame::loadCurrentValues()
{
  immediateApplyTimer->stop();
  if ((frameLayout == 0) ||
      (valueEditors.size() != controlledSignals.size()))
  {
    ui->statusLabel->setText(
      QString::fromUtf8("读取失败：当前报文布局无效"));
    return;
  }

  loadingValues = true;
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
  loadingValues = false;

  ui->statusLabel->setText(
    QString::fromUtf8("已读取当前报文的 %1 个信号状态")
      .arg(controlledSignals.size()));
}

void SignalControlFrame::applyValues()
{
  writeEditorValues(true);
}

void SignalControlFrame::applyValuesWithoutNotification()
{
  writeEditorValues(false);
}

void SignalControlFrame::scheduleImmediateApply()
{
  if (!loadingValues && (frameLayout != 0))
    immediateApplyTimer->start();
}

void SignalControlFrame::writeEditorValues(bool notifyParent)
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
  if (notifyParent)
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
