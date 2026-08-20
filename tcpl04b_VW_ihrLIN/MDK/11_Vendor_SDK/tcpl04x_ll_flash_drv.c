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

#define LL_FLASH_DRV_BUSY_TIMEOUT_CYCLES    (DEFAULT_SYSTEM_CLOCK)


/* #1 this offset should align with scatter file */
#define FLASH_DRV_OFFSET     (0x00000008)
#define CAL_OFFSET(func_ptr) ((uint32_t)(func_ptr) - FLASH_DRV_OFFSET)


typedef uint8_t (*flash_erase_func_t)(void);
typedef uint8_t (*flash_write_func_t)(void);

typedef struct
{
    flash_erase_func_t    erase;
    flash_write_func_t    write;
} flash_driver_api_t;

/********************************************************
** \brief   ll_flash_erase_reg
**
** \param   None
**
** \retval  None
*********************************************************/
__attribute__((section(".Flash_Driver_Section")))
void ll_flash_erase_reg(void)
{
    uint32_t timeout = LL_FLASH_DRV_BUSY_TIMEOUT_CYCLES;

    EFLASH->ERASE_TRIG_F.SECTOR_ERASE_TRIG = 1;

    while ((EFLASH->STATUS_F.ERASE_BUSY_STATUS == 1U) && (timeout > 0U))
    {
        timeout--;
    }
}

/********************************************************
** \brief   ll_flash_write_reg
**
** \param   uint32_t        addr
** \param   uint8_t*        ptr
**
** \retval  None
*********************************************************/
__attribute__((section(".Flash_Driver_Section")))
void ll_flash_write_reg(uint32_t addr, uint8_t *ptr)
{
    uint32_t timeout = LL_FLASH_DRV_BUSY_TIMEOUT_CYCLES;

    *((volatile flash_size_t *)addr) = *((volatile flash_size_t *)ptr);
    while ((EFLASH->STATUS_F.PROG_BUSY_STATUS == 1U) && (timeout > 0U))
    {
        timeout--;
    }
}

/********************************************************
**
** \brief   Flash drv API
**
*********************************************************/
__attribute__((used, section(".Flash_Driver_Section_Offset")))
static const flash_driver_api_t g_flash_driver_api =
{
    (flash_erase_func_t) CAL_OFFSET(ll_flash_erase_reg),
    (flash_write_func_t) CAL_OFFSET(ll_flash_write_reg),
};
