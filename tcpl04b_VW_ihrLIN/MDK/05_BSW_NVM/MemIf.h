#ifndef MEMIF_H
#define MEMIF_H

#include "NvM.h"

void MemIf_Init(void);
Std_ReturnType MemIf_Read(NvM_BlockIdType BlockId, void *DataPtr,
                          uint16_t Length);
Std_ReturnType MemIf_Write(NvM_BlockIdType BlockId, const void *DataPtr,
                           uint16_t Length);

#endif
