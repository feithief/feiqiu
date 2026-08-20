#ifndef RTE_DEM_H
#define RTE_DEM_H

#include "ComStack_Types.h"

#define RTE_DEM_EVENT_BATTERY_VOLTAGE   (1U)

Std_ReturnType Rte_Call_Dem_SetEventFailed(uint16_t EventId,
                                           bool EventFailed);
Std_ReturnType Rte_Call_Dem_GetEventFailed(uint16_t EventId,
                                           bool *EventFailedPtr);

#endif
