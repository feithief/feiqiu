#ifndef DEBUGSTORE_H
#define DEBUGSTORE_H

#include <QMutex>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QtGlobal>

enum DebugVariable
{
  DebugAppState = 0,
  DebugSchedulerState,
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

struct DebugSnapshotItem
{
  QString name;
  QVariant value;
  qint64 updatedAtMs;
  quint64 revision;
  bool reserved;
};

/*
 * Thread-safe debug value storage.
 *
 * Business code may only write diagnostic mirrors here.  It must never read
 * these values back to control application behaviour.
 */
class DebugStore
{
public:
  DebugStore();

  template <typename T>
  void setValue(DebugVariable variable, const T &value)
  {
    setVariant(variable, QVariant::fromValue(value));
  }

  void setVariant(DebugVariable variable, const QVariant &value);
  void increment(DebugVariable variable, quint64 amount = 1);

  /* reservedIndex is zero based: 0..9. */
  void setReserved(int reservedIndex,
                   const QString &name,
                   const QVariant &value);

  template <typename T>
  void setReserved(int reservedIndex,
                   const QString &name,
                   const T &value)
  {
    setReserved(reservedIndex, name, QVariant::fromValue(value));
  }

  QVector<DebugSnapshotItem> snapshot() const;

private:
  Q_DISABLE_COPY(DebugStore)

  struct DebugItem
  {
    QString name;
    QVariant value;
    qint64 updatedAtMs;
    quint64 revision;
    bool reserved;
  };

  static int reservedVariableIndex(int reservedIndex);

  mutable QMutex mutex;
  QVector<DebugItem> items;
};

#endif // DEBUGSTORE_H
