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

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "isr_nvic_irq5_handler.h"


/* Check if NVIC node 5 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
  extern uint8 u8_interrupt_cnt_irq5;
#endif


/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (LDRV_EOD_INT_EN == 1u)
void LDRV_EOD_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_INT_EN == 1u)
void LDRV_EOC_CH0_CALLBACK(void);
void LDRV_EOC_CH1_CALLBACK(void);
void LDRV_EOC_CH2_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_OVLF_INT_EN == 1u)
void LDRV_OVLF_CALLBACK(void);
#endif /* Interrupt enabled */ 

#if (LDRV_PWM_SYNCH_INT_EN == 1u)
void LDRV_PWM_SYNCH_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((LDRV_EOD_INT_EN == 1u)        || \
     (LDRV_EOC_INT_EN == 1u)        || \
	 (LDRV_OVLF_INT_EN == 1u)       || \
	 (LDRV_PWM_SYNCH_INT_EN == 1u))

/** \brief ISR from LDRV_DIAG
 */
void NVIC_IRQ5_Handler(void)
{
  /*{D143E52C-936F-4658-BFEA-3DD560D1E35B}*/

  #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
  	u8_interrupt_cnt_irq5 = 0;
  #endif

  #if (LDRV_EOC_INT_EN == 1u)
	if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH0_Msk)
    {
      if (LDRV->IS0.bit.EOC & LDRV_EOC_CH0_Msk)
      {
        LDRV_EOC_CH0_CALLBACK();
        LDRV->ISC0.bit.EOC = LDRV_EOC_CH0_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }
  
  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH1_Msk)
    {
      if (LDRV->IS0.bit.EOC & LDRV_EOC_CH1_Msk)
      {
        LDRV_EOC_CH1_CALLBACK();
        LDRV->ISC0.bit.EOC = LDRV_EOC_CH1_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH2_Msk)
    {
      if (LDRV->IS0.bit.EOC & LDRV_EOC_CH2_Msk)
      {
        LDRV_EOC_CH2_CALLBACK();
        LDRV->ISC0.bit.EOC = LDRV_EOC_CH2_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */

  #if (LDRV_EOD_INT_EN == 1u)
	if (LDRV->IEN0.bit.EOD == LDRV_IEN0_EOD_Enable)
    {
      if (LDRV->IS0.bit.EOD == LDRV_IS0_EOD_Occurred)
      {
        LDRV_EOD_CALLBACK();
        LDRV->ISC0.bit.EOD = LDRV_ISC0_EOD_Cleared;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */
  
  #if (LDRV_OVLF_INT_EN == 1u)
	if (LDRV->IEN0.bit.OVLF == LDRV_IEN0_OVLF_Enable)
    {
      if (LDRV->IS0.bit.OVLF == LDRV_IS0_OVLF_Occurred)
      {
        LDRV_OVLF_CALLBACK();
        LDRV->ISC0.bit.OVLF = LDRV_ISC0_OVLF_Cleared;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */
  
  #if (LDRV_PWM_SYNCH_INT_EN == 1u)
	if (LDRV->IEN0.bit.PWM_SYNCH == LDRV_IEN0_PWM_SYNCH_Enable)
    {
      if (LDRV->IS0.bit.PWM_SYNCH == LDRV_IS0_PWM_SYNCH_Occurred)
      {
        LDRV_PWM_SYNCH_CALLBACK();
        LDRV->ISC0.bit.PWM_SYNCH = LDRV_ISC0_PWM_SYNCH_Cleared;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */
	
}

#endif /* ((LDRV_EOD_INT_EN == 1u)        || \
           (LDRV_EOC_INT_EN == 1u)        || \
	       (LDRV_OVLF_INT_EN == 1u)       || \
	       (LDRV_PWM_SYNCH_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos)) */



