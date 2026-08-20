#include "Fee.h"

#include "Fls.h"
#include "prj_board_cfg.h"

void Fee_Init(void)
{
    Fls_Init();
}

Std_ReturnType Fee_Read(NvM_BlockIdType BlockId, void *DataPtr,
                        uint16_t Length)
{
    if (BlockId != NVM_BLOCK_SWSC_CONFIG)
    {
        return E_NOT_OK;
    }
    return Fls_Read(SWSC_CONFIG_FLASH_ADDRESS, DataPtr, Length);
}

Std_ReturnType Fee_Write(NvM_BlockIdType BlockId, const void *DataPtr,
                         uint16_t Length)
{
    if (BlockId != NVM_BLOCK_SWSC_CONFIG)
    {
        return E_NOT_OK;
    }
    return Fls_Write(SWSC_CONFIG_FLASH_ADDRESS, DataPtr, Length);
}
