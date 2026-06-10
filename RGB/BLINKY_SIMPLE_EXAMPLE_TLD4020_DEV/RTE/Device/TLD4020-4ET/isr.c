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
#include "isr.h"

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/

#if (CPU_HARDFAULT_INT_EN == 1u)
  void CPU_HARDFAULT_CALLBACK(void);
#endif /* (CPU_HARDFAULT_INT_EN == 1u) */

#if (MEM_DSRAM_INT_EN == 1u)
  void MEM_DSRAM_CALLBACK(void);
#endif /* Interrupt enabled */

#if (MEM_NVMDBE_INT_EN == 1u)
  void MEM_NVMDBE_CALLBACK(void);
#endif /* Interrupt enabled */

#if (SCU_NVMCLKRATIO_FAIL_INT_EN == 1u)
  void SCU_NVMCLKRATIO_FAIL_CALLBACK(void);
#endif /* Interrupt enabled */

#if (CPU_PENDSV_INT_EN == 1u)
  void CPU_PENDSV_CALLBACK(void);
#endif /* (CPU_PENDSV_INT_EN == 1u) */

#if (CPU_SVCALL_INT_EN == 1u)
  void CPU_SVCALL_CALLBACK(void);
#endif /* (CPU_SVCALL_INT_EN == 1u) */

#if (CPU_SYSTICK_INT_EN == 1u)
  void CPU_SYSTICK_CALLBACK(void);
#endif /* (CPU_SYSTICK_INT_EN == 1u) */

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/

volatile uint32_t u32_globTimestamp_ms = 0;

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if (CPU_HARDFAULT_INT_EN == 1u)
/** \brief HardFault ISR
 */
void HardFault_Handler(void)
{
  /*{AB8641FE-2AB1-4186-BD69-546047269353}*/
  CPU_HARDFAULT_CALLBACK();
}
#endif /* (CPU_HARDFAULT_INT_EN == 1u) */


#if ((MEM_DSRAM_INT_EN == 1u) || \
     (MEM_NVMDBE_INT_EN == 1u) || \
     (SCU_NVMCLKRATIO_FAIL_INT_EN == 1u))
/** \brief NMI ISR
 */
void NMI_Handler(void)
{
  /*{B8FB6C0E-8B2D-4277-A637-EBDB021F5BD4}*/
#if (MEM_DSRAM_INT_EN == 1u)
  if (MEM->NMICTRL.bit.DSRAM == MEM_NMICTRL_DSRAM_Enabled)
  {
    if (MEM->NMIS.bit.DSRAM == MEM_NMIS_DSRAM_Occurred)
    {
      MEM_DSRAM_CALLBACK();
      MEM->NMISC.bit.DSRAM = MEM_NMISC_DSRAM_Cleared;
    }
  }

#endif /* Interrupt enabled */
#if (MEM_NVMDBE_INT_EN == 1u)

  if (MEM->NMICTRL.bit.NVMDBE == MEM_NMICTRL_NVMDBE_Enabled)
  {
    if (MEM->NMIS.bit.NVMDBE == MEM_NMIS_NVMDBE_Occurred)
    {
      MEM_NVMDBE_CALLBACK();
      MEM->NMISC.bit.NVMDBE = MEM_NMISC_NVMDBE_Cleared;
    }
  }

#endif /* Interrupt enabled */
#if (SCU_NVMCLKRATIO_FAIL_INT_EN == 1u)

  if (SCU->NMIEN.bit.NVMCLKRATIO == SCU_NMIEN_NVMCLKRATIO_Enabled)
  {
    if (SCU->NMIS.bit.NVMCLKRATIO == SCU_NMIS_NVMCLKRATIO_Occurred)
    {
      SCU_NVMCLKRATIO_FAIL_CALLBACK();
      SCU->NMISC.bit.NVMCLKRATIO = SCU_NMISC_NVMCLKRATIO_Cleared;
    }
  }

#endif /* Interrupt enabled */
}
#endif /* ((MEM_DSRAM_INT_EN == 1u) || \
           (MEM_NVMDBE_INT_EN == 1u) || \
           (SCU_NVMCLKRATIO_FAIL_INT_EN == 1u)) */

#if (CPU_PENDSV_INT_EN == 1u)
/** \brief PendSV ISR
 */
void PendSV_Handler(void)
{
  /*{8A23FE5A-BCBF-490c-B863-220272F4A840}*/
  CPU_PENDSV_CALLBACK();
}
#endif /* (CPU_PENDSV_INT_EN == 1u) */

#if (CPU_SVCALL_INT_EN == 1u)
/** \brief SVCall ISR
 */
void SVC_Handler(void)
{
  /*{8A91E31E-22BD-46c4-9480-C1A4ACD7A745}*/
  CPU_SVCALL_CALLBACK();
}
#endif /* (CPU_SVCALL_INT_EN == 1u) */

/** \brief SysTick ISR
 */
void SysTick_Handler(void)
{
  /*{7EF6D967-03FE-4dcb-85BB-8CDD6890D604}*/
#if (CPU_SYSTICK_INT_EN == 1u)
  CPU_SYSTICK_CALLBACK();
#endif /* (CPU_SYSTICK_INT_EN == 1u) */
  u32_globTimestamp_ms++;
  PMU_countWatchdog();
}

