#ifndef PRODUCTIONVERIFY_H
#define PRODUCTIONVERIFY_H

#include <QWidget>
#include <QFrame>
#include <QMap>
#include <QSignalMapper>
#include <QTimer>
#include "keyboard.h"
#include "ambientlinscheduler.h"

namespace Ui {
  class ProductionVerify;
}

class QPushButton;
class QHideEvent;

class ProductionVerify : public QWidget
{
  Q_OBJECT

public:
  explicit ProductionVerify(AmbientLinScheduler *scheduler,
                            QWidget *parent = 0);
  ~ProductionVerify();
  void init(void);

protected:
  void hideEvent(QHideEvent *event);

private slots:
  void changeAddress(int targetMask);
  void changeColor(void);
  void changeColorAuto(void);
  void changeColorManual(void);
  void intensityChanged(int intensity);
  void closePage();


private:
  Ui::ProductionVerify *ui;

  QFrame *backgroundframe;
  AmbientLinScheduler *linScheduler;
  BCMSignal masterSignals;
  QSignalMapper *nodeMapper;
  QMap<int, QPushButton *> nodeButtons;
  int colorIndex;
  int selectedTargetMaskRecord;
  QTimer *colorChangeTimer;
  KeyBoard *keys;
  void showInputKeyBoard(bool show);
  bool eventFilter(QObject *watched, QEvent *event);
};

#endif // PRODUCTIONVERIFY_H
