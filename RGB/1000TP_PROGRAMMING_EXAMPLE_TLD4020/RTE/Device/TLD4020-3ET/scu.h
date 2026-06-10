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
* \file     scu.h
*
* \brief    System Control Unit low level access library
*
* \version  V1.1.1
* \date     08. Sep 2025
*
*/

/** \addtogroup SCU_api
*  @{
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
** V1.0.2: 2024-07-17, VO:   [ATVCOMETLL-112] Added delay_us function and     **
**                            necessary INLINE functions                      **
** V1.0.3: 2024-10-23, VO:   [ATVCOMETLL-143] Improved SCU_enterSleepMode     **
** V1.0.4: 2024-10-28, VO:   [ATVCOMETLL-114] Removed config of Sys0 and CPU  **
**                            divider from init                               **
** V1.0.5: 2024-11-19, VO:   [ATVCOMETLL-170] Fixed MISRA violations          **
** V1.0.6: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.7: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V1.0.8: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.9: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.1.0: 2025-08-19, VO:   [ATVCOMETLL-216] Add API to get clocks           **
** V1.1.1: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
*******************************************************************************/

#if !defined(_SCU_H_)
#define _SCU_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "scu_defines.h"

/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/
/** \brief SCU clock source 0 in kHz */
#define SCU_CLKSRC0               40000
/** \brief SCU clock source 2 in kHz */
#define SCU_CLKSRC2               80000
/** \brief SCU reference clock in kHz */
#define SCU_REFCLK                100
/** \brief SCU master clock in kHz */
#define SCU_MCLK                  20000

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/
void SCU_enterSleepMode(void);
uint8_t SCU_init(void);
void SCU_delay_us(uint32_t u32_time_us);
uint32_t SCU_getClkSrc1_kHz(void);
uint32_t SCU_getSys0Clk_kHz(void);
uint32_t SCU_getCpuClk_kHz(void);
uint32_t SCU_getFiltClk_kHz(void);
uint32_t SCU_getSystickClk_kHz(void);
INLINE uint32_t SCU_getSysTickCntVal(void);
INLINE uint32_t SCU_getSysTickRelVal(void);


/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/
/** \brief Get the current SysTick Count Value
 *
 * \return current SysTick Count Value
 */
INLINE uint32_t SCU_getSysTickCntVal(void)
{
  return (uint32_t)CPU->SYST_CVR.bit.CURRENT;
}

/** \brief Get the current SysTick Reload Value
 *
 * \return SysTick Reload Value
 */
INLINE uint32_t SCU_getSysTickRelVal(void)
{
  return (uint32_t)CPU->SYST_RVR.bit.RELOAD;
}


#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_SCU_H_)*/
