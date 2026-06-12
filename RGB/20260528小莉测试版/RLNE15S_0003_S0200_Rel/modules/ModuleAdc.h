/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleAdc.h
* Project:		RLNE15S
* Description:	The header file of ModuleAdc.
* Note(s):		None.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/
#ifndef MOD_ADC_H
#define MOD_ADC_H

#include <stdint.h>
#include "adc11.h"

/**
* @brief 		Get ADC register value.
* @note 		None
* @param[in]	u32_Channel -- the channel of ADC, where the following channels are supported:
*                 - ADC_CH_VS: Battery Voltage ADC
*                 - ADC_CH_LED0: Blue LED ADC
*                 - ADC_CH_LED1: Green LED ADC
*                 - ADC_CH_LED2: Red LED ADC
*                 - ADC_CH_VTEMP: MCU Temperature ADC
* @return		u32_RegValue.
* @retval		Returns the ADC value for the specified channel. 
*				If an invalid channel is provided, returns 0xFFFFFFFEU.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
uint32_t ModuleAdc_RegValueGet(uint32_t u32_Channel);


#endif
