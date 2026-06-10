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
* \file     variants.h
*
* \brief    Device specific memory layout defines and features
*
* \version  V1.2.0
* \date     10. Sep 2025
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
** V1.0.2: 2024-03-20, VO:   [ATVCOMETLL-147] Added IAR support               **
** V1.0.3: 2024-03-25, SL:   [ATVCOMETLL-156] Added 1000TP info               **
** V1.0.4: 2024-07-11, VO:   [ATVCOMETLL-118] Fixed MISRA violations          **
** V1.0.5: 2024-07-15, VO:   [ATVCOMETLL-87] Added function to check whether  **
**                            pointer is inside RAM range                     **
** V1.0.6: 2024-11-19, VO:   [ATVCOMETLL-170] Fixed MISRA violations          **
** V1.0.7: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.8: 2025-02-17, VO:   [ATVCOMETLL-183] Refactoring                     **
** V1.0.9: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.10: 2025-07-07, VO:  [ATVCOMETLL-202] Updated license text            **
** V1.1.0: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
** V1.2.0: 2025-09-10, VO:   [ATVCOMETLL-217] Added support for TLD4030-12ES  **
*******************************************************************************/

#ifndef _VARIANTS_H
#define _VARIANTS_H

#include "RTE_Components.h"

/* Family definitions  */
#define TLD4       (4)

/* Series definitions  */
#define TLD40xx    (40)

/* Device definitions  */
#define TLD4020_3  (40203)
#define TLD4020_4  (40204)
#define TLD4030_12 (403012)

/* Package definitions */
#define TFDSO16     (1)
#define TSDSO24     (2)

/* TLD40xx devices */
/* TLD4020_3ET:  16pin, 3K RAM, 32K Flash (2K config sector + 1000TP and 30K user code), LIN, ADC */
#if defined(TLD4020_3ET)
  #define UC_FAMILY       TLD4
  #define UC_SERIES       TLD40xx
  #define UC_DEVICE       TLD4020_3
  #define UC_PACKAGE      TFDSO16
  /* RAM */
  /* UC_RAM_START is identical for all devices */
  #define UC_RAM_SIZE          (0x0C00U)
  /* Flash */
  #define UC_FLASH_SIZE        (0x7800U)

  /* TLD4020_4ET:  16pin, 3K RAM, 32K Flash (2K config sector + 1000TP and 30K user code), LIN, ADC */
#elif defined(TLD4020_4ET)
  #define UC_FAMILY       TLD4
  #define UC_SERIES       TLD40xx
  #define UC_DEVICE       TLD4020_4
  #define UC_PACKAGE      TFDSO16
  /* RAM */
  /* UC_RAM_START is identical for all devices */
  #define UC_RAM_SIZE          (0x0C00U)
  /* Flash */
  #define UC_FLASH_SIZE        (0x7800U)

  /* TLD4030_12ES:  24pin, 3K RAM, 38K Flash (2K config sector + 1000TP and 36K user code), LIN, ADC */
#elif defined(TLD4030_12ES)
  #define UC_FAMILY       TLD4
  #define UC_SERIES       TLD40xx
  #define UC_DEVICE       TLD4030_12
  #define UC_PACKAGE      TSDSO24
  /* RAM */
  /* UC_RAM_START is identical for all devices */
  #define UC_RAM_SIZE          (0x0C00U)
  /* Flash */
  #define UC_FLASH_SIZE        (0x9000U)

#else
  #error "variant.h: device not supported"
#endif

/* Define Memory - common for all TLD40xx devices */
/* RAM */
#define UC_RAM_START          (0x18000000U)
/* Flash */
#define UC_FLASH_PAGE_SIZE    (64U)
#define UC_FLASH_SECTOR_SIZE  (0x800U)
#define UC_FLASH_START        (0x12000000U)
/* 1000TP */
#define UC_1000TP_PAGE_SIZE   (64U)
#define UC_1000TP_START       (0x11FFFDC0U)
#define UC_1000TP_SIZE        (0x240U)

/* Variables for UC_FLASH_UBSL and UC_FLASH_UCODE defined in Device_init */

#if (UC_DEVICE == 40203)
  #include "tld40xx.h"
#elif (UC_DEVICE == 40204)
  #include "tld40xx_4et.h"
#elif (UC_DEVICE == 403012)
  #include "tld40xx_12es.h"
#else
  #error "variant.h: device not supported"
#endif

__STATIC_INLINE bool PtrRangeCheck(const void *ptr)
{
  bool b_res;
  //lint --e{9034}
  b_res = false;

  /* check if input pointer resides inside the RAM */
  if (((uint32_t)ptr >= UC_RAM_START) && ((uint32_t)ptr < (UC_RAM_START + UC_RAM_SIZE)))
  {
    //lint --e{9034}
    b_res = true;
  }

  return (b_res);
}

#endif /* _VARIANTS_H */
