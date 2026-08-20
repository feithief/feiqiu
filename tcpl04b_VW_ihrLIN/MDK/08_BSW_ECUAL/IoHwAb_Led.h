#ifndef SWSC_MODULE_LED_H
#define SWSC_MODULE_LED_H

#include <stdbool.h>
#include <stdint.h>
#include "prj_color_cfg.h"

typedef enum
{
    ELedStatusInit = 1,
    ELedStatusNormal = 2,
    ELedStatusOpen = 4,
    ELedStatusShort = 8
} ELedStatus;

typedef enum
{
    ELedChannelRed = 2,
    ELedChannelGreen = 3,
    ELedChannelBlue = 4
} ELedChannel;

void IoHwAb_Led_Init(void);
void IoHwAb_Led_UpdatePWM(uint16_t red, uint16_t green, uint16_t blue);
void IoHwAb_Led_PowerOff(void);
void IoHwAb_Led_UpdateStatus(void);
void IoHwAb_Led_Sleep(void);
ELedStatus IoHwAb_Led_GetStatus(ELedChannel channel);
int16_t IoHwAb_Led_GetVoltage(ELedChannel channel);
uint16_t IoHwAb_Led_GetPWM(ELedChannel channel);
void IoHwAb_Led_ResetDetection(void);
bool IoHwAb_Led_SetCalibrationControl(uint8_t Control);
uint8_t IoHwAb_Led_GetCalibrationControl(void);

#endif
