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
#include "isr_nvic_irq0_handler.h"


/* Check if NVIC node 0 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN0_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN0_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ0_HANDLER_INT_CHECK) && (NVIC_IRQ0_HANDLER_INT_CHECK == 1))
  extern uint8 u8_interrupt_cnt_irq0;
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



