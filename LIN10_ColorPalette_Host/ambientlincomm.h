#ifndef AMBIENTLINCOMM_H
#define AMBIENTLINCOMM_H

#include <QByteArray>
#include <QString>
#include <QtGlobal>

#include "lin_types.h"

enum LinIoError
{
  ELinIoNoError = 0,
  ELinIoNotOpen,
  ELinIoInvalidArgument,
  ELinIoUnsupportedBaud,
  ELinIoOpenFailed,
  ELinIoConfigureFailed,
  ELinIoWriteFailed,
  ELinIoReadFailed,
  ELinIoTimeout,
  ELinIoEchoMismatch,
  ELinIoFrameIdMismatch,
  ELinIoDriverError,
  ELinIoChecksumError
};

/*
 * Low-level LIN/UART transport.
 *
 * This class deliberately has no singleton and no internal lock.  One
 * LinBusWorker creates and owns it, so the file descriptor has exactly one
 * execution context.
 */
class AmbientLinComm
{
public:
  AmbientLinComm(const QString &deviceName,
                 quint32 baudRate,
                 int ioTimeoutMs);
  ~AmbientLinComm();

  bool openDevice();
  void closeDevice();
  bool isDeviceReady() const;

  bool sendFrame(quint8 frameId,
                 const QByteArray &payload,
                 LinChecksumMode checksumMode);
  bool sendHeader(quint8 frameId);
  bool readResponse(quint8 expectedFrameId,
                    int expectedDataLength,
                    LinChecksumMode checksumMode,
                    QByteArray *payload);

  LinIoError lastError() const;
  QString lastErrorText() const;

private:
  Q_DISABLE_COPY(AmbientLinComm)

  QString device;
  int fileDescriptor;
  quint32 baud;
  int ioDeadlineMs;
  LinIoError currentError;
  QString currentErrorText;

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

#endif // AMBIENTLINCOMM_H
