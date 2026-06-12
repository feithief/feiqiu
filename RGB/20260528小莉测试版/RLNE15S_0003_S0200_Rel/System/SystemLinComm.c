/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemLinComm.c
 * Project:		RLNE15S
 * Description:	This file manages the LIN slave communication, including 
 *               receiving and responding to LIN signals, handling NAD changes, 
 *               and updating signal status. It supports signal update detection, 
 *               and adjusts response based on the status of connected LED channels 
 *               and temperature derating conditions.
 * Note(s):	- Includes functions for preparing LIN modules to enter sleep mode.
 *           - Uses global variables to store signal data for LIN communication.
 * Update Record:	
 * Version     	Date     	 Modified by     Why & What is modified
 * 00.01      2017.10.24     Leo Song         Initial Version
 * 00.02      2024.12.12     Genhua Yang      Code standardization 
 ************************************************************************
 */

/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

#include "SystemLinComm.h"
#include "ModuleLed.h"
#include "SystemControl.h"
#include "SystemStatus.h"
#include "SystemDerating.h"
#include "ModuleFlash.h"
#include "lin_driver_api.h"
#include "lin_app.h"
#include "genLinConfig.h"

/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/


/**
 *@details   Updates the LIN slave response signal based on the status of the LEDs 
 *           and temperature conditions. It checks for initialization errors, open 
 *           circuit errors, and short circuit errors for each LED u8_LedChannel and 
 *           updates the corresponding error flags in the LIN communication.
 *
 *@retval    None.
 */
typedef void (*ErrorHandler)(uint8_t u8_ErrorFlag);
/*--- LIN Product Identification --------------------------------------*/

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< Current color index for predefined colors */
static uint8_t g_u8_ColorIndex = 0;

/*!< Structure to hold the old LIN signal data */
static ST_SLinSignals g_st_OldLinData = {0};

/*!< PID table containing 15 predefined PID values >*/
static const uint8_t g_au8_Pid[15][2] = {
		{1, 193}, {2,  66}, {3,    3}, {4,  196}, {5,  133},  /*!<  0 -  4 >*/
		{6,   6}, {7,  71}, {8,    8}, {9,   73}, {10, 202},  /*!<  5 -  9 >*/
		{12, 76}, {17, 17}, {18, 146}, {19, 211}, {33,  97}   /*!< 10 - 14 >*/
};

/*!< Predefined color table with 20 RGB color values >*/
static const uint8_t g_au8_PreDefColor[20][3] = {
		{0xFF, 0X3D, 0X3D}, {0xFF, 0X43, 0X3D}, {0XFF, 0X77, 0X3D}, {0XFF, 0XAA, 0X47}, {0XFF, 0XEB, 0X61}, /*!<  0 -  4 >*/
		{0XF4, 0XF4, 0XE9}, {0X93, 0XFF, 0X7E}, {0X2D, 0XFF, 0X5E}, {0X2D, 0XFF, 0X9D}, {0X2D, 0XFF, 0XFF}, /*!<  5 -  9 >*/
		{0X2D, 0XD5, 0xFF}, {0X1F, 0XB9, 0XFF}, {0X1F, 0X90, 0XFF}, {0X0B, 0X6E, 0XFF}, {0X0B, 0X36, 0XFF}, /*!< 10 - 14 >*/
		{0X4F, 0X0B, 0xFF}, {0X7E, 0X0B, 0xFF}, {0XB1, 0X6C, 0xFF}, {0xFF, 0XA9, 0XCC}, {0XFF, 0X26, 0X7D}, /*!< 15 - 19 >*/
};

/*!< Group and single address table containing 15 predefined address pairs >*/
static const uint16_t g_au16_GroupSingleAddr[15][2] = {
		{0x0001, 0x21}, {0x0002, 0x02}, {0x0004, 0x13}, {0x0008, 0x04}, {0x0010, 0x05}, /*!<  0 -  4 >*/
		{0x0020, 0x06}, {0x0040, 0x07}, {0x0080, 0x08}, {0x0100, 0x09}, {0x0200, 0x0A}, /*!<  5 -  9 >*/
		{0x0400, 0X01}, {0x0800, 0X0C}, {0x1000, 0X03}, {0x2000, 0X11}, {0x4000, 0X12}  /*!< 10 - 14 >*/
};

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/


/**
 * @brief 		Change the Node Address (NAD) in the LIN communication.
 * @note 		This function updates the PID frame with the new NAD if it matches an entry in the predefined PID table.
 * @param[in]	u16_NewNad -- The new Node Address (NAD) to be set.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemLinComm_ChangeNad(uint16_t u16_NewNad)
{
		/* Iterate through the PID table to find a match for the new NAD */
		for (uint32_t u32_Cnt = 0; u32_Cnt < 15; u32_Cnt++)
		{
				/* Check if the current entry in the PID table matches the new NAD */
				if (g_au8_Pid[u32_Cnt][0] == u16_NewNad)
				{
						/* Update the PID frame with the corresponding frame value for the new NAD */
						LIN_CHANGE_PID_FRAME_ATL8_FB_CLIN1(g_au8_Pid[u32_Cnt][1]);

						/* Break the loop once the matching entry is found and the PID frame is updated */
						break;
				}
		}
}




/**
 * @brief 		Update RGB color values based on the current LIN signal.
 * @note 		This function updates the RGB values from either a predefined color array or custom color values received through LIN signals.
 *               The selection depends on the `bool_IsPredefined` flag.
 * @param[in]	bool_IsPredefined -- Boolean flag indicating the color source:
 *                                 - `0`: Use predefined color values.
 *                                 - `1`: Use custom color values from LIN signals.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinComm_UpdateColorValues(bool_t bool_IsPredefined) 
{
		/* Check if the color update should use predefined color values */
		if (bool_IsPredefined == 0) 
		{
				/* Use predefined color values */

				/* Read the predefined color index from LIN signal */
				g_u8_ColorIndex = l_u8_rd_CDCU_SigColorR_Predef_CLIN1();

				/* Store the current predefined color index in the system LIN structure */
				g_st_sysLin.u8_BcmRgbPreColorNumber = g_u8_ColorIndex;

				/* Update RGB values using the predefined color array */
				g_st_sysLin.u8_BcmRgbBlau = g_au8_PreDefColor[g_u8_ColorIndex][2];  // Blue component
				g_st_sysLin.u8_BcmRgbGruen = g_au8_PreDefColor[g_u8_ColorIndex][1]; // Green component
				g_st_sysLin.u8_BcmRgbRot = g_au8_PreDefColor[g_u8_ColorIndex][0];   // Red component
		} 
		else 
		{
				/* Use custom color values directly from LIN signals */

				/* Update RGB values using LIN signals */
				g_st_sysLin.u8_BcmRgbRot = l_u8_rd_CDCU_SigColorR_Predef_CLIN1();  // Red component
				g_st_sysLin.u8_BcmRgbGruen = l_u8_rd_CDCU_SigColorG_CLIN1();       // Green component
				g_st_sysLin.u8_BcmRgbBlau = l_u8_rd_CDCU_SigColorB_CLIN1();        // Blue component
		}
}


/**
 * @brief 		Save the current LIN signal data as the old signal data.
 * @note 		This function verifies that the intensity and dimming ramp values are within valid ranges before saving the current LIN data.
 *               The old data is used for comparison to detect changes in future signal updates.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinComm_SaveOldSignalData(void) 
{
		/* Check if the RGB intensity and dimming ramp values are within valid ranges */
		if (g_st_sysLin.u8_BcmRgbIntensitaet <= 100u && g_st_sysLin.u8_BcmRgbDimmrampe <= 250u) 
		{
				/* Save the current LIN signal data to the old signal data structure */
				g_st_OldLinData = *(ST_SLinSignals *)l_LinData.frames.l_frm_CDCU_ATLCTR_CLIN1.frame_data;
		}
}



/**
 * @brief 		Check and update LIN communication signal status.
 * @note 		This function verifies if there are updates in the LIN signals by comparing the current signal data with the old data.
 *               It updates the LIN data accordingly and sets a flag if a new signal is detected. The function also validates fading time and other parameters.
 * @return		`btrue` if there is a new signal update; otherwise, `bfalse`.
 * @retval		Returns `btrue` when a new LIN signal is detected and processed.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
bool_t SystemLinComm_SignalUpdate(void)
{
		bool_t bool_LinFlag = bfalse;
		static uint32_t g_u32_SignalUpdateFlag = 0;  // Signal update flag to track changes

		do {
				/* Update NAD (Node Address) from LIN signals */

				g_st_sysLin.ST_LED_ID.Group_Adresse.BcmRgbSlaveAdresse = (((uint16_t)(l_u8_rd_CDCU_SigNAD_LEDID2_CLIN1()) << 8) | (uint16_t)(l_u8_rd_CDCU_SigNAD_LEDID1_CLIN1())); // Read new NAD
				/* Iterate through predefined group addresses to check if the current NAD is valid */
				for (volatile uint32_t u32_Cnt = 0; u32_Cnt < 15; u32_Cnt++)
				{
						if (g_au16_GroupSingleAddr[u32_Cnt][1] == g_st_SavedConfig.u16_SingleAddr)
						{
								if (0 != (g_st_sysLin.ST_LED_ID.Group_Adresse.BcmRgbSlaveAdresse & g_au16_GroupSingleAddr[u32_Cnt][0]))
								{
										g_u32_SignalUpdateFlag = 1;  // Set update flag if valid NAD is found
										break;
								}
						}
				}

				/* Check if any signal value is out of range and reset the update flag if necessary */
				if ((l_u8_rd_CDCU_Sig_FadingTime_CLIN1() > 250u) ||
								((l_bool_rd_CDCU_SigEnable_ColorProtocol_CLIN1() == 0u) && 
								 (l_u8_rd_CDCU_SigColorR_Predef_CLIN1() > 19u)) ||
								(l_u8_rd_CDCU_SigIntensity_CLIN1() > 100u))
				{
						g_u32_SignalUpdateFlag = 0;  // Reset update flag if any condition is invalid
				}

				if (g_u32_SignalUpdateFlag == 1)
				{
						/* Check if any of the key LIN signal data has changed */
						if ((g_st_OldLinData.u8_BcmRgbRot != l_u8_rd_CDCU_SigColorR_Predef_CLIN1()) ||
										(g_st_OldLinData.u8_BcmRgbGruen != l_u8_rd_CDCU_SigColorG_CLIN1()) ||
										(g_st_OldLinData.u8_BcmRgbBlau != l_u8_rd_CDCU_SigColorB_CLIN1()) ||
										(g_st_OldLinData.u8_BcmRgbIntensitaet != l_u8_rd_CDCU_SigIntensity_CLIN1()) ||
										(g_st_OldLinData.u8_BcmRgbDimmrampe != l_u8_rd_CDCU_Sig_FadingTime_CLIN1()) ||
										(g_st_OldLinData.u8_CdcuSigEnableColorProtocol != l_bool_rd_CDCU_SigEnable_ColorProtocol_CLIN1()) ||
										(g_st_OldLinData.u8_CdcuSigEnableLed != l_bool_rd_CDCU_SigEnable_LED_CLIN1()) ||
										(g_st_OldLinData.u8_CdcuSigEnableFading != l_bool_rd_CDCU_SigEnable_Fading_CLIN1()))
						{
								/* Update current signal status with new values */
								g_st_sysLin.u8_CdcuSigEnableLed = l_bool_rd_CDCU_SigEnable_LED_CLIN1();
								g_st_sysLin.u8_CdcuSigEnableColorProtocol = l_bool_rd_CDCU_SigEnable_ColorProtocol_CLIN1();
								g_st_sysLin.u8_CdcuSigEnableFading = l_bool_rd_CDCU_SigEnable_Fading_CLIN1();
								g_st_sysLin.u8_BcmRgbIntensitaet = l_u8_rd_CDCU_SigIntensity_CLIN1();

								if (g_st_sysLin.u8_CdcuSigEnableLed == 1)
								{
										/* Update RGB values if the LED is enabled */
										SystemLinComm_UpdateColorValues(g_st_sysLin.u8_CdcuSigEnableColorProtocol);
								}
								else
								{
										/* Turn off LED and reset RGB values if it is disabled */
										g_st_sysLin.u8_BcmRgbRot = 0;
										g_st_sysLin.u8_BcmRgbGruen = 0;
										g_st_sysLin.u8_BcmRgbBlau = 0;
										g_st_sysLin.u8_BcmRgbIntensitaet = 0;
								}

								/* Update dimming ramp time if fading is enabled */
								if (g_st_sysLin.u8_CdcuSigEnableFading == 1)
								{
										g_st_sysLin.u8_BcmRgbDimmrampe = l_u8_rd_CDCU_Sig_FadingTime_CLIN1();
								}
								else
								{
										g_st_sysLin.u8_BcmRgbDimmrampe = 0;
								}

								/* Save the current signal data as old data for future comparisons */
								(void)SystemLinComm_SaveOldSignalData();

								/* Indicate that a new LIN signal has been received */
								g_st_SysStatus.u8_NewLinSignal = btrue;

								/* Reset the update flag and set the return flag to true */
								g_u32_SignalUpdateFlag = 0;
								bool_LinFlag = btrue;
								break;
						}
				}

				/* Reset the update flag if no valid update is found */
				g_u32_SignalUpdateFlag = 0;

		} while (0);

		/* Return the flag indicating if a new LIN signal was detected and processed */
		return bool_LinFlag;
}


/**
 * @brief 		Put the LIN communication module into sleep mode by resetting old LIN data.
 * @note 		This function clears the previous LIN data values, effectively turning off the LEDs and stopping any ongoing dimming.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemLinComm_Sleep(void)
{
		/* Reset old LIN data to turn off LEDs */

		/* Set the red component of the old LIN data to zero */
		g_st_OldLinData.u8_BcmRgbRot = 0;

		/* Set the green component of the old LIN data to zero */
		g_st_OldLinData.u8_BcmRgbGruen = 0;

		/* Set the blue component of the old LIN data to zero */
		g_st_OldLinData.u8_BcmRgbBlau = 0;

		/* Set the intensity of the old LIN data to zero */
		g_st_OldLinData.u8_BcmRgbIntensitaet = 0;

		/* Set the dimming ramp of the old LIN data to zero */
		g_st_OldLinData.u8_BcmRgbDimmrampe = 0;
}



/**
 * @brief 		Handle the red LED error and update the LIN communication flag accordingly.
 * @note 		This function sets the LIN communication error flag for the red LED based on the error status.
 *               If an error is detected (`u8_ErrorFlag` is non-zero), it sets the flag to `FEHLER`; otherwise, it sets it to `KEIN_FEHLER`.
 * @param[in]	u8_ErrorFlag -- Error flag indicating the status of the red LED (1 for error, 0 for no error).
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinComm_HandleRedError(uint8_t u8_ErrorFlag) 
{
		/* Write the error status to the LIN signal for the red output error */
		l_bool_wr_ATL8_ROutputErr_CLIN1((u8_ErrorFlag != 0) ? FEHLER : KEIN_FEHLER);
}


/**
 * @brief 		Handle the green LED error and update the LIN communication flag accordingly.
 * @note 		This function sets the LIN communication error flag for the green LED based on the error status.
 *               If an error is detected (`u8_ErrorFlag` is non-zero), it sets the flag to `FEHLER`; otherwise, it sets it to `KEIN_FEHLER`.
 * @param[in]	u8_ErrorFlag -- Error flag indicating the status of the green LED (1 for error, 0 for no error).
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinComm_HandleGreenError(uint8_t u8_ErrorFlag) 
{
		/* Write the error status to the LIN signal for the green output error */
		l_bool_wr_ATL8_GOutput_Err_CLIN1((u8_ErrorFlag != 0) ? FEHLER : KEIN_FEHLER);
}



/**
 * @brief 		Handle the blue LED error and update the LIN communication flag accordingly.
 * @note 		This function sets the LIN communication error flag for the blue LED based on the error status.
 *               If an error is detected (`u8_ErrorFlag` is non-zero), it sets the flag to `FEHLER`; otherwise, it sets it to `KEIN_FEHLER`.
 * @param[in]	u8_ErrorFlag -- Error flag indicating the status of the blue LED (1 for error, 0 for no error).
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinComm_HandleBlueError(uint8_t u8_ErrorFlag) 
{
		/* Write the error status to the LIN signal for the blue output error */
		l_bool_wr_ATL8_Butput_Err_CLIN1((u8_ErrorFlag != 0) ? FEHLER : KEIN_FEHLER);
}


/**
 * @brief 		Update the LED error status and call the corresponding error handler based on the LED channel.
 * @note 		This function verifies the validity of the LED channel and invokes the appropriate error handler for red, green, or blue LEDs.
 *               The error flag indicates whether an open or short circuit error has occurred.
 * @param[in]	u8_LedChannel -- The LED channel (red, green, or blue).
 * @param[in]	u8_ErrorFlag -- Error flag indicating the status of the LED (1 for error, 0 for no error).
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinComm_UpdateLedError(uint8_t u8_LedChannel, uint8_t u8_ErrorFlag)
{
		do {
				/* Array of error handler functions for red, green, and blue LED channels */
				ErrorHandler ap_Handlers[] = {SystemLinComm_HandleRedError, SystemLinComm_HandleGreenError, SystemLinComm_HandleBlueError};

				/* Check if the LED channel is within a valid range (RED to BLUE) */
				if ((u8_LedChannel < (uint8_t)E_LED_CHANNEL_RED) || 
								(u8_LedChannel > (uint8_t)E_LED_CHANNEL_BLUE)) 
				{
						/* Exit the loop if the LED channel is out of range */
						break;
				}

				/* Call the appropriate error handler for the specified LED channel */
				ap_Handlers[u8_LedChannel - 2](u8_ErrorFlag); 

		} while (0);
}




/**
 * @brief 		Update the LIN communication response with the current status of the LEDs.
 * @note 		This function retrieves the status of the red, green, and blue LEDs, checks for open or short circuit errors,
 *               and updates the corresponding error flags in the LIN communication response.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemLinComm_RespUpdate(void)
{
		/* Get the status of the red LED, casting the status value to an 8-bit unsigned integer */
		uint8_t u8_StatusRed = (uint8_t)ModuleLed_GetStatus(E_LED_CHANNEL_RED);

		/* Get the status of the green LED, casting the status value to an 8-bit unsigned integer */
		uint8_t u8_StatusGreen = (uint8_t)ModuleLed_GetStatus(E_LED_CHANNEL_GREEN);

		/* Get the status of the blue LED, casting the status value to an 8-bit unsigned integer */
		uint8_t u8_StatusBlue = (uint8_t)ModuleLed_GetStatus(E_LED_CHANNEL_BLUE);

		/* Update the error status of the red LED
		   - Check if the status contains an open or short circuit error.
		   - If an error exists, call SystemLinComm_UpdateLedError to set the error flag. */
		SystemLinComm_UpdateLedError(
						(uint8_t)E_LED_CHANNEL_RED,
						(uint8_t)((u8_StatusRed & ((uint8_t)E_LED_STATUS_OPEN | (uint8_t)E_LED_STATUS_SHORT)) != 0)
						);

		/* Update the error status of the green LED
		   - Check if the status contains an open or short circuit error.
		   - If an error exists, call SystemLinComm_UpdateLedError to set the error flag. */
		SystemLinComm_UpdateLedError(
						(uint8_t)E_LED_CHANNEL_GREEN,
						(uint8_t)((u8_StatusGreen & ((uint8_t)E_LED_STATUS_OPEN | (uint8_t)E_LED_STATUS_SHORT)) != 0)
						);

		/* Update the error status of the blue LED
		   - Check if the status contains an open or short circuit error.
		   - If an error exists, call SystemLinComm_UpdateLedError to set the error flag. */
		SystemLinComm_UpdateLedError(
						(uint8_t)E_LED_CHANNEL_BLUE,
						(uint8_t)((u8_StatusBlue & ((uint8_t)E_LED_STATUS_OPEN | (uint8_t)E_LED_STATUS_SHORT)) != 0)
						);
}


