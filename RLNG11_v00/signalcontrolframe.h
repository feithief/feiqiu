#ifndef SIGNALCONTROLFRAME_H
#define SIGNALCONTROLFRAME_H

#include <QList>
#include <QWidget>

class LinRuntime;
struct LinFrameLayout;
struct LinSignalLayout;

namespace Ui {
class SignalControlFrame;
}

/*
 * Generic editor for every signal declared in the active primary control
 * frame. The page is built from LinLayout at runtime; no LDF signal name is
 * hard-coded in this class.
 */
class SignalControlFrame : public QWidget
{
  Q_OBJECT

public:
  explicit SignalControlFrame(LinRuntime *runtime,
                              QWidget *parent = 0);
  ~SignalControlFrame();

  void init();

signals:
  void valuesApplied();

private:
  Ui::SignalControlFrame *ui;
  LinRuntime *linRuntime;
  const LinFrameLayout *frameLayout;
  QList<const LinSignalLayout *> controlledSignals;
  QList<QWidget *> valueEditors;

  void buildSignalRows();
  void loadCurrentValues();
  void applyValues();
  void closePage();
};

#endif // SIGNALCONTROLFRAME_H
