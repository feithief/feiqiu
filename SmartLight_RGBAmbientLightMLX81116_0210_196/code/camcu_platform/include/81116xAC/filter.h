/**
 * @file
 * @brief Header file for functions related to filtering
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
 *
 * The types of functions available:
 *
 * - Finite impulse response (FIR) filter calculations
 * - (First order) infinite impulse response (IIR) filter calculations
 * - Low pass filter (LPF) calculations
 * - Hysteresis filter calculations
 * - Biquad121 filter calculation
 */

#ifndef MATHLIB_FILTER_H_
#define MATHLIB_FILTER_H_

extern void fir_init(uint16_t * const p, uint16_t init_value, uint16_t n);
extern uint16_t fir11   (uint16_t in, uint16_t *p_in);
extern uint16_t fir121  (uint16_t in, uint16_t *p_in);
extern uint16_t fir1111 (uint16_t in, uint16_t *p_in);
extern uint16_t fir1331 (uint16_t in, uint16_t *p_in);
extern uint16_t fir12221(uint16_t in, uint16_t *p_in);
static __inline__ void  firI16_init (int16_t *p_in, int16_t init_value, uint16_t n) __attribute__ ((always_inline));
static __inline__ int16_t fir11_I16   (int16_t in, int16_t *p_in) __attribute__ ((always_inline));
static __inline__ int16_t fir121_I16  (int16_t in, int16_t *p_in) __attribute__ ((always_inline));
static __inline__ int16_t fir1111_I16 (int16_t in, int16_t *p_in) __attribute__ ((always_inline));
static __inline__ int16_t fir1331_I16 (int16_t in, int16_t *p_in) __attribute__ ((always_inline));
static __inline__ int16_t fir12221_I16(int16_t in, int16_t *p_in) __attribute__ ((always_inline));

extern void iir_init    (uint16_t *p_out, uint16_t init_value, uint16_t n);
extern void iirU32_init (uint32_t * const p, uint16_t init_value, uint16_t n);
/* a negative */
extern uint16_t iir1      (uint16_t in, uint16_t *p_out, uint16_t a);
extern uint16_t iir1_U32  (uint16_t in, uint32_t *p_out, uint16_t a);
/* a1 negative, a2 positive */
extern uint16_t iir2      (uint16_t in, uint16_t *p_out, uint16_t a1, uint16_t a2);
static __inline__ void  iirI16_init(int16_t *p_out, int16_t init_value, uint16_t n) __attribute__ ((always_inline));
static __inline__ void  iirI32_init(int32_t *p_out, int16_t init_value, uint16_t n) __attribute__ ((always_inline));
/* a negative */
static __inline__ int16_t iir1_I16   (int16_t in, int16_t *p_out, uint16_t a) __attribute__ ((always_inline));
static __inline__ int16_t iir1_I32   (int16_t in, int32_t *p_out, uint16_t a) __attribute__ ((always_inline));

extern uint16_t biquad121           (uint16_t in, uint16_t *p_out,      uint16_t a1, uint16_t a2);
extern uint16_t biquad121_f         (uint16_t in, uint16_t *p_state,    uint16_t a1, uint16_t a2);
extern int16_t biquad121a           (int16_t in,   int16_t *p_out,       int16_t a1,  int16_t a2);
extern uint16_t biquad121_halfrange (uint16_t in, uint16_t *p_out,      uint16_t a1, uint16_t a2);

/* can add till 15 */
extern void   lpf_init (uint16_t *p_out, uint16_t init_value, uint16_t n);
extern void   lpfI16_init ( int16_t *p_out,  int16_t init_value, uint16_t n);
extern void   lpfU32_init (uint32_t *p_out, uint16_t init_value, uint16_t n);
extern void   lpfI32_init ( int32_t *p_out,  int16_t init_value, uint16_t n);

extern uint16_t lpf1     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf1_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf1_I16 ( int16_t in,  int16_t *p_out);

extern uint16_t lpf2     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf2_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf2_I16 ( int16_t in,  int16_t *p_out);

extern uint16_t lpf3     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf3_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf3_I16 ( int16_t in,  int16_t *p_out);

extern uint16_t lpf4     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf4_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf4_I16 ( int16_t in,  int16_t *p_out);

extern uint16_t lpf5     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf5_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf5_I16 ( int16_t in,  int16_t *p_out);

extern uint16_t lpf6     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf6_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf6_I16 ( int16_t in,  int16_t *p_out);

extern uint16_t lpf7     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf7_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf7_I16 ( int16_t in,  int16_t *p_out);

extern uint16_t lpf8     (uint16_t in, uint16_t * const p_out);
extern uint16_t lpf8_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  lpf8_I16 ( int16_t in,  int16_t *p_out);
extern uint16_t lpf8_U32  (uint16_t in, uint32_t *p_out);
extern int16_t  lpf8_I32  ( int16_t in,  int32_t *p_out);

extern uint16_t lpf9_U32  (uint16_t in, uint32_t *p_out);
extern int16_t  lpf9_I32  ( int16_t in,  int32_t *p_out);

extern uint16_t lpf10_U32 (uint16_t in, uint32_t *p_out);
extern int16_t  lpf10_I32 ( int16_t in,  int32_t *p_out);

extern uint16_t lpf11_U32 (uint16_t in, uint32_t *p_out);
extern int16_t  lpf11_I32 ( int16_t in,  int32_t *p_out);

extern uint16_t lpf12_U32 (uint16_t in, uint32_t *p_out);
extern int16_t  lpf12_I32 ( int16_t in,  int32_t *p_out);

extern uint16_t lpf13_U32 (uint16_t in, uint32_t *p_out);
extern int16_t  lpf13_I32 ( int16_t in,  int32_t *p_out);

extern uint16_t lpf14_U32 (uint16_t in, uint32_t *p_out);
extern int16_t  lpf14_I32 ( int16_t in,  int32_t *p_out);

extern uint16_t lpf15_U32 (uint16_t in, uint32_t *p_out);
extern int16_t  lpf15_I32 ( int16_t in,  int32_t *p_out);

/* to add init functions */

extern uint16_t fhystn_U16   (uint16_t in, uint16_t *p_out, const uint16_t hyst);
extern uint16_t fhyst1_U16   (uint16_t in, uint16_t *p_out);
extern uint16_t fhyst16_U16  (uint16_t in, uint16_t *p_out);
extern uint16_t fhyst64_U16  (uint16_t in, uint16_t *p_out);
extern uint16_t fhyst256_U16 (uint16_t in, uint16_t *p_out);
extern int16_t  fhystn_I16   ( int16_t in,  int16_t *p_out, const uint16_t hyst);
extern int16_t  fhyst1_I16   ( int16_t in,  int16_t *p_out);
extern int16_t  fhyst16_I16  ( int16_t in,  int16_t *p_out);
extern int16_t  fhyst64_I16  ( int16_t in,  int16_t *p_out);
extern int16_t  fhyst256_I16 ( int16_t in,  int16_t *p_out);
extern int16_t  fhystan_I16  ( int16_t in,  int16_t *p_out, const uint16_t hyst);
extern int16_t  fhysta1_I16  ( int16_t in,  int16_t *p_out);
extern int16_t  fhysta16_I16 ( int16_t in,  int16_t *p_out);
extern int16_t  fhysta64_I16 ( int16_t in,  int16_t *p_out);
extern int16_t  fhysta256_I16( int16_t in,  int16_t *p_out);

extern int16_t  fhyst16_A16c ( int16_t in,  int16_t * const p_out);
extern int16_t  fhyst16_I16c ( int16_t in,  int16_t * const p_out);
extern uint16_t fhyst16_U16c (uint16_t in, uint16_t * const p_out);

static __inline__ uint16_t convert_to_offset_binary (int16_t input) __attribute__ ((always_inline));

/** Function for converting a signed 16 bit integer (2s complement) to offset binary format
 *
 *  @param[in]  input   The signed input to be converted
 *  @return     The input data in offset binary format
 */
static __inline__ uint16_t convert_to_offset_binary (int16_t input)
{
    uint16_t tmp = (uint16_t)input;
    tmp += 0x8000u;
    return tmp;
}

/* Notes:
    [1] p_in may contain other values than the provided input values!
    [2] Pointer to filter memory `p_in` is converted from `int16_t *` to
        `uint16_t *` while passig parameters form the "signed" version to
        "unsigned" version of the filter. Such pointer conversion uses
        intermediate conversion to `void *` type to help MISRA-2012 checker
        identify this legitimate case (will be reported as deviation of
        the advisory Rule 11.5 instead of required Rule 11.3).
 */

/** Initializes FIR filter delay line of size *n* with value *init_value*
 *
 *  @param[in]  init_value  The initial value for the filter
 *  @param[in]  n           The size of the filter
 *  @param[out] *p_in       The initialized filter contents
 *
 *  @note   This function makes use of the unsigned version of the function.
 *          In order to do so, the signed input value is converted to offset
 *          binary format.
 */
static __inline__ void   firI16_init(int16_t *p_in, int16_t init_value, uint16_t n)
{
    fir_init ((uint16_t * const)(void *)p_in, convert_to_offset_binary(init_value), n);
}

/** FIR11 filter
 *
 *  Calculates \f$ out = (in[n] + in[n-1]) / 2 \f$, and updates filter delay line
 *
 *  @param[in]      in      Input value for filter
 *  @param[in,out]  *p_in   Filter delay line
 *
 *  @return     The result of the filter calculation
 *
 *  @note       This function makes use of the unsigned version of the function.
 *              In order to do so:
 *              - The signed input value is converted to offset binary format.
 *              - The output of the unsigned function (in offset binary format)
 *                is converted back to 2s-complement
 */
static __inline__ int16_t fir11_I16   (int16_t in, int16_t *p_in)
{
    uint16_t tmp = fir11 (convert_to_offset_binary(in), (uint16_t *)(void *)p_in) - 0x8000U; /* see note [2] above */
    return (int16_t)tmp;
}

/** FIR121 filter
 *
 *  Calculates \f$ out = (in[n] + 2 * in[n-1] + in[n-2]) / 4 \f$, and updates filter delay line
 *
 *  @param[in]      in      Input value for filter
 *  @param[in,out]  *p_in   Filter delay line
 *
 *  @return     The result of the filter calculation
 *
 *  @note       This function makes use of the unsigned version of the function.
 *              In order to do so:
 *              - The signed input value is converted to offset binary format.
 *              - The output of the unsigned function (in offset binary format)
 *                is converted back to 2s-complement
 */
static __inline__ int16_t fir121_I16  (int16_t in, int16_t *p_in)
{
    uint16_t tmp = fir121 (convert_to_offset_binary(in), (uint16_t *)(void *)p_in) - 0x8000U; /* see note [2] above */
    return (int16_t)tmp;
}

/** FIR1111 filter
 *
 *  Calculates \f$ out = (in[n] + in[n-1] + in[n-2] + in[n-3]) / 4 \f$, and updates filter delay line
 *
 *  @param[in]      in      Input value for filter
 *  @param[in,out]  *p_in   Filter delay line
 *
 *  @return     The result of the filter calculation
 *
 *  @note       This function makes use of the unsigned version of the function.
 *              In order to do so:
 *              - The signed input value is converted to offset binary format.
 *              - The output of the unsigned function (in offset binary format)
 *                is converted back to 2s-complement
 */
static __inline__ int16_t fir1111_I16 (int16_t in, int16_t *p_in)
{
    uint16_t tmp = fir1111 (convert_to_offset_binary(in), (uint16_t *)(void *)p_in) - 0x8000U; /* see note [2] above */
    return (int16_t)tmp;
}

/** FIR1331 filter
 *
 *  Calculates \f$ out = (in[n] + 3*in[n-1] + 3*in[n-2] + in[n-3]) / 8 \f$, and updates filter delay line
 *
 *  @param[in]      in      Input value for filter
 *  @param[in,out]  *p_in   Filter delay line
 *
 *  @return     The result of the filter calculation
 *
 *  @note       This function makes use of the unsigned version of the function.
 *              In order to do so:
 *              - The signed input value is converted to offset binary format.
 *              - The output of the unsigned function (in offset binary format)
 *                is converted back to 2s-complement
 */
static __inline__ int16_t fir1331_I16 (int16_t in, int16_t *p_in)
{
    uint16_t tmp = fir1331 (convert_to_offset_binary(in), (uint16_t *)(void *)p_in) - 0x8000U; /* see note [2] above */
    return (int16_t)tmp;
}

/** FIR12221 filter
 *
 *  Calculates \f$ out = (in[n] + 2*in[n-1] + 2*in[n-2] + 2*in[n-3] + in[n-4]) / 8 \f$ , and updates filter delay line
 *
 *  @param[in]      in      Input value for filter
 *  @param[in,out]  *p_in   Filter delay line
 *
 *  @return     The result of the filter calculation
 *
 *  @note       This function makes use of the unsigned version of the function.
 *              In order to do so:
 *              - The signed input value is converted to offset binary format.
 *              - The output of the unsigned function (in offset binary format)
 *                is converted back to 2s-complement
 */
static __inline__ int16_t fir12221_I16(int16_t in, int16_t *p_in)
{
    uint16_t tmp = fir12221 (convert_to_offset_binary(in), (uint16_t *)(void *)p_in) - 0x8000U; /* see note [2] above */
    return (int16_t)tmp;
}

/* Note: p_out may contain other values than the provided input values! */

/** Initializes IIR filter delay line of size *n* with value *init_value*
 *
 *  @param[in]  init_value  The initial value for the filter
 *  @param[in]  n           The size of the filter
 *  @param[out] *p_out      The initialized filter contents
 *
 *  @note   This function makes use of the unsigned version of the function.
 *          In order to do so, the signed input value is converted to offset
 *          binary format.
 */
static __inline__ void  iirI16_init(int16_t *p_out, int16_t init_value, uint16_t n)
{
    iir_init ((uint16_t *)(void *)p_out, convert_to_offset_binary(init_value), n);
}

/** Initializes IIR filter delay line of size *n* with value *init_value*
 *
 *  @param[in]  init_value  The initial value for the filter
 *  @param[in]  n           The size of the filter
 *  @param[out] *p_out      The initialized filter contents
 *
 *  @note   This function makes use of the unsigned version of the function.
 *          In order to do so, the signed input value is converted to offset
 *          binary format.
 */
static __inline__ void  iirI32_init(int32_t *p_out, int16_t init_value, uint16_t n)
{
    iirU32_init ((uint32_t * const)(void *)p_out, convert_to_offset_binary(init_value), n);
}

/** First order IIR filter
 *
 *  Calculates
 *
 *  - \f$ out[k] = (1+a) * in - a*out[k-1] \f$
 *  - \f$ out[k] = in + a * (in - out[k-1]) \f$
 *
 *  and updates filter delay line
 *
 *  @param[in]      in      Input value for filter
 *  @param[in,out]  *p_out  Filter delay line, with values in Q0.16 format
 *  @param[in]      a       Coefficient in Q0.16 format, representing a floating
 *                          point value in the range of [0 .. 1) with resolution
 *                          of 2^-16
 *
 *  @return         The result of the filter calculation
 *
 *  @note       Supports full input range [-32768 .. 32767]
 *  @note       This function makes use of the unsigned version of the function.
 *              In order to do so:
 *              - The signed input value is converted to offset binary format.
 *              - The output of the unsigned function (in offset binary format)
 *                is converted back to 2s-complement
 *
 *  @warning  The calculations will not exercise any overflow and underflow.
 */
static __inline__ int16_t iir1_I16   (int16_t in, int16_t *p_out, uint16_t a)
{
    uint16_t tmp = iir1 (convert_to_offset_binary(in), (uint16_t *)(void *)p_out, a) - 0x8000U; /* see note [2] above */
    return (int16_t)tmp;
}

/** First order IIR filter
 *
 *  Calculates
 *
 *  - \f$ out[k] = (1+a) * in - a*out[k-1] \f$
 *  - \f$ out[k] = in + a * (in - out[k-1]) \f$
 *
 *  and updates filter delay line
 *
 *  @param[in]      in      Input value for filter
 *  @param[in,out]  *p_out  Filter delay line, with values in Q16.16 format
 *  @param[in]      a       Coefficient in Q0.16 format, representing a floating
 *                          point value in the range of [0 .. 1) with resolution
 *                          of 2^-16
 *
 *  @return         The result of the filter calculation
 *
 *  @note       Supports full input range [-32768 .. 32767]
 *  @note       This function makes use of the unsigned version of the function.
 *              In order to do so:
 *              - The signed input value is converted to offset binary format.
 *              - The output of the unsigned function (in offset binary format)
 *                is converted back to 2s-complement
 *  @warning  The calculations will not exercise any overflow and underflow.
 */
static __inline__ int16_t iir1_I32   (int16_t in, int32_t *p_out, uint16_t a)
{
    uint16_t tmp = iir1_U32 (convert_to_offset_binary(in), (uint32_t *)(void *)p_out, a) - 0x8000U; /* see note [2] above */
    return (int16_t)tmp;
}

#endif /* MATHLIB_FILTER_H_ */
