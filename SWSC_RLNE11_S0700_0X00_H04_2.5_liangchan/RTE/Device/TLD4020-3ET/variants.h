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

 
 /**
 * \file     variants.h
 *
 * \brief    Device specific memory layout defines and features
 *
 * \version  V1.0.1
 * \date     28. Sep 2023
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
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
*******************************************************************************/

#ifndef _VARIANTS_H
#define _VARIANTS_H

/* Family definitions  */
#define TLD4       (4)

/* Series definitions  */
#define TLD40xx    (40)
#define TLD4xx1    (41)

/* Device definitions  */
#define TLD4020    (4020)
#define TLD4221    (4221)

/* Package definitions */
#define TFDSO16     (1)
#define TSDSO24     (2)

/* TLD40xx, TLD4xx1 devices */
/* supported features:  */
/* UC_FEATURE_DATAFLASH */
/* UC_FEATURE_16PIN, UC_FEATURE_24PIN*/

/* TLD40xx devices */
/* TLD4021_3ET:  16pin, 3K RAM, 30K Flash, LIN, DADC */
#if defined(TLD4020_3ET)
  #define UC_FAMILY       TLD4
  #define UC_SERIES       TLD40xx
  #define UC_DEVICE       TLD4020
  #define UC_PACKAGE      TFDSO16
  /* RAM */
  /* UC_RAM_START is identical for all devices */
  #define UC_RAM_SIZE          (0x0C00U)
  /* Flash */
  #define UC_FLASH_SIZE        (0x7800U)
  /* Features */
  #define UC_FEATURE_DATAFLASH
  #define UC_FEATURE_DADC
  #define UC_FEATURE_LIN
  #define UC_FEATURE_16PIN
#else
  #error "variant.h: device not supported"
#endif

/* Define Memory - common for all TLD40xx devices */
/* RAM */
#define UC_RAM_START          (0x18000000U)
#define UC_RAM_SIZE           (0x0C00U)
/* Flash */
#define UC_FLASH_PAGE_SIZE    (64U)
#define UC_FLASH_SECTOR_SIZE  (0x800U)
#define UC_FLASH_START        (0x12000000U)
/* Variables for UC_FLASH_UBSL and UC_FLASH_UCODE defined in Device_init */

/*******************************************************************************
**                   Definition of configuration page in FLASH                **
*******************************************************************************/
#define __CONFIG_TABLE_ATTRIBUTE  __attribute__((section(".ARM.__at_0x120000C0"),used))

#endif /* _VARIANTS_H */
