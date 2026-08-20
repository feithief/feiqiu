#ifndef TCPL04B_MCU_H
#define TCPL04B_MCU_H

#include <stdint.h>

void Mcu_InterruptDisable(void);
void Mcu_InterruptEnable(void);
void Mcu_RemapToApplication(uint32_t VectorWordAddress);
uint32_t Mcu_GetMilliseconds(void);

#endif
