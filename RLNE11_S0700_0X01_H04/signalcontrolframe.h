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
 * Generic frame-by-frame editor for every master-published LIN signal. The
 * page is built from LinLayout at runtime; no frame or signal name is
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
  void applyCurrentFrame();
  bool applyPresetToCurrentFrame(int presetIndex);

signals:
  void valuesApplied();

private:
  Ui::SignalControlFrame *ui;
  LinRuntime *linRuntime;
  const LinFrameLayout *frameLayout;
  int selectedFrameIndex;
  QList<const LinSignalLayout *> controlledSignals;
  QList<QWidget *> valueEditors;

  void populateFrameSelector();
  void selectFrame(int comboIndex);
  void buildSignalRows();
  QString shortSignalName(const char *sourceName) const;
  void loadCurrentValues();
  void applyValues();
  void closePage();
};

#endif // SIGNALCONTROLFRAME_H
