#ifndef NVM_H
#define NVM_H

#include "ComStack_Types.h"

typedef uint16_t NvM_BlockIdType;

#define NVM_BLOCK_SWSC_CONFIG          ((NvM_BlockIdType)1U)

typedef enum
{
    NVM_REQ_OK = 0,
    NVM_REQ_NOT_OK,
    NVM_REQ_PENDING
} NvM_RequestResultType;

void NvM_Init(void);
void NvM_MainFunction(void);
Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, void *DstPtr);
Std_ReturnType NvM_WriteBlock(NvM_BlockIdType BlockId, const void *SrcPtr);
Std_ReturnType NvM_GetErrorStatus(NvM_BlockIdType BlockId,
                                  NvM_RequestResultType *RequestResultPtr);

#endif
