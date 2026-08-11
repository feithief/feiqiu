#ifndef LINLAYOUT_H
#define LINLAYOUT_H

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QtGlobal>

#include "lin_types.h"

enum LinDiagnosticProtocol
{
  ELinDiagnosticReadByIdentifier = 0,
  ELinDiagnosticProductIdentification
};

enum LinDiagnosticNadAccess
{
  ELinDiagnosticNadNormal      = 0x01,
  ELinDiagnosticNadCalibration = 0x02
};

struct LinSignalLayout
{
  LinLogicalSignal signal;
  const char *name;
  quint16 startBit;
  quint8 bitLength;
  quint32 defaultValue;
};

struct LinFrameLayout
{
  const char *name;
  /* Raw 6-bit frame ID (0x00..0x3F); transport code adds parity bits. */
  quint8 frameId;
  quint8 length;
  LinChecksumMode checksumMode;
  /* LDF initial bytes, including reserved-bit values. */
  quint8 defaultPayload[8];
  const LinSignalLayout *signalLayouts;
  int signalCount;
};

struct LinStatusFieldLayout
{
  LinStatusField field;
  const char *name;
  quint16 startBit;
  /* Raw fields may be 1..32 bits and may use non-Boolean state encodings. */
  quint8 bitLength;
  quint32 normalValue;
  quint32 errorValue;
  /* Some LDF states (for example thermal derating) treat every non-normal
   * raw value as an error instead of naming one exact error code. */
  bool anyNonNormalIsError;
};

struct LinStatusLayout
{
  const char *name;
  const LinStatusFieldLayout *fields;
  int fieldCount;
};

struct LinNodeLayout
{
  int uiSlot;
  /* Diagnostic/configured NAD; never substitute a status frame ID here. */
  quint8 diagnosticNad;
  /* Profile-specific target mask or address value used by the control frame. */
  quint16 controlAddressMask;
  /* Raw 6-bit frame ID (0x00..0x3F); transport code adds parity bits. */
  quint8 statusFrameId;
  quint8 statusLength;
  int statusLayoutIndex;
  LinChecksumMode checksumMode;
  ENodeType nodeType;
  const char *name;
};

struct LinServiceLayout
{
  OperationType operation;
  const char *name;
  quint16 serviceId;
  quint8 dataLength;
  quint8 nadAccess;
  bool readable;
  bool writable;
  bool readOnConfiguration;
  /* This slave deliberately sends no positive response after successful 0x2E. */
  bool expectPositiveWriteResponse;
  LinDiagnosticProtocol protocol;
};

struct LinSecurityAccessLayout
{
  bool enabled;
  quint8 requestSeedSubFunction;
  quint8 sendKeySubFunction;
  quint32 keyAddend;
  /* Number of little-endian key bytes carried after the sub-function. */
  quint8 keyLength;
};

struct LinPredefinedColor
{
  quint8 red;
  quint8 green;
  quint8 blue;
};

/* One quick-action value written directly to an exact LDF signal.  A preset
 * may span any number of selected master-published frames. */
struct LinSignalPresetAssignment
{
  const char *signalName;
  int frameIndex;
  quint16 startBit;
  quint8 bitLength;
  quint32 value;
};

struct LinSignalPreset
{
  const char *name;
  const LinSignalPresetAssignment *assignments;
  int assignmentCount;
};

enum LinScheduleSlotType
{
  ELinScheduleTransmitFrame = 0,
  ELinSchedulePollNode
};

struct LinScheduleSlot
{
  LinScheduleSlotType type;
  int tableIndex;
  int delayAfterMs;
};

struct LinLayout
{
  const char *layoutName;
  const char *deviceName;
  quint32 baudRate;
  int serialIoTimeoutMs;
  LinColorModel colorModel;
  LinAddressingModel addressingModel;
  LinDimmingModel dimmingModel;
  LinDiagnosticModel diagnosticModel;
  quint32 intensityMaximum;
  quint32 dimmingMaximum;
  const LinPredefinedColor *predefinedColors;
  int predefinedColorCount;
  const char *signalPresetGroupName;
  const LinSignalPreset *signalPresets;
  int signalPresetCount;

  const LinFrameLayout *publishedFrames;
  int publishedFrameCount;
  /* Published frame edited by the primary lighting-control UI. */
  int primaryControlFrameIndex;

  const LinStatusLayout *statusLayouts;
  int statusLayoutCount;
  const LinNodeLayout *nodes;
  int nodeCount;

  quint8 diagnosticRequestFrameId;
  quint8 diagnosticResponseFrameId;
  quint8 temporaryNad;
  const LinServiceLayout *services;
  int serviceCount;
  const OperationType *bulkWriteSequence;
  int bulkWriteCount;
  LinSecurityAccessLayout securityAccess;

  const LinScheduleSlot *schedule;
  int scheduleSlotCount;
  /* P2: end of diagnostic request to the response-frame header. */
  int diagnosticRequestToResponseDelayMs;
  /* ST: separation between consecutive diagnostic transport frames. */
  int diagnosticInterFrameDelayMs;
  /* Header-to-driver-read guard used by unconditional and diagnostic frames. */
  int responseDelayMs;
  /* SecurityAccess may schedule a flash save before writes can begin. */
  int postWriteSettleMs;
  /* Wait once after the complete bulk write, before unified read-back. */
  int bulkWriteReadBackDelayMs;
  int maximumDiagnosticQueueDepth;
};

/* Active profile entry point.  LDF data lives in generated/ and is produced
 * by tools/ldf_profile_gen.py; runtime, scheduler and UI code stay unchanged. */
const LinLayout &defaultLinLayout();
bool validateLinLayout(const LinLayout &layout, QStringList *errors);
bool supportsDirectRgb(const LinLayout &layout);
bool supportsPredefinedColors(const LinLayout &layout);
/* UI capability checks must use the configured primary control frame. */
const LinFrameLayout *primaryControlFrame(const LinLayout &layout);
const LinSignalLayout *findPrimaryControlSignal(
  const LinLayout &layout,
  LinLogicalSignal logicalSignal);
bool primaryControlHasSignal(const LinLayout &layout,
                             LinLogicalSignal logicalSignal);

const LinNodeLayout *findLinNode(const LinLayout &layout, quint8 nad);
const LinNodeLayout *findLinNodeByControlMask(const LinLayout &layout,
                                              quint16 controlAddressMask);
const LinServiceLayout *findLinService(const LinLayout &layout,
                                       OperationType operation);
const LinServiceLayout *findLinServiceById(const LinLayout &layout,
                                           quint16 serviceId);

BCMSignal createDefaultBCMSignal(const LinLayout &layout);
bool encodePublishedFrame(const LinFrameLayout &frameLayout,
                          const BCMSignal &values,
                          QByteArray *frame,
                          QString *errorMessage = 0);
bool applySignalPreset(const LinLayout &layout,
                       int presetIndex,
                       BCMSignal *values,
                       QString *errorMessage = 0);
SlaveStatus decodeStatusFrame(const LinLayout &layout,
                              const LinNodeLayout &node,
                              const QByteArray &frame,
                              bool online);
void applyLinServiceValue(const LinServiceLayout &service,
                          const QByteArray &value,
                          SlaveConfigInfo *info);
bool encodeLinServiceValue(const LinServiceLayout &service,
                           const SlaveConfigInfo &info,
                           QByteArray *value,
                           QString *errorMessage = 0);
bool isLinServiceApplicable(const LinServiceLayout &service,
                            ENodeType nodeType);

#endif // LINLAYOUT_H
