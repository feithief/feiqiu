#ifndef GCC_TEST_GCC_STUB_H
#define GCC_TEST_GCC_STUB_H

#include <stdint.h>
#include <stddef.h>

typedef int bool_t;

#ifndef btrue
#define btrue  (1)
#endif

#ifndef bfalse
#define bfalse (0)
#endif

#ifndef TRUE
#define TRUE  (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef PWM_PERIOD
#define PWM_PERIOD (32000)
#endif

#ifndef DTEMP_MULTIPLIED_RATIO
#define DTEMP_MULTIPLIED_RATIO (10)
#endif

typedef struct
{
    uint16_t Utemp;
    uint16_t Vtemp;
    int16_t whitex;
    int16_t whitey;
    uint8_t brightness_factor;
} SFlashContent;

typedef struct
{
    uint8_t BCM_RGB_Sonderfunktion;
} SRgbLinStub;

typedef struct
{
    SRgbLinStub RGB;
} SSysLinStack;

extern SFlashContent savedConfig;
extern SSysLinStack sysLin_Stack;

int16_t moduleTempMcuGet_debug(void);

#endif
