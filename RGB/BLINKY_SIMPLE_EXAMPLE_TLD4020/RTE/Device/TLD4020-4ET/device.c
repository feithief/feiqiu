/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/

#include "device.h"

/*******************************************************************************
**                        Private Constant Declarations                       **
*******************************************************************************/

/*******************************************************************************
**                         Private Macro Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                         Global Variable Definitions                        **
*******************************************************************************/
uint32_t UC_FLASH_UBSL_START;
uint32_t UC_FLASH_UBSL_SIZE;
uint32_t UC_FLASH_UCODE_START;
uint32_t UC_FLASH_UCODE_SIZE;

/*******************************************************************************
**                        Private Function Declarations                       **
*******************************************************************************/

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize all modules of the device
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t Device_init(void)
{
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;
  /* RTE LIB initialization */
#ifdef RTE_DEVICE_LIB_PMU
  u8_returnCode |= PMU_init();
#endif
#ifdef RTE_DEVICE_LIB_ADC11
  u8_returnCode |= ADC11_init();
#endif
#ifdef RTE_DEVICE_LIB_LDRV
  u8_returnCode |= LDRV_init();
#endif
#ifdef RTE_DEVICE_LIB_CPU
  u8_returnCode |= CPU_init();
#endif
#ifdef RTE_DEVICE_LIB_MEM
  u8_returnCode |= MEM_init();
#endif
#ifdef RTE_DEVICE_LIB_GPIO
  u8_returnCode |= GPIO_init();
#endif
#ifdef RTE_DEVICE_LIB_LIN
  u8_returnCode |= LIN_init();
#endif
#ifdef RTE_DEVICE_LIB_UART2
  u8_returnCode |= UART2_init();
#endif
  /* Set variables for memory map */
  UC_FLASH_UBSL_START = (UC_FLASH_START);
  UC_FLASH_UBSL_SIZE = (uint32_t)DEVICE_CONFIG->UBSL_UCODE.bit.UBSL_SIZE * UC_FLASH_SECTOR_SIZE;
  UC_FLASH_UCODE_SIZE = (uint32_t)DEVICE_CONFIG->UBSL_UCODE.bit.UCODE_SIZE * UC_FLASH_SECTOR_SIZE;
  UC_FLASH_UCODE_START = UC_FLASH_UBSL_START + UC_FLASH_UBSL_SIZE;
  return u8_returnCode;
}
