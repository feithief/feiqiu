#include "ambientlincomm.h"

#include <QByteArray>
#include <QElapsedTimer>
#include <QThread>

#ifdef Q_OS_LINUX
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace {

#ifdef Q_OS_LINUX
bool termiosBaudRate(quint32 baudRate, speed_t *result)
{
  if (result == 0)
    return false;

  switch (baudRate)
  {
    case 300:    *result = B300; break;
    case 600:    *result = B600; break;
    case 1200:   *result = B1200; break;
    case 2400:   *result = B2400; break;
    case 4800:   *result = B4800; break;
    case 9600:   *result = B9600; break;
    case 19200:  *result = B19200; break;
    case 38400:  *result = B38400; break;
    case 57600:  *result = B57600; break;
    case 115200: *result = B115200; break;
    default:     return false;
  }

  return true;
}

QString systemErrorText(const QString &operation)
{
  return QString("%1 failed: errno=%2 (%3)")
         .arg(operation)
         .arg(errno)
         .arg(QString::fromLocal8Bit(std::strerror(errno)));
}
#endif

} // namespace

LinTransport *AmbientLinCommFactory::create(const QString &deviceName,
                                            quint32 baudRate,
                                            int ioTimeoutMs) const
{
  return new AmbientLinComm(deviceName, baudRate, ioTimeoutMs);
}

AmbientLinComm::AmbientLinComm(const QString &deviceName,
                               quint32 baudRate,
                               int ioTimeoutMs)
  : device(deviceName),
    fileDescriptor(-1),
    baud(baudRate),
    ioDeadlineMs(ioTimeoutMs),
    currentError(ELinIoNoError),
    ownerThread(QThread::currentThread())
{
}

AmbientLinComm::~AmbientLinComm()
{
  assertOwnerThread();
  closeDevice();
}

bool AmbientLinComm::openDevice()
{
  assertOwnerThread();

  if (isDeviceReady())
    return true;

#ifndef Q_OS_LINUX
  setError(ELinIoOpenFailed,
           QString("Real LIN transport is only available on Linux; %1 cannot be opened on this platform")
           .arg(device));
  return false;
#else
  if (ioDeadlineMs <= 0)
  {
    setError(ELinIoInvalidArgument,
             QString("Serial I/O timeout must be positive"));
    return false;
  }

  speed_t speed = 0;
  if (!termiosBaudRate(baud, &speed))
  {
    setError(ELinIoUnsupportedBaud,
             QString("Unsupported LIN baud rate: %1").arg(baud));
    return false;
  }

  const QByteArray path = device.toLocal8Bit();
  /*
   * ttySAC3 is a vendor LIN/UART driver rather than a normal desktop UART.
   * Its proven application contract uses a blocking descriptor.  In
   * particular, keeping O_NONBLOCK set makes the driver's transmit echo look
   * like a hard I/O failure even though the LIN frame has already gone out.
   */
  fileDescriptor = ::open(path.constData(), O_RDWR | O_NOCTTY);
  if (fileDescriptor < 0)
  {
    setError(ELinIoOpenFailed, systemErrorText(QString("open %1").arg(device)));
    return false;
  }

  if (::fcntl(fileDescriptor, F_SETFL, 0) < 0)
  {
    setError(ELinIoConfigureFailed, systemErrorText("fcntl F_SETFL"));
    closeDevice();
    return false;
  }

  struct termios options;
  std::memset(&options, 0, sizeof(options));
  options.c_cflag = CLOCAL | CREAD | CS8;
  options.c_cflag &= static_cast<tcflag_t>(~PARENB);
  options.c_cflag &= static_cast<tcflag_t>(~CSTOPB);
  options.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO | ECHOE | ISIG));
  options.c_oflag &= static_cast<tcflag_t>(~OPOST);
  /* Match the deployed ttySAC3 driver contract: 0.1 s inter-byte timeout. */
  options.c_cc[VTIME] = 1;
  options.c_cc[VMIN] = 0;

  ::cfsetispeed(&options, speed);
  ::cfsetospeed(&options, speed);

  ::tcflush(fileDescriptor, TCIFLUSH);
  if (::tcsetattr(fileDescriptor, TCSANOW, &options) != 0)
  {
    setError(ELinIoConfigureFailed, systemErrorText("tcsetattr"));
    closeDevice();
    return false;
  }

  /* The target kernel driver interprets this as UART-to-LIN mode. */
  if (::ioctl(fileDescriptor, TIOCSBRK, 0) < 0)
  {
    setError(ELinIoConfigureFailed,
             systemErrorText("enable UART-to-LIN mode"));
    closeDevice();
    return false;
  }

  setError(ELinIoNoError, "Device opened");
  return true;
#endif
}

void AmbientLinComm::closeDevice()
{
  assertOwnerThread();

#ifdef Q_OS_LINUX
  if (fileDescriptor >= 0)
  {
    ::close(fileDescriptor);
    fileDescriptor = -1;
  }
#else
  fileDescriptor = -1;
#endif
}

bool AmbientLinComm::isDeviceReady() const
{
  assertOwnerThread();
  return fileDescriptor >= 0;
}

bool AmbientLinComm::sendFrame(quint8 frameId,
                               const QByteArray &payload,
                               LinChecksumMode checksumMode)
{
  assertOwnerThread();

  if (!isDeviceReady())
  {
    setError(ELinIoNotOpen, "Cannot send LIN frame: device is not open");
    return false;
  }

  if ((frameId > 0x3F) || payload.isEmpty() || (payload.size() > 8))
  {
    setError(ELinIoInvalidArgument, "LIN frame ID or payload length is invalid");
    return false;
  }

  const quint8 parityPid = calculateParityPid(frameId);
  QByteArray frame;
  frame.reserve(payload.size() + 3);
  frame.append(static_cast<char>(0x55));
  frame.append(static_cast<char>(parityPid));
  frame.append(payload);
  frame.append(static_cast<char>(calculateChecksum(
    parityPid,
    reinterpret_cast<const quint8 *>(payload.constData()),
    payload.size(),
    checksumMode)));

  return writeAndReadEcho(frame);
}

bool AmbientLinComm::sendHeader(quint8 frameId)
{
  assertOwnerThread();

  if (!isDeviceReady())
  {
    setError(ELinIoNotOpen, "Cannot send LIN header: device is not open");
    return false;
  }

  if (frameId > 0x3F)
  {
    setError(ELinIoInvalidArgument, "LIN frame ID is invalid");
    return false;
  }

  QByteArray header;
  header.append(static_cast<char>(0x55));
  header.append(static_cast<char>(calculateParityPid(frameId)));

#ifdef Q_OS_LINUX
  ::tcflush(fileDescriptor, TCIFLUSH);
  if (!writeWithTimeout(header, QString("write LIN header")))
    return false;

  setError(ELinIoNoError, "LIN header sent");
  return true;
#else
  Q_UNUSED(header);
  setError(ELinIoNotOpen, "Real LIN transport is unavailable on this platform");
  return false;
#endif
}

bool AmbientLinComm::readResponse(quint8 expectedFrameId,
                                  int expectedDataLength,
                                  LinChecksumMode checksumMode,
                                  QByteArray *payload)
{
  assertOwnerThread();

  if (payload == 0)
  {
    setError(ELinIoInvalidArgument, "Response output pointer is null");
    return false;
  }

  payload->clear();

  if (!isDeviceReady())
  {
    setError(ELinIoNotOpen, "Cannot receive LIN frame: device is not open");
    return false;
  }

  if ((expectedFrameId > 0x3F) ||
      (expectedDataLength <= 0) || (expectedDataLength > 8))
  {
    setError(ELinIoInvalidArgument, "Expected LIN payload length is invalid");
    return false;
  }

  /*
   * Target driver format:
   * break/sync marker, sync, PID, data, checksum, followed by a driver status
   * byte.  The status byte is written after the returned byte count.
   */
  const int expectedReadLength = expectedDataLength + 4;
  QByteArray received(expectedReadLength + 1, static_cast<char>(0));
  if (!readWithTimeout(&received,
                       expectedReadLength,
                       QString("read LIN response")))
    return false;

  if (static_cast<quint8>(received.at(expectedReadLength)) != 0)
  {
    setError(ELinIoDriverError,
             QString("LIN driver status is %1")
             .arg(static_cast<int>(static_cast<quint8>(
               received.at(expectedReadLength)))));
    return false;
  }

  const quint8 parityPid = static_cast<quint8>(received.at(2));
  const quint8 expectedParityPid = calculateParityPid(expectedFrameId);
  if (parityPid != expectedParityPid)
  {
    setError(ELinIoFrameIdMismatch,
             QString("LIN response PID mismatch: expected 0x%1, got 0x%2")
             .arg(static_cast<int>(expectedParityPid), 2, 16, QChar('0'))
             .arg(static_cast<int>(parityPid), 2, 16, QChar('0')));
    return false;
  }

  const quint8 expectedChecksum = calculateChecksum(
    parityPid,
    reinterpret_cast<const quint8 *>(received.constData() + 3),
    expectedDataLength,
    checksumMode);
  const quint8 actualChecksum = static_cast<quint8>(
    received.at(3 + expectedDataLength));

  if (expectedChecksum != actualChecksum)
  {
    setError(ELinIoChecksumError,
             QString("LIN checksum mismatch: expected 0x%1, got 0x%2")
             .arg(static_cast<int>(expectedChecksum), 2, 16, QChar('0'))
             .arg(static_cast<int>(actualChecksum), 2, 16, QChar('0')));
    return false;
  }

  *payload = received.mid(3, expectedDataLength);
#ifdef Q_OS_LINUX
  ::tcflush(fileDescriptor, TCIFLUSH);
#endif
  setError(ELinIoNoError, "LIN response received");
  return true;
}

LinIoError AmbientLinComm::lastError() const
{
  assertOwnerThread();
  return currentError;
}

QString AmbientLinComm::lastErrorText() const
{
  assertOwnerThread();
  return currentErrorText;
}

void AmbientLinComm::assertOwnerThread() const
{
  Q_ASSERT(QThread::currentThread() == ownerThread);
}

void AmbientLinComm::setError(LinIoError error, const QString &message)
{
  currentError = error;
  currentErrorText = message;
}

bool AmbientLinComm::writeAndReadEcho(const QByteArray &data)
{
#ifndef Q_OS_LINUX
  Q_UNUSED(data);
  setError(ELinIoNotOpen, "Real LIN transport is unavailable on this platform");
  return false;
#else
  ::tcflush(fileDescriptor, TCIFLUSH);
  if (!writeWithTimeout(data, QString("write LIN frame")))
    return false;

  /*
   * A complete write is the transport's send result.  ttySAC3 revisions do
   * not all expose the optional local echo in the same way, so echo is useful
   * diagnostic information but must never turn a transmitted frame into a
   * failed transaction.
  */
  QByteArray echo(data.size(), static_cast<char>(0));
  const ssize_t echoLength = ::read(fileDescriptor,
                                    echo.data(),
                                    echo.size());
  if (echoLength != data.size())
  {
    const QString echoWarning = (echoLength < 0)
                                ? systemErrorText("read LIN transmit echo")
                                : QString("received %1/%2 bytes")
                                  .arg(static_cast<qlonglong>(echoLength))
                                  .arg(data.size());
    setError(ELinIoNoError,
             QString("LIN frame sent; transmit echo unavailable (%1)")
             .arg(echoWarning));
    return true;
  }

  if (echo != data)
  {
    setError(ELinIoNoError,
             QString("LIN frame sent; transmit echo differs: expected [%1], got [%2]")
             .arg(QString::fromLatin1(data.toHex(' ').toUpper()))
             .arg(QString::fromLatin1(echo.toHex(' ').toUpper())));
    return true;
  }

  setError(ELinIoNoError, "LIN frame sent");
  return true;
#endif
}

bool AmbientLinComm::waitForIo(short events,
                               int timeoutMs,
                               const QString &operation)
{
#ifndef Q_OS_LINUX
  Q_UNUSED(events);
  Q_UNUSED(timeoutMs);
  setError(ELinIoNotOpen,
           QString("%1 is unavailable on this platform").arg(operation));
  return false;
#else
  if (timeoutMs <= 0)
  {
    setError(ELinIoTimeout,
             QString("%1 timed out after %2 ms")
             .arg(operation)
             .arg(ioDeadlineMs));
    return false;
  }

  struct pollfd descriptor;
  descriptor.fd = fileDescriptor;
  descriptor.events = events;
  descriptor.revents = 0;

  QElapsedTimer timer;
  timer.start();
  int result = -1;
  while (timer.elapsed() < timeoutMs)
  {
    descriptor.revents = 0;
    const int remaining = timeoutMs - static_cast<int>(timer.elapsed());
    result = ::poll(&descriptor, 1, qMax(0, remaining));
    if ((result >= 0) || (errno != EINTR))
      break;
  }

  if ((result == 0) || ((result < 0) && (errno == EINTR)))
  {
    setError(ELinIoTimeout,
             QString("%1 timed out after %2 ms")
             .arg(operation)
             .arg(ioDeadlineMs));
    return false;
  }

  if (result < 0)
  {
    setError(ELinIoDriverError, systemErrorText(QString("poll for %1")
                                                .arg(operation)));
    return false;
  }

  if ((descriptor.revents & events) != 0)
    return true;

  setError(ELinIoDriverError,
           QString("%1 failed: poll events=0x%2")
           .arg(operation)
           .arg(static_cast<int>(descriptor.revents), 0, 16));
  return false;
#endif
}

bool AmbientLinComm::writeWithTimeout(const QByteArray &data,
                                      const QString &operation)
{
#ifndef Q_OS_LINUX
  Q_UNUSED(data);
  setError(ELinIoNotOpen,
           QString("%1 is unavailable on this platform").arg(operation));
  return false;
#else
  QElapsedTimer timer;
  timer.start();

  /*
   * ttySAC3 treats one write() call as one complete LIN transaction and
   * generates the Break in the driver.  Never repair a short write with a
   * second write(), because that can create another Break and a partial
   * frame.  Frames are at most 11 bytes, so an exact atomic write is the only
   * successful result.
   */
  while (timer.elapsed() < ioDeadlineMs)
  {
    const int remaining = ioDeadlineMs - static_cast<int>(timer.elapsed());
    if (remaining <= 0)
    {
      setError(ELinIoTimeout,
               QString("%1 timed out after %2 ms")
               .arg(operation)
               .arg(ioDeadlineMs));
      return false;
    }

    if (!waitForIo(POLLOUT, remaining, operation))
      return false;

    const ssize_t written = ::write(fileDescriptor,
                                    data.constData(),
                                    data.size());
    if (written == data.size())
      return true;

    if (written >= 0)
    {
      setError(ELinIoWriteFailed,
               QString("%1 wrote only %2/%3 bytes; frame was not retried")
               .arg(operation)
               .arg(static_cast<qlonglong>(written))
               .arg(data.size()));
      return false;
    }

    if ((written < 0) && ((errno == EAGAIN) ||
                          (errno == EWOULDBLOCK) ||
                          (errno == EINTR)))
      continue;

    setError(ELinIoWriteFailed, systemErrorText(operation));
    return false;
  }

  setError(ELinIoTimeout,
           QString("%1 timed out after %2 ms")
           .arg(operation)
           .arg(ioDeadlineMs));
  return false;
#endif
}

bool AmbientLinComm::readWithTimeout(QByteArray *buffer,
                                     int expectedLength,
                                     const QString &operation)
{
#ifndef Q_OS_LINUX
  Q_UNUSED(buffer);
  Q_UNUSED(expectedLength);
  setError(ELinIoNotOpen,
           QString("%1 is unavailable on this platform").arg(operation));
  return false;
#else
  if ((buffer == 0) || (expectedLength <= 0) ||
      (buffer->size() < expectedLength))
  {
    setError(ELinIoInvalidArgument,
             QString("%1 has an invalid output buffer").arg(operation));
    return false;
  }

  QElapsedTimer timer;
  timer.start();
  int offset = 0;

  while ((offset < expectedLength) && (timer.elapsed() < ioDeadlineMs))
  {
    const int remaining = ioDeadlineMs - static_cast<int>(timer.elapsed());
    if (!waitForIo(POLLIN, remaining, operation))
      return false;

    const ssize_t length = ::read(fileDescriptor,
                                  buffer->data() + offset,
                                  buffer->size() - offset);
    if (length > 0)
    {
      if (length > (expectedLength - offset))
      {
        setError(ELinIoDriverError,
                 QString("%1 returned more bytes than expected")
                 .arg(operation));
        return false;
      }

      offset += static_cast<int>(length);
      continue;
    }

    if ((length < 0) && ((errno == EAGAIN) ||
                         (errno == EWOULDBLOCK) ||
                         (errno == EINTR)))
      continue;

    if (length < 0)
    {
      setError(ELinIoReadFailed, systemErrorText(operation));
      return false;
    }
  }

  if (offset != expectedLength)
  {
    setError(ELinIoTimeout,
             QString("%1 timed out after %2 ms (%3/%4 bytes)")
             .arg(operation)
             .arg(ioDeadlineMs)
             .arg(offset)
             .arg(expectedLength));
    return false;
  }

  return true;
#endif
}

#define BIT_VALUE(value, bit) (((value) >> (bit)) & 0x01)

quint8 AmbientLinComm::calculateParityPid(quint8 frameId) const
{
  quint8 parityPid = static_cast<quint8>(frameId & 0x3F);
  const quint8 p0 = static_cast<quint8>(
    (BIT_VALUE(parityPid, 0) ^ BIT_VALUE(parityPid, 1) ^
     BIT_VALUE(parityPid, 2) ^ BIT_VALUE(parityPid, 4)) << 6);
  const quint8 p1 = static_cast<quint8>(
    (!(BIT_VALUE(parityPid, 1) ^ BIT_VALUE(parityPid, 3) ^
       BIT_VALUE(parityPid, 4) ^ BIT_VALUE(parityPid, 5))) << 7);
  return static_cast<quint8>(parityPid | p0 | p1);
}

quint8 AmbientLinComm::calculateChecksum(quint8 parityPid,
                                         const quint8 *buffer,
                                         int length,
                                         LinChecksumMode checksumMode) const
{
  quint16 sum = (checksumMode == ELinChecksumClassic) ? 0 : parityPid;

  for (int index = 0; index < length; ++index)
  {
    sum = static_cast<quint16>(sum + buffer[index]);
    if ((sum & 0xFF00) != 0)
      sum = static_cast<quint16>((sum & 0x00FF) + 1);
  }

  return static_cast<quint8>((sum ^ 0x00FF) & 0xFF);
}
