/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		Algorithm.c
 * Project:		RLNE15S
 * Description:	This file contains algorithms used to generate PWM signals and 
 *               process fixed LIN signals for LED color management. The algorithms 
 *               use fixed color coordinates for red, green, and blue LEDs to 
 *               calculate the color temperature and apply PWM adjustments.
 * Note(s):		- This module supports LED color and brightness adjustments 
 *                 based on preset color values.
 *               - The algorithms rely on pre-computed fixed values for LED color 
 *                 coordinates, stored as global constants.
 * Update Record:	
 * Version     	Date     	 Modified by     Why & What is modified
 * 00.01      2017.10.23     Leo Song        Initial Version
 * 00.02      2024.12.12     Genhua Yang     Code standardization and optimization
 ************************************************************************
 */
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

#include "Algorithm.h"
#include "FloatLib.h"
#include "ModuleLed.h"
#include "ModuleTemperature.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< Fix16 constant for multiplying by 2.5 */
static const int32_t g_s32_Fix16Const25 = F16(25);

/*!< Fix16 constant representing 10000 in fixed-point format */
static const int32_t g_s32_Fix16Const10000 = 10000 * 0x00010000;

/*!< Intensity adjustment constant (0.79 in fix16 format) */
static const int32_t g_s32_IntensityAdjustConst = F16(0.79);

/*!< Structure to represent CIE xyY coordinates (16-bit for x and y, 32-bit for Y) */
typedef struct {
		/*!< CIE x-coordinate, 16-bit signed integer */
		int16_t s16_x;

		/*!< CIE y-coordinate, 16-bit signed integer */
		int16_t s16_y;

		/*!< CIE Y (luminance) value, 32-bit signed integer */
		int32_t s32_Y;
} ST_CIEXYY;


/*!< Structure to represent CIE color converted to fix16 floating format */
typedef struct {
		/*!< CIE x-coordinate in fix16 format (scaled integer representation) */
		int32_t s32_x;

		/*!< CIE y-coordinate in fix16 format (scaled integer representation) */
		int32_t s32_y;

		/*!< CIE Y (luminance) value in fix16 format (scaled integer representation) */
		int32_t s32_Y;
} ST_CIECOLOR;


/*!< Original Red, Green, and Blue colors' CIE coordinates */
static ST_CIEXYY g_st_RedxyY = {0}, g_st_GreenxyY = {0}, g_st_BluexyY = {0};

/*!< CIE coordinates for R, G, B colors after temperature compensation */
static ST_CIECOLOR g_st_CurrentRedCIE = {0}, g_st_CurrentGreenCIE = {0}, g_st_CurrentBlueCIE = {0};

/**
 * @brief Structure for temperature compensation.
 * @note For more details about this structure, please refer to the development document.
 */
typedef struct {
		int16_t  s16_Temperature;    /*!< Temperature */

		int16_t  s16_RedYShift;      /*!< Red Luminous shift at current temperature (percentage, multiplied by 1000) */
		int16_t  s16_GreenYShift;    /*!< Green Luminous shift at current temperature (percentage, multiplied by 1000) */
		int16_t  s16_BlueYshift;     /*!< Blue Luminous shift at current temperature (percentage, multiplied by 1000) */

		int16_t  s16_RedxShift;      /*!< Red x-coordinate shift at current temperature */
		int16_t  s16_RedyShift;      /*!< Red y-coordinate shift at current temperature */

		int16_t  s16_GreenxShift;    /*!< Green x-coordinate shift at current temperature */
		int16_t  s16_GreenyShift;    /*!< Green y-coordinate shift at current temperature */

		int16_t  s16_BluexShift;     /*!< Blue x-coordinate shift at current temperature */
		int16_t  s16_BlueyShift;     /*!< Blue y-coordinate shift at current temperature */
} ST_LED_TEMP_SHIFT;

/*!< Maximum amount of temperature shift records */
#define DMAX_TEMP_SHIFT_RECORD_AMOUNT 8

/*!< Table of temperature shift records for LED compensation */
static const ST_LED_TEMP_SHIFT g_ast_TtoCie[DMAX_TEMP_SHIFT_RECORD_AMOUNT] = {
		{-40,  1480, 1126, 1064, -75,  69, -183, 125,  16, -39},
		{-20,  1331, 1090, 1046, -52,  47, -127,  88,   8, -27},
		{-10,  1257, 1072, 1037, -41,  37,  -99,  69,   4, -21},
		{  0,  1193, 1053, 1028, -30,  26,  -71,  50,   0, -15},
		{ 25,  1000, 1000, 1000,   0,   0,    0,   0,   0,   0},
		{ 50,   765,  967,  995,  27, -29,   67, -39,  -9,  15},
		{ 85,   505,  894,  956,  62, -66,  169, -114, -24, 36},
		{120,   275,  833,  922, 103, -104, 265, -174, -47, 57}
};

/*!< Transformation matrix for converting RGB to CIE XYZ in fix16 format */
static const int32_t g_s32_M[3][3] = {
		{F16(0.4124564), F16(0.3575761), F16(0.1804375)},
		{F16(0.2126729), F16(0.7151522), F16(0.0721750)},
		{F16(0.0193339), F16(0.1192920), F16(0.9503041)}
};

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/


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
void Algorithm_AlgoUpdateRedCIE(int16_t s16_x, int16_t s16_y, int32_t s32_Y)
{
		g_st_RedxyY.s16_x = s16_x;              // Update the red LED's x coordinate
		g_st_RedxyY.s16_y = s16_y;              // Update the red LED's y coordinate
		g_st_RedxyY.s32_Y = s32_Y / 10;         // Update the luminous intensity, scaling down by 10
}

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
void Algorithm_AlgoUpdateGreenCIE(int16_t s16_x, int16_t s16_y, int32_t s32_Y)
{
		g_st_GreenxyY.s16_x = s16_x;             // Update the green LED's x chromaticity coordinate
		g_st_GreenxyY.s16_y = s16_y;             // Update the green LED's y chromaticity coordinate
		g_st_GreenxyY.s32_Y = s32_Y / 10;        // Update the green LED's luminous intensity, scaled down by 10
}


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
void Algorithm_AlgoUpdateBlueCIE(int16_t s16_x, int16_t s16_y, int32_t s32_Y)
{
		g_st_BluexyY.s16_x = s16_x;             // Update the blue LED's x chromaticity coordinate
		g_st_BluexyY.s16_y = s16_y;             // Update the blue LED's y chromaticity coordinate
		g_st_BluexyY.s32_Y = s32_Y / 10;        // Update the blue LED's luminous intensity, scaled down by 10
}



/**
 * @brief 		Convert RGB value to linear color space.
 * @note 		Performs linearization using a polynomial approximation if the input value exceeds a threshold.
 * @param[in]	s32_V -- The input RGB value in fixed-point format (Q16).
 * @return		Converted linear color space value in fixed-point format (Q16).
 * @retval		Returns the linearized color space value.
 *               If s32_V less than 0.04045, returns s32_V / 12.92.
 *               Otherwise, calculates the result using a cubic polynomial approximation.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static int32_t Algorithm_RGBtolinear(int32_t s32_V) 
{
		int32_t s32_Vsq = 0;          // Squared value of s32_V (V^2)
		int32_t s32_Vcubed = 0;       // Cubed value of s32_V (V^3)
		int32_t s32_result = 0;       // Result of the polynomial calculation
		int32_t s32_ret = 0;          // Final return value

		// Convert RGB to linear space
		if (s32_V <= F16(0.04045)) {
				s32_ret = FloatLib_Fix16Div(s32_V, F16(12.92));  // Linear conversion for small values
		} else {
				// Polynomial calculation for larger values
				s32_Vsq = FloatLib_Fix16Mul(s32_V, s32_V);       // Calculate V^2
				s32_Vcubed = FloatLib_Fix16Mul(s32_Vsq, s32_V);  // Calculate V^3

				// Polynomial formula calculation
				s32_result = FloatLib_Fix16Mul(F16(0.2925), s32_Vcubed);  // 0.2925 * V^3
				s32_result = FloatLib_Fix16Add(s32_result, FloatLib_Fix16Mul(F16(0.7093), s32_Vsq));  // + 0.7093 * V^2
				s32_result = FloatLib_Fix16Sub(s32_result, FloatLib_Fix16Mul(F16(0.0039), s32_V));    // - 0.0039 * V
				s32_result = FloatLib_Fix16Add(s32_result, F16(0.0027));                              // + 0.0027

				s32_ret = s32_result;  // Set the final result
		}

		return s32_ret;  // Return the converted linear value
}

/**
 * @brief 		Convert sRGB values to CIE 1931 XYZ color space.
 * @note 		Converts sRGB to linear RGB space first, then applies a transformation matrix to obtain XYZ values.
 * @param[in]	ps32_RGB -- Pointer to the input sRGB values array (fixed-point format, Q16).
 * @param[out]	ps32_XYZ -- Pointer to the output XYZ values array (fixed-point format, Q16).
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void Algorithm_SRGBtoXYZ(const int32_t *ps32_RGB, int32_t *ps32_XYZ) 
{
		int32_t s32_RGBLinear[3] = {0};  // Linear space representation of the input RGB
		int32_t s32_V = 0;               // Temporary variable for scaled RGB values
		uint32_t s32_cnt = 0, s32_cntj = 0;  // Loop counters

		// Scale RGB values to the [0, 1] range and convert to linear space
		for (s32_cnt = 0; s32_cnt < 3; s32_cnt++) {
				s32_V = FloatLib_Fix16Div(ps32_RGB[s32_cnt], 255);  // Scale RGB value to [0, 1]
				s32_RGBLinear[s32_cnt] = Algorithm_RGBtolinear(s32_V);  // Convert to linear space using custom function
		}

		// Convert linear RGB to XYZ using the sRGB to XYZ transformation matrix
		for (s32_cnt = 0; s32_cnt < 3; s32_cnt++) {
				ps32_XYZ[s32_cnt] = 0;  // Initialize the current XYZ component to 0

				// Perform matrix multiplication to calculate XYZ components
				for (s32_cntj = 0; s32_cntj < 3; s32_cntj++) {

						ps32_XYZ[s32_cnt] = FloatLib_Fix16Add(ps32_XYZ[s32_cnt], FloatLib_Fix16Mul(g_s32_M[s32_cnt][s32_cntj], s32_RGBLinear[s32_cntj])); // ???????
						// Add the result of g_s32_M * s32_RGBLinear to the current XYZ component
				}
		}
}




/**
 * @brief 		Convert input color parameters to CIE 1931 xyY color space.
 * @note 		Converts sRGB values to XYZ color space, then calculates xy chromaticity coordinates and adjusts luminance Y.
 * @param[in]	pst_InputColor -- Pointer to input color structure containing RGB values and intensity.
 * @param[out]	pst_MxyzY -- Pointer to output CIE1931 color structure containing x, y, and Y values.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void Algorithm_AlgoGetTargetColor(const ST_ColorParams *pst_InputColor, ST_CIECOLOR *pst_MxyzY) {
		int32_t s32_rgb[3] = {0};   // Store input RGB values
		int32_t s32_xyz[3] = {0};   // Store converted XYZ values

		int32_t s32_X = 0;	// Store input XYZ values
		int32_t s32_Y = 0;	// Store input XYZ values
		int32_t s32_Z = 0;	// Store input XYZ values

		int32_t s32_Total = 0;	// Store input X+Y+Z values
		// Assign RGB values from input color parameters
		s32_rgb[0] = (int32_t)(pst_InputColor->u16_ColorR);  // Red component
		s32_rgb[1] = (int32_t)(pst_InputColor->u16_ColorG);  // Green component
		s32_rgb[2] = (int32_t)(pst_InputColor->u16_ColorB);  // Blue component

		// Convert sRGB to XYZ color space
		Algorithm_SRGBtoXYZ(s32_rgb, s32_xyz);

		s32_X = s32_xyz[0];  // Extract X component
		s32_Y = s32_xyz[1];  // Extract Y component
		s32_Z = s32_xyz[2];  // Extract Z component

		// Calculate chromaticity coordinates (x, y)
		s32_Total = FloatLib_Fix16Add(FloatLib_Fix16Add(s32_X, s32_Y), s32_Z);  // Sum of X, Y, Z components

		if (s32_Total == 0) {
				pst_MxyzY->s32_x = 0;  // Prevent division by zero
				pst_MxyzY->s32_y = 0;
		} else {
				pst_MxyzY->s32_x = FloatLib_Fix16Div(s32_X, s32_Total);  // Calculate x coordinate
				pst_MxyzY->s32_y = FloatLib_Fix16Div(s32_Y, s32_Total);  // Calculate y coordinate
		}

		pst_MxyzY->s32_Y = s32_Y;  // Assign the Y component for luminance

		// Adjust luminance based on input intensity and predefined scaling factors
		pst_MxyzY->s32_Y = FloatLib_Fix16Div(pst_InputColor->u16_Intensity, 6400);  // Normalize intensity
		pst_MxyzY->s32_Y = FloatLib_Fix16Mul(F16(2.86), pst_MxyzY->s32_Y);          // Scale by 2.86 lm
		pst_MxyzY->s32_Y = FloatLib_Fix16Mul(s32_Y, pst_MxyzY->s32_Y);                 // Final Y value adjustment

		// Prevent negative values for chromaticity coordinates and luminance
		if (pst_MxyzY->s32_x < 0)
		{
				pst_MxyzY->s32_x = 0;
		}
		if (pst_MxyzY->s32_y < 0)
		{
				pst_MxyzY->s32_y = 0;
		}
		if (pst_MxyzY->s32_Y < 0) 
		{
				pst_MxyzY->s32_Y = 0;
		}
}



/**
 * @brief 		Adjust LED CIE1931 color coordinates (x, y, Y) based on temperature.
 * @note 		The function calculates color shifts for red, green, and blue components using linear interpolation 
 *               based on the LED temperature and updates the current CIE1931 color coordinates.
 * @param[in]	s16_LedTemp -- The LED temperature value used for calculating the corresponding color shift.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void Algorithm_AlgoLedFixCIE(int16_t s16_LedTemp) {
		int16_t s16_FixLevel = -1, s16_Cnt;
		int16_t s16_Linep, s16_Redp, s16_Greenp, s16_Bluep;
		int16_t s16_TempShift;

		// Determine the temperature range for adjustment by comparing LED temperature with predefined temperature ranges
		for (s16_Cnt = 0; s16_Cnt < (DMAX_TEMP_SHIFT_RECORD_AMOUNT - 1); s16_Cnt++) {
				if ((s16_LedTemp >= g_ast_TtoCie[s16_Cnt].s16_Temperature) &&
								(s16_LedTemp < g_ast_TtoCie[s16_Cnt + 1].s16_Temperature)) {
						s16_FixLevel = s16_Cnt;  // Store the level for interpolation
						break;
				}
		}

		// If temperature is higher than 120 degree, set the maximum fix level
		if (s16_LedTemp > 120) {
				s16_FixLevel = 7;
		}

		// If no appropriate range is found, default to the first level
		if (s16_FixLevel == -1) {
				s16_Cnt = 0;
		}

		// Calculate the temperature shift for interpolation
		s16_TempShift = (int16_t)(g_ast_TtoCie[s16_Cnt + 1].s16_Temperature - g_ast_TtoCie[s16_Cnt].s16_Temperature);

		/** Fix Red Color **/
		// Interpolate the red Y shift value based on the current LED temperature
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_RedYShift - g_ast_TtoCie[s16_Cnt].s16_RedYShift) << 4) / s16_TempShift);
		s16_Redp = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_RedYShift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentRedCIE.s32_Y = FloatLib_Fix16Div(FloatLib_Fix16FromInt(s16_Redp * g_st_RedxyY.s32_Y / 1000), g_s32_Fix16Const10000);
		g_st_CurrentRedCIE.s32_Y = FloatLib_Fix16Mul(g_st_CurrentRedCIE.s32_Y, g_s32_Fix16Const25);

		// Interpolate the red x coordinate shift
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_RedxShift - g_ast_TtoCie[s16_Cnt].s16_RedxShift) << 4) / s16_TempShift);
		s16_Redp = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_RedxShift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentRedCIE.s32_x = FloatLib_Fix16Div(FloatLib_Fix16FromInt((int32_t)(g_st_RedxyY.s16_x + s16_Redp)), g_s32_Fix16Const10000);

		// Interpolate the red y coordinate shift
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_RedyShift - g_ast_TtoCie[s16_Cnt].s16_RedyShift) << 4) / s16_TempShift);
		s16_Redp = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_RedyShift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentRedCIE.s32_y = FloatLib_Fix16Div(FloatLib_Fix16FromInt((int32_t)(g_st_RedxyY.s16_y + s16_Redp)), g_s32_Fix16Const10000);

		/** Fix Green Color **/
		// Interpolate the green Y shift value
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_GreenYShift - g_ast_TtoCie[s16_Cnt].s16_GreenYShift) << 4) / s16_TempShift);
		s16_Greenp = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_GreenYShift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentGreenCIE.s32_Y = FloatLib_Fix16Div(FloatLib_Fix16FromInt(s16_Greenp * g_st_GreenxyY.s32_Y / 1000), g_s32_Fix16Const10000);
		g_st_CurrentGreenCIE.s32_Y = FloatLib_Fix16Mul(g_st_CurrentGreenCIE.s32_Y, g_s32_Fix16Const25);

		// Interpolate the green x coordinate shift
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_GreenxShift - g_ast_TtoCie[s16_Cnt].s16_GreenxShift) << 4) / s16_TempShift);
		s16_Greenp = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_GreenxShift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentGreenCIE.s32_x = FloatLib_Fix16Div(FloatLib_Fix16FromInt((int32_t)(g_st_GreenxyY.s16_x + s16_Greenp)), g_s32_Fix16Const10000);

		// Interpolate the green y coordinate shift
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_GreenyShift - g_ast_TtoCie[s16_Cnt].s16_GreenyShift) << 4) / s16_TempShift);
		s16_Greenp = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_GreenyShift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentGreenCIE.s32_y = FloatLib_Fix16Div(FloatLib_Fix16FromInt((int32_t)(g_st_GreenxyY.s16_y + s16_Greenp)), g_s32_Fix16Const10000);

		/** Fix Blue Color **/
		// Interpolate the blue Y shift value
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_BlueYshift - g_ast_TtoCie[s16_Cnt].s16_BlueYshift) << 4) / s16_TempShift);
		s16_Bluep = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_BlueYshift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentBlueCIE.s32_Y = (int32_t)(FloatLib_Fix16Div(FloatLib_Fix16FromInt(s16_Bluep*g_st_BluexyY.s32_Y/1000), g_s32_Fix16Const10000));
		g_st_CurrentBlueCIE.s32_Y = (int32_t)(FloatLib_Fix16Mul(g_st_CurrentBlueCIE.s32_Y, g_s32_Fix16Const25));

		// Interpolate the blue x coordinate shift
		s16_Linep = (int16_t)(((g_ast_TtoCie[s16_Cnt + 1].s16_BluexShift - g_ast_TtoCie[s16_Cnt].s16_BluexShift) << 4) / s16_TempShift);
		s16_Bluep = (int16_t)(g_ast_TtoCie[s16_Cnt].s16_BluexShift + ((s16_Linep * (s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature)) >> 4));
		g_st_CurrentBlueCIE.s32_x = (int32_t)(FloatLib_Fix16Div(FloatLib_Fix16FromInt((int32_t)(g_st_BluexyY.s16_x+s16_Bluep)), g_s32_Fix16Const10000));

		// Interpolate the blue y coordinate shift
		s16_Linep = ((g_ast_TtoCie[s16_Cnt+1].s16_BlueyShift - g_ast_TtoCie[s16_Cnt].s16_BlueyShift)<<4)/s16_TempShift;
		s16_Bluep = g_ast_TtoCie[s16_Cnt].s16_BlueyShift + ((s16_Linep * ( s16_LedTemp - g_ast_TtoCie[s16_Cnt].s16_Temperature))>>4) ;
		g_st_CurrentBlueCIE.s32_y = FloatLib_Fix16Div(FloatLib_Fix16FromInt((int32_t)(g_st_BluexyY.s16_y + s16_Bluep)), g_s32_Fix16Const10000);
}


/**
 * @brief 		Calculate the duty ratios for RGB PWM based on input color and CIE1931 color space.
 * @note 		This function determines the appropriate PWM duty cycle for red, green, and blue LEDs
 *               to achieve the target color and intensity while applying temperature adjustments and safety limits.
 * @param[in]	pst_InputColor -- Pointer to the input color parameters, including RGB values and temperature adjustments.
 * @param[in]	pst_MxyzY -- Pointer to the target CIE1931 color coordinates (x, y, Y).
 * @param[out]	pst_OutputPWM -- Pointer to the output structure containing calculated PWM duty cycles for R, G, and B.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void Algorithm_AlgoGetDutyRatio(const ST_ColorParams * const pst_InputColor, const ST_CIECOLOR * const pst_MxyzY, ST_PWM_PARAMS *pst_OutputPWM)
{
		int32_t s32_mixY;  // Intermediate variable to calculate mixed Y (luminance)
		int32_t s32_DetA, s32_DetB, s32_DetC, s32_DetD, s32_DetE, s32_DetF;  // Variables for calculating matrix determinants
		int32_t s32_DetRm;  // Variables for adjusting red, green, and blue luminance
		int32_t s32_DetGm;
		int32_t s32_DetBm;
		int32_t s32_DutyR, s32_DutyG, s32_DutyB;  // Variables to store calculated PWM duty ratios for RGB
		int32_t s32_DetRall, s32_DetGall, s32_DetBall;  // Variables to store final determinants for RGB calculation
		int32_t temp;
		int32_t s32_DutyPwmR, s32_DutyPwmG, s32_DutyPwmB;

		// Step 1: Calculate the differences between the input CIE coordinates and the current blue reference CIE coordinates
		s32_DetA = FloatLib_Fix16Sub(pst_MxyzY->s32_x, g_st_CurrentBlueCIE.s32_x);  // Difference in x-coordinates
		s32_DetD = FloatLib_Fix16Sub(pst_MxyzY->s32_y, g_st_CurrentBlueCIE.s32_y);  // Difference in y-coordinates
		s32_DetBm = FloatLib_Fix16Div(g_st_CurrentBlueCIE.s32_Y, g_st_CurrentBlueCIE.s32_y);  // Blue luminance adjustment

		// Step 2: Calculate the differences for green reference CIE coordinates
		s32_DetB = FloatLib_Fix16Sub(g_st_CurrentGreenCIE.s32_y, pst_MxyzY->s32_y);  // Difference in y-coordinates
		s32_DetC = FloatLib_Fix16Sub(g_st_CurrentGreenCIE.s32_x, pst_MxyzY->s32_x);  // Difference in x-coordinates
		s32_DetGm = FloatLib_Fix16Div(g_st_CurrentGreenCIE.s32_Y, g_st_CurrentGreenCIE.s32_y);  // Green luminance adjustment

		// Step 3: Calculate the differences for red reference CIE coordinates
		s32_DetE = FloatLib_Fix16Sub(g_st_CurrentRedCIE.s32_x, pst_MxyzY->s32_x);  // Difference in x-coordinates
		s32_DetF = FloatLib_Fix16Sub(g_st_CurrentRedCIE.s32_y, pst_MxyzY->s32_y);  // Difference in y-coordinates
		s32_DetRm = FloatLib_Fix16Div(g_st_CurrentRedCIE.s32_Y, g_st_CurrentRedCIE.s32_y);  // Red luminance adjustment

		

		// Step 4: Calculate matrix determinants for RGB
		s32_DetRall = FloatLib_Fix16Mul(s32_DetA, s32_DetB);  // Red determinant part 1
		s32_DetBall = FloatLib_Fix16Mul(s32_DetE, s32_DetB);  // Blue determinant part 1
		s32_DetGall = FloatLib_Fix16Mul(s32_DetD, s32_DetE);  // Green determinant part 1

		// Adjust determinants by subtracting cross products
		s32_DetRall = FloatLib_Fix16Sub(s32_DetRall, FloatLib_Fix16Mul(s32_DetC, s32_DetD));  // Red determinant part 2
		s32_DetBall = FloatLib_Fix16Sub(s32_DetBall, FloatLib_Fix16Mul(s32_DetC, s32_DetF));  // Blue determinant part 2
		s32_DetGall = FloatLib_Fix16Sub(s32_DetGall, FloatLib_Fix16Mul(s32_DetF, s32_DetA));  // Green determinant part 2

		

		s32_DutyR = s32_DetBm;
		s32_DutyG = s32_DetRm;
		s32_DutyB = s32_DetRm;

		s32_DutyR = FloatLib_Fix16Mul(s32_DutyR, s32_DetGm);
		s32_DutyG = FloatLib_Fix16Mul(s32_DutyG, s32_DetBm);
		s32_DutyB = FloatLib_Fix16Mul(s32_DutyB, s32_DetGm);
		s32_DutyR = FloatLib_Fix16Mul(s32_DutyR, s32_DetRall);
		s32_DutyG = FloatLib_Fix16Mul(s32_DutyG, s32_DetGall);
		s32_DutyB = FloatLib_Fix16Mul(s32_DutyB, s32_DetBall);

		// Step 6: Apply temperature protection and adjust the mixed Y luminance
		s32_mixY = FloatLib_Fix16Mul(pst_MxyzY->s32_Y, FloatLib_Fix16FromInt(pst_InputColor->u16_TempAdjust / 2));
		s32_mixY = FloatLib_Fix16Mul(s32_mixY, F16(0.02));  // Scale down luminance by multiplying with 0.02
		s32_mixY = FloatLib_Fix16Mul(s32_mixY, F16(0.015625));  // Further scaling by multiplying with 0.015625 (1/64)

		temp = FloatLib_Fix16Add(FloatLib_Fix16Mul(g_st_CurrentRedCIE.s32_Y, s32_DutyR),
						FloatLib_Fix16Mul(g_st_CurrentGreenCIE.s32_Y, s32_DutyG));

		temp = FloatLib_Fix16Add(temp, FloatLib_Fix16Mul(g_st_CurrentBlueCIE.s32_Y, s32_DutyB));
		temp = FloatLib_Fix16Div(s32_mixY, temp);

		// Step 8: Scale the duty ratios with the calculated scaling factor
		s32_DutyR = FloatLib_Fix16Mul(s32_DutyR, temp);
		s32_DutyG = FloatLib_Fix16Mul(s32_DutyG, temp);
		s32_DutyB = FloatLib_Fix16Mul(s32_DutyB, temp);

		/*Algorithm may failed to generate PWM. We fix the abnormal value here*/
		s32_DutyR = FloatLib_Fix16Min(s32_DutyR, g_s32_Fix16one);
		s32_DutyG = FloatLib_Fix16Min(s32_DutyG, g_s32_Fix16one);
		s32_DutyB = FloatLib_Fix16Min(s32_DutyB, g_s32_Fix16one);
		s32_DutyR = FloatLib_Fix16Max(s32_DutyR, 0);
		s32_DutyG = FloatLib_Fix16Max(s32_DutyG, 0);
		s32_DutyB = FloatLib_Fix16Max(s32_DutyB, 0);

		s32_DutyR = FloatLib_Fix16Mul(s32_DutyR, F16(PWM_PERIOD));
		s32_DutyG = FloatLib_Fix16Mul(s32_DutyG, F16(PWM_PERIOD));
		s32_DutyB = FloatLib_Fix16Mul(s32_DutyB, F16(PWM_PERIOD));

		

		// Step 11: Convert the fixed-point duty ratios to integer PWM values
		s32_DutyPwmR = FloatLib_Fix16ToInt(s32_DutyR);
		s32_DutyPwmG = FloatLib_Fix16ToInt(s32_DutyG);
		s32_DutyPwmB = FloatLib_Fix16ToInt(s32_DutyB);

		// Step 12: Adjust the PWM values based on the user-defined intensity factor
		s32_DutyPwmR = ((s32_DutyPwmR * (pst_InputColor->u16_Factor >> 7u)) >> 8u);
		s32_DutyPwmG = ((s32_DutyPwmG * (pst_InputColor->u16_Factor >> 7u)) >> 8u);
		s32_DutyPwmB = ((s32_DutyPwmB * (pst_InputColor->u16_Factor >> 7u)) >> 8u);

		// Step 13: Store the calculated PWM values in the output structure
		pst_OutputPWM->u16_PwmR = (uint16_t)s32_DutyPwmR;
		pst_OutputPWM->u16_PwmG = (uint16_t)s32_DutyPwmG;
		pst_OutputPWM->u16_PwmB = (uint16_t)s32_DutyPwmB;
}


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
void Algorithm_AlgoGetPWMOutput(ST_ColorParams * const pst_InputColor, ST_PWM_PARAMS *pst_OutputPWM)
{
		ST_CIECOLOR st_TargetColor;

		// Step 1: Turn off lights by default to avoid uninitialized output
		pst_OutputPWM->u16_PwmR = 0;
		pst_OutputPWM->u16_PwmG = 0;
		pst_OutputPWM->u16_PwmB = 0;

		// Step 2: Check if intensity is zero or all RGB values are zero
		// If true, the lights remain off and the function returns without further processing
		if ((pst_InputColor->u16_Intensity != 0) &&
						((pst_InputColor->u16_ColorR !=0) ||
						 (pst_InputColor->u16_ColorG !=0) ||
						 (pst_InputColor->u16_ColorB !=0)))
		{
				// Step 3: Convert input color parameters to target CIE color (st_TargetColor)
				Algorithm_AlgoGetTargetColor(pst_InputColor, &st_TargetColor);

				// Step 4: Apply LED temperature compensation to adjust current LED CIE coordinates
				Algorithm_AlgoLedFixCIE(pst_InputColor->s16_LedTemprature / D_TEMP_MULTIPLIED_RATIO);

				// Step 5: Calculate and generate PWM duty cycle based on the target CIE color,
				// input intensity control signals, and the compensated LED attributes
				Algorithm_AlgoGetDutyRatio(pst_InputColor, &st_TargetColor, pst_OutputPWM);
		}
}



