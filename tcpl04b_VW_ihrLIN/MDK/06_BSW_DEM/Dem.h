#ifndef DEM_H
#define DEM_H

#include "ComStack_Types.h"

typedef uint16_t Dem_EventIdType;

typedef enum
{
    DEM_EVENT_STATUS_PASSED = 0,
    DEM_EVENT_STATUS_FAILED
} Dem_EventStatusType;

void Dem_Init(void);
void Dem_MainFunction(void);
Std_ReturnType Dem_SetEventStatus(Dem_EventIdType EventId,
                                  Dem_EventStatusType EventStatus);
Std_ReturnType Dem_GetEventFailed(Dem_EventIdType EventId,
                                  bool *EventFailedPtr);

#endif
