/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleBattery.h
* Project:		RLNE15S
* Description:	The header file of Modulebattery.
* Note(s):		None
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/

#ifndef MODULEBATTERY_H
#define MODULEBATTERY_H

#include "ModuleAdc.h"


/*!< Input voltage status >*/
typedef enum
{
  E_BATSTATUS_INIT		= 0x0,	/*!< input voltage initial status >*/
  E_BATSTATUS_NORMAL	= 0x1,	/*!< input voltage normal >*/
  E_BATSTATUS_UNDERVOLT = 0x2,  /*!< input voltage too low >*/
  E_BATSTATUS_OVERVOLT  = 0x4,  /*!< input voltage too High >*/
} E_BATSTATUS;


/**
* @brief 		Initialize input voltage detection.
* @note 		None
* @param[in]	void                           
* @return		void
* @retval		None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleBattery_Init(void);


/**
* @brief 		Update the current status of the battery based on voltage readings.
* @note 		The battery status is updated based on predefined voltage thresholds.
* @param[in]	void                          
* @return		g_e_CurrentBatStatus
* @retval		Current battery status:
*               	- E_BATSTATUS_UNDERVOLT: Voltage is below the undervoltage threshold.
*               	- E_BATSTATUS_OVERVOLT: Voltage is above the overvoltage threshold.
*               	- E_BATSTATUS_NORMAL: Voltage is within normal operating range.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
E_BATSTATUS ModuleBattery_StatusUpdate(void);


/**
* @brief 		Get the current input power voltage from the battery.
* @note 		None
* @param[in]	void                           
* @return		g_u32_BatteryVoltage
* @retval		The current input battery voltage in millivolts (mV).
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
uint32_t ModuleBattery_VoltageGet(void);


#endif
