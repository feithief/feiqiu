#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ambientlinscheduler.h"
#include "bcmmasterbutton.h"
#include "slavebutton.h"
#include "bcmmasterframe.h"
#include "unlockfrom.h"
#include "productionverify.h"
#include <slaveframeconfig.h>
#include <QDebug>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BCMMasterButton *masterButton, *exitButton, *settingButton;
    SlaveButton *slaveButton[DSLAVE_NODE_AMOUNT];
    AmbientLinScheduler *linScheduler;

    SlaveFrameConfig *slaveFrame;
    ProductionVerify *productVerifyFrame;
    BCMMasterFrame* masterFrame;
    unlockFrom* lockFrame;
    void closeEvent(QCloseEvent *evt);

private slots:
    void closeApp();
    void unlockFeature();
    void slaveConfig(int index);
    void enterMasterConfigMode();
    void slaveStatusHandle(struct SlaveStatus status);
};

#endif // MAINWINDOW_H
