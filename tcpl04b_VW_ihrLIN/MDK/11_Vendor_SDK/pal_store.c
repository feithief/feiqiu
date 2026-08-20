/**
 *****************************************************************************
 * @brief   pal store source file.
 *
 * @file    pal_store.c
 * @author  AE/FAE team
 * @date    2024.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#include "pal_store.h"
#include "utilities.h"

static uint32_t store_total_size = STORE_SECTOR_SIZE;


/********************************************************
** \brief   pal_store_type_check
**
** \param   uint32_t            addr
**
** \retval  None
*********************************************************/
static flash_type_e pal_store_type_check(uint32_t addr)
{
    if (addr < NVM_FLASH_END)
    {
        return FLASH_TYPE_NVM;
    }
    else if (addr >= NVR_FLASH_BASE_ADDR && addr < NVR_FLASH_END_ADDR)
    {
        return FLASH_TYPE_NVR;
    }
    else
    {
        return FLASH_TYPE_MAX;
    }
}

/********************************************************
** \brief   pal_store_length_set
**
** \param   uint32_t            length
**
** \retval  None
*********************************************************/
void pal_store_length_set(uint32_t length)
{
    store_total_size = ((length > 0U) && (length <= STORE_SECTOR_SIZE)) ?
                       length : STORE_SECTOR_SIZE;
}

/********************************************************
** \brief   pal_store_data_set
**
** \param   uint32_t            addr
** \param   uint8_t*            data
** \param   uint32_t            length
**
** \retval  bool
*********************************************************/
bool pal_store_data_set(uint32_t addr, uint8_t *data, uint32_t length)
{
    uint8_t nvrdata[STORE_SECTOR_SIZE] __attribute((aligned(4)));
    uint32_t offset = addr % STORE_SECTOR_SIZE;
    uint32_t crc __attribute__((unused));

    flash_type_e type = pal_store_type_check(addr);

    if ((type >= FLASH_TYPE_MAX) || (NULL == data) || (0U == length) ||
        ((offset + length + sizeof(uint32_t)) > store_total_size))
    {
        return false;
    }

    if (0 != ll_flash_read(type, addr - offset, (uint8_t *)nvrdata, store_total_size))
    {
        return false;
    }

    if (memcmp((uint8_t *)&nvrdata[offset], data, length))
    {
        memcpy((uint8_t *)&nvrdata[offset], data, length);
        crc = crc16_calculate_func(0xFFFF, (uint8_t *)&nvrdata[offset], length);
        memcpy(&nvrdata[offset + length], &crc, sizeof(uint32_t));
        return (0 == ll_flash_smart_write(type, addr - offset, (uint8_t *)nvrdata,
                                          store_total_size));
    }

    return false;
}

/********************************************************
** \brief   pal_store_data_get
**
** \param   uint32_t            addr
** \param   uint8_t*            data
** \param   uint32_t            length
**
** \retval  bool
*********************************************************/
bool pal_store_data_get(uint32_t addr, uint8_t *data, uint32_t length)
{
    uint8_t nvrdata[STORE_SECTOR_SIZE] __attribute((aligned(4)));
    uint32_t offset = addr % STORE_SECTOR_SIZE;
    uint32_t crc;

    flash_type_e type = pal_store_type_check(addr);

    if ((type >= FLASH_TYPE_MAX) || (NULL == data) || (0U == length) ||
        ((offset + length + sizeof(uint32_t)) > store_total_size))
    {
        return false;
    }

    if (0 != ll_flash_read(type, addr - offset, (uint8_t *)nvrdata, store_total_size))
    {
        return false;
    }

    memcpy((uint8_t *)&crc, (uint8_t *)&nvrdata[offset + length], sizeof(uint32_t));

    if (crc == crc16_calculate_func(0xFFFF, (uint8_t *)&nvrdata[offset], length))
    {
        memcpy((uint8_t *)data, (uint8_t *)&nvrdata[offset], length);
        return true;
    }

    return false;
}

/********************************************************
** \brief   pal_store_data_init
**
** \param   uint32_t            addr
** \param   uint8_t*            data
** \param   uint32_t            length
**
** \retval  bool
*********************************************************/
bool pal_store_data_init(uint32_t addr, uint8_t *data, uint32_t length)
{
    uint8_t nvrdata[STORE_SECTOR_SIZE] __attribute((aligned(4)));
    uint32_t offset = addr % STORE_SECTOR_SIZE;
    uint32_t crc;

    flash_type_e type = pal_store_type_check(addr);

    if ((type >= FLASH_TYPE_MAX) || (NULL == data) || (0U == length) ||
        ((offset + length + sizeof(uint32_t)) > STORE_SECTOR_SIZE))
    {
        return false;
    }

    if (0 != ll_flash_read(type, addr - offset, (uint8_t *)nvrdata, STORE_SECTOR_SIZE))
    {
        return false;
    }

    memcpy((uint8_t *)&crc, (uint8_t *)&nvrdata[offset + length], sizeof(uint32_t));

    if (crc != crc16_calculate_func(0xFFFF, (uint8_t *)&nvrdata[offset], length))
    {
        memcpy((uint8_t *)&nvrdata[offset], (uint8_t *)data, length);
        crc = crc16_calculate_func(0xFFFF, (uint8_t *)&nvrdata[offset], length);
        memcpy(&nvrdata[offset + length], (uint8_t *)&crc, sizeof(uint32_t));
        (void)ll_flash_smart_write(type, addr - offset, (uint8_t *)nvrdata,
                                   STORE_SECTOR_SIZE);
        return false;
    }

    memcpy(data, &nvrdata[offset], length);

    return true;
}

/********************************************************
** \brief   pal_store_data_clear
**
** \param   uint32_t            addr
** \param   uint32_t            length
**
** \retval  bool
*********************************************************/
bool pal_store_data_clear(uint32_t addr, uint32_t length)
{
    uint32_t offset = addr % STORE_SECTOR_SIZE;

    flash_type_e type = pal_store_type_check(addr);

    if ((type >= FLASH_TYPE_MAX) || (0U == length) ||
        ((offset != 0U) && ((offset + length) > STORE_SECTOR_SIZE)))
    {
        return false;
    }

    if (!offset && !(length % STORE_SECTOR_SIZE))
    {
        return (0 == ll_flash_erase(type, addr, length));
    }
    else
    {
        uint8_t nvrdata[STORE_SECTOR_SIZE] __attribute((aligned(4)));
        if (0 != ll_flash_read(type, addr - offset, (uint8_t *)nvrdata, STORE_SECTOR_SIZE))
        {
            return false;
        }
        memset((uint8_t *)&nvrdata[offset], 0xFF, length);
        return (0 == ll_flash_smart_write(type, addr - offset, (uint8_t *)nvrdata,
                                          STORE_SECTOR_SIZE));
    }
}

/********************************************************
** \brief   pal_store_erase
**
** \param   flash_type_e    type
** \param   uint32_t        addr
** \param   uint32_t        length
**
** \retval  bool
*********************************************************/
bool pal_store_erase(flash_type_e type, uint32_t addr, uint32_t length)
{
    if (0 != ll_flash_erase(type, addr, length))
    {
        return false;
    }

    return true;
}

/********************************************************
** \brief   pal_store_write
**
** \param   flash_type_e    type
** \param   uint32_t        addr
** \param   uint8_t*        value
** \param   uint32_t        length
**
** \retval  bool
*********************************************************/
bool pal_store_write(flash_type_e type, uint32_t addr, uint8_t *value, uint32_t length)
{
    if (0 != ll_flash_write(type, addr, value, length))
    {
        return false;
    }

    return true;
}

/********************************************************
** \brief   pal_store_read
**
** \param   uint32_t        addr
** \param   uint8_t*        value
** \param   uint32_t        length
**
** \retval  bool
*********************************************************/
bool pal_store_read(flash_type_e type, uint32_t addr, uint8_t *value, uint32_t length)
{
    if (0 != ll_flash_read(type, addr, value, length))
    {
        return false;
    }

    return true;
}

/********************************************************
** \brief   Return the unique device identifier (UID based on 96 bits)
**
** \param   uint32_t        *uid
**
** \retval  None
*********************************************************/
void pal_store_uid_get(uint32_t *uid)
{
    uint32_t buffer[5];

#if defined (__TCPL01X__)
    ll_flash_read(FLASH_TYPE_NVR, UID_BASE_ADDR, (uint8_t *)&buffer[0], 3 * sizeof(uint32_t));

    if (buffer[0] == 0xFFFFFFFF && buffer[1] == 0xFFFFFFFF && buffer[2] == 0xFFFFFFFF)
    {
        ll_flash_read(FLASH_TYPE_NVR, UID_BASE_ADDR_BAK, (uint8_t *)&buffer[0], 5 * sizeof(uint32_t));

        buffer[2] = ((buffer[2] & 0xFF) << 24) | ((buffer[3] & 0xFF) << 16) | ((buffer[4] & 0xFF)  << 8) | 0xFF;
    }

    memcpy((uint8_t *)uid, (uint8_t *)&buffer[0], 3 * sizeof(uint32_t));
#else
    ll_flash_read(FLASH_TYPE_NVR, UID_BASE_ADDR, (uint8_t *)&buffer[0], 4 * sizeof(uint32_t));

    buffer[2] = ((buffer[2] & 0xFF) << 16) | (buffer[3] & 0xFF);
#endif

    memcpy((uint8_t *)uid, (uint8_t *)&buffer[0], 3 * sizeof(uint32_t));
}

/********************************************************
** \brief   Return the unique device identifier (UID based on 96 bits)
**
** \param   uint32_t        *boot_ver
**
** \retval  None
*********************************************************/
void pal_store_boot_ver_get(uint32_t *boot_ver)
{
    uint32_t ver;
    ll_flash_read(FLASH_TYPE_NVM, BOOT_VERSION_ADDR, (uint8_t *)&ver, sizeof(uint32_t));
    ver = (ver & 0xFF) * 10000 + ((ver & 0xFF00) >> 8)  * 100 + ((ver & 0xFF0000) >> 16) ;
    memcpy((uint8_t *)boot_ver, (uint8_t *)&ver, sizeof(ver));
}

/********************************************************
** \brief   pal_store_chip_ver_id_get
**
** \param   uint8_t*        chip_ver
** \param   uint16_t*       chip_id
**
** \retval  None
*********************************************************/
void pal_store_chip_ver_id_get(uint8_t *chip_ver, uint16_t *chip_id)
{
    ll_syscfg_info_get(chip_ver, chip_id);
}

/********************************************************
** \brief   pal_store_reg_rw
**
** \param   bool            is_write
** \param   uint32_t        addr
** \param   uint32_t        *value
**
** \retval  None
*********************************************************/
void pal_store_reg_rw(bool is_write, uint32_t addr, uint32_t *value)
{
    ll_flash_reg_wr(is_write, addr, value);
}
