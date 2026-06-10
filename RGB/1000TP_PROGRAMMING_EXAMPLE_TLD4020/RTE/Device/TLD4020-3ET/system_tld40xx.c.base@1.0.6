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

#include "device.h"
#include "cpu_defines.h"

/*******************************************************************************
**                        Global Constant Declarations                        **
*******************************************************************************/

/*---------------------------------------------------------------------------
  Define clocks
 *---------------------------------------------------------------------------*/
#define SYSTEM_CLOCK    (20000000UL)


/*******************************************************************************
**                          Global Variable Declarations                      **
*******************************************************************************/

/*---------------------------------------------------------------------------
  System Core Clock Variable
 *---------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;  /* System Clock Frequency (Core Clock)*/


/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Update system core clock variable
 */
void SystemCoreClockUpdate (void)
{
#define  CLKSRC0_HZ   40000000UL
  uint32_t u32_clkScr1_Hz;
  uint32_t u32_freqSys0_Hz;
  uint8_t u8_scuCpuPresc;

  if (OSC->FCLKDIV.bit.EN == (uint8_t)OSC_FCLKDIV_EN_Disabled ||
      OSC->FCLKDIV.bit.PDIV == 0 ||
      OSC->FCLKDIV.bit.QDIV == 0 ||
      OSC->FCLKDIV.bit.PDIV >= OSC->FCLKDIV.bit.QDIV )
  {
    u32_clkScr1_Hz = CLKSRC0_HZ;
  }
  else
  {
    u32_clkScr1_Hz = (uint32_t)(((uint64_t)CLKSRC0_HZ * (uint64_t)(OSC->FCLKDIV.bit.PDIV) + (OSC->FCLKDIV.bit.QDIV >> 1UL)) / OSC->FCLKDIV.bit.QDIV);
  }

  u32_freqSys0_Hz = (SCU->CLKSEL.bit.SYS0 == (uint8_t)SCU_CLKSEL_SYS0_CLKSRC0) ? CLKSRC0_HZ : u32_clkScr1_Hz;

  switch (SCU->CLKCTRL.bit.CPUDIV)
  {
    default:
    case SCU_CLKCTRL_CPUDIV_DIV1 :
      u8_scuCpuPresc = 1;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV2 :
      u8_scuCpuPresc = 2;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV3 :
      u8_scuCpuPresc = 3;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV4 :
      u8_scuCpuPresc = 4;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV6 :
      u8_scuCpuPresc = 6;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV8 :
      u8_scuCpuPresc = 8;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV16:
      u8_scuCpuPresc = 16;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV20:
      u8_scuCpuPresc = 20;
      break;
  }

  SystemCoreClock = (u32_freqSys0_Hz + (u8_scuCpuPresc >> 1U)) / u8_scuCpuPresc;
}

/** \brief Initialize the system
 */
void SystemInit(void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
  /* Update VTOR register to point to 0x12000000 (location of vector table) */
  SCB->VTOR = UC_FLASH_START;
#endif
  /* Initialize clock related settings*/
  (void)SCU_init();
  /* Initialize SysTick */
  (void)CPU_initSysTick(CPU_SYST_RVR);
  /* Initialize Watchdog */
  (void)PMU_initWatchdog();
  SystemCoreClockUpdate();
}

