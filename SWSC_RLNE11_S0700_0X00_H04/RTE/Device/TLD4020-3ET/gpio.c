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
#include "gpio.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize the GPIO module
 * 
 * \return sint8 0: success, <0: error codes
 */
sint8 GPIO_init(void)
{
  /*{0C84DE37-A85C-4f81-9C1D-BF362506B9EF}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS;  /* config can not be disabled in CW */

  GPIO->PADCFG0_DRVCFG.bit.DIR = 1;
  GPIO->PADCFG0_DRVCFG.bit.OD = 1;
  GPIO->PADCFG0_DRVCFG.bit.MODE = 0;
  GPIO->PDISC.bit.PDIS0 = 0;
  GPIO->OMSR0.bit.PS0 = 1;
  //GPIO->PADCFG0_GPIO.bit.OUT = 1;
  GPIO->OUT.bit.P0 = 0;
  //GPIO->PADCFG0_GPIO.bit.SET = 1;
  
#if 0
  GPIO->PADCFG1_DRVCFG.bit.DIR = 1;
  GPIO->PADCFG1_DRVCFG.bit.OD = 0;
  GPIO->PADCFG1_DRVCFG.bit.MODE = 0;
  GPIO->PADCFG1_GPIO.bit.SET = 1;

  GPIO->PADCFG0_GPIO.reg = (uint32) GPIO_PADCFG0_GPIO;
  GPIO->PADCFG0_DRVCFG.reg = (uint32) GPIO_PADCFG0_DRVCFG;
  GPIO->PADCFG1_GPIO.reg = (uint32) GPIO_PADCFG1_GPIO;
  GPIO->PADCFG1_DRVCFG.reg = (uint32) GPIO_PADCFG1_DRVCFG;
  
  GPIO->IEN.reg = (uint32) GPIO_IEN;
#endif

  return s8_returnCode;
	
} 

