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
* \file     isr.h
*
* \brief    Interrupt Service Routine
*
* \version  V1.0.6
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
** V1.0.1: 2023-03-03, SL:   Added missing ISR callback definitions and fixed **
**                            typos                                           **
** V1.0.2: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.3: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.4: 2025-02-17, VO:   [ATVCOMETLL-183] Refactoring                     **
** V1.0.5: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.6: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

#if !defined(_ISR_H_)
#define _ISR_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "isr_defines.h"
#include "pmu.h"

/* =======================================================  NVIC_ISER  ======================================================= */
#define CPU_NVIC_ISER0_SETENA_IRQEN0_Pos          (0UL)                     /*!< IRQEN0 (Bit 0)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN0_Msk          (0x1UL)                   /*!< IRQEN0 (Bitfield-Mask: 0x01)                          */
#define CPU_NVIC_ISER0_SETENA_IRQEN1_Pos          (1UL)                     /*!< IRQEN1 (Bit 1)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN1_Msk          (0x2UL)                   /*!< IRQEN1 (Bitfield-Mask: 0x01)                          */
#define CPU_NVIC_ISER0_SETENA_IRQEN2_Pos          (2UL)                     /*!< IRQEN2 (Bit 2)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN2_Msk          (0x4UL)                   /*!< IRQEN2 (Bitfield-Mask: 0x01)                          */
#define CPU_NVIC_ISER0_SETENA_IRQEN3_Pos          (3UL)                     /*!< IRQEN3 (Bit 3)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN3_Msk          (0x8UL)                   /*!< IRQEN3 (Bitfield-Mask: 0x01)                          */
#define CPU_NVIC_ISER0_SETENA_IRQEN4_Pos          (4UL)                     /*!< IRQEN4 (Bit 4)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN4_Msk          (0x10UL)                  /*!< IRQEN4 (Bitfield-Mask: 0x01)                          */
#define CPU_NVIC_ISER0_SETENA_IRQEN5_Pos          (5UL)                     /*!< IRQEN5 (Bit 5)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN5_Msk          (0x20UL)                  /*!< IRQEN5 (Bitfield-Mask: 0x01)                          */
#define CPU_NVIC_ISER0_SETENA_IRQEN6_Pos          (6UL)                     /*!< IRQEN6 (Bit 6)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN6_Msk          (0x40UL)                  /*!< IRQEN6 (Bitfield-Mask: 0x01)                          */
#define CPU_NVIC_ISER0_SETENA_IRQEN7_Pos          (7UL)                     /*!< IRQEN7 (Bit 7)                                        */
#define CPU_NVIC_ISER0_SETENA_IRQEN7_Msk          (0x80UL)                  /*!< IRQEN7 (Bitfield-Mask: 0x01)                          */


extern volatile uint32_t u32_globTimestamp_ms; /* global counter variable for ms, can count ~1.5 months */

#if (CPU_HARDFAULT_INT_EN == 1u)
void HardFault_Handler(void);
#endif /* (CPU_HARDFAULT_INT_EN == 1u) */

#if ((MEM_DSRAM_INT_EN == 1u) || \
     (MEM_NVMDBE_INT_EN == 1u) || \
     (SCU_NVMCLKRATIO_FAIL_INT_EN == 1u))
void NMI_Handler(void);
#endif /* ((MEM_DSRAM_INT_EN == 1u) || \
            (MEM_NVMDBE_INT_EN == 1u) || \
            (SCU_NVMCLKRATIO_FAIL_INT_EN == 1u)) */

#if (CPU_PENDSV_INT_EN == 1)
void PendSV_Handler(void);
#endif /* (CPU_PENDSV_INT_EN == 1) */

#if (CPU_SVCALL_INT_EN == 1)
void SVC_Handler(void);
#endif /* (CPU_SVCALL_INT_EN == 1) */

void SysTick_Handler(void);

/*******************************************************************************
**                        Global Function Definitions                         **
*******************************************************************************/
/** \brief Get the global timestamp value
 *
 * \return global timestamp value
 */
__STATIC_INLINE uint32_t ISR_getGlobTimestamp(void)
{
  return u32_globTimestamp_ms;
}


#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_ISR_H_)*/
