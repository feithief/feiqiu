#ifndef AMBIENTLINCOMM_H
#define AMBIENTLINCOMM_H

#include <QObject>
#include <QWidget>
#include <stdint-uintn.h>

class AmbientLinComm : QObject
{
  Q_OBJECT

public:
  //no multiple thread used!
  static AmbientLinComm* getInstance(QObject *parent = 0, uint32_t LinBaudrate = 19200)
  {
    if (mInstance == NULL)
    {
      mInstance = new AmbientLinComm(parent, LinBaudrate);
    }

    return mInstance;
  };

  bool isDeviceReady(void);
  //basicly the length will always be 8.
  void transferDataToLin(uint8_t PID, uint8_t* buffer, uint32_t length);
  void transferHeaderToLin(uint8_t header);
  //basicly the length will always be 8.
  uint32_t  retrieveDataFromLin(uint8_t* buffer, uint32_t desiredLength);

private:
  bool LinDeviceStatus;
  int fileDescriptor;
  unsigned long int baud;
  static AmbientLinComm* mInstance;

  AmbientLinComm(QObject *parent = 0, uint32_t LinBaudrate = 19200);

  void openLinDevice(const char* devName);
  unsigned char calculateParityPID(uint8_t PID);
  unsigned char calculateCheckSum(uint8_t parityPID, uint8_t* buffer, uint32_t length);
};

#endif // AMBIENTLINCOMM_H
