/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemDerating.c
 * Project:		RLNE15S
 * Description:	This file contains functions for temperature-based 
 *               derating of LED and MCU components. It manages temperature 
 *               readings, computes appropriate intensity levels, and 
 *               adjusts output based on thermal conditions.
 *               Functions include the calculation of derating levels for 
 *               both LEDs and the MCU and dynamically adjust the output 
 *               based on predefined temperature ranges.
 * Note(s):	    - This module is intended for use in temperature-sensitive 
 *                 applications where LED and MCU components require 
 *                 adaptive intensity adjustments.
 *               - Global variables are used to store the current temperature 
 *                 levels and adjustment factors.
 * Update Record:	
 * Version     	Date     	 Modified by     Why & What is modified
 * 00.01         2017.10.24     Leo Song         Initial Version
 * 00.02         2024.10.28     Genhua Yang      Code standardization 
 ************************************************************************
 */
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

#include "SystemDerating.h"  
#include "ModuleTemperature.h" 
#include "lin_app.h"
#include "genLinConfig.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

typedef enum
{
		E_DERATING_SOURCE_LED  = 0x1, /*!< LED as the derating source! */
		E_DERATING_SOURCE_MCU  = 0x2, /*!< MCU as the derating source! */
} E_DERATING_SOURCE;


/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< Current temperature protection level */
static uint8 g_u8_TempProtectLevel = 0;

/*!< Current levels for LED and MCU */
static uint8 g_u8_CurrentLedLevel = 0, g_u8_CurrentMcuLevel = 0;

/*!< Current adjustment factor */
static uint16 g_u16_CurrentAdjust = 6400u;

/*!< Maximum temperature of LED and MCU */
static sint16 g_s8_TemperatureMax = 0;

/*!< Current intensity level */
static uint8 g_u8_CurrentLevel = 0;

/*!< Define the temperature ranges for the MCU */
static const sint16 as16_TempMcuRange[D_TEMP_RANGE]={
    -40 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 0 */
    114 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 1 */
    118 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 2 */
    122 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 3 */
    126 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 4 */
    130 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 5 */
    135 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 6 */
};

/*!< Define the temperature ranges for the LED */
static const sint16 as16_TempLedRange[D_TEMP_RANGE]={
    -40 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 0 */
    110 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 1 */
    114 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 2 */
    118 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 3 */
    122 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 4 */
    125 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 5 */
    129 * D_TEMP_MULTIPLIED_RATIO,        /*!< Level 6 */
};

/*!< Define the intensity levels corresponding to temperature ranges */
static const uint8 g_au8_IntensityLevel[D_TEMP_RANGE+1]={
    100,  /*!< Level 0 */
    100,  /*!< Level 1 */
    90,   /*!< Level 2 */
    80,   /*!< Level 3 */
    70,   /*!< Level 4 */
    60,   /*!< Level 5 */
    0,    /*!< Level 6 */
    0     
};

/*!< Variables to store last temperature readings for LED and MCU */
static sint16 g_s16_TemperatureLed = 0, g_s16_TemperatureMcu = 0;

/*!< Last recorded temperature readings for LED and MCU */
volatile static sint16 g_s16_LastTemperatureLed = 0, g_s16_LastTemperatureMcu = 0;

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/


/**
 * @brief 		Determine the derating level based on the given temperature and temperature range.
 * @note 		This function checks the input temperature against a predefined range and returns the corresponding derating level.
 *               It prevents level jumping by adding hysteresis to the temperature comparison.
 * @param[in]	s16_Temperature -- Current temperature value.
 * @param[in]	ps16_TempRange -- Pointer to the array of temperature range values.
 * @return		Current derating level as an 8-bit unsigned integer.
 * @retval		Returns the appropriate level based on the temperature range.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static uint8 SystemDerating_GetLevel(sint16 s16_Temperature, const sint16 *ps16_TempRange)
{
	static uint8 u8_Level = 0;
	uint16 u16_Cnt = 0;

	/* Determine the initial level based on whether the temperature range is for LED or MCU */
	if (ps16_TempRange == as16_TempLedRange)
	{
			/* Use the current LED level if the temperature range corresponds to the LED */
			u8_Level = g_u8_CurrentLedLevel;
	}
	else
	{
			/* Use the current MCU level if the temperature range corresponds to the MCU */
			u8_Level = g_u8_CurrentMcuLevel;
	}

	/* Determine the level based on the given temperature and range */
	if (s16_Temperature <= ps16_TempRange[0])
	{
		/* If the temperature is less than or equal to the lowest range, keep the initial level */
		;
	}

	else if (s16_Temperature >= (ps16_TempRange[D_TEMP_RANGE - 1]) )
	{
		/* If the temperature exceeds the highest range, set the level to the maximum defined level */
		u8_Level = D_TEMP_RANGE - 1;
	}
	else
	{
		/* Check which range the current temperature falls into */
		for (u16_Cnt = 0; u16_Cnt < (D_TEMP_RANGE - 1); u16_Cnt++)
		{
			/* Prevent the level from jumping back by applying a hysteresis of 1 * D_TEMP_MULTIPLIED_RATIO */
			if (((s16_Temperature >= (ps16_TempRange[u16_Cnt] + (1 * D_TEMP_MULTIPLIED_RATIO))) &&
			    (s16_Temperature < (ps16_TempRange[u16_Cnt + 1] - (1 * D_TEMP_MULTIPLIED_RATIO)))) && 
					(s16_Temperature < (ps16_TempRange[D_TEMP_RANGE - 2])))
			{
				/* Set the level to the current index if the temperature falls within the range */
				u8_Level = (uint8_t)u16_Cnt;
				break;
			}
			else if (((s16_Temperature >= (ps16_TempRange[u16_Cnt] + (1 * D_TEMP_MULTIPLIED_RATIO))) &&
			    (s16_Temperature < (ps16_TempRange[u16_Cnt + 1] - (2 * D_TEMP_MULTIPLIED_RATIO)))) && 
					(s16_Temperature >= (ps16_TempRange[D_TEMP_RANGE - 2])))
			{
				u8_Level = (uint8_t)u16_Cnt;
				break;
			}
			else
			{
				;
			}
		}
	}

		/* Return the determined level */
	return u8_Level;
}



/**
 * @brief 		Update and determine the current derating source based on LED and MCU temperature levels.
 * @note 		This function retrieves the current temperatures for LED and MCU, updates the corresponding derating levels, 
 *               and determines the derating source by comparing the LED and MCU levels.
 * @return		Derating source (E_DERATING_SOURCE_LED or E_DERATING_SOURCE_MCU).
 * @retval		Returns `E_DERATING_SOURCE_LED` if the LED level is greater than or equal to the MCU level; otherwise, returns `E_DERATING_SOURCE_MCU`.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static E_DERATING_SOURCE SystemDerating_UpdateLevel(void)
{
		E_DERATING_SOURCE e_DeratingSource;

		/* Get the current LED temperature */
		g_s16_TemperatureLed = ModuleTemperature_LedGet();

		/* Get the current MCU temperature */
		g_s16_TemperatureMcu = ModuleTemperature_McuGet();

		/* Update the current derating level for the LED based on its temperature range */
		g_u8_CurrentLedLevel = SystemDerating_GetLevel(g_s16_TemperatureLed, as16_TempLedRange);

		/* Update the current derating level for the MCU based on its temperature range */
		g_u8_CurrentMcuLevel = SystemDerating_GetLevel(g_s16_TemperatureMcu, as16_TempMcuRange);

		/* Determine the primary derating source based on the higher level between LED and MCU */
		e_DeratingSource = (g_u8_CurrentLedLevel >= g_u8_CurrentMcuLevel) ? E_DERATING_SOURCE_LED : E_DERATING_SOURCE_MCU;

		/* Return the derating source */
		return e_DeratingSource;
}







/**
 * @brief 		Update the current derating level and maximum temperature based on the derating source.
 * @note 		This function sets the current derating level and retrieves the corresponding maximum temperature 
 *               depending on whether the derating source is LED or MCU.
 * @param[in]	e_Source -- Derating source (LED or MCU).
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemDerating_UpdateCurrentLevelAndTemp(E_DERATING_SOURCE e_Source)
{
		/* Check if the derating source is from the LED */
		if (e_Source == E_DERATING_SOURCE_LED) 
		{
				/* Update the current level with the LED derating level */
				g_u8_CurrentLevel = g_u8_CurrentLedLevel;

				/* Update the maximum temperature with the current LED temperature */
				g_s8_TemperatureMax = ModuleTemperature_LedGet();
		} 
		else if (e_Source == E_DERATING_SOURCE_MCU) 
		{
				/* Update the current level with the MCU derating level */
				g_u8_CurrentLevel = g_u8_CurrentMcuLevel;

				/* Update the maximum temperature with the current MCU temperature */
				g_s8_TemperatureMax = ModuleTemperature_McuGet();
		} 
		else 
		{
				/* No specific action required for other sources */
				;
		}
}


/**
 * @brief 		Calculate the intensity adjustment for temperature values above the current range.
 * @note 		This function computes the adjustment based on the difference between current and next intensity levels, 
 *               taking into account the temperature range. It ensures that the adjusted intensity does not drop below zero.
 * @param[in]	s32_CurrentIntensity -- Current intensity level in fixed-point format.
 * @param[in]	s32_NextIntensity -- Next intensity level in fixed-point format.
 * @param[in]	ps16_TempRange -- Pointer to the array of temperature range values.
 * @param[in]	u8_CurrentLevel -- Current derating level index.
 * @return		Adjusted intensity value as a 32-bit unsigned integer.
 * @retval		Returns the adjusted intensity value, ensuring it does not go below zero.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static uint32_t SystemDerating_CalculateIntensityAdjustmentAboveRange(sint32 s32_CurrentIntensity, sint32 s32_NextIntensity, const sint16* ps16_TempRange, uint8 u8_CurrentLevel)
{
		uint32_t u32_Adjusted = 0;
		sint32 s32_Adjustment = 0;

		/* Calculate the intensity adjustment based on the current temperature and the difference between the current and next intensity levels */
		s32_Adjustment = (s32_CurrentIntensity - s32_NextIntensity) * ((int32_t)g_s8_TemperatureMax - (int32_t)ps16_TempRange[u8_CurrentLevel]) /
				((int32_t)ps16_TempRange[u8_CurrentLevel + 1] - (int32_t)ps16_TempRange[u8_CurrentLevel]);

		/* Ensure the adjusted intensity is not negative; if it is, set it to zero */
		u32_Adjusted = ((s32_CurrentIntensity >= s32_Adjustment) ? (uint32_t)(s32_CurrentIntensity - s32_Adjustment) : 0u); // Adjust intensity

		/* Return the adjusted intensity value */
		return u32_Adjusted;
}


/**
 * @brief 		Calculate the intensity adjustment for temperature values below the current range.
 * @note 		This function computes the adjustment based on the difference between current and next intensity levels, 
 *               taking into account the temperature range and ensuring the adjustment does not exceed 100% intensity.
 * @param[in]	s32_CurrentIntensity -- Current intensity level in fixed-point format.
 * @param[in]	s32_NextIntensity -- Next intensity level in fixed-point format.
 * @param[in]	ps16_TempRange -- Pointer to the array of temperature range values.
 * @param[in]	u8_CurrentLevel -- Current derating level index.
 * @return		Calculated intensity adjustment as a 32-bit unsigned integer.
 * @retval		Returns the adjusted intensity value in fixed-point format, capped at 100% if necessary.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static uint32_t SystemDerating_CalculateIntensityAdjustmentBelowRange(sint32 s32_CurrentIntensity, sint32 s32_NextIntensity, const sint16* ps16_TempRange, uint8 u8_CurrentLevel)
{
		sint32 s32_Adjustment = 0;

		/* Calculate the adjustment value based on the temperature difference and intensity change */

		s32_Adjustment = (s32_CurrentIntensity - s32_NextIntensity) * ((int32_t)ps16_TempRange[u8_CurrentLevel] - (int32_t)g_s8_TemperatureMax) /
				((int32_t)ps16_TempRange[u8_CurrentLevel + 1] - (int32_t)ps16_TempRange[u8_CurrentLevel]);

		/* Add the calculated adjustment to the current intensity */
		s32_CurrentIntensity += s32_Adjustment;

		/* Ensure the adjusted intensity does not exceed the maximum allowable value (100 shifted by the adjustment level) */
		if (s32_CurrentIntensity >= (int32_t)((uint32_t)100 << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL)){
				/* Cap the intensity value to the maximum allowed */
				s32_CurrentIntensity = (int32_t)((uint32_t)100 << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL);
		}

		/* Return the adjusted intensity as an unsigned 32-bit integer */
		return (uint32_t)s32_CurrentIntensity;
}



/**
 * @brief 		Calculate the target intensity adjustment based on the derating source and current temperature level.
 * @note 		This function determines the appropriate target intensity by comparing the current intensity level 
 *               with the next level and adjusting it based on the temperature range. It handles both above and below range cases.
 * @param[in]	e_Source -- Derating source (LED or MCU).
 * @return		Target intensity adjustment value.
 * @retval		Returns the calculated intensity adjustment in fixed-point format.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static sint16 SystemDerating_CalculateTargetIntensityAdjust(E_DERATING_SOURCE e_Source)
{
		sint16 u16_TargetIntensityAdjust = 0;
		int32_t s32_CurrentIntensity = 0;
		int32_t s32_NextIntensity = 0;

		/* Select the appropriate temperature range based on the derating source (LED or MCU) */
		const int16_t* ps16_TempRange = (e_Source == E_DERATING_SOURCE_LED) ? as16_TempLedRange : as16_TempMcuRange;

		/* Get the current derating level */
		uint8 u8_CurrentLevel = g_u8_CurrentLevel;

		do {
				/* Check if the intensity level remains the same between the current and the next level */
				if (g_au8_IntensityLevel[u8_CurrentLevel] == g_au8_IntensityLevel[u8_CurrentLevel + 1]) {
						/* If there is no change in intensity, calculate the target intensity directly */
						u16_TargetIntensityAdjust = (uint16_t)((uint16_t)g_au8_IntensityLevel[u8_CurrentLevel] << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL);
						break;
				}

				/* Calculate the current intensity in shifted format */
				s32_CurrentIntensity = (int32_t)((uint32_t)g_au8_IntensityLevel[u8_CurrentLevel] << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL);

				/* Calculate the next intensity in shifted format */
				s32_NextIntensity = (int32_t)((uint32_t)g_au8_IntensityLevel[u8_CurrentLevel + 1] << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL);

				/* Check if the maximum temperature exceeds or equals the current level's temperature range */
				if (g_s8_TemperatureMax >= ps16_TempRange[u8_CurrentLevel]) {
						/* Calculate the intensity adjustment for the above range */
						u16_TargetIntensityAdjust = (uint16_t)(SystemDerating_CalculateIntensityAdjustmentAboveRange(s32_CurrentIntensity, s32_NextIntensity, ps16_TempRange, u8_CurrentLevel));
				} 
				else 
				{
						/* Calculate the intensity adjustment for the below range */
						u16_TargetIntensityAdjust = (uint16_t)(SystemDerating_CalculateIntensityAdjustmentBelowRange(s32_CurrentIntensity, s32_NextIntensity, ps16_TempRange, u8_CurrentLevel));
				}

		} while (0);

		/* Return the target intensity adjustment */
		return u16_TargetIntensityAdjust;
}




/**
 * @brief 		Apply dimming adjustment towards the target intensity adjustment value.
 * @note 		This function smoothly adjusts the current intensity value (`g_u16_CurrentAdjust`) 
 *               to approach the target intensity (`u16_TargetIntensityAdjust`) in small steps to avoid sudden changes.
 *               If the difference is too small, it directly sets the current adjustment to the target value.
 * @param[in]	u16_TargetIntensityAdjust -- Target intensity adjustment value.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemDerating_ApplyDimmingAdjustment(uint16_t u16_TargetIntensityAdjust)
{
		uint16 u16_Delta = 0;

		/* Calculate the adjustment based on the difference between the current and target intensity */
		if (g_u16_CurrentAdjust < u16_TargetIntensityAdjust)
		{
				/* If the current adjustment is less than the target, calculate the difference (delta) */
				u16_Delta = ((u16_TargetIntensityAdjust - g_u16_CurrentAdjust) >> (DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL + 1));

				/* Increase the current adjustment by the calculated delta */
				g_u16_CurrentAdjust += u16_Delta;
		}
		else if (g_u16_CurrentAdjust > u16_TargetIntensityAdjust)
		{
				/* If the current adjustment is greater than the target, calculate the difference (delta) */
				u16_Delta = ((g_u16_CurrentAdjust - u16_TargetIntensityAdjust) >> (DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL + 1));

				/* Decrease the current adjustment by the calculated delta */
				g_u16_CurrentAdjust -= u16_Delta;
		}
		else
		{
				/* If the current adjustment already equals the target, no adjustment is needed */
				;
		}

		/* If the calculated delta is zero, directly set the current adjustment to the target intensity */
		if (u16_Delta == 0)
		{
				g_u16_CurrentAdjust = u16_TargetIntensityAdjust;
		}
}



/**
 * @brief 		Perform temperature-based derating and adjust the intensity accordingly.
 * @note 		This function updates the derating level and calculates the target intensity adjustment 
 *               based on the current temperature. It applies the adjustment to dim the output and updates 
 *               the input color structure with the latest adjustment value.
 * @param[in,out]	st_InputColor -- Pointer to the input color structure, which will be updated with temperature adjustment values.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemDerating_TempDerating(ST_ColorParams *st_InputColor) 
{
		static uint16_t u16_TargetIntensityAdjust;  // Target intensity adjustment value
		E_DERATING_SOURCE e_Source = 0;

		/* Update the derating source (LED or MCU) based on the current temperature */
		e_Source = SystemDerating_UpdateLevel();

		/* Update the current derating level and maximum temperature based on the source */
		SystemDerating_UpdateCurrentLevelAndTemp(e_Source);

		/* Calculate the target intensity adjustment based on the derating source */
		u16_TargetIntensityAdjust = SystemDerating_CalculateTargetIntensityAdjust(e_Source);

		/* Apply the dimming adjustment to gradually approach the target intensity */
		SystemDerating_ApplyDimmingAdjustment(u16_TargetIntensityAdjust);

		/* Update the output color parameters with the current adjustment value */
		st_InputColor->u16_TempAdjust = g_u16_CurrentAdjust;

		/* Update the temperature protection level */
		g_u8_TempProtectLevel = g_u8_CurrentLevel;
 
		/* Set the temperature error flag in the LIN interface if the protection level is not zero */
		l_bool_wr_ATL8_Temp_Err_CLIN1((uint8_t)((g_s16_TemperatureLed >= as16_TempLedRange[1]) || (g_s16_TemperatureMcu >= as16_TempMcuRange[1])));

		/* Save the current LED and MCU temperatures as the last recorded temperatures */
		g_s16_LastTemperatureLed = g_s16_TemperatureLed;
		g_s16_LastTemperatureMcu = g_s16_TemperatureMcu;
}





