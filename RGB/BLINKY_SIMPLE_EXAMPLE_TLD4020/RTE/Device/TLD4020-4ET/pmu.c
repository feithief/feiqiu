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
#include "pmu.h"

/*******************************************************************************
**                        Private Variable Definitions                        **
*******************************************************************************/

static volatile uint32_t u32_watchdogCnt;
//lint --e{9034}
static bool b_SOWactive = false;

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/


/** \brief Count up since the last watchdog trigger
 */
void PMU_countWatchdog(void)
{
  /*{27213E96-B824-4b48-A533-014CC9287D29}*/
  u32_watchdogCnt++;
}

/** \brief Initialize the PMU module
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t PMU_init(void)
{
  /*{D8BBCB73-3364-461b-B33D-B7C86149E73E}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS; /* config can not be disabled in CW */
  PMU->VDD1V5IEN.reg = (uint32_t) PMU_VDD1V5IEN;
  PMU->VDD1V5NVMIEN.reg = (uint32_t) PMU_VDD1V5NVMIEN;
  PMU->WAKEFILTCTRL.reg = (uint32_t) PMU_WAKEFILTCTRL;
  PMU->IOWAKECTRL0.reg = (uint32_t) PMU_IOWAKECTRL0;
  PMU->WAKECTRL.reg = (uint32_t) PMU_WAKECTRL;
  return u8_returnCode;
}

/** \brief Initialize the watchdog
 *
 */
uint8_t PMU_initWatchdog(void)
{
  /*{E0E438B3-FF50-4b73-AF37-7835F2C72FC0}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;
#if (PMU_WDCTRL_DISABLE != 0)
  PMU->WDCTRL.bit.EN = 0;
#endif
  /* The watchdog starts in Long Open Window (typically 180 ms) */
  /* The watchdog period can only be programmed during the LOW phase */
  /* wd_en is masked: the FW automatically resets the EN bit when entering debug mode or sets it when entering user mode */
  PMU->WDCTRL.bit.WDP = (uint8_t)(((uint32_t)PMU_WDCTRL & PMU_WDCTRL_WDP_Msk) >> PMU_WDCTRL_WDP_Pos);
  /* The number of SOW must be set before the first watchdog in the LOW */
  PMU->WDCTRL.bit.SOW = (uint8_t)(((uint32_t)PMU_WDCTRL & PMU_WDCTRL_SOW_Msk) >> PMU_WDCTRL_SOW_Pos);
  /* The bit TRIG needs to be toggled to provide a proper trigger */
  PMU->WDTRIG.bit.TRIG ^= 1u;
  /* The fail-safe watchdog counter must be reset, since a new window starts */
  u32_watchdogCnt = 0;
  return u8_returnCode;
}

/** \brief Service the watchdog
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t PMU_serviceWatchdog(void)
{
  /*{06C656FA-96A7-4aa8-84F0-791255894CE6}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;

  /* Check if the counter is within the effective open window (safe trigger point) or if a SOW service has been done before */
  if ((u32_watchdogCnt > PMU_SAFE_TRIGGER) || b_SOWactive)
  {
    /* Service watchdog by toggling bit PMU->WDTRIG.bit.TRIG */
    PMU->WDTRIG.bit.TRIG ^= 1u;
    /* Reset the watchdog counter */
    u32_watchdogCnt = (uint32_t)0;
    /* Reset "short open window" active flag */
    b_SOWactive = false;
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_NO_TRIGGER;
  }

  return u8_returnCode;
}

/** \brief Service a Short Open Window for the watchdog
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t PMU_serviceWatchdogSOW(void)
{
  /*{FF119D0E-0380-4cdb-874C-5F736DDA855F}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;

  if (PMU->WDCTRL.bit.SOW != 0u)
  {
    PMU->WDTRIGSOW.bit.TRIG ^= 1u;
    b_SOWactive = true;
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_SOW_DISABLED;
  }

  return u8_returnCode;
}

/** \brief Stop the watchdog
 */
void PMU_stopWatchdog(void)
{
  /*{6543D91E-A42C-4efa-8FB2-43163B2B2E04}*/
  /* Disable the SysTick Timer */
  CPU->SYST_CSR.bit.ENABLE = 0u;
}

