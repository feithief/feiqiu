/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2022-2023, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * This software is provided by the copyright holders and contributors "as is" and any express or implied warranties,
 * including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are
 * disclaimed. In no event shall the copyright holder or contributors be liable  for any direct, indirect, incidental,
 * special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or
 * services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability,
 * whether in contract, strict liability,or tort (including negligence or otherwise) arising in any way out  of the
 * use of this software, even if advised of the possibility of such damage.
 *
 **********************************************************************************************************************/

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
uint32 UC_FLASH_UBSL_START;
uint32 UC_FLASH_UBSL_SIZE;
uint32 UC_FLASH_UCODE_START;
uint32 UC_FLASH_UCODE_SIZE;

/*******************************************************************************
**                        Private Function Declarations                       **
*******************************************************************************/

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize all modules of the device
 *
 * \return sint8 0: success, <0: error codes
 */
sint8 Device_init(void)
{
  sint8 s8_returnCode;
  uint32* up32_ucodeUbslSizeAdress = (uint32*)(0x120000C8);
  uint32 u32_ucodeUbslSize;
  s8_returnCode = ERR_LOG_SUCCESS;

  /* RTE LIB initialization */
#ifdef RTE_DEVICE_LIB_PMU
  s8_returnCode |= PMU_init();
#endif
#ifdef RTE_DEVICE_LIB_ADC11
  s8_returnCode |= ADC11_init();
#endif
#ifdef RTE_DEVICE_LIB_LDRV
	s8_returnCode |= LDRV_init();
#endif
#ifdef RTE_DEVICE_LIB_CPU
  s8_returnCode |= CPU_init();
#endif
#ifdef RTE_DEVICE_LIB_MEM
  s8_returnCode |= MEM_init();
#endif
#ifdef RTE_DEVICE_LIB_GPIO
  s8_returnCode |= GPIO_init();
#endif
#ifdef RTE_DEVICE_LIB_LIN
  s8_returnCode |= LIN_init();
#endif

  /* Set variables for memory map */
  u32_ucodeUbslSize = *(up32_ucodeUbslSizeAdress);
  UC_FLASH_UBSL_START = (UC_FLASH_START);
  UC_FLASH_UBSL_SIZE = (uint32) (u32_ucodeUbslSize & 0x00FF) * UC_FLASH_SECTOR_SIZE;
  UC_FLASH_UCODE_SIZE = (uint32) ((u32_ucodeUbslSize & 0xFF00) >> 8) * UC_FLASH_SECTOR_SIZE;
  UC_FLASH_UCODE_START = UC_FLASH_UBSL_START + UC_FLASH_UBSL_SIZE;

  return s8_returnCode;
}
