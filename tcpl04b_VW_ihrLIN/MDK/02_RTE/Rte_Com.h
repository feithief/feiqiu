#ifndef RTE_COM_H
#define RTE_COM_H

#include "ComStack_Types.h"
#include "Rte_Types.h"

typedef struct
{
    uint8_t ResponseError;
    uint8_t RedShort;
    uint8_t GreenShort;
    uint8_t BlueShort;
    uint8_t RedOpen;
    uint8_t GreenOpen;
    uint8_t BlueOpen;
    uint8_t TemperatureState;
    uint8_t RamError;
    uint8_t RomError;
    uint8_t NvMError;
} Rte_LightStatusType;

bool Rte_IsUpdated_LightCommand(void);
Std_ReturnType Rte_Read_LightCommand(Rte_LightCommandType *CommandPtr);
Std_ReturnType Rte_Write_LightStatus(
    const Rte_LightStatusType *StatusPtr);

#endif
