#include "Fls.h"
#include "tcpl04x_ll_flash.h"
#include "pal_store.h"

void Fls_Init(void)
{
		ll_flash_init();
    pal_store_length_set(STORE_SECTOR_SIZE);
}

Std_ReturnType Fls_Read(uint32_t Address, void *DataPtr, uint16_t Length)
{
    if ((DataPtr == NULL) || (Length == 0U))
    {
        return E_NOT_OK;
    }
    return pal_store_data_get(Address, (uint8_t *)DataPtr, Length) ?
           E_OK : E_NOT_OK;
}

Std_ReturnType Fls_Write(uint32_t Address, const void *DataPtr,
                         uint16_t Length)
{
    if ((DataPtr == NULL) || (Length == 0U))
    {
        return E_NOT_OK;
    }
    return pal_store_data_set(Address, (uint8_t *)DataPtr, Length) ?
           E_OK : E_NOT_OK;
}
