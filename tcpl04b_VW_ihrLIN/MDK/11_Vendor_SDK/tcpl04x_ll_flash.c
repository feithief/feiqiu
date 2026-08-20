/**
 *****************************************************************************
 * @brief   flash Source file.
 *
 * @file    tcpl04x_ll_flash.c
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

#include "tcpl04x_ll_flash.h"

#define FLASH_DRV_CIPER_SIZE   (52)

#ifdef __BOOTLOADER_SERES__
typedef uint8_t (*flash_erase_func_t)(void);
typedef uint8_t (*flash_write_func_t)(uint32_t addr, uint8_t *ptr);

/* xxd -i -c 10  flashdrv.bin > test.c */
flash_erase_func_t flash_drv_sector_erase = (flash_erase_func_t)(0x10000009u);
flash_write_func_t flash_drv_sector_write = (flash_write_func_t)(0x10000021u);
#endif

#define LL_FLASH_BUSY_TIMEOUT_CYCLES    (DEFAULT_SYSTEM_CLOCK)

static bool ll_flash_erase_ready_wait(void)
{
    uint32_t timeout = LL_FLASH_BUSY_TIMEOUT_CYCLES;

    while (EFLASH->STATUS_F.ERASE_BUSY_STATUS == 1U)
    {
        if (timeout-- == 0U)
        {
            return false;
        }
    }

    return true;
}

static bool ll_flash_program_ready_wait(void)
{
    uint32_t timeout = LL_FLASH_BUSY_TIMEOUT_CYCLES;

    while (EFLASH->STATUS_F.PROG_BUSY_STATUS == 1U)
    {
        if (timeout-- == 0U)
        {
            return false;
        }
    }

    return true;
}

/********************************************************
** \brief   This function initializes flash
**
** \param   None
**
** \retval  None
*********************************************************/
void ll_flash_init(void)
{
    FLASH_UNLOCK_CONFIG();

    /*Read Timing configuration*/
    EFLASH->RD_TIME_CFG      = 0X00003033;
    /*Recovery Timing configuration*/
    EFLASH->RCV_TIME_CFG     = 0x00035CDC;
    /*Erase or program to WE setup timing configuration*/
    EFLASH->NVS_TIME_CFG     = 0x00016058;
    /*Program timing configuration*/
    EFLASH->PROG_TIME_CFG    = 0x00161ADC;
    /*Erase timing configuration*/
    EFLASH->ERASE_TIME_CFG   = 0x00000809;
    /*Latency timing configuration*/
    EFLASH->LATENCY_TIME_CFG = 0x0000301A;

    FLASH_LOCK_CONFIG();
}

/********************************************************
** \brief   ll_flash_configrctc
**
** \param   uint8_t      rc_time
**
** \retval  void
*********************************************************/
void ll_flash_configrctc(uint8_t rc_time)
{
    FLASH_UNLOCK_CONFIG();
    EFLASH->RD_TIME_CFG_F.RC_CYC = rc_time;      /** Read time (HCLK) ,  reset:1*/
    FLASH_LOCK_CONFIG();
}

/********************************************************
** \brief   ll_flash_sector_get
**
** \param   flash_type_e    type
** \param   uint32_t        addr
**
** \retval  uint16_t sector index
*********************************************************/
static uint16_t ll_flash_sector_get(flash_type_e type, uint32_t addr)
{
    return ((addr - NVM_FLASH_BASE_ADDR) >> 9);
}

/********************************************************
** \brief   ll_flash_addr_valid_check
**
** \param   flash_type_e    type
** \param   uint32_t        addr
** \param   uint32_t        length
**
** \retval  uint8_t         sector index
*********************************************************/
static uint8_t ll_flash_addr_valid_check(flash_type_e type, uint32_t addr, uint32_t length)
{
    uint8_t res = true;

    if ((type >= FLASH_TYPE_MAX) || (0U == length))
    {
        return false;
    }

    if (FLASH_TYPE_NVM == type)
    {
        res = (addr < NVM_FLASH_END) && (length <= (NVM_FLASH_END - addr));
    }
    else
    {
        res = (addr >= NVR_FLASH_BASE_ADDR) && (addr < NVR_FLASH_END_ADDR) &&
              (length <= (NVR_FLASH_END_ADDR - addr));
    }

    return res;
}

/********************************************************
** \brief   ll_flash_erase_reg
**
** \param   flash_type_e    type:0-NVM 1-NVR
** \param   uint32_t        addr
** \param   uint32_t        length
**
** \retval  None
*********************************************************/
static int ll_flash_erase_reg(flash_type_e type, uint32_t addr, uint32_t length)
{
    uint16_t sector_start = ll_flash_sector_get(type, addr);
    uint16_t sector_end = ll_flash_sector_get(type, (addr + length - 1)) + 1;

    EFLASH->OP_CTRL_F.RDONLY_EN = false;

    for (uint32_t index = sector_start; index < sector_end; index++)
    {
        /* erase sector */
        EFLASH->ERASE_CFG_F.SECTOR_INDEX = index;
#ifdef __BOOTLOADER_SERES__
        flash_drv_sector_erase();
#else
        EFLASH->ERASE_TRIG_F.SECTOR_ERASE_TRIG = 1;

        if (!ll_flash_erase_ready_wait())
        {
            EFLASH->OP_CTRL_F.RDONLY_EN = true;
            return -1;
        }

#endif
    }

    EFLASH->OP_CTRL_F.RDONLY_EN = true;
    return 0;
}

/********************************************************
** \brief   ll_flash_write_reg
**
** \param   uint32_t        addr
** \param   uint8_t*        buffer
** \param   uint32_t        length
**
** \retval  None
*********************************************************/
static int ll_flash_write_reg(uint32_t addr, uint8_t *buffer, uint32_t length)
{
    uint32_t remain_length, reseverd_length;
    flash_size_t temp_data;
    uint32_t offset = addr % FLASH_BYTE_ALIGN;
    uint8_t *ptr = (uint8_t *)&temp_data;
    uint32_t write_len __attribute__((unused));

    EFLASH->OP_CTRL_F.RDONLY_EN = false;

    if (offset)
    {
#if 8 == FLASH_BYTE_ALIGN
        temp_data = 0xFFFFFFFFFFFFFFFF;
#else
        temp_data = 0xFFFFFFFF;
#endif
        *((volatile flash_size_t *)ptr) = *((volatile flash_size_t *)(addr - offset)); //读出
        write_len = (length > (FLASH_BYTE_ALIGN - offset)) ? FLASH_BYTE_ALIGN - offset : length;
        memcpy((uint8_t *)(ptr + offset), (uint8_t *)buffer, write_len); //补齐赋值
#ifdef __BOOTLOADER_SERES__
        flash_drv_sector_write(addr - offset, ptr);
#else
        *((volatile flash_size_t *)(addr - offset)) = *((volatile flash_size_t *)ptr); //回写

        if (!ll_flash_program_ready_wait())
        {
            EFLASH->OP_CTRL_F.RDONLY_EN = true;
            return -1;
        }

#endif
        offset = write_len;
    }

    remain_length = length - offset;
    reseverd_length = remain_length % FLASH_BYTE_ALIGN;

    if (remain_length > reseverd_length)
    {
        for (uint32_t i = 0; i < remain_length - reseverd_length; i += FLASH_BYTE_ALIGN)
        {
            memcpy((uint8_t *)ptr, (uint8_t *)(buffer + offset), sizeof(flash_size_t));
#ifdef __BOOTLOADER_SERES__
            flash_drv_sector_write(addr + offset, ptr);
#else
            *((volatile flash_size_t *)(addr + offset)) = *((volatile flash_size_t *)ptr);

            if (!ll_flash_program_ready_wait())
            {
                EFLASH->OP_CTRL_F.RDONLY_EN = true;
                return -1;
            }

#endif
            offset += FLASH_BYTE_ALIGN;
        }
    }

    if (reseverd_length)
    {
#if 8 == FLASH_BYTE_ALIGN
        temp_data = 0xFFFFFFFFFFFFFFFF;
#else
        temp_data = 0xFFFFFFFF;
#endif
        memcpy((uint8_t *)ptr, (uint8_t *)&buffer[length - reseverd_length], reseverd_length);
#ifdef __BOOTLOADER_SERES__
        flash_drv_sector_write(addr + offset, ptr);
#else
        *((volatile flash_size_t *)(addr + offset)) = *((volatile flash_size_t *)ptr) ;

        if (!ll_flash_program_ready_wait())
        {
            EFLASH->OP_CTRL_F.RDONLY_EN = true;
            return -1;
        }

#endif
    }

    EFLASH->OP_CTRL_F.RDONLY_EN = true;
    return 0;
}

/********************************************************
** \brief   ll_flash_read_reg
**
** \param   uint32_t        addr
** \param   uint8_t*        buffer
** \param   uint32_t        length
**
** \retval  None
*********************************************************/
static void ll_flash_read_reg(uint32_t addr, uint8_t *buffer, uint32_t length)
{
    uint32_t remain_length, reseverd_length;
    flash_size_t temp_data;

    uint32_t offset = addr % FLASH_BYTE_ALIGN;
    uint8_t *ptr = (uint8_t *)&temp_data;
    uint32_t read_len __attribute__((unused));

    if (offset)
    {
        *((volatile flash_size_t *)ptr) = *((volatile flash_size_t *)(addr - offset));
        read_len = (length > (FLASH_BYTE_ALIGN - offset)) ? FLASH_BYTE_ALIGN - offset : length;
        memcpy((uint8_t *)(buffer), (uint8_t *)(ptr + offset), read_len);
        offset = read_len;
    }

    remain_length = length - offset;
    reseverd_length = remain_length % FLASH_BYTE_ALIGN;

    if (remain_length > reseverd_length)
    {
        for (uint32_t i = 0; i < remain_length - reseverd_length; i += FLASH_BYTE_ALIGN)
        {

            *((volatile flash_size_t *)ptr) = *((volatile flash_size_t *)(addr + offset));
            memcpy((uint8_t *)(buffer + offset), (uint8_t *)ptr, sizeof(flash_size_t));
            offset += FLASH_BYTE_ALIGN;
        }
    }

    if (reseverd_length)
    {
        *((volatile flash_size_t *)ptr) = *((volatile flash_size_t *)(addr + offset));

        memcpy((uint8_t *)(buffer + offset), (uint8_t *)ptr, reseverd_length);
    }
}

/********************************************************
** \brief   ll_flash_erase_chip
**
** \param   None
**
** \retval  uint8_t         0-success
*********************************************************/
int ll_flash_erase_chip(void)
{
    /*mask all interupt but NMI and HardFault*/
    __disable_irq();

    FLASH_UNLOCK_CONFIG();

    EFLASH->ERASE_CFG_F.NVR_SECTOR_SEL = 0;
    EFLASH->ERASE_TRIG_F.CHIP_ERASE_TRIG = 1;

    if (!ll_flash_erase_ready_wait())
    {
        FLASH_LOCK_CONFIG();
        __enable_irq();
        return -1;
    }

    FLASH_LOCK_CONFIG();

    /*resume all interupt enabled*/
    __enable_irq();

    return 0;
}

/********************************************************
** \brief   ll_flash_erase  flash erase func
**
** \param   flash_type_e    type:0-NVM 1-NVR
** \param   uint32_t        addr
** \param   uint32_t        length
**
** \retval  int             0-success
*********************************************************/
int ll_flash_erase(flash_type_e type, uint32_t addr, uint32_t length)
{
    int res = 0;

    if ((FLASH_TYPE_NVM != type) || (false == ll_flash_addr_valid_check(type, addr, length)))
    {
        return -1;
    }

    /*mask all interupt but NMI and HardFault*/
    __disable_irq();

    FLASH_UNLOCK_CONFIG();

    EFLASH->ERASE_CFG_F.NVR_SECTOR_SEL = 0;

    res = ll_flash_erase_reg(type, addr, length);

    FLASH_LOCK_CONFIG();

    /*resume all interupt enabled*/
    __enable_irq();

    return res;
}

/********************************************************
** \brief   ll_flash_read      flash read func
**
** \param   flash_type_e    type:0-NVM 1-NVR
** \param   uint32_t        addr
** \param   uint8_t*        buffer
** \param   uint32_t        length
**
** \retval  int             0-success
*********************************************************/
int ll_flash_read(flash_type_e type, uint32_t addr, uint8_t *buffer, uint32_t length)
{
    if ((NULL == buffer) || (false == ll_flash_addr_valid_check(type, addr, length)))
    {
        return -1;
    }

    ll_flash_read_reg(addr, buffer, length);

    return 0;
}

/********************************************************
** \brief   ll_flash_write
**
** \param   flash_type_e    type:0-NVM 1-NVR
** \param   uint32_t        addr
** \param   uint8_t*        buffer
** \param   uint32_t        length
**
** \retval  int             0-success
*********************************************************/
int ll_flash_write(flash_type_e type, uint32_t addr, uint8_t *buffer, uint32_t length)
{
    if ((NULL == buffer) || (FLASH_TYPE_NVM != type) ||
        (false == ll_flash_addr_valid_check(type, addr, length)))
    {
        return -1;
    }

    /*mask all interupt but NMI and HardFault*/
    __disable_irq();

    FLASH_UNLOCK_CONFIG();

    EFLASH->ERASE_CFG_F.NVR_SECTOR_SEL = 0;

    int res = ll_flash_write_reg(addr, buffer, length);

    FLASH_LOCK_CONFIG();

    /*resume all interupt enabled*/
    __enable_irq();

    return res;
}

/********************************************************
** \brief   ll_flash_smart_write
**
** \param   flash_type_e    type:0-NVM 1-NVR
** \param   uint32_t        addr
** \param   uint8_t*        buffer
** \param   uint32_t        length
**
** \retval  int             0-success
*********************************************************/
int ll_flash_smart_write(flash_type_e type, uint32_t addr, uint8_t *buffer, uint32_t length)
{
    if ((NULL == buffer) || (FLASH_TYPE_NVM != type) ||
        (false == ll_flash_addr_valid_check(type, addr, length)))
    {
        return -1;
    }

    /*mask all interupt but NMI and HardFault*/
    __disable_irq();

    FLASH_UNLOCK_CONFIG();

    EFLASH->ERASE_CFG_F.NVR_SECTOR_SEL = 0;

    int res = ll_flash_erase_reg(type, addr, length);
    if (0 == res)
    {
        res = ll_flash_write_reg(addr, buffer, length);
    }

    FLASH_LOCK_CONFIG();

    /*resume all interupt enabled*/
    __enable_irq();

    return res;
}

/********************************************************
** \brief   ll_flash_reg_wr
**
** \param   bool            is_write:write read
** \param   uint32_t        addr
** \param   uint32_t        *reg_value
**
** \retval  int             0-success
*********************************************************/
int ll_flash_reg_wr(bool is_write, uint32_t addr, uint32_t *reg_value)
{
    /* APB_SLV address */
    if (addr > APB_BASE_ADDRESS)
    {
        if (is_write)
        {
            REG_WRITE32(addr, *reg_value);
        }
        else
        {
            *reg_value = REG_READ32(addr);
        }
    }

    {
        *reg_value = REG_READ32(addr);
    }

    return 0;
}

/********************************************************
** \brief   ll_flash_isr_enable
**
** \param   uint32_t        isr_flag
** \param   bool            enable
**
** \retval  int             0-success
*********************************************************/
int ll_flash_isr_enable(uint32_t isr_flag, bool enable)
{
    FLASH_UNLOCK_CONFIG();

    if (enable)
    {
        EFLASH->IMR &= ~(isr_flag);
    }
    else
    {
        EFLASH->IMR |= isr_flag;
    }

    FLASH_LOCK_CONFIG();

    return 0;
}

/********************************************************
** \brief   FLASH_IRQHandler
**
** \param   None
**
** \retval  None
*********************************************************/
void FLASH_IRQHandler(void)
{
    uint32_t isr = EFLASH->ISR;

    FLASH_UNLOCK_CONFIG();

    EFLASH->ICR = isr;

    FLASH_LOCK_CONFIG();
}
