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
 * \file     isr_nvic_irq5_handler.h
 *
 * \brief    Interrupt Service Routine for NVIC IRQ5
 *
 * \version  V1.1.3
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
*******************************************************************************/
 
#if !defined(_ISR_NVIC_IRQ5_HANDLER_H_)
#define _ISR_NVIC_IRQ5_HANDLER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "tld40xx.h"
#include "isr_defines.h"
#include "isr.h"
#include "cpu_defines.h"

#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos))

#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
  extern uint8 u8_interrupt_cnt_irq5;
#endif

#if ((LDRV_EOD_INT_EN == 1u)        || \
     (LDRV_EOC_INT_EN == 1u)        || \
	 (LDRV_OVLF_INT_EN == 1u)       || \
	 (LDRV_PWM_SYNCH_INT_EN == 1u))

void NVIC_IRQ5_Handler(void);

#endif /* ((LDRV_EOD_INT_EN == 1u)        || \
           (LDRV_EOC_INT_EN == 1u)        || \
	       (LDRV_OVLF_INT_EN == 1u)       || \
	       (LDRV_PWM_SYNCH_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos)) */

#ifdef __cplusplus
}
#endif


#endif /*!defined(_ISR_NVIC_IRQ5_HANDLER_H_)*/
