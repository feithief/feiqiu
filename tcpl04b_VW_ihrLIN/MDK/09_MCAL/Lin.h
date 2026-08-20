#ifndef LIN_H
#define LIN_H

#include "ComStack_Types.h"

typedef struct
{
    uint32_t Baudrate;
    uint16_t FrameTimeoutMs;
    uint16_t IdleTimeoutMs;
} Lin_ConfigType;

#define LIN_CHANNEL_0 (0U)

extern const Lin_ConfigType Lin_Config;

void Lin_Init(const Lin_ConfigType *Config);
void Lin_DeInit(void);
void Lin_MainFunction(void);
void Lin_GoToSleepInternal(uint8_t Channel);
void Lin_WakeupInternal(uint8_t Channel);

#endif
