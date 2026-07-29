#ifndef SLAVEBUTTON_H
#define SLAVEBUTTON_H

#include <QPushButton>
#include <QString>

class SlaveButton : public QPushButton
{
  Q_OBJECT

public:
  SlaveButton(QWidget *parent,
              int nodeAddress,
              const QString &displayName);
  void setConfigurationEnabled(bool enabled);
  void setFeedbackAvailable(bool available);
  bool hasValidFeedback() const;

signals:
  void slaveClicked(int nodeAddress);

private slots:
  void buttonClicked();

private:
  int address;
  QString name;
  bool feedbackAvailable;
  bool configurationEnabled;
  void refreshAppearance();
};

#endif // SLAVEBUTTON_H
