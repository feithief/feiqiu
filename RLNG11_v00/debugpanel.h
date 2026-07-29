#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

#include <QWidget>

class DebugSnapshotSource;
class QLabel;
class QPushButton;
class QTableWidget;
class QTimer;

class DebugPanel : public QWidget
{
  Q_OBJECT

public:
  explicit DebugPanel(DebugSnapshotSource *source, QWidget *parent = 0);

public slots:
  void openPanel();
  void closePanel();
  void togglePanel();

private slots:
  void refreshValues();

private:
  DebugSnapshotSource *debugSource;
  QLabel *titleLabel;
  QTableWidget *table;
  QPushButton *closeButton;
  QTimer *refreshTimer;
};

#endif // DEBUGPANEL_H
