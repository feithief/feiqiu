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
#include "isr_nvic_irq6_handler.h"


/* Check if NVIC node 6 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN6_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN6_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ6_HANDLER_INT_CHECK) && (NVIC_IRQ6_HANDLER_INT_CHECK == 1))
  extern uint8 u8_interrupt_cnt_irq6;
#endif

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (LDRV_TH_OVLD_INT_EN == 1u)
void LDRV_TH_OVLD_CH0_CALLBACK(void);
void LDRV_TH_OVLD_CH1_CALLBACK(void);
void LDRV_TH_OVLD_CH2_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_INT_EN == 1u)
void LDRV_S2G_CH0_CALLBACK(void);
void LDRV_S2G_CH1_CALLBACK(void);
void LDRV_S2G_CH2_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((LDRV_TH_OVLD_INT_EN == 1u) || \
     (LDRV_S2G_INT_EN == 1u))

/** \brief ISR from LDRV_OSP
 */
void NVIC_IRQ6_Handler(void)
{
	/*{BDDC669B-9E6E-4790-9DBE-FCAE4CEB4D77}*/

  #if ((defined NVIC_IRQ6_HANDLER_INT_CHECK) && (NVIC_IRQ6_HANDLER_INT_CHECK == 1))
  	u8_interrupt_cnt_irq6 = 0;
  #endif

  #if (LDRV_TH_OVLD_INT_EN == 1u)
    if (LDRV->IEN1.bit.TH_OVLD == LDRV_EOC_CH0_Msk)
    {
      if (LDRV->IS1.bit.TH_OVLD == LDRV_EOC_CH0_Msk)
      {
        LDRV_TH_OVLD_CH0_CALLBACK();
        LDRV->ISC1.bit.TH_OVLD = LDRV_EOC_CH0_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }

    if (LDRV->IEN1.bit.TH_OVLD == LDRV_EOC_CH1_Msk)
    {
      if (LDRV->IS1.bit.TH_OVLD == LDRV_EOC_CH1_Msk)
      {
        LDRV_TH_OVLD_CH1_CALLBACK();
        LDRV->ISC1.bit.TH_OVLD = LDRV_EOC_CH1_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }

    if (LDRV->IEN1.bit.TH_OVLD == LDRV_EOC_CH2_Msk)
    {
      if (LDRV->IS1.bit.TH_OVLD == LDRV_EOC_CH2_Msk)
      {
        LDRV_TH_OVLD_CH2_CALLBACK();
        LDRV->ISC1.bit.TH_OVLD = LDRV_EOC_CH2_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */

  #if (LDRV_S2G_INT_EN == 1u)
    if (LDRV->IEN1.bit.S2G == LDRV_EOC_CH0_Msk)
    {
      if (LDRV->IS1.bit.S2G == LDRV_EOC_CH0_Msk)
      {
        LDRV_S2G_CH0_CALLBACK();
        LDRV->ISC1.bit.S2G = LDRV_EOC_CH0_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }

    if (LDRV->IEN1.bit.S2G == LDRV_EOC_CH1_Msk)
    {
      if (LDRV->IS1.bit.S2G == LDRV_EOC_CH1_Msk)
      {
        LDRV_S2G_CH1_CALLBACK();
        LDRV->ISC1.bit.S2G = LDRV_EOC_CH1_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }

    if (LDRV->IEN1.bit.S2G == LDRV_EOC_CH2_Msk)
    {
      if (LDRV->IS1.bit.S2G == LDRV_EOC_CH2_Msk)
      {
        LDRV_S2G_CH2_CALLBACK();
        LDRV->ISC1.bit.S2G = LDRV_EOC_CH2_Msk;
        #if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
          u8_interrupt_cnt_irq5 += 1u;
        #endif
      }
    }
  #endif /* Interrupt enabled */
	
}

#endif /* ((LDRV_TH_OVLD_INT_EN == 1u) || \
           (LDRV_S2G_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN6_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN6_Pos)) */
