/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleTemperature.h
* Project:		RLNE15S
* Description:	The header file of ModuleTemperature
* Note(s):	
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12      Sen Gao            Code standardization 
************************************************************************
*/

#ifndef MODULETEMPERATURE_H
#define MODULETEMPERATURE_H
#include "ModuleAdc.h"


/*!< Multiplying factor for temperature >*/
#define D_TEMP_MULTIPLIED_RATIO       10


/**
* @brief      Update the temperature value of LED.
* @note       Temperature detection callback function, Currently, we do this every 250ms(experience time).
*             This function will update g_s16_LedTemperature value.
* @param      void
* @return     void 
* @retval     None
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleTemperature_Update(void);


/**
* @brief      Get current LED temperature.
* @note       This function returns the temperature of LED.
* @param      void
* @return     g_s16_LedTemperature
* @retval     The current LED temperature
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12      Sen Gao
**/
int16_t ModuleTemperature_LedGet(void);


/**
* @brief      Get current MCU temperature.
* @note       This function returns the temperature of MCU.
* @param      void
* @return     g_s16_McuTemperature
* @retval     The current MCU temperature
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
int16_t ModuleTemperature_McuGet(void);


#endif
