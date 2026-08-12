#ifndef LINRUNTIME_H
#define LINRUNTIME_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "lin_types.h"

struct LinLayout;

/*
 * Stable application boundary seen by every UI feature module.
 *
 * UI code depends only on this interface.  The concrete scheduler, worker
 * thread, serial driver and diagnostic state machines stay replaceable behind
 * it.  Commands are GUI-thread calls that enqueue value snapshots; results
 * return asynchronously through signals.
 */
class LinRuntime : public QObject
{
  Q_OBJECT

public:
  explicit LinRuntime(QObject *parent = 0)
    : QObject(parent)
  {
  }

  virtual ~LinRuntime()
  {
  }

  virtual void start() = 0;
  virtual bool stop(unsigned long timeoutMs = 5000) = 0;
  virtual bool isRunning() const = 0;
  virtual bool isReady() const = 0;

  virtual const LinLayout &layout() const = 0;
  virtual bool isLayoutValid() const = 0;
  virtual QString layoutErrorText() const = 0;

  virtual void setBCMSignal(const BCMSignal &signal) = 0;
  virtual void setPublishedFrameSignal(int frameIndex,
                                       const BCMSignal &signal) = 0;
  virtual void switchBCMSignal(const BCMSignal &signal) = 0;
  virtual BCMSignal getBCMSignal() const = 0;
  virtual void applySignalPreset(int presetIndex) = 0;

  virtual quint32 readNodeConfiguration(quint8 node) = 0;
  virtual quint32 writeNodeConfiguration(const SlaveConfigInfo &info) = 0;
  virtual quint32 calibrateNode(quint8 node, quint8 mode) = 0;
  virtual quint32 lockNode(quint8 node) = 0;
  virtual quint32 unlockNode(quint8 node) = 0;
  virtual void cancel(quint32 requestId) = 0;

  virtual void setReservedDebugValue(int reservedIndex,
                                     const QString &name,
                                     const QVariant &value) = 0;

  template <typename T>
  void setReservedDebugValue(int reservedIndex,
                             const QString &name,
                             const T &value)
  {
    setReservedDebugValue(reservedIndex,
                          name,
                          QVariant::fromValue(value));
  }

  virtual void sleepBus() = 0;
  virtual void wakeBus() = 0;
  virtual void setBusEnabled(bool enabled) = 0;

signals:
  void SlaveStatusChanged(SlaveStatus status);
  /* Any valid application-frame or diagnostic response from this node. */
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

private:
  Q_DISABLE_COPY(LinRuntime)
};

#endif // LINRUNTIME_H
