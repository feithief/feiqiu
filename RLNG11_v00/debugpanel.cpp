#include "debugpanel.h"

#include "debugsnapshot.h"

#include <QBrush>
#include <QColor>
#include <QDateTime>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>

DebugPanel::DebugPanel(DebugSnapshotSource *source, QWidget *parent)
  : QWidget(parent),
    debugSource(source)
{
  Q_ASSERT(debugSource != 0);

  setGeometry(0, 0, 1366, 768);
  setStyleSheet("DebugPanel { background-color: rgba(0, 0, 0, 235); }"
                "QLabel { color: #0fbacd; font-size: 28px; }"
                "QTableWidget { color: white; background-color: #151515; "
                "gridline-color: #0f6570; font-size: 17px; }"
                "QHeaderView::section { color: white; background-color: #16434a; "
                "font-size: 18px; padding: 5px; }"
                "QPushButton { color: white; border: 2px solid #0fbacd; "
                "border-radius: 8px; font-size: 22px; background: #16434a; }");

  titleLabel = new QLabel(tr("Runtime Debug Variables"), this);
  titleLabel->setGeometry(55, 20, 600, 50);

  closeButton = new QPushButton(tr("Close (F12)"), this);
  closeButton->setGeometry(1120, 20, 190, 50);

  table = new QTableWidget(DebugVariableCount, 4, this);
  table->setGeometry(55, 85, 1255, 635);
  table->setHorizontalHeaderLabels(QStringList()
                                   << tr("Variable")
                                   << tr("Value")
                                   << tr("Updated")
                                   << tr("Revision"));
  table->verticalHeader()->hide();
  table->horizontalHeader()->setStretchLastSection(false);
  table->setColumnWidth(0, 330);
  table->setColumnWidth(1, 545);
  table->setColumnWidth(2, 230);
  table->setColumnWidth(3, 120);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);

  for (int row = 0; row < DebugVariableCount; ++row)
  {
    for (int column = 0; column < 4; ++column)
      table->setItem(row, column, new QTableWidgetItem());
  }

  refreshTimer = new QTimer(this);
  refreshTimer->setInterval(250);

  connect(closeButton, SIGNAL(clicked()), this, SLOT(closePanel()));
  connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshValues()));

  hide();
}

void DebugPanel::openPanel()
{
  refreshValues();
  refreshTimer->start();
  show();
  raise();
  setFocus();
}

void DebugPanel::closePanel()
{
  refreshTimer->stop();
  hide();
}

void DebugPanel::togglePanel()
{
  if (isVisible())
    closePanel();
  else
    openPanel();
}

void DebugPanel::refreshValues()
{
  if (debugStore == 0)
    return;

  const QVector<DebugSnapshotItem> values = debugSource->snapshot();
  for (int row = 0; row < values.size(); ++row)
  {
    const DebugSnapshotItem &value = values[row];
    table->item(row, 0)->setText(value.name);
    table->item(row, 1)->setText(value.value.toString());
    table->item(row, 2)->setText(
      QDateTime::fromMSecsSinceEpoch(value.updatedAtMs).toString("HH:mm:ss.zzz"));
    table->item(row, 3)->setText(QString::number(value.revision));

    const QColor color = value.reserved ? QColor(255, 220, 120)
                                        : QColor(255, 255, 255);
    for (int column = 0; column < 4; ++column)
      table->item(row, column)->setForeground(QBrush(color));
  }
}
