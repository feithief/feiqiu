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
#include "isr_nvic_irq0_handler.h"


/* Check if NVIC node 0 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN0_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN0_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ0_HANDLER_INT_CHECK) && (NVIC_IRQ0_HANDLER_INT_CHECK == 1))
  extern uint8_t u8_interrupt_cnt_irq0;
#endif

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (PMU_VDD1V5_OV_INT_EN == 1u)
  void PMU_VDD1V5_OV_CALLBACK(void);
#endif /* Interrupt enabled */

#if (PMU_VDD1V5NVM_OV_INT_EN == 1u)
  void PMU_VDD1V5NVM_OV_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((PMU_VDD1V5_OV_INT_EN == 1u) || \
     (PMU_VDD1V5NVM_OV_INT_EN == 1u))

/** \brief ISR from PMU
 */
void NVIC_IRQ0_Handler(void)
{
  /*{D1634804-DD01-4e84-9B50-D0F8BC4D62B1}*/
#if ((defined NVIC_IRQ0_HANDLER_INT_CHECK) && (NVIC_IRQ0_HANDLER_INT_CHECK == 1))
  u8_interrupt_cnt_irq0 = 0;
#endif
#if (PMU_VDD1V5_OV_INT_EN == 1u)

  if (PMU->VDD1V5IEN.bit.OV == PMU_VDD1V5IEN_OV_Enabled)
  {
    if (PMU->VDD1V5IS.bit.OV == PMU_VDD1V5IS_OV_Occurred)
    {
      PMU_VDD1V5_OV_CALLBACK();
      PMU->VDD1V5ISC.bit.OV = PMU_VDD1V5SC_OV_Cleared;
#if ((defined NVIC_IRQ0_HANDLER_INT_CHECK) && (NVIC_IRQ0_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq0 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (PMU_VDD1V5NVM_OV_INT_EN == 1u)

  if (PMU->VDD1V5NVMIEN.bit.OV == PMU_VDD1V5NVMIEN_OV_Enabled)
  {
    if (PMU->VDD1V5NVMIS.bit.OV == PMU_VDD1V5NVMIS_OV_Occurred)
    {
      PMU_VDD1V5NVM_OV_CALLBACK();
      PMU->VDD1V5NVMISC.bit.OV = PMU_VDD1V5NVMSC_OV_Cleared;
#if ((defined NVIC_IRQ0_HANDLER_INT_CHECK) && (NVIC_IRQ0_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq0 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
}

#endif /* ((PMU_VDD1V5_OV_INT_EN == 1u) || \
           (PMU_VDD1V5NVM_OV_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN0_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN0_Pos)) */



