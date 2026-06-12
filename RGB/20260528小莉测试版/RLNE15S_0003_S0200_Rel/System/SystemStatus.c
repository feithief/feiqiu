/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemStatus.c
 * Project:		RLNE15S
 * Description:	This file contains functions for initializing and updating 
 *               the system status, including battery status, LED status, 
 *               temperature, and LIN signals. It ensures error handling 
 *               and status management for efficient system operation.
 * Note(s):	- This module provides system-wide status monitoring for 
 *                 diagnostics and safety-critical functions.
 *           - Global variables are used to store the system status 
 *                 and LIN control signals.
 * Update Record:	
 * Version     	Date     	 Modified by     Why & What is modified
 * 00.01      2024.11.26       Genhua Yang           Initial Version
 ************************************************************************
 */
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
#include "SystemStatus.h"
#include "ModuleTemperature.h"
#include "ModuleBattery.h"
#include "ModuleLed.h"
#include "SystemLinComm.h"
#include "lin_app.h"
#include "genLinConfig.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< This variable shows system status. It's dangerous to make this external, only for smaller code size. */
ST_SystemStatus g_st_SysStatus;

/*!< This variable shows current LIN control signals. It's dangerous to make this external, only for smaller code size. */
ST_SLinSignals g_st_sysLin;

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/


/**
 * @brief 		Initialize the system status and set default values for LIN communication and error flags.
 * @note 		This function resets LIN-related variables, clears error flags, and sets the system to awake state.
 *               It ensures the system starts with a clean state and prepares for LIN communication.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemStatus_Init(void)
{
		/* Initialize LIN communication parameters */
		g_st_sysLin.u8_BcmRgbBlau = 0;                  // Set blue component of RGB to 0
		g_st_sysLin.u8_BcmRgbDimmrampe = 0;             // Set dimming ramp to 0
		g_st_sysLin.u8_BcmRgbGruen = 0;                 // Set green component of RGB to 0
		g_st_sysLin.u8_BcmRgbIntensitaet = 0;           // Set RGB intensity to 0
		g_st_sysLin.u8_BcmRgbRot = 0;                   // Set red component of RGB to 0
		g_st_sysLin.u8_CdcuSigEnableColorProtocol = 1;  // Enable color protocol by default

		/* Initialize system status parameters */
		g_st_SysStatus.u8_ErrFlag = SYSTEM_ERROR_NONE;  // Set system error flag to no error
		g_st_SysStatus.u8_NewLinSignal = btrue;         // Indicate that a new LIN signal is available
		g_st_SysStatus.e_SleepFlag = E_SYSTEM_SLEEP_AWAKE;  // Set sleep flag to awake state
		g_e_SleepType = E_SLEEP_TYPE_IDLE;              // Set sleep type to idle mode

		/* System status initialized to default values */
}



/**
 * @brief 		Update the current system status by checking battery, LED, and temperature conditions.
 * @note 		This function processes the battery status, performs LED open/short detection, and updates temperature readings.
 *               It sets or clears the corresponding error flags based on the status of each component.
 *               If a new LIN signal is detected while an LED error exists, the LED detection process is reset.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemStatus_Update(void)
{
		/* Process battery status */
		if (ModuleBattery_StatusUpdate() != E_BATSTATUS_NORMAL)
		{
				// If battery status is not normal, set battery error flag and update LIN error signal
				g_st_SysStatus.u8_ErrFlag = (g_st_SysStatus.u8_ErrFlag | (uint8_t)SYSTEM_ERROR_BAT_ERROR);
				l_bool_wr_ATL8_Voltage_Err_CLIN1(1); // Indicate battery voltage error in LIN signal
		}
		else
		{
				// If battery status is normal, clear battery error flag and update LIN error signal
				g_st_SysStatus.u8_ErrFlag = (g_st_SysStatus.u8_ErrFlag & (~((uint8_t)SYSTEM_ERROR_BAT_ERROR)));
				l_bool_wr_ATL8_Voltage_Err_CLIN1(0); // Clear battery voltage error in LIN signal
		}

		/* Process LED open/short detection */
		ModuleLed_StatusUpdate(); // Check and update the status of LED channels (open/short detection)

		/* Process temperature status */
		ModuleTemperature_Update(); // Update temperature status for MCU and LED, and handle over-temperature protection

		/* If a new LIN signal is received and an LED error is detected, reset LED detection */
		if ((((uint8_t)g_st_SysStatus.u8_ErrFlag & (uint8_t)SYSTEM_ERROR_LED_ERROR) != 0) &&
						(g_st_SysStatus.u8_NewLinSignal == btrue))
		{
				ModuleLed_DetectionReset(); // Reset LED detection process
		}
}



