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
* \file     isr_nvic_irq4_handler.h
*
* \brief    Interrupt Service Routine for NVIC IRQ4
*
* \version  V1.0.7
* \date     07. Jul 2025
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
** V1.0.2: 2023-09-19, VO:   [ATVCOMETLL-129] Fixed ovtemp callback bugs      **
** V1.0.3: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.4: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.5: 2025-02-17, VO:   [ATVCOMETLL-183] Refactoring                     **
** V1.0.6: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.7: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

#if !defined(_ISR_NVIC_IRQ4_HANDLER_H_)
#define _ISR_NVIC_IRQ4_HANDLER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "cpu_defines.h"
#include "isr_defines.h"
#include "isr.h"

#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN4_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN4_Pos))

#if ((defined NVIC_IRQ4_HANDLER_INT_CHECK) && (NVIC_IRQ4_HANDLER_INT_CHECK == 1))
extern uint8_t u8_interrupt_cnt_irq4;
#endif

#if ((ADC11_FW_EOC_INT_EN == 1u) || \
     (ADC11_DTS_OVTEMP_INT_EN == 1u))

void NVIC_IRQ4_Handler(void);

#endif /* ((ADC11_FW_EOC_INT_EN == 1u) || \
           (ADC11_DTS_OVTEMP_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN4_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN4_Pos)) */

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_ISR_NVIC_IRQ4_HANDLER_H_)*/
