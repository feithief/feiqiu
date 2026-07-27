#ifndef SLAVEBUTTON_H
#define SLAVEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

class SlaveButton : public QPushButton
{
  Q_OBJECT

public:
  SlaveButton(QWidget *parent = 0, int index = 0);
  void setButtonEnabled(int index, bool enabled);
  bool isOnLine();

signals:
  void slaveClicked(int index);

private slots:
  void buttonClicked();

private:
  bool onLineState;
  int buttonIndex;
};

#endif // SLAVEBUTTON_H
