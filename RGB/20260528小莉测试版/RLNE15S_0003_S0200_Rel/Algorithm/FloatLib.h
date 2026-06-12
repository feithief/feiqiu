/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		FloatLib.h
 * Project:		RLNE15S
 * Description:	This header file provides fixed-point arithmetic functions in Q16 format. 
 *               It supports addition, subtraction, multiplication, division, and 
 *               conversions between integer and fixed-point formats.
 * Note(s):	- This library is optimized for systems with limited floating-point support.
 *           - Overflow checks can be disabled with FIXMATH_NO_OVERFLOW to improve performance.
 * Update Record:	
 * Version     	Date     	 Modified by     Why & What is modified
 * 00.01      2017.10.24       Leo Song           Initial Version
 * 00.02      2024.12.12       Genhua Yang        Code standardization and optimization
 ************************************************************************
 */

#ifndef FLOATLIB_H
#define FLOATLIB_H
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
#include "../System/SystemType.h"
#include "cpu.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

extern const int32_t g_s32_Fix16one;


/**@brief We do not deal with overflow situation.*/
#define FIXMATH_NO_OVERFLOW
/**@brief No further attributes.*/
#define FIXMATH_FUNC_ATTRS
/**@brief No rounding will not cause any interference on mixing color.*/


/**@brief fix16 basic type.*/



/**
 * @brief 		Perform fixed-point multiplication (Q16 format) of two 32-bit signed integers.
 * @note 		This function multiplies two fixed-point numbers by decomposing them into high and low parts 
 *               to ensure precision and prevent overflow. Rounding is applied unless `FIXMATH_NO_ROUNDING` is defined.
 *               It handles corner cases for negative numbers and overflow conditions.
 * @param[in]	s16_Input0 -- First multiplicand (32-bit signed integer).
 * @param[in]	s16_Input1 -- Second multiplicand (32-bit signed integer).
 * @return		32-bit signed integer result in fixed-point Q16 format.
 * @retval		Returns the multiplication result in Q16 format. If overflow occurs, returns `g_s32_Fix16one`.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16Mul(int32_t s16_Input0, int32_t s16_Input1) FIXMATH_FUNC_ATTRS;

/**
 * @brief 		Perform fixed-point division (Q16 format) of two 32-bit signed integers.
 * @note 		This function divides two fixed-point numbers using long division, ensuring precision and minimizing overflow risk. 
 *               It supports proper rounding and handles special cases such as division by zero and power-of-2 divisors.
 *               The result retains the Q16 format.
 * @param[in]	s16_Input0 -- Dividend (32-bit signed integer).
 * @param[in]	s16_Input1 -- Divisor (32-bit signed integer).
 * @return		32-bit signed integer result in fixed-point Q16 format.
 * @retval		Returns the division result in Q16 format. If division by zero occurs, returns `g_fix16_minimum`.
 *               If overflow occurs, returns `g_s32_Fix16one`.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16Div(int32_t s16_Input0, int32_t s16_Input1) FIXMATH_FUNC_ATTRS;

/**
 * @brief 		Get the minimum value between two fixed-point Q16 format numbers.
 * @note 		This function compares two fixed-point numbers and returns the smaller of the two.
 *               It operates in Q16 format and ensures that the result is also in Q16 format.
 * @param[in]	s32_x -- First 32-bit signed integer in Q16 format.
 * @param[in]	s32_y -- Second 32-bit signed integer in Q16 format.
 * @return		32-bit signed integer representing the minimum value in Q16 format.
 * @retval		Returns the smaller of `s32_x` and `s32_y`.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16Min(int32_t s32_x, int32_t s32_y);

/**
 * @brief 		Get the maximum value between two fixed-point Q16 format numbers.
 * @note 		This function compares two fixed-point numbers and returns the larger of the two.
 *               It operates in Q16 format and ensures that the result is also in Q16 format.
 * @param[in]	s32_x -- First 32-bit signed integer in Q16 format.
 * @param[in]	s32_y -- Second 32-bit signed integer in Q16 format.
 * @return		32-bit signed integer representing the maximum value in Q16 format.
 * @retval		Returns the larger of `s32_x` and `s32_y`.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16Max(int32_t s32_x, int32_t s32_y);

/**
 * @brief 		Convert a 32-bit integer to fixed-point Q16 format.
 * @note 		This function multiplies the input integer by the fixed-point scaling factor (`g_s32_Fix16one`) to convert it into Q16 format.
 * @param[in]	s32_a -- 32-bit signed integer to be converted.
 * @return		32-bit signed integer in fixed-point Q16 format.
 * @retval		Returns the converted fixed-point number.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16FromInt(int32_t s32_a);

/**
 * @brief 		Convert a fixed-point Q16 format number to a 32-bit integer with rounding.
 * @note 		This function converts a fixed-point number to an integer by applying rounding. 
 *               If the input is positive, it rounds up, and if it is negative, it rounds down.
 * @param[in]	f16_a -- Fixed-point Q16 format number (32-bit signed integer).
 * @return		32-bit signed integer result.
 * @retval		Returns the rounded integer value of the fixed-point input.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16ToInt(int32_t f16_a);

/**
 * @brief 		Perform fixed-point addition (Q16 format) of two 32-bit signed integers.
 * @note 		This function calculates the sum of two fixed-point numbers while maintaining Q16 format.
 *               It returns the result without additional overflow or underflow checks.
 * @param[in]	s16_Input0 -- First addend (32-bit signed integer in Q16 format).
 * @param[in]	s16_Input1 -- Second addend (32-bit signed integer in Q16 format).
 * @return		32-bit signed integer result in fixed-point Q16 format.
 * @retval		Returns the addition result in Q16 format.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16Add(int32_t s16_Input0, int32_t s16_Input1);

/**
 * @brief 		Perform fixed-point subtraction (Q16 format) of two 32-bit signed integers.
 * @note 		This function calculates the difference between two fixed-point numbers while maintaining Q16 format.
 *               It returns the result without any additional overflow or underflow checks.
 * @param[in]	s16_Input0 -- Minuend (32-bit signed integer in Q16 format).
 * @param[in]	s16_Input1 -- Subtrahend (32-bit signed integer in Q16 format).
 * @return		32-bit signed integer result in fixed-point Q16 format.
 * @retval		Returns the subtraction result in Q16 format.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
int32_t FloatLib_Fix16Sub(int32_t s16_Input0, int32_t s16_Input1);






#endif

