#include "Wdg.h"

#include "pal_wdg.h"

void Wdg_Init(void)
{
    wdg_init(1000U);
    wdg_enable(true);
}

void Wdg_Feed(void)
{
    wdg_reload();
}
