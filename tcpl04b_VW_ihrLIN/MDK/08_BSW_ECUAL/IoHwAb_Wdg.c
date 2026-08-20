#include "IoHwAb_Wdg.h"
#include "pal_wdg.h"

void IoHwAb_Wdg_Init(void)
{
    wdg_init(1000U);
    wdg_enable(true);
}

void IoHwAb_Wdg_Feed(void)
{
    wdg_reload();
}
