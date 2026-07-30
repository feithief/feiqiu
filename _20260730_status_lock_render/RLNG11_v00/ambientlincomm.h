#ifndef AMBIENTLINCOMM_H
#define AMBIENTLINCOMM_H

#include <QByteArray>
#include <QString>
#include <QtGlobal>

#include "lintransport.h"

class QThread;

/*
 * Low-level LIN/UART transport.
 *
 * This class deliberately has no singleton and no internal lock.  One
 * LinBusWorker creates and owns it, so the file descriptor has exactly one
 * execution context.
 */
class AmbientLinComm : public LinTransport
{
public:
  AmbientLinComm(const QString &deviceName,
                 quint32 baudRate,
                 int ioTimeoutMs);
  ~AmbientLinComm() override;

  bool openDevice() override;
  void closeDevice() override;
  bool isDeviceReady() const override;

  bool sendFrame(quint8 frameId,
                 const QByteArray &payload,
                 LinChecksumMode checksumMode) override;
  bool sendHeader(quint8 frameId) override;
  bool readResponse(quint8 expectedFrameId,
                    int expectedDataLength,
                    LinChecksumMode checksumMode,
                    QByteArray *payload) override;

  LinIoError lastError() const override;
  QString lastErrorText() const override;

private:
  Q_DISABLE_COPY(AmbientLinComm)

  QString device;
  int fileDescriptor;
  quint32 baud;
  int ioDeadlineMs;
  LinIoError currentError;
  QString currentErrorText;
  QThread *ownerThread;

  void assertOwnerThread() const;
  void setError(LinIoError error, const QString &message);
  quint8 calculateParityPid(quint8 frameId) const;
  quint8 calculateChecksum(quint8 parityPid,
                           const quint8 *buffer,
                           int length,
                           LinChecksumMode checksumMode) const;
  bool waitForIo(short events,
                 int timeoutMs,
                 const QString &operation);
  bool writeWithTimeout(const QByteArray &data,
                        const QString &operation);
  bool readWithTimeout(QByteArray *buffer,
                       int expectedLength,
                       const QString &operation);
  bool writeAndReadEcho(const QByteArray &data);
};

class AmbientLinCommFactory : public LinTransportFactory
{
public:
  LinTransport *create(const QString &deviceName,
                       quint32 baudRate,
                       int ioTimeoutMs) const override;
};

#endif // AMBIENTLINCOMM_H
