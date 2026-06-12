/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		Algorithm.h
* Project:		RLNE15S
* Description:	This header file defines the data structures and function 
*               prototypes for RGB color conversion, temperature compensation, 
*               and PWM signal generation for LED control. It supports CIE1931 
*               color space conversion and temperature-based color correction.
* Note(s):		- This module processes LIN RGB signals and compensates LED 
*                 parameters based on temperature readings to generate accurate 
*                 PWM signals for LED control.
*               - LED color coordinates and luminance are represented in CIE1931 
*                 color space (x, y, Y).
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.23     Leo Song        Initial Version
* 00.02      2024.12.12     Genhua Yang     Code standardization and optimization
************************************************************************
*/

#ifndef ALGORITHM_H
#define ALGORITHM_H
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

#include "../System/SystemType.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

/** @file
*
* @defgroup RGB_Algorithm RGB Color Algorithm
* @{
*
* @brief This module converts LIN RGB signal to PWM pulse.
*
* @details This module converts LIN RGB signal to target CIE color,
*          than compensate LED parameters referring to temperature.
*          At last it generates PWM pulse.
*/

/**@brief Parameters this module needed to generate PWM.*/
typedef struct
{
	uint16_t u16_ColorR;        /**<Color Red.*/
	uint16_t u16_ColorG;        /**<Color Green.*/
	uint16_t u16_ColorB;        /**<Color Blue.*/
	int16_t  s16_LedTemprature; /**<Current LED junction temperature.*/
	uint16_t u16_Factor;        /**<Parameter for adjusting u16_Intensity.*/
	uint16_t  u16_Intensity;    /**<Intensity.*/
	uint16_t  u16_TempAdjust;   /**<Temperature protection u16_Intensity adjusting parameter.*/
}ST_ColorParams;

/**@brief this macro is to enhance the performance of dimming.*/
#define DINTENSITY_ADJUST_SHIFT_LEVEL 6u

/**@brief this macro is to enhance the performance of temperature protection dimming.*/
#define DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL 6u

/**@brief PWM pulse this module generated.*/
typedef struct
{
	uint16_t u16_PwmR;   /**<PWM Red.*/
	uint16_t u16_PwmG;   /**<PWM Green.*/
	uint16_t u16_PwmB;   /**<PWM Blue.*/
}ST_PWM_PARAMS;

/**
* @brief 		Update Red CIE1931 color coordinates (x, y) and luminance (Y).
* @note 		The Y value is scaled down by dividing it by 10 before being stored.
* @param[in]	s16_x -- x-coordinate of Red CIE1931 color space.
* @param[in]	s16_y -- y-coordinate of Red CIE1931 color space.
* @param[in]	s32_Y -- Luminance value in CIE1931 color space (before scaling).
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01				2017.10.24			 Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void Algorithm_AlgoUpdateRedCIE(int16_t s16_x, int16_t s16_y, int32_t s32_Y);

/**
* @brief 		Update Green CIE1931 color coordinates (x, y) and luminance (Y).
* @note 		The Y value is scaled down by dividing it by 10 before being stored.
* @param[in]	s16_x -- x-coordinate of Green CIE1931 color space.
* @param[in]	s16_y -- y-coordinate of Green CIE1931 color space.
* @param[in]	s32_Y -- Luminance value in CIE1931 color space (before scaling).
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void Algorithm_AlgoUpdateGreenCIE(int16_t s16_x, int16_t s16_y, int32_t s32_Y);
/**
* @brief 		Update Blue CIE1931 color coordinates (x, y) and luminance (Y).
* @note 		The Y value is scaled down by dividing it by 10 before being stored.
* @param[in]	s16_x -- x-coordinate of Blue CIE1931 color space.
* @param[in]	s16_y -- y-coordinate of Blue CIE1931 color space.
* @param[in]	s32_Y -- Luminance value in CIE1931 color space (before scaling).
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void Algorithm_AlgoUpdateBlueCIE(int16_t s16_x, int16_t s16_y, int32_t s32_Y);



/**
* @brief 		Generate PWM output for RGB LEDs based on input color parameters.
* @note 		This function calculates the PWM duty cycle for red, green, and blue LEDs 
*               using the target CIE1931 color coordinates, temperature compensation, and input intensity control signals.
* @param[in]	pst_InputColor -- Pointer to the input color structure containing RGB values, intensity, temperature, and other control parameters.
* @param[out]	pst_OutputPWM -- Pointer to the output structure containing calculated PWM duty cycles for R, G, and B.
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void Algorithm_AlgoGetPWMOutput(ST_ColorParams * const pst_InputColor, ST_PWM_PARAMS *pst_OutputPWM);



#endif
