#ifndef ADIALOG_H
#define ADIALOG_H

#include <QObject>
#include <QWidget>
#include "ui_deviceDialog.h"

enum DialogType
{
  EDialogTypeWaitting = 0x0,
  EDialogTypeDone = 0x1,
};

class ADialog : public QWidget
{
  Q_OBJECT
public:
  explicit ADialog(QWidget *parent = NULL);//nullptr
  void setContent(QString content);
  void setMode(enum DialogType mode);

public slots:
  void changeToDoneMode();

private:
  Ui::DialogForm *ui;
  enum DialogType currentMode;
};

#endif // ADIALOG_H
