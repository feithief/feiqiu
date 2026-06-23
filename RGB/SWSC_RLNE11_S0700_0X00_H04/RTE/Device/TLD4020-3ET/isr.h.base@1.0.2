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
 * \file     isr.h
 *
 * \brief    Interrupt Service Routine
 *
 * \version  V1.0.2
 * \date     28. Sep 2023
 *
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
** V1.0.1: 2023-03-03, SL:   Added missing ISR callback definitions and fixed **
**                            typos                                           **
** V1.0.2: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
*******************************************************************************/
 
#if !defined(_ISR_H_)
#define _ISR_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "tld40xx.h"
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


/* LDRV 3 channel masks */
#define LDRV_EOC_CH0_Msk                           (0x1)
#define LDRV_EOC_CH1_Msk                           (0x2)
#define LDRV_EOC_CH2_Msk                           (0x4)

extern volatile uint32 u32_globTimestamp_ms; /* global counter variable for ms, can count ~1.5 months */

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
__STATIC_INLINE uint32 ISR_getGlobTimestamp(void)
{
	return u32_globTimestamp_ms;
}


#ifdef __cplusplus
}
#endif


#endif /*!defined(_ISR_H_)*/
