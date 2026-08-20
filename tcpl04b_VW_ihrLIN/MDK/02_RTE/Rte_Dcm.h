#ifndef RTE_DCM_H
#define RTE_DCM_H

#include "ComStack_Types.h"

Std_ReturnType Rte_Call_Dcm_ReadData(
    uint16_t Did, uint8_t Nad, uint8_t *DataPtr,
    uint16_t DataCapacity, uint16_t *DataLengthPtr);
Std_ReturnType Rte_Call_Dcm_WriteData(
    uint16_t Did, uint8_t Nad,
    const uint8_t *DataPtr, uint16_t DataLength);
Std_ReturnType Rte_Call_Dcm_SecurityAccess(
    const uint8_t *RequestPtr, uint16_t RequestLength,
    uint8_t *ResponsePtr, uint16_t ResponseCapacity,
    uint16_t *ResponseLengthPtr);

#endif
