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
#include "isr_nvic_irq3_handler.h"


/* Check if NVIC node 3 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN3_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN3_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ3_HANDLER_INT_CHECK) && (NVIC_IRQ3_HANDLER_INT_CHECK == 1))
  extern uint8_t u8_interrupt_cnt_irq3;
#endif

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (UART2_RX_INT_EN == 1u)
  void UART2_RX_CALLBACK(void);
#endif /* Interrupt enabled */

#if (UART2_TX_INT_EN == 1u)
  void UART2_TX_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((UART2_RX_INT_EN == 1u) || \
     (UART2_TX_INT_EN == 1u))

/** \brief ISR from UART2
 */
void NVIC_IRQ3_Handler(void)
{
#if (UART2_RX_INT_EN == 1u)

  if (UART2->UART_IEN.bit.RIEN == UART2_UART_IEN_RIEN_Enable)
  {
    if (UART2->UART_IS.bit.RI == UART2_UART_IS_RI_Occurred)
    {
      UART2_RX_CALLBACK();
      UART2->UART_ISC.bit.RICLR = UART2_UART_ISC_RICLR_Cleared;
#if ((defined NVIC_IRQ3_HANDLER_INT_CHECK) && (NVIC_IRQ3_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq3 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (UART2_TX_INT_EN == 1u)

  if (UART2->UART_IEN.bit.TIEN == UART2_UART_IEN_TIEN_Enable)
  {
    if (UART2->UART_IS.bit.TI == UART2_UART_IS_TI_Occurred)
    {
      UART2_TX_CALLBACK();
      UART2->UART_ISC.bit.TICLR = UART2_UART_ISC_TICLR_Cleared;
#if ((defined NVIC_IRQ3_HANDLER_INT_CHECK) && (NVIC_IRQ3_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq3 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
}

#endif /* ((UART2_RX_INT_EN == 1u) || \
           (UART2_TX_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN3_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN3_Pos)) */



