#ifndef PRODUCTIONVERIFY_H
#define PRODUCTIONVERIFY_H

#include <QWidget>
#include <QFrame>
#include <QSignalMapper>
#include <QTimer>
#include "keyboard.h"
#include "ambientlinscheduler.h"

namespace Ui {
  class ProductionVerify;
}

class ProductionVerify : public QWidget
{
  Q_OBJECT

public:
  explicit ProductionVerify(QWidget *parent = 0);
  ~ProductionVerify();
  void init(void);

private slots:
  void changeAddress(int index);
  void changeColor(void);
  void changeColorAuto(void);
  void changeColorManual(void);
  void intensityChanged(int intensity);


private:
  Ui::ProductionVerify *ui;

  QFrame *backgroundframe;
  AmbientLinScheduler *linScheduler;
  struct BCMSignal masterSignals;
  QSignalMapper *colorMapper, *nodeMapper;
  int colorIndex, colorIndexRecord;
  QTimer *colorChangeTimer;
  KeyBoard *keys;
  void showInputKeyBoard(bool show);
  bool eventFilter(QObject *watched, QEvent *event);
};

#endif // PRODUCTIONVERIFY_H
