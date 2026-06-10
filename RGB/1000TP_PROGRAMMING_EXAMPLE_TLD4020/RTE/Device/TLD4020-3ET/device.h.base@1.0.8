/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
* \file     device.h
*
* \brief    SDK initialization
*
* \version  V1.0.8
* \date     02. Sep 2025
*
* \note
*/

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** JO           Julia Ott                                                     **
** VO           Vanessa Ongaro                                                **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.2: 2023-10-10, VO:   [ATVCOMETLL-125] Added Bootrom driver            **
** V1.0.3: 2024-07-11, VO:   [ATVCOMETLL-118] Fixed MISRA violations          **
** V1.0.4: 2024-11-19, VO:   [ATVCOMETLL-170] Fixed MISRA violations          **
** V1.0.5: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V1.0.6: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.7: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.0.8: 2025-09-02, VO:   [ATVCOMETLL-205] Added UART2                     **
*******************************************************************************/

#ifndef _DEVICE_H
#define _DEVICE_H

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/

#include "RTE_Components.h"
#include "variants.h"
#include "types.h"
#include "error_codes.h"

/* RTE LIB includes */

#ifdef RTE_DEVICE_LIB_SCU
  #include "scu.h"
#endif
#ifdef RTE_DEVICE_LIB_PMU
  #include "pmu.h"
#endif
#ifdef RTE_DEVICE_LIB_ADC11
  #include "adc11.h"
#endif
#ifdef RTE_DEVICE_LIB_LDRV
  #include "ldrv.h"
#endif
#ifdef RTE_DEVICE_LIB_CPU
  #include "cpu.h"
#endif
#ifdef RTE_DEVICE_LIB_MEM
  #include "mem.h"
#endif
#ifdef RTE_DEVICE_LIB_GPIO
  #include "gpio.h"
#endif
#ifdef RTE_DEVICE_LIB_LIN
  #include "lin.h"
#endif
#ifdef RTE_DEVICE_LIB_UART2
  #include "uart2.h"
#endif
#ifdef RTE_DEVICE_LIB_BOOTROM
  #include "bootrom.h"
#endif


/*******************************************************************************
**                           Global Type Definitions                          **
*******************************************************************************/
/** \brief Struct for device configuration data */
typedef struct
{
  union
  {
    __IM  uint32_t reg;                    /*!< (@ 0x120000C0) NAC/NAD/FTO values */
    struct
    {
      __IM  uint32_t NAC         :  8;     /*!< [7..0]   BSL no activity counter value */
      __IM  uint32_t NAD         :  8;     /*!< [15..8]  LIN node address */
      __IM  uint32_t FTO         :  8;     /*!< [23..16] Frame time out */
      uint32_t             :  8;
    } bit;
  } NAC_NAD;

  union
  {
    __IM  uint32_t reg;                    /*!< (@ 0x120000C4) reserved */
    struct
    {
      __IM  uint32_t RESERVED    : 32;
    } bit;
  } RESERVED;

  union
  {
    __IM  uint32_t reg;                    /*!< (@ 0x120000C8) UBSL/UCODE sizes */
    struct
    {
      __IM  uint32_t UBSL_SIZE   :  8;     /*!< [7..0]  Number of sectors for UBSL */
      __IM  uint32_t UCODE_SIZE  :  8;     /*!< [15..8] Number of sectors for UCODE */
      uint32_t             : 16;
    } bit;
  } UBSL_UCODE;
} DEVICE_CONFIG_Type;


/*******************************************************************************
**                        Global Variable Declarations                        **
*******************************************************************************/
extern uint32_t UC_FLASH_UBSL_START;
extern uint32_t UC_FLASH_UBSL_SIZE;
extern uint32_t UC_FLASH_UCODE_START;
extern uint32_t UC_FLASH_UCODE_SIZE;

#ifndef UNIT_TESTING_CMT
  #define DEVICE_CONFIG_BASE       0x120000C0
  #define DEVICE_CONFIG            ((DEVICE_CONFIG_Type*) DEVICE_CONFIG_BASE)
#else
  extern DEVICE_CONFIG_Type        *DEVICE_CONFIG;
#endif /* UNIT_TESTING_CMT */


/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/

/** \brief Initialize the device modules
 *
 * \ingroup device_api
 */
uint8_t Device_init(void);

#endif
