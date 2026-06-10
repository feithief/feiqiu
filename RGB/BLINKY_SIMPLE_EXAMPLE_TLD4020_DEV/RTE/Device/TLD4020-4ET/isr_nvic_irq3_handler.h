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
* \file     isr_nvic_irq3_handler.h
*
* \brief    Interrupt Service Routine for NVIC IRQ3
*
* \version  V1.0.1
* \date     23. Oct 2025
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
** VO           Vanessa Ongaro                                                **
** KP           Krina Panagou                                                 **
*******************************************************************************/


/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2025-08-27, VO:   [ATVCOMETLL-205] Initial version                 **
** V1.0.1: 2025-10-23, KP:   [ATVCOMETLL-217] Fixed formatting                **
*******************************************************************************/

#if !defined(_ISR_NVIC_IRQ3_HANDLER_H_)
#define _ISR_NVIC_IRQ3_HANDLER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "cpu_defines.h"
#include "isr_defines.h"
#include "isr.h"

#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN3_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN3_Pos))

#if ((defined NVIC_IRQ3_HANDLER_INT_CHECK) && (NVIC_IRQ3_HANDLER_INT_CHECK == 1))
extern uint8_t u8_interrupt_cnt_3;
#endif

#if ((UART2_TX_INT_EN == 1u) || \
     (UART2_TX_INT_EN == 1u))

void NVIC_IRQ3_Handler(void);

#endif /* ((UART2_RX_INT_EN == 1u) || \
           (UART2_TX_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN3_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN3_Pos)) */

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_ISR_NVIC_IRQ3_HANDLER_H_)*/
