#include "slaveframeconfig.h"

#include "adialog.h"
#include "ambientlinscheduler.h"
#include "keyboard.h"
#include "linlayout.h"
#include "ui_slavenodeframe.h"

#include <QApplication>
#include <QEvent>
#include <QFrame>

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

static const QString kNormalStatusStyle = "QLabel{"
                                          "font-size:22px;"
                                          "color:rgb(104,194,53);"
                                          "}";

static const QString kErrorStatusStyle = "QLabel{"
                                         "font-size:22px;"
                                         "color:rgb(254,67,101);"
                                         "}";

static const QString kUnknownStatusStyle = "QLabel{"
                                           "font-size:22px;"
                                           "color:rgb(160,160,160);"
                                           "}";

void setStatusLabel(QLabel *label, SlaveErrorFlag status)
{
  if (status == ESlaveErrorFlagError)
  {
    label->setText("Error");
    label->setStyleSheet(kErrorStatusStyle);
  }
  else if (status == ESlaveErrorFlagNormal)
  {
    label->setText("Normal");
    label->setStyleSheet(kNormalStatusStyle);
  }
  else
  {
    label->setText("Unknown");
    label->setStyleSheet(kUnknownStatusStyle);
  }
}

} // namespace

SlaveFrameConfig::SlaveFrameConfig(AmbientLinScheduler *scheduler,
                                   QWidget *parent)
  : QWidget(parent),
    linScheduler(scheduler),
    currentNode(0),
    nodeType(ENodeTypeRGB),
    ui(new Ui::SlaveNodeFrame),
    dialog(0),
    backgroundframe(0),
    keys(0),
    readRequestId(0),
    writeRequestId(0),
    calibrationRequestId(0)
{
  backgroundframe = new QFrame(this);
  backgroundframe->setStyleSheet(kFrameStyle);
  backgroundframe->setGeometry(0, 0, 1366, 768);
  backgroundframe->show();

  ui->setupUi(this);

  const LinLayout &profile = linScheduler->layout();
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
  connect(ui->spinBoxSA, SIGNAL(valueChanged(int)),
          this, SLOT(singleAddressChanged(int)));

  connect(linScheduler, SIGNAL(SlaveStatusChanged(SlaveStatus)),
          this, SLOT(updateNodeState(SlaveStatus)));
  connect(linScheduler,
          SIGNAL(nodeConfigurationRead(quint32,SlaveConfigInfo,bool,QString)),
          this,
          SLOT(handleReadResult(quint32,SlaveConfigInfo,bool,QString)));
  connect(linScheduler,
          SIGNAL(nodeConfigurationWritten(quint32,quint8,bool,QString)),
          this,
          SLOT(handleWriteResult(quint32,quint8,bool,QString)));
  connect(linScheduler,
          SIGNAL(calibrationFinished(quint32,quint8,bool,QString)),
          this,
          SLOT(handleCalibrationResult(quint32,quint8,bool,QString)));

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

void SlaveFrameConfig::SlaveFrameConfigInit(int slaveNode)
{
  const LinLayout &profile = linScheduler->layout();
  if ((profile.diagnosticModel != ELinDiagnosticModelCustomDid) ||
      (profile.serviceCount <= 0))
  {
    /* An LDF can describe standard node configuration without defining this
     * application's proprietary DID page.  Do not send guessed services. */
    hide();
    return;
  }

  if (findLinNode(profile, static_cast<quint8>(slaveNode)) == 0)
  {
    /* Keep diagnostic failures in Debug; this page only pops up read/write OK. */
    dialog->hide();
    return;
  }

  if (readRequestId != 0)
    linScheduler->cancel(readRequestId);
  if (writeRequestId != 0)
    linScheduler->cancel(writeRequestId);
  if (calibrationRequestId != 0)
    linScheduler->cancel(calibrationRequestId);

  readRequestId = 0;
  writeRequestId = 0;
  calibrationRequestId = 0;

  currentNode = slaveNode;
  const LinNodeLayout *node = findLinNode(linScheduler->layout(),
                                         static_cast<quint8>(slaveNode));
  nodeType = node->nodeType;
  resetForm();

  /* Diagnostic progress is intentionally silent; only read/write OK is shown. */
  dialog->hide();
  readRequestId = linScheduler->readNodeConfiguration(
    static_cast<quint8>(currentNode));
  if (readRequestId == 0)
    dialog->hide();
}

void SlaveFrameConfig::updateNodeState(SlaveStatus status)
{
  if ((currentNode == 0) || (status.slaveNAD != currentNode))
    return;

  if (!status.isOnLine)
  {
    exitSlaveConfig();
    return;
  }

  setStatusLabel(ui->ROutput_Err, status.ROutput_Err);
  setStatusLabel(ui->GOutput_Err, status.GOutput_Err);
  setStatusLabel(ui->BOutput_Err, status.BOutput_Err);
  setStatusLabel(ui->Temp_Err, status.Temp_Err);
  setStatusLabel(ui->Voltage_Err, status.Voltage_Err);
  setStatusLabel(ui->Lin_Err, status.Lin_Err);
}

void SlaveFrameConfig::exitSlaveConfig()
{
  if (readRequestId != 0)
    linScheduler->cancel(readRequestId);
  if (writeRequestId != 0)
    linScheduler->cancel(writeRequestId);
  if (calibrationRequestId != 0)
    linScheduler->cancel(calibrationRequestId);

  readRequestId = 0;
  writeRequestId = 0;
  calibrationRequestId = 0;
  currentNode = 0;
  dialog->hide();
  keys->hide();
  resetForm();
  hide();
}

void SlaveFrameConfig::changeConfigs()
{
  if ((currentNode == 0) || (writeRequestId != 0))
    return;

  const int requestedNad = ui->spinBoxSA->value();
  const LinLayout &profile = linScheduler->layout();
  const LinNodeLayout *requestedNode = findLinNode(
    profile, static_cast<quint8>(requestedNad));
  if (requestedNode == 0)
  {
    dialog->hide();
    return;
  }

  if (ui->spinBoxGA->value() != requestedNode->controlAddressMask)
  {
    ui->spinBoxGA->setValue(requestedNode->controlAddressMask);
    dialog->hide();
    return;
  }

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

  /* Diagnostic progress is intentionally silent; only read/write OK is shown. */
  dialog->hide();
  writeRequestId = linScheduler->writeNodeConfiguration(info);
  if (writeRequestId == 0)
    dialog->hide();
}

void SlaveFrameConfig::singleAddressChanged(int value)
{
  const LinNodeLayout *node = findLinNode(
    linScheduler->layout(), static_cast<quint8>(value));
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
  if (success)
  {
    showReadWriteOk();
  }
  else
  {
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
  if ((currentNode == 0) || (calibrationRequestId != 0))
    return;

  dialog->hide();
  calibrationRequestId = linScheduler->calibrateNode(
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
    linScheduler->layout(), static_cast<quint8>(info.SA));
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

  if (info.locked)
  {
    ui->pushButtonLock->setStyleSheet(kButtonEnabledStyle);
    ui->pushButtonUnlock->setStyleSheet(kButtonDisabledStyle);
  }
  else
  {
    ui->pushButtonLock->setStyleSheet(kButtonDisabledStyle);
    ui->pushButtonUnlock->setStyleSheet(kButtonEnabledStyle);
  }
}

void SlaveFrameConfig::showReadWriteOk()
{
  dialog->setMode(EDialogTypeDone);
  dialog->setContent(QString("OK"));
  dialog->show();
}

void SlaveFrameConfig::resetForm()
{
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
  linScheduler->sleepBus();
}

void SlaveFrameConfig::on_AwakeButton_clicked()
{
  linScheduler->wakeBus();
}
