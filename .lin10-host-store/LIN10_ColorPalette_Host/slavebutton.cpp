#include "slavebutton.h"

namespace {

static const QString kEnabledStyle = "QPushButton{"
                                     "border:2px solid #0fbacd;"
                                     "border-top-right-radius:15px;"
                                     "border-bottom-left-radius:15px;"
                                     "color:rgb(255,251,240);"
                                     "background:rgba(29,165,219,0.3);"
                                     "}";

static const QString kDisabledStyle = "QPushButton{"
                                      "border:0px;"
                                      "border-top-right-radius:15px;"
                                      "border-bottom-left-radius:15px;"
                                      "color:rgba(255,251,240,0.15);"
                                      "background:rgba(29,165,219,0.15);"
                                      "}";

} // namespace

SlaveButton::SlaveButton(QWidget *parent,
                         int nodeAddress,
                         const QString &displayName)
  : QPushButton(parent),
    address(nodeAddress),
    name(displayName),
    online(false),
    configurationEnabled(true)
{
  connect(this, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  setButtonEnabled(false);
}

void SlaveButton::setButtonEnabled(bool enabled)
{
  online = enabled;
  refreshAppearance();
}

void SlaveButton::setConfigurationEnabled(bool enabled)
{
  configurationEnabled = enabled;
  refreshAppearance();
}

void SlaveButton::refreshAppearance()
{
  setEnabled(online && configurationEnabled);
  setStyleSheet(online ? kEnabledStyle : kDisabledStyle);
  setText(QString("%1  [NAD %2]\n%3")
          .arg(name)
          .arg(address)
          .arg(online
               ? (configurationEnabled ? "ON-Line" : "ON-Line (Status Only)")
               : "OFF-Line"));
}

bool SlaveButton::isOnLine() const
{
  return online;
}

void SlaveButton::buttonClicked()
{
  emit slaveClicked(address);
}
