#include "Rte_Dem.h"

#include "Dem.h"

Std_ReturnType Rte_Call_Dem_SetEventFailed(uint16_t EventId,
                                           bool EventFailed)
{
    return Dem_SetEventStatus((Dem_EventIdType)EventId,
                              EventFailed ? DEM_EVENT_STATUS_FAILED :
                                            DEM_EVENT_STATUS_PASSED);
}

Std_ReturnType Rte_Call_Dem_GetEventFailed(uint16_t EventId,
                                           bool *EventFailedPtr)
{
    return Dem_GetEventFailed((Dem_EventIdType)EventId, EventFailedPtr);
}
