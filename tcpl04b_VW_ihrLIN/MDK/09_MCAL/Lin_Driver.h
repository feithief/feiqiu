#ifndef LIN_DRIVER_H
#define LIN_DRIVER_H

#include "ComStack_Types.h"

typedef uint32_t Lin_DriverEventType;

#define LIN_DRV_EVENT_BREAK          ((Lin_DriverEventType)1UL << 0U)
#define LIN_DRV_EVENT_SYNC           ((Lin_DriverEventType)1UL << 1U)
#define LIN_DRV_EVENT_PID            ((Lin_DriverEventType)1UL << 2U)
#define LIN_DRV_EVENT_RX_BYTE        ((Lin_DriverEventType)1UL << 3U)
#define LIN_DRV_EVENT_TX_DONE        ((Lin_DriverEventType)1UL << 4U)
#define LIN_DRV_EVENT_CHECKSUM_ERR   ((Lin_DriverEventType)1UL << 5U)
#define LIN_DRV_EVENT_FRAMING_ERR    ((Lin_DriverEventType)1UL << 6U)
#define LIN_DRV_EVENT_READBACK_ERR   ((Lin_DriverEventType)1UL << 7U)
#define LIN_DRV_EVENT_PID_ERR        ((Lin_DriverEventType)1UL << 8U)
#define LIN_DRV_EVENT_SYNC_ERR       ((Lin_DriverEventType)1UL << 9U)
#define LIN_DRV_EVENT_FIFO_ERR       ((Lin_DriverEventType)1UL << 10U)
#define LIN_DRV_EVENT_RX_DONE        ((Lin_DriverEventType)1UL << 11U)

typedef void (*Lin_DriverNotificationType)(uint8_t Channel,
                                            Lin_DriverEventType Events);

Std_ReturnType Lin_Driver_Init(uint8_t Channel, uint32_t Baudrate,
                              Lin_DriverNotificationType Notification);
void Lin_Driver_DeInit(uint8_t Channel);
uint8_t Lin_Driver_ReadPid(uint8_t Channel);
uint8_t Lin_Driver_ReadByte(uint8_t Channel);
Std_ReturnType Lin_Driver_StartRx(uint8_t Channel, uint8_t ProtectedId,
                                 Lin_FrameCsModelType Checksum,
                                 uint8_t Length);
Std_ReturnType Lin_Driver_StartTx(uint8_t Channel, uint8_t ProtectedId,
                                 Lin_FrameCsModelType Checksum,
                                 const uint8_t *Data, uint8_t Length);
void Lin_Driver_Abort(uint8_t Channel);
void Lin_Driver_CheckAutoBaud(uint8_t Channel);
void Lin_Driver_EnterCritical(void);
void Lin_Driver_ExitCritical(void);

#endif
