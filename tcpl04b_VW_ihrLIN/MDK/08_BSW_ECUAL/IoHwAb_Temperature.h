#ifndef SWSC_MODULE_TEMPERATURE_H
#define SWSC_MODULE_TEMPERATURE_H

#include <stdint.h>
#include "prj_color_cfg.h"

#define DTEMP_MULTIPLIED_RATIO          SWSC_TEMPERATURE_SCALE

void IoHwAb_Temperature_Init(void);
void IoHwAb_Temperature_Update(void);
int16_t IoHwAb_Temperature_GetLed(void);
int16_t IoHwAb_Temperature_GetMcu(void);

#endif
