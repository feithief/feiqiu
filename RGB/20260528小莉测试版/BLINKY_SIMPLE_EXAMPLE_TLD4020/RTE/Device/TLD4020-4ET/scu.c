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
#include "scu.h"

/*******************************************************************************
**                         Private Macro Declarations                         **
*******************************************************************************/
#define SYSTICK_DELAY_THRESHOLD (SCU_F_SYSTICK * 5U)

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Set the device into Sleep Mode
 */
void SCU_enterSleepMode(void)
{
  /*{FF88B454-884E-43e9-8A99-FC4B2D646A82}*/
  SCU->PMCTRL.bit.SLEEP = 1u;
  CMSIS_WFE();
  CMSIS_WFE();
  CMSIS_WFE();
}

/** \brief Initialize the SCU module
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t SCU_init(void)
{
  /*{DF41A130-00C6-4c2b-AE63-45A7DE5C1026}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;  /* config can not be disabled in CW */
  SCU->CLKCTRL.bit.FILTDIV = (uint8_t) (((uint32_t)SCU_CLKCTRL & SCU_CLKCTRL_FILTDIV_Msk) >>  SCU_CLKCTRL_FILTDIV_Pos);
  SCU->SYSTCKCTRL.reg = (uint32_t) SCU_SYSTCKCTRL;
  SCU->NMIEN.reg = (uint32_t) SCU_NMIEN;
  /* SCU->RAMBISTCFG.reg = (uint32_t) SCU_RAMBISTCFG; */
  return u8_returnCode;
}

/** \brief Delays the regular program execution by a given number of Microseconds.
 *         The function returns when the given time has elapsed.
 *         Too small delay times are being falsified by execution time
 *         of the function itself.
 *
 * \warning Handle this function with care, as the watchdog will not be serviced
 *          during the delay time.
 * \warning The user has to take care of WDT1/WDT service by themselves.
 *
 * \param u32_time_us Delay time in Microseconds
 *
 * \brief <b>Example</b><br>
 * \brief This example sets a delay of 100 us.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     SCU_delay_us(100);
 * }
 * ~~~~~~~~~~~~~~~
 */
void SCU_delay_us(uint32_t u32_time_us)
{
  uint32_t u32_systickTargetVal;
  uint32_t u32_systickVal;
  uint32_t u32_delayCnt;
  uint32_t u32_systickCur;
  uint32_t u32_systickRelVal;
  /* Get current systick value */
  u32_systickRelVal = SCU_getSysTickRelVal();

  /* Adapt systick value into range SYSTICK_DELAY_THRESHOLD...reload value-SYSTICK_DELAY_THRESHOLD to avoid getting stuck in a while, wait in case needed */
  do
  {
    u32_systickVal = SCU_getSysTickCntVal();
  }
  while ((u32_systickVal < SYSTICK_DELAY_THRESHOLD) || (u32_systickVal > (u32_systickRelVal - SYSTICK_DELAY_THRESHOLD)));

  while (u32_time_us >= (uint32_t)1000)
  {
    /* Wait for underflow */
    while (SCU_getSysTickCntVal() < u32_systickVal)
    {}

    /* Wait target underflowed */
    while (SCU_getSysTickCntVal() > u32_systickVal)
    {}

    u32_time_us -= (uint32_t)1000;
  }

  u32_delayCnt = u32_time_us * (uint32_t)SCU_F_SYSTICK;
  u32_systickVal = SCU_getSysTickCntVal();

  if (u32_systickVal >= u32_delayCnt)
  {
    u32_systickTargetVal = u32_systickVal - u32_delayCnt;

    /* Wait underflow with 1 countdown detection */
    do
    {
      u32_systickCur = SCU_getSysTickCntVal();
    }
    while ((u32_systickCur > u32_systickTargetVal) && (u32_systickCur < u32_systickVal));
  }
  else
  {
    u32_systickTargetVal = SCU_getSysTickRelVal() - (u32_delayCnt - u32_systickVal);

    /* Wait for underflow */
    while (SCU_getSysTickCntVal() < u32_systickVal)
    {}

    /* Wait target underflowed */
    while (SCU_getSysTickCntVal() > u32_systickTargetVal)
    {}
  }

  return;
}

/** \brief Get clock source 1 in kHz
 *
 * \return Clock source 1 value
 */
uint32_t SCU_getClkSrc1_kHz(void)
{
  uint32_t clock_source_1;
  uint32_t p_div;
  uint32_t q_div;

  if (OSC->FCLKDIV.bit.EN == 1u)
  {
    p_div = OSC->FCLKDIV.bit.PDIV;
    q_div = OSC->FCLKDIV.bit.QDIV;

    if ((p_div < q_div) && (p_div != 0u))
    {
      clock_source_1 = ((uint32_t)SCU_CLKSRC0 * p_div) / q_div;
    }
    else
    {
      clock_source_1 = (uint32_t)SCU_CLKSRC0;
    }
  }
  else
  {
    clock_source_1 = (uint32_t)SCU_CLKSRC0;
  }

  return clock_source_1;
}

/** \brief Get SYS0 clock in kHz
 *
 * \return SYS0 clock value
 */
uint32_t SCU_getSys0Clk_kHz(void)
{
  uint32_t sys_0_clk;

  if (SCU->CLKSEL.bit.SYS0 == 1u)
  {
    sys_0_clk = SCU_getClkSrc1_kHz();
  }
  else
  {
    sys_0_clk = SCU_CLKSRC0;
  }

  return sys_0_clk;
}

/** \brief Get CPU clock in kHz
 *
 * \return CPU clock value
 */
uint32_t SCU_getCpuClk_kHz(void)
{
  uint32_t cpu_clk;

  switch (SCU->CLKCTRL.bit.CPUDIV)
  {
    default:
      cpu_clk = SCU_getSys0Clk_kHz();
      break;

    case SCU_CLKCTRL_CPUDIV_DIV2:
      cpu_clk = SCU_getSys0Clk_kHz() / 2u;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV3:
      cpu_clk = SCU_getSys0Clk_kHz() / 3u;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV4:
      cpu_clk = SCU_getSys0Clk_kHz() / 4u;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV6:
      cpu_clk = SCU_getSys0Clk_kHz() / 6u;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV8:
      cpu_clk = SCU_getSys0Clk_kHz() / 8u;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV16:
      cpu_clk = SCU_getSys0Clk_kHz() / 16u;
      break;

    case SCU_CLKCTRL_CPUDIV_DIV20:
      cpu_clk = SCU_getSys0Clk_kHz() / 20u;
      break;
  }

  return cpu_clk;
}

/** \brief Get Filter clock in kHz
 *
 * \return Filter clock value
 */
uint32_t SCU_getFiltClk_kHz(void)
{
  uint32_t filt_clk;
  filt_clk = SCU_getSys0Clk_kHz() / ((uint32_t)SCU->CLKCTRL.bit.FILTDIV + 1u);
  return filt_clk;
}

/** \brief Get Systick clock in kHz
 *
 * \return Systick clock value
 */
uint32_t SCU_getSystickClk_kHz(void)
{
  uint32_t systick_clk;
  uint32_t systick_clk_div;
  systick_clk_div = ((uint32_t)SCU->SYSTCKCTRL.bit.SYSTCKDIV < 40u) ? 40u : (uint32_t)SCU->SYSTCKCTRL.bit.SYSTCKDIV;
  systick_clk = SCU_getSys0Clk_kHz() / systick_clk_div;
  return systick_clk;
}

