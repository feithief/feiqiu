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
 * \file     system_tld40xx.h
 *
 * \brief    System file for TLD40xx
 *
 * \version  V1.0.2
 * \date     28. Sep 2023
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** VO           Vanessa Ongaro                                                **
** JO           Julia Ott                                                     **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-09-08, VO:   Removed rev history from system_tld40xx.c        **
** V1.0.2: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
*******************************************************************************/

#ifndef __SYSTEM_TLD40xx_H
#define __SYSTEM_TLD40xx_H

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/

#include "types.h"
#include "error_codes.h"
#include "cmsis_misra.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UNIT_TESTING
	/* Allow modification of read-only values when running unit tests */
	#undef __IM
	#define __IM volatile
#endif

#if defined (__CC_ARM) || defined(__ARMCC_VERSION)
  #define VECTOR_TABLE_SIZE       24
  #define CONFIG_TABLE_SIZE       16
  #define CONFIG_TABLE_ATTRIBUTE  __attribute__((section(".ARM.__at_0x120000C0"),used))
#else
  #define VECTOR_TABLE_SIZE       48
  #define CONFIG_TABLE_SIZE       16
  #define CONFIG_TABLE_ATTRIBUTE  __VECTOR_TABLE_ATTRIBUTE
#endif

/**
  \brief Exception / Interrupt Handler Function Prototype
*/
typedef void(*VECTOR_TABLE_Type)(void);

/*******************************************************************************
**                        Global Variable Declarations                        **
*******************************************************************************/

/**
  \brief System Clock Frequency (Core Clock)
*/
extern uint32_t SystemCoreClock;

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/

/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_TLD40xx_H */
