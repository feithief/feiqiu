#include "mainwindow.h"
#include "debugstore.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  DebugStore debugStore;
  debugStore.setValue(DebugAppState, QString("Application starting"));

  MainWindow w(&debugStore);
  w.show();

  return a.exec();
}
