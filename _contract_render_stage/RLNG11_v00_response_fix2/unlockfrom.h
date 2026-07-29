#ifndef UNLOCKFROM_H
#define UNLOCKFROM_H

#include <QWidget>
#include <QFrame>
#include <QTimer>
#include "adialog.h"
#include "keyboard.h"
#include <QString>

namespace Ui {
  class unlockFrom;
}

enum lockType
{
  ELockTypeMaster = 0x1,
  ELockTypeSlave = 0x2,
  ElockTypeFactory = 0x4,
};

class unlockFrom : public QWidget
{
  Q_OBJECT

public:
  explicit unlockFrom(QWidget *parent = 0);
  ~unlockFrom();

  void init();
  bool getMasterLockStatus();
  bool getSlaveLockStatus();
  bool getFactoryLockStatus();

private:
  Ui::unlockFrom *ui;
  QFrame *backgroundframe;
  bool isMasterLocked, isSlaveLocked, isFactoryLocked;
  bool isMasterSelected, isSlaveSelected, isFactorySelected;
  QTimer *countDownTimer;
  unsigned long int generatedCode;
  ADialog *dialog;
  KeyBoard *keys;

  QString decode(int value, unsigned short int type);
  void showInputKeyBoard(bool show);
  bool eventFilter(QObject *watched, QEvent *event);

private slots:
  void checkReusult();
  void unlockTimeOut();

  void selectMasterClicked();
  void selectSlaveClicked();
  void selectFactoryClicked();
};

#endif // UNLOCKFROM_H
