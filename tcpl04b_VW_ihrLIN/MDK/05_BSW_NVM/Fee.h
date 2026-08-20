#ifndef FEE_H
#define FEE_H

#include "NvM.h"

void Fee_Init(void);
Std_ReturnType Fee_Read(NvM_BlockIdType BlockId, void *DataPtr,
                        uint16_t Length);
Std_ReturnType Fee_Write(NvM_BlockIdType BlockId, const void *DataPtr,
                         uint16_t Length);

#endif
