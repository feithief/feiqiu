/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

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
  extern uint8_t u8_interrupt_cnt_irq4;
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



