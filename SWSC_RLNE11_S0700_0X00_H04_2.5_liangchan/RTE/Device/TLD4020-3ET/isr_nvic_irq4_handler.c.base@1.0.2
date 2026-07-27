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
#include "isr_nvic_irq4_handler.h"


/* Check if NVIC node 4 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN4_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN4_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ4_HANDLER_INT_CHECK) && (NVIC_IRQ4_HANDLER_INT_CHECK == 1))
  extern uint8 u8_interrupt_cnt_irq4;
#endif

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (ADC11_FW_EOC_INT_EN == 1u)
void ADC11_FW_EOC_CALLBACK(void);
#endif /* Interrupt enabled */

#if (ADC11_DTS_OVTEMP_INT_EN == 1u)
void ADC11_DTS_OVTEMP_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((ADC11_FW_EOC_INT_EN == 1u) || \
     (ADC11_DTS_OVTEMP_INT_EN == 1u))

/** \brief ISR from ADC
 */
void NVIC_IRQ4_Handler(void)
{
	/*{DBE859D3-DE5B-45e2-871A-1179BF6A18D7}*/

  #if ((defined NVIC_IRQ4_HANDLER_INT_CHECK) && (NVIC_IRQ4_HANDLER_INT_CHECK == 1))
  	u8_interrupt_cnt_irq4 = 0;
  #endif

  #if (ADC11_FW_EOC_INT_EN == 1u)
    if (ADC11->IEN.bit.ADC_FW_EOC == 1u)
    {
      if (ADC11->IS.bit.ADC_FW_EOC == 1u)
      {
        ADC11_FW_EOC_CALLBACK();
        ADC11->ISC.bit.ADC_FW_EOC = 1u;
        #if ((defined NVIC_IRQ4_HANDLER_INT_CHECK) && (NVIC_IRQ4_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq4 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */

  #if (ADC11_DTS_OVTEMP_INT_EN == 1u)
    if (ADC11->IEN.bit.DTS_OVT == 1u)
    {
      if (ADC11->IS.bit.DTS_OVT == 1u)
      {
        ADC11_DTS_OVTEMP_CALLBACK();
        ADC11->ISC.bit.DTS_OVT = 1u;
        #if ((defined NVIC_IRQ4_HANDLER_INT_CHECK) && (NVIC_IRQ4_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq4 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */
	
}

#endif /* ((ADC11_FW_EOC_INT_EN == 1u) || \
           (ADC11_DTS_OVTEMP_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN4_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN4_Pos)) */



