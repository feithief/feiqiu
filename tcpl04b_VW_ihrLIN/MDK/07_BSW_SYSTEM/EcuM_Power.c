#include "ModulePower.h"

void modulePowerInit(void)
{
}

void sleepEnSet(void)
{
}

void modulePowerSleep(void)
{
    /* Logical sleep is preserved in ASW.  TCPL04B physical sleep/wakeup is
     * deliberately left at the MCAL boundary until a board wake source is
     * configured and qualified. */
}
