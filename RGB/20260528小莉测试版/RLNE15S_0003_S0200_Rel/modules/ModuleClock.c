/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleClock.c
* Project:		RLNE15S
* Description:	This file maintains the configuration of clock and SysTimer.
* Note(s):	
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/

/**********************************************************************************************************************
INCLUDE FILES
**********************************************************************************************************************/
#include "ModuleClock.h"
/**********************************************************************************************************************
INCLUDE FILES
**********************************************************************************************************************/
/**********************************************************************************************************************
GLOBAL VARIABLES
**********************************************************************************************************************/
/*!< Current system ticks >*/
tick_t g_u16_CurrentTick = 0;	
/**********************************************************************************************************************
GLOBAL VARIABLES
**********************************************************************************************************************/


/**
* @brief 		Initialize the OSC module
* @note 		This function is used to configure whether to divide the CLKSRC1 or not.
* @param[in]	void                           
* @return		s8_ReturnCode
* @retval		ERR_LOG_SUCCESS       Indicates successful initialization.
* @par  		Edition History
	Version Number   	Time    	 Author
	00.01		2017.10.24		Leo Song
	00.02       2024.10.12       Sen Gao
**/
int8_t ModuleClock_Init(void)
{
	int8_t s8_ReturnCode;
	s8_ReturnCode = ERR_LOG_SUCCESS;
	// Set the clock division for the oscillator
	OSC->FCLKDIV.reg = (uint32)OSC_FCLKDIV;

	return s8_ReturnCode;
}


/**
* @brief 		Get current tick
* @note 		This function is used to get current tick
* @param[in]	void                           
* @return		u16_CurTick
* @retval		The value of current tick
* @par  		Edition History
    Version Number   	Time    	 Author
  00.01		2017.10.24		Leo Song
  00.02       2024.10.12      Sen Gao
**/
tick_t ModuleClock_TickGet(void)
{
	tick_t u16_CurTick;
	u16_CurTick = g_u16_CurrentTick;
	return u16_CurTick;
}

