#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t Std_ReturnType;
typedef uint8_t NetworkHandleType;
typedef uint16_t PduIdType;
typedef uint16_t PduLengthType;

#ifndef E_OK
#define E_OK       ((Std_ReturnType)0U)
#endif
#ifndef E_NOT_OK
#define E_NOT_OK   ((Std_ReturnType)1U)
#endif

typedef struct
{
    uint8_t *SduDataPtr;
    uint16_t SduLength;
    uint8_t *MetaDataPtr;
} PduInfoType;

typedef enum
{
    BUFREQ_OK = 0,
    BUFREQ_E_NOT_OK,
    BUFREQ_E_BUSY,
    BUFREQ_E_OVFL
} BufReq_ReturnType;

typedef enum
{
    LIN_FRAMERESPONSE_IGNORE = 0,
    LIN_FRAMERESPONSE_RX,
    LIN_FRAMERESPONSE_TX
} Lin_FrameResponseType;

typedef enum
{
    LIN_CLASSIC_CS = 0,
    LIN_ENHANCED_CS
} Lin_FrameCsModelType;

typedef struct
{
    uint8_t Pid;
    Lin_FrameResponseType Drc;
    Lin_FrameCsModelType Cs;
    uint8_t Dl;
    uint8_t *SduPtr;
} Lin_PduType;

typedef enum
{
    LIN_SLAVE_ERR_NONE = 0,
    LIN_SLAVE_ERR_PID_PARITY,
    LIN_SLAVE_ERR_CHECKSUM,
    LIN_SLAVE_ERR_FRAMING,
    LIN_SLAVE_ERR_READBACK,
    LIN_SLAVE_ERR_TIMEOUT,
    LIN_SLAVE_ERR_DRIVER
} Lin_SlaveErrorType;

#endif
