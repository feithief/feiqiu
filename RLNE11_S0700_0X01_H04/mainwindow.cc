#include "mainwindow.h"

#include "linruntime.h"
#include "application_config.h"
#include "bcmmasterbutton.h"
#include "bcmmasterframe.h"
#include "debugpanel.h"
#include "debugsink.h"
#include "debugsnapshot.h"
#include "linlayout.h"
#include "productionverify.h"
#include "slavebutton.h"
#include "slaveframeconfig.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFont>
#include <QKeySequence>
#include <QLabel>
#include <QPushButton>
#include <QRect>
#include <QShortcut>

namespace {

const QSize kApplicationWindowSize(DAPP_CONFIG_WINDOW_WIDTH,
                                   DAPP_CONFIG_WINDOW_HEIGHT);

static const QString kMainWindowStyle = "QMainWindow{"
                                        "border:2px solid #0fbacd;"
                                        "border-radius:15px;"
                                        "background-color:rgba(0,0,0,0.5);"
                                        "}";

QRect nodeButtonGeometry(int index, int nodeCount)
{
  const int areaX = 24;
  const int areaY = 24;
  const int areaWidth = 910;
  const int areaHeight = 720;
  const int gap = 12;

  int columns = 2;
  if (nodeCount > 10)
    columns = 3;
  if (nodeCount > 18)
    columns = 4;

  const int rows = qMax(1, (nodeCount + columns - 1) / columns);
  const int width = (areaWidth - ((columns - 1) * gap)) / columns;
  const int height = qMin(105, (areaHeight - ((rows - 1) * gap)) / rows);
  const int column = index % columns;
  const int row = index / columns;

  return QRect(areaX + column * (width + gap),
               areaY + row * (height + gap),
               width,
               height);
}

} // namespace

MainWindow::MainWindow(LinRuntime *runtime,
                       DebugSink *debugSink,
                       DebugSnapshotSource *debugSource,
                       QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow),
    debug(debugSink),
    linRuntime(runtime),
    masterButton(0),
    exitButton(0),
    debugQuickButton(0),
    slaveFrame(0),
    productVerifyFrame(0),
    masterFrame(0),
    debugPanel(0)
{
  Q_ASSERT(linRuntime != 0);
  Q_ASSERT(debugSource != 0);
  ui->setupUi(this);
  setMinimumSize(kApplicationWindowSize);
  setMaximumSize(kApplicationWindowSize);
  setStyleSheet(kMainWindowStyle);
  setWindowFlags(Qt::FramelessWindowHint);
  showFullScreen();

  QFont controlFont;
  controlFont.setPointSize(22);

  const LinLayout &profile = linRuntime->layout();
  const bool layoutValid = linRuntime->isLayoutValid();
  const bool nodeConfigurationAvailable =
    layoutValid &&
    (profile.diagnosticModel == ELinDiagnosticModelCustomDid) &&
    (profile.serviceCount > 0);
  if (layoutValid)
  {
    for (int index = 0; index < profile.nodeCount; ++index)
    {
      const LinNodeLayout &node = profile.nodes[index];
      SlaveButton *button = new SlaveButton(this,
                                            node.diagnosticNad,
                                            QString::fromLatin1(node.name));
      button->setFont(controlFont);
      button->setGeometry(nodeButtonGeometry(node.uiSlot, profile.nodeCount));
      button->setFeedbackAvailable(false);
      button->setConfigurationEnabled(nodeConfigurationAvailable);
      connect(button, SIGNAL(slaveClicked(int)),
              this, SLOT(slaveConfig(int)));
      if (!nodeConfigurationAvailable)
      {
        button->setToolTip(
          tr("Open node status. Proprietary DID read/write is not "
             "defined by this LDF."));
      }
      button->show();
      slaveButtons.insert(node.diagnosticNad, button);
    }
  }
  else if (debug != 0)
  {
    debug->setValue(DebugLastError,
                    QString("Invalid LIN layout: %1")
                    .arg(linRuntime->layoutErrorText()));
  }

  masterButton = new BCMMasterButton(this);
  masterButton->setText("BCM\nMaster Control");
  masterButton->setGeometry(995, 45, 315, 315);
  masterButton->setFont(controlFont);
  masterButton->setEnabled(layoutValid);
  if (layoutValid)
  {
    connect(masterButton, SIGNAL(clicked()),
            this, SLOT(enterMasterConfigMode()));
  }
  else
  {
    masterButton->setToolTip(linRuntime->layoutErrorText());
  }
  masterButton->show();

  exitButton = new BCMMasterButton(this);
  exitButton->setText("Exit");
  exitButton->setGeometry(990, 390, 155, 155);
  exitButton->setFont(controlFont);
  exitButton->setButtonMode(EButtonModeExit);
  connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
  exitButton->show();

  /* One scheduler is explicitly injected into every feature page. */
  if (layoutValid)
  {
    masterFrame = new BCMMasterFrame(linRuntime, this);
    productVerifyFrame = new ProductionVerify(linRuntime, this);
    slaveFrame = new SlaveFrameConfig(linRuntime, this);
    connect(slaveFrame, SIGNAL(configurationReady(int)),
            this, SLOT(showSlaveConfig(int)));
  }

  debugPanel = new DebugPanel(debugSource, this);
  debugQuickButton = new QPushButton(tr("DEBUG  F12"), this);
  debugQuickButton->setGeometry(1160, 705, 175, 45);
  debugQuickButton->setStyleSheet(
    "QPushButton{color:#ffff80;border:2px solid #ffff80;border-radius:7px;"
    "background-color:rgba(0,0,0,190);font-size:18px;}");
  connect(debugQuickButton, SIGNAL(clicked()),
          debugPanel, SLOT(togglePanel()));

  if (!layoutValid)
  {
    QLabel *layoutErrorLabel = new QLabel(this);
    layoutErrorLabel->setGeometry(45, 250, 880, 210);
    layoutErrorLabel->setWordWrap(true);
    layoutErrorLabel->setAlignment(Qt::AlignCenter);
    layoutErrorLabel->setStyleSheet(
      "QLabel{color:#ff6666;border:2px solid #ff6666;border-radius:12px;"
      "background-color:rgba(35,0,0,220);font-size:22px;padding:18px;}");
    layoutErrorLabel->setText(
      QString("LIN layout invalid\n%1\nPress F12 for Debug")
      .arg(linRuntime->layoutErrorText()));
    layoutErrorLabel->show();
    layoutErrorLabel->raise();
  }

  QShortcut *debugShortcut = new QShortcut(QKeySequence(Qt::Key_F12), this);
  connect(debugShortcut, SIGNAL(activated()),
          debugPanel, SLOT(togglePanel()));

  connect(linRuntime, SIGNAL(SlaveStatusChanged(SlaveStatus)),
          this, SLOT(slaveStatusHandle(SlaveStatus)));

  if (layoutValid)
    linRuntime->start();
  if (debug != 0)
    debug->setValue(DebugAppState,
                    layoutValid
                    ? QString("Application running")
                    : QString("Application running with layout error"));

  if (layoutValid)
    enterMasterConfigMode();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slaveStatusHandle(SlaveStatus status)
{
  SlaveButton *button = slaveButtons.value(status.slaveNAD, 0);
  if (button != 0)
  {
    button->setFeedbackAvailable(status.isOnLine);
    const LinLayout &profile = linRuntime->layout();
    button->setConfigurationEnabled(
      (profile.diagnosticModel == ELinDiagnosticModelCustomDid) &&
      (profile.serviceCount > 0));
  }
}

void MainWindow::enterMasterConfigMode()
{
  if ((masterFrame != 0) && masterFrame->isHidden())
  {
    masterFrame->setGeometry(0, 0, 1366, 768);
    masterFrame->init();
    masterFrame->show();
    if (linRuntime->layout().colorModel == ELinColorModelGenericSignals)
      masterFrame->showSignalControl();
    keepDebugAccessVisible();
  }
}

void MainWindow::slaveConfig(int nodeAddress)
{
  SlaveButton *button = slaveButtons.value(
    static_cast<quint8>(nodeAddress), 0);
  if ((button == 0) || !button->hasValidFeedback())
  {
    if (debug != 0)
    {
      debug->setValue(
        DebugLastError,
        QString("Diagnostic page blocked: NAD %1 has no valid status feedback")
        .arg(nodeAddress));
    }
    return;
  }

  if ((slaveFrame != 0) && slaveFrame->isHidden())
  {
    slaveFrame->setGeometry(0, 0, 1366, 768);
    slaveFrame->SlaveFrameConfigInit(nodeAddress);
  }
}

void MainWindow::showSlaveConfig(int nodeAddress)
{
  SlaveButton *button = slaveButtons.value(
    static_cast<quint8>(nodeAddress), 0);
  if ((slaveFrame == 0) || (button == 0) ||
      !button->hasValidFeedback())
    return;

  slaveFrame->setGeometry(0, 0, 1366, 768);
  slaveFrame->show();
  slaveFrame->raise();
  keepDebugAccessVisible();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->ignore();
  if (debug != 0)
    debug->setValue(DebugAppState, QString("Application stopping"));

  if (linRuntime->stop(5000))
  {
    if (debug != 0)
      debug->setValue(DebugAppState, QString("Application stopped"));
    event->accept();
  }
  else
  {
    debugPanel->openPanel();
  }
}

void MainWindow::keepDebugAccessVisible()
{
  if (debugQuickButton != 0)
  {
    debugQuickButton->show();
    debugQuickButton->raise();
  }

  if ((debugPanel != 0) && debugPanel->isVisible())
    debugPanel->raise();
}
