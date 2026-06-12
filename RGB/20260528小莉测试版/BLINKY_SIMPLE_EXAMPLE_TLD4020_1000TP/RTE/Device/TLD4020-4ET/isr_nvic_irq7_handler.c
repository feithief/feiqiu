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
#include "isr_nvic_irq7_handler.h"


/* Check if NVIC node 7 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN7_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN7_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
  extern uint8_t u8_interrupt_cnt_irq7;
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

#if (GPIO_IEN3_INT_EN == 1u)
  void GPIO_IEN3_CALLBACK(void);
#endif /* Interrupt enabled */

#if (GPIO_IEN4_INT_EN == 1u)
  void GPIO_IEN4_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((GPIO_IEN0_INT_EN == 1u) || \
     (GPIO_IEN1_INT_EN == 1u) || \
     (GPIO_IEN3_INT_EN == 1u) || \
     (GPIO_IEN4_INT_EN == 1u))

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
    if (GPIO->IS.bit.IS0 == GPIO_IS_IS0_Occurred)
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
    if (GPIO->IS.bit.IS1 == GPIO_IS_IS1_Occurred)
    {
      GPIO_IEN1_CALLBACK();
      GPIO->ISC.bit.ISC1 = GPIO_ISC_ISC1_Cleared;
#if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq7 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (GPIO_IEN3_INT_EN == 1u)

  if (GPIO->IEN.bit.IEN3 == GPIO_IEN_IEN3_Enabled)
  {
    if (GPIO->IS.bit.IS3 == GPIO_IS_IS3_Occurred)
    {
      GPIO_IEN3_CALLBACK();
      GPIO->ISC.bit.ISC3 = GPIO_ISC_ISC3_Cleared;
#if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq7 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (GPIO_IEN4_INT_EN == 1u)

  if (GPIO->IEN.bit.IEN4 == GPIO_IEN_IEN4_Enabled)
  {
    if (GPIO->IS.bit.IS4 == GPIO_IS_IS4_Occurred)
    {
      GPIO_IEN4_CALLBACK();
      GPIO->ISC.bit.ISC4 = GPIO_ISC_ISC4_Cleared;
#if ((defined NVIC_IRQ7_HANDLER_INT_CHECK) && (NVIC_IRQ7_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq7 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
}

#endif /* ((GPIO_IEN0_INT_EN == 1u) || \
           (GPIO_IEN1_INT_EN == 1u) || \
           (GPIO_IEN3_INT_EN == 1u) || \
           (GPIO_IEN4_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN7_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN7_Pos)) */



