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
#include "cpu.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize the CPU module
 * 
 * \return sint8 0: success, <0: error codes
 */
sint8 CPU_init(void)
{
  /*{0712BDDD-2523-4350-820D-D3C4397A7FC0}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS; /* config can not be disabled in CW */
  
  CPU->NVIC_ISER0.reg = (uint32) CPU_NVIC_ISER0;
  CPU->NVIC_IPR0.reg = (uint32) CPU_NVIC_IPR0;
  CPU->NVIC_IPR1.reg = (uint32) CPU_NVIC_IPR1;

  CPU->SHPR2.reg = (uint32) CPU_SHPR2;
  CPU->SHPR3.reg = (uint32) CPU_SHPR3;

  return s8_returnCode;

}

/** \brief Initialize the SysTick
 * 
 * \param u32_reloadValue SysTick Reload Value
 * \param u8_clockSource SysTick Clock Source selection
 * 
 * \return sint8 0: success, <0: error codes
 */
sint8 CPU_initSysTick(uint32 u32_reloadValue, uint8 u8_clockSource)
{
  /*{35AA5A31-E98D-4411-9C7A-7A6D40613E97}*/

  if ((u32_reloadValue - 1UL) > CPU_SYST_RVR_RELOAD_Msk || u8_clockSource > 1u )
  {
    return ERR_LOG_CODE_PARAM_INVALID; /* Reload value or clock source invalid */
  }

  /* Program the SysTick reload value */
  CPU->SYST_RVR.reg = u32_reloadValue;
  /* Reset the SysTick current value */
  CPU->SYST_CVR.bit.CURRENT = 0u;
  /* Initialize SysTick timer clock source */
  CPU->SYST_CSR.bit.CLKSOURCE = (u8_clockSource != 0u) ? 1u : 0u;
  /* Enable the SysTick exception request */
  CPU->SYST_CSR.bit.TICKINT = 1u;
  /* Enable the SysTick counter */
  CPU->SYST_CSR.bit.ENABLE = 1u;

  return ERR_LOG_SUCCESS;
} 


