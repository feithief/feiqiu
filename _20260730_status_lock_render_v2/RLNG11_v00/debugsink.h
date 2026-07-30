#ifndef DEBUGSINK_H
#define DEBUGSINK_H

#include <QString>
#include <QVariant>
#include <QtGlobal>

enum DebugVariable
{
  DebugAppState = 0,
  DebugSchedulerState,
  DebugGuiThread,
  DebugLinThread,
  DebugDiagnosticQueueDepth,
  DebugActiveRequestId,
  DebugDeviceState,
  DebugCurrentFrameId,
  DebugCurrentNad,
  DebugLastTx,
  DebugLastRx,
  DebugLastIoResult,
  DebugTxCount,
  DebugRxCount,
  DebugTimeoutCount,
  DebugChecksumErrorCount,
  DebugDiagnosticState,
  DebugDiagnosticError,
  DebugDiagnosticRequest,
  DebugDiagnosticResponse,
  DebugActiveSignalPreset,
  DebugLastError,

  /* Ten independent slots reserved for future features. */
  DebugReserved01,
  DebugReserved02,
  DebugReserved03,
  DebugReserved04,
  DebugReserved05,
  DebugReserved06,
  DebugReserved07,
  DebugReserved08,
  DebugReserved09,
  DebugReserved10,

  DebugVariableCount
};

/*
 * Write-only observability port used by business and infrastructure modules.
 * No runtime decision may read values back from this interface.
 */
class DebugSink
{
public:
  virtual ~DebugSink()
  {
  }

  template <typename T>
  void setValue(DebugVariable variable, const T &value)
  {
    setVariant(variable, QVariant::fromValue(value));
  }

  virtual void setVariant(DebugVariable variable,
                          const QVariant &value) = 0;
  virtual void increment(DebugVariable variable,
                         quint64 amount = 1) = 0;
  virtual void setReserved(int reservedIndex,
                           const QString &name,
                           const QVariant &value) = 0;

protected:
  DebugSink()
  {
  }

private:
  Q_DISABLE_COPY(DebugSink)
};

#endif // DEBUGSINK_H
