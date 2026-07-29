#include "debugstore.h"

#include <QDateTime>
#include <QMutexLocker>

namespace {

static const char *const kDebugNames[] = {
  "Application.State",
  "Scheduler.State",
  "Thread.GUI",
  "Thread.LIN",
  "Queue.DiagnosticDepth",
  "Diagnostic.ActiveRequestID",
  "Device.State",
  "LIN.CurrentFrameID",
  "LIN.CurrentNAD",
  "LIN.LastTx",
  "LIN.LastRx",
  "LIN.LastIoResult",
  "LIN.TxCount",
  "LIN.RxCount",
  "LIN.TimeoutCount",
  "LIN.ChecksumErrorCount",
  "Diagnostic.State",
  "Diagnostic.LastError",
  "Diagnostic.Request",
  "Diagnostic.Response",
  "LIN.ActiveSignalPreset",
  "Error.Last",
  "Reserved.01",
  "Reserved.02",
  "Reserved.03",
  "Reserved.04",
  "Reserved.05",
  "Reserved.06",
  "Reserved.07",
  "Reserved.08",
  "Reserved.09",
  "Reserved.10"
};

static_assert((sizeof(kDebugNames) / sizeof(kDebugNames[0])) ==
              DebugVariableCount,
              "Debug variable names must match DebugVariableCount");

} // namespace

DebugStore::DebugStore()
  : items(DebugVariableCount)
{
  const qint64 now = QDateTime::currentMSecsSinceEpoch();

  for (int index = 0; index < DebugVariableCount; ++index)
  {
    items[index].name = QString::fromLatin1(kDebugNames[index]);
    items[index].value = QString("-");
    items[index].updatedAtMs = now;
    items[index].revision = 0;
    items[index].reserved = (index >= DebugReserved01);
  }

  items[DebugTxCount].value = static_cast<qulonglong>(0);
  items[DebugRxCount].value = static_cast<qulonglong>(0);
  items[DebugTimeoutCount].value = static_cast<qulonglong>(0);
  items[DebugChecksumErrorCount].value = static_cast<qulonglong>(0);
  items[DebugDiagnosticQueueDepth].value = 0;
  items[DebugActiveRequestId].value = static_cast<qulonglong>(0);
}

void DebugStore::setVariant(DebugVariable variable, const QVariant &value)
{
  const int index = static_cast<int>(variable);
  if ((index < 0) || (index >= DebugVariableCount))
    return;

  QMutexLocker locker(&mutex);
  items[index].value = value;
  items[index].updatedAtMs = QDateTime::currentMSecsSinceEpoch();
  ++items[index].revision;
}

void DebugStore::increment(DebugVariable variable, quint64 amount)
{
  const int index = static_cast<int>(variable);
  if ((index < 0) || (index >= DebugVariableCount))
    return;

  QMutexLocker locker(&mutex);
  const quint64 oldValue = items[index].value.toULongLong();
  items[index].value = static_cast<qulonglong>(oldValue + amount);
  items[index].updatedAtMs = QDateTime::currentMSecsSinceEpoch();
  ++items[index].revision;
}

int DebugStore::reservedVariableIndex(int reservedIndex)
{
  if ((reservedIndex < 0) || (reservedIndex >= 10))
    return -1;

  return DebugReserved01 + reservedIndex;
}

void DebugStore::setReserved(int reservedIndex,
                             const QString &name,
                             const QVariant &value)
{
  const int index = reservedVariableIndex(reservedIndex);
  if (index < 0)
    return;

  QMutexLocker locker(&mutex);
  if (!name.isEmpty())
    items[index].name = name;
  items[index].value = value;
  items[index].updatedAtMs = QDateTime::currentMSecsSinceEpoch();
  ++items[index].revision;
}

QVector<DebugSnapshotItem> DebugStore::snapshot() const
{
  QVector<DebugSnapshotItem> result;
  QMutexLocker locker(&mutex);
  result.reserve(items.size());

  for (int index = 0; index < items.size(); ++index)
  {
    DebugSnapshotItem item;
    item.name = items[index].name;
    item.value = items[index].value;
    item.updatedAtMs = items[index].updatedAtMs;
    item.revision = items[index].revision;
    item.reserved = items[index].reserved;
    result.append(item);
  }

  return result;
}
