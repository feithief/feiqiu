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
#include "isr_nvic_irq5_handler.h"


/* Check if NVIC node 5 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
  extern uint8_t u8_interrupt_cnt_irq5;
#endif


/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (LDRV_EOD_INT_EN == 1u)
  void LDRV_EOD_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH0_INT_EN == 1u)
  void LDRV_EOC_CH0_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH1_INT_EN == 1u)
  void LDRV_EOC_CH1_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH2_INT_EN == 1u)
  void LDRV_EOC_CH2_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH3_INT_EN == 1u)
  void LDRV_EOC_CH3_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH4_INT_EN == 1u)
  void LDRV_EOC_CH4_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH5_INT_EN == 1u)
  void LDRV_EOC_CH5_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH6_INT_EN == 1u)
  void LDRV_EOC_CH6_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH7_INT_EN == 1u)
  void LDRV_EOC_CH7_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH8_INT_EN == 1u)
  void LDRV_EOC_CH8_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH9_INT_EN == 1u)
  void LDRV_EOC_CH9_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH10_INT_EN == 1u)
  void LDRV_EOC_CH10_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_EOC_CH11_INT_EN == 1u)
  void LDRV_EOC_CH11_CALLBACK(void);
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
     (LDRV_EOC_CH0_INT_EN == 1u)    || \
     (LDRV_EOC_CH1_INT_EN == 1u)    || \
     (LDRV_EOC_CH2_INT_EN == 1u)    || \
     (LDRV_EOC_CH3_INT_EN == 1u)    || \
     (LDRV_EOC_CH4_INT_EN == 1u)    || \
     (LDRV_EOC_CH5_INT_EN == 1u)    || \
     (LDRV_EOC_CH6_INT_EN == 1u)    || \
     (LDRV_EOC_CH7_INT_EN == 1u)    || \
     (LDRV_EOC_CH8_INT_EN == 1u)    || \
     (LDRV_EOC_CH9_INT_EN == 1u)    || \
     (LDRV_EOC_CH10_INT_EN == 1u)   || \
     (LDRV_EOC_CH11_INT_EN == 1u)   || \
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
#if (LDRV_EOC_CH0_INT_EN == 1u)

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

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH1_INT_EN == 1u)

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

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH2_INT_EN == 1u)

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
#if (LDRV_EOC_CH3_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH3_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH3_Msk)
    {
      LDRV_EOC_CH3_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH3_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH4_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH4_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH4_Msk)
    {
      LDRV_EOC_CH4_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH4_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH5_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH5_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH5_Msk)
    {
      LDRV_EOC_CH5_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH5_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH6_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH6_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH6_Msk)
    {
      LDRV_EOC_CH6_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH6_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH7_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH7_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH7_Msk)
    {
      LDRV_EOC_CH7_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH7_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH8_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH8_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH8_Msk)
    {
      LDRV_EOC_CH8_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH8_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH9_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH9_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH9_Msk)
    {
      LDRV_EOC_CH9_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH9_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH10_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH10_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH10_Msk)
    {
      LDRV_EOC_CH10_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH10_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_EOC_CH11_INT_EN == 1u)

  if (LDRV->IEN0.bit.EOC & LDRV_EOC_CH11_Msk)
  {
    if (LDRV->IS0.bit.EOC & LDRV_EOC_CH11_Msk)
    {
      LDRV_EOC_CH11_CALLBACK();
      LDRV->ISC0.bit.EOC = LDRV_EOC_CH11_Msk;
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
           (LDRV_EOC_CH0_INT_EN == 1u)    || \
           (LDRV_EOC_CH1_INT_EN == 1u)    || \
           (LDRV_EOC_CH2_INT_EN == 1u)    || \
           (LDRV_EOC_CH3_INT_EN == 1u)    || \
           (LDRV_EOC_CH4_INT_EN == 1u)    || \
           (LDRV_EOC_CH5_INT_EN == 1u)    || \
           (LDRV_EOC_CH6_INT_EN == 1u)    || \
           (LDRV_EOC_CH7_INT_EN == 1u)    || \
           (LDRV_EOC_CH8_INT_EN == 1u)    || \
           (LDRV_EOC_CH9_INT_EN == 1u)    || \
           (LDRV_EOC_CH10_INT_EN == 1u)   || \
           (LDRV_EOC_CH11_INT_EN == 1u)   || \
           (LDRV_OVLF_INT_EN == 1u)       || \
           (LDRV_PWM_SYNCH_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN5_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN5_Pos)) */



