#include "Dem_Cfg.h"

const Dem_EventConfigType Dem_EventConfig[] =
{
    { DEM_EVENT_ID_BATTERY_VOLTAGE, 1U, 1U },
    { DEM_EVENT_ID_LED_OUTPUT,      1U, 1U },
    { DEM_EVENT_ID_NVM_STORAGE,     1U, 1U }
};

const uint16_t Dem_EventConfigCount =
    (uint16_t)(sizeof(Dem_EventConfig) / sizeof(Dem_EventConfig[0]));
