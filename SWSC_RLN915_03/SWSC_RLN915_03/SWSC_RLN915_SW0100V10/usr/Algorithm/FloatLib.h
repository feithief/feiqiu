#ifndef _FLOATLIB_H_
#define _FLOATLIB_H_

#include "SystemType.h"

/** @file
*
* @defgroup FLOATING_LIB_MODULE floating fix math LIB.
* @{
*
* @brief This module maintains float to fix math LIB.
*
* @details This module maintains float to fix math LIB.The integer part maximum value is 32768!
*/

/**@brief We do not deal with overflow situation.*/
#define FIXMATH_NO_OVERFLOW
/**@brief No further attributes.*/
#define FIXMATH_FUNC_ATTRS
/**@brief No rounding will not cause any interference on mixing color.*/
//#define FIXMATH_NO_ROUNDING

/**@brief fix16 basic type.*/
typedef int32_t fix16_t;

static const fix16_t fix16_minimum  = 0x80000000; /**< the minimum value of fix16_t */
static const fix16_t fix16_one = 0x00010000; /**< fix16_t value of 1 */

static inline  fix16_t fix16_min(fix16_t x, fix16_t y)
{
  return (x < y ? x : y);
}

static inline  fix16_t fix16_max(fix16_t x, fix16_t y)
{
  return (x > y ? x : y);
}

/**
*@details   Transfer a int32 to fix16.
*
*@param[in] a  int32 value wants to transfer.
*
*@retval    fix16_t value.
*/
static inline  fix16_t fix16_from_int(int32_t a)     { return a * fix16_one; }

/**
*@details   Transfer a fix16 to int32.
*
*@param[in] a  fix16_t value wants to transfer.
*
*@retval    int32 value.
*/
static inline  int fix16_to_int(fix16_t a)
{
#ifdef FIXMATH_NO_ROUNDING
    return (a >> 16);
#else
        if (a >= 0)
                return (a + (fix16_one >> 1)) / fix16_one;
        return (a - (fix16_one >> 1)) / fix16_one;
#endif
}

/**@brief Macro for defining fix16_t constant values.
   The functions above can't be used from e.g. global variable initializers,
   and their names are quite long also. This macro is useful for constants
   springled alongside code, e.g. F16(1.234).

   Note that the argument is evaluated multiple times, and also otherwise
   you should only use this for constant values. For runtime-conversions,
   use the functions above.
*/
#define F16(x) ((fix16_t)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))

#define F16ToFloat(x) (float)((x) / 65536.0)


/**
*@details   Transfer a float to fix16.
*
*@param[in] a  float value wants to transfer.
*
*@retval    fix16_t value.
*/
//static inline fix16_t fix16_from_float(float a)
//{
//        float temp = a * fix16_one;
//#ifndef FIXMATH_NO_ROUNDING
//        temp += (temp >= 0) ? 0.5f : -0.5f;
//#endif
//        return (fix16_t)temp;
//}

/**
*@details   Add two fix16_t value together.
*
*@param[in] inArg0  add value.
*
*@param[in] inArg1  add value.
*
*@retval    Added fix16_t value.
*/
static inline fix16_t fix16_add(fix16_t inArg0, fix16_t inArg1) { return (inArg0 + inArg1); }

/**
*@details   Sub two fix16_t value together.
*
*@param[in] inArg0  minuend value.
*
*@param[in] inArg1  subtractor value.
*
*@retval    Divided fix16_t value.
*/
static inline fix16_t fix16_sub(fix16_t inArg0, fix16_t inArg1) { return (inArg0 - inArg1); }

/**
*@details   Multiplies the two given fix16_t's and returns the result.
*
*@param[in] inArg0  one of the multiplied value.
*
*@param[in] inArg1  one of the multiplied value.
*
*@retval    Multiplied fix16_t value.
*/
extern fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1) FIXMATH_FUNC_ATTRS;

/**
*@details   Divides the first given fix16_t by the second and returns the result.
*
*@param[in] inArg0  dividend value.
*
*@param[in] inArg1  divisor value.
*
*@retval    Divided fix16_t value.
*/
extern fix16_t fix16_div(fix16_t inArg0, fix16_t inArg1) FIXMATH_FUNC_ATTRS;

/** @} */

#endif
