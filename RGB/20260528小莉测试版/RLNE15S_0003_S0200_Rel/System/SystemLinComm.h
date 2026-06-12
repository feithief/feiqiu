/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		SystemLinComm.h
* Project:		RLNE15S
* Description:	This header file declares functions and macros for managing 
*               LIN communication, including signal updates, NAD changes, 
*               and response handling for RGB system control.
* Note(s):	- Provides function prototypes for initializing and updating 
*             LIN communication.
*           - Defines macros for LIN modes and error statuses.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24     Leo Song         Initial Version
* 00.02      2024.12.12     Genhua Yang      Code standardization 
************************************************************************
*/

#ifndef SYSTEMLINCOMM_H
#define SYSTEMLINCOMM_H
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

#include "SystemType.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/



#define KEINE_SOUNDERFUNKTION 0     /*!< Normal mode >*/
#define SHOWROOMMODUS        2     /*!< Showroom mode >*/

#define KEIN_FEHLER   0  /*!< Error flag no error >*/
#define FEHLER        1  /*!< Error flag error >*/



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
void SystemLinComm_ChangeNad(uint16_t u16_NewNad);



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
bool_t SystemLinComm_SignalUpdate(void);


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
void SystemLinComm_RespUpdate(void);

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
void SystemLinComm_Sleep(void);



#endif
