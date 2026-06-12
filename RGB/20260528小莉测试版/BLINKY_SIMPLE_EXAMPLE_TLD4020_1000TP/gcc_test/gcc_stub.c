#include "include/gcc_stub.h"

SFlashContent savedConfig =
{
    .Utemp = 80u,
    .Vtemp = 188u,
    .whitex = 3333,
    .whitey = 3491,
    .brightness_factor = 100u
};

SSysLinStack sysLin_Stack =
{
    .RGB = { .BCM_RGB_Sonderfunktion = 0u }
};

int16_t moduleTempMcuGet_debug(void)
{
    return 1000;
}
