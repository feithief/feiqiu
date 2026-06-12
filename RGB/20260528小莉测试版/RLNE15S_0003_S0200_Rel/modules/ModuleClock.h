/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleClock.h
* Project:		RLNE15S
* Description:	The header file of ModuleClock.
* Note(s):	
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/

#ifndef MODULECLOCK_H
#define MODULECLOCK_H

#include "tld40xx.h"
#include "osc_defines.h"
#include "SystemType.h"


/*!< Current system ticks >*/
extern tick_t g_u16_CurrentTick;

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
int8_t ModuleClock_Init(void);


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
tick_t ModuleClock_TickGet(void);


#endif 
