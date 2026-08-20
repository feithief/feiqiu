#ifndef SWC_DIAGNOSTIC_H
#define SWC_DIAGNOSTIC_H

#include "ComStack_Types.h"
#include "Rte_Types.h"

void Swc_Diagnostic_Init(void);
void Swc_Diagnostic_MainFunction(void);
Std_ReturnType Swc_Diagnostic_StoreNad(uint8_t Nad);
Std_ReturnType Swc_Diagnostic_ReadData(
    uint16_t Did, uint8_t Nad, uint8_t *DataPtr,
    uint16_t DataCapacity, uint16_t *DataLengthPtr);
Std_ReturnType Swc_Diagnostic_WriteData(
    uint16_t Did, uint8_t Nad, const uint8_t *DataPtr,
    uint16_t DataLength);
Std_ReturnType Swc_Diagnostic_SecurityAccess(
    const uint8_t *RequestPtr, uint16_t RequestLength,
    uint8_t *ResponsePtr, uint16_t ResponseCapacity,
    uint16_t *ResponseLengthPtr);

#endif
