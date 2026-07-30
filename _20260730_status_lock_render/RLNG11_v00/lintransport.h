#ifndef LINTRANSPORT_H
#define LINTRANSPORT_H

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
 * Worker-side transport port.
 *
 * Implementations are created, used and destroyed by the LIN worker thread.
 * A fake transport can therefore test scheduling and diagnostics without
 * changing UI, runtime facade or protocol code.
 */
class LinTransport
{
public:
  virtual ~LinTransport()
  {
  }

  virtual bool openDevice() = 0;
  virtual void closeDevice() = 0;
  virtual bool isDeviceReady() const = 0;

  virtual bool sendFrame(quint8 frameId,
                         const QByteArray &payload,
                         LinChecksumMode checksumMode) = 0;
  virtual bool sendHeader(quint8 frameId) = 0;
  virtual bool readResponse(quint8 expectedFrameId,
                            int expectedDataLength,
                            LinChecksumMode checksumMode,
                            QByteArray *payload) = 0;

  virtual LinIoError lastError() const = 0;
  virtual QString lastErrorText() const = 0;

protected:
  LinTransport()
  {
  }

private:
  Q_DISABLE_COPY(LinTransport)
};

class LinTransportFactory
{
public:
  virtual ~LinTransportFactory()
  {
  }

  virtual LinTransport *create(const QString &deviceName,
                               quint32 baudRate,
                               int ioTimeoutMs) const = 0;

protected:
  LinTransportFactory()
  {
  }

private:
  Q_DISABLE_COPY(LinTransportFactory)
};

#endif // LINTRANSPORT_H
