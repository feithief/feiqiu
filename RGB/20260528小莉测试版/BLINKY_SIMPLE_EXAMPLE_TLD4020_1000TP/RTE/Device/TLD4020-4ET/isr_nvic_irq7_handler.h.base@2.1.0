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
* \file     isr_nvic_irq7_handler.h
*
* \brief    Interrupt Service Routine for NVIC IRQ7
*
* \version  V2.1.0
* \date     10. Oct 2025
*
*/

/** \addtogroup ISR_api
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
** V1.0.1: 2023-03-01, SL:   Added missing function declarations              **
** V1.0.2: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.3: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.4: 2025-02-17, VO:   [ATVCOMETLL-183] Refactoring                     **
** V1.0.5: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.6: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V2.0.0: 2025-07-24, VO:   [ATVCOMETLL-203] Updated enums to tld40xx_4et.h  **
**                            latest version v4.0.0                           **
** V2.1.0: 2025-10-10, VO:   [ATVCOMETLL-217] Added support for TLD4030_12ES  **
*******************************************************************************/

#if !defined(_ISR_NVIC_IRQ7_HANDLER_H_)
#define _ISR_NVIC_IRQ7_HANDLER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "cpu_defines.h"
#include "isr_defines.h"
#include "isr.h"

#ifndef GPIO_IEN3_INT_EN
#define GPIO_IEN3_INT_EN 0
#endif
#ifndef GPIO_IEN4_INT_EN
#define GPIO_IEN4_INT_EN 0
#endif

#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN7_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN7_Pos))

#if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
extern uint8_t u8_interrupt_cnt_irq7;
#endif

#if ((GPIO_IEN0_INT_EN == 1u) || \
     (GPIO_IEN1_INT_EN == 1u) || \
     (GPIO_IEN3_INT_EN == 1u) || \
     (GPIO_IEN4_INT_EN == 1u))

void NVIC_IRQ7_Handler(void);

#endif /* ((GPIO_IEN0_INT_EN == 1u) || \
           (GPIO_IEN1_INT_EN == 1u) || \
           (GPIO_IEN3_INT_EN == 1u) || \
           (GPIO_IEN4_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN7_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN7_Pos)) */

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_ISR_NVIC_IRQ7_HANDLER_H_)*/
