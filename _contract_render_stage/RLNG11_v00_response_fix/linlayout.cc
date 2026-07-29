#include "linlayout.h"
#include "generated/linprofile_generated.h"

#include <cstring>

namespace {

static const quint8 kNormalNad =
  static_cast<quint8>(ELinDiagnosticNadNormal);
static const quint8 kCalibrationNad =
  static_cast<quint8>(ELinDiagnosticNadCalibration);
static const quint8 kNormalAndCalibrationNad = static_cast<quint8>(
  ELinDiagnosticNadNormal | ELinDiagnosticNadCalibration);

/* Profile tables are generated from LDF + JSON and compiled separately. */

const int kMaximumSerialIoTimeoutMs = 1000;
const int kMaximumBlockingLinDelayMs = 1000;

quint64 logicalSignalValue(const BCMSignal &values, LinLogicalSignal signal)
{
  switch (signal)
  {
    case ELinSignalTargetMask:           return values.targetMask;
    case ELinSignalDirectRgbEnable:      return values.directRgbEnabled ? 1 : 0;
    case ELinSignalLedEnable:            return values.ledEnabled ? 1 : 0;
    case ELinSignalFadingEnable:         return values.fadingEnabled ? 1 : 0;
    case ELinSignalFadingTime:           return values.fadingTime;
    case ELinSignalIntensity:            return values.intensity;
    case ELinSignalRedOrPredefinedColor: return values.redOrPredefinedColor;
    case ELinSignalGreen:                return values.green;
    case ELinSignalBlue:                 return values.blue;
    case ELinSignalGroupAddressing:      return values.groupAddressing ? 1 : 0;
    case ELinSignalIntensityNormalization:
      return values.intensityNormalization ? 1 : 0;
    case ELinSignalDimmingTimeRelative:
      return values.dimmingTimeRelative ? 1 : 0;
    case ELinSignalDimmingTimeBase250ms:
      return values.dimmingTimeBase250ms ? 1 : 0;
    case ELinSignalSpecialFunction:      return values.specialFunction;
    case ELinSignalCommandValidity:      return values.commandValidity ? 1 : 0;
  }

  return 0;
}

bool isBooleanLogicalSignal(LinLogicalSignal signal)
{
  switch (signal)
  {
    case ELinSignalDirectRgbEnable:
    case ELinSignalLedEnable:
    case ELinSignalFadingEnable:
    case ELinSignalGroupAddressing:
    case ELinSignalIntensityNormalization:
    case ELinSignalDimmingTimeRelative:
    case ELinSignalDimmingTimeBase250ms:
    case ELinSignalCommandValidity:
      return true;
    default:
      return false;
  }
}

quint8 logicalSignalStorageBits(LinLogicalSignal signal)
{
  switch (signal)
  {
    case ELinSignalTargetMask:
      return 16;
    case ELinSignalFadingTime:
    case ELinSignalIntensity:
    case ELinSignalRedOrPredefinedColor:
    case ELinSignalGreen:
    case ELinSignalBlue:
    case ELinSignalSpecialFunction:
      return 8;
    case ELinSignalDirectRgbEnable:
    case ELinSignalLedEnable:
    case ELinSignalFadingEnable:
    case ELinSignalGroupAddressing:
    case ELinSignalIntensityNormalization:
    case ELinSignalDimmingTimeRelative:
    case ELinSignalDimmingTimeBase250ms:
    case ELinSignalCommandValidity:
      return 1;
  }

  return 0;
}

void assignLogicalSignal(BCMSignal *values,
                         LinLogicalSignal signal,
                         quint64 value)
{
  switch (signal)
  {
    case ELinSignalTargetMask:           values->targetMask = static_cast<quint16>(value); break;
    case ELinSignalDirectRgbEnable:      values->directRgbEnabled = (value != 0); break;
    case ELinSignalLedEnable:            values->ledEnabled = (value != 0); break;
    case ELinSignalFadingEnable:         values->fadingEnabled = (value != 0); break;
    case ELinSignalFadingTime:           values->fadingTime = static_cast<quint8>(value); break;
    case ELinSignalIntensity:            values->intensity = static_cast<quint8>(value); break;
    case ELinSignalRedOrPredefinedColor: values->redOrPredefinedColor = static_cast<quint8>(value); break;
    case ELinSignalGreen:                values->green = static_cast<quint8>(value); break;
    case ELinSignalBlue:                 values->blue = static_cast<quint8>(value); break;
    case ELinSignalGroupAddressing:      values->groupAddressing = (value != 0); break;
    case ELinSignalIntensityNormalization:
      values->intensityNormalization = (value != 0);
      break;
    case ELinSignalDimmingTimeRelative:
      values->dimmingTimeRelative = (value != 0);
      break;
    case ELinSignalDimmingTimeBase250ms:
      values->dimmingTimeBase250ms = (value != 0);
      break;
    case ELinSignalSpecialFunction:
      values->specialFunction = static_cast<quint8>(value);
      break;
    case ELinSignalCommandValidity:
      values->commandValidity = (value != 0);
      break;
  }
}

bool writeBits(QByteArray *frame,
               quint16 startBit,
               quint8 bitLength,
               quint64 value)
{
  if ((bitLength == 0) || (bitLength > 32))
    return false;

  if ((static_cast<int>(startBit) + bitLength) > (frame->size() * 8))
    return false;

  for (quint8 bit = 0; bit < bitLength; ++bit)
  {
    const int frameBit = startBit + bit;
    const int byteIndex = frameBit / 8;
    const quint8 mask = static_cast<quint8>(1U << (frameBit % 8));
    quint8 byteValue = static_cast<quint8>(frame->at(byteIndex));

    if ((value & (static_cast<quint64>(1) << bit)) != 0)
      byteValue = static_cast<quint8>(byteValue | mask);
    else
      byteValue = static_cast<quint8>(byteValue & ~mask);

    (*frame)[byteIndex] = static_cast<char>(byteValue);
  }

  return true;
}

quint64 readBits(const QByteArray &frame,
                 quint16 startBit,
                 quint8 bitLength)
{
  quint64 value = 0;

  if ((bitLength == 0) || (bitLength > 32))
    return 0;

  if ((static_cast<int>(startBit) + bitLength) > (frame.size() * 8))
    return 0;

  for (quint8 bit = 0; bit < bitLength; ++bit)
  {
    const int frameBit = startBit + bit;
    const quint8 byteValue = static_cast<quint8>(frame.at(frameBit / 8));
    if ((byteValue & (1U << (frameBit % 8))) != 0)
      value |= (static_cast<quint64>(1) << bit);
  }

  return value;
}

quint16 readLittleEndian16(const QByteArray &value)
{
  if (value.size() < 2)
    return 0;

  return static_cast<quint16>(static_cast<quint8>(value.at(0))) |
         static_cast<quint16>(static_cast<quint8>(value.at(1)) << 8);
}

void appendLittleEndian16(QByteArray *value, quint16 number)
{
  value->append(static_cast<char>(number & 0xFF));
  value->append(static_cast<char>((number >> 8) & 0xFF));
}

CalibrationInfo decodeCalibration(const QByteArray &value)
{
  CalibrationInfo result;
  result.x = 0;
  result.y = 0;
  result.Y = 0;

  if (value.size() < 8)
    return result;

  const quint16 x = readLittleEndian16(value.mid(0, 2));
  const quint16 y = readLittleEndian16(value.mid(2, 2));
  const quint32 luminance =
    static_cast<quint32>(static_cast<quint8>(value.at(4))) |
    (static_cast<quint32>(static_cast<quint8>(value.at(5))) << 8) |
    (static_cast<quint32>(static_cast<quint8>(value.at(6))) << 16) |
    (static_cast<quint32>(static_cast<quint8>(value.at(7))) << 24);

  result.x = static_cast<double>(x) / 10000.0;
  result.y = static_cast<double>(y) / 10000.0;
  result.Y = static_cast<double>(luminance) / 10000.0;
  return result;
}

QByteArray encodeCalibration(const CalibrationInfo &calibration)
{
  QByteArray value;
  value.reserve(8);

  const double scaledX = qBound(0.0,
                                calibration.x * 10000.0,
                                65535.0);
  const double scaledY = qBound(0.0,
                                calibration.y * 10000.0,
                                65535.0);
  const double scaledLuminance = qBound(
    0.0,
    calibration.Y * 10000.0,
    static_cast<double>(0xFFFFFFFFU));
  const quint16 x = static_cast<quint16>(scaledX);
  const quint16 y = static_cast<quint16>(scaledY);
  const quint32 luminance = static_cast<quint32>(scaledLuminance);

  appendLittleEndian16(&value, x);
  appendLittleEndian16(&value, y);
  value.append(static_cast<char>(luminance & 0xFF));
  value.append(static_cast<char>((luminance >> 8) & 0xFF));
  value.append(static_cast<char>((luminance >> 16) & 0xFF));
  value.append(static_cast<char>((luminance >> 24) & 0xFF));
  return value;
}

QString payloadString(const QByteArray &value)
{
  QString text = QString::fromLatin1(value.constData(), value.size());
  text.remove(QChar('\0'));
  return text.trimmed();
}

int fixedServiceLength(OperationType operation)
{
  switch (operation)
  {
    case EOperationTypeLock:
    case EOperationTypeSingleAddr:
    case EOperationTypeGroupAddr:
    case EOperationTypePlatform:
    case EOperationTypeIntensity:
      return 2;
    case EOperationTypeRValue:
    case EOperationTypeGValue:
    case EOperationTypeBValue:
      return 8;
    case EOperationTypeCalibration:
      return 1;
    case EOperationTypeID:
      return 5;
    case EOperationTypePartNO:
    case EOperationTypeSerialNO:
    case EOperationTypeSoftwareVer:
    case EOperationTypeHardwareVer:
    case EOperationTypeRaw:
      return -1;
  }

  return -1;
}

bool hasConfigurationEncoder(OperationType operation)
{
  switch (operation)
  {
    case EOperationTypeSingleAddr:
    case EOperationTypeGroupAddr:
    case EOperationTypePlatform:
    case EOperationTypeIntensity:
    case EOperationTypeRValue:
    case EOperationTypeGValue:
    case EOperationTypeBValue:
    case EOperationTypePartNO:
    case EOperationTypeSerialNO:
      return true;
    default:
      return false;
  }
}

void setStatusField(SlaveStatus *status,
                    LinStatusField field,
                    SlaveErrorFlag value)
{
  switch (field)
  {
    case ELinStatusRedOutputError:     status->ROutput_Err = value; break;
    case ELinStatusGreenOutputError:   status->GOutput_Err = value; break;
    case ELinStatusBlueOutputError:    status->BOutput_Err = value; break;
    case ELinStatusTemperatureError:   status->Temp_Err = value; break;
    case ELinStatusVoltageError:       status->Voltage_Err = value; break;
    case ELinStatusCommunicationError: status->Lin_Err = value; break;
  }
}

void initializeStatusFields(SlaveStatus *status,
                            SlaveErrorFlag value)
{
  if (status == 0)
    return;

  status->ROutput_Err = value;
  status->GOutput_Err = value;
  status->BOutput_Err = value;
  status->Temp_Err = value;
  status->Voltage_Err = value;
  status->Lin_Err = value;
}

} // namespace

const LinLayout &defaultLinLayout()
{
  return generatedLinProfile();
}

bool supportsDirectRgb(const LinLayout &layout)
{
  return (layout.colorModel == ELinColorModelDirectRgbOnly) ||
         (layout.colorModel == ELinColorModelSelectable);
}

bool supportsPredefinedColors(const LinLayout &layout)
{
  return (layout.colorModel == ELinColorModelPredefinedOnly) ||
         (layout.colorModel == ELinColorModelSelectable);
}

const LinFrameLayout *primaryControlFrame(const LinLayout &layout)
{
  if ((layout.publishedFrames == 0) ||
      (layout.primaryControlFrameIndex < 0) ||
      (layout.primaryControlFrameIndex >= layout.publishedFrameCount))
    return 0;

  return &layout.publishedFrames[layout.primaryControlFrameIndex];
}

const LinSignalLayout *findPrimaryControlSignal(
  const LinLayout &layout,
  LinLogicalSignal logicalSignal)
{
  const LinFrameLayout *frame = primaryControlFrame(layout);
  if ((frame == 0) || (frame->signalLayouts == 0) ||
      (frame->signalCount <= 0))
    return 0;

  for (int index = 0; index < frame->signalCount; ++index)
  {
    if (frame->signalLayouts[index].signal == logicalSignal)
      return &frame->signalLayouts[index];
  }

  return 0;
}

bool primaryControlHasSignal(const LinLayout &layout,
                             LinLogicalSignal logicalSignal)
{
  return findPrimaryControlSignal(layout, logicalSignal) != 0;
}

bool validateLinLayout(const LinLayout &layout, QStringList *errors)
{
  QStringList localErrors;
  const bool diagnosticsEnabled =
    layout.diagnosticModel != ELinDiagnosticModelNone;
  bool primarySignalPresent[ELinSignalCommandValidity + 1] = {false};
  quint8 primarySignalWidth[ELinSignalCommandValidity + 1] = {0};

  if ((layout.deviceName == 0) || (layout.deviceName[0] == '\0'))
    localErrors.append("LIN device name is empty");
  if (layout.baudRate == 0)
    localErrors.append("LIN baud rate is zero");
  if ((layout.serialIoTimeoutMs <= 0) ||
      (layout.serialIoTimeoutMs > kMaximumSerialIoTimeoutMs))
    localErrors.append("Serial I/O timeout must be in range 1..1000 ms");
  if ((layout.layoutName == 0) || (layout.layoutName[0] == '\0'))
    localErrors.append("LIN layout name is empty");
  if ((layout.colorModel < ELinColorModelPredefinedOnly) ||
      (layout.colorModel > ELinColorModelSelectable))
    localErrors.append("LIN color model is invalid");
  if ((layout.addressingModel < ELinAddressingModelFixedTargetMask) ||
      (layout.addressingModel >
       ELinAddressingModelAddressValueAndGroupBit))
    localErrors.append("LIN addressing model is invalid");
  if ((layout.dimmingModel < ELinDimmingModelFadeEnableAndTime) ||
      (layout.dimmingModel > ELinDimmingModelNone))
    localErrors.append("LIN dimming model is invalid");
  if ((layout.diagnosticModel < ELinDiagnosticModelNone) ||
      (layout.diagnosticModel > ELinDiagnosticModelCustomDid))
    localErrors.append("LIN diagnostic model is invalid");
  if (layout.intensityMaximum == 0)
    localErrors.append("LIN intensity maximum must be positive");
  if ((layout.dimmingModel != ELinDimmingModelNone) &&
      (layout.dimmingMaximum == 0))
    localErrors.append("LIN dimming maximum must be positive");
  if ((layout.predefinedColorCount < 0) ||
      (layout.predefinedColorCount > 30) ||
      ((layout.predefinedColorCount == 0) !=
       (layout.predefinedColors == 0)))
    localErrors.append("Predefined-color table must contain 0..30 colors");
  if ((layout.publishedFrameCount < 0) ||
      ((layout.publishedFrameCount == 0) != (layout.publishedFrames == 0)))
    localErrors.append("Published-frame pointer/count pair is invalid");
  if ((layout.primaryControlFrameIndex < 0) ||
      (layout.primaryControlFrameIndex >= layout.publishedFrameCount))
    localErrors.append("Primary control frame index is out of range");
  if ((layout.signalPresetCount < 0) ||
      (layout.signalPresetCount > 512) ||
      ((layout.signalPresetCount == 0) != (layout.signalPresets == 0)))
  {
    localErrors.append("Signal-preset table must contain 0..512 presets");
  }
  if ((layout.signalPresetCount > 0) &&
      ((layout.signalPresetGroupName == 0) ||
       (layout.signalPresetGroupName[0] == '\0')))
  {
    localErrors.append("Signal-preset group name is empty");
  }
  if ((layout.signalPresetCount > 0) &&
      (layout.publishedFrames != 0) &&
      (layout.primaryControlFrameIndex >= 0) &&
      (layout.primaryControlFrameIndex < layout.publishedFrameCount))
  {
    const LinFrameLayout &presetFrame =
      layout.publishedFrames[layout.primaryControlFrameIndex];
    for (int presetIndex = 0;
         presetIndex < layout.signalPresetCount;
         ++presetIndex)
    {
      const LinSignalPreset &preset = layout.signalPresets[presetIndex];
      if ((preset.name == 0) || (preset.name[0] == '\0'))
        localErrors.append(QString("Signal preset %1 has an empty name")
                           .arg(presetIndex));
      if ((preset.assignmentCount <= 0) || (preset.assignments == 0))
      {
        localErrors.append(QString("Signal preset %1 has no assignments")
                           .arg(presetIndex));
        continue;
      }
      for (int assignmentIndex = 0;
           assignmentIndex < preset.assignmentCount;
           ++assignmentIndex)
      {
        const LinSignalPresetAssignment &assignment =
          preset.assignments[assignmentIndex];
        bool valueFits = false;
        if (assignment.bitLength == 32)
          valueFits = true;
        else if ((assignment.bitLength > 0) &&
                 (assignment.bitLength < 32))
        {
          valueFits = assignment.value <
            (static_cast<quint32>(1) << assignment.bitLength);
        }
        if ((assignment.signalName == 0) ||
            (assignment.signalName[0] == '\0') ||
            (assignment.bitLength == 0) ||
            (assignment.bitLength > 32) ||
            ((static_cast<int>(assignment.startBit) +
              static_cast<int>(assignment.bitLength)) >
             (static_cast<int>(presetFrame.length) * 8)) ||
            !valueFits)
        {
          localErrors.append(
            QString("Invalid assignment %1 in signal preset %2")
            .arg(assignmentIndex)
            .arg(presetIndex));
        }
      }
    }
  }
  if ((layout.nodeCount < 0) ||
      ((layout.nodeCount == 0) != (layout.nodes == 0)))
    localErrors.append("Node pointer/count pair is invalid");
  if ((layout.statusLayoutCount < 0) ||
      ((layout.statusLayoutCount == 0) != (layout.statusLayouts == 0)))
    localErrors.append("Status-layout pointer/count pair is invalid");
  if ((layout.serviceCount < 0) ||
      ((layout.serviceCount == 0) != (layout.services == 0)))
    localErrors.append("Diagnostic-service pointer/count pair is invalid");
  if ((layout.diagnosticModel == ELinDiagnosticModelCustomDid) &&
      (layout.serviceCount == 0))
    localErrors.append("Custom-DID model has no diagnostic services");
  if ((layout.diagnosticModel != ELinDiagnosticModelCustomDid) &&
      (layout.serviceCount != 0))
    localErrors.append("Diagnostic services require the Custom-DID model");
  if ((layout.nodeCount > 0) && (layout.statusLayoutCount <= 0))
    localErrors.append("Nodes require at least one status layout");
  if ((layout.bulkWriteCount < 0) ||
      ((layout.bulkWriteCount == 0) != (layout.bulkWriteSequence == 0)))
    localErrors.append("Bulk-write pointer/count pair is invalid");
  if ((layout.diagnosticModel != ELinDiagnosticModelCustomDid) &&
      (layout.bulkWriteCount != 0))
    localErrors.append("Bulk-write sequence requires the Custom-DID model");
  if ((layout.scheduleSlotCount <= 0) || (layout.schedule == 0))
    localErrors.append("LIN schedule is empty");
  if (diagnosticsEnabled &&
      ((layout.diagnosticRequestFrameId > 0x3F) ||
       (layout.diagnosticResponseFrameId > 0x3F)))
    localErrors.append("Diagnostic frame identifier is invalid");
  if (diagnosticsEnabled &&
      (layout.diagnosticRequestFrameId == layout.diagnosticResponseFrameId))
    localErrors.append("Diagnostic request/response frame IDs must differ");
  if ((layout.diagnosticRequestToResponseDelayMs < 0) ||
      (layout.diagnosticRequestToResponseDelayMs >
       kMaximumBlockingLinDelayMs) ||
      (layout.diagnosticInterFrameDelayMs < 0) ||
      (layout.diagnosticInterFrameDelayMs > kMaximumBlockingLinDelayMs) ||
      (layout.responseDelayMs < 0) ||
      (layout.responseDelayMs > kMaximumBlockingLinDelayMs) ||
      (layout.postWriteSettleMs < 0) ||
      (layout.postWriteSettleMs > kMaximumBlockingLinDelayMs))
    localErrors.append("Blocking LIN delays must be in range 0..1000 ms");
  if ((layout.bulkWriteReadBackDelayMs < 0) ||
      (layout.bulkWriteReadBackDelayMs > 10000))
    localErrors.append(
      "Bulk-write read-back delay must be in range 0..10000 ms");
  if (!diagnosticsEnabled && layout.securityAccess.enabled)
    localErrors.append("Security access requires diagnostics");
  if (layout.securityAccess.enabled &&
      ((layout.securityAccess.requestSeedSubFunction == 0) ||
       (layout.securityAccess.sendKeySubFunction == 0) ||
       (layout.securityAccess.requestSeedSubFunction ==
        layout.securityAccess.sendKeySubFunction)))
    localErrors.append("Security-access sub-functions are invalid");
  if (layout.securityAccess.enabled &&
      ((layout.securityAccess.keyLength < 1) ||
       (layout.securityAccess.keyLength > 4)))
    localErrors.append("Security-access key length must be in range 1..4 bytes");
  if (layout.maximumDiagnosticQueueDepth < 0)
    localErrors.append("Diagnostic queue depth cannot be negative");
  else if (diagnosticsEnabled &&
           (layout.maximumDiagnosticQueueDepth == 0))
    localErrors.append("Diagnostic queue depth must be positive");

  if (((layout.publishedFrameCount > 0) && (layout.publishedFrames == 0)) ||
      ((layout.nodeCount > 0) && (layout.nodes == 0)) ||
      ((layout.statusLayoutCount > 0) && (layout.statusLayouts == 0)) ||
      ((layout.serviceCount > 0) && (layout.services == 0)) ||
      (layout.schedule == 0) ||
      ((layout.bulkWriteCount > 0) && (layout.bulkWriteSequence == 0)))
  {
    if (errors != 0)
      *errors = localErrors;
    return false;
  }

  for (int frameIndex = 0;
       frameIndex < layout.publishedFrameCount;
       ++frameIndex)
  {
    const LinFrameLayout &frame = layout.publishedFrames[frameIndex];
    if ((frame.name == 0) || (frame.name[0] == '\0') ||
        (frame.length == 0) || (frame.length > 8) ||
        (frame.frameId > 0x3F) ||
        ((frame.checksumMode != ELinChecksumClassic) &&
         (frame.checksumMode != ELinChecksumEnhanced)))
      localErrors.append(QString("Invalid published frame: %1")
                         .arg(frame.name == 0
                              ? QString("<unnamed>")
                              : QString::fromLatin1(frame.name)));

    if ((frame.signalCount < 0) ||
        ((frame.signalCount == 0) != (frame.signalLayouts == 0)))
    {
      localErrors.append(QString("Signal pointer/count pair is invalid in frame %1")
                         .arg(frame.name == 0
                              ? QString("<unnamed>")
                              : QString::fromLatin1(frame.name)));
      continue;
    }

    for (int otherFrame = frameIndex + 1;
         otherFrame < layout.publishedFrameCount;
         ++otherFrame)
    {
      if (layout.publishedFrames[otherFrame].frameId == frame.frameId)
        localErrors.append(QString("Duplicate published frame ID: 0x%1")
                           .arg(static_cast<int>(frame.frameId),
                                2, 16, QChar('0')));
    }

    if (diagnosticsEnabled &&
        ((frame.frameId == layout.diagnosticRequestFrameId) ||
         (frame.frameId == layout.diagnosticResponseFrameId)))
      localErrors.append(QString("Published frame conflicts with diagnostic frame ID: 0x%1")
                         .arg(static_cast<int>(frame.frameId),
                              2, 16, QChar('0')));

    const QByteArray defaultFrame(
      reinterpret_cast<const char *>(frame.defaultPayload),
      frame.length);
    for (int index = 0; index < frame.signalCount; ++index)
    {
      const LinSignalLayout &signal = frame.signalLayouts[index];
      if ((signal.name == 0) || (signal.name[0] == '\0') ||
          (signal.signal < ELinSignalTargetMask) ||
          (signal.signal > ELinSignalCommandValidity) ||
          (signal.bitLength == 0) || (signal.bitLength > 32) ||
          ((signal.startBit + signal.bitLength) > (frame.length * 8)))
      {
        localErrors.append(QString("Invalid signal %1 in frame %2")
                           .arg(signal.name == 0
                                ? QString("<unnamed>")
                                : QString::fromLatin1(signal.name))
                           .arg(frame.name == 0
                                ? QString("<unnamed>")
                                 : QString::fromLatin1(frame.name)));
      }
      else
      {
        if (isBooleanLogicalSignal(signal.signal) &&
            (signal.bitLength != 1))
          localErrors.append(QString("Boolean signal must be one bit: %1")
                             .arg(QString::fromLatin1(signal.name)));
        else if (signal.bitLength > logicalSignalStorageBits(signal.signal))
          localErrors.append(QString("Signal is wider than its application value: %1")
                             .arg(QString::fromLatin1(signal.name)));
      }

      if ((frameIndex == layout.primaryControlFrameIndex) &&
          (signal.signal >= ELinSignalTargetMask) &&
          (signal.signal <= ELinSignalCommandValidity) &&
          (signal.bitLength > 0) && (signal.bitLength <= 32))
      {
        const int logicalIndex = static_cast<int>(signal.signal);
        primarySignalPresent[logicalIndex] = true;
        primarySignalWidth[logicalIndex] = qMax(
          primarySignalWidth[logicalIndex],
          signal.bitLength);
      }

      if ((signal.bitLength > 0) && (signal.bitLength <= 32))
      {
        const quint64 maximumValue = (signal.bitLength == 32)
                                     ? 0xFFFFFFFFULL
                                     : ((static_cast<quint64>(1) << signal.bitLength) - 1);
        if (signal.defaultValue > maximumValue)
          localErrors.append(QString("Default value is too large for signal %1")
                             .arg(signal.name == 0
                                  ? QString("<unnamed>")
                                  : QString::fromLatin1(signal.name)));
        else if (readBits(defaultFrame,
                          signal.startBit,
                          signal.bitLength) != signal.defaultValue)
          localErrors.append(QString("Signal/default-payload mismatch: %1")
                             .arg(signal.name == 0
                                  ? QString("<unnamed>")
                                  : QString::fromLatin1(signal.name)));
      }

      for (int other = index + 1; other < frame.signalCount; ++other)
      {
        const LinSignalLayout &otherSignal = frame.signalLayouts[other];
        if (otherSignal.signal == signal.signal)
          localErrors.append(QString("Duplicate signal in frame %1: %2")
                             .arg(frame.name == 0
                                  ? QString("<unnamed>")
                                  : QString::fromLatin1(frame.name))
                             .arg(signal.name == 0
                                  ? QString("<unnamed>")
                                  : QString::fromLatin1(signal.name)));

        const int signalEnd = signal.startBit + signal.bitLength;
        const int otherEnd = otherSignal.startBit + otherSignal.bitLength;
        if ((signal.startBit < otherEnd) && (otherSignal.startBit < signalEnd))
          localErrors.append(QString("Overlapping signals in frame %1")
                             .arg(frame.name == 0
                                  ? QString("<unnamed>")
                                  : QString::fromLatin1(frame.name)));
      }
    }
  }

  if (!primarySignalPresent[ELinSignalTargetMask])
    localErrors.append("Primary control frame has no target/address signal");
  if (!primarySignalPresent[ELinSignalIntensity])
    localErrors.append("Primary control frame has no intensity signal");
  if ((layout.addressingModel ==
       ELinAddressingModelAddressValueAndGroupBit) &&
      !primarySignalPresent[ELinSignalGroupAddressing])
    localErrors.append("Primary control frame has no group-addressing signal");
  if ((layout.colorModel == ELinColorModelSelectable) &&
      !primarySignalPresent[ELinSignalDirectRgbEnable])
    localErrors.append("Primary control frame has no color-selector signal");
  if (supportsPredefinedColors(layout) &&
      !primarySignalPresent[ELinSignalRedOrPredefinedColor])
    localErrors.append("Primary control frame has no color-index signal");
  if (supportsDirectRgb(layout) &&
      (!primarySignalPresent[ELinSignalRedOrPredefinedColor] ||
       !primarySignalPresent[ELinSignalGreen] ||
       !primarySignalPresent[ELinSignalBlue]))
    localErrors.append("Primary control frame does not provide R/G/B signals");
  if ((layout.dimmingModel == ELinDimmingModelFadeEnableAndTime) &&
      (!primarySignalPresent[ELinSignalFadingEnable] ||
       !primarySignalPresent[ELinSignalFadingTime]))
    localErrors.append("Primary control frame has no fade enable/time signals");
  if ((layout.dimmingModel == ELinDimmingModelRampWithTimeBase) &&
      (!primarySignalPresent[ELinSignalFadingTime] ||
       !primarySignalPresent[ELinSignalDimmingTimeRelative] ||
       !primarySignalPresent[ELinSignalDimmingTimeBase250ms]))
    localErrors.append("Primary control frame has incomplete ramp signals");

  if (primarySignalPresent[ELinSignalIntensity])
  {
    const quint8 width = primarySignalWidth[ELinSignalIntensity];
    const quint64 maximum = (width == 32)
                            ? 0xFFFFFFFFULL
                            : ((static_cast<quint64>(1) << width) - 1);
    if (layout.intensityMaximum > maximum)
      localErrors.append("Intensity maximum does not fit its signal");
  }
  if (primarySignalPresent[ELinSignalFadingTime])
  {
    const quint8 width = primarySignalWidth[ELinSignalFadingTime];
    const quint64 maximum = (width == 32)
                            ? 0xFFFFFFFFULL
                            : ((static_cast<quint64>(1) << width) - 1);
    if (layout.dimmingMaximum > maximum)
      localErrors.append("Dimming maximum does not fit its signal");
  }

  for (int index = 0; index < layout.nodeCount; ++index)
  {
    const LinNodeLayout &node = layout.nodes[index];
    if ((node.name == 0) || (node.name[0] == '\0') ||
        (node.uiSlot < 0) || (node.uiSlot >= layout.nodeCount) ||
        (node.diagnosticNad == 0) || (node.diagnosticNad > 0x7D) ||
        (node.statusFrameId > 0x3F) ||
        (node.statusLength == 0) || (node.statusLength > 8))
      localErrors.append(QString("Invalid node layout at UI slot %1").arg(node.uiSlot));

    if ((node.controlAddressMask == 0) ||
        ((layout.addressingModel ==
          ELinAddressingModelFixedTargetMask) &&
         ((node.controlAddressMask &
           (node.controlAddressMask - 1U)) != 0)))
      localErrors.append(QString("Invalid control address for node %1")
                         .arg(static_cast<int>(node.diagnosticNad)));

    if (primarySignalPresent[ELinSignalTargetMask])
    {
      const quint8 width = primarySignalWidth[ELinSignalTargetMask];
      const quint64 maximum = (width == 32)
                              ? 0xFFFFFFFFULL
                              : ((static_cast<quint64>(1) << width) - 1);
      if (node.controlAddressMask > maximum)
        localErrors.append(QString("Control address does not fit node %1")
                           .arg(static_cast<int>(node.diagnosticNad)));
    }

    if ((node.statusLayoutIndex < 0) ||
        (node.statusLayoutIndex >= layout.statusLayoutCount))
      localErrors.append(QString("Invalid status layout for node %1")
                         .arg(static_cast<int>(node.diagnosticNad)));
    if ((node.checksumMode != ELinChecksumClassic) &&
        (node.checksumMode != ELinChecksumEnhanced))
      localErrors.append(QString("Invalid checksum mode for node %1")
                         .arg(static_cast<int>(node.diagnosticNad)));

    for (int other = index + 1; other < layout.nodeCount; ++other)
    {
      if (layout.nodes[other].diagnosticNad == node.diagnosticNad)
        localErrors.append(QString("Duplicate node NAD: %1")
                           .arg(static_cast<int>(node.diagnosticNad)));
      if (layout.nodes[other].uiSlot == node.uiSlot)
        localErrors.append(QString("Duplicate node UI slot: %1").arg(node.uiSlot));
      if (layout.nodes[other].controlAddressMask == node.controlAddressMask)
        localErrors.append(QString("Duplicate node control mask: 0x%1")
                           .arg(static_cast<int>(node.controlAddressMask),
                                4, 16, QChar('0')));
      if (layout.nodes[other].statusFrameId == node.statusFrameId)
        localErrors.append(QString("Duplicate node status frame ID: 0x%1")
                           .arg(static_cast<int>(node.statusFrameId),
                                2, 16, QChar('0')));
    }


    for (int frameIndex = 0;
         frameIndex < layout.publishedFrameCount;
         ++frameIndex)
    {
      if (layout.publishedFrames[frameIndex].frameId == node.statusFrameId)
        localErrors.append(QString("Frame ID is both published and node status: 0x%1")
                           .arg(static_cast<int>(node.statusFrameId),
                                2, 16, QChar('0')));
    }


    if (diagnosticsEnabled &&
        ((node.statusFrameId == layout.diagnosticRequestFrameId) ||
         (node.statusFrameId == layout.diagnosticResponseFrameId)))
      localErrors.append(QString("Node status frame conflicts with diagnostic frame ID: 0x%1")
                         .arg(static_cast<int>(node.statusFrameId),
                              2, 16, QChar('0')));
  }

  for (int statusIndex = 0;
       statusIndex < layout.statusLayoutCount;
       ++statusIndex)
  {
    const LinStatusLayout &statusLayout = layout.statusLayouts[statusIndex];
    if ((statusLayout.name == 0) || (statusLayout.name[0] == '\0') ||
        (statusLayout.fieldCount < 0) ||
        ((statusLayout.fieldCount == 0) != (statusLayout.fields == 0)))
    {
      localErrors.append(QString("Invalid status layout at index %1")
                         .arg(statusIndex));
      continue;
    }

    for (int fieldIndex = 0;
         fieldIndex < statusLayout.fieldCount;
         ++fieldIndex)
    {
      const LinStatusFieldLayout &field = statusLayout.fields[fieldIndex];
      if ((field.name == 0) || (field.name[0] == '\0') ||
          (field.field < ELinStatusRedOutputError) ||
          (field.field > ELinStatusCommunicationError) ||
          (field.bitLength == 0) || (field.bitLength > 32) ||
          (field.normalValue == field.errorValue))
      {
        localErrors.append(QString("Invalid status field in layout %1")
                           .arg(QString::fromLatin1(statusLayout.name)));
        continue;
      }

      const quint64 maximumValue = (field.bitLength == 32)
                                   ? 0xFFFFFFFFULL
                                   : ((static_cast<quint64>(1)
                                       << field.bitLength) - 1);
      if ((field.normalValue > maximumValue) ||
          (field.errorValue > maximumValue))
        localErrors.append(QString("Status values do not fit field %1")
                           .arg(QString::fromLatin1(field.name)));

      for (int otherIndex = fieldIndex + 1;
           otherIndex < statusLayout.fieldCount;
           ++otherIndex)
      {
        const LinStatusFieldLayout &other = statusLayout.fields[otherIndex];
        const int fieldEnd = field.startBit + field.bitLength;
        const int otherEnd = other.startBit + other.bitLength;
        if ((field.startBit < otherEnd) && (other.startBit < fieldEnd))
          localErrors.append(QString("Overlapping status fields in layout %1")
                             .arg(QString::fromLatin1(statusLayout.name)));
      }

      for (int nodeIndex = 0; nodeIndex < layout.nodeCount; ++nodeIndex)
      {
        const LinNodeLayout &node = layout.nodes[nodeIndex];
        if ((node.statusLayoutIndex == statusIndex) &&
            ((field.startBit + field.bitLength) > (node.statusLength * 8)))
        {
          localErrors.append(QString("Status field %1 does not fit node %2")
                             .arg(QString::fromLatin1(field.name))
                             .arg(static_cast<int>(node.diagnosticNad)));
          break;
        }
      }
    }
  }

  for (int index = 0; index < layout.serviceCount; ++index)
  {
    const LinServiceLayout &service = layout.services[index];
    if ((service.name == 0) || (service.name[0] == '\0') ||
        (service.operation < EOperationTypeLock) ||
        (service.operation > EOperationTypeRaw) ||
        ((service.protocol != ELinDiagnosticReadByIdentifier) &&
         (service.protocol != ELinDiagnosticProductIdentification)) ||
        (service.dataLength == 0) ||
        ((service.nadAccess & kNormalAndCalibrationNad) == 0) ||
        ((service.nadAccess & ~kNormalAndCalibrationNad) != 0))
      localErrors.append(QString("Invalid diagnostic service at index %1")
                         .arg(index));

    const int requiredLength = fixedServiceLength(service.operation);
    if ((requiredLength > 0) && (service.dataLength != requiredLength))
      localErrors.append(QString("Unexpected data length for service %1")
                         .arg(service.name == 0
                              ? QString("<unnamed>")
                              : QString::fromLatin1(service.name)));
    if (!service.readable && !service.writable)
      localErrors.append(QString("Service has no read/write direction: %1")
                         .arg(service.name == 0
                              ? QString("<unnamed>")
                              : QString::fromLatin1(service.name)));
    if (service.readOnConfiguration && !service.readable)
      localErrors.append(QString("Non-readable service is marked for configuration read: %1")
                         .arg(service.name == 0
                              ? QString("<unnamed>")
                              : QString::fromLatin1(service.name)));
    if (service.expectPositiveWriteResponse && !service.writable)
      localErrors.append(QString("Non-writable service expects a write response: %1")
                         .arg(service.name == 0
                              ? QString("<unnamed>")
                              : QString::fromLatin1(service.name)));
    if ((service.protocol == ELinDiagnosticProductIdentification) &&
        (service.operation != EOperationTypeID))
      localErrors.append("Product-identification protocol must map to ProductId");

    if ((service.protocol == ELinDiagnosticProductIdentification) &&
        (service.dataLength > 5))
      localErrors.append(QString("Product-identification service is too long: %1")
                         .arg(service.name == 0
                              ? QString("<unnamed>")
                              : QString::fromLatin1(service.name)));

    for (int other = index + 1; other < layout.serviceCount; ++other)
    {
      if ((layout.services[other].operation == service.operation) &&
          (service.operation != EOperationTypeRaw))
        localErrors.append(QString("Duplicate diagnostic service: %1")
                           .arg(service.name == 0
                                ? QString("<unnamed>")
                                : QString::fromLatin1(service.name)));
      if (layout.services[other].serviceId == service.serviceId)
        localErrors.append(QString("Duplicate diagnostic service ID: 0x%1")
                           .arg(static_cast<int>(service.serviceId),
                                4, 16, QChar('0')));
    }
  }

  if ((layout.positiveWriteResponseEchoMode !=
       ELinPositiveWriteResponseEchoDid) &&
      (layout.positiveWriteResponseEchoMode !=
       ELinPositiveWriteResponseEchoDidOrFinalConsecutiveData))
    localErrors.append("Invalid positive 0x2E response echo mode");

  bool usesTemporaryNad = false;
  for (int index = 0; index < layout.serviceCount; ++index)
    usesTemporaryNad = usesTemporaryNad ||
      ((layout.services[index].nadAccess & kCalibrationNad) != 0);

  if (usesTemporaryNad)
  {
    for (int index = 0; index < layout.nodeCount; ++index)
    {
      if (layout.nodes[index].diagnosticNad == layout.temporaryNad)
      {
        localErrors.append(QString("Temporary NAD conflicts with node NAD: 0x%1")
                           .arg(static_cast<int>(layout.temporaryNad),
                                2, 16, QChar('0')));
        break;
      }
    }
  }

  bool primaryControlIsScheduled = false;
  for (int index = 0; index < layout.scheduleSlotCount; ++index)
  {
    const LinScheduleSlot &slot = layout.schedule[index];
    const bool validFrameSlot =
      (slot.type == ELinScheduleTransmitFrame) &&
      (slot.tableIndex >= 0) &&
      (slot.tableIndex < layout.publishedFrameCount);
    const bool validNodeSlot =
      (slot.type == ELinSchedulePollNode) &&
      (slot.tableIndex >= 0) &&
      (slot.tableIndex < layout.nodeCount);

    if ((!validFrameSlot && !validNodeSlot) ||
        (slot.delayAfterMs < 0) ||
        (slot.delayAfterMs > kMaximumBlockingLinDelayMs))
      localErrors.append(QString("Invalid schedule slot at index %1").arg(index));

    if (validFrameSlot &&
        (slot.tableIndex == layout.primaryControlFrameIndex))
      primaryControlIsScheduled = true;
  }

  if (!primaryControlIsScheduled)
    localErrors.append("Primary control frame is absent from the schedule");

  for (int nodeIndex = 0; nodeIndex < layout.nodeCount; ++nodeIndex)
  {
    bool scheduled = false;
    for (int slotIndex = 0; slotIndex < layout.scheduleSlotCount; ++slotIndex)
    {
      const LinScheduleSlot &slot = layout.schedule[slotIndex];
      if ((slot.type == ELinSchedulePollNode) &&
          (slot.tableIndex == nodeIndex))
      {
        scheduled = true;
        break;
      }
    }
    if (!scheduled)
      localErrors.append(QString("Node is not present in the schedule: %1")
                         .arg(static_cast<int>(
                           layout.nodes[nodeIndex].diagnosticNad)));
  }


  for (int index = 0; index < layout.bulkWriteCount; ++index)
  {
    const LinServiceLayout *service = findLinService(
      layout,
      layout.bulkWriteSequence[index]);
    if ((service == 0) || !service->writable ||
        !hasConfigurationEncoder(layout.bulkWriteSequence[index]))
      localErrors.append(QString("Bulk-write service at index %1 is invalid")
                         .arg(index));
  }

  if (errors != 0)
    *errors = localErrors;

  return localErrors.isEmpty();
}

const LinNodeLayout *findLinNode(const LinLayout &layout, quint8 nad)
{
  if ((layout.nodeCount <= 0) || (layout.nodes == 0))
    return 0;

  for (int index = 0; index < layout.nodeCount; ++index)
  {
    if (layout.nodes[index].diagnosticNad == nad)
      return &layout.nodes[index];
  }

  return 0;
}

const LinNodeLayout *findLinNodeByControlMask(const LinLayout &layout,
                                              quint16 controlAddressMask)
{
  if ((layout.nodeCount <= 0) || (layout.nodes == 0))
    return 0;

  for (int index = 0; index < layout.nodeCount; ++index)
  {
    if (layout.nodes[index].controlAddressMask == controlAddressMask)
      return &layout.nodes[index];
  }

  return 0;
}

const LinServiceLayout *findLinService(const LinLayout &layout,
                                       OperationType operation)
{
  if ((layout.serviceCount <= 0) || (layout.services == 0))
    return 0;

  for (int index = 0; index < layout.serviceCount; ++index)
  {
    if (layout.services[index].operation == operation)
      return &layout.services[index];
  }

  return 0;
}

const LinServiceLayout *findLinServiceById(const LinLayout &layout,
                                           quint16 serviceId)
{
  if ((layout.serviceCount <= 0) || (layout.services == 0))
    return 0;

  for (int index = 0; index < layout.serviceCount; ++index)
  {
    if (layout.services[index].serviceId == serviceId)
      return &layout.services[index];
  }

  return 0;
}

BCMSignal createDefaultBCMSignal(const LinLayout &layout)
{
  BCMSignal values;
  std::memset(&values, 0, sizeof(values));

  if ((layout.publishedFrameCount <= 0) || (layout.publishedFrames == 0))
    return values;

  for (int frameIndex = 0;
       frameIndex < layout.publishedFrameCount;
       ++frameIndex)
  {
    const LinFrameLayout &frame = layout.publishedFrames[frameIndex];
    if ((frame.signalCount <= 0) || (frame.signalLayouts == 0))
      continue;
    for (int signalIndex = 0;
         signalIndex < frame.signalCount;
         ++signalIndex)
    {
      const LinSignalLayout &signal = frame.signalLayouts[signalIndex];
      assignLogicalSignal(&values, signal.signal, signal.defaultValue);
    }
  }

  /* Profiles without a selector bit still have an unambiguous UI mode. */
  if (layout.colorModel == ELinColorModelDirectRgbOnly)
    values.directRgbEnabled = true;
  else if (layout.colorModel == ELinColorModelPredefinedOnly)
    values.directRgbEnabled = false;

  return values;
}

bool encodePublishedFrame(const LinFrameLayout &frameLayout,
                          const BCMSignal &values,
                          QByteArray *frame,
                          QString *errorMessage)
{
  if ((frame == 0) || (frameLayout.length == 0) ||
      (frameLayout.length > 8) ||
      (frameLayout.signalCount < 0) ||
      ((frameLayout.signalCount > 0) && (frameLayout.signalLayouts == 0)))
    return false;

  frame->clear();
  frame->append(reinterpret_cast<const char *>(frameLayout.defaultPayload),
                frameLayout.length);

  for (int index = 0; index < frameLayout.signalCount; ++index)
  {
    const LinSignalLayout &signal = frameLayout.signalLayouts[index];
    if (!writeBits(frame,
                   signal.startBit,
                   signal.bitLength,
                   logicalSignalValue(values, signal.signal)))
    {
      if (errorMessage != 0)
        *errorMessage = QString("Signal '%1' is outside the control frame")
                          .arg(QString::fromLatin1(signal.name));
      return false;
    }
  }

  return true;
}

bool applySignalPresetToPrimaryFrame(const LinLayout &layout,
                                     int presetIndex,
                                     QByteArray *frame,
                                     QString *errorMessage)
{
  const LinFrameLayout *frameLayout = primaryControlFrame(layout);
  if ((frameLayout == 0) || (frame == 0) ||
      (frame->size() != frameLayout->length) ||
      (presetIndex < 0) || (presetIndex >= layout.signalPresetCount) ||
      (layout.signalPresets == 0))
  {
    if (errorMessage != 0)
      *errorMessage = QString("Invalid signal preset or primary frame");
    return false;
  }

  const LinSignalPreset &preset = layout.signalPresets[presetIndex];
  for (int index = 0; index < preset.assignmentCount; ++index)
  {
    const LinSignalPresetAssignment &assignment = preset.assignments[index];
    if (!writeBits(frame,
                   assignment.startBit,
                   assignment.bitLength,
                   assignment.value))
    {
      if (errorMessage != 0)
      {
        *errorMessage = QString("Preset '%1' signal '%2' is outside the primary frame")
          .arg(QString::fromUtf8(preset.name))
          .arg(QString::fromLatin1(assignment.signalName));
      }
      return false;
    }
  }

  return true;
}

SlaveStatus decodeStatusFrame(const LinLayout &layout,
                              const LinNodeLayout &node,
                              const QByteArray &frame,
                              bool online)
{
  SlaveStatus status;
  status.slaveNAD = node.diagnosticNad;
  status.isOnLine = online;
  initializeStatusFields(&status, ESlaveErrorFlagUnknown);

  if (!online || (frame.size() < node.statusLength))
    return status;

  if ((node.statusLayoutIndex < 0) ||
      (node.statusLayoutIndex >= layout.statusLayoutCount) ||
      (layout.statusLayouts == 0))
    return status;

  const LinStatusLayout &statusLayout =
    layout.statusLayouts[node.statusLayoutIndex];
  if ((statusLayout.fieldCount > 0) && (statusLayout.fields == 0))
    return status;

  const int logicalFieldCount =
    static_cast<int>(ELinStatusCommunicationError) + 1;
  bool mapped[logicalFieldCount] = {false};
  bool allNormal[logicalFieldCount];
  bool anyError[logicalFieldCount] = {false};
  for (int index = 0; index < logicalFieldCount; ++index)
    allNormal[index] = true;

  for (int index = 0; index < statusLayout.fieldCount; ++index)
  {
    const LinStatusFieldLayout &field = statusLayout.fields[index];
    if ((field.field < ELinStatusRedOutputError) ||
        (field.field > ELinStatusCommunicationError) ||
        (field.bitLength == 0) || (field.bitLength > 32) ||
        ((field.startBit + field.bitLength) > (node.statusLength * 8)) ||
        ((field.startBit + field.bitLength) > (frame.size() * 8)))
      continue;

    const int logicalIndex = static_cast<int>(field.field);
    const quint32 rawValue = static_cast<quint32>(
      readBits(frame, field.startBit, field.bitLength));
    const bool isNormal = (rawValue == field.normalValue);
    const bool isError = (rawValue == field.errorValue) ||
                         (field.anyNonNormalIsError && !isNormal);
    mapped[logicalIndex] = true;
    anyError[logicalIndex] = anyError[logicalIndex] || isError;
    allNormal[logicalIndex] = allNormal[logicalIndex] && isNormal;
  }

  for (int index = 0; index < logicalFieldCount; ++index)
  {
    SlaveErrorFlag value = ESlaveErrorFlagUnknown;
    if (anyError[index])
      value = ESlaveErrorFlagError;
    else if (mapped[index] && allNormal[index])
      value = ESlaveErrorFlagNormal;

    setStatusField(&status,
                   static_cast<LinStatusField>(index),
                   value);
  }

  return status;
}

void applyLinServiceValue(const LinServiceLayout &service,
                          const QByteArray &value,
                          SlaveConfigInfo *info)
{
  /* Extend this switch only when a profile introduces a new business value. */
  if (info == 0)
    return;

  if (service.operation == EOperationTypeRaw)
    info->rawDiagnosticValues.insert(service.serviceId, value);

  switch (service.operation)
  {
    case EOperationTypeLock:
      info->locked = !value.isEmpty() &&
                     ((static_cast<quint8>(value.at(0)) & 0x80) != 0);
      break;
    case EOperationTypeSingleAddr: info->SA = readLittleEndian16(value); break;
    case EOperationTypeGroupAddr:  info->GA = readLittleEndian16(value); break;
    case EOperationTypePlatform:   info->platform = readLittleEndian16(value); break;
    case EOperationTypeIntensity:  info->intensity = readLittleEndian16(value); break;
    case EOperationTypeRValue:     info->r = decodeCalibration(value); break;
    case EOperationTypeGValue:     info->g = decodeCalibration(value); break;
    case EOperationTypeBValue:     info->b = decodeCalibration(value); break;
    case EOperationTypePartNO:     info->PN = payloadString(value); break;
    case EOperationTypeSerialNO:   info->SN = payloadString(value); break;
    case EOperationTypeSoftwareVer: info->softwareVersion = payloadString(value); break;
    case EOperationTypeHardwareVer: info->hardwareVersion = payloadString(value); break;
    case EOperationTypeID:
      if (value.size() >= 5)
      {
        info->supplierId = QString("0x%1")
                           .arg(readLittleEndian16(value.mid(0, 2)),
                                4, 16, QChar('0')).toUpper();
        info->functionId = QString("0x%1")
                           .arg(readLittleEndian16(value.mid(2, 2)),
                                4, 16, QChar('0')).toUpper();
        info->variantId = QString("0x%1")
                          .arg(static_cast<int>(
                                 static_cast<quint8>(value.at(4))),
                               2, 16, QChar('0')).toUpper();
      }
      break;
    case EOperationTypeCalibration:
    case EOperationTypeRaw:
      break;
  }
}

bool encodeLinServiceValue(const LinServiceLayout &service,
                           const SlaveConfigInfo &info,
                           QByteArray *value,
                           QString *errorMessage)
{
  if (value == 0)
    return false;

  value->clear();
  switch (service.operation)
  {
    case EOperationTypeSingleAddr: appendLittleEndian16(value, info.SA); break;
    case EOperationTypeGroupAddr:  appendLittleEndian16(value, info.GA); break;
    case EOperationTypePlatform:   appendLittleEndian16(value, info.platform); break;
    case EOperationTypeIntensity:  appendLittleEndian16(value, info.intensity); break;
    case EOperationTypeRValue:     *value = encodeCalibration(info.r); break;
    case EOperationTypeGValue:     *value = encodeCalibration(info.g); break;
    case EOperationTypeBValue:     *value = encodeCalibration(info.b); break;
    case EOperationTypePartNO:
      *value = info.PN.left(service.dataLength).toLatin1();
      value->append(QByteArray(service.dataLength - value->size(),
                               static_cast<char>(' ')));
      break;
    case EOperationTypeSerialNO:
      *value = info.SN.left(service.dataLength).toLatin1();
      value->append(QByteArray(service.dataLength - value->size(),
                               static_cast<char>(0)));
      break;
    case EOperationTypeRaw:
      if (!info.rawDiagnosticValues.contains(service.serviceId))
      {
        if (errorMessage != 0)
          *errorMessage = QString("Raw value for service 0x%1 is not set")
                          .arg(static_cast<int>(service.serviceId),
                               4, 16, QChar('0'));
        return false;
      }
      *value = info.rawDiagnosticValues.value(service.serviceId);
      break;
    default:
      if (errorMessage != 0)
        *errorMessage = QString("Service '%1' has no configuration encoder")
                        .arg(service.name == 0
                             ? QString("<unnamed>")
                             : QString::fromLatin1(service.name));
      return false;
  }

  if (value->size() < service.dataLength)
  {
    if (errorMessage != 0)
      *errorMessage = QString("Encoded value for '%1' is too short")
                      .arg(service.name == 0
                           ? QString("<unnamed>")
                           : QString::fromLatin1(service.name));
    return false;
  }

  value->truncate(service.dataLength);
  return true;
}

bool isLinServiceApplicable(const LinServiceLayout &service,
                            ENodeType nodeType)
{
  if ((nodeType == ENodeTypeWhite) &&
      ((service.operation == EOperationTypeGValue) ||
       (service.operation == EOperationTypeBValue)))
    return false;

  return true;
}
