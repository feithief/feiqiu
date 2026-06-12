/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemMain.h
 * Project:		RLNE15S
 * Description:	This header file declares functions for the system's main 
 *              control loop and initialization. It manages system status, 
 *              real-time LIN signal processing, and module-level operations.
 * Note(s):	    - The main loop runs at a fixed 10ms interval, synchronized 
 *                with the LIN signal update frequency.
 * Update Record:	
 * Version     	Date     	 Modified by       Changes Made
 * 00.01      2024.11.28      Genhua Yang      Initial version
 ************************************************************************
 */

#ifndef SYSTEMMAIN_H
#define SYSTEMMAIN_H

/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

#include "Algorithm.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/


/*!< Defines system main loop time period, because our LIN signal time is 10ms, we set 10ms as well >*/
#define DSYSTEM_EVENT_LOOP_PERIOD   10 /*!< 10ms >*/


/**
* @brief 		Initialize the main system components, including hardware modules, communication protocols, and algorithms.
* @note 		This function sets up the device hardware, initializes key modules like ADC, battery, and LED control, 
*               and configures the LIN communication interface. It ensures all system flags and parameters are set to their initial values.
*               A watchdog is also initialized to monitor system health during operation.
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void SystemMain_Init(void);

/**
* @brief 		Main system loop for managing LIN communication, LED control, and system status.
* @note 		This function runs continuously, handling flash updates, LIN communication tasks, 
*               system control, and power management, including transitioning the system into sleep mode when necessary.
*               It also ensures that system events are processed at regular intervals.
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void SystemMain_Loop(void);


extern ST_ColorParams inputColor;

#endif
