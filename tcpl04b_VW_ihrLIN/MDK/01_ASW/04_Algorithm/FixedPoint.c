#include "FloatLib.h"

/* 32-bit implementation of fix16_div. Fastest version for e.g. ARM Cortex M3.
 * Performs 32-bit divisions repeatedly to reduce the remainder. For this to
 * be efficient, the processor has to have 32-bit hardware division.
 */
static uint8_t clz(uint32_t x)
{
  uint8_t result = 0;
  if (x == 0) return 32;
  while (!(x & 0xF0000000)) { result += 4; x <<= 4; }
  while (!(x & 0x80000000)) { result += 1; x <<= 1; }
  return result;
}

fix16_t fix16_div(fix16_t a, fix16_t b)
{
  // This uses a hardware 32/32 bit division multiple times, until we have
  // computed all the bits in (a<<17)/b. Usually this takes 1-3 iterations.
  if (b == 0)
    return fix16_minimum;

  uint32_t remainder = (a >= 0) ? a : (-a);
  uint32_t divider = (b >= 0) ? b : (-b);
  uint32_t quotient = 0;
  int bit_pos = 17;

  // Kick-start the division a bit.
  // This improves speed in the worst-case scenarios where N and D are large
  // It gets a lower estimate for the result by N/(D >> 17 + 1).
  if (divider & 0xFFF00000)
  {
    uint32_t shifted_div = ((divider >> 17) + 1);
    quotient = remainder / shifted_div;
    remainder -= ((uint64_t)quotient * divider) >> 17;
  }

  // If the divider is divisible by 2^n, take advantage of it.
  while (!(divider & 0xF) && bit_pos >= 4)
  {
    divider >>= 4;
    bit_pos -= 4;
  }

  while (remainder && bit_pos >= 0)
  {
    // Shift remainder as much as we can without overflowing
    int shift = clz(remainder);
    if (shift > bit_pos) shift = bit_pos;
    remainder <<= shift;
    bit_pos -= shift;

    uint32_t div = remainder / divider;
    remainder = remainder % divider;
    quotient += div << bit_pos;

    #ifndef FIXMATH_NO_OVERFLOW
    if (div & ~(0xFFFFFFFF >> bit_pos))
                    return fix16_overflow;
    #endif

    remainder <<= 1;
    bit_pos--;
  }

  #ifndef FIXMATH_NO_ROUNDING
  // Quotient is always positive so rounding is easy
  quotient++;
  #endif

  fix16_t result = quotient >> 1;

  // Figure out the sign of the result
  if ((a ^ b) & 0x80000000)
  {
    #ifndef FIXMATH_NO_OVERFLOW
    if (result == fix16_minimum)
                    return fix16_overflow;
    #endif

    result = -result;
  }

  return result;
}

fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1)
{
  // Each argument is divided to 16-bit parts.
  //                                      AB
  //                      *        CD
  // -----------
  //                                      BD      16 * 16 -> 32 bit products
  //                               CB
  //                               AD
  //                              AC
  //                       |----| 64 bit product
  int32_t A = (inArg0 >> 16), C = (inArg1 >> 16);
  uint32_t B = (inArg0 & 0xFFFF), D = (inArg1 & 0xFFFF);

  int32_t AC = A*C;
  int32_t AD_CB = A*D + C*B;
  uint32_t BD = B*D;

  int32_t product_hi = AC + (AD_CB >> 16);

  // Handle carry from lower 32 bits to upper part of result.
  uint32_t ad_cb_temp = AD_CB << 16;
  uint32_t product_lo = BD + ad_cb_temp;
  if (product_lo < BD)
    product_hi++;

#ifndef FIXMATH_NO_OVERFLOW
  // The upper 17 bits should all be the same (the sign).
  if (product_hi >> 31 != product_hi >> 15)
    return fix16_overflow;
#endif

#ifdef FIXMATH_NO_ROUNDING
  return (product_hi << 16) | (product_lo >> 16);
#else
  // Subtracting 0x8000 (= 0.5) and then using signed right shift
  // achieves proper rounding to result-1, except in the corner
  // case of negative numbers and lowest word = 0x8000.
  // To handle that, we also have to subtract 1 for negative numbers.
  uint32_t product_lo_tmp = product_lo;
  product_lo -= 0x8000;
  product_lo -= (uint32_t)product_hi >> 31;
  if (product_lo > product_lo_tmp)
    product_hi--;

  // Discard the lowest 16 bits. Note that this is not exactly the same
  // as dividing by 0x10000. For example if product = -1, result will
  // also be -1 and not 0. This is compensated by adding +1 to the result
  // and compensating this in turn in the rounding above.
  fix16_t result = (product_hi << 16) | (product_lo >> 16);
  result += 1;
  return result;
#endif
}
