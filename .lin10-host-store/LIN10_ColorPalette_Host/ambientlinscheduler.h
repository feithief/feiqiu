#ifndef AMBIENTLINSCHEDULER_H
#define AMBIENTLINSCHEDULER_H

#include <QMutex>
#include <QObject>
#include <QPointer>
#include <QVariant>

#include "lin_types.h"

class DebugStore;
class LinBusWorker;
class QThread;
class QTimer;
struct LinLayout;

/*
 * GUI-thread facade for the LIN subsystem.
 *
 * Every public command is asynchronous except stop(), which is only used
 * during application shutdown.  The worker owns all serial I/O in one thread.
 * The LinLayout and all tables referenced by it must outlive this object;
 * tools/ldf_profile_gen.py emits the intended static generated profile.
 */
class AmbientLinScheduler : public QObject
{
  Q_OBJECT

public:
  AmbientLinScheduler(const LinLayout &layout,
                      DebugStore *debugStore,
                      QObject *parent = 0);
  ~AmbientLinScheduler();

  void start();
  bool stop(unsigned long timeoutMs = 5000);
  bool isRunning() const;

  const LinLayout &layout() const;
  bool isLayoutValid() const;
  QString layoutErrorText() const;

  void setBCMSignal(const BCMSignal &signal);
  void switchBCMSignal(const BCMSignal &signal);
  BCMSignal getBCMSignal() const;
  void applySignalPreset(int presetIndex);

  quint32 readNodeConfiguration(quint8 node);
  quint32 writeNodeConfiguration(const SlaveConfigInfo &info);
  quint32 calibrateNode(quint8 node, quint8 mode);
  void cancel(quint32 requestId);

  void setReservedDebugValue(int reservedIndex,
                             const QString &name,
                             const QVariant &value);

  template <typename T>
  void setReservedDebugValue(int reservedIndex,
                             const QString &name,
                             const T &value)
  {
    setReservedDebugValue(reservedIndex,
                          name,
                          QVariant::fromValue(value));
  }

  void sleepBus();
  void wakeBus();
  void setBusEnabled(bool enabled);

signals:
  void SlaveStatusChanged(SlaveStatus status);
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
  void busStateChanged(bool ready, QString message);

  /* Internal queued commands. */
  void stopRequested();
  void controlSignalRequested(BCMSignal signal);
  void switchControlSignalRequested(BCMSignal signal);
  void signalPresetRequested(int presetIndex);
  void readNodeRequested(quint32 requestId, quint8 node);
  void writeNodeRequested(quint32 requestId, SlaveConfigInfo info);
  void calibrationRequested(quint32 requestId, quint8 node, quint8 mode);
  void cancelRequested(quint32 requestId);
  void sleepRequested();
  void wakeRequested();
  void busEnabledRequested(bool enabled);

private slots:
  void flushControlSignal();

private:
  const LinLayout *linLayout;
  DebugStore *debug;
  QThread *workerThread;
  QPointer<LinBusWorker> worker;
  QTimer *controlCoalesceTimer;
  bool startedOnce;
  bool validLayout;
  QString validationError;

  mutable QMutex controlMutex;
  BCMSignal desiredControlSignal;
  quint32 requestSequence;

  void createWorker();
  bool canSubmitDiagnosticRequest() const;
  quint32 nextRequestId();
};

#endif // AMBIENTLINSCHEDULER_H
