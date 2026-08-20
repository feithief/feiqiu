#ifndef RTE_NVM_H
#define RTE_NVM_H

#include "ComStack_Types.h"
#include "Rte_Types.h"

Std_ReturnType Rte_Call_NvM_ReadSwscConfig(Rte_SwscConfigType *DataPtr);
Std_ReturnType Rte_Call_NvM_WriteSwscConfig(
    const Rte_SwscConfigType *DataPtr);
#endif
