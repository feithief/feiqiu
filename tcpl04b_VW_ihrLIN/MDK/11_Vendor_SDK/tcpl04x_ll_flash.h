/**
 *****************************************************************************
 * @brief   flash header file.
 *
 * @file    tcpl04x_ll_flash.h
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

#ifndef __TCPL04X_LL_FLASH_H__
#define __TCPL04X_LL_FLASH_H__

#include "tcpl04x_ll_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define  FLASH_BYTE_ALIGN                   (8)

#if 8 == FLASH_BYTE_ALIGN
typedef  uint64_t  flash_size_t;
#else
typedef  uint32_t  flash_size_t;
#endif


/** @defgroup FLASH_LOCK_Definitions
  * @{
  */
#define FLASH_LOCK_CONFIG()             (EFLASH->WR_LOCK = 0X12345678)
#define FLASH_UNLOCK_CONFIG()           (EFLASH->WR_LOCK = 0XAA55AA55)

#define FLASH_LOCK_NVR_ACCESS()         (EFLASH->NVR_PROT = 0X12345678)
#define FLASH_UNLOCK_NVR_ACCESS()       (EFLASH->NVR_PROT = 0XAA55AA55)

/** @defgroup FLASH_NVM_Definitions
  * @{
  */
#define  FLASH_SECTOR_SIZE              (512)

/* TCPL04B NVM: 48KB, including the final 1KB UserData area. */
#define  NVM_FLASH_BASE_ADDR            (0x00000000UL)
#define  NVM_FLASH_SIZE                 (0x0000C000UL)
#define  NVM_FLASH_END                  (NVM_FLASH_BASE_ADDR + NVM_FLASH_SIZE)
#define  NVM_FLASH_SECTOR_SIZE          (FLASH_SECTOR_SIZE)

/* NVR: 512B*/
#define  NVR_FLASH_BASE_ADDR            (0x00800000UL)
#define  NVR_FLASH_SIZE                 (0x00000200UL)
#define  NVR_FLASH_END_ADDR             (NVR_FLASH_BASE_ADDR + NVR_FLASH_SIZE)

#define  UID_BASE_ADDR                  (0x00800100UL)

typedef enum
{
    FLASH_TYPE_NVM,
    FLASH_TYPE_NVR,
    FLASH_TYPE_MAX,
} flash_type_e;

void ll_flash_init(void);
void ll_flash_configrctc(uint8_t rc_time);
int ll_flash_erase(flash_type_e type, uint32_t addr, uint32_t length);
int ll_flash_read(flash_type_e type, uint32_t addr, uint8_t *buffer, uint32_t length);
int ll_flash_write(flash_type_e type, uint32_t addr, uint8_t *buffer, uint32_t length);
int ll_flash_smart_write(flash_type_e type, uint32_t addr, uint8_t *buffer, uint32_t length);
int ll_flash_reg_wr(bool is_write, uint32_t addr, uint32_t *reg_value);

#ifdef __cplusplus
}
#endif
#endif /* __TCPL04X_LL_FLASH_H__ */
