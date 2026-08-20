#include "Dem.h"

#include "Dem_Cfg.h"

typedef struct
{
    uint8_t FailureCounter;
    uint8_t HealingCounter;
    bool Failed;
} Dem_EventRuntimeType;

static Dem_EventRuntimeType s_eventRuntime[3];

static int16_t Dem_FindEvent(Dem_EventIdType EventId)
{
    uint16_t index;
    for (index = 0U; index < Dem_EventConfigCount; index++)
    {
        if (Dem_EventConfig[index].EventId == EventId)
        {
            return (int16_t)index;
        }
    }
    return -1;
}

void Dem_Init(void)
{
    uint16_t index;
    for (index = 0U; index < Dem_EventConfigCount; index++)
    {
        s_eventRuntime[index].FailureCounter = 0U;
        s_eventRuntime[index].HealingCounter = 0U;
        s_eventRuntime[index].Failed = false;
    }
}

void Dem_MainFunction(void)
{
    /* Debouncing is applied synchronously by Dem_SetEventStatus. */
}

Std_ReturnType Dem_SetEventStatus(Dem_EventIdType EventId,
                                  Dem_EventStatusType EventStatus)
{
    int16_t index = Dem_FindEvent(EventId);
    Dem_EventRuntimeType *runtime;
    const Dem_EventConfigType *config;
    if (index < 0)
    {
        return E_NOT_OK;
    }
    runtime = &s_eventRuntime[(uint16_t)index];
    config = &Dem_EventConfig[(uint16_t)index];
    if (EventStatus == DEM_EVENT_STATUS_FAILED)
    {
        runtime->HealingCounter = 0U;
        if (runtime->FailureCounter < config->FailureThreshold)
        {
            runtime->FailureCounter++;
        }
        if (runtime->FailureCounter >= config->FailureThreshold)
        {
            runtime->Failed = true;
        }
    }
    else
    {
        runtime->FailureCounter = 0U;
        if (runtime->HealingCounter < config->HealingThreshold)
        {
            runtime->HealingCounter++;
        }
        if (runtime->HealingCounter >= config->HealingThreshold)
        {
            runtime->Failed = false;
        }
    }
    return E_OK;
}

Std_ReturnType Dem_GetEventFailed(Dem_EventIdType EventId,
                                  bool *EventFailedPtr)
{
    int16_t index = Dem_FindEvent(EventId);
    if ((index < 0) || (EventFailedPtr == NULL))
    {
        return E_NOT_OK;
    }
    *EventFailedPtr = s_eventRuntime[(uint16_t)index].Failed;
    return E_OK;
}
