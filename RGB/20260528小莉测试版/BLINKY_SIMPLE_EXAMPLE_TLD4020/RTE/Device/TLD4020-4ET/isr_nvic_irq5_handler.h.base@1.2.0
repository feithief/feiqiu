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
* \file     isr_nvic_irq5_handler.h
*
* \brief    Interrupt Service Routine for NVIC IRQ5
*
* \version  V1.2.0
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
** VO           Vanessa Ongaro                                                **
** JO           Julia Ott                                                     **
*******************************************************************************/


/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-03-01, SL:   Added missing function declarations              **
** V1.1.0: 2023-06-27, SL:   Bugfix on End-Of-Conversion ISR handling;        **
**                            changed servicing order of LDRV ISRs            **
** V1.1.1: 2023-08-07, VO:   [ATVCOMETLL-119] Added int handling of           **
**                            PWM_SYNCH and OVLF                              **
** V1.1.2: 2023-09-19, VO:   [ATVCOMETLL-128] Added missing func declarations **
** V1.1.3: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.1.4: 2024-03-11, SL:   [ATVCOMETLL-155] Added per channel ISR for EOC   **
** V1.1.5: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.1.6: 2025-02-17, VO:   [ATVCOMETLL-183] Refactoring                     **
** V1.1.7: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.1.8: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.1.9: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
** V1.2.0: 2025-10-10, VO:   [ATVCOMETLL-217] Added support for TLD4030_12ES  **
*******************************************************************************/

#if !defined(_ISR_NVIC_IRQ5_HANDLER_H_)
#define _ISR_NVIC_IRQ5_HANDLER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "cpu_defines.h"
#include "isr_defines.h"
#include "isr.h"

/* LDRV 3 channel masks */
#define LDRV_EOC_CH0_Msk                           (0x1)
#define LDRV_EOC_CH1_Msk                           (0x2)
#define LDRV_EOC_CH2_Msk                           (0x4)
/* LDRV 4 channel masks */
#define LDRV_EOC_CH3_Msk                           (0x8)
/* LDRV 12 channel masks */
#define LDRV_EOC_CH4_Msk                           (0x10)
#define LDRV_EOC_CH5_Msk                           (0x20)
#define LDRV_EOC_CH6_Msk                           (0x40)
#define LDRV_EOC_CH7_Msk                           (0x80)
#define LDRV_EOC_CH8_Msk                           (0x100)
#define LDRV_EOC_CH9_Msk                           (0x200)
#define LDRV_EOC_CH10_Msk                          (0x400)
#define LDRV_EOC_CH11_Msk                          (0x800)

#ifndef LDRV_EOC_CH3_INT_EN
#define LDRV_EOC_CH3_INT_EN 0
#endif
#ifndef LDRV_EOC_CH4_INT_EN
#define LDRV_EOC_CH4_INT_EN 0
#endif
#ifndef LDRV_EOC_CH5_INT_EN
#define LDRV_EOC_CH5_INT_EN 0
#endif
#ifndef LDRV_EOC_CH6_INT_EN
#define LDRV_EOC_CH6_INT_EN 0
#endif
#ifndef LDRV_EOC_CH7_INT_EN
#define LDRV_EOC_CH7_INT_EN 0
#endif
#ifndef LDRV_EOC_CH8_INT_EN
#define LDRV_EOC_CH8_INT_EN 0
#endif
#ifndef LDRV_EOC_CH9_INT_EN
#define LDRV_EOC_CH9_INT_EN 0
#endif
#ifndef LDRV_EOC_CH10_INT_EN
#define LDRV_EOC_CH10_INT_EN 0
#endif
#ifndef LDRV_EOC_CH11_INT_EN
#define LDRV_EOC_CH11_INT_EN 0
#endif

#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos))

#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
extern uint8_t u8_interrupt_cnt_irq5;
#endif

#if ((LDRV_EOD_INT_EN == 1u)        || \
     (LDRV_EOC_CH0_INT_EN == 1u)    || \
     (LDRV_EOC_CH1_INT_EN == 1u)    || \
     (LDRV_EOC_CH2_INT_EN == 1u)    || \
     (LDRV_EOC_CH3_INT_EN == 1u)    || \
     (LDRV_EOC_CH4_INT_EN == 1u)    || \
     (LDRV_EOC_CH5_INT_EN == 1u)    || \
     (LDRV_EOC_CH6_INT_EN == 1u)    || \
     (LDRV_EOC_CH7_INT_EN == 1u)    || \
     (LDRV_EOC_CH8_INT_EN == 1u)    || \
     (LDRV_EOC_CH9_INT_EN == 1u)    || \
     (LDRV_EOC_CH10_INT_EN == 1u)   || \
     (LDRV_EOC_CH11_INT_EN == 1u)   || \
     (LDRV_OVLF_INT_EN == 1u)       || \
     (LDRV_PWM_SYNCH_INT_EN == 1u))

void NVIC_IRQ5_Handler(void);

#endif /* ((LDRV_EOD_INT_EN == 1u)        || \
           (LDRV_EOC_CH0_INT_EN == 1u)    || \
           (LDRV_EOC_CH1_INT_EN == 1u)    || \
           (LDRV_EOC_CH2_INT_EN == 1u)    || \
           (LDRV_EOC_CH3_INT_EN == 1u)    || \
           (LDRV_EOC_CH4_INT_EN == 1u)    || \
           (LDRV_EOC_CH5_INT_EN == 1u)    || \
           (LDRV_EOC_CH6_INT_EN == 1u)    || \
           (LDRV_EOC_CH7_INT_EN == 1u)    || \
           (LDRV_EOC_CH8_INT_EN == 1u)    || \
           (LDRV_EOC_CH9_INT_EN == 1u)    || \
           (LDRV_EOC_CH10_INT_EN == 1u)   || \
           (LDRV_EOC_CH11_INT_EN == 1u)   || \
           (LDRV_OVLF_INT_EN == 1u)       || \
           (LDRV_PWM_SYNCH_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos)) */

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_ISR_NVIC_IRQ5_HANDLER_H_)*/
