#ifndef LIN_TYPES_H
#define LIN_TYPES_H

#include <QByteArray>
#include <QMap>
#include <QMetaType>
#include <QString>
#include <QtGlobal>

/*
 * Application-facing LIN data types.
 *
 * These structures describe logical values used by the UI.  Their physical
 * byte/bit positions belong in the generated LIN profile, not in UI or
 * scheduler code.
 */

enum LinLogicalSignal
{
  ELinSignalTargetMask = 0,
  ELinSignalDirectRgbEnable,
  ELinSignalLedEnable,
  ELinSignalFadingEnable,
  ELinSignalFadingTime,
  ELinSignalIntensity,
  ELinSignalRedOrPredefinedColor,
  ELinSignalGreen,
  ELinSignalBlue,
  ELinSignalGroupAddressing,
  ELinSignalIntensityNormalization,
  ELinSignalDimmingTimeRelative,
  ELinSignalDimmingTimeBase250ms,
  ELinSignalSpecialFunction,
  ELinSignalCommandValidity
};

enum LinColorModel
{
  ELinColorModelPredefinedOnly = 0,
  ELinColorModelDirectRgbOnly,
  ELinColorModelSelectable
};

enum LinAddressingModel
{
  ELinAddressingModelFixedTargetMask = 0,
  ELinAddressingModelAddressValueAndGroupBit
};

enum LinDimmingModel
{
  ELinDimmingModelFadeEnableAndTime = 0,
  ELinDimmingModelRampWithTimeBase,
  /* One raw LDF ramp-time signal without extra flags. */
  ELinDimmingModelRampTimeOnly
};

enum LinDiagnosticModel
{
  ELinDiagnosticModelNone = 0,
  ELinDiagnosticModelLin20NodeConfiguration,
  ELinDiagnosticModelCustomDid
};

enum LinChecksumMode
{
  ELinChecksumClassic = 0,
  ELinChecksumEnhanced
};

enum LinStatusField
{
  ELinStatusRedOutputError = 0,
  ELinStatusGreenOutputError,
  ELinStatusBlueOutputError,
  ELinStatusTemperatureError,
  ELinStatusVoltageError,
  ELinStatusCommunicationError
};

enum SlaveErrorFlag
{
  ESlaveErrorFlagNormal = 0,
  ESlaveErrorFlagError = 1,
  ESlaveErrorFlagUnknown = 2
};

struct BCMSignal
{
  /* Target mask or address value, according to LinAddressingModel. */
  quint16 targetMask;
  /* Selects the color representation when LinColorModel is Selectable. */
  bool directRgbEnabled;
  bool ledEnabled;
  bool fadingEnabled;
  quint8 fadingTime;
  quint8 intensity;
  quint8 redOrPredefinedColor;
  quint8 green;
  quint8 blue;
  /* true selects group interpretation for address-value based layouts. */
  bool groupAddressing;
  bool intensityNormalization;
  bool dimmingTimeRelative;
  bool dimmingTimeBase250ms;
  quint8 specialFunction;
  bool commandValidity;
};

struct SlaveStatus
{
  quint8 slaveNAD;
  SlaveErrorFlag ROutput_Err;
  SlaveErrorFlag GOutput_Err;
  SlaveErrorFlag BOutput_Err;
  SlaveErrorFlag Temp_Err;
  SlaveErrorFlag Voltage_Err;
  SlaveErrorFlag Lin_Err;
  bool isOnLine;
};

struct CalibrationInfo
{
  double x;
  double y;
  double Y;
};

enum ENodeType
{
  ENodeTypeRGB = 0,
  ENodeTypeWhite = 1
};

struct SlaveConfigInfo
{
  quint8 slaveNode;
  quint16 SA;
  quint16 GA;
  quint16 platform;
  quint16 intensity;
  QString PN;
  QString SN;
  QString softwareVersion;
  QString hardwareVersion;
  QString supplierId;
  QString functionId;
  QString variantId;
  bool locked;
  CalibrationInfo r;
  CalibrationInfo g;
  CalibrationInfo b;
  ENodeType nodeType;
  /* Values for profile DIDs that have no dedicated UI binding yet. */
  QMap<quint16, QByteArray> rawDiagnosticValues;
};

enum OperationType
{
  EOperationTypeLock = 0,
  EOperationTypeSingleAddr,
  EOperationTypeGroupAddr,
  EOperationTypePlatform,
  EOperationTypeIntensity,
  EOperationTypeRValue,
  EOperationTypeGValue,
  EOperationTypeBValue,
  EOperationTypePartNO,
  EOperationTypeSerialNO,
  EOperationTypeCalibration,
  EOperationTypeSoftwareVer,
  EOperationTypeHardwareVer,
  EOperationTypeID,
  EOperationTypeRaw
};

Q_DECLARE_METATYPE(BCMSignal)
Q_DECLARE_METATYPE(SlaveStatus)
Q_DECLARE_METATYPE(CalibrationInfo)
Q_DECLARE_METATYPE(ENodeType)
Q_DECLARE_METATYPE(SlaveConfigInfo)

#endif // LIN_TYPES_H
