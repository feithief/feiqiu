#include "signalcontrolframe.h"

#include "linlayout.h"
#include "linruntime.h"
#include "ui_signalcontrolframe.h"

#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSpinBox>

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
    frameLayout(0)
{
  Q_ASSERT(linRuntime != 0);

  ui->setupUi(this);
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

void SignalControlFrame::buildSignalRows()
{
  frameLayout = primaryControlFrame(linRuntime->layout());
  controlledSignals.clear();
  valueEditors.clear();

  while (QLayoutItem *item = ui->verticalLayoutSignals->takeAt(0))
  {
    delete item->widget();
    delete item;
  }

  const LinLayout &layout = linRuntime->layout();
  int totalSignalCount = 0;
  for (int frameIndex = 0;
       frameIndex < layout.publishedFrameCount;
       ++frameIndex)
    totalSignalCount += layout.publishedFrames[frameIndex].signalCount;

  if (frameLayout != 0)
  {
    ui->frameInformation->setText(
      QString::fromUtf8(
        "主控制帧：%1    已发布帧：%2    全部信号：%3")
        .arg(QString::fromLatin1(frameLayout->name))
        .arg(layout.publishedFrameCount)
        .arg(totalSignalCount));
  }
  else
  {
    ui->frameInformation->setText(
      QString::fromUtf8("当前布局没有可控制的主帧"));
  }

  if ((frameLayout == 0) || (totalSignalCount <= 0))
  {
    QLabel *empty = new QLabel(
      QString::fromUtf8("活动控制帧中没有信号。"),
      ui->signalScrollContent);
    empty->setAlignment(Qt::AlignCenter);
    ui->verticalLayoutSignals->addWidget(empty);
    ui->verticalLayoutSignals->addStretch();
    return;
  }

  for (int frameIndex = 0;
       frameIndex < layout.publishedFrameCount;
       ++frameIndex)
  {
    const LinFrameLayout &publishedFrame =
      layout.publishedFrames[frameIndex];
    QLabel *frameHeader = new QLabel(
      QString::fromUtf8("帧：%1    ID：0x%2    长度：%3 字节")
        .arg(QString::fromLatin1(publishedFrame.name))
        .arg(static_cast<int>(publishedFrame.frameId),
             2,
             16,
             QChar('0'))
        .arg(static_cast<int>(publishedFrame.length))
        .toUpper(),
      ui->signalScrollContent);
    frameHeader->setFixedHeight(44);
    frameHeader->setStyleSheet(
      "color:#33d7f3;font-size:20px;font-weight:bold;"
      "background:rgba(15,186,205,35);padding-left:16px;");
    ui->verticalLayoutSignals->addWidget(frameHeader);

    for (int signalIndex = 0;
         signalIndex < publishedFrame.signalCount;
         ++signalIndex)
    {
      const LinSignalLayout &signal =
        publishedFrame.signalLayouts[signalIndex];
      QFrame *row = new QFrame(ui->signalScrollContent);
      row->setFixedHeight(64);
      row->setStyleSheet(
        "QFrame{border:1px solid rgba(15,186,205,130);"
        "background:rgba(2,22,31,190);}");

      QHBoxLayout *rowLayout = new QHBoxLayout(row);
      rowLayout->setContentsMargins(18, 5, 18, 5);
      rowLayout->setSpacing(12);

      QLabel *name = columnLabel(
        QString::fromLatin1(signal.name), 360, row);
      name->setToolTip(QString::fromLatin1(signal.name));
      rowLayout->addWidget(name);
      rowLayout->addWidget(
        columnLabel(QString::fromUtf8("起始 %1 / 长度 %2 bit")
                      .arg(static_cast<int>(signal.startBit))
                      .arg(static_cast<int>(signal.bitLength)),
                    180,
                    row));
      rowLayout->addWidget(
        columnLabel(QString("0x0 - 0x%1")
                      .arg(maximumValue(signal.bitLength), 0, 16)
                      .toUpper(),
                    280,
                    row));

      QWidget *editor = 0;
      if (signal.bitLength == 1)
      {
        QCheckBox *checkBox = new QCheckBox(
          QString::fromUtf8("值为 1"), row);
        checkBox->setFixedWidth(250);
        editor = checkBox;
      }
      else if (signal.bitLength <= 30)
      {
        QSpinBox *spinBox = new QSpinBox(row);
        spinBox->setFixedWidth(250);
        spinBox->setRange(
          0, static_cast<int>(maximumValue(signal.bitLength)));
        spinBox->setDisplayIntegerBase(16);
        spinBox->setPrefix("0x");
        spinBox->setAlignment(Qt::AlignCenter);
        spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        editor = spinBox;
      }
      else
      {
        QLineEdit *lineEdit = new QLineEdit(row);
        lineEdit->setFixedWidth(250);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setMaxLength(8);
        lineEdit->setValidator(
          new QRegExpValidator(QRegExp("[0-9A-Fa-f]{1,8}"), lineEdit));
        lineEdit->setToolTip(
          QString::fromUtf8("输入十六进制值，不需要 0x 前缀"));
        editor = lineEdit;
      }

      editor->setObjectName(
        QString("signalValue_%1").arg(valueEditors.size()));
      rowLayout->addWidget(editor);
      controlledSignals.append(&signal);
      valueEditors.append(editor);
      ui->verticalLayoutSignals->addWidget(row);
    }
  }

  ui->verticalLayoutSignals->addStretch();
}

void SignalControlFrame::init()
{
  loadCurrentValues();
}

void SignalControlFrame::loadCurrentValues()
{
  if ((frameLayout == 0) ||
      (valueEditors.size() != controlledSignals.size()))
  {
    ui->statusLabel->setText(
      QString::fromUtf8("读取失败：控制帧布局无效"));
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
      lineEdit->setText(QString::number(value, 16).toUpper());
  }

  ui->statusLabel->setText(
    QString::fromUtf8("已同步当前发送值"));
}

void SignalControlFrame::applyValues()
{
  if ((frameLayout == 0) ||
      (valueEditors.size() != controlledSignals.size()))
  {
    ui->statusLabel->setText(
      QString::fromUtf8("应用失败：控制帧布局无效"));
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
      value = lineEdit->text().toULongLong(&valid, 16);
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

  linRuntime->setBCMSignal(nextValues);
  emit valuesApplied();
  ui->statusLabel->setText(
    QString::fromUtf8("已应用 %1 个信号，控制帧等待发送")
      .arg(controlledSignals.size()));
}

void SignalControlFrame::closePage()
{
  hide();
}
