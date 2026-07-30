#include "slaveframeconfig.h"

#include "adialog.h"
#include "linruntime.h"
#include "keyboard.h"
#include "linlayout.h"
#include "ui_slavenodeframe.h"

#include <QApplication>
#include <QEvent>
#include <QFrame>
#include <QLabel>
#include <QTimer>

namespace {

static const QString kFrameStyle = "QFrame{"
                                   "border:2px solid #0fbacd;"
                                   "border-radius:15px;"
                                   "background-color:rgba(0,0,0,0.7);"
                                   "}";

static const QString kButtonEnabledStyle = "QPushButton{"
                                           "border:2px solid #0fbacd;"
                                           "border-top-right-radius:15px;"
                                           "border-bottom-left-radius:15px;"
                                           "color:rgb(255,251,240);"
                                           "font-size:22px;"
                                           "background:rgba(29,165,219,0.3);"
                                           "}";

static const QString kButtonDisabledStyle = "QPushButton{"
                                            "border:0px;"
                                            "border-top-right-radius:15px;"
                                            "border-bottom-left-radius:15px;"
                                            "color:rgba(255,251,240,0.5);"
                                            "font-size:22px;"
                                            "background:rgba(29,165,219,0.1);"
                                            "}";

static const QString kRawStatusStyle = "QLabel{"
                                       "font-size:22px;"
                                       "color:rgb(255,251,240);"
                                       "}";

} // namespace

SlaveFrameConfig::SlaveFrameConfig(LinRuntime *runtime,
                                   QWidget *parent)
  : QWidget(parent),
    linRuntime(runtime),
    currentNode(0),
    nodeType(ENodeTypeRGB),
    configurationAvailable(false),
    ui(new Ui::SlaveNodeFrame),
    dialog(0),
    backgroundframe(0),
    keys(0),
    readRequestId(0),
    writeRequestId(0),
    calibrationRequestId(0),
    feedbackWatchdog(new QTimer(this)),
    lockRequestId(0),
    unlockRequestId(0),
    statusUsesRawFrame(false)
{
  Q_ASSERT(linRuntime != 0);
  backgroundframe = new QFrame(this);
  backgroundframe->setStyleSheet(kFrameStyle);
  backgroundframe->setGeometry(0, 0, 1366, 768);
  backgroundframe->show();

  ui->setupUi(this);

  const LinLayout &profile = linRuntime->layout();
  const LinServiceLayout *partNumberService = findLinService(
    profile, EOperationTypePartNO);
  const LinServiceLayout *serialNumberService = findLinService(
    profile, EOperationTypeSerialNO);

  ui->spinBoxSA->setRange(0, 0x7F);
  ui->spinBoxSA->setToolTip(
    "The single address must be a NAD declared in the active LIN profile.");
  ui->spinBoxGA->setRange(0, 0x7FFF);
  ui->spinBoxGA->setReadOnly(true);
  ui->spinBoxGA->setToolTip(
    "Automatically derived from the SA/control-mask table in the profile.");
  ui->spinBoxPlatform->setRange(0, 2);
  ui->spinBoxIntensity->setRange(0, 0xFFFF);
  ui->labelIntensity->setText("Factor:");
  if (partNumberService != 0)
    ui->lineEditPN->setMaxLength(partNumberService->dataLength);
  if (serialNumberService != 0)
    ui->lineEditSerial->setMaxLength(serialNumberService->dataLength);

  dialog = new ADialog(this);
  dialog->hide();

  keys = new KeyBoard(this);
  keys->hide();

  feedbackWatchdog->setSingleShot(true);
  feedbackWatchdog->setInterval(5000);
  connect(feedbackWatchdog, SIGNAL(timeout()),
          this, SLOT(handleFeedbackTimeout()));

  connect(ui->pushButtonCancel, SIGNAL(clicked()),
          this, SLOT(exitSlaveConfig()));
  connect(ui->pushButtonNoCalibrate, SIGNAL(clicked()),
          this, SLOT(buttonCalibrateNormal()));
  connect(ui->pushButtonCalibrateR, SIGNAL(clicked()),
          this, SLOT(buttonCalibrateR()));
  connect(ui->pushButtonCalibrateG, SIGNAL(clicked()),
          this, SLOT(buttonCalibrateG()));
  connect(ui->pushButtonCalibrateB, SIGNAL(clicked()),
          this, SLOT(buttonCalibrateB()));
  connect(ui->pushButtonAccept, SIGNAL(clicked()),
          this, SLOT(changeConfigs()));
  connect(ui->pushButtonLock, SIGNAL(clicked()),
          this, SLOT(requestLock()));
  connect(ui->pushButtonUnlock, SIGNAL(clicked()),
          this, SLOT(requestUnlock()));
  connect(ui->spinBoxSA, SIGNAL(valueChanged(int)),
          this, SLOT(singleAddressChanged(int)));

  connect(linRuntime, SIGNAL(SlaveStatusChanged(SlaveStatus)),
          this, SLOT(updateNodeState(SlaveStatus)));
  connect(linRuntime, SIGNAL(nodeResponseObserved(quint8)),
          this, SLOT(handleNodeResponse(quint8)));
  connect(linRuntime,
          SIGNAL(nodeConfigurationRead(quint32,SlaveConfigInfo,bool,QString)),
          this,
          SLOT(handleReadResult(quint32,SlaveConfigInfo,bool,QString)));
  connect(linRuntime,
          SIGNAL(nodeConfigurationWritten(quint32,quint8,bool,QString)),
          this,
          SLOT(handleWriteResult(quint32,quint8,bool,QString)));
  connect(linRuntime,
          SIGNAL(calibrationFinished(quint32,quint8,bool,QString)),
          this,
          SLOT(handleCalibrationResult(quint32,quint8,bool,QString)));
  connect(linRuntime,
          SIGNAL(nodeLockStateChanged(quint32,quint8,bool,bool,QString)),
          this,
          SLOT(handleLockStateResult(quint32,quint8,bool,bool,QString)));

  ui->spinBoxGA->installEventFilter(this);
  ui->spinBoxSA->installEventFilter(this);
  ui->spinBoxIntensity->installEventFilter(this);
  ui->spinBoxPlatform->installEventFilter(this);
  ui->lineEditPN->installEventFilter(this);
  ui->lineEditSerial->installEventFilter(this);
  ui->doubleSpinBoxRX->installEventFilter(this);
  ui->doubleSpinBoxRY->installEventFilter(this);
  ui->doubleSpinBoxRL->installEventFilter(this);
  ui->doubleSpinBoxGX->installEventFilter(this);
  ui->doubleSpinBoxGY->installEventFilter(this);
  ui->doubleSpinBoxGL->installEventFilter(this);
  ui->doubleSpinBoxBX->installEventFilter(this);
  ui->doubleSpinBoxBY->installEventFilter(this);
  ui->doubleSpinBoxBL->installEventFilter(this);

  resetForm();
  hide();
}

SlaveFrameConfig::~SlaveFrameConfig()
{
  delete ui;
}

void SlaveFrameConfig::setConfigurationControlsEnabled(bool enabled)
{
  ui->pushButtonAccept->setEnabled(enabled);
  ui->pushButtonAccept->setText("Apply");
  ui->pushButtonNoCalibrate->setEnabled(enabled);
  ui->pushButtonCalibrateR->setEnabled(enabled);
  ui->pushButtonCalibrateG->setEnabled(enabled);
  ui->pushButtonCalibrateB->setEnabled(enabled);

  ui->spinBoxSA->setReadOnly(!enabled);
  ui->spinBoxPlatform->setReadOnly(!enabled);
  ui->spinBoxIntensity->setReadOnly(!enabled);
  ui->lineEditPN->setReadOnly(!enabled);
  ui->lineEditSerial->setReadOnly(!enabled);
  ui->doubleSpinBoxRX->setReadOnly(!enabled);
  ui->doubleSpinBoxRY->setReadOnly(!enabled);
  ui->doubleSpinBoxRL->setReadOnly(!enabled);
  ui->doubleSpinBoxGX->setReadOnly(!enabled);
  ui->doubleSpinBoxGY->setReadOnly(!enabled);
  ui->doubleSpinBoxGL->setReadOnly(!enabled);
  ui->doubleSpinBoxBX->setReadOnly(!enabled);
  ui->doubleSpinBoxBY->setReadOnly(!enabled);
  ui->doubleSpinBoxBL->setReadOnly(!enabled);

  const QString reason = enabled
    ? QString()
    : QString("Status-only: this LDF defines no proprietary DID services.");
  ui->pushButtonAccept->setToolTip(reason);
}

void SlaveFrameConfig::SlaveFrameConfigInit(int slaveNode)
{
  const LinLayout &profile = linRuntime->layout();
  const LinNodeLayout *node = findLinNode(
    profile, static_cast<quint8>(slaveNode));
  if (node == 0)
  {
    /* Keep diagnostic failures in Debug; this page only pops up read/write OK. */
    dialog->hide();
    return;
  }

  if (readRequestId != 0)
    linRuntime->cancel(readRequestId);
  if (writeRequestId != 0)
    linRuntime->cancel(writeRequestId);
  if (calibrationRequestId != 0)
    linRuntime->cancel(calibrationRequestId);
  if (lockRequestId != 0)
    linRuntime->cancel(lockRequestId);
  if (unlockRequestId != 0)
    linRuntime->cancel(unlockRequestId);

  readRequestId = 0;
  writeRequestId = 0;
  calibrationRequestId = 0;
  lockRequestId = 0;
  unlockRequestId = 0;

  currentNode = slaveNode;
  nodeType = node->nodeType;
  configurationAvailable =
    (profile.diagnosticModel == ELinDiagnosticModelCustomDid) &&
    (profile.serviceCount > 0);
  resetForm();
  configureStatusRows(*node);
  ui->spinBoxSA->setValue(node->diagnosticNad);
  ui->spinBoxGA->setValue(node->controlAddressMask);
  setConfigurationControlsEnabled(configurationAvailable);
  setLockButtonsBusy(false);
  feedbackWatchdog->start();

  /*
   * LDF-only profiles still open this page in status-only mode.  Never guess
   * proprietary DIDs that are absent from the supplied protocol data.
   */
  dialog->hide();
  if (!configurationAvailable)
    return;

  /* Diagnostic progress is intentionally silent; only read/write OK is shown. */
  readRequestId = linRuntime->readNodeConfiguration(
    static_cast<quint8>(currentNode));
  if (readRequestId == 0)
    dialog->hide();
}

void SlaveFrameConfig::updateNodeState(SlaveStatus status)
{
  if ((currentNode == 0) || (status.slaveNAD != currentNode))
    return;

  if (!status.isOnLine)
    return;

  feedbackWatchdog->start();
  QLabel *values[6] = {
    ui->ROutput_Err, ui->GOutput_Err, ui->BOutput_Err,
    ui->Temp_Err, ui->Voltage_Err, ui->Lin_Err
  };
  if (statusUsesRawFrame)
  {
    const QByteArray rawText = status.rawFrame.toHex(' ').toUpper();
    values[0]->setText(
      rawText.isEmpty() ? QString("--") : QString::fromLatin1(rawText));
    values[0]->setToolTip(values[0]->text());
    return;
  }

  for (int index = 0; index < 6; ++index)
  {
    if (values[index]->isHidden() || !status.rawValueValid[index])
      continue;
    values[index]->setText(
      QString("0x%1").arg(status.rawValues[index], 0, 16).toUpper());
    values[index]->setStyleSheet(kRawStatusStyle);
  }
}

void SlaveFrameConfig::handleNodeResponse(quint8 node)
{
  if ((currentNode != 0) && (node == currentNode))
    feedbackWatchdog->start();
}

void SlaveFrameConfig::exitSlaveConfig()
{
  if (readRequestId != 0)
    linRuntime->cancel(readRequestId);
  if (writeRequestId != 0)
    linRuntime->cancel(writeRequestId);
  if (calibrationRequestId != 0)
    linRuntime->cancel(calibrationRequestId);
  if (lockRequestId != 0)
    linRuntime->cancel(lockRequestId);
  if (unlockRequestId != 0)
    linRuntime->cancel(unlockRequestId);

  feedbackWatchdog->stop();
  readRequestId = 0;
  writeRequestId = 0;
  calibrationRequestId = 0;
  lockRequestId = 0;
  unlockRequestId = 0;
  currentNode = 0;
  dialog->hide();
  keys->hide();
  resetForm();
  hide();
}

void SlaveFrameConfig::changeConfigs()
{
  if (!configurationAvailable || (currentNode == 0))
  {
    ui->pushButtonAccept->setText("Unavailable");
    ui->pushButtonAccept->setToolTip(
      "No writable diagnostic configuration is available for this node.");
    return;
  }

  if (writeRequestId != 0)
  {
    ui->pushButtonAccept->setText(QString::fromUtf8("正在写入..."));
    return;
  }

  const int requestedNad = ui->spinBoxSA->value();
  const LinLayout &profile = linRuntime->layout();
  const LinNodeLayout *requestedNode = findLinNode(
    profile, static_cast<quint8>(requestedNad));
  if (requestedNode == 0)
  {
    ui->pushButtonAccept->setText("Invalid NAD");
    ui->pushButtonAccept->setToolTip(
      "Single Address is not declared in the active node table.");
    return;
  }

  if (ui->spinBoxGA->value() != requestedNode->controlAddressMask)
    ui->spinBoxGA->setValue(requestedNode->controlAddressMask);

  SlaveConfigInfo info;
  info.slaveNode = static_cast<quint8>(currentNode);
  info.nodeType = nodeType;
  info.SA = static_cast<quint16>(requestedNad);
  info.GA = static_cast<quint16>(ui->spinBoxGA->value());
  info.platform = static_cast<quint16>(ui->spinBoxPlatform->value());
  info.intensity = static_cast<quint16>(ui->spinBoxIntensity->value());
  info.PN = ui->lineEditPN->text();
  info.SN = ui->lineEditSerial->text();
  info.locked = false;
  info.r.x = ui->doubleSpinBoxRX->value();
  info.r.y = ui->doubleSpinBoxRY->value();
  info.r.Y = ui->doubleSpinBoxRL->value();
  info.g.x = ui->doubleSpinBoxGX->value();
  info.g.y = ui->doubleSpinBoxGY->value();
  info.g.Y = ui->doubleSpinBoxGL->value();
  info.b.x = ui->doubleSpinBoxBX->value();
  info.b.y = ui->doubleSpinBoxBY->value();
  info.b.Y = ui->doubleSpinBoxBL->value();

  /*
   * Do not use a progress popup: the diagnostic page only pops up read/write
   * OK.  The Apply button itself must still acknowledge the click immediately.
   */
  dialog->hide();
  ui->pushButtonAccept->setText(QString::fromUtf8("正在写入..."));
  ui->pushButtonAccept->setToolTip(
    QString::fromUtf8("先写完全部参数，等待 Flash 1 秒，再用 0x22 统一回读校验。"));
  ui->pushButtonAccept->setEnabled(false);
  writeRequestId = linRuntime->writeNodeConfiguration(info);
  if (writeRequestId == 0)
  {
    ui->pushButtonAccept->setEnabled(true);
    ui->pushButtonAccept->setText(QString::fromUtf8("失败 - 按F12"));
    ui->pushButtonAccept->setToolTip(
      QString::fromUtf8("诊断请求未进入队列，请按 F12 查看具体原因。"));
  }
}

void SlaveFrameConfig::singleAddressChanged(int value)
{
  const LinNodeLayout *node = findLinNode(
    linRuntime->layout(), static_cast<quint8>(value));
  if (node != 0)
    ui->spinBoxGA->setValue(node->controlAddressMask);
}

void SlaveFrameConfig::handleReadResult(quint32 requestId,
                                        SlaveConfigInfo info,
                                        bool success,
                                        QString errorMessage)
{
  Q_UNUSED(errorMessage);
  if ((requestId != readRequestId) ||
      (info.slaveNode != currentNode))
    return;

  readRequestId = 0;
  if (success)
  {
    displayConfiguration(info);
    showReadWriteOk();
  }
  else
  {
    dialog->hide();
  }
}

void SlaveFrameConfig::handleWriteResult(quint32 requestId,
                                         quint8 node,
                                         bool success,
                                         QString errorMessage)
{
  Q_UNUSED(errorMessage);
  if ((requestId != writeRequestId) || (node != currentNode))
    return;

  writeRequestId = 0;
  ui->pushButtonAccept->setEnabled(configurationAvailable);
  if (success)
  {
    ui->pushButtonAccept->setText("Apply");
    ui->pushButtonAccept->setToolTip(QString());
    showReadWriteOk();
  }
  else
  {
    ui->pushButtonAccept->setText(QString::fromUtf8("失败 - 按F12"));
    ui->pushButtonAccept->setToolTip(
      errorMessage.isEmpty()
      ? QString::fromUtf8("配置写入或 0x22 回读校验失败，请按 F12 查看具体原因。")
      : errorMessage);
    dialog->hide();
  }
}

void SlaveFrameConfig::handleCalibrationResult(quint32 requestId,
                                               quint8 node,
                                               bool success,
                                               QString errorMessage)
{
  if ((requestId != calibrationRequestId) || (node != currentNode))
    return;

  calibrationRequestId = 0;
  /* Calibration results remain available in Debug; do not interrupt the UI. */
  Q_UNUSED(success);
  Q_UNUSED(errorMessage);
  dialog->hide();
}

void SlaveFrameConfig::requestLock()
{
  if ((currentNode == 0) || (lockRequestId != 0) ||
      (unlockRequestId != 0))
    return;

  setLockButtonsBusy(true);
  lockRequestId = linRuntime->lockNode(static_cast<quint8>(currentNode));
  if (lockRequestId == 0)
    setLockButtonsBusy(false);
}

void SlaveFrameConfig::requestUnlock()
{
  if ((currentNode == 0) || (lockRequestId != 0) ||
      (unlockRequestId != 0))
    return;

  setLockButtonsBusy(true);
  unlockRequestId = linRuntime->unlockNode(static_cast<quint8>(currentNode));
  if (unlockRequestId == 0)
    setLockButtonsBusy(false);
}

void SlaveFrameConfig::handleLockStateResult(quint32 requestId,
                                             quint8 node,
                                             bool locked,
                                             bool success,
                                             QString errorMessage)
{
  Q_UNUSED(errorMessage);
  if ((node != currentNode) ||
      ((requestId != lockRequestId) && (requestId != unlockRequestId)))
    return;

  lockRequestId = 0;
  unlockRequestId = 0;
  setLockButtonsBusy(false);
  if (success)
    showLockState(locked);
}

void SlaveFrameConfig::handleFeedbackTimeout()
{
  if (currentNode != 0)
  {
    linRuntime->setReservedDebugValue(
      0,
      QString("Diag.FeedbackWatchdog"),
      QString("NAD %1: no response for 5000 ms; page closed")
      .arg(currentNode));
    exitSlaveConfig();
  }
}

void SlaveFrameConfig::buttonCalibrateNormal()
{
  requestCalibration(0);
}

void SlaveFrameConfig::buttonCalibrateR()
{
  requestCalibration(1);
}

void SlaveFrameConfig::buttonCalibrateG()
{
  requestCalibration(2);
}

void SlaveFrameConfig::buttonCalibrateB()
{
  requestCalibration(3);
}

void SlaveFrameConfig::requestCalibration(quint8 mode)
{
  if (!configurationAvailable ||
      (currentNode == 0) ||
      (calibrationRequestId != 0))
    return;

  dialog->hide();
  calibrationRequestId = linRuntime->calibrateNode(
    static_cast<quint8>(currentNode), mode);
  if (calibrationRequestId == 0)
    dialog->hide();
}

void SlaveFrameConfig::displayConfiguration(const SlaveConfigInfo &info)
{
  nodeType = info.nodeType;
  ui->labelSWVer->setText(info.softwareVersion);
  ui->labelHWVer->setText(info.hardwareVersion);
  ui->labelSuppVer->setText(info.supplierId);
  ui->labelFuncVer->setText(info.functionId);
  ui->labelVariantVer->setText(info.variantId);
  ui->spinBoxSA->setValue(info.SA);
  const LinNodeLayout *addressNode = findLinNode(
    linRuntime->layout(), static_cast<quint8>(info.SA));
  ui->spinBoxGA->setValue(addressNode != 0
    ? addressNode->controlAddressMask
    : info.GA);
  ui->spinBoxPlatform->setValue(info.platform);
  ui->spinBoxIntensity->setValue(info.intensity);
  ui->lineEditPN->setText(info.PN);
  ui->lineEditSerial->setText(info.SN);
  ui->doubleSpinBoxRX->setValue(info.r.x);
  ui->doubleSpinBoxRY->setValue(info.r.y);
  ui->doubleSpinBoxRL->setValue(info.r.Y);
  ui->doubleSpinBoxGX->setValue(info.g.x);
  ui->doubleSpinBoxGY->setValue(info.g.y);
  ui->doubleSpinBoxGL->setValue(info.g.Y);
  ui->doubleSpinBoxBX->setValue(info.b.x);
  ui->doubleSpinBoxBY->setValue(info.b.y);
  ui->doubleSpinBoxBL->setValue(info.b.Y);

  showLockState(info.locked);
}

void SlaveFrameConfig::showReadWriteOk()
{
  dialog->setMode(EDialogTypeDone);
  dialog->setContent(QString("OK"));
  dialog->show();
}

void SlaveFrameConfig::resetForm()
{
  ui->pushButtonAccept->setText("Apply");
  ui->ROutput_Err->clear();
  ui->GOutput_Err->clear();
  ui->BOutput_Err->clear();
  ui->Temp_Err->clear();
  ui->Voltage_Err->clear();
  ui->Lin_Err->clear();
  ui->labelSWVer->clear();
  ui->labelHWVer->clear();
  ui->labelSuppVer->clear();
  ui->labelFuncVer->clear();
  ui->labelVariantVer->clear();
  ui->lineEditPN->clear();
  ui->lineEditSerial->clear();
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
}

void SlaveFrameConfig::configureStatusRows(const LinNodeLayout &node)
{
  QLabel *titles[6] = {
    ui->labelRedOpenTitle, ui->labelGreenOpenTitle,
    ui->labelBlueOpenTitle, ui->labelRedShortTitle,
    ui->labelGreenShortTitle, ui->labelBlueShortTitle
  };
  QLabel *values[6] = {
    ui->ROutput_Err, ui->GOutput_Err, ui->BOutput_Err,
    ui->Temp_Err, ui->Voltage_Err, ui->Lin_Err
  };

  for (int index = 0; index < 6; ++index)
  {
    titles[index]->hide();
    values[index]->hide();
    titles[index]->setToolTip(QString());
    values[index]->setToolTip(QString());
  }
  statusUsesRawFrame = false;

  const LinLayout &profile = linRuntime->layout();
  if ((node.statusLayoutIndex < 0) ||
      (node.statusLayoutIndex >= profile.statusLayoutCount) ||
      (profile.statusLayouts == 0))
  {
    statusUsesRawFrame = true;
    titles[0]->setText("RAW");
    values[0]->setText("--");
    titles[0]->show();
    values[0]->show();
    return;
  }

  const LinStatusLayout &statusLayout =
    profile.statusLayouts[node.statusLayoutIndex];
  bool configured[6] = {false, false, false, false, false, false};
  int row = 0;
  for (int index = 0; index < statusLayout.fieldCount; ++index)
  {
    const LinStatusFieldLayout &field = statusLayout.fields[index];
    const int logicalIndex = static_cast<int>(field.field);
    if ((logicalIndex < 0) || (logicalIndex >= 6) ||
        configured[logicalIndex])
      continue;

    const QString fullName = QString::fromLatin1(field.name);
    const QString shortName = (fullName.size() <= 12)
                              ? fullName
                              : fullName.left(11) + QString("~");
    const int rowY = 140 + (row * 40);
    titles[logicalIndex]->setGeometry(70, rowY, 125, 42);
    values[logicalIndex]->setGeometry(205, rowY, 80, 42);
    titles[logicalIndex]->setText(shortName);
    titles[logicalIndex]->setToolTip(fullName);
    values[logicalIndex]->setText("--");
    values[logicalIndex]->setStyleSheet(kRawStatusStyle);
    titles[logicalIndex]->show();
    values[logicalIndex]->show();
    configured[logicalIndex] = true;
    ++row;
  }

  if (row == 0)
  {
    statusUsesRawFrame = true;
    titles[0]->setGeometry(70, 140, 125, 42);
    values[0]->setGeometry(205, 140, 80, 42);
    titles[0]->setText("RAW");
    values[0]->setText("--");
    titles[0]->show();
    values[0]->show();
  }
}

void SlaveFrameConfig::setLockButtonsBusy(bool busy)
{
  const LinLayout &profile = linRuntime->layout();
  const LinServiceLayout *lockService = findLinService(
    profile, EOperationTypeLock);
  const bool lockAvailable =
    configurationAvailable &&
    (lockService != 0) &&
    lockService->writable &&
    (lockService->serviceId == 0x0002) &&
    (lockService->dataLength == 2);
  const bool unlockAvailable =
    configurationAvailable && profile.securityAccess.enabled;
  ui->pushButtonLock->setEnabled(!busy && lockAvailable);
  ui->pushButtonUnlock->setEnabled(!busy && unlockAvailable);
  ui->pushButtonLock->setText(busy ? "Wait..." : "Lock");
  ui->pushButtonUnlock->setText(busy ? "Wait..." : "Unlock");
}

void SlaveFrameConfig::showLockState(bool locked)
{
  ui->pushButtonLock->setText("Lock");
  ui->pushButtonUnlock->setText("Unlock");
  ui->pushButtonLock->setStyleSheet(
    locked ? kButtonEnabledStyle : kButtonDisabledStyle);
  ui->pushButtonUnlock->setStyleSheet(
    locked ? kButtonDisabledStyle : kButtonEnabledStyle);
}

bool SlaveFrameConfig::eventFilter(QObject *watched, QEvent *event)
{
  if (event->type() == QEvent::FocusIn)
  {
    if (watched->inherits("QSpinBox") ||
        watched->inherits("QDoubleSpinBox") ||
        watched->inherits("QLineEdit"))
      showInputKeyBoard(true);
  }
  else if (event->type() == QEvent::FocusOut)
  {
    if (watched->inherits("QSpinBox") ||
        watched->inherits("QDoubleSpinBox") ||
        watched->inherits("QLineEdit"))
      showInputKeyBoard(false);
  }

  return QWidget::eventFilter(watched, event);
}

void SlaveFrameConfig::showInputKeyBoard(bool show)
{
  QWidget *widget = QApplication::focusWidget();
  if (show && (widget != 0))
  {
    if (widget->y() > height() / 2)
      keys->setGeometry(68, 0, 1230, 353);
    else
      keys->setGeometry(68, height() - 358, 1230, 353);
    keys->setCurrentObject(widget);
    keys->show();
  }
  else
  {
    keys->hide();
  }
}

void SlaveFrameConfig::on_SleepButton_clicked()
{
  linRuntime->sleepBus();
}

void SlaveFrameConfig::on_AwakeButton_clicked()
{
  linRuntime->wakeBus();
}
