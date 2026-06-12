/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		SystemControl.h
* Project:		RLNE15S
* Description:	This header file declares the core functions for the system control module, 
*               including color control, sleep management, and access to current and origin 
*               color values. It provides the necessary interface for RGB LED control 
*               based on LIN signals.
* Note(s):		- This module works in conjunction with Algorithm and ModuleLed for complete control.
*               - It supports sleep transitions and maintains color state across different operations.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24     Leo Song        Initial Version
* 00.02      2024.12.12     Genhua Yang     Code standardization and minor improvements
************************************************************************
*/

#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

#include "Algorithm.h"
#include "ModuleLed.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/




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
void SystemControl_Control(ST_ColorParams *pst_InputColor);

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
void SystemControl_Sleep(void);



#endif

