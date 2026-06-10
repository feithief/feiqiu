#ifndef _HOST_SYSTEMLINCOMM_H_
#define _HOST_SYSTEMLINCOMM_H_

#include <stdint.h>

typedef struct HostRgbLinState
{
    uint8_t BCM_RGB_Sonderfunktion;
} SHostRgbLinState;

typedef struct HostSysLinStack
{
    SHostRgbLinState RGB;
} SHostSysLinStack;

extern SHostSysLinStack sysLin_Stack;

#endif