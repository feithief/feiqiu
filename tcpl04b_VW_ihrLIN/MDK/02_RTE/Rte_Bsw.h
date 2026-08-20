#ifndef RTE_BSW_H
#define RTE_BSW_H

#include "ComStack_Types.h"

void Rte_Call_Diagnostic_SetNad(uint8_t Nad);
bool Rte_Call_Diagnostic_TakeNadChange(uint8_t *NadPtr);
uint8_t Rte_Call_Diagnostic_GetNad(void);
void Rte_Call_Lin_GoToSleep(void);
Std_ReturnType Rte_Call_Lin_SetApplicationAddress(uint8_t Address);

#endif
