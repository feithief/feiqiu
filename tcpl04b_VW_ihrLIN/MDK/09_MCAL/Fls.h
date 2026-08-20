#ifndef FLS_H
#define FLS_H

#include "ComStack_Types.h"

void Fls_Init(void);
Std_ReturnType Fls_Read(uint32_t Address, void *DataPtr, uint16_t Length);
Std_ReturnType Fls_Write(uint32_t Address, const void *DataPtr,
                         uint16_t Length);

#endif
