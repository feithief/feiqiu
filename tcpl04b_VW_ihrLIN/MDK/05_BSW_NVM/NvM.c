#include "NvM.h"

#include "MemIf.h"
#include "NvM_Cfg.h"

#define NVM_LEGACY_BUFFER_LENGTH       (128U)

static NvM_RequestResultType s_result;
static NvM_BlockIdType s_pendingBlock;

static const NvM_BlockDescriptorType *NvM_FindBlock(NvM_BlockIdType BlockId)
{
    uint16_t index;
    for (index = 0U; index < NvM_BlockDescriptorCount; index++)
    {
        if (NvM_BlockDescriptorTable[index].BlockId == BlockId)
        {
            return &NvM_BlockDescriptorTable[index];
        }
    }
    return NULL;
}

static void NvM_Copy(void *DestinationPtr, const void *SourcePtr,
                     uint16_t Length)
{
    uint16_t index;
    uint8_t *destination = (uint8_t *)DestinationPtr;
    const uint8_t *source = (const uint8_t *)SourcePtr;
    for (index = 0U; index < Length; index++)
    {
        destination[index] = source[index];
    }
}

void NvM_Init(void)
{
    uint16_t index;
    uint8_t legacyData[NVM_LEGACY_BUFFER_LENGTH];

    MemIf_Init();
    s_pendingBlock = 0U;
    s_result = NVM_REQ_OK;
    for (index = 0U; index < NvM_BlockDescriptorCount; index++)
    {
        const NvM_BlockDescriptorType *block =
            &NvM_BlockDescriptorTable[index];
        if ((MemIf_Read(block->BlockId, block->RamBlockDataAddress,
                        block->Length) == E_OK) &&
            block->Validate(block->RamBlockDataAddress))
        {
            continue;
        }
        if ((block->LegacyLength <= sizeof(legacyData)) &&
            (MemIf_Read(block->BlockId, legacyData,
                        block->LegacyLength) == E_OK) &&
            (block->ValidateLegacy != NULL) &&
            block->ValidateLegacy(legacyData) &&
            (block->MigrateLegacy != NULL))
        {
            block->MigrateLegacy(block->RamBlockDataAddress, legacyData);
        }
        else
        {
            NvM_Copy(block->RamBlockDataAddress,
                     block->RomBlockDataAddress, block->Length);
        }
        (void)MemIf_Write(block->BlockId, block->RamBlockDataAddress,
                          block->Length);
    }
}

void NvM_MainFunction(void)
{
    const NvM_BlockDescriptorType *block;
    if (s_result != NVM_REQ_PENDING)
    {
        return;
    }
    block = NvM_FindBlock(s_pendingBlock);
    if (block == NULL)
    {
        s_result = NVM_REQ_NOT_OK;
        return;
    }
    s_result = (MemIf_Write(block->BlockId, block->RamBlockDataAddress,
                            block->Length) == E_OK) ?
               NVM_REQ_OK : NVM_REQ_NOT_OK;
    s_pendingBlock = 0U;
}

Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, void *DstPtr)
{
    const NvM_BlockDescriptorType *block = NvM_FindBlock(BlockId);
    if ((block == NULL) || (DstPtr == NULL) ||
        (s_result == NVM_REQ_PENDING))
    {
        return E_NOT_OK;
    }
    NvM_Copy(DstPtr, block->RamBlockDataAddress, block->Length);
    s_result = NVM_REQ_OK;
    return E_OK;
}

Std_ReturnType NvM_WriteBlock(NvM_BlockIdType BlockId,
                              const void *SrcPtr)
{
    const NvM_BlockDescriptorType *block = NvM_FindBlock(BlockId);

    if ((block == NULL) ||
        (SrcPtr == NULL) ||
        !block->Validate(SrcPtr))
    {
        return E_NOT_OK;
    }

#if (NVM_WRITE_SYNC_MODE == 0U)
    if (s_result == NVM_REQ_PENDING)
    {
        return E_NOT_OK;
    }
#endif

    NvM_Copy(block->RamBlockDataAddress,
             SrcPtr,
             block->Length);

#if (NVM_WRITE_SYNC_MODE == 1U)

    /* 同步模式：这里直接真正写 Flash */
    if (MemIf_Write(block->BlockId,
                    block->RamBlockDataAddress,
                    block->Length) == E_OK)
    {
        s_result = NVM_REQ_OK;
        s_pendingBlock = 0U;
        return E_OK;
    }

    s_result = NVM_REQ_NOT_OK;
    s_pendingBlock = 0U;
    return E_NOT_OK;

#else

    /* 异步模式：保持原来的实现 */
    s_pendingBlock = BlockId;
    s_result = NVM_REQ_PENDING;

    return E_OK;

#endif
}

Std_ReturnType NvM_GetErrorStatus(NvM_BlockIdType BlockId,
                                  NvM_RequestResultType *RequestResultPtr)
{
    if ((NvM_FindBlock(BlockId) == NULL) || (RequestResultPtr == NULL))
    {
        return E_NOT_OK;
    }
    *RequestResultPtr = s_result;
    return E_OK;
}
