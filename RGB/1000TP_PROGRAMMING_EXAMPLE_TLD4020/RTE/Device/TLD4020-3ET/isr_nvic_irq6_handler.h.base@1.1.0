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
* \file     isr_nvic_irq6_handler.h
*
* \brief    Interrupt Service Routine for NVIC IRQ6
*
* \version  V1.1.0
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
** V1.0.3: 2024-03-11, SL:   [ATVCOMETLL-155] Added per channel ISR for S2G,  **
**                           and TH_OVLD                                      **
** V1.0.4: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.5: 2025-02-17, VO:   [ATVCOMETLL-183] Refactoring                     **
** V1.0.6: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.7: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.0.8: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
** V1.1.0: 2025-10-10, VO:   [ATVCOMETLL-217] Added support for TLD4030_12ES  **
*******************************************************************************/

#if !defined(_ISR_NVIC_IRQ6_HANDLER_H_)
#define _ISR_NVIC_IRQ6_HANDLER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "cpu_defines.h"
#include "isr_defines.h"
#include "isr.h"

/* LDRV 3 channel masks */
#define LDRV_TH_OVLD_CH0_Msk                           (0x1)
#define LDRV_TH_OVLD_CH1_Msk                           (0x2)
#define LDRV_TH_OVLD_CH2_Msk                           (0x4)
#define LDRV_S2G_CH0_Msk                               (0x1)
#define LDRV_S2G_CH1_Msk                               (0x2)
#define LDRV_S2G_CH2_Msk                               (0x4)
/* LDRV 4 channel masks */
#define LDRV_TH_OVLD_CH3_Msk                           (0x8)
#define LDRV_S2G_CH3_Msk                               (0x8)
/* LDRV 12 channel masks */
#define LDRV_TH_OVLD_CH4_Msk                           (0x10)
#define LDRV_TH_OVLD_CH5_Msk                           (0x20)
#define LDRV_TH_OVLD_CH6_Msk                           (0x40)
#define LDRV_TH_OVLD_CH7_Msk                           (0x80)
#define LDRV_TH_OVLD_CH8_Msk                           (0x100)
#define LDRV_TH_OVLD_CH9_Msk                           (0x200)
#define LDRV_TH_OVLD_CH10_Msk                          (0x400)
#define LDRV_TH_OVLD_CH11_Msk                          (0x800)
#define LDRV_S2G_CH4_Msk                               (0x10)
#define LDRV_S2G_CH5_Msk                               (0x20)
#define LDRV_S2G_CH6_Msk                               (0x40)
#define LDRV_S2G_CH7_Msk                               (0x80)
#define LDRV_S2G_CH8_Msk                               (0x100)
#define LDRV_S2G_CH9_Msk                               (0x200)
#define LDRV_S2G_CH10_Msk                              (0x400)
#define LDRV_S2G_CH11_Msk                              (0x800)

#ifndef LDRV_TH_OVLD_CH3_INT_EN
#define LDRV_TH_OVLD_CH3_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH4_INT_EN
#define LDRV_TH_OVLD_CH4_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH5_INT_EN
#define LDRV_TH_OVLD_CH5_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH6_INT_EN
#define LDRV_TH_OVLD_CH6_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH7_INT_EN
#define LDRV_TH_OVLD_CH7_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH8_INT_EN
#define LDRV_TH_OVLD_CH8_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH9_INT_EN
#define LDRV_TH_OVLD_CH9_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH10_INT_EN
#define LDRV_TH_OVLD_CH10_INT_EN 0
#endif
#ifndef LDRV_TH_OVLD_CH11_INT_EN
#define LDRV_TH_OVLD_CH11_INT_EN 0
#endif
#ifndef LDRV_S2G_CH3_INT_EN
#define LDRV_S2G_CH3_INT_EN 0
#endif
#ifndef LDRV_S2G_CH4_INT_EN
#define LDRV_S2G_CH4_INT_EN 0
#endif
#ifndef LDRV_S2G_CH5_INT_EN
#define LDRV_S2G_CH5_INT_EN 0
#endif
#ifndef LDRV_S2G_CH6_INT_EN
#define LDRV_S2G_CH6_INT_EN 0
#endif
#ifndef LDRV_S2G_CH7_INT_EN
#define LDRV_S2G_CH7_INT_EN 0
#endif
#ifndef LDRV_S2G_CH8_INT_EN
#define LDRV_S2G_CH8_INT_EN 0
#endif
#ifndef LDRV_S2G_CH9_INT_EN
#define LDRV_S2G_CH9_INT_EN 0
#endif
#ifndef LDRV_S2G_CH10_INT_EN
#define LDRV_S2G_CH10_INT_EN 0
#endif
#ifndef LDRV_S2G_CH11_INT_EN
#define LDRV_S2G_CH11_INT_EN 0
#endif

#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN6_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN6_Pos))

#if ((defined NVIC_IRQ6_HANDLER_INT_CHECK) && (NVIC_IRQ6_HANDLER_INT_CHECK == 1))
extern uint8_t u8_interrupt_cnt_irq6;
#endif

#if ((LDRV_TH_OVLD_CH0_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH1_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH2_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH3_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH4_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH5_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH6_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH7_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH8_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH9_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH10_INT_EN == 1u) || \
     (LDRV_TH_OVLD_CH11_INT_EN == 1u) || \
     (LDRV_S2G_CH0_INT_EN == 1u)      || \
     (LDRV_S2G_CH1_INT_EN == 1u)      || \
     (LDRV_S2G_CH2_INT_EN == 1u)      || \
     (LDRV_S2G_CH3_INT_EN == 1u)      || \
     (LDRV_S2G_CH4_INT_EN == 1u)      || \
     (LDRV_S2G_CH5_INT_EN == 1u)      || \
     (LDRV_S2G_CH6_INT_EN == 1u)      || \
     (LDRV_S2G_CH7_INT_EN == 1u)      || \
     (LDRV_S2G_CH8_INT_EN == 1u)      || \
     (LDRV_S2G_CH9_INT_EN == 1u)      || \
     (LDRV_S2G_CH10_INT_EN == 1u)     || \
     (LDRV_S2G_CH11_INT_EN == 1u))

void NVIC_IRQ6_Handler(void);

#endif /* ((LDRV_TH_OVLD_CH0_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH1_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH2_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH3_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH4_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH5_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH6_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH7_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH8_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH9_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH10_INT_EN == 1u) || \
           (LDRV_TH_OVLD_CH11_INT_EN == 1u) || \
           (LDRV_S2G_CH0_INT_EN == 1u)      || \
           (LDRV_S2G_CH1_INT_EN == 1u)      || \
           (LDRV_S2G_CH2_INT_EN == 1u)      || \
           (LDRV_S2G_CH3_INT_EN == 1u)      || \
           (LDRV_S2G_CH4_INT_EN == 1u)      || \
           (LDRV_S2G_CH5_INT_EN == 1u)      || \
           (LDRV_S2G_CH6_INT_EN == 1u)      || \
           (LDRV_S2G_CH7_INT_EN == 1u)      || \
           (LDRV_S2G_CH8_INT_EN == 1u)      || \
           (LDRV_S2G_CH9_INT_EN == 1u)      || \
           (LDRV_S2G_CH10_INT_EN == 1u)     || \
           (LDRV_S2G_CH11_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN6_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN6_Pos)) */

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_ISR_NVIC_IRQ6_HANDLER_H_)*/
