/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleBattery.c
* Project:		RLNE15S
* Description:	This module handles the battery voltage monitoring 
*               and management functionalities for the project.
* Note(s):		None.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.08.12       Sen Gao            Code standardization 
************************************************************************
*/

/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
#include "ModuleBattery.h"
#include "ModuleAdc.h"
#include "SystemType.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/
/*!< The threshold for undervoltage >*/
#define D_UNDER_VOLTAGE_OFF		(7500U) 
/*!< The threshold for undervoltage recovery >*/
#define D_UNDER_VOLTAGE_ON   	(8500U) 
/*!< The threshold for overvoltage recovery >*/
#define D_OVER_VOLTAGE_ON    	(16500U)
/*!< The threshold for overvoltage >*/
#define D_OVER_VOLTAGE_OFF   	(17500U)
/*!< Number of filters >*/
#define D_MAX_FILTER_COUNTER  	(100)

/*!< Current input voltage value >*/
static uint32_t g_u32_BatteryVoltage = 12000U;
/*!< Input voltage status detection filter >*/
static int8_t g_s8_StatusCounter = 0;
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/
/**********************************************************************************************************************
  LOCAL FUNCTIONS DECLARATION
  **********************************************************************************************************************/ 
 static uint32_t ModuleBattery_VoltageConvert(void);
 static E_BATSTATUS ModuleBattery_StatusSet(uint32_t u32_Voltage, uint32_t u32_MinThreshold, uint32_t u32_MaxThreshold);
/**********************************************************************************************************************
  LOCAL FUNCTIONS DECLARATION
  **********************************************************************************************************************/  
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
void ModuleBattery_Init(void)
{
	g_s8_StatusCounter = D_MAX_FILTER_COUNTER;
	(void)ModuleBattery_StatusUpdate();
}


/**
* @brief 		Get the current filtered input voltage value from the ADC.
* @note 		The ADC channel used for voltage measurement is defined by `ADC_CH_VS`.
* @param[in]	void                           
* @return		u32_ConvVal
* @retval		Converted voltage value in millivolts (mV).
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
static uint32_t ModuleBattery_VoltageConvert(void)
{
	uint32_t u32_ConvVal = 0;
	// Check if the ADC is ready for reading
	if (ADC11->RESULT.bit.ADC_READY  == btrue)
	{
		ADC11->CFG_1.bit.ADC_SOC = 1; 
	}
	// Get the ADC value, scale it to mV, and return
	u32_ConvVal	= (ModuleAdc_RegValueGet(ADC_CH_VS) * 20067 / 2047);
	return u32_ConvVal;
}


/**
* @brief 		Get the current filtered input voltage value from the ADC.
* @note 		The ADC channel used for voltage measurement is defined by `ADC_CH_VS`.
* @param[in]	void                           
* @return		u32_ConvVal
* @retval		Converted voltage value in millivolts (mV).
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
static E_BATSTATUS ModuleBattery_StatusSet(uint32_t u32_Voltage, uint32_t u32_MinThreshold, uint32_t u32_MaxThreshold)
{
	E_BATSTATUS e_BatteryStatus = E_BATSTATUS_NORMAL;
	
	// Check if the voltage is below the minimum threshold
	if (u32_Voltage < u32_MinThreshold) 
	{
		e_BatteryStatus = E_BATSTATUS_UNDERVOLT;
    } 
	// Check if the voltage exceeds the maximum threshold
	else if (u32_Voltage > u32_MaxThreshold) 
	{
        e_BatteryStatus = E_BATSTATUS_OVERVOLT;
    } 
	// Voltage is within the normal range
	else 
	{
        e_BatteryStatus = E_BATSTATUS_NORMAL;
    }
	
	return e_BatteryStatus;
}


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
E_BATSTATUS ModuleBattery_StatusUpdate(void)
{
	uint32_t u32_FilterLimitMax = D_UNDER_VOLTAGE_ON;
	uint32_t u32_FilterLimitMin = D_OVER_VOLTAGE_ON;
	E_BATSTATUS e_BatStatusTemp;
	static E_BATSTATUS g_e_CurrentBatStatus = E_BATSTATUS_INIT;

	// Setting the voltage threshold according to the current battery status.
	switch(g_e_CurrentBatStatus)
	{
		// When the battery status is undervolt
		case E_BATSTATUS_UNDERVOLT:
			u32_FilterLimitMin = D_UNDER_VOLTAGE_ON;
			u32_FilterLimitMax = D_OVER_VOLTAGE_OFF;
			break;
		// When the battery status is overvolt
		case E_BATSTATUS_OVERVOLT:
			u32_FilterLimitMin = D_UNDER_VOLTAGE_OFF;
			u32_FilterLimitMax = D_OVER_VOLTAGE_ON;
			break;
		// When the battery status is initializing or normal
		case E_BATSTATUS_INIT:
		case E_BATSTATUS_NORMAL:
			u32_FilterLimitMin = D_UNDER_VOLTAGE_OFF;
			u32_FilterLimitMax = D_OVER_VOLTAGE_OFF;
			break;
		default:
			// Handle any undefined states, do nothing
			break;
	}

	// Getting the Converted voltage value
	g_u32_BatteryVoltage = ModuleBattery_VoltageConvert();
	
	// Check if the current voltage is undervoltage or overvoltage and set the status
	e_BatStatusTemp = ModuleBattery_StatusSet(g_u32_BatteryVoltage, u32_FilterLimitMin, u32_FilterLimitMax);
	
	//Detection Status Filtering
	if (e_BatStatusTemp != g_e_CurrentBatStatus)
	{	// If the status has changed, decrement the status counter
		g_s8_StatusCounter--;
	}
	else
	{	// Reset the counter if the status remains the same
		g_s8_StatusCounter = D_MAX_FILTER_COUNTER;
	}
	
	if (g_e_CurrentBatStatus == E_BATSTATUS_INIT)
	{
		g_s8_StatusCounter = 0;
	}
		
	// Update the current battery status if the counter has reached zero
	if (g_s8_StatusCounter <= 0)
	{
		g_e_CurrentBatStatus = e_BatStatusTemp;
		g_s8_StatusCounter = D_MAX_FILTER_COUNTER;
	}

	return g_e_CurrentBatStatus;
}


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
uint32_t ModuleBattery_VoltageGet(void)
{
	return g_u32_BatteryVoltage;
}
