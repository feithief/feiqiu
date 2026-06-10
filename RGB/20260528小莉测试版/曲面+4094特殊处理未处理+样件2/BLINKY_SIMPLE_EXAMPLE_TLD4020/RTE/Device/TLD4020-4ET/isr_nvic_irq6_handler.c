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
#include "isr_nvic_irq6_handler.h"


/* Check if NVIC node 6 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN6_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN6_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ6_HANDLER_INT_CHECK) && (NVIC_IRQ6_HANDLER_INT_CHECK == 1))
  extern uint8_t u8_interrupt_cnt_irq6;
#endif

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (LDRV_TH_OVLD_CH0_INT_EN == 1u)
  void LDRV_TH_OVLD_CH0_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH1_INT_EN == 1u)
  void LDRV_TH_OVLD_CH1_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH2_INT_EN == 1u)
  void LDRV_TH_OVLD_CH2_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH3_INT_EN == 1u)
  void LDRV_TH_OVLD_CH3_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH4_INT_EN == 1u)
  void LDRV_TH_OVLD_CH4_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH5_INT_EN == 1u)
  void LDRV_TH_OVLD_CH5_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH6_INT_EN == 1u)
  void LDRV_TH_OVLD_CH6_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH7_INT_EN == 1u)
  void LDRV_TH_OVLD_CH7_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH8_INT_EN == 1u)
  void LDRV_TH_OVLD_CH8_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH9_INT_EN == 1u)
  void LDRV_TH_OVLD_CH9_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH10_INT_EN == 1u)
  void LDRV_TH_OVLD_CH10_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_TH_OVLD_CH11_INT_EN == 1u)
  void LDRV_TH_OVLD_CH11_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH0_INT_EN == 1u)
  void LDRV_S2G_CH0_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH1_INT_EN == 1u)
  void LDRV_S2G_CH1_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH2_INT_EN == 1u)
  void LDRV_S2G_CH2_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH3_INT_EN == 1u)
  void LDRV_S2G_CH3_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH4_INT_EN == 1u)
  void LDRV_S2G_CH4_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH5_INT_EN == 1u)
  void LDRV_S2G_CH5_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH6_INT_EN == 1u)
  void LDRV_S2G_CH6_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH7_INT_EN == 1u)
  void LDRV_S2G_CH7_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH8_INT_EN == 1u)
  void LDRV_S2G_CH8_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH9_INT_EN == 1u)
  void LDRV_S2G_CH9_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH10_INT_EN == 1u)
  void LDRV_S2G_CH10_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LDRV_S2G_CH11_INT_EN == 1u)
  void LDRV_S2G_CH11_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((LDRV_TH_OVLD_CH0_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH1_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH2_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH3_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH4_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH5_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH6_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH7_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH8_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH9_INT_EN == 1u)  || \
     (LDRV_TH_OVLD_CH10_INT_EN == 1u) || \
     (LDRV_TH_OVLD_CH11_INT_EN == 1u) || \
     (LDRV_S2G_CH0_INT_EN == 1u)      || \
     (LDRV_S2G_CH1_INT_EN == 1u)      || \
     (LDRV_S2G_CH2_INT_EN == 1u)      || \
     (LDRV_S2G_CH3_INT_EN == 1u)      || \
     (LDRV_S2G_CH4_INT_EN == 1u)      || \
     (LDRV_S2G_CH5_INT_EN == 1u)      || \
     (LDRV_S2G_CH6_INT_EN == 1u)      || \
     (LDRV_S2G_CH7_INT_EN == 1u)      || \
     (LDRV_S2G_CH8_INT_EN == 1u)      || \
     (LDRV_S2G_CH9_INT_EN == 1u)      || \
     (LDRV_S2G_CH10_INT_EN == 1u)     || \
     (LDRV_S2G_CH11_INT_EN == 1u))

/** \brief ISR from LDRV_OSP
 */
void NVIC_IRQ6_Handler(void)
{
  /*{BDDC669B-9E6E-4790-9DBE-FCAE4CEB4D77}*/
#if ((defined NVIC_IRQ6_HANDLER_INT_CHECK) && (NVIC_IRQ6_HANDLER_INT_CHECK == 1))
  u8_interrupt_cnt_irq6 = 0;
#endif
#if (LDRV_TH_OVLD_CH0_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH0_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH0_Msk)
    {
      LDRV_TH_OVLD_CH0_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH0_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH1_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH1_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH1_Msk)
    {
      LDRV_TH_OVLD_CH1_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH1_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH2_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH2_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH2_Msk)
    {
      LDRV_TH_OVLD_CH2_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH2_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH3_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH3_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH3_Msk)
    {
      LDRV_TH_OVLD_CH3_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH3_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH4_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH4_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH4_Msk)
    {
      LDRV_TH_OVLD_CH4_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH4_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH5_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH5_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH5_Msk)
    {
      LDRV_TH_OVLD_CH5_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH5_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH6_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH6_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH6_Msk)
    {
      LDRV_TH_OVLD_CH6_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH6_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH7_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH7_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH7_Msk)
    {
      LDRV_TH_OVLD_CH7_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH7_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH8_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH8_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH8_Msk)
    {
      LDRV_TH_OVLD_CH8_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH8_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH9_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH9_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH9_Msk)
    {
      LDRV_TH_OVLD_CH9_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH9_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH10_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH10_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH10_Msk)
    {
      LDRV_TH_OVLD_CH10_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH10_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_TH_OVLD_CH11_INT_EN == 1u)

  if (LDRV->IEN1.bit.TH_OVLD & LDRV_TH_OVLD_CH11_Msk)
  {
    if (LDRV->IS1.bit.TH_OVLD & LDRV_TH_OVLD_CH11_Msk)
    {
      LDRV_TH_OVLD_CH11_CALLBACK();
      LDRV->ISC1.bit.TH_OVLD = LDRV_TH_OVLD_CH11_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH0_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH0_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH0_Msk)
    {
      LDRV_S2G_CH0_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH0_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH1_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH1_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH1_Msk)
    {
      LDRV_S2G_CH1_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH1_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH2_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH2_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH2_Msk)
    {
      LDRV_S2G_CH2_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH2_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH3_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH3_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH3_Msk)
    {
      LDRV_S2G_CH3_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH3_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH4_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH4_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH4_Msk)
    {
      LDRV_S2G_CH4_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH4_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH5_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH5_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH5_Msk)
    {
      LDRV_S2G_CH5_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH5_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH6_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH6_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH6_Msk)
    {
      LDRV_S2G_CH6_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH6_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH7_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH7_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH7_Msk)
    {
      LDRV_S2G_CH7_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH7_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH8_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH8_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH8_Msk)
    {
      LDRV_S2G_CH8_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH8_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH9_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH9_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH9_Msk)
    {
      LDRV_S2G_CH9_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH9_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH10_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH10_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH10_Msk)
    {
      LDRV_S2G_CH10_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH10_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LDRV_S2G_CH11_INT_EN == 1u)

  if (LDRV->IEN1.bit.S2G & LDRV_S2G_CH11_Msk)
  {
    if (LDRV->IS1.bit.S2G & LDRV_S2G_CH11_Msk)
    {
      LDRV_S2G_CH11_CALLBACK();
      LDRV->ISC1.bit.S2G = LDRV_S2G_CH11_Msk;
#if ((defined NVIC_IRQ5_HANDLER_INT_CHECK) && (NVIC_IRQ5_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq5 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
}

#endif /* ((LDRV_TH_OVLD_CH0_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH1_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH2_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH3_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH4_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH5_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH6_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH7_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH8_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH9_INT_EN == 1u)  || \
           (LDRV_TH_OVLD_CH10_INT_EN == 1u) || \
           (LDRV_TH_OVLD_CH11_INT_EN == 1u) || \
           (LDRV_S2G_CH0_INT_EN == 1u)      || \
           (LDRV_S2G_CH1_INT_EN == 1u)      || \
           (LDRV_S2G_CH2_INT_EN == 1u)      || \
           (LDRV_S2G_CH3_INT_EN == 1u)      || \
           (LDRV_S2G_CH4_INT_EN == 1u)      || \
           (LDRV_S2G_CH5_INT_EN == 1u)      || \
           (LDRV_S2G_CH6_INT_EN == 1u)      || \
           (LDRV_S2G_CH7_INT_EN == 1u)      || \
           (LDRV_S2G_CH8_INT_EN == 1u)      || \
           (LDRV_S2G_CH9_INT_EN == 1u)      || \
           (LDRV_S2G_CH10_INT_EN == 1u)     || \
           (LDRV_S2G_CH11_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN6_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN6_Pos)) */
