#ifndef LINBUSWORKER_H
#define LINBUSWORKER_H

#include <QByteArray>
#include <QObject>
#include <QQueue>

#include "lin_types.h"

class DebugSink;
class LinTransport;
class LinTransportFactory;
class QTimer;
struct LinLayout;
struct LinServiceLayout;

class LinBusWorker : public QObject
{
  Q_OBJECT

public:
  LinBusWorker(const LinLayout *layout,
               const BCMSignal &initialSignal,
               const LinTransportFactory *transportFactory,
               DebugSink *debugSink);

signals:
  void slaveStatusChanged(SlaveStatus status);
  void nodeResponseObserved(quint8 node);
  void nodeConfigurationRead(quint32 requestId,
                             SlaveConfigInfo info,
                             bool success,
                             QString errorMessage);
  void nodeConfigurationWritten(quint32 requestId,
                                quint8 node,
                                bool success,
                                QString errorMessage);
  void calibrationFinished(quint32 requestId,
                           quint8 node,
                           bool success,
                           QString errorMessage);
  void nodeLockStateChanged(quint32 requestId,
                            quint8 node,
                            bool locked,
                            bool success,
                            QString errorMessage);
  void busStateChanged(bool ready, QString message);
  void stopped();

public slots:
  void initialize();
  void stopWorker();
  void updateControlSignal(BCMSignal signal);
  void switchControlSignal(BCMSignal signal);
  void applySignalPreset(int presetIndex);
  void enqueueReadNode(quint32 requestId, quint8 node);
  void enqueueWriteNode(quint32 requestId, SlaveConfigInfo info);
  void enqueueCalibration(quint32 requestId, quint8 node, quint8 mode);
  void enqueueLockNode(quint32 requestId, quint8 node);
  void enqueueUnlockNode(quint32 requestId, quint8 node);
  void cancelRequest(quint32 requestId);
  void sleepBus();
  void wakeBus();
  void setBusEnabled(bool enabled);

private slots:
  void processScheduleSlot();
  void processTaskStep();

private:
  enum TaskKind
  {
    ETaskNone = 0,
    ETaskReadNode,
    ETaskWriteNode,
    ETaskCalibration,
    ETaskLockNode,
    ETaskUnlockNode
  };

  enum PendingBusAction
  {
    EPendingBusActionNone = 0,
    EPendingBusActionSleep,
    EPendingBusActionEnable,
    EPendingBusActionDisable
  };

  enum WriteTaskPhase
  {
    EWriteTaskPhaseWrite = 0,
    EWriteTaskPhaseWaitForFlash,
    EWriteTaskPhaseReadBack
  };

  enum LockTaskPhase
  {
    ELockTaskPhaseCommand = 0,
    ELockTaskPhaseWaitForFlash,
    ELockTaskPhaseReadBack
  };

  struct PendingTask
  {
    TaskKind kind;
    quint32 requestId;
    quint8 node;
    quint8 calibrationMode;
    SlaveConfigInfo config;
  };

  const LinLayout *linLayout;
  const LinTransportFactory *transportFactory;
  DebugSink *debug;
  LinTransport *comm;
  QTimer *scheduleTimer;
  BCMSignal controlSignal;
  QByteArray primaryControlPayload;
  bool initialized;
  bool busEnabled;
  bool stopping;
  int scheduleIndex;

  QQueue<PendingTask> taskQueue;
  PendingTask activeTask;
  bool taskActive;
  bool activeTaskCancelled;
  bool temporaryNadActive;
  bool activeSecurityUnlocked;
  quint8 activeInitialNad;
  int activeStepIndex;
  WriteTaskPhase activeWritePhase;
  LockTaskPhase activeLockPhase;
  bool activeNadChanged;
  SlaveConfigInfo activeReadResult;
  QString protocolError;
  PendingBusAction pendingBusAction;
  bool controlSwitchPending;
  BCMSignal pendingControlSignal;
  /* Coalesced, high-priority control update; bus I/O remains single-threaded. */
  bool priorityControlPending;
  /* Guarantee one normal schedule slot between two priority transmissions. */
  bool normalSlotRequiredAfterPriority;
  /* Allow one control refresh between complete diagnostic service steps. */
  bool taskControlYieldPending;
  bool taskControlYieldServed;
  /* True until the first response header after the latest diagnostic write. */
  bool diagnosticResponsePending;

  void assertWorkerThread() const;
  void updateTaskQueueDebug() const;
  void startNextTask();
  void finishActiveTask(bool success, const QString &errorMessage);
  void emitTaskResult(const PendingTask &task,
                      const SlaveConfigInfo &readResult,
                      bool success,
                      const QString &errorMessage);
  void rejectQueuedTasks(const QString &errorMessage);
  void finishStopping();
  void resumeSchedule();
  int primaryControlScheduleSlot() const;
  int primaryControlDelayMs() const;
  bool transmitPublishedFrame(int frameIndex);
  bool transmitPrimaryControlFrame();
  bool refreshPrimaryControlPayload();
  void requestPriorityControlTransmission();
  bool shouldAbortCurrentTransaction() const;
  bool waitInterruptibly(int delayMs,
                         bool allowDuringShutdown = false) const;
  void performSleepBus();
  void performControlSwitch(const BCMSignal &signal);
  void applyPendingBusAction();

  bool sendDiagnosticFrame(const QByteArray &payload,
                           bool allowDuringShutdown = false);
  bool receiveDiagnosticFrame(QByteArray *payload,
                              bool allowDuringShutdown = false);
  bool configureNad(quint8 initialNad,
                    quint8 requestedNad,
                    bool allowDuringShutdown = false);
  bool selectServiceNad(quint8 initialNad,
                        const LinServiceLayout &service,
                        quint8 *requestNad);
  bool restoreInitialNad(bool allowDuringShutdown = false);
  bool unlockSecurity(quint8 requestNad);
  bool readServiceValue(quint8 nad,
                        const LinServiceLayout &service,
                        QByteArray *value);
  bool writeServiceValue(quint8 nad,
                         const LinServiceLayout &service,
                         const QByteArray &value);

  SlaveConfigInfo createEmptyConfig(quint8 node) const;

  void recordTransmit(quint8 frameId,
                      const QByteArray &payload,
                      bool success);
  void recordReceive(quint8 frameId,
                     const QByteArray &payload,
                     bool success);
  void recordIoResult(bool success);
  QString transactionErrorText() const;
};

#endif // LINBUSWORKER_H
