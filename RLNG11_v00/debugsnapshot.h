#ifndef DEBUGSNAPSHOT_H
#define DEBUGSNAPSHOT_H

#include <QString>
#include <QVariant>
#include <QVector>
#include <QtGlobal>

struct DebugSnapshotItem
{
  QString name;
  QVariant value;
  qint64 updatedAtMs;
  quint64 revision;
  bool reserved;
};

/* Read-only F12 data source.  The panel cannot mutate runtime state. */
class DebugSnapshotSource
{
public:
  virtual ~DebugSnapshotSource()
  {
  }

  virtual QVector<DebugSnapshotItem> snapshot() const = 0;

protected:
  DebugSnapshotSource()
  {
  }

private:
  Q_DISABLE_COPY(DebugSnapshotSource)
};

#endif // DEBUGSNAPSHOT_H
