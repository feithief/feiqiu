#ifndef DEBUGSTORE_H
#define DEBUGSTORE_H

#include <QMutex>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QtGlobal>

#include "debugsink.h"
#include "debugsnapshot.h"

/*
 * Thread-safe debug value storage.
 *
 * Business code may only write diagnostic mirrors here.  It must never read
 * these values back to control application behaviour.
 */
class DebugStore : public DebugSink, public DebugSnapshotSource
{
public:
  DebugStore();

  void setVariant(DebugVariable variable,
                  const QVariant &value) override;
  void increment(DebugVariable variable,
                 quint64 amount = 1) override;

  /* reservedIndex is zero based: 0..9. */
  void setReserved(int reservedIndex,
                   const QString &name,
                   const QVariant &value) override;

  QVector<DebugSnapshotItem> snapshot() const override;

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
