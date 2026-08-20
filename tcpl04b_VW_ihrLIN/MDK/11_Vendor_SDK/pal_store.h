/**
 *****************************************************************************
 * @brief   pal store header file.
 *
 * @file    pal_store.h
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

#ifndef __PAL_STORE_H__
#define __PAL_STORE_H__

#include "pal_func_def.h"

#if defined (__TCPL01X__)
#define STORE_TYPE_SEL              (FLASH_TYPE_NVR)
#define STORE_SECTOR_SIZE           (NVR_FLASH_SECTOR_SIZE)
#define BOOT_VERSION_ADDR           (0x00002E18UL)
#elif defined (__TCPL03X__) || defined (__TCPL04X__)
#define STORE_TYPE_SEL              (FLASH_TYPE_NVM)
#define STORE_SECTOR_SIZE           (NVM_FLASH_SECTOR_SIZE)
#define BOOT_VERSION_ADDR           (0x00001E18UL)
#elif defined (__TCPL08X__)
#define STORE_TYPE_SEL              (FLASH_TYPE_NVM)
#define STORE_SECTOR_SIZE           (NVM_FLASH_SECTOR_SIZE)
#define BOOT_VERSION_ADDR           (0x00003E18UL)
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool pal_store_data_set(uint32_t addr, uint8_t *data, uint32_t length);
bool pal_store_data_get(uint32_t addr, uint8_t *data, uint32_t length);
bool pal_store_data_init(uint32_t addr, uint8_t *data, uint32_t length);
void pal_store_length_set(uint32_t length);
bool pal_store_data_clear(uint32_t addr, uint32_t length);
bool pal_store_erase(flash_type_e type, uint32_t addr, uint32_t length);
bool pal_store_write(flash_type_e type, uint32_t addr, uint8_t *value, uint32_t length);
bool pal_store_read(flash_type_e type, uint32_t addr, uint8_t *value, uint32_t length);

void pal_store_uid_get(uint32_t *uid);
void pal_store_boot_ver_get(uint32_t *boot_ver);
void pal_store_chip_ver_id_get(uint8_t *chip_ver, uint16_t *chip_id);
void pal_store_reg_rw(bool is_write, uint32_t addr, uint32_t *value);

#ifdef __cplusplus
}
#endif
#endif /*__PAL_STORE_H__*/
