/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		FloatLib.c
 * Project:		RLNE15S
 * Description:	This source file implements fixed-point arithmetic functions in Q16 format. 
 *               It includes operations for addition, subtraction, multiplication, division, 
 *               and conversions between integer and fixed-point formats.
 * Note(s):	- This library is designed for embedded systems with limited floating-point support.
 *           - Overflow checks and rounding options can be customized using predefined macros.
 * Update Record:	
 * Version     	Date     	 Modified by     Why & What is modified
 * 00.01      2017.10.24       Leo Song           Initial Version
 * 00.02      2024.12.12       Genhua Yang        Code standardization and optimization
 ************************************************************************
 */

/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
#include "FloatLib.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< The minimum value of int32_t in fix16 format */
static const int32_t g_fix16_minimum = 0x80000000u;

/*!< int32_t representation of the value 1 in fix16 format */
const int32_t g_s32_Fix16one = 0x00010000;

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/


/**
 * @brief 		Count the number of leading zeros in a 32-bit unsigned integer.
 * @note 		This function iteratively checks the most significant bits to determine how many leading zeros are present.
 *               It shifts the input left until a non-zero bit is found, counting the number of shifts.
 *               This is typically used for normalization in fixed-point arithmetic operations.
 * @param[in]	u32_x -- 32-bit unsigned integer to be processed.
 * @return		Number of leading zeros as an 8-bit unsigned integer.
 * @retval		Returns a value between 0 and 32, representing the number of leading zeros. Returns 32 if the input is 0.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static uint8_t FloatLib_clz(uint32_t u32_x)
{
		uint8_t u8_Result = 0;  // Initialize result to count leading zeros

		// Check if the input is zero; if true, the number of leading zeros is 32
		if (u32_x == 0)
		{
				u8_Result = 32;
		} 
		else
		{
				// First loop: Shift left in chunks of 4 bits until a non-zero nibble is found
				while (0u == (u32_x & 0xF0000000u))  // Check if the top 4 bits are all zeros
				{
						u8_Result += 4;  // Add 4 to the count of leading zeros
						u32_x <<= 4;     // Shift the input left by 4 bits
				}

				// Second loop: Shift one bit at a time until the most significant bit becomes 1
				while (0u == (u32_x & 0x80000000u))  // Check if the most significant bit is zero
				{
						u8_Result += 1;  // Add 1 to the count of leading zeros
						u32_x <<= 1;     // Shift the input left by 1 bit
				}
		}

		// Return the total number of leading zeros
		return u8_Result;
}


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
int32_t FloatLib_Fix16Div(int32_t s16_Input0, int32_t s16_Input1)
{
		int32_t s32_Val = 0;
		uint32_t u32_Remainder = (s16_Input0 >= 0) ? (uint32_t)s16_Input0 : (uint32_t)(-s16_Input0);  // Absolute value of numerator
		uint32_t u32_Divider   = (s16_Input1 >= 0) ? (uint32_t)s16_Input1 : (uint32_t)(-s16_Input1);  // Absolute value of denominator
		uint32_t u32_Quotient  = 0;
		int16_t  i16_BitPos = 17;  // Bit position for long division

		// Check for division by zero
		if (s16_Input1 == 0)
		{
				s32_Val = g_fix16_minimum;  // Return minimum value for division by zero
		}
		else
		{
				// Kick-start the division for large divisors
				if ((u32_Divider & 0xFFF00000u) > 0)
				{
						uint32_t shifted_div = ((u32_Divider >> 17u) + 1);  // Pre-shift the divisor
						u32_Quotient = u32_Remainder / shifted_div;
						u32_Remainder -= ((uint64_t)u32_Quotient * u32_Divider) >> 17u;
				}

				// Optimize for power-of-2 divisors by shifting out lower bits
				while (!(u32_Divider & 0xF) && i16_BitPos >= 4)
				{
						u32_Divider >>= 4u;
						i16_BitPos -= 4;
				}

				// Perform long division until remainder is zero or bit position is exhausted
				while (u32_Remainder && i16_BitPos >= 0)
				{
						int16_t i16_shift = FloatLib_clz(u32_Remainder);  // Count leading zeros
						if (i16_shift > i16_BitPos) 
						{
								i16_shift = i16_BitPos;  // Limit shift to available bit positions
						}
						u32_Remainder <<= i16_shift;
						i16_BitPos -= i16_shift;

						uint32_t u32_Div = u32_Remainder / u32_Divider;
						u32_Remainder = u32_Remainder % u32_Divider;
						u32_Quotient += u32_Div << i16_BitPos;

						// Overflow check
#ifndef FIXMATH_NO_OVERFLOW
						if ((u32_Div & ~(0xFFFFFFFFu >> i16_BitPos)) != 0u)
						{
								return g_s32_Fix16one;  // Return max value on overflow
						}
#endif

						u32_Remainder <<= 1;
						i16_BitPos--;
				}

				// Round the result
#ifndef FIXMATH_NO_ROUNDING
				u32_Quotient++;
#endif

				int32_t s32_Result = u32_Quotient >> 1u;  // Final division by 2 to adjust fixed-point scaling

				// Handle sign of the result
				if (((s16_Input0 ^ s16_Input1) & 0x80000000u) != 0)
				{
						s32_Result = -s32_Result;  // Apply negative sign if inputs have different signs
				}
				s32_Val = s32_Result;
		}

		// Return the final computed value
		return s32_Val;
}


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
int32_t FloatLib_Fix16Mul(int32_t s16_Input0, int32_t s16_Input1)
{
		// Extract high and low 16-bit parts of the inputs
		const int32_t s32_A = (s16_Input0 >> 16u), s32_C = (s16_Input1 >> 16u);
		const uint16_t u16_B = (s16_Input0 & 0xFFFFu), u16_D = (uint16_t)(s16_Input1 & 0xFFFF);

		// Compute individual partial products
		int32_t s32_AC = s32_A * s32_C;                          // High * High
		int32_t s32_AD_CB = s32_A * (int32_t)u16_D + s32_C * (int32_t)u16_B;  // High * Low + Low * High
		uint32_t u32_BD = (uint32_t)u16_B * u16_D;               // Low * Low

		int32_t s32_ProductHi = s32_AC + (s32_AD_CB >> 16u);     // Combine high product parts

		// Handle carry from lower 32 bits to upper part of the result
		const uint32_t u32_Ad_Cb_Temp = s32_AD_CB << 16;
		uint32_t u32_Product_lo = u32_BD + u32_Ad_Cb_Temp;
		uint32_t u32_Product_lo_tmp = u32_Product_lo;
		int32_t s32_Result = 0;

		if (u32_Product_lo < u32_BD) 
		{
				s32_ProductHi++;  // Handle overflow in the lower product
		}

#ifndef FIXMATH_NO_OVERFLOW
		// Overflow detection: Check if the upper 17 bits are the same (i.e., no overflow occurred)
		if (s32_ProductHi >> 31u != s32_ProductHi >> 15u)
		{
				s32_Result = g_s32_Fix16one;
				break;
		}
#endif

#ifdef FIXMATH_NO_ROUNDING
		s32_Result = (s32_ProductHi << 16) | (u32_Product_lo >> 16);
		return  s32_Result; // Return result without rounding
#else
		// Round the result by subtracting 0.5 (0x8000) and adjusting for negative numbers
		u32_Product_lo_tmp = u32_Product_lo;
		u32_Product_lo -= 0x8000u;                // Subtract 0.5 for rounding
		u32_Product_lo -= (s32_ProductHi > 0 ? 0u : 1u);  // Further adjustment for negative numbers

		if (u32_Product_lo > u32_Product_lo_tmp)
		{
				s32_ProductHi--;  // Handle overflow correction in rounding
		}

		// Discard the lowest 16 bits for the final result
		s32_Result = (s32_ProductHi << 16) | (int32_t)(u32_Product_lo >> 16u);
		s32_Result += 1;  // Adjust the result for proper rounding

		return s32_Result;
#endif
}

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
int32_t FloatLib_Fix16Min(int32_t s32_x, int32_t s32_y)
{
		int32_t s32_z = 0;

		// Compare s32_x and s32_y, assign the smaller value to s32_z
		s32_z = (s32_x < s32_y ? s32_x : s32_y);

		// Return the smaller value
		return s32_z;
}



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
int32_t FloatLib_Fix16Max(int32_t s32_x, int32_t s32_y)
{
		int32_t s32_z = 0;

		// Compare s32_x and s32_y, assign the larger value to s32_z
		s32_z = (s32_x > s32_y ? s32_x : s32_y);

		// Return the larger value
		return s32_z;
}


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
int32_t FloatLib_Fix16FromInt(int32_t s32_a)
{
		int32_t s32_z = 0;

		// Convert integer to fixed-point format by multiplying with the fixed-point representation of 1
		s32_z = s32_a * g_s32_Fix16one;

		// Return the converted fixed-point value
		return s32_z;
}



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
int32_t FloatLib_Fix16ToInt(int32_t f16_a)
{
		int32_t s32_Ret = 0;

		// Check if the input fixed-point value is non-negative
		if (f16_a >= 0)
		{
				// Round to the nearest integer for positive values
				s32_Ret = (f16_a + (g_s32_Fix16one >> 1)) / g_s32_Fix16one;
		}
		else
		{
				// Round to the nearest integer for negative values
				s32_Ret = (f16_a - (g_s32_Fix16one >> 1)) / g_s32_Fix16one;
		}

		// Return the integer equivalent of the fixed-point value
		return s32_Ret;
}






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
int32_t FloatLib_Fix16Add(int32_t s16_Input0, int32_t s16_Input1)
{
		int32_t s32_z = 0;

		// Perform addition of two fixed-point values
		s32_z = s16_Input0 + s16_Input1;

		// Return the result
		return s32_z;
}



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
int32_t FloatLib_Fix16Sub(int32_t s16_Input0, int32_t s16_Input1)
{
		int32_t s32_z = 0;

		// Perform subtraction of two fixed-point values
		s32_z = s16_Input0 - s16_Input1;

		// Return the result
		return s32_z;
}

