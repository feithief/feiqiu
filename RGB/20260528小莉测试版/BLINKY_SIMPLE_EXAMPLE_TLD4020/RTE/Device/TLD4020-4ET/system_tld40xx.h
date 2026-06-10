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
 * \file     system_tld40xx.h
 *
 * \brief    System file for TLD40xx
 *
 * \version  V1.0.6
 * \date     02. Sep 2025
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
** V1.0.3: 2024-03-20, VO:   [ATVCOMETLL-147] Added IAR support               **
** V1.0.4: 2024-07-12, VO:   [ATVCOMETLL-118] Fixed MISRA violations          **
** V1.0.5: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.0.6: 2025-09-02, VO:   [ATVCOMETLL-205] Removed param for systick init  **
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


#define VECTOR_TABLE_SIZE       24
#define CONFIG_TABLE_SIZE       16

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
