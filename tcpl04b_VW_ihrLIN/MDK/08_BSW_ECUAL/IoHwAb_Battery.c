#include "IoHwAb_Battery.h"
#include "IoHwAb_Adc.h"

#define BATTERY_UV_MV                   (6000)
#define BATTERY_OV_MV                   (19000)

static int16_t s_battery_mv;

void IoHwAb_Battery_Init(void)
{
    (void)IoHwAb_Adc_GetBatteryMv(&s_battery_mv);
}

EBatStatus IoHwAb_Battery_UpdateStatus(void)
{
    if (!IoHwAb_Adc_GetBatteryMv(&s_battery_mv))
    {
        return EBatStatusInitial;
    }
    if (s_battery_mv < BATTERY_UV_MV)
    {
        return EBatStatusUnderVoltage;
    }
    if (s_battery_mv > BATTERY_OV_MV)
    {
        return EBatStatusOverVoltage;
    }
    return EBatStatusNormal;
}

uint32_t IoHwAb_Battery_GetVoltage(void)
{
    return (s_battery_mv > 0) ? (uint32_t)s_battery_mv : 0U;
}

uint32_t IoHwAb_Battery_GetVoltage_2(void)
{
    return IoHwAb_Battery_GetVoltage();
}
