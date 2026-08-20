#include "Rte_NvM.h"

#include "NvM.h"

Std_ReturnType Rte_Call_NvM_ReadSwscConfig(Rte_SwscConfigType *DataPtr)
{
    return NvM_ReadBlock(NVM_BLOCK_SWSC_CONFIG, DataPtr);
}

Std_ReturnType Rte_Call_NvM_WriteSwscConfig(
    const Rte_SwscConfigType *DataPtr)
{
    return NvM_WriteBlock(NVM_BLOCK_SWSC_CONFIG, DataPtr);
}
