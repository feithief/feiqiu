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
#include "ldrv.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize the LDRV module
 * 
 * \return sint8 0: success, <0: error codes
 */
sint8 LDRV_init(void)
{
  /*{45CE83A7-839F-4ea4-8C52-91DBC6DEAB23}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_CODE_MODULE_DISABLED_IN_CW;

  #if ((LDRV_MOD_CFG & LDRV_MOD_CFG_LD_ENABLE_Msk) == LDRV_MOD_CFG_LD_ENABLE_Msk)
    s8_returnCode = ERR_LOG_SUCCESS;

    LDRV->PWM_FREQ.reg = (uint32) LDRV_PWM_FREQ;
	
	LDRV->PWM_SYNCH.reg = (uint32) LDRV_PWM_SYNCH;

    LDRV->PWM_PHASE_CFG0.reg = (uint32) LDRV_PWM_PHASE_CFG0;
    LDRV->PWM_PHASE_CFG1.reg = (uint32) LDRV_PWM_PHASE_CFG1;
    LDRV->PWM_PHASE_CFG2.reg = (uint32) LDRV_PWM_PHASE_CFG2;

    LDRV->PWM_DC_CFG0.reg = (uint32) LDRV_PWM_DC_CFG0;
    LDRV->PWM_DC_CFG1.reg = (uint32) LDRV_PWM_DC_CFG1;
    LDRV->PWM_DC_CFG2.reg = (uint32) LDRV_PWM_DC_CFG2;

    LDRV->PWM_ISET_CFG0.reg = (uint32) LDRV_PWM_ISET_CFG0;
    LDRV->PWM_ISET_CFG1.reg = (uint32) LDRV_PWM_ISET_CFG1;
    LDRV->PWM_ISET_CFG2.reg = (uint32) LDRV_PWM_ISET_CFG2;

    LDRV->OSP_CFG.reg = (uint32) LDRV_OSP_CFG;

    LDRV->DIAG_CFG.reg = (uint32) LDRV_DIAG_CFG;
    LDRV->DIAG_PHASE_CFG0.reg = (uint32) LDRV_DIAG_PHASE_CFG0;
    LDRV->DIAG_PHASE_CFG1.reg = (uint32) LDRV_DIAG_PHASE_CFG1;
    LDRV->DIAG_PHASE_CFG2.reg = (uint32) LDRV_DIAG_PHASE_CFG2;

    LDRV->IEN0.reg = (uint32) LDRV_IEN0;
    LDRV->IEN1.reg = (uint32) LDRV_IEN1;

    LDRV->MOD_CFG.reg = (uint32) LDRV_MOD_CFG;

  #endif

  return s8_returnCode;
	
} 

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
/**
* @brief 		Initialize LED functions.
* @note 		This function must be invoked before any other LED functions.
* @param[in]	void                          
* @return		void
* @retval		None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleLed_Init(void)
{
	// Set the PWM frequency(250Hz)
	LDRV->PWM_FREQ.reg = (uint32) LDRV_PWM_FREQ;

	// Configure the phase settings for each PWM channel (0, 1, 2)
	LDRV->PWM_PHASE_CFG0.reg = (uint32) LDRV_PWM_PHASE_CFG0;
	LDRV->PWM_PHASE_CFG1.reg = (uint32) LDRV_PWM_PHASE_CFG1;
	LDRV->PWM_PHASE_CFG2.reg = (uint32) LDRV_PWM_PHASE_CFG2;

	LDRV->PWM_DC_CFG0.reg = (uint32) LDRV_PWM_DC_CFG0;
	LDRV->PWM_DC_CFG1.reg = (uint32) LDRV_PWM_DC_CFG1;
	LDRV->PWM_DC_CFG2.reg = (uint32) LDRV_PWM_DC_CFG2;
	 
	// Configure the current settings for each PWM channel (0, 1, 2)
	LDRV->PWM_ISET_CFG0.reg = (uint32) LDRV_PWM_ISET_CFG0;
	LDRV->PWM_ISET_CFG1.reg = (uint32) LDRV_PWM_ISET_CFG1;
	LDRV->PWM_ISET_CFG2.reg = (uint32) LDRV_PWM_ISET_CFG2;

	LDRV->OSP_CFG.reg = (uint32) LDRV_OSP_CFG;

	LDRV->DIAG_CFG.reg = (uint32) LDRV_DIAG_CFG;
	LDRV->DIAG_PHASE_CFG0.reg = (uint32) LDRV_DIAG_PHASE_CFG0;
	LDRV->DIAG_PHASE_CFG1.reg = (uint32) LDRV_DIAG_PHASE_CFG1;
	LDRV->DIAG_PHASE_CFG2.reg = (uint32) LDRV_DIAG_PHASE_CFG2;

	LDRV->IEN0.reg = (uint32) LDRV_IEN0;
	LDRV->IEN1.reg = (uint32) LDRV_IEN1;

	LDRV->MOD_CFG.reg = (uint32) LDRV_MOD_CFG;
}
