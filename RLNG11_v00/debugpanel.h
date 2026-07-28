#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

#include <QWidget>

class DebugStore;
class QLabel;
class QPushButton;
class QTableWidget;
class QTimer;

class DebugPanel : public QWidget
{
  Q_OBJECT

public:
  explicit DebugPanel(DebugStore *store, QWidget *parent = 0);

public slots:
  void openPanel();
  void closePanel();
  void togglePanel();

private slots:
  void refreshValues();

private:
  DebugStore *debugStore;
  QLabel *titleLabel;
  QTableWidget *table;
  QPushButton *closeButton;
  QTimer *refreshTimer;
};

#endif // DEBUGPANEL_H
