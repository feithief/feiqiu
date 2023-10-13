/**
 * @file
 * @brief General mathlib header file
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

#ifndef MATHLIB_H_
#define MATHLIB_H_

#include <mlx16_cfg.h>

/* SW component version */
#define MLX_MATHLIB_SW_MAJOR_VERSION    2
#define MLX_MATHLIB_SW_MINOR_VERSION    12
#define MLX_MATHLIB_SW_PATCH_VERSION    0

#ifdef MLX_FORCE_C_IMPLEMENTATION
#warning "MLX_FORCE_C_IMPLEMENTATION is defined in mathlib"
#endif

/* The user should not be able to define the following macros */
#if defined (__COVERITY__) || defined (__POLYSPACE__)
#warning "COVERITY or POLYSPACE should not be explicitly defined (ignore this warning for static analysis builds)"
#endif

/* Validate MLX16-GCC version */
#if ((__MLX16_GCC_MAJOR__ == 1) && (__MLX16_GCC_MINOR__ >= 8)) || (__MLX16_GCC_MAJOR__ > 1)
/* ok */
#else
#warning "Math library requires MLX16-GCC release 1.8 or later"
#endif


#if !defined (__ASSEMBLER__)

#include <stdint.h>
#include <builtin_mlx16.h>
#include <find_bits.h>

/* Check MathLib INLINE_ASM availability;
   Choosing INLINE_ASM or ASM functions declaration */
#if defined (HAS_MLX16_COPROCESSOR) && !defined (__MATHLIB_NON_INLINE__)
/* Use inline asm implementation from mathlib_inline.h */
#define MATHLIB_INLINE static __inline__ __attribute__ ((always_inline))
#else
/* Use call-able implementation */
#define MATHLIB_INLINE
#endif

/*
 *	Multiplication
 */

MATHLIB_INLINE int16_t  mulI16_I8byI8(int8_t multiplicand, int8_t multiplier);
MATHLIB_INLINE int16_t  mulI16_I8byU8(int8_t multiplicand, uint8_t multiplier);
MATHLIB_INLINE uint16_t mulU16_U8byU8(uint8_t multiplicand, uint8_t multiplier);

MATHLIB_INLINE int16_t  mulI16hi_I16byI8(int16_t multiplicand, int8_t multiplier);
MATHLIB_INLINE uint16_t mulU16hi_U16byU8(uint16_t multiplicand, uint8_t multiplier);
MATHLIB_INLINE int16_t  mulI16hi_I16byU8(int16_t multiplicand, uint8_t multiplier);

MATHLIB_INLINE int16_t  mulI16hi_I16byI16(int16_t multiplicand, int16_t multiplier);
MATHLIB_INLINE uint16_t mulU16hi_U16byU16(uint16_t multiplicand, uint16_t multiplier);
int16_t  mulI16hi_I16byU16(int16_t multiplicand, uint16_t multiplier);

MATHLIB_INLINE int32_t  mulI24_I16byI8(int16_t multiplicand, int8_t multiplier);
MATHLIB_INLINE uint32_t mulU24_U16byU8(uint16_t multiplicand, uint8_t multiplier);
MATHLIB_INLINE int32_t  mulI24_I16byU8(int16_t multiplicand, uint8_t multiplier);

MATHLIB_INLINE uint32_t mulU32_U16byU16(uint16_t multiplicand, uint16_t multiplier);
MATHLIB_INLINE int32_t  mulI32_I16byI16(int16_t multiplicand, int16_t multiplier);
int32_t  mulI32_I16byU16(int16_t multiplicand, uint16_t multiplier);

int32_t  mulI32lo_I32byI16(int32_t multiplicand, int16_t multiplier);
int32_t  mulI32lo_I32byU16(int32_t multiplicand, uint16_t multiplier);
uint32_t mulU32lo_U32byU16(uint32_t multiplicand, uint16_t multiplier);

int32_t  mulI32hi_I32byI16(int32_t multiplicand, int16_t multiplier);
int32_t  mulI32hi_I32byU16(int32_t multiplicand, uint16_t multiplier);
uint32_t mulU32hi_U32byU16(uint32_t multiplicand, uint16_t multiplier);

MATHLIB_INLINE int16_t  mulQ15_Q15byQ15(int16_t multiplicand, int16_t multiplier);

/*
 *	Division
 */

uint32_t divU32_U32byU16(uint32_t n, uint16_t d);
int32_t  divI32_I32byI16(int32_t n, int16_t d);
int32_t  divI32_I32byU16(int32_t n, uint16_t d);

MATHLIB_INLINE uint16_t divU16_U32byU16(uint32_t dividend, uint16_t divisor);
int16_t  divI16_I32byI16(int32_t n, int16_t d);
int16_t  divI16_I32byU16(int32_t n, uint16_t d);

uint16_t divU16hi_U16byU16(uint16_t n, uint16_t d);
int16_t  divI16hi_I16byI16(int16_t n, int16_t d);
int16_t  divI16hi_I16byU16(int16_t n, uint16_t d);

uint8_t  divU8_U8byU8(uint8_t n, uint8_t d);
int8_t   divI8_I8byI8(int8_t n, int8_t d);
int8_t   divI8_I8byU8(int8_t n, uint8_t d);

uint8_t  divU8hi_U8byU8(uint8_t n, uint8_t d);

/*
 *	Power
 */

uint16_t isqrt16 (uint16_t a);
uint16_t isqrt32 (uint32_t a);

/*
 *	log/exp
 */

uint16_t ilog2_U16 (uint16_t v);
uint16_t ilog2_U32 (uint32_t v);

uint16_t iexp2_U16 (uint16_t v);
uint32_t iexp2_U32 (uint16_t v);

/*
 *	Trigonometric
 */

int16_t sinU16 (uint16_t in);
int16_t sinI16 (int16_t x);

int16_t cosU16 (uint16_t x);
int16_t cosI16 (int16_t x);

int32_t tanU16 (uint16_t x);
int32_t tanI16 (int16_t x);

int16_t atan2U16 (uint16_t y, uint16_t x);
int16_t atan2I16 (int16_t y, int16_t x);

/*
 *	Other
 */

uint32_t rand32(uint32_t seed);

void   init_lfsr16(uint16_t seed);
void   init_lfsr32(uint32_t seed);
uint16_t lfsr16(void);
uint16_t lfsr16_B400 (void);
uint32_t lfsr32(void);
uint32_t lfsr32_0000000C (void);
uint32_t lfsr32_80200003 (void);

uint16_t parity4 (uint8_t in);
uint16_t parity8 (uint8_t in);
uint16_t parity16(uint16_t in);
uint16_t parity32(uint32_t v);

uint8_t   bitrev4 (uint8_t x);
uint8_t   bitrev8 (uint8_t x);
uint16_t  bitrev16(uint16_t in);

uint8_t   interleave4 (uint8_t x, uint8_t y);
uint16_t  interleave8 (uint8_t x, uint8_t y);
uint32_t  interleave16(uint16_t x, uint16_t y);

#ifdef MATHLIB_HAS_DEPRECATED_API
#include "mathlib_deprecated.h"
#endif

/* Check MathLib INLINE_ASM availability */
#if defined (HAS_MLX16_COPROCESSOR) && !defined (__MATHLIB_NON_INLINE__)
#include "mathlib_inline.h"
#endif

#endif /* ! __ASSEMBLER__ */

#endif /* MATHLIB_H_ */
