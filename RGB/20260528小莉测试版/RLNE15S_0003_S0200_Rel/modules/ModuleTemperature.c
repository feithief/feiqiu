/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleTemperature.c
* Project:		RLNE15S
* Description:	This module is responsible for handling temperature data 
*               from the LED and MCU by interfacing with the ADC. It includes 
*               functions to update and retrieve temperature values.
* Note(s):	-   The temperature values are calculated with specific scaling 
*               using the defined multiplier (D_TEMP_MULTIPLIED_RATIO).
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/


/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
#include "ModuleTemperature.h"
#include "ModuleAdc.h"
#include "ModuleClock.h"
#include "ModuleBattery.h"
#include "ModuleLed.h"
#include "ModuleFlash.h"
#include "SystemType.h"

/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/
/*!< Defines temperature update period. Update function will capture temperature 
    every 10ms and make average every DTEMP_UPDATE_CYCLE_PERIOD(250ms), 
      and then make a filter((old*3+new)/4) >*/
#define DTEMP_UPDATE_CYCLE_PERIOD   (250U)
/*!< The filter counter of mcu temperature >*/
#define DTEMP_MCU_FILTER_LEVEL		(8U)
/*!< The filter counter of led temperature >*/
#define DTEMP_LED_FILTER_LEVEL		(16)
/*!< Peak current >*/
#define DLED_CURRENT_MAX			(35U)

/*!< Current LED temperature value >*/
static int16_t g_s16_LedTemperature = 0;
/*!< Current MCU temperature value >*/
static int16_t g_s16_McuTemperature = 0;
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/

/**
* @brief      Convert the raw ADC value of temperature.
* @note       This function reads the ADC value and applies a conversion formula to calculate the temperature.
* @param      None
* @return     s32_ConvertVal 
* @retval     The converted temperature value.
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12      Sen Gao
**/
static int16_t ModuleTemperature_Convert(void)
{
	uint32_t u32_Temp = 0;
	int32_t s32_ConvertVal = 0;
	int32_t s32_Temp1 = 0;
	int32_t s32_Temp2 = 0;
	
	// Get the raw ADC value of temperature from the temperature channel.
	u32_Temp = ModuleAdc_RegValueGet(ADC_CH_VTEMP);
	s32_Temp1 = (int32_t)((u32_Temp * (uint32_t)1222) - (uint32_t)204800);
	s32_Temp2 = s32_Temp1 * 190 * 5;
	s32_ConvertVal = -40 + (s32_Temp2/ 4/ 2048000) ;
	
	return (int16_t)s32_ConvertVal;
}


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
void ModuleTemperature_Update(void)
{
	int16_t s16_JointTemperature = ModuleTemperature_Convert();
	int16_t s16_TempTemperature = 0;
	int32_t s32_PowerMcu = 0;
	int32_t s32_PowerRedLed = 0;
	int32_t s32_PowerGreenLed = 0;
	int32_t s32_PowerBlueLed = 0;
	int16_t s16_RatioAdjusting = 0;
	static	bool_t 	u8_InitFlag = bfalse;
	static 	tick_t 	u16_UpdateTick = 0;
	static int16_t 	g_s16_TempJunctionFilter = 0;
	static int16_t 	g_s16_TempLedFilter = 0;

	/*Every temperature multiplied by DTEMP_MUTIPLIED_RATIO.*/
	if (u8_InitFlag == bfalse)
	{
		g_s16_TempJunctionFilter = s16_JointTemperature*D_TEMP_MULTIPLIED_RATIO;
	}
	else
	{
		g_s16_TempJunctionFilter = (g_s16_TempJunctionFilter*((int16_t)(DTEMP_MCU_FILTER_LEVEL-1))) + (int16_t)(s16_JointTemperature*(int16_t)D_TEMP_MULTIPLIED_RATIO);
		g_s16_TempJunctionFilter = g_s16_TempJunctionFilter/(int16_t)DTEMP_MCU_FILTER_LEVEL;
	}

	//Calculate the power of the LED(R/G/B)
	s32_PowerRedLed = (int32_t)ModuleLed_GetPwm(E_LED_CHANNEL_RED)*(int32_t)ModuleLed_GetVoltage(E_LED_CHANNEL_RED)/PWM_PERIOD;
	s32_PowerGreenLed = (int32_t)ModuleLed_GetPwm(E_LED_CHANNEL_GREEN)*(int32_t)ModuleLed_GetVoltage(E_LED_CHANNEL_GREEN)/PWM_PERIOD;
	s32_PowerBlueLed = (int32_t)ModuleLed_GetPwm(E_LED_CHANNEL_BLUE)*(int32_t)ModuleLed_GetVoltage(E_LED_CHANNEL_BLUE)/PWM_PERIOD;

	s32_PowerRedLed = s32_PowerRedLed*(int32_t)DLED_CURRENT_MAX;
	s32_PowerGreenLed = s32_PowerGreenLed*(int32_t)DLED_CURRENT_MAX;
	s32_PowerBlueLed = s32_PowerBlueLed*(int32_t)DLED_CURRENT_MAX;

	s32_PowerRedLed = s32_PowerRedLed*(int32_t)g_st_SavedConfig.s16_RThermolRatio;
	s32_PowerGreenLed = s32_PowerGreenLed*(int32_t)g_st_SavedConfig.s16_GThermolRatio;
	s32_PowerBlueLed = s32_PowerBlueLed*(int32_t)g_st_SavedConfig.s16_BThermolRatio;

	//Calculate the heat transfer power of mcu
	s32_PowerMcu  = (int32_t)ModuleLed_GetPwm(E_LED_CHANNEL_RED)*((int32_t)ModuleBattery_VoltageGet() - (int32_t)ModuleLed_GetVoltage(E_LED_CHANNEL_RED));
	s32_PowerMcu += (int32_t)ModuleLed_GetPwm(E_LED_CHANNEL_GREEN)*((int32_t)ModuleBattery_VoltageGet() - (int32_t)ModuleLed_GetVoltage(E_LED_CHANNEL_GREEN));
	s32_PowerMcu += (int32_t)ModuleLed_GetPwm(E_LED_CHANNEL_BLUE)*((int32_t)ModuleBattery_VoltageGet() - (int32_t)ModuleLed_GetVoltage(E_LED_CHANNEL_BLUE));

	if (ModuleBattery_VoltageGet() >= 12000)
	{
		s16_RatioAdjusting = ((int16_t)(ModuleBattery_VoltageGet() - 12000))/1000*g_st_SavedConfig.s8_RatingHigherVoltage/10;
	}
	else
	{
		s16_RatioAdjusting = ((int16_t)(12000 - ModuleBattery_VoltageGet()))/1000*g_st_SavedConfig.s8_RatingLowerVoltage/10;
	}

	s32_PowerMcu = s32_PowerMcu/(int32_t)PWM_PERIOD*((int32_t)(s16_RatioAdjusting + g_st_SavedConfig.s16_McuThermolRatio));
	s32_PowerMcu = (s32_PowerMcu*(int32_t)DLED_CURRENT_MAX) + (10*(int32_t)ModuleBattery_VoltageGet()* (int32_t)g_st_SavedConfig.s16_McuThermolRatio);
	 
	//Calculate led temperature
	s16_TempTemperature = (int16_t)((int32_t)g_s16_TempJunctionFilter -
				   ((s32_PowerMcu - s32_PowerRedLed - s32_PowerGreenLed - s32_PowerBlueLed)/(int32_t)1000000)*(int32_t)D_TEMP_MULTIPLIED_RATIO);

	s16_TempTemperature += 1*D_TEMP_MULTIPLIED_RATIO;

	//Filtering of led temperature
	if (u8_InitFlag == bfalse)
	{
		g_s16_TempLedFilter = s16_TempTemperature;
	}
	else
	{
		g_s16_TempLedFilter = (int16_t)(((int16_t)(g_s16_TempLedFilter*(DTEMP_LED_FILTER_LEVEL-1)) + s16_TempTemperature)/(int16_t)DTEMP_LED_FILTER_LEVEL);
	}

	/*we update temperature every DTEMP_UPDATE_CYCLE_PERIOD MS*/
	if ((DTickAfter(ModuleClock_TickGet(), u16_UpdateTick + DTEMP_UPDATE_CYCLE_PERIOD)) || (u8_InitFlag == bfalse))
	{
		u16_UpdateTick = ModuleClock_TickGet();
		g_s16_LedTemperature = g_s16_TempLedFilter;
		g_s16_McuTemperature = g_s16_TempJunctionFilter;
	}
	u8_InitFlag = btrue;
}


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
int16_t ModuleTemperature_LedGet(void)
{
	return g_s16_LedTemperature;
}


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
int16_t ModuleTemperature_McuGet(void)
{
	return g_s16_McuTemperature;
}
