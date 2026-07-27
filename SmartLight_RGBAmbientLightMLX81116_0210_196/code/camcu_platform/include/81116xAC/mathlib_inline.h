/**
 * @file
 * @brief Mathlib header file for inlined function
 * @internal
 *
 * @copyright (C) Melexis N.V.
 * git flash 137e5489
 *
 * Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @endinternal
 *
 * @ingroup mathlib
 *
 * @details
 */

/*
 *  Declaration of functions for inline_asm realization
 */
#ifndef MATHLIB_INLINE_H_
#define MATHLIB_INLINE_H_


/* --------------------
 *    Multiplication
 * --------------------*/

/** Signed integer multiplication ( 32 = 16 * 16 )
 *
 *  @param[in]   multiplicand           signed 16-bit multiplicand
 *  @param[in]   multiplier             signed 16-bit multiplier
 *
 *  @return      Signed 32-bit multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int32_t result = ((int32_t) multiplicand) * multiplier
 *      @endcode
 */
static __inline__ int32_t mulI32_I16byI16(int16_t multiplicand, int16_t multiplier)
{
    int32_t result;

    __asm__ __volatile__ (
        "muls %Q0, A, %w2"
        : "=qb" (result)
        : "%a" (multiplicand), "r" (multiplier)
        );

    return result;
}

/** Unsigned integer multiplication ( 32 = 16 * 16 )
 *
 *  @param[in]   multiplicand           unsigned 16-bit multiplicand
 *  @param[in]   multiplier             unsigned 16-bit multiplier
 *
 *  @return      Unsigned 32-bit multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      uint32_t result = ((uint32_t) multiplicand) * multiplier
 *      @endcode
 */
static __inline__ uint32_t mulU32_U16byU16(uint16_t multiplicand, uint16_t multiplier)
{
    uint32_t result;

    __asm__ __volatile__ (
        "mulu %Q0, A, %w2"
        : "=qb" (result)
        : "%a" (multiplicand), "r" (multiplier)
        );

    return result;
}

/** Signed integer multiplication ( 16 = high (16 * 16) )
 *
 *  @param[in]   multiplicand           signed 16-bit multiplicand
 *  @param[in]   multiplier             signed 16-bit multiplier
 *
 *  @return      Signed 16-bit multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int16_t result = (int16_t)( ((int32_t) multiplicand * multiplier) >> 16 );
 *      @endcode
 */
static __inline__ int16_t mulI16hi_I16byI16(int16_t multiplicand, int16_t multiplier)
{
    int16_t result;
    int16_t result2;    /* clobbering of the register */

    __asm__ __volatile__ (
        "muls YA, A, %w3"
        : "=y" (result), "=a" (result2)
        : "%a" (multiplicand), "r" (multiplier)
        );

    return result;
}

/** Unsigned integer multiplication ( 16 = high (16 * 16) )
 *
 *  @param[in]   multiplicand           unsigned 16-bit multiplicand
 *  @param[in]   multiplier             unsigned 16-bit multiplier
 *
 *  @return      Unsigned 16-bit multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      uint16_t result = (uint16_t)( ((uint32_t) multiplicand * multiplier) >> 16 );
 *      @endcode
 */
static __inline__ uint16_t mulU16hi_U16byU16(uint16_t multiplicand, uint16_t multiplier)
{
    uint16_t result;
    uint16_t result2;    /* clobbering of the register */

    __asm__ __volatile__ (
        "mulu YA, A, %w3"
        : "=y" (result), "=a" (result2)
        : "%a" (multiplicand), "r" (multiplier)
        );

    return result;
}

/** Signed integer fraction multiplication ( 15 = high (15 * 15) )
 *
 *  @param[in]   multiplicand           signed Q15 multiplicand
 *  @param[in]   multiplier             signed Q15 multiplier
 *
 *  @return      Signed Q15 multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int16_t result = (int16_t)( ((int32_t) multiplicand * multiplier) >> 15 );
 *      @endcode
 */
static __inline__ int16_t mulQ15_Q15byQ15(int16_t multiplicand, int16_t multiplier)
{
    int16_t result;
    int16_t result2;    /* clobbering of the register */

    __asm__ __volatile__ (
        "muls YA, A, %w3\n\t"
        "asr YA, #15"
        : "=a" (result), "=y" (result2)
        : "%a" (multiplicand), "r" (multiplier)
        );

    return result;
}

/** Signed integer multiplication ( 24 = 16 * 8 )
 *
 *  @param[in]   multiplicand           signed 16-bit multiplicand
 *  @param[in]   multiplier             signed 8-bit multiplier
 *
 *  @return      Signed 24-bit result (most significant byte 0)
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int32_t result = ((int32_t) multiplicand * (int16_t) multiplier);
 *      @endcode
 */
static __inline__ int32_t mulI24_I16byI8(int16_t multiplicand, int8_t multiplier)
{
    int32_t result;
    int16_t b_16 = (int16_t) multiplier; /* cast to Word */

    __asm__ __volatile__ (
        "muls %Q0, A, %w2"
        : "=qb" (result)
        : "%a" (multiplicand), "r" (b_16)
        );

    return result;
}

/** Unsigned integer multiplication ( 24 = 16 * 8 )
 *
 *  @param[in]   multiplicand           unsigned 16-bit multiplicand
 *  @param[in]   multiplier             unsigned 8-bit multiplier
 *
 *  @return      Unsigned 24-bit result (most significant byte 0)
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      uint32_t result = ((uint32_t) multiplicand * (uint16_t) multiplier);
 *      @endcode
 */
static __inline__ uint32_t mulU24_U16byU8(uint16_t multiplicand, uint8_t multiplier)
{
    uint32_t result;
    uint16_t b_16 = (uint16_t) multiplier; /* cast to Word */

    __asm__ __volatile__ (
        "mulu %Q0, A, %w2"
        : "=qb" (result)
        : "%a" (multiplicand), "r" (b_16)
        );

    return result;
}

/** Signed integer multiplication ( 24 = 16 * 8 )
 *
 *  @param[in]   multiplicand           signed 16-bit multiplicand
 *  @param[in]   multiplier             unsigned 8-bit multiplier
 *
 *  @return      Signed 24-bit result (most significant byte 0)
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int32_t result = ((int32_t) multiplicand * (int16_t) multiplier);
 *      @endcode
 */
static __inline__ int32_t mulI24_I16byU8(int16_t multiplicand, uint8_t multiplier)
{
    int32_t result;
    int16_t b_16 = (int16_t) multiplier; /* cast to Word */

    __asm__ __volatile__ (
        "muls %Q0, A, %w2"
        : "=qb" (result)
        : "%a" (multiplicand), "r" (b_16)
        );

    return result;
}

/** Signed integer multiplication ( 16 = high (16 * 8) )
 *
 *  @param[in]   multiplicand           signed 16-bit multiplicand
 *  @param[in]   multiplier             signed 8-bit multiplier
 *
 *  @return      Signed 16-bit result (high part of the 24-bit)
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int32_t result = (int16_t)( ((int32_t) multiplicand * (int16_t) multiplier) >> 8);
 *      @endcode
 */
static __inline__ int16_t mulI16hi_I16byI8(int16_t multiplicand, int8_t multiplier)
{
    int16_t result;
    int16_t result2;            /* clobbering of the register */
    int16_t b_16 = (int16_t) multiplier; /* cast to Word */

    __asm__ __volatile__ (
        "muls YA, A, %w3\n\t"
        "asr YA, #8"
        : "=a" (result), "=y" (result2)
        : "%a" (multiplicand), "r" (b_16)
        );

    return result;
}

/** Unsigned integer multiplication ( 16 = high (16 * 8) )
 *
 *  @param[in]   multiplicand           unsigned 16-bit multiplicand
 *  @param[in]   multiplier             unsigned 8-bit multiplier
 *
 *  @return      Unsigned 16-bit result (high part of the 24-bit)
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      uint32_t result = (uint16_t)( ((uint32_t) multiplicand * (uint16_t) multiplier) >> 8);
 *      @endcode
 */
static __inline__ uint16_t mulU16hi_U16byU8(uint16_t multiplicand, uint8_t multiplier)
{
    uint16_t result;
    uint16_t result2;                /* clobbering of the register */
    uint16_t b_16 = (uint16_t) multiplier;     /* cast to Word */

    __asm__ __volatile__ (
        "mulu YA, A, %w3\n\t"
        "lsr YA, #8"
        : "=a" (result), "=y" (result2)
        : "%a" (multiplicand), "r" (b_16)
        );

    return result;
}

/** Signed integer multiplication ( 16 = high (16 * 8) )
 *
 *  @param[in]   multiplicand           signed 16-bit multiplicand
 *  @param[in]   multiplier             unsigned 8-bit multiplier
 *
 *  @return      Signed 16-bit result (high part of the 24-bit)
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int32_t result = (int16_t)( ((int32_t) multiplicand * (int16_t) multiplier) >> 8);
 *      @endcode
 */
static __inline__ int16_t mulI16hi_I16byU8(int16_t multiplicand, uint8_t multiplier)
{
    int16_t result;
    int16_t result2;            /* clobbering of the register */
    int16_t b_16 = (int16_t) multiplier; /* cast to Word */

    __asm__ __volatile__ (
        "muls YA, A, %w3\n\t"
        "asr YA, #8"
        : "=a" (result), "=y" (result2)
        : "%a" (multiplicand), "r" (b_16)
        );

    return result;
}

/** Signed integer multiplication ( 16 = 8 * 8 )
 *
 *  @param[in]   multiplicand           signed 8-bit multiplicand
 *  @param[in]   multiplier             signed 8-bit multiplier
 *
 *  @return      Signed 16-bit multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int16_t result = ((int16_t) multiplicand) * multiplier;
 *      @endcode
 */
static __inline__ int16_t mulI16_I8byI8(int8_t multiplicand, int8_t multiplier)
{
    int16_t result;

    __asm__ __volatile__ (
        "muls %w0, AL, %b2"
        : "=ay" (result)
        : "%a" (multiplicand), "r" (multiplier)
        );

    return result;
}

/** Signed integer multiplication ( 16 = 8 * 8 )
 *
 *  @param[in]   multiplicand           signed 8-bit multiplicand
 *  @param[in]   multiplier             unsigned 8-bit multiplier
 *
 *  @return      Signed 16-bit multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      int16_t result = ((int16_t) multiplicand) * multiplier;
 *      @endcode
 */
static __inline__ int16_t mulI16_I8byU8(int8_t multiplicand, uint8_t multiplier)
{
    int16_t result;
    int16_t a_16 = (int16_t) multiplicand; /* convert arguments to int16_t to mul. mixed types */
    int16_t b_16 = (int16_t) multiplier;

    __asm__ __volatile__ (
        "muls %w0, A, %w2"
        : "=ay" (result)
        : "%a" (a_16), "r" (b_16)
        );

    return result;
}


/** Unsigned integer multiplication ( 16 = 8 * 8 )
 *
 *  @param[in]   multiplicand           unsigned 8-bit multiplicand
 *  @param[in]   multiplier             unsigned 8-bit multiplier
 *
 *  @return      Unsigned 16-bit multiplication result
 *
 *  @note
 *  1.  Same functionality could be realized more optimally
 *      in some cases by using C operators
 *
 *      Example:
 *      @code
 *      uint16_t    result = ((uint16_t) multiplicand) * multiplier;
 *      @endcode
 */
static __inline__ uint16_t mulU16_U8byU8(uint8_t multiplicand, uint8_t multiplier)
{
    uint16_t result;

    __asm__ __volatile__ (
        "mulu %w0, AL, %b2"
        : "=ay" (result)
        : "%a" (multiplicand), "r" (multiplier)
        );

    return result;
}

/* --------------------
 *    Division
 * --------------------*/

/** Unsigned integer division ( 16 = 32 / 16 )
 *
 *  @param[in]   dividend           unsigned 32-bit dividend
 *  @param[in]   divisor            unsigned 16-bit divisor
 *
 *  @return      Unsigned 16-bit quotient
 */
static __inline__ uint16_t divU16_U32byU16(uint32_t dividend, uint16_t divisor)
{
    uint16_t result;
    uint16_t result2;    /* clobbering of the register */

    __asm__ __volatile__ (
        "divu YA, X\n\t"
        "divu YA, X"
        : "=a" (result), "=y" (result2)
        : "b" (dividend), "x" (divisor)
        );

    return result;
}

#endif /* MATHLIB_INLINE_H_ */
