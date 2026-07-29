#ifndef AMBIENTLINSCHEDULER_H
#define AMBIENTLINSCHEDULER_H

#include <QPointer>

#include "linruntime.h"

class DebugSink;
class LinBusWorker;
class LinTransportFactory;
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
class AmbientLinScheduler : public LinRuntime
{
  Q_OBJECT

public:
  AmbientLinScheduler(const LinLayout &layout,
                      const LinTransportFactory *transportFactory,
                      DebugSink *debugSink,
                      QObject *parent = 0);
  ~AmbientLinScheduler() override;

  void start() override;
  bool stop(unsigned long timeoutMs = 5000) override;
  bool isRunning() const override;
  bool isReady() const override;

  const LinLayout &layout() const override;
  bool isLayoutValid() const override;
  QString layoutErrorText() const override;

  void setBCMSignal(const BCMSignal &signal) override;
  void switchBCMSignal(const BCMSignal &signal) override;
  BCMSignal getBCMSignal() const override;
  void applySignalPreset(int presetIndex) override;

  quint32 readNodeConfiguration(quint8 node) override;
  quint32 writeNodeConfiguration(const SlaveConfigInfo &info) override;
  quint32 calibrateNode(quint8 node, quint8 mode) override;
  void cancel(quint32 requestId) override;

  void setReservedDebugValue(int reservedIndex,
                             const QString &name,
                             const QVariant &value) override;

  void sleepBus() override;
  void wakeBus() override;
  void setBusEnabled(bool enabled) override;

signals:
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
  void handleWorkerBusStateChanged(bool ready, QString message);

private:
  const LinLayout *linLayout;
  const LinTransportFactory *transportFactory;
  DebugSink *debug;
  QThread *workerThread;
  QPointer<LinBusWorker> worker;
  QTimer *controlCoalesceTimer;
  bool startedOnce;
  bool workerReady;
  bool validLayout;
  QString validationError;

  BCMSignal desiredControlSignal;
  quint32 requestSequence;

  void assertFacadeThread() const;
  void createWorker();
  bool canSubmitDiagnosticRequest() const;
  quint32 nextRequestId();
};

#endif // AMBIENTLINSCHEDULER_H
