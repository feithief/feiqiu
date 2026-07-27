#include "ambientlincomm.h"
#include <QThread>
#include <QDebug>
#include "application_config.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define DMODULE_NAME "LIN_COMM"

static const char device[] = DLIN_COMMUNICATION_DEVICE;

AmbientLinComm::AmbientLinComm(QObject *parent,uint32_t LinBaudrate):
  QObject(parent)
{
  fileDescriptor = -1;
  LinDeviceStatus = false;
  baud = LinBaudrate;
  openLinDevice(device);
}

bool AmbientLinComm::isDeviceReady(void)
{
  return LinDeviceStatus;
}

void AmbientLinComm::openLinDevice(const char* devName)
{
  struct termios newtio,oldtio;
  int errID;

  fileDescriptor = ::open(devName ,O_RDWR|O_NOCTTY);

  if (fileDescriptor < 0)
  {
      qDebug()<<DMODULE_NAME<<":"<<"open device failed!\r\n";
      return;
  }

  errID = ::fcntl(fileDescriptor, F_SETFL, 0);
  if(errID < 0)
    qDebug()<<DMODULE_NAME<<":"<<"fcntl failed!\n";
  else
    qDebug()<<DMODULE_NAME<<":"<<"fcntl="<<errID<<"\n";

  errID = ::isatty(STDIN_FILENO);
  if(errID == 0)
    qDebug()<<DMODULE_NAME<<":"<<"standard input is not a terminal device\n";
  else
    qDebug()<<DMODULE_NAME<<":"<<"isatty success!\n";

  qDebug()<<DMODULE_NAME<<":"<<"open device succeed!\n";

  if (::tcgetattr(fileDescriptor, &oldtio) != 0)
  {
      qDebug()<<DMODULE_NAME<<":"<<"get old termios failed!\n";
      goto failed;
  }

  ::bzero( &newtio, sizeof( newtio ) );

  newtio.c_cflag  |=  CLOCAL | CREAD;
  newtio.c_cflag &= ~CSIZE;
  newtio.c_cflag |= CS8;//8 bit
  newtio.c_cflag &= ~PARENB;//no parity

  switch( baud )
  {
    case 300:
      ::cfsetispeed(&newtio, B300);
      ::cfsetospeed(&newtio, B300);
      break;
    case 600:
      ::cfsetispeed(&newtio, B600);
      ::cfsetospeed(&newtio, B600);
      break;
    case 1200:
      ::cfsetispeed(&newtio, B1200);
      ::cfsetospeed(&newtio, B1200);
      break;
    case 2400:
      ::cfsetispeed(&newtio, B2400);
      ::cfsetospeed(&newtio, B2400);
      break;
    case 4800:
      ::cfsetispeed(&newtio, B4800);
      ::cfsetospeed(&newtio, B4800);
      break;
    case 9600:
      ::cfsetispeed(&newtio, B9600);
      ::cfsetospeed(&newtio, B9600);
      break;
    case 19200:
      ::cfsetispeed(&newtio, B19200);
      ::cfsetospeed(&newtio, B19200);
      break;
    case 38400:
      ::cfsetispeed(&newtio, B38400);
      ::cfsetospeed(&newtio, B38400);
      break;
    case 57600:
      ::cfsetispeed(&newtio, B57600);
      ::cfsetospeed(&newtio, B57600);
      break;
    case 115200:
      ::cfsetispeed(&newtio, B115200);
      ::cfsetospeed(&newtio, B115200);
      break;
    default:
      qDebug()<<DMODULE_NAME<<":"<<"sorry! ["<<baud<<" bps] is not support ,Set default 19200bps now!\n";
      ::cfsetispeed(&newtio, B19200);
      ::cfsetospeed(&newtio, B19200);
      break;
  }

  newtio.c_cflag &=  ~CSTOPB;//1 stop bit
  newtio.c_cc[VTIME]  = 1;
  newtio.c_cc[VMIN] = 0;

  newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
  newtio.c_oflag &= ~OPOST; /*Output*/

  ::tcflush(fileDescriptor, TCIFLUSH);

  errID = ::tcsetattr(fileDescriptor, TCSANOW, &newtio);
  if(errID != 0)
  {
    qDebug()<<DMODULE_NAME<<":"<<"com set error";
    goto failed;
  }

  //set uart to lin state, need modify kernel
  ::ioctl(fileDescriptor, TIOCSBRK, 0);

  LinDeviceStatus = true;
  return;
failed:
  ::close(fileDescriptor);
}

//basicly the length will always be 8.
void AmbientLinComm::transferDataToLin(uint8_t PID, uint8_t* buffer, uint32_t length)
{
  uint8_t parityPID;
  uint8_t sendBuffer[11] ,verifyBuffer[11];

//  qDebug()<<DMODULE_NAME<<":"<<"write to lin:\n";
//  qDebug()<<buffer[0];
//  qDebug()<<buffer[1];
//  qDebug()<<buffer[2];
//  qDebug()<<buffer[3];
//  qDebug()<<buffer[4];
//  qDebug()<<buffer[5];
//  qDebug()<<buffer[6];
//  qDebug()<<buffer[7]<<"\n";

  if (LinDeviceStatus == false)
    return;

  if (length == 0)
    return;

  length = (length>8)?8:length;

  parityPID = this->calculateParityPID(PID);

  ::memset(sendBuffer, 0xff, 11);

  sendBuffer[0] = 0x55;
  sendBuffer[1] = parityPID;
  ::memcpy(&sendBuffer[2], buffer, length);
  sendBuffer[10] = this->calculateCheckSum(parityPID, &sendBuffer[2], 8);

  ::tcflush(fileDescriptor, TCIFLUSH);
  ::write(fileDescriptor, sendBuffer, 11);
  ::read(fileDescriptor, verifyBuffer , 11);
  //TODO...
  //verify what we sent.
}

void AmbientLinComm::transferHeaderToLin(uint8_t header)
{
  uint8_t sendBuffer[2];

//  qDebug()<<DMODULE_NAME<<":"<<"write to lin:"<<header;

  if (LinDeviceStatus == false)
    return;

  if (header == 0)
    return;

  sendBuffer[0] = 0x55;
  sendBuffer[1] = this->calculateParityPID(header);
  ::tcflush(fileDescriptor, TCIFLUSH);
  ::write(fileDescriptor, sendBuffer, 2);
}

//basicly the length will always be 8.
uint32_t AmbientLinComm::retrieveDataFromLin(uint8_t* buffer, uint32_t desiredLength)
{
  uint8_t recvBuffer[13];
  uint8_t parityPID;
  uint8_t checkSum;
  int32_t length;

  if (desiredLength == 0)
    return 0;

  ::memset(buffer, 0x0, desiredLength);
  ::memset(recvBuffer, 0x0, 13);
  length = ::read(fileDescriptor, recvBuffer, 13);

//  qDebug()<<DMODULE_NAME<<":"<<"receive from lin:\n";
//  for (int a = 0; a < 11 ; a++)
//  {
//    qDebug()<<recvBuffer[a];
//  }

  if (recvBuffer[12] != 0)
    return 0;

  if (length != 12)
    return 0;

//  qDebug()<<"length = "<<length;

  desiredLength = 8;
  parityPID = recvBuffer[2];
  checkSum = this->calculateCheckSum(parityPID, &recvBuffer[3], desiredLength);

  if (checkSum != recvBuffer[11])
    return 0;

  ::memcpy(buffer, &recvBuffer[3], desiredLength);

  ::tcflush(fileDescriptor, TCIFLUSH);

  return desiredLength;
}

#define BIT(A,B)      ((A>>B)&0x01)
uint8_t AmbientLinComm::calculateParityPID(uint8_t PID)
{
  uint8_t parity, p0,p1;

  parity = PID;

  p0=(BIT(parity,0)^BIT(parity,1)^BIT(parity,2)^BIT(parity,4))<<6;
  p1=(!(BIT(parity,1)^BIT(parity,3)^BIT(parity,4)^BIT(parity,5)))<<7;
  parity|=(p0|p1);

  return parity;
}

uint8_t AmbientLinComm::calculateCheckSum(uint8_t parityPID, uint8_t* buffer, uint32_t length)
{
  unsigned short sum = parityPID;
  uint32_t i;

  //after calculating parity PID, 0x3c becomes 0x3c, 0x3d becomes 0x7d
  if ((parityPID == 0x3c) || (parityPID == 0x7d))
    sum = 0;

  for(i = 0;i < length;i++)
  {
      sum += buffer[i];

      if(sum&0xFF00)
      {
          sum = (sum&0x00FF) + 1;
      }
  }
  sum ^= 0x00FF;
  return((uint8_t)sum);
}
