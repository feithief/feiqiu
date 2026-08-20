#ifndef DEM_CFG_H
#define DEM_CFG_H

#include "Dem.h"

#define DEM_EVENT_ID_BATTERY_VOLTAGE    ((Dem_EventIdType)1U)
#define DEM_EVENT_ID_LED_OUTPUT         ((Dem_EventIdType)2U)
#define DEM_EVENT_ID_NVM_STORAGE        ((Dem_EventIdType)3U)

typedef struct
{
    Dem_EventIdType EventId;
    uint8_t FailureThreshold;
    uint8_t HealingThreshold;
} Dem_EventConfigType;

extern const Dem_EventConfigType Dem_EventConfig[];
extern const uint16_t Dem_EventConfigCount;

#endif
