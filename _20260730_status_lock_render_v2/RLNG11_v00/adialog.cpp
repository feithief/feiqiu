#include "adialog.h"

ADialog::ADialog(QWidget *parent):
  QWidget(parent),
  ui(new Ui::DialogForm)
{
  ui->setupUi(this);

  currentMode = EDialogTypeDone;
  connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(hide()));
}

void ADialog::setContent(QString content)
{
  ui->labelTitle->setText(content);
}

void ADialog::setMode(enum DialogType mode)
{
  switch (mode)
  {
    case EDialogTypeWaitting:
      ui->labelTitle->setGeometry(340,340,684,81);
      ui->pushButtonOK->hide();
    break;
    case EDialogTypeDone:
      ui->labelTitle->setGeometry(340,300,684,81);
      ui->pushButtonOK->show();
    break;
    default:
    break;
  }
}

void ADialog::changeToDoneMode()
{
  setContent("Done!");
  setMode(EDialogTypeDone);
}
