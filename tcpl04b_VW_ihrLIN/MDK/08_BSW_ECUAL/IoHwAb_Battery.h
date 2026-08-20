#ifndef SWSC_MODULE_BATTERY_H
#define SWSC_MODULE_BATTERY_H

#include <stdint.h>

typedef enum
{
    EBatStatusInitial = 0,
    EBatStatusNormal = 1,
    EBatStatusUnderVoltage = 2,
    EBatStatusOverVoltage = 4
} EBatStatus;

void IoHwAb_Battery_Init(void);
EBatStatus IoHwAb_Battery_UpdateStatus(void);
uint32_t IoHwAb_Battery_GetVoltage(void);
uint32_t IoHwAb_Battery_GetVoltage_2(void);

#endif
