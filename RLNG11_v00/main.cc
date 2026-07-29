#include "ambientlincomm.h"
#include "ambientlinscheduler.h"
#include "debugstore.h"
#include "linlayout.h"
#include "mainwindow.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  DebugStore debugStore;
  debugStore.setValue(DebugAppState, QString("Application starting"));
  debugStore.setValue(
    DebugGuiThread,
    QString("0x%1").arg(
      static_cast<qulonglong>(
        reinterpret_cast<quintptr>(QThread::currentThreadId())),
      0,
      16));

  /* Composition root: only this file creates concrete infrastructure. */
  AmbientLinCommFactory transportFactory;
  AmbientLinScheduler linRuntime(defaultLinLayout(),
                                 &transportFactory,
                                 &debugStore);
  MainWindow w(&linRuntime,
               &debugStore,
               &debugStore);
  w.show();

  return a.exec();
}
