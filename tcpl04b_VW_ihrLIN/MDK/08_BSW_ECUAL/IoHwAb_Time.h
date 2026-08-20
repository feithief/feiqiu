#ifndef SWSC_MODULE_CLOCK_H
#define SWSC_MODULE_CLOCK_H

#include <stdint.h>
#include <stdbool.h>

void IoHwAb_Time_Init(void);
uint16_t IoHwAb_Time_Get(void);
bool IoHwAb_Time_IsAfter(uint16_t Time, uint16_t Reference);

#endif
