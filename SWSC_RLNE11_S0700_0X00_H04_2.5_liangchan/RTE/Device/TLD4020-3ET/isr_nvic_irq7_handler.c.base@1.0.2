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
#include "isr_nvic_irq7_handler.h"


/* Check if NVIC node 7 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN7_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN7_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
  extern uint8 u8_interrupt_cnt_irq7;
#endif

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (GPIO_IEN0_INT_EN == 1u)
void GPIO_IEN0_CALLBACK(void);
#endif /* Interrupt enabled */

#if (GPIO_IEN1_INT_EN == 1u)
void GPIO_IEN1_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((GPIO_IEN0_INT_EN == 1u) || \
     (GPIO_IEN1_INT_EN == 1u))

/** \brief ISR from GPIO
 */
void NVIC_IRQ7_Handler(void)
{
	/*{27FF86B2-1E5D-4534-8842-45681C5A454E}*/

  #if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
  	u8_interrupt_cnt_irq7 = 0;
  #endif

  #if (GPIO_IEN0_INT_EN == 1u)
	if (GPIO->IEN.bit.IEN0 == GPIO_IEN_IEN0_Enabled)
    {
      if (GPIO->IS.bit.IS0 == GPIO_IS_IS0_Occured)
      {
        GPIO_IEN0_CALLBACK();
        GPIO->ISC.bit.ISC0 = GPIO_ISC_ISC0_Cleared;
        #if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq7 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */

  #if (GPIO_IEN1_INT_EN == 1u)
	if (GPIO->IEN.bit.IEN1 == GPIO_IEN_IEN1_Enabled)
    {
      if (GPIO->IS.bit.IS1 == GPIO_IS_IS1_Occured)
      {
        GPIO_IEN1_CALLBACK();
        GPIO->ISC.bit.ISC1 = GPIO_ISC_ISC1_Cleared;
        #if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq7 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */

}

#endif /* ((GPIO_IEN0_INT_EN == 1u) || \
           (GPIO_IEN1_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN7_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN7_Pos)) */



