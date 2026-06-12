/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemControl.c
 * Project:		RLNE15S
 * Description:	This file manages system functions, including color dimming, 
 *               intensity control, and sleep mode transition. It provides the 
 *               core logic for RGB LED control using LIN signals and handles 
 *               error states and temperature-based adjustments.
 * Note(s):		- This module supports gradual color and intensity transitions.
 *               - The color and intensity dimming are managed separately for 
 *                 more precise control.
 * Update Record:	
 * Version     	Date     	 Modified by     Why & What is modified
 * 00.01      2017.10.24     Leo Song        Initial Version
 * 00.02      2024.12.12     Genhua Yang     Code standardization and optimization
 ************************************************************************
 */
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
#include "SystemControl.h"
#include "SystemStatus.h"
#include "SystemLinComm.h"
#include "SystemMain.h"
#include "ModuleTemperature.h"
#include "ModuleFlash.h"
#include "ModuleClock.h"
#include "SystemDerating.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/

/*!< Intensity dimming flag, used to indicate if intensity dimming is currently active. */
static uint8_t g_u8_IntensityDimmingFlag = bfalse;

/*!< Time slice period for dimming operation, default is 20 ms. */
static uint32_t g_u32_DimmingTimeSlicePeriod = 20;

/*!< Color dimming flag, used to indicate if color dimming is currently active. */
static uint8_t g_u8_ColorDimmingFlag = bfalse;

/*!< Total number of ticks required for the complete intensity dimming process. */
static uint16_t g_u16_IntensityTotaltick = 0;

/*!< Record the last mode, initial value based on LDF file configuration. */
static uint8_t g_u8_OldMode = KEINE_SOUNDERFUNKTION;

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/



/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< Crash and actuator use, blink flag */
static uint8_t g_u8_IntensityLastStatus = bfalse;

/*!< Show-room mode and dimming use, new show room mode color flag */
static uint8_t g_u8_NewShowroomColor = bfalse;

/*!< Total tick count for color transitions */
static uint16_t g_u16_ColorTotalTick = 0;

/*!< Current Red, Green, and Blue LED values during transition */
static uint16_t g_u16_CurrentR = 0, g_u16_CurrentG = 0, g_u16_CurrentB = 0;

/*!< Original Red, Green, and Blue LED values at the start of transition */
static uint16_t g_u16_OriginR = 0, g_u16_OriginG = 0, g_u16_OriginB = 0;

/*!< Tick count for color transitions */
static uint16_t g_u16_ColorTickCount = 0;

/*!< Current and original Yellow LED values for intensity control */
static uint16_t g_u16_CurrentY = 0, g_u16_OriginY = 0;

/*!< Tick count for intensity transitions */
static uint16_t g_u16_IntensityTickCount = 0;

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/



/**
 * @brief 		Perform intensity dimming based on the current tick, operating mode, and input color parameters.
 * @note 		This function handles the gradual change (dimming) of intensity over a specified time period.
 *               The dimming process starts when a new LIN signal is detected and continues until the total dimming duration is reached.
 * @param[in]	u16_CurrentTick -- Current system tick used to track time for dimming.
 * @param[in]	u8_Mode -- Current operating mode of the system.
 * @param[in,out]	pst_InputColor -- Pointer to the input color structure, which will be modified for intensity dimming.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemControl_IntensityDimming(tick_t u16_CurrentTick, uint8_t u8_Mode, ST_ColorParams *pst_InputColor)
{
		uint32_t u32_Temp;
		static tick_t u16_IntensityStartTick;

		/* Check if a new LIN signal is detected */
		if (g_st_SysStatus.u8_NewLinSignal == btrue)
		{
				/* When switching the current mode or if the mode is not normal, stop dimming immediately */
				if (((u8_Mode == g_u8_OldMode) &&
										(u8_Mode == KEINE_SOUNDERFUNKTION)))
				{
						/* Initialize intensity dimming parameters */
						u16_IntensityStartTick = u16_CurrentTick;
						g_u16_IntensityTickCount = 0;
						g_u16_IntensityTotaltick = (uint16_t)g_st_sysLin.u8_BcmRgbDimmrampe * ((uint16_t)g_u32_DimmingTimeSlicePeriod);
						/* Save the initial intensity value for dimming calculations */
						g_u16_OriginY = g_u16_CurrentY;
						g_u8_IntensityDimmingFlag = btrue;  // Enable intensity dimming
				}
				else
				{
						/* Disable intensity dimming if the mode is not suitable */
						g_u8_IntensityDimmingFlag = bfalse;
				}
		}

		/* Check if the dimming duration is complete or if total ticks are set to zero */
		if ((DTickAfter(u16_CurrentTick, (u16_IntensityStartTick + g_u16_IntensityTotaltick)) != 0) ||
						(g_u16_IntensityTotaltick == 0))
		{
				g_u8_IntensityDimmingFlag = bfalse;  // Stop dimming
		}

		/* Perform intensity dimming if the dimming flag is enabled */
		if (g_u8_IntensityDimmingFlag == btrue)
		{
				/* Calculate the elapsed tick count since the start of dimming */
				if (u16_CurrentTick > u16_IntensityStartTick)
				{
						g_u16_IntensityTickCount = u16_CurrentTick - u16_IntensityStartTick;
				}
				else
				{
						/* Handle tick overflow by wrapping around the maximum tick value */
						g_u16_IntensityTickCount = (uint16_t)(DMAX_SYSTEM_TICK - (u16_IntensityStartTick - u16_CurrentTick) + 1u);
				}

				/* Adjust intensity based on the elapsed ticks and total dimming duration */
				if (g_u16_IntensityTickCount < g_u16_IntensityTotaltick)
				{
						/* If the original intensity is greater than the target, reduce the intensity */
						if (g_u16_OriginY > pst_InputColor->u16_Intensity)
						{
								u32_Temp = ((uint32_t)g_u16_OriginY - (uint32_t)pst_InputColor->u16_Intensity)*(uint32_t)((uint32_t)g_u16_IntensityTotaltick - (uint32_t)g_u16_IntensityTickCount);
								u32_Temp = u32_Temp/(uint32_t)g_u16_IntensityTotaltick;
								pst_InputColor->u16_Intensity += (uint16_t)u32_Temp;
						}
						else
						{
								/* If the target intensity is greater than the original, increase the intensity */
								u32_Temp = ((uint32_t)pst_InputColor->u16_Intensity - (uint32_t)g_u16_OriginY)*(uint32_t)((uint32_t)g_u16_IntensityTotaltick - (uint32_t)g_u16_IntensityTickCount);
								u32_Temp = u32_Temp / (uint32_t)g_u16_IntensityTotaltick;
								pst_InputColor->u16_Intensity -= (uint16_t)u32_Temp;
						}
				}
		}

		/* Update the current intensity value */
		g_u16_CurrentY = pst_InputColor->u16_Intensity;
}



/**
 * @brief 		Update the color component value gradually over a specified time period.
 * @note 		This function adjusts the current color component value (`pu16_CurrentColor`) towards the target color (`u16_OriginColor`)
 *               in proportion to the elapsed time (`u32_colorTickCount`) relative to the total transition time (`u32_ColorTotalTick`).
 * @param[in,out]	pu16_CurrentColor -- Pointer to the current color component to be updated.
 * @param[in]	u16_OriginColor -- The target color component value to reach.
 * @param[in]	u32_ColorTotalTick -- Total number of ticks for the color transition.
 * @param[in]	u32_colorTickCount -- Current tick count indicating the progress of the transition.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemControl_UpdateColorComponent(uint16_t *pu16_CurrentColor, uint16_t u16_OriginColor, uint32_t u32_ColorTotalTick, uint32_t u32_colorTickCount)
{
		uint32_t u32_Temp;

		/* Check if the target color value (u16_OriginColor) is greater than the current value (*pu16_CurrentColor) */
		if (u16_OriginColor > *pu16_CurrentColor)
		{
				/* Calculate the adjustment value based on the remaining tick count and the difference between target and current color */
				u32_Temp = ((uint32_t)u16_OriginColor - (uint32_t)(*pu16_CurrentColor)) * ((uint32_t)u32_ColorTotalTick - (uint32_t)u32_colorTickCount);
				u32_Temp /= (uint32_t)u32_ColorTotalTick;

				/* Increase the current color component value by the calculated adjustment */
				*pu16_CurrentColor += (uint16_t)u32_Temp;
		}
		else
		{
				/* Calculate the adjustment value if the current color is greater than the target color */
				u32_Temp = ((uint32_t)(*pu16_CurrentColor) - (uint32_t)u16_OriginColor) * ((uint32_t)u32_ColorTotalTick - (uint32_t)u32_colorTickCount);
				u32_Temp /= (uint32_t)u32_ColorTotalTick;

				/* Decrease the current color component value by the calculated adjustment */
				*pu16_CurrentColor -= (uint16_t)u32_Temp;
		}
}


/**
 * @brief 		Perform color dimming over a specified time period for RGB components.
 * @note 		This function gradually adjusts the RGB color components (`u16_ColorR`, `u16_ColorG`, `u16_ColorB`) 
 *               towards target values based on the elapsed time, current operating mode, and input parameters.
 *               Dimming starts when a new LIN signal or showroom color update is detected.
 * @param[in]	u16_CurrentTick -- Current system tick used to track time for dimming.
 * @param[in]	u8_Mode -- Current operating mode of the system.
 * @param[in,out]	pst_InputColor -- Pointer to the input color structure, which will be updated during the dimming process.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemControl_ColorDimming(tick_t u16_CurrentTick, uint8_t u8_Mode, ST_ColorParams *pst_InputColor)
{
		uint32_t u32_Temp;
		static tick_t u16_ColorStartTick;

		/* Check if a new LIN signal or a new showroom color update is detected */
		if ((g_st_SysStatus.u8_NewLinSignal == btrue) || (g_u8_NewShowroomColor == btrue))
		{
				/* Verify if the current mode matches the old mode and is in a valid mode for color dimming */
				if ((u8_Mode == g_u8_OldMode) && ((u8_Mode == KEINE_SOUNDERFUNKTION) || (u8_Mode == SHOWROOMMODUS)))
				{
						/* Initialize color dimming parameters */
						u16_ColorStartTick = u16_CurrentTick;
						g_u16_ColorTickCount = 0;
						g_u16_ColorTotalTick = (uint16_t)g_st_sysLin.u8_BcmRgbDimmrampe * ((uint16_t)g_u32_DimmingTimeSlicePeriod);

						/* Determine the initial color values for the dimming process */
						if (pst_InputColor->u16_Intensity == 0)
						{
								/* If the intensity is zero, use the input color as the origin */
								g_u16_OriginR = pst_InputColor->u16_ColorR;
								g_u16_OriginG = pst_InputColor->u16_ColorG;
								g_u16_OriginB = pst_InputColor->u16_ColorB;
						}
						else
						{
								/* Otherwise, use the current color values as the origin */
								g_u16_OriginR = g_u16_CurrentR;
								g_u16_OriginG = g_u16_CurrentG;
								g_u16_OriginB = g_u16_CurrentB;
						}

						/* Enable the color dimming process */
						g_u8_ColorDimmingFlag = btrue;
				}
				else
				{
						/* Disable color dimming if the mode is not valid */
						g_u8_ColorDimmingFlag = bfalse;
				}
		}

		/* Check if the dimming duration is complete or if the total tick count is zero */
		if ((DTickAfter(u16_CurrentTick, (u16_ColorStartTick + g_u16_ColorTotalTick)) != 0) || (g_u16_ColorTotalTick == 0))
		{
				g_u8_ColorDimmingFlag = bfalse;  // Stop the color dimming process
		}

		/* Perform color dimming if the dimming flag is enabled */
		if (g_u8_ColorDimmingFlag == btrue)
		{
				/* Calculate the elapsed tick count since the start of color dimming */
				if (u16_CurrentTick > u16_ColorStartTick)
				{
						g_u16_ColorTickCount = u16_CurrentTick - u16_ColorStartTick;
				}
				else
				{
						/* Handle tick overflow by wrapping around the maximum tick value */
						g_u16_ColorTickCount = (uint16_t)(DMAX_SYSTEM_TICK - (u16_ColorStartTick - u16_CurrentTick) + 1u);
				}

				/* Update the red, green, and blue color components based on the elapsed time */
				SystemControl_UpdateColorComponent(&pst_InputColor->u16_ColorR, g_u16_OriginR, g_u16_ColorTotalTick, g_u16_ColorTickCount);
				SystemControl_UpdateColorComponent(&pst_InputColor->u16_ColorG, g_u16_OriginG, g_u16_ColorTotalTick, g_u16_ColorTickCount);
				SystemControl_UpdateColorComponent(&pst_InputColor->u16_ColorB, g_u16_OriginB, g_u16_ColorTotalTick, g_u16_ColorTickCount);
		}

		/* Save the current color values for the next iteration */
		g_u16_CurrentR = pst_InputColor->u16_ColorR;
		g_u16_CurrentG = pst_InputColor->u16_ColorG;
		g_u16_CurrentB = pst_InputColor->u16_ColorB;
}



/**
 * @brief 		Put the system into sleep mode by resetting color and mode-related parameters.
 * @note 		This function sets the current RGB and intensity values to zero and changes the mode to `KEINE_SOUNDERFUNKTION`.
 * @param		None.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemControl_Sleep(void)
{
		/* Reset the current red color value to zero */
		g_u16_CurrentR = 0;

		/* Reset the current green color value to zero */
		g_u16_CurrentG = 0;

		/* Reset the current blue color value to zero */
		g_u16_CurrentB = 0;

		/* Reset the current intensity (Y component) to zero */
		g_u16_CurrentY = 0;

		/* Set the old mode to KEINE_SOUNDERFUNKTION, indicating no sound function */
		g_u8_OldMode = KEINE_SOUNDERFUNKTION;
}



/**
 * @brief 		Control system operations, including dimming, temperature protection, and sleep management.
 * @note 		This function manages the overall control flow for the system, handling intensity and color dimming, 
 *               checking temperature, and preparing the system for sleep mode if conditions are met.
 * @param[in,out]	pst_InputColor -- Pointer to the input color structure containing RGB values, intensity, and temperature.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemControl_Control(ST_ColorParams *pst_InputColor)
{
		// Get the current tick count from the system clock
		tick_t u16_CurrentTick = ModuleClock_TickGet();

		/* Check if the system is preparing for sleep */
		if (g_st_SysStatus.e_SleepFlag == E_SYSTEM_SLEEP_PREPARE)
		{
				// Set the sleep flag to processing state, indicating the sleep process has started
				g_st_SysStatus.e_SleepFlag = E_SYSTEM_SLEEP_PROCESSING;

				// Initialize RGB intensity to zero
				g_st_sysLin.u8_BcmRgbIntensitaet = 0;

				/* Set the dimming ramp rate based on the system error flag */
				if (g_st_SysStatus.u8_ErrFlag == SYSTEM_ERROR_NONE)
				{
						// If no error, set the dimming ramp rate to 2
						g_st_sysLin.u8_BcmRgbDimmrampe = 2;
				}
				else
				{
						// If an error exists, set the dimming ramp rate to 0
						g_st_sysLin.u8_BcmRgbDimmrampe = 0;
				}

				// Indicate that a new LIN signal has been generated
				g_st_SysStatus.u8_NewLinSignal = btrue;
		}

		/* Initialize control parameters */
		// Set the color factor from the saved configuration
		pst_InputColor->u16_Factor = g_st_SavedConfig.u16_Factor;

		// Adjust intensity level based on LIN signal data and shift level
		pst_InputColor->u16_Intensity = (uint16_t)g_st_sysLin.u8_BcmRgbIntensitaet << DINTENSITY_ADJUST_SHIFT_LEVEL;

		// Get the current LED temperature and assign it to the input color structure
		pst_InputColor->s16_LedTemprature = ModuleTemperature_LedGet();

		/* Assign RGB color values from the LIN signal to the input color structure */
		pst_InputColor->u16_ColorR = g_st_sysLin.u8_BcmRgbRot;
		pst_InputColor->u16_ColorG = g_st_sysLin.u8_BcmRgbGruen;
		pst_InputColor->u16_ColorB = g_st_sysLin.u8_BcmRgbBlau;

		/* Perform intensity and color dimming operations */
		// Set the time slice period for dimming operations
		g_u32_DimmingTimeSlicePeriod = 20;

		// Control intensity dimming based on the current tick and input color
		SystemControl_IntensityDimming(u16_CurrentTick, 0, pst_InputColor);

		// Control color dimming based on the current tick and input color
		SystemControl_ColorDimming(u16_CurrentTick, 0, pst_InputColor);

		/* Reset the new LIN signal and showroom color flags */
		g_st_SysStatus.u8_NewLinSignal = bfalse;
		g_u8_NewShowroomColor = bfalse;

		/* Perform temperature protection by applying derating if necessary */
		SystemDerating_TempDerating(pst_InputColor);

		/* Check if the system should enter sleep mode */
		if ((g_st_SysStatus.e_SleepFlag == E_SYSTEM_SLEEP_PROCESSING) && (pst_InputColor->u16_Intensity == 0))
		{
				// Set the sleep flag to sleep state, indicating the system is now in sleep mode
				g_st_SysStatus.e_SleepFlag = E_SYSTEM_SLEEP_SLEEP;
		}
}




