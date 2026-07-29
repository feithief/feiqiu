#include "ambientlinscheduler.h"

#include "debugsink.h"
#include "linbusworker.h"
#include "linlayout.h"

#include <QThread>
#include <QTimer>
#include <QVariant>

AmbientLinScheduler::AmbientLinScheduler(const LinLayout &layout,
                                         const LinTransportFactory *factory,
                                         DebugSink *debugSink,
                                         QObject *parent)
  : LinRuntime(parent),
    linLayout(&layout),
    transportFactory(factory),
    debug(debugSink),
    workerThread(new QThread(this)),
    worker(0),
    controlCoalesceTimer(new QTimer(this)),
    startedOnce(false),
    workerReady(false),
    validLayout(false),
    desiredControlSignal(),
    requestSequence(0)
{
  qRegisterMetaType<BCMSignal>("BCMSignal");
  qRegisterMetaType<SlaveStatus>("SlaveStatus");
  qRegisterMetaType<CalibrationInfo>("CalibrationInfo");
  qRegisterMetaType<ENodeType>("ENodeType");
  qRegisterMetaType<SlaveConfigInfo>("SlaveConfigInfo");

  controlCoalesceTimer->setInterval(20);
  controlCoalesceTimer->setSingleShot(true);
  connect(controlCoalesceTimer, &QTimer::timeout,
          this, &AmbientLinScheduler::flushControlSignal);

  QStringList errors;
  validLayout = validateLinLayout(layout, &errors);
  if (transportFactory == 0)
  {
    errors.append(QString("LIN transport factory is null"));
    validLayout = false;
  }
  if (validLayout)
    desiredControlSignal = createDefaultBCMSignal(layout);
  else
    validationError = errors.join("; ");

  if (debug != 0)
  {
    debug->setValue(DebugSchedulerState,
                    validLayout ? QString("Created") : QString("Layout error"));
    if (!validLayout)
      debug->setValue(DebugLastError,
                      QString("Invalid LIN layout: %1").arg(validationError));
  }
}

AmbientLinScheduler::~AmbientLinScheduler()
{
  if (workerThread->isRunning() && !stop(5000))
  {
    /* A QThread must never be destroyed while it is still running. */
    workerThread->requestInterruption();
    emit stopRequested();
    workerThread->wait();
  }
}

void AmbientLinScheduler::createWorker()
{
  assertFacadeThread();
  Q_ASSERT(worker.isNull());

  worker = new LinBusWorker(linLayout,
                            desiredControlSignal,
                            transportFactory,
                            debug);
  worker->moveToThread(workerThread);

  connect(workerThread, &QThread::started,
          worker.data(), &LinBusWorker::initialize);
  connect(this, &AmbientLinScheduler::stopRequested,
          worker.data(), &LinBusWorker::stopWorker, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::controlSignalRequested,
          worker.data(), &LinBusWorker::updateControlSignal, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::switchControlSignalRequested,
          worker.data(), &LinBusWorker::switchControlSignal, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::signalPresetRequested,
          worker.data(), &LinBusWorker::applySignalPreset, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::readNodeRequested,
          worker.data(), &LinBusWorker::enqueueReadNode, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::writeNodeRequested,
          worker.data(), &LinBusWorker::enqueueWriteNode, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::calibrationRequested,
          worker.data(), &LinBusWorker::enqueueCalibration, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::cancelRequested,
          worker.data(), &LinBusWorker::cancelRequest, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::sleepRequested,
          worker.data(), &LinBusWorker::sleepBus, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::wakeRequested,
          worker.data(), &LinBusWorker::wakeBus, Qt::QueuedConnection);
  connect(this, &AmbientLinScheduler::busEnabledRequested,
          worker.data(), &LinBusWorker::setBusEnabled, Qt::QueuedConnection);

  connect(worker.data(), &LinBusWorker::slaveStatusChanged,
          this, &LinRuntime::SlaveStatusChanged, Qt::QueuedConnection);
  connect(worker.data(), &LinBusWorker::nodeConfigurationRead,
          this, &LinRuntime::nodeConfigurationRead,
          Qt::QueuedConnection);
  connect(worker.data(), &LinBusWorker::nodeConfigurationWritten,
          this, &LinRuntime::nodeConfigurationWritten,
          Qt::QueuedConnection);
  connect(worker.data(), &LinBusWorker::calibrationFinished,
          this, &LinRuntime::calibrationFinished,
          Qt::QueuedConnection);
  connect(worker.data(), &LinBusWorker::busStateChanged,
          this, &AmbientLinScheduler::handleWorkerBusStateChanged,
          Qt::QueuedConnection);

  /* quit() is thread-safe; direct connection also works while GUI waits. */
  connect(worker.data(), &LinBusWorker::stopped,
          workerThread, &QThread::quit, Qt::DirectConnection);
  connect(workerThread, &QThread::finished,
          worker.data(), &QObject::deleteLater);
}

void AmbientLinScheduler::start()
{
  assertFacadeThread();

  if (workerThread->isRunning())
    return;

  if (!validLayout)
    return;

  if (startedOnce)
  {
    if (debug != 0)
      debug->setValue(DebugLastError,
                      QString("A stopped LIN worker cannot be restarted"));
    return;
  }

  startedOnce = true;
  workerReady = false;

  if (debug != 0)
    debug->setValue(DebugSchedulerState, QString("Starting"));

  createWorker();
  workerThread->start(QThread::NormalPriority);
}

bool AmbientLinScheduler::stop(unsigned long timeoutMs)
{
  assertFacadeThread();
  workerReady = false;

  if (!workerThread->isRunning())
    return true;

  workerThread->requestInterruption();
  emit stopRequested();
  const bool stoppedInTime = workerThread->wait(timeoutMs);

  if (!stoppedInTime && (debug != 0))
  {
    debug->setValue(DebugSchedulerState, QString("Stop timeout"));
    debug->setValue(DebugLastError,
                    QString("LIN worker did not stop within %1 ms")
                    .arg(timeoutMs));
  }

  return stoppedInTime;
}

bool AmbientLinScheduler::isRunning() const
{
  assertFacadeThread();
  return workerThread->isRunning();
}

bool AmbientLinScheduler::isReady() const
{
  assertFacadeThread();
  return workerReady && workerThread->isRunning();
}

const LinLayout &AmbientLinScheduler::layout() const
{
  assertFacadeThread();
  return *linLayout;
}

bool AmbientLinScheduler::isLayoutValid() const
{
  assertFacadeThread();
  return validLayout;
}

QString AmbientLinScheduler::layoutErrorText() const
{
  assertFacadeThread();
  return validationError;
}

void AmbientLinScheduler::setBCMSignal(const BCMSignal &signal)
{
  assertFacadeThread();
  desiredControlSignal = signal;

  /* UI sliders may emit rapidly; only the latest value is sent every 20 ms. */
  if (!controlCoalesceTimer->isActive())
    controlCoalesceTimer->start();
}

void AmbientLinScheduler::switchBCMSignal(const BCMSignal &signal)
{
  assertFacadeThread();

  controlCoalesceTimer->stop();
  desiredControlSignal = signal;
  emit switchControlSignalRequested(signal);
}

BCMSignal AmbientLinScheduler::getBCMSignal() const
{
  assertFacadeThread();
  return desiredControlSignal;
}

void AmbientLinScheduler::applySignalPreset(int presetIndex)
{
  assertFacadeThread();

  if (!validLayout || (linLayout == 0) ||
      (presetIndex < 0) || (presetIndex >= linLayout->signalPresetCount))
    return;

  /* A preset is an immediate whole-combination command.  Do not let an older
   * coalesced slider update overwrite it after it reaches the worker. */
  controlCoalesceTimer->stop();
  emit signalPresetRequested(presetIndex);
}

void AmbientLinScheduler::flushControlSignal()
{
  assertFacadeThread();
  emit controlSignalRequested(desiredControlSignal);
}

quint32 AmbientLinScheduler::nextRequestId()
{
  assertFacadeThread();
  ++requestSequence;
  if (requestSequence == 0)
    ++requestSequence;
  return requestSequence;
}

bool AmbientLinScheduler::canSubmitDiagnosticRequest() const
{
  assertFacadeThread();
  return validLayout &&
         (linLayout != 0) &&
         (linLayout->diagnosticModel == ELinDiagnosticModelCustomDid) &&
         (linLayout->serviceCount > 0) &&
         workerReady &&
         workerThread->isRunning() &&
         !worker.isNull();
}

quint32 AmbientLinScheduler::readNodeConfiguration(quint8 node)
{
  assertFacadeThread();

  if (!canSubmitDiagnosticRequest())
  {
    if (debug != 0)
      debug->setValue(DebugLastError,
                      QString("Diagnostic request rejected: scheduler is not running"));
    return 0;
  }

  const quint32 requestId = nextRequestId();
  emit readNodeRequested(requestId, node);
  return requestId;
}

quint32 AmbientLinScheduler::writeNodeConfiguration(const SlaveConfigInfo &info)
{
  assertFacadeThread();

  if (!canSubmitDiagnosticRequest())
  {
    if (debug != 0)
      debug->setValue(DebugLastError,
                      QString("Diagnostic request rejected: scheduler is not running"));
    return 0;
  }

  const quint32 requestId = nextRequestId();
  emit writeNodeRequested(requestId, info);
  return requestId;
}

quint32 AmbientLinScheduler::calibrateNode(quint8 node, quint8 mode)
{
  assertFacadeThread();

  if (!canSubmitDiagnosticRequest())
  {
    if (debug != 0)
      debug->setValue(DebugLastError,
                      QString("Diagnostic request rejected: scheduler is not running"));
    return 0;
  }

  const quint32 requestId = nextRequestId();
  emit calibrationRequested(requestId, node, mode);
  return requestId;
}

void AmbientLinScheduler::cancel(quint32 requestId)
{
  assertFacadeThread();
  emit cancelRequested(requestId);
}

void AmbientLinScheduler::setReservedDebugValue(int reservedIndex,
                                                const QString &name,
                                                const QVariant &value)
{
  assertFacadeThread();
  if (debug != 0)
    debug->setReserved(reservedIndex, name, value);
}

void AmbientLinScheduler::sleepBus()
{
  assertFacadeThread();
  emit sleepRequested();
}

void AmbientLinScheduler::wakeBus()
{
  assertFacadeThread();
  emit wakeRequested();
}

void AmbientLinScheduler::setBusEnabled(bool enabled)
{
  assertFacadeThread();
  emit busEnabledRequested(enabled);
}

void AmbientLinScheduler::handleWorkerBusStateChanged(bool ready,
                                                      QString message)
{
  assertFacadeThread();
  workerReady = ready;
  if (debug != 0)
    debug->setValue(DebugSchedulerState,
                    ready ? QString("Ready") : QString("Not ready"));
  emit busStateChanged(ready, message);
}

void AmbientLinScheduler::assertFacadeThread() const
{
  Q_ASSERT(QThread::currentThread() == thread());
}
