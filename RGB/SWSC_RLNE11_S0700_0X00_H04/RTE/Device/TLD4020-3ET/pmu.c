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
#include "pmu.h"

/*******************************************************************************
**                        Private Variable Definitions                        **
*******************************************************************************/

static volatile uint32 u32_watchdogCnt;
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
 * \return sint8 0: success, <0: error codes
 */
sint8 PMU_init(void)
{
  /*{D8BBCB73-3364-461b-B33D-B7C86149E73E}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS; /* config can not be disabled in CW */

  PMU->VDD1V5IEN.reg = (uint32) PMU_VDD1V5IEN;
  PMU->VDD1V5NVMIEN.reg = (uint32) PMU_VDD1V5NVMIEN;
  PMU->WAKEFILTCTRL.reg = (uint32) PMU_WAKEFILTCTRL;
  PMU->IOWAKECTRL0.reg = (uint32) PMU_IOWAKECTRL0;
  PMU->WAKECTRL.reg = (uint32) PMU_WAKECTRL;

  return s8_returnCode;
}

/** \brief Initialize the watchdog
 *
 */
sint8 PMU_initWatchdog(void)
{
  /*{E0E438B3-FF50-4b73-AF37-7835F2C72FC0}*/
  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS;

#if (PMU_WDCTRL_DISABLE != 0)
  PMU->WDCTRL.bit.EN = 0;
#endif
	
  /* The watchdog starts in Long Open Window (typically 180 ms) */
  /* The watchdog period can only be programmed during the LOW phase */
  /* wd_en is masked: the FW automatically resets the EN bit when entering debug mode or sets it when entering user mode */
  PMU->WDCTRL.bit.WDP = (uint8)(((uint32)PMU_WDCTRL & PMU_WDCTRL_WDP_Msk) >> PMU_WDCTRL_WDP_Pos);
  /* The number of SOW must be set before the first watchdog in the LOW */
  PMU->WDCTRL.bit.SOW = (uint8)(((uint32)PMU_WDCTRL & PMU_WDCTRL_SOW_Msk) >> PMU_WDCTRL_SOW_Pos);
  /* The bit TRIG needs to be toggled to provide a proper trigger */
  PMU->WDTRIG.bit.TRIG ^= 1u;
  /* The fail-safe watchdog counter must be reset, since a new window starts */
  u32_watchdogCnt = 0;
	
	return s8_returnCode;
}

/** \brief Service the watchdog
 *
 * \return sint8 0: success, <0: error codes
 */
sint8 moduleWatchdogInit(void)
{
  /*{06C656FA-96A7-4aa8-84F0-791255894CE6}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS;
  
  /* Check if the counter is within the effective open window (safe trigger point) or if a SOW service has been done before */
  if ((u32_watchdogCnt > PMU_SAFE_TRIGGER) || (b_SOWactive == true))
  {
    /* Service watchdog by toggling bit PMU->WDTRIG.bit.TRIG */
    PMU->WDTRIG.bit.TRIG ^= 1u;
    /* Reset the watchdog counter */
    u32_watchdogCnt = (uint32)0;
    /* Reset "short open window" active flag */
    b_SOWactive = false;
  }
  else
  {
    s8_returnCode = ERR_LOG_CODE_NO_TRIGGER;
  }
  
  return s8_returnCode;
}


sint8 moduleWatchdogFeed(void)
{
  /*{06C656FA-96A7-4aa8-84F0-791255894CE6}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS;
  
  /* Check if the counter is within the effective open window (safe trigger point) or if a SOW service has been done before */
  if ((u32_watchdogCnt > PMU_SAFE_TRIGGER) || (b_SOWactive == true))
  {
    /* Service watchdog by toggling bit PMU->WDTRIG.bit.TRIG */
    PMU->WDTRIG.bit.TRIG ^= 1u;
    /* Reset the watchdog counter */
    u32_watchdogCnt = (uint32)0;
    /* Reset "short open window" active flag */
    b_SOWactive = false;
  }
  else
  {
    s8_returnCode = ERR_LOG_CODE_NO_TRIGGER;
  }
  
  return s8_returnCode;
}

/** \brief Service a Short Open Window for the watchdog
 *
 * \return sint8 0: success, <0: error codes
 */
sint8 PMU_serviceWatchdogSOW(void)
{
  /*{FF119D0E-0380-4cdb-874C-5F736DDA855F}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS;

  if (PMU->WDCTRL.bit.SOW != 0u)
  {
    PMU->WDTRIGSOW.bit.TRIG ^= 1u;
    b_SOWactive = true;
  }
  else
  {
    s8_returnCode = ERR_LOG_CODE_SOW_DISABLED;
  }

  return s8_returnCode;
}

/** \brief Stop the watchdog
 */
void PMU_stopWatchdog(void)
{
  /*{6543D91E-A42C-4efa-8FB2-43163B2B2E04}*/

  /* Disable the SysTick Timer */
  CPU->SYST_CSR.bit.ENABLE = 0u;
} 


//sint8 PMU_serviceWatchdog(void)
//{}
