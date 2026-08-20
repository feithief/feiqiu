#ifndef SWSC_SYSTEM_LIN_COMM_H
#define SWSC_SYSTEM_LIN_COMM_H

#include "Swc_Types.h"

void Swc_Communication_Init(void);
void Swc_Communication_Handler(void);
bool_t Swc_Communication_SignalUpdate(void);
void Swc_Communication_RespUpdate(void);
void Swc_Communication_ChangeNad(uint8_t new_nad);
void Swc_Communication_Sleep(void);
uint8_t GetBrightnessDiffForSpeedBasedDimming(void);

#endif
