#include "linbusworker.h"

#include "ambientlincomm.h"
#include "debugstore.h"
#include "linlayout.h"

#include <QThread>
#include <QTimer>

namespace {

QString toHexText(const QByteArray &value)
{
  const QByteArray hex = value.toHex(' ').toUpper();
  return QString::fromLatin1(hex.constData(), hex.size());
}

} // namespace

LinBusWorker::LinBusWorker(const LinLayout *layout,
                           const BCMSignal &initialSignal,
                           DebugStore *debugStore)
  : linLayout(layout),
    debug(debugStore),
    comm(0),
    scheduleTimer(0),
    controlSignal(initialSignal),
    initialized(false),
    busEnabled(true),
    stopping(false),
    scheduleIndex(0),
    taskActive(false),
    activeTaskCancelled(false),
    temporaryNadActive(false),
    activeSecurityUnlocked(false),
    activeInitialNad(0),
    activeStepIndex(0),
    pendingBusAction(EPendingBusActionNone),
    controlSwitchPending(false),
    pendingControlSignal(initialSignal),
    priorityControlPending(false),
    normalSlotRequiredAfterPriority(false),
    taskControlYieldPending(false),
    taskControlYieldServed(false),
    diagnosticResponsePending(false)
{
  activeTask.kind = ETaskNone;
  activeTask.requestId = 0;
  activeTask.node = 0;
  activeTask.calibrationMode = 0;
}

void LinBusWorker::initialize()
{
  if (initialized || (linLayout == 0))
    return;

  if (QThread::currentThread()->isInterruptionRequested())
    return;

  if (debug != 0)
    debug->setValue(DebugSchedulerState, QString("Initializing"));

  QStringList layoutErrors;
  if (!validateLinLayout(*linLayout, &layoutErrors))
  {
    const QString error = QString("Invalid LIN layout: %1")
                          .arg(layoutErrors.join("; "));
    if (debug != 0)
    {
      debug->setValue(DebugSchedulerState, QString("Layout error"));
      debug->setValue(DebugLastError, error);
    }
    emit busStateChanged(false, error);
    return;
  }

  if (!refreshPrimaryControlPayload())
  {
    const QString error = QString("Cannot initialize primary control payload");
    if (debug != 0)
      debug->setValue(DebugLastError, error);
    emit busStateChanged(false, error);
    return;
  }

  comm = new AmbientLinComm(QString::fromLatin1(linLayout->deviceName),
                            linLayout->baudRate,
                            linLayout->serialIoTimeoutMs);
  const bool opened = comm->openDevice();
  recordIoResult(opened);

  if (!opened)
  {
    const QString error = comm->lastErrorText();
    if (debug != 0)
      debug->setValue(DebugDeviceState, QString("Open failed"));
    emit busStateChanged(false, error);
    delete comm;
    comm = 0;
    return;
  }

  if (QThread::currentThread()->isInterruptionRequested())
    return;

  scheduleTimer = new QTimer(this);
  scheduleTimer->setSingleShot(true);
  connect(scheduleTimer, &QTimer::timeout,
          this, &LinBusWorker::processScheduleSlot);

  initialized = true;
  if (debug != 0)
  {
    debug->setValue(DebugSchedulerState, QString("Running"));
    debug->setValue(DebugDeviceState, QString("Ready"));
  }

  emit busStateChanged(true, QString("LIN bus ready"));
  resumeSchedule();
}

void LinBusWorker::stopWorker()
{
  if (stopping)
    return;

  stopping = true;
  busEnabled = false;
  pendingBusAction = EPendingBusActionNone;
  controlSwitchPending = false;
  rejectQueuedTasks(QString("Application is stopping"));
  activeTaskCancelled = taskActive;

  if (scheduleTimer != 0)
    scheduleTimer->stop();

  if (debug != 0)
    debug->setValue(DebugSchedulerState, QString("Stopping"));

  if (!taskActive)
    finishStopping();
}

void LinBusWorker::updateControlSignal(BCMSignal signal)
{
  controlSignal = signal;
  if (!refreshPrimaryControlPayload())
    return;
  if (debug != 0)
    debug->setValue(DebugActiveSignalPreset, QString("Manual UI"));

  /* A newer ordinary edit must also supersede an older delayed target edit. */
  if (controlSwitchPending)
    pendingControlSignal = signal;

  requestPriorityControlTransmission();
}

void LinBusWorker::applySignalPreset(int presetIndex)
{
  if ((linLayout == 0) ||
      (presetIndex < 0) || (presetIndex >= linLayout->signalPresetCount))
    return;

  if (primaryControlPayload.isEmpty() && !refreshPrimaryControlPayload())
    return;

  QString error;
  if (!applySignalPresetToPrimaryFrame(*linLayout,
                                       presetIndex,
                                       &primaryControlPayload,
                                       &error))
  {
    if (debug != 0)
      debug->setValue(DebugLastError, error);
    return;
  }

  if (debug != 0)
  {
    debug->setValue(DebugActiveSignalPreset,
                    QString::fromUtf8(
                      linLayout->signalPresets[presetIndex].name));
  }

  requestPriorityControlTransmission();
}

void LinBusWorker::requestPriorityControlTransmission()
{

  /*
   * A control change must not wait for a complete status-poll cycle.  With
   * offline nodes, each response timeout can otherwise add 100 ms and make a
   * button click appear several seconds late.  Move the schedule cursor to
   * the first master-published frame and run it at high priority.  This slot
   * executes in the LIN worker thread, so it never races another bus write.
   */
  const int transmitSlot = primaryControlScheduleSlot();
  if (transmitSlot < 0)
    return;

  if (taskActive)
  {
    taskControlYieldPending = true;
    return;
  }

  const bool wasAlreadyPending = priorityControlPending;
  priorityControlPending = true;
  if ((scheduleTimer == 0) || stopping || taskActive || !initialized ||
      !busEnabled || (comm == 0))
    return;

  /* Do not restart the timer for every GUI event: the latest payload is
   * already stored and status polling keeps making progress. */
  if (wasAlreadyPending)
    return;

  const int delayMs = primaryControlDelayMs();
  if (!scheduleTimer->isActive() ||
      (scheduleTimer->remainingTime() > delayMs))
  {
    scheduleTimer->stop();
    scheduleTimer->start(delayMs);
  }
}

void LinBusWorker::switchControlSignal(BCMSignal signal)
{
  if (stopping)
    return;

  if (QThread::currentThread()->isInterruptionRequested())
    return;

  /* A later target switch supersedes any earlier pending bus action. */
  pendingBusAction = EPendingBusActionNone;

  if (taskActive)
  {
    pendingControlSignal = signal;
    controlSwitchPending = true;
    activeTaskCancelled = true;
    busEnabled = false;
    rejectQueuedTasks(QString("Cancelled by control-target switch"));
    if (scheduleTimer != 0)
      scheduleTimer->stop();
    if (debug != 0)
      debug->setValue(DebugSchedulerState, QString("Target switch pending"));
    return;
  }

  performControlSwitch(signal);
  if (!busEnabled)
    return;

  if (!taskQueue.isEmpty())
    startNextTask();
  else
    resumeSchedule();
}

void LinBusWorker::enqueueReadNode(quint32 requestId, quint8 node)
{
  PendingTask task;
  task.kind = ETaskReadNode;
  task.requestId = requestId;
  task.node = node;
  task.calibrationMode = 0;
  task.config = createEmptyConfig(node);

  QString rejection;
  if (requestId == 0)
    rejection = QString("Invalid diagnostic request ID");
  else if (stopping)
    rejection = QString("Application is stopping");
  else if (!initialized || (comm == 0))
    rejection = QString("LIN device is not ready");
  else if (!busEnabled)
    rejection = QString("LIN bus is disabled");
  else if ((linLayout == 0) || (findLinNode(*linLayout, node) == 0))
    rejection = QString("Node is not present in the active LIN layout");
  else if (linLayout->serviceCount <= 0)
    rejection = QString("No diagnostic services are configured");
  else if (taskQueue.size() >= linLayout->maximumDiagnosticQueueDepth)
    rejection = QString("Diagnostic request queue is full");

  if (!rejection.isEmpty())
  {
    emitTaskResult(task, task.config, false, rejection);
    return;
  }

  taskQueue.enqueue(task);
  startNextTask();
}

void LinBusWorker::enqueueWriteNode(quint32 requestId, SlaveConfigInfo info)
{
  PendingTask task;
  task.kind = ETaskWriteNode;
  task.requestId = requestId;
  task.node = info.slaveNode;
  task.calibrationMode = 0;
  task.config = info;

  QString rejection;
  if (requestId == 0)
    rejection = QString("Invalid diagnostic request ID");
  else if (stopping)
    rejection = QString("Application is stopping");
  else if (!initialized || (comm == 0))
    rejection = QString("LIN device is not ready");
  else if (!busEnabled)
    rejection = QString("LIN bus is disabled");
  else if ((linLayout == 0) || (findLinNode(*linLayout, task.node) == 0))
    rejection = QString("Node is not present in the active LIN layout");
  else if (linLayout->bulkWriteCount <= 0)
    rejection = QString("No bulk-write sequence is configured");
  else if (taskQueue.size() >= linLayout->maximumDiagnosticQueueDepth)
    rejection = QString("Diagnostic request queue is full");

  if (!rejection.isEmpty())
  {
    emitTaskResult(task, createEmptyConfig(task.node), false, rejection);
    return;
  }

  taskQueue.enqueue(task);
  startNextTask();
}

void LinBusWorker::enqueueCalibration(quint32 requestId,
                                      quint8 node,
                                      quint8 mode)
{
  PendingTask task;
  task.kind = ETaskCalibration;
  task.requestId = requestId;
  task.node = node;
  task.calibrationMode = mode;
  task.config = createEmptyConfig(node);

  QString rejection;
  const LinServiceLayout *service = (linLayout == 0)
                                    ? 0
                                    : findLinService(*linLayout,
                                                     EOperationTypeCalibration);
  if (requestId == 0)
    rejection = QString("Invalid diagnostic request ID");
  else if (stopping)
    rejection = QString("Application is stopping");
  else if (!initialized || (comm == 0))
    rejection = QString("LIN device is not ready");
  else if (!busEnabled)
    rejection = QString("LIN bus is disabled");
  else if ((linLayout == 0) || (findLinNode(*linLayout, node) == 0))
    rejection = QString("Node is not present in the active LIN layout");
  else if ((service == 0) || !service->writable || (mode > 3))
    rejection = QString("Calibration service or mode is invalid");
  else if (taskQueue.size() >= linLayout->maximumDiagnosticQueueDepth)
    rejection = QString("Diagnostic request queue is full");

  if (!rejection.isEmpty())
  {
    emitTaskResult(task, task.config, false, rejection);
    return;
  }

  taskQueue.enqueue(task);
  startNextTask();
}

void LinBusWorker::cancelRequest(quint32 requestId)
{
  if (taskActive && (activeTask.requestId == requestId))
    activeTaskCancelled = true;

  for (int index = taskQueue.size() - 1; index >= 0; --index)
  {
    if (taskQueue.at(index).requestId == requestId)
    {
      const PendingTask cancelled = taskQueue.at(index);
      taskQueue.removeAt(index);
      emitTaskResult(cancelled,
                     createEmptyConfig(cancelled.node),
                     false,
                     QString("Request cancelled"));
    }
  }
}

void LinBusWorker::sleepBus()
{
  if (stopping)
    return;

  if (QThread::currentThread()->isInterruptionRequested())
    return;

  busEnabled = false;
  rejectQueuedTasks(QString("Cancelled because LIN bus is sleeping"));
  if (scheduleTimer != 0)
    scheduleTimer->stop();

  if (taskActive)
  {
    pendingBusAction = EPendingBusActionSleep;
    if (debug != 0)
      debug->setValue(DebugSchedulerState, QString("Sleep pending"));
    return;
  }

  pendingBusAction = EPendingBusActionNone;
  performSleepBus();
}

void LinBusWorker::wakeBus()
{
  setBusEnabled(true);
}

void LinBusWorker::setBusEnabled(bool enabled)
{
  if (stopping)
    return;

  if (QThread::currentThread()->isInterruptionRequested())
    return;

  if (enabled && (!initialized || (comm == 0)))
  {
    busEnabled = false;
    if (debug != 0)
      debug->setValue(DebugSchedulerState,
                      QString("Cannot enable: device not ready"));
    return;
  }

  busEnabled = enabled;

  if (!enabled)
  {
    rejectQueuedTasks(QString("Cancelled because LIN bus is disabled"));
    if (scheduleTimer != 0)
      scheduleTimer->stop();
  }

  if (taskActive)
  {
    pendingBusAction = enabled ? EPendingBusActionEnable
                               : EPendingBusActionDisable;
    if (debug != 0)
      debug->setValue(DebugSchedulerState,
                      enabled ? QString("Enable pending")
                              : QString("Disable pending"));
    return;
  }

  pendingBusAction = EPendingBusActionNone;
  if (debug != 0)
    debug->setValue(DebugSchedulerState,
                    enabled ? QString("Running") : QString("Bus disabled"));

  if (enabled && !taskQueue.isEmpty())
    startNextTask();
  else
    resumeSchedule();
}

void LinBusWorker::processScheduleSlot()
{
  if (QThread::currentThread()->isInterruptionRequested())
  {
    busEnabled = false;
    if (scheduleTimer != 0)
      scheduleTimer->stop();
    return;
  }

  if (stopping || taskActive || !initialized || !busEnabled ||
      (comm == 0) || (linLayout->scheduleSlotCount <= 0))
    return;

  if ((scheduleIndex < 0) ||
      (scheduleIndex >= linLayout->scheduleSlotCount))
    scheduleIndex = 0;

  bool prioritySlot = false;
  int slotIndex = scheduleIndex;
  if (priorityControlPending && !normalSlotRequiredAfterPriority)
  {
    const int transmitSlot = primaryControlScheduleSlot();
    if (transmitSlot >= 0)
    {
      slotIndex = transmitSlot;
      prioritySlot = true;
    }
  }

  const LinScheduleSlot &slot = linLayout->schedule[slotIndex];
  if (slot.type == ELinScheduleTransmitFrame)
  {
    transmitPublishedFrame(slot.tableIndex);
  }
  else if (slot.type == ELinSchedulePollNode)
  {
    const LinNodeLayout &node = linLayout->nodes[slot.tableIndex];
    if (debug != 0)
      debug->setValue(DebugCurrentNad,
                      static_cast<int>(node.diagnosticNad));

    const bool headerSent = comm->sendHeader(node.statusFrameId);
    recordTransmit(node.statusFrameId, QByteArray(), headerSent);
    bool success = headerSent;
    QByteArray response;
    if (headerSent)
    {
      if (!waitInterruptibly(linLayout->responseDelayMs))
        return;
      success = comm->readResponse(node.statusFrameId,
                                   node.statusLength,
                                   node.checksumMode,
                                   &response);
      recordReceive(node.statusFrameId, response, success);
    }
    else
    {
      success = false;
    }
    emit slaveStatusChanged(decodeStatusFrame(*linLayout,
                                              node,
                                              response,
                                              success));
  }

  if (prioritySlot)
  {
    priorityControlPending = false;
    normalSlotRequiredAfterPriority = true;
  }
  else
  {
    scheduleIndex = (scheduleIndex + 1) % linLayout->scheduleSlotCount;
    normalSlotRequiredAfterPriority = false;
  }
  if (QThread::currentThread()->isInterruptionRequested())
  {
    busEnabled = false;
    if (scheduleTimer != 0)
      scheduleTimer->stop();
    return;
  }

  if ((scheduleTimer != 0) && !stopping && !taskActive && busEnabled)
    scheduleTimer->start(slot.delayAfterMs);
}

void LinBusWorker::processTaskStep()
{
  if (!taskActive)
  {
    if (stopping)
      finishStopping();
    return;
  }

  if (activeTaskCancelled || shouldAbortCurrentTransaction())
  {
    finishActiveTask(false, QString("Request cancelled"));
    return;
  }

  /* A diagnostic operation may contain many DIDs.  Yield once between two
   * complete service transactions so a lamp command is not delayed for the
   * whole operation, while never interleaving a transport-layer exchange. */
  if (taskControlYieldPending && !taskControlYieldServed)
  {
    taskControlYieldPending = false;
    taskControlYieldServed = true;
    transmitPrimaryControlFrame();
    QTimer::singleShot(primaryControlDelayMs(),
                       this,
                       SLOT(processTaskStep()));
    return;
  }
  taskControlYieldServed = false;

  if (!initialized || (comm == 0))
  {
    finishActiveTask(false, QString("LIN device is not ready"));
    return;
  }

  if (activeTask.kind == ETaskReadNode)
  {
    while ((activeStepIndex < linLayout->serviceCount) &&
           (!linLayout->services[activeStepIndex].readable ||
            !linLayout->services[activeStepIndex].readOnConfiguration ||
             !isLinServiceApplicable(linLayout->services[activeStepIndex],
                                     activeReadResult.nodeType)))
      ++activeStepIndex;

    if (activeStepIndex >= linLayout->serviceCount)
    {
      finishActiveTask(true, QString());
      return;
    }

    const LinServiceLayout &service = linLayout->services[activeStepIndex++];
    QByteArray value;
    if (!readServiceValue(activeTask.node, service, &value))
    {
      finishActiveTask(false,
                       QString("Read %1 failed: %2")
                       .arg(QString::fromLatin1(service.name))
                       .arg(transactionErrorText()));
      return;
    }

    applyLinServiceValue(service, value, &activeReadResult);
    QTimer::singleShot(0, this, SLOT(processTaskStep()));
    return;
  }

  if (activeTask.kind == ETaskWriteNode)
  {
    if (activeStepIndex >= linLayout->bulkWriteCount)
    {
      finishActiveTask(true, QString());
      return;
    }

    const OperationType operation =
      linLayout->bulkWriteSequence[activeStepIndex++];

    const LinServiceLayout *service = findLinService(*linLayout, operation);
    if ((service == 0) || !service->writable)
    {
      finishActiveTask(false, QString("Writable service is missing from layout"));
      return;
    }

    if (!isLinServiceApplicable(*service, activeTask.config.nodeType))
    {
      QTimer::singleShot(0, this, SLOT(processTaskStep()));
      return;
    }

    QByteArray value;
    QString encodeError;
    if (!encodeLinServiceValue(*service,
                               activeTask.config,
                               &value,
                               &encodeError) ||
        !writeServiceValue(activeTask.node, *service, value))
    {
      const QString error = encodeError.isEmpty() ? transactionErrorText()
                                                   : encodeError;
      finishActiveTask(false,
                       QString("Write %1 failed: %2")
                       .arg(QString::fromLatin1(service->name))
                       .arg(error));
      return;
    }

    QTimer::singleShot(0, this, SLOT(processTaskStep()));
    return;
  }

  if (activeTask.kind == ETaskCalibration)
  {
    const LinServiceLayout *service = findLinService(
      *linLayout,
      EOperationTypeCalibration);
    if ((service == 0) || !service->writable ||
        (activeTask.calibrationMode > 3))
    {
      finishActiveTask(false, QString("Calibration service or mode is invalid"));
      return;
    }

    QByteArray value;
    value.append(static_cast<char>(activeTask.calibrationMode));
    const bool success = writeServiceValue(activeTask.node, *service, value);
    finishActiveTask(success,
                     success ? QString() : transactionErrorText());
    return;
  }

  finishActiveTask(false, QString("Unknown worker task"));
}

void LinBusWorker::startNextTask()
{
  if (stopping || taskActive || !busEnabled || taskQueue.isEmpty())
    return;

  if (scheduleTimer != 0)
    scheduleTimer->stop();

  activeTask = taskQueue.dequeue();
  taskActive = true;
  activeTaskCancelled = false;
  temporaryNadActive = false;
  activeSecurityUnlocked = false;
  activeInitialNad = activeTask.node;
  activeStepIndex = 0;
  diagnosticResponsePending = false;
  activeReadResult = createEmptyConfig(activeTask.node);

  if (debug != 0)
  {
    debug->setValue(DebugDiagnosticState,
                    QString("Request %1, node %2")
                    .arg(activeTask.requestId)
                    .arg(static_cast<int>(activeTask.node)));
    debug->setValue(DebugCurrentNad, static_cast<int>(activeTask.node));
  }

  QTimer::singleShot(0, this, SLOT(processTaskStep()));
}

void LinBusWorker::finishActiveTask(bool success, const QString &errorMessage)
{
  const PendingTask completed = activeTask;
  const SlaveConfigInfo readResult = activeReadResult;
  QString finalError = errorMessage;

  /*
   * Assign-NAD is a temporary diagnostic session switch.  Always put the
   * slave back on its Initial NAD before completing, including cancellation,
   * protocol errors and application shutdown.
   */
  if (temporaryNadActive && !restoreInitialNad(true))
  {
    const QString restoreError = transactionErrorText();
    finalError = finalError.isEmpty()
                 ? QString("Initial NAD restoration failed: %1")
                   .arg(restoreError)
                 : QString("%1; Initial NAD restoration failed: %2")
                   .arg(finalError)
                   .arg(restoreError);
    success = false;
  }

  taskActive = false;
  activeTaskCancelled = false;
  temporaryNadActive = false;
  activeSecurityUnlocked = false;
  activeInitialNad = 0;
  taskControlYieldPending = false;
  taskControlYieldServed = false;
  diagnosticResponsePending = false;
  activeTask.kind = ETaskNone;

  if (debug != 0)
  {
    debug->setValue(DebugDiagnosticState,
                    success ? QString("Idle") : QString("Failed"));
    if (!success && !finalError.isEmpty())
      debug->setValue(DebugLastError, finalError);
  }

  emitTaskResult(completed, readResult, success, finalError);

  if (!stopping && QThread::currentThread()->isInterruptionRequested())
  {
    busEnabled = false;
    if (scheduleTimer != 0)
      scheduleTimer->stop();
    return;
  }

  if (stopping)
  {
    finishStopping();
    return;
  }

  if (controlSwitchPending)
  {
    const BCMSignal signal = pendingControlSignal;
    controlSwitchPending = false;
    performControlSwitch(signal);
  }

  if (QThread::currentThread()->isInterruptionRequested())
  {
    busEnabled = false;
    return;
  }

  applyPendingBusAction();
  if (!busEnabled)
    return;

  if (!taskQueue.isEmpty())
    startNextTask();
  else
    resumeSchedule();
}

void LinBusWorker::emitTaskResult(const PendingTask &task,
                                  const SlaveConfigInfo &readResult,
                                  bool success,
                                  const QString &errorMessage)
{
  switch (task.kind)
  {
    case ETaskReadNode:
      emit nodeConfigurationRead(task.requestId,
                                 readResult,
                                 success,
                                 errorMessage);
      break;
    case ETaskWriteNode:
      emit nodeConfigurationWritten(task.requestId,
                                    task.node,
                                    success,
                                    errorMessage);
      break;
    case ETaskCalibration:
      emit calibrationFinished(task.requestId,
                               task.node,
                               success,
                               errorMessage);
      break;
    case ETaskNone:
      break;
  }
}

void LinBusWorker::rejectQueuedTasks(const QString &errorMessage)
{
  while (!taskQueue.isEmpty())
  {
    const PendingTask task = taskQueue.dequeue();
    emitTaskResult(task,
                   createEmptyConfig(task.node),
                   false,
                   errorMessage);
  }
}

void LinBusWorker::performSleepBus()
{
  busEnabled = false;

  if (initialized && (comm != 0))
  {
    /* LIN 2.x go-to-sleep command: NAD 00 followed by seven FF bytes. */
    QByteArray sleepFrame(8, static_cast<char>(0xFF));
    sleepFrame[0] = static_cast<char>(0x00);
    const bool success = comm->sendFrame(
      linLayout->diagnosticRequestFrameId,
      sleepFrame,
      ELinChecksumClassic);
    recordTransmit(linLayout->diagnosticRequestFrameId,
                   sleepFrame,
                   success);
  }

  if (debug != 0)
    debug->setValue(DebugSchedulerState,
                    initialized && (comm != 0)
                    ? QString("Bus sleeping")
                    : QString("Bus disabled: device not ready"));
}

void LinBusWorker::performControlSwitch(const BCMSignal &signal)
{
  if (scheduleTimer != 0)
    scheduleTimer->stop();

  controlSignal = signal;
  if (!refreshPrimaryControlPayload())
    return;
  if (debug != 0)
    debug->setValue(DebugActiveSignalPreset, QString("Manual UI"));
  priorityControlPending = true;
  normalSlotRequiredAfterPriority = false;
  const bool ready = initialized && (comm != 0);
  busEnabled = ready;

  /*
   * Switching the target mask is an ordinary high-priority control update.
   * Let the schedule send it once with the configured inter-frame spacing;
   * a direct write followed by start(0) used to create two adjacent frames.
   */
  if (debug != 0)
    debug->setValue(DebugSchedulerState,
                    ready ? QString("Running (target switched)")
                          : QString("Target stored: device not ready"));

  if (!ready)
    return;
}

void LinBusWorker::applyPendingBusAction()
{
  const PendingBusAction action = pendingBusAction;
  pendingBusAction = EPendingBusActionNone;

  switch (action)
  {
    case EPendingBusActionSleep:
      performSleepBus();
      break;
    case EPendingBusActionEnable:
      busEnabled = true;
      if (debug != 0)
        debug->setValue(DebugSchedulerState, QString("Running"));
      break;
    case EPendingBusActionDisable:
      busEnabled = false;
      if (debug != 0)
        debug->setValue(DebugSchedulerState, QString("Bus disabled"));
      break;
    case EPendingBusActionNone:
      break;
  }
}

void LinBusWorker::finishStopping()
{
  if (comm != 0)
  {
    comm->closeDevice();
    delete comm;
    comm = 0;
  }

  initialized = false;
  stopping = false;
  if (debug != 0)
  {
    debug->setValue(DebugSchedulerState, QString("Stopped"));
    debug->setValue(DebugDeviceState, QString("Closed"));
  }

  emit busStateChanged(false, QString("LIN bus stopped"));
  emit stopped();
}

void LinBusWorker::resumeSchedule()
{
  if ((scheduleTimer != 0) && initialized && busEnabled &&
      !taskActive && !stopping)
  {
    int delayMs = priorityControlPending &&
                  !normalSlotRequiredAfterPriority
                  ? primaryControlDelayMs()
                  : 0;
    if ((delayMs == 0) && (scheduleIndex >= 0) &&
        (scheduleIndex < linLayout->scheduleSlotCount) &&
        (linLayout->schedule[scheduleIndex].type ==
         ELinScheduleTransmitFrame))
      delayMs = linLayout->schedule[scheduleIndex].delayAfterMs;
    scheduleTimer->start(delayMs);
  }
}

int LinBusWorker::primaryControlScheduleSlot() const
{
  if (linLayout == 0)
    return -1;

  for (int slotIndex = 0;
       slotIndex < linLayout->scheduleSlotCount;
       ++slotIndex)
  {
    if (linLayout->schedule[slotIndex].type ==
        ELinScheduleTransmitFrame &&
        linLayout->schedule[slotIndex].tableIndex ==
        linLayout->primaryControlFrameIndex)
      return slotIndex;
  }

  return -1;
}

int LinBusWorker::primaryControlDelayMs() const
{
  const int slotIndex = primaryControlScheduleSlot();
  return (slotIndex >= 0) ? linLayout->schedule[slotIndex].delayAfterMs : 0;
}

bool LinBusWorker::refreshPrimaryControlPayload()
{
  if (linLayout == 0)
    return false;

  const LinFrameLayout *frameLayout = primaryControlFrame(*linLayout);
  if (frameLayout == 0)
    return false;

  QString error;
  if (!encodePublishedFrame(*frameLayout,
                            controlSignal,
                            &primaryControlPayload,
                            &error))
  {
    if (debug != 0)
      debug->setValue(DebugLastError, error);
    primaryControlPayload.clear();
    return false;
  }

  return true;
}

bool LinBusWorker::transmitPublishedFrame(int frameIndex)
{
  if ((linLayout == 0) || (comm == 0) ||
      (frameIndex < 0) || (frameIndex >= linLayout->publishedFrameCount))
    return false;

  const LinFrameLayout &frameLayout = linLayout->publishedFrames[frameIndex];
  QByteArray frame;
  QString encodeError;
  if ((frameIndex == linLayout->primaryControlFrameIndex) &&
      (primaryControlPayload.size() == frameLayout.length))
  {
    frame = primaryControlPayload;
  }
  else if (!encodePublishedFrame(frameLayout,
                                 controlSignal,
                                 &frame,
                                 &encodeError))
  {
    if (debug != 0)
      debug->setValue(DebugLastError, encodeError);
    return false;
  }

  const bool success = comm->sendFrame(frameLayout.frameId,
                                       frame,
                                       frameLayout.checksumMode);
  recordTransmit(frameLayout.frameId, frame, success);
  return success;
}

bool LinBusWorker::transmitPrimaryControlFrame()
{
  const int slotIndex = primaryControlScheduleSlot();
  if (slotIndex < 0)
    return false;

  priorityControlPending = false;
  return transmitPublishedFrame(linLayout->schedule[slotIndex].tableIndex);
}

bool LinBusWorker::shouldAbortCurrentTransaction() const
{
  return stopping || activeTaskCancelled ||
         QThread::currentThread()->isInterruptionRequested();
}

bool LinBusWorker::waitInterruptibly(int delayMs,
                                     bool allowDuringShutdown) const
{
  int remaining = qMax(0, delayMs);
  while (remaining > 0)
  {
    if (shouldAbortCurrentTransaction() && !allowDuringShutdown)
      return false;

    const int sleepMs = qMin(remaining, 10);
    QThread::msleep(static_cast<unsigned long>(sleepMs));
    remaining -= sleepMs;
  }

  return true;
}

bool LinBusWorker::sendDiagnosticFrame(const QByteArray &payload,
                                       bool allowDuringShutdown)
{
  diagnosticResponsePending = false;
  if (shouldAbortCurrentTransaction() && !allowDuringShutdown)
  {
    protocolError = QString("Request interrupted");
    return false;
  }

  const bool success = comm->sendFrame(linLayout->diagnosticRequestFrameId,
                                       payload,
                                       ELinChecksumClassic);
  recordTransmit(linLayout->diagnosticRequestFrameId, payload, success);
  if (!success)
  {
    protocolError = comm->lastErrorText();
    return false;
  }
  if (!waitInterruptibly(linLayout->diagnosticInterFrameDelayMs,
                         allowDuringShutdown))
  {
    protocolError = QString("Request interrupted");
    return false;
  }
  diagnosticResponsePending = true;
  return true;
}

bool LinBusWorker::receiveDiagnosticFrame(QByteArray *payload,
                                          bool allowDuringShutdown)
{
  if (payload == 0)
  {
    protocolError = QString("Diagnostic response buffer is null");
    return false;
  }
  payload->clear();

  if (shouldAbortCurrentTransaction() && !allowDuringShutdown)
  {
    protocolError = QString("Request interrupted");
    return false;
  }

  if (diagnosticResponsePending)
  {
    /* sendDiagnosticFrame() already waited ST after the request frame.  The
     * first response must satisfy both ST and P2, so wait only the remaining
     * part instead of adding the two protocol minima together. */
    const int remainingP2Ms = qMax(
      0,
      linLayout->diagnosticRequestToResponseDelayMs -
      linLayout->diagnosticInterFrameDelayMs);
    if (!waitInterruptibly(
          remainingP2Ms,
          allowDuringShutdown))
    {
      protocolError = QString("Request interrupted");
      return false;
    }
    diagnosticResponsePending = false;
  }

  const bool headerSent = comm->sendHeader(
    linLayout->diagnosticResponseFrameId);
  recordTransmit(linLayout->diagnosticResponseFrameId,
                 QByteArray(),
                 headerSent);
  if (!headerSent)
  {
    protocolError = comm->lastErrorText();
    return false;
  }

  if (!waitInterruptibly(linLayout->responseDelayMs,
                         allowDuringShutdown))
  {
    protocolError = QString("Request interrupted");
    return false;
  }

  const bool success = comm->readResponse(
    linLayout->diagnosticResponseFrameId,
    8,
    ELinChecksumClassic,
    payload);
  recordReceive(linLayout->diagnosticResponseFrameId, *payload, success);
  if (!success)
  {
    protocolError = comm->lastErrorText();
    return false;
  }

  /* Give the slave main loop time to prepare the next transport frame. */
  if (!waitInterruptibly(linLayout->diagnosticInterFrameDelayMs,
                         allowDuringShutdown))
  {
    protocolError = QString("Request interrupted");
    return false;
  }
  return true;
}

bool LinBusWorker::configureNad(quint8 initialNad,
                                quint8 requestedNad,
                                bool allowDuringShutdown)
{
  if (debug != 0)
    debug->setValue(DebugDiagnosticState,
                    QString("B0 NAD 0x%1 -> 0x%2")
                    .arg(static_cast<int>(initialNad), 2, 16, QChar('0'))
                    .arg(static_cast<int>(requestedNad), 2, 16, QChar('0'))
                    .toUpper());

  /* The slave's B0 callback only accepts Initial NAD, never current A0. */
  QByteArray request(8, static_cast<char>(0xFF));
  request[0] = static_cast<char>(initialNad);
  request[1] = static_cast<char>(0x06);
  request[2] = static_cast<char>(0xB0);
  request[3] = static_cast<char>(0xFF);
  request[4] = static_cast<char>(0x7F);
  request[5] = static_cast<char>(0xFF);
  request[6] = static_cast<char>(0xFF);
  request[7] = static_cast<char>(requestedNad);

  /*
   * Mark the session before transmitting.  If cancellation arrives after the
   * bytes leave the UART but before the ACK, finishActiveTask() must still
   * attempt the Initial-NAD cleanup.
   */
  if (requestedNad != initialNad)
  {
    temporaryNadActive = true;
    activeInitialNad = initialNad;
  }

  if (!sendDiagnosticFrame(request, allowDuringShutdown))
    return false;

  QByteArray response;
  if (!receiveDiagnosticFrame(&response, allowDuringShutdown))
    return false;

  const bool validAck =
    (response.size() == 8) &&
    (static_cast<quint8>(response.at(0)) == initialNad) &&
    (static_cast<quint8>(response.at(1)) == 0x01) &&
    (static_cast<quint8>(response.at(2)) == 0xF0);
  if (!validAck)
  {
    protocolError = QString("Unexpected Assign-NAD response; expected NAD 0x%1, PCI 01, SID F0")
                    .arg(static_cast<int>(initialNad), 2, 16, QChar('0'))
                    .toUpper();
    return false;
  }

  if (requestedNad == initialNad)
    temporaryNadActive = false;
  if (debug != 0)
    debug->setValue(DebugCurrentNad, static_cast<int>(requestedNad));
  return true;
}

bool LinBusWorker::selectServiceNad(quint8 initialNad,
                                    const LinServiceLayout &service,
                                    quint8 *requestNad)
{
  if (requestNad == 0)
  {
    protocolError = QString("Diagnostic NAD output pointer is null");
    return false;
  }

  const bool allowsNormal =
    (service.nadAccess & ELinDiagnosticNadNormal) != 0;
  const bool allowsCalibration =
    (service.nadAccess & ELinDiagnosticNadCalibration) != 0;

  if (!allowsNormal && !allowsCalibration)
  {
    protocolError = QString("Service has no valid NAD access mode");
    return false;
  }

  /* Keep consecutive calibration services in one A0 session. */
  if (temporaryNadActive)
  {
    if (activeInitialNad != initialNad)
    {
      protocolError = QString("Temporary NAD belongs to another node");
      return false;
    }
    if (allowsCalibration)
    {
      *requestNad = linLayout->temporaryNad;
      return true;
    }
    if (!restoreInitialNad())
      return false;
  }

  if (allowsNormal)
  {
    *requestNad = initialNad;
    return true;
  }

  activeInitialNad = initialNad;
  if (!configureNad(initialNad, linLayout->temporaryNad))
    return false;

  *requestNad = linLayout->temporaryNad;
  return true;
}

bool LinBusWorker::restoreInitialNad(bool allowDuringShutdown)
{
  if (!temporaryNadActive)
    return true;

  const quint8 initialNad = activeInitialNad;
  /* Restore is B0 [Initial NAD ... Initial NAD], not [A0 ... Initial NAD]. */
  const bool restored = configureNad(initialNad,
                                     initialNad,
                                     allowDuringShutdown);
  if (restored)
    temporaryNadActive = false;
  return restored;
}

bool LinBusWorker::unlockSecurity(quint8 requestNad)
{
  if (!linLayout->securityAccess.enabled || activeSecurityUnlocked)
    return true;

  if (debug != 0)
    debug->setValue(DebugDiagnosticState,
                    QString("27 SecurityAccess NAD 0x%1")
                    .arg(static_cast<int>(requestNad), 2, 16, QChar('0'))
                    .toUpper());

  quint32 seed = 0;
  bool usableSeed = false;
  for (int attempt = 0; attempt < 2; ++attempt)
  {
    QByteArray seedRequest(8, static_cast<char>(0xFF));
    seedRequest[0] = static_cast<char>(requestNad);
    seedRequest[1] = static_cast<char>(0x02);
    seedRequest[2] = static_cast<char>(0x27);
    seedRequest[3] = static_cast<char>(
      linLayout->securityAccess.requestSeedSubFunction);

    QByteArray seedResponse;
    if (!sendDiagnosticFrame(seedRequest) ||
        !receiveDiagnosticFrame(&seedResponse))
      return false;

    const bool validSeed =
      (seedResponse.size() == 8) &&
      (static_cast<quint8>(seedResponse.at(0)) == requestNad) &&
      (static_cast<quint8>(seedResponse.at(1)) == 0x06) &&
      (static_cast<quint8>(seedResponse.at(2)) == 0x67) &&
      (static_cast<quint8>(seedResponse.at(3)) ==
       linLayout->securityAccess.requestSeedSubFunction);
    if (!validSeed)
    {
      protocolError = QString("Unexpected SecurityAccess seed response");
      return false;
    }

    seed =
      static_cast<quint8>(seedResponse.at(4)) |
      (static_cast<quint32>(static_cast<quint8>(seedResponse.at(5))) << 8) |
      (static_cast<quint32>(static_cast<quint8>(seedResponse.at(6))) << 16) |
      (static_cast<quint32>(static_cast<quint8>(seedResponse.at(7))) << 24);

    /*
     * This slave compares a 16-bit key with an unmasked seed+offset.  During
     * the final keyAddend milliseconds of its 16-bit tick, no key can match.
     * Wait for the tick rollover once and request a fresh seed instead of
     * creating a predictable intermittent SecurityAccess failure.
     */
    const quint32 keySum = seed + linLayout->securityAccess.keyAddend;
    if (keySum <= 0xFFFFu)
    {
      usableSeed = true;
      break;
    }

    if (attempt == 0)
    {
      const int rolloverWaitMs = static_cast<int>(
        0x10000u - (seed & 0xFFFFu) + 2u);
      if (debug != 0)
        debug->setValue(DebugDiagnosticState,
                        QString("27 seed rollover wait: %1 ms")
                        .arg(rolloverWaitMs));
      if (!waitInterruptibly(rolloverWaitMs))
      {
        protocolError = QString("Request interrupted while waiting for a usable security seed");
        return false;
      }
    }
  }

  if (!usableSeed)
  {
    protocolError = QString("SecurityAccess seed cannot produce the slave's 16-bit key");
    return false;
  }

  const quint16 key = static_cast<quint16>(
    seed + linLayout->securityAccess.keyAddend);

  QByteArray keyRequest(8, static_cast<char>(0xFF));
  keyRequest[0] = static_cast<char>(requestNad);
  keyRequest[1] = static_cast<char>(0x04);
  keyRequest[2] = static_cast<char>(0x27);
  keyRequest[3] = static_cast<char>(
    linLayout->securityAccess.sendKeySubFunction);
  keyRequest[4] = static_cast<char>(key & 0xFF);
  keyRequest[5] = static_cast<char>((key >> 8) & 0xFF);

  QByteArray keyResponse;
  if (!sendDiagnosticFrame(keyRequest) ||
      !receiveDiagnosticFrame(&keyResponse))
    return false;

  const bool validKey =
    (keyResponse.size() == 8) &&
    (static_cast<quint8>(keyResponse.at(0)) == requestNad) &&
    (static_cast<quint8>(keyResponse.at(1)) == 0x02) &&
    (static_cast<quint8>(keyResponse.at(2)) == 0x67) &&
    (static_cast<quint8>(keyResponse.at(3)) ==
     linLayout->securityAccess.sendKeySubFunction);
  if (!validKey)
  {
    protocolError = QString("SecurityAccess key was rejected");
    return false;
  }

  /* A successful 27/04 schedules a flash save in this slave. */
  if (!waitInterruptibly(linLayout->postWriteSettleMs))
  {
    protocolError = QString("Request interrupted during SecurityAccess save");
    return false;
  }

  activeSecurityUnlocked = true;
  return true;
}

bool LinBusWorker::readServiceValue(quint8 nad,
                                    const LinServiceLayout &service,
                                    QByteArray *value)
{
  if (value == 0)
  {
    protocolError = QString("Diagnostic value output pointer is null");
    return false;
  }

  value->clear();
  protocolError.clear();
  quint8 requestNad = 0;
  bool success = selectServiceNad(nad, service, &requestNad);

  if (success &&
      (service.protocol == ELinDiagnosticProductIdentification))
  {
    if (debug != 0)
      debug->setValue(DebugDiagnosticState,
                      QString("B2 ProductId NAD 0x%1")
                      .arg(static_cast<int>(requestNad), 2, 16, QChar('0'))
                      .toUpper());
    QByteArray request(8, static_cast<char>(0xFF));
    request[0] = static_cast<char>(requestNad);
    request[1] = static_cast<char>(0x06);
    request[2] = static_cast<char>(0xB2);
    request[3] = static_cast<char>(0x00);
    request[4] = static_cast<char>(0xFF);
    request[5] = static_cast<char>(0x7F);
    request[6] = static_cast<char>(0xFF);
    request[7] = static_cast<char>(0xFF);

    QByteArray response;
    success = sendDiagnosticFrame(request) &&
              receiveDiagnosticFrame(&response) &&
              (response.size() == 8);
    if (success &&
        ((static_cast<quint8>(response.at(0)) != requestNad) ||
         (static_cast<quint8>(response.at(1)) != 0x06) ||
         (static_cast<quint8>(response.at(2)) != 0xF2)))
    {
      success = false;
      protocolError = QString("Unexpected product-identification response");
    }
    if (success)
      *value = response.mid(3, service.dataLength);
    return success;
  }

  if (!success)
    return false;

  if (debug != 0)
    debug->setValue(DebugDiagnosticState,
                    QString("22 DID 0x%1 NAD 0x%2")
                    .arg(static_cast<int>(service.serviceId),
                         4, 16, QChar('0'))
                    .arg(static_cast<int>(requestNad), 2, 16, QChar('0'))
                    .toUpper());

  QByteArray request(8, static_cast<char>(0xFF));
  request[0] = static_cast<char>(requestNad);
  request[1] = static_cast<char>(0x03);
  request[2] = static_cast<char>(0x22);
  request[3] = static_cast<char>((service.serviceId >> 8) & 0xFF);
  request[4] = static_cast<char>(service.serviceId & 0xFF);
  success = sendDiagnosticFrame(request);

  int frameCounter = 0;
  quint8 expectedSequence = 1;
  bool firstFrameReceived = false;
  const int maximumFrames = 3 + ((service.dataLength + 5) / 6);
  while (success && (value->size() < service.dataLength) &&
         (frameCounter < maximumFrames))
  {
    ++frameCounter;
    QByteArray response;
    success = receiveDiagnosticFrame(&response);
    if (!success || (response.size() != 8))
      break;

    if (static_cast<quint8>(response.at(0)) != requestNad)
    {
      success = false;
      protocolError = QString("Diagnostic response NAD does not match request");
      break;
    }

    const quint8 pci = static_cast<quint8>(response.at(1));
    if (((pci & 0xF0) == 0x00) &&
        ((pci & 0x0F) >= 3) &&
        (static_cast<quint8>(response.at(2)) == 0x7F))
    {
      success = false;
      protocolError = QString("Negative response: SID 0x%1, NRC 0x%2")
                      .arg(static_cast<int>(static_cast<quint8>(
                             response.at(3))), 2, 16, QChar('0'))
                      .arg(static_cast<int>(static_cast<quint8>(
                             response.at(4))), 2, 16, QChar('0'))
                      .toUpper();
      break;
    }

    const quint8 pciType = static_cast<quint8>(response.at(1)) & 0xF0;
    if (pciType == 0x00)
    {
      const int declaredLength =
        static_cast<quint8>(response.at(1)) & 0x0F;
      const bool identifierMatches =
        (static_cast<quint8>(response.at(2)) == 0x62) &&
        (static_cast<quint8>(response.at(3)) ==
         ((service.serviceId >> 8) & 0xFF)) &&
        (static_cast<quint8>(response.at(4)) ==
         (service.serviceId & 0xFF));
      if (!identifierMatches ||
          (declaredLength < (service.dataLength + 3)))
      {
        success = false;
        protocolError = QString("Unexpected single-frame diagnostic response");
        break;
      }
      value->append(response.mid(5, service.dataLength));
      break;
    }
    else if (pciType == 0x10)
    {
      const int declaredLength =
        ((static_cast<quint8>(response.at(1)) & 0x0F) << 8) |
        static_cast<quint8>(response.at(2));
      const bool identifierMatches =
        (static_cast<quint8>(response.at(3)) == 0x62) &&
        (static_cast<quint8>(response.at(4)) ==
         ((service.serviceId >> 8) & 0xFF)) &&
        (static_cast<quint8>(response.at(5)) ==
         (service.serviceId & 0xFF));
      if (firstFrameReceived || !identifierMatches ||
          (declaredLength < (service.dataLength + 3)))
      {
        success = false;
        protocolError = QString("Unexpected first-frame diagnostic response");
        break;
      }
      firstFrameReceived = true;
      value->append(response.mid(6,
                                 qMin(2,
                                      static_cast<int>(service.dataLength))));
    }
    else if (pciType == 0x20)
    {
      const quint8 sequence = static_cast<quint8>(response.at(1)) & 0x0F;
      if (!firstFrameReceived || (sequence != expectedSequence))
      {
        success = false;
        protocolError = QString("Diagnostic sequence mismatch: expected %1, got %2")
                        .arg(static_cast<int>(expectedSequence))
                        .arg(static_cast<int>(sequence));
      }
      else
      {
        value->append(response.mid(2,
                                   qMin(6,
                                        service.dataLength - value->size())));
        expectedSequence = static_cast<quint8>((expectedSequence + 1) & 0x0F);
      }
    }
    else
    {
      success = false;
      protocolError = QString("Unexpected diagnostic PCI type");
    }
  }

  if (success && (value->size() < service.dataLength))
    protocolError = QString("Diagnostic response ended before all data arrived");
  success = success && (value->size() >= service.dataLength);
  value->truncate(service.dataLength);
  return success;
}

bool LinBusWorker::writeServiceValue(quint8 nad,
                                     const LinServiceLayout &service,
                                     const QByteArray &value)
{
  protocolError.clear();
  if (!service.writable || (value.size() < service.dataLength))
  {
    protocolError = QString("Service is not writable or value length is too short");
    return false;
  }

  quint8 requestNad = 0;
  bool success = selectServiceNad(nad, service, &requestNad);

  /* DID 000A is the slave's explicit lock-bypass calibration command. */
  if (success && (service.serviceId != 0x000A))
    success = unlockSecurity(requestNad);

  if (success && (debug != 0))
    debug->setValue(DebugDiagnosticState,
                    QString("2E DID 0x%1 NAD 0x%2")
                    .arg(static_cast<int>(service.serviceId),
                         4, 16, QChar('0'))
                    .arg(static_cast<int>(requestNad), 2, 16, QChar('0'))
                    .toUpper());

  int written = 0;
  if (success && (service.dataLength <= 3))
  {
    QByteArray request(8, static_cast<char>(0xFF));
    request[0] = static_cast<char>(requestNad);
    request[1] = static_cast<char>(service.dataLength + 3);
    request[2] = static_cast<char>(0x2E);
    request[3] = static_cast<char>((service.serviceId >> 8) & 0xFF);
    request[4] = static_cast<char>(service.serviceId & 0xFF);
    for (int index = 0; index < service.dataLength; ++index)
      request[5 + index] = value.at(index);
    success = sendDiagnosticFrame(request);
    if (success)
      written = service.dataLength;
  }

  if (success && (service.dataLength > 3))
  {
    QByteArray firstFrame(8, static_cast<char>(0xFF));
    firstFrame[0] = static_cast<char>(requestNad);
    const int transportLength = service.dataLength + 3;
    firstFrame[1] = static_cast<char>(0x10 |
                                      ((transportLength >> 8) & 0x0F));
    firstFrame[2] = static_cast<char>(transportLength & 0xFF);
    firstFrame[3] = static_cast<char>(0x2E);
    firstFrame[4] = static_cast<char>((service.serviceId >> 8) & 0xFF);
    firstFrame[5] = static_cast<char>(service.serviceId & 0xFF);
    firstFrame[6] = value.at(0);
    firstFrame[7] = value.at(1);
    success = sendDiagnosticFrame(firstFrame);
    if (success)
      written = 2;

    quint8 sequence = 1;
    while (success && (written < service.dataLength))
    {
      if (shouldAbortCurrentTransaction())
      {
        protocolError = QString("Request interrupted");
        success = false;
        break;
      }

      QByteArray consecutiveFrame(8, static_cast<char>(0xFF));
      consecutiveFrame[0] = static_cast<char>(requestNad);
      consecutiveFrame[1] = static_cast<char>(0x20 | (sequence & 0x0F));
      const int chunk = qMin(6, service.dataLength - written);
      for (int index = 0; index < chunk; ++index)
        consecutiveFrame[2 + index] = value.at(written + index);
      success = sendDiagnosticFrame(consecutiveFrame);
      if (success)
        written += chunk;
      sequence = static_cast<quint8>((sequence + 1) & 0x0F);
    }
  }

  bool writeComplete = success && (written == service.dataLength);
  if (!writeComplete)
  {
    if (protocolError.isEmpty())
      protocolError = success ? QString("Diagnostic write ended early")
                              : transactionErrorText();
    return false;
  }

  if (service.expectPositiveWriteResponse)
  {
    QByteArray response;
    writeComplete = receiveDiagnosticFrame(&response);
    if (writeComplete)
    {
      const bool validResponse =
        (response.size() == 8) &&
        (static_cast<quint8>(response.at(0)) == requestNad) &&
        ((static_cast<quint8>(response.at(1)) & 0xF0) == 0x00) &&
        ((static_cast<quint8>(response.at(1)) & 0x0F) >= 3) &&
        (static_cast<quint8>(response.at(2)) == 0x6E) &&
        (static_cast<quint8>(response.at(3)) ==
         ((service.serviceId >> 8) & 0xFF)) &&
        (static_cast<quint8>(response.at(4)) ==
         (service.serviceId & 0xFF));
      if (!validResponse)
      {
        protocolError = QString("Unexpected positive write response");
        writeComplete = false;
      }
    }
  }

  /* This firmware sends no 0x6E on success and saves flash synchronously. */
  if (writeComplete &&
      !waitInterruptibly(linLayout->postWriteSettleMs))
  {
    protocolError = QString("Request interrupted during diagnostic write save");
    writeComplete = false;
  }

  /* The slave silently ignores writes while locked; read-back is definitive. */
  if (writeComplete && service.readable)
  {
    QByteArray readBack;
    if (!readServiceValue(nad, service, &readBack))
    {
      if (protocolError.isEmpty())
        protocolError = QString("Written value could not be read back");
      return false;
    }
    const QByteArray expected = value.left(service.dataLength);
    if (readBack != expected)
    {
      protocolError = QString("Write verification mismatch: expected [%1], got [%2]")
                      .arg(toHexText(expected))
                      .arg(toHexText(readBack));
      return false;
    }
  }

  return writeComplete;
}

SlaveConfigInfo LinBusWorker::createEmptyConfig(quint8 node) const
{
  SlaveConfigInfo info;
  info.slaveNode = node;
  info.SA = 0;
  info.GA = 0;
  info.platform = 0;
  info.intensity = 0;
  info.locked = false;
  info.r.x = info.r.y = info.r.Y = 0;
  info.g.x = info.g.y = info.g.Y = 0;
  info.b.x = info.b.y = info.b.Y = 0;

  const LinNodeLayout *nodeLayout = (linLayout == 0)
                                    ? 0
                                    : findLinNode(*linLayout, node);
  info.nodeType = (nodeLayout != 0) ? nodeLayout->nodeType : ENodeTypeRGB;
  return info;
}

void LinBusWorker::recordTransmit(quint8 frameId,
                                  const QByteArray &payload,
                                  bool success)
{
  if (debug == 0)
    return;

  debug->setValue(DebugCurrentFrameId,
                  QString("0x%1")
                  .arg(static_cast<int>(frameId), 2, 16, QChar('0')).toUpper());
  debug->setValue(DebugLastTx,
                  QString("%1 ID=0x%2  %3")
                  .arg(success ? QString("OK") : QString("FAIL"))
                  .arg(static_cast<int>(frameId), 2, 16, QChar('0'))
                  .arg(toHexText(payload)));
  if (frameId == linLayout->diagnosticRequestFrameId)
  {
    const QString nadText = payload.isEmpty()
                            ? QString("--")
                            : QString("0x%1")
                              .arg(static_cast<int>(static_cast<quint8>(
                                     payload.at(0))), 2, 16, QChar('0'))
                              .toUpper();
    debug->setValue(DebugDiagnosticRequest,
                    QString("%1 RAW_ID=0x%2 NAD=%3 DATA=[%4]")
                    .arg(success ? QString("OK") : QString("FAIL"))
                    .arg(static_cast<int>(frameId), 2, 16, QChar('0'))
                    .arg(nadText)
                    .arg(toHexText(payload))
                    .toUpper());
  }
  debug->increment(DebugTxCount);
  recordIoResult(success);
}

void LinBusWorker::recordReceive(quint8 frameId,
                                 const QByteArray &payload,
                                 bool success)
{
  if (debug == 0)
    return;

  debug->setValue(DebugLastRx,
                  QString("%1 ID=0x%2  %3")
                  .arg(success ? QString("OK") : QString("FAIL"))
                  .arg(static_cast<int>(frameId), 2, 16, QChar('0'))
                  .arg(toHexText(payload)));
  if (frameId == linLayout->diagnosticResponseFrameId)
  {
    const QString nadText = payload.isEmpty()
                            ? QString("--")
                            : QString("0x%1")
                              .arg(static_cast<int>(static_cast<quint8>(
                                     payload.at(0))), 2, 16, QChar('0'))
                              .toUpper();
    debug->setValue(DebugDiagnosticResponse,
                    QString("%1 RAW_ID=0x%2 NAD=%3 DATA=[%4]")
                    .arg(success ? QString("OK") : QString("FAIL"))
                    .arg(static_cast<int>(frameId), 2, 16, QChar('0'))
                    .arg(nadText)
                    .arg(toHexText(payload))
                    .toUpper());
  }
  if (success)
    debug->increment(DebugRxCount);
  recordIoResult(success);
}

void LinBusWorker::recordIoResult(bool success)
{
  if ((debug == 0) || (comm == 0))
    return;

  const QString ioText = comm->lastErrorText();
  const bool echoWarning = success &&
    (ioText.contains(QString("echo unavailable"), Qt::CaseInsensitive) ||
     ioText.contains(QString("echo differs"), Qt::CaseInsensitive));
  debug->setValue(DebugLastIoResult,
                  echoWarning ? QString("WARNING: %1").arg(ioText)
                              : (success ? QString("OK") : ioText));

  if (!success)
  {
    debug->setValue(DebugLastError, ioText);
    if (comm->lastError() == ELinIoTimeout)
      debug->increment(DebugTimeoutCount);
    if (comm->lastError() == ELinIoChecksumError)
      debug->increment(DebugChecksumErrorCount);
  }
}

QString LinBusWorker::transactionErrorText() const
{
  if (!protocolError.isEmpty())
    return protocolError;
  if (comm != 0)
    return comm->lastErrorText();
  return QString("LIN transport is unavailable");
}
