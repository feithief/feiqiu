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
#include "cpu.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize the CPU module
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t CPU_init(void)
{
  /*{0712BDDD-2523-4350-820D-D3C4397A7FC0}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS; /* config can not be disabled in CW */
  CPU->NVIC_ISER0.reg = (uint32_t) CPU_NVIC_ISER0;
  CPU->NVIC_IPR0.reg = (uint32_t) CPU_NVIC_IPR0;
  CPU->NVIC_IPR1.reg = (uint32_t) CPU_NVIC_IPR1;
  CPU->SHPR2.reg = (uint32_t) CPU_SHPR2;
  CPU->SHPR3.reg = (uint32_t) CPU_SHPR3;
  return u8_returnCode;
}

/** \brief Initialize the SysTick
 *
 * \param u32_reloadValue SysTick Reload Value
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t CPU_initSysTick(uint32_t u32_reloadValue)
{
  /*{35AA5A31-E98D-4411-9C7A-7A6D40613E97}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;

  if (((u32_reloadValue - 1u) <= CPU_SYST_RVR_RELOAD_Msk))
  {
    /* Program the SysTick reload value */
    CPU->SYST_RVR.reg = u32_reloadValue;
    /* Reset the SysTick current value */
    CPU->SYST_CVR.bit.CURRENT = 0u;
    /* Enable the SysTick exception request */
    CPU->SYST_CSR.bit.TICKINT = 1u;
    /* Enable the SysTick counter */
    CPU->SYST_CSR.bit.ENABLE = 1u;
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_INVALID; /* Reload value or clock source invalid */
  }

  return u8_returnCode;
}


