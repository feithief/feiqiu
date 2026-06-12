/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleAdc.c
* Project:		RLNE15S
* Description:	This file contains functions for handling ADC channels 
*               related to the battery voltage, LED colors (blue, green, 
*               red), and MCU temperature. It includes interrupt service 
*               routines to handle ADC updates.
* Note(s):	- This module is designed for use in systems with an LDRV 
*                 for diagnostics and ADC conversions for LED forward 
*                 voltage and temperature readings.
*           - Global variables are used to store the ADC values.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/

/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
#include "ModuleAdc.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/
/*!< Current Red LED ADC voltage value >*/
static uint32_t g_u32_RedAdcVoltage		= 600U;
/*!< Current Green LED ADC voltage value >*/
static uint32_t g_u32_GreenAdcVoltage 	= 600U;
/*!< Current Blue LED ADC voltage value >*/
static uint32_t g_u32_BlueAdcVoltage 	= 600U;
/*!< Current Battery ADC voltage value >*/
static uint32_t g_u32_BatteryAdcVoltage = 1000U;
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/
/**********************************************************************************************************************
  LOCAL FUNCTIONS DECLARATION
  **********************************************************************************************************************/ 
/*!< Interrupt service routine for ch0 >*/
void ModuleAdc_Ch0Isr(void);
/*!< Interrupt service routine for ch1 >*/
void ModuleAdc_Ch1Isr(void);
/*!< Interrupt service routine for ch2 >*/
void ModuleAdc_Ch2Isr(void);
/*!< Interrupt service routine for adc11 >*/
void ModuleAdc_Adc11Isr(void);
/*!< Interrupt service routine for OverTemperature >*/
void ModuleAdc_TempIsr(void);
/*!< Interrupt service routine for EOD >*/
void ModuleAdc_EodIsr(void);
/**********************************************************************************************************************
  LOCAL FUNCTIONS DECLARATION
  **********************************************************************************************************************/  
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
uint32_t ModuleAdc_RegValueGet(uint32_t u32_Channel) 
{
	uint32_t u32_RegValue = 0xFFFFFFFEU;
	static uint32_t g_u32_McuAdcTemperature = 400U;
	
	// Use a switch statement to determine the correct ADC value based on the input channel
	switch(u32_Channel)
	{
		// If the channel is ADC_CH_VS (Battery Voltage), return the battery ADC value
		case ADC_CH_VS:
			u32_RegValue = g_u32_BatteryAdcVoltage;
			break;
		
		// If the channel is ADC_CH_LED0 (Blue LED), return the blue LED ADC value
		case ADC_CH_LED0:
			u32_RegValue = g_u32_RedAdcVoltage;
			break;
		
		// If the channel is ADC_CH_LED1 (Green LED), return the green LED ADC value
		case ADC_CH_LED1:
			u32_RegValue = g_u32_BlueAdcVoltage;
			break;
		
		// If the channel is ADC_CH_LED2 (Red LED), return the red LED ADC value
		case ADC_CH_LED2:
			u32_RegValue = g_u32_GreenAdcVoltage;
			break;
		
		// If the channel is ADC_CH_VTEMP (Temperature Sensor), return the MCU temperature ADC value
		case ADC_CH_VTEMP:
			g_u32_McuAdcTemperature = ADC11->TS_RESULT.bit.TS_VALUE;
			u32_RegValue = g_u32_McuAdcTemperature;
			break;
		
		// If the channel number is invalid, return a specific error value (0xFFFFFFFE)
		default:
			u32_RegValue = 0xFFFFFFFEU;
			break;
	}
	
	return u32_RegValue;
}


/**
* @brief 		LDRV CH0 interrupt service routine.
* @note 		This function handles the interrupt for ADC Channel 0 and updates the global ADC value for the red LED.
* @param[in]	void                          
* @return		void
* @retval		None
* @details     The function reads the value from the LDRV diagnostic register for Channel 0 (VFWD0) and stores it in the global variable `g_u32_RedAdcVoltage`, 
				which represents the ADC value for the Red LED.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleAdc_Ch0Isr(void)
{
	g_u32_RedAdcVoltage = LDRV->DIAG_VFWD0.bit.VFWD;
}       


/**
* @brief 		LDRV CH1 interrupt service routine.
* @note 		This function handles the interrupt for ADC Channel 1 and updates the global ADC value for the blue LED.
* @param[in]	void                          
* @return		void
* @retval		None
* @details     The function reads the value from the LDRV diagnostic register for Channel 1 (VFWD1) and stores it in the global variable `g_u32_BlueAdcVoltage`, 
				which represents the ADC value for the blue LED.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleAdc_Ch1Isr(void)
{
    g_u32_BlueAdcVoltage = LDRV->DIAG_VFWD1.bit.VFWD;
}  


/**
* @brief 		LDRV CH2 interrupt service routine.
* @note 		This function handles the interrupt for ADC Channel 2 and updates the global ADC value for the green LED.
* @param[in]	void                          
* @return		void
* @retval		None
* @details     The function reads the value from the LDRV diagnostic register for Channel 2 (VFWD2) and stores it in the global variable `g_u32_GreenAdcVoltage`, 
				which represents the ADC value for the green LED.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleAdc_Ch2Isr(void)
{
    g_u32_GreenAdcVoltage = LDRV->DIAG_VFWD2.bit.VFWD;
}
  

/**
* @brief 		ADC11 interrupt service routine.
* @note 		This function handles the ADC11 interrupt and updates the battery voltage.
* @param[in]	void                          
* @return		void
* @retval		None
* @details     The function checks if the interrupt is triggered by ADC channel 0, which corresponds to the battery voltage. 
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleAdc_Adc11Isr(void)
{
  g_u32_BatteryAdcVoltage = ADC11->RESULT.bit.ADC_VALUE;
}

