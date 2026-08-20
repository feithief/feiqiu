#include "IoHwAb_Time.h"
#include "pal_systick.h"

void IoHwAb_Time_Init(void)
{
}

uint16_t IoHwAb_Time_Get(void)
{
    return (uint16_t)systick_count_get();
}

bool IoHwAb_Time_IsAfter(uint16_t Time, uint16_t Reference)
{
    return ((int16_t)((int16_t)Reference - (int16_t)Time) <= 0);
}
