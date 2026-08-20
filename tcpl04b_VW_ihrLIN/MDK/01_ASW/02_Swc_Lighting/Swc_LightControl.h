#ifndef SWSC_SYSTEM_CONTROL_H
#define SWSC_SYSTEM_CONTROL_H

#include "Algorithm.h"

#define SWSC_MODE_NORMAL              (0U)
#define SWSC_MODE_CRASH               (1U)
#define SWSC_MODE_SHOWROOM            (2U)
#define SWSC_MODE_ACTUATOR_TEST       (3U)

void Swc_LightControl_(SColorParams *inputColor);
void Swc_LightControl_Sleep(void);

#endif
