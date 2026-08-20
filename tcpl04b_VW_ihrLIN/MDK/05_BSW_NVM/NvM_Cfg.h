#ifndef NVM_CFG_H
#define NVM_CFG_H

#include "NvM.h"
#include "Rte_Types.h"

#define NVM_WRITE_SYNC_MODE    (1U)

typedef bool (*NvM_ValidateBlockFctType)(const void *DataPtr);
typedef void (*NvM_MigrateBlockFctType)(void *DestinationPtr,
                                        const void *LegacyPtr);

typedef struct
{
    NvM_BlockIdType BlockId;
    uint16_t Length;
    uint16_t LegacyLength;
    void *RamBlockDataAddress;
    const void *RomBlockDataAddress;
    NvM_ValidateBlockFctType Validate;
    NvM_ValidateBlockFctType ValidateLegacy;
    NvM_MigrateBlockFctType MigrateLegacy;
} NvM_BlockDescriptorType;

extern const NvM_BlockDescriptorType NvM_BlockDescriptorTable[];
extern const uint16_t NvM_BlockDescriptorCount;

const Rte_SwscConfigType *NvM_CfgGetSwscConfig(void);

#endif
