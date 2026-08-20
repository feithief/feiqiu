#ifndef COM_CFG_H
#define COM_CFG_H

#include "LinIf.h"

#define LIN_CONFIG_BAUDRATE           (19200UL)
#define LIN_NUMBER_OF_FRAMES          (2U)
#define LINIF_FRAME_COUNT              (4U)
#define LINIF_APPLICATION_FRAME_COUNT  (2U)
#define COM_IPDU_COUNT                 (2U)
#define LINIF_PDU_LIGHT_COMMAND        ((PduIdType)0U)
#define LINIF_PDU_LIGHT_STATUS         ((PduIdType)1U)
#define LINIF_PDU_MASTER_REQUEST       ((PduIdType)2U)
#define LINIF_PDU_SLAVE_RESPONSE       ((PduIdType)3U)
#define LINIF_RESPONSE_ERROR_FRAME_INDEX (1U)
#define COM_INVALID_PDU_ID             ((PduIdType)0xFFFFU)
#define COM_INVALID_SIGNAL_ID          (0xFFFFU)

typedef enum
{
    COM_SIG_RX_SLAVE_ADDRESS = 0,
    COM_SIG_RX_GROUP_ADDRESSING,
    COM_SIG_RX_RED,
    COM_SIG_RX_GREEN,
    COM_SIG_RX_BLUE,
    COM_SIG_RX_INTENSITY,
    COM_SIG_RX_NORMALIZED_INTENSITY,
    COM_SIG_RX_DIM_RAMP,
    COM_SIG_RX_WORK_MODE,
    COM_SIG_RX_RELATIVE_DIM_TIME,
    COM_SIG_RX_VALID,
    COM_SIG_RX_TIME_BASE_250MS,
    COM_SIG_TX_RESPONSE_ERROR,
    COM_SIG_TX_RED_SHORT,
    COM_SIG_TX_GREEN_SHORT,
    COM_SIG_TX_BLUE_SHORT,
    COM_SIG_TX_RED_OPEN,
    COM_SIG_TX_GREEN_OPEN,
    COM_SIG_TX_BLUE_OPEN,
    COM_SIG_TX_TEMP_STATE,
    COM_SIG_TX_RAM_ERROR,
    COM_SIG_TX_ROM_ERROR,
    COM_SIG_TX_NVM_ERROR,
    COM_SIG_COUNT
} Com_LinSignalIdType;

typedef enum
{
    COM_SIGNAL_GROUP_LIGHT_COMMAND = 0,
    COM_SIGNAL_GROUP_COUNT
} Com_LinSignalGroupIdType;

#define COM_SIGNAL_GROUP_STORAGE_COUNT \
    ((COM_SIGNAL_GROUP_COUNT > 0U) ? COM_SIGNAL_GROUP_COUNT : 1U)
#define COM_RESPONSE_ERROR_SIGNAL_ID    ((uint16_t)COM_SIG_TX_RESPONSE_ERROR)

typedef enum
{
    COM_SIGNAL_DIRECTION_RX = 0,
    COM_SIGNAL_DIRECTION_TX
} Com_SignalDirectionType;

typedef struct
{
    uint8_t IpduIndex;
    uint8_t BitPosition;
    uint8_t BitLength;
    Com_SignalDirectionType Direction;
} Com_SignalConfigType;

typedef struct
{
    PduIdType PduId;
    uint8_t Length;
    Com_SignalDirectionType Direction;
    uint8_t InitData[8];
} Com_IpduConfigType;

extern const Com_SignalConfigType Com_SignalConfig[COM_SIG_COUNT];
extern const Com_IpduConfigType Com_IpduConfig[COM_IPDU_COUNT];
extern const uint8_t
    Com_SignalGroupIpduIndex[COM_SIGNAL_GROUP_STORAGE_COUNT];
extern LinIf_FrameConfigType LinIf_FrameConfig[LINIF_FRAME_COUNT];
#endif
