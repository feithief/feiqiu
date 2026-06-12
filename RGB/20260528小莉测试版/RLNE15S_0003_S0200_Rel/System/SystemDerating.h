/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		SystemDerating.h
* Project:		RLNE15S
* Description:	This header file provides the interface for the temperature derating module. 
*               It contains declarations for functions and macros used to perform LED derating 
*               based on temperature conditions, ensuring system stability and longevity.
* Note(s):		- This module adjusts color intensity to prevent overheating and damage.
*               - It operates in conjunction with the Algorithm and SystemType modules.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24     Leo Song        Initial Version
* 00.02      2024.12.12     Genhua Yang     Code standardization and macro definition added
************************************************************************
*/

#ifndef SYSTEMDERATING_H
#define SYSTEMDERATING_H
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

#include "SystemType.h"
#include "Algorithm.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

#define D_TEMP_RANGE    7 /*!< Define the number of temperature ranges >*/


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
void SystemDerating_TempDerating(ST_ColorParams *st_InputColor);



#endif

