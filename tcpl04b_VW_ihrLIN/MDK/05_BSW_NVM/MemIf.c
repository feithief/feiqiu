#include "MemIf.h"

#include "Fee.h"

void MemIf_Init(void)
{
    Fee_Init();
}

Std_ReturnType MemIf_Read(NvM_BlockIdType BlockId, void *DataPtr,
                          uint16_t Length)
{
    return Fee_Read(BlockId, DataPtr, Length);
}

Std_ReturnType MemIf_Write(NvM_BlockIdType BlockId, const void *DataPtr,
                           uint16_t Length)
{
    return Fee_Write(BlockId, DataPtr, Length);
}
