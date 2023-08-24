#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QSignalMapper>
#include <QList>
#include "ui_amkeyboard.h"

class KeyBoard : public QWidget
{
  Q_OBJECT
public:
  explicit KeyBoard(QWidget *parent = NULL);
  void setCurrentObject(QWidget* object);

private slots:
  void hideKeyboard();

private:
  Ui::AMKeyboard *ui;
  QFrame *backgroundFrame;
  QWidget *currentWidget;

  bool eventFilter(QObject *watched, QEvent *event);
};

#endif // KEYBOARD_H
