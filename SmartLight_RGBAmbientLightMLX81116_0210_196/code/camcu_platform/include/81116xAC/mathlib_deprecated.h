/**
 * @file
 * @brief Header file containing deprecated mathlib functions
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

#ifndef MATHLIB_DEPRECATED_H_
#define MATHLIB_DEPRECATED_H_

#include "builtin_mlx16.h"
#include "mathlib.h"
#include "find_bits.h"

__attribute__((deprecated, always_inline))
static inline void mlx16_disable_interrupts (void);
/** @deprecated Use builtin_mlx16_disable_interrupts instead */
static inline void mlx16_disable_interrupts (void)
{
    builtin_mlx16_disable_interrupts();
}
__attribute__((deprecated, always_inline))
static inline void mlx16_set_priority (uint8_t level);
/** @deprecated Use builtin_mlx16_set_priority instead */
static inline void mlx16_set_priority (uint8_t level)
{
    builtin_mlx16_set_priority(level);
}

__attribute__((deprecated, always_inline))
static inline uint16_t mlx16_get_status (void);
/** @deprecated Use builtin_mlx16_get_status instead */
static inline uint16_t mlx16_get_status (void)
{
    return builtin_mlx16_get_status();
}

__attribute__((deprecated, always_inline))
static inline void mlx16_set_status (uint16_t status);
/** @deprecated Use builtin_mlx16_set_status instead */
static inline void mlx16_set_status (uint16_t status)
{
    builtin_mlx16_set_status(status);
}

__attribute__((deprecated, always_inline))
static inline void mlx16_enter_user_mode (void);
/** @deprecated Use builtin_mlx16_enter_user_mode instead */
static inline void mlx16_enter_user_mode (void)
{
    builtin_mlx16_enter_user_mode();
}

__attribute__((deprecated, always_inline))
static inline int16_t _q15(int32_t v);
/** @deprecated Use builtin_mlx16_q15 instead */
static inline int16_t _q15(int32_t v)
{
    return builtin_mlx16_q15(v);
}

__attribute__((deprecated, always_inline))
static inline uint16_t _uavg(uint16_t val1, uint16_t val2);
/** @deprecated Use builtin_mlx16_uavg instead */
static inline uint16_t _uavg(uint16_t val1, uint16_t val2)
{
    return builtin_mlx16_uavg(val1, val2);
}

__attribute__((deprecated, always_inline))
static inline int16_t _iavg(int16_t val1, int16_t val2);
/** @deprecated Use builtin_mlx16_iavg instead */
static inline int16_t _iavg(int16_t val1, int16_t val2)
{
    return builtin_mlx16_iavg(val1, val2);
}

__attribute__((deprecated, always_inline))
static inline int16_t _fsb(uint16_t v);
/** @deprecated Use builtin_mlx16_fsb or find_first_one_left instead */
static inline int16_t _fsb(uint16_t v)
{
#if defined (HAS_MLX16_FSB_SFB_INSTRUCTIONS)
    return (int16_t)builtin_mlx16_fsb(v);
#else
    return (int16_t)find_first_one_left(v);
#endif /* HAS_MLX16_FSB_SFB_INSTRUCTIONS */
}

__attribute__((deprecated, always_inline))
static inline uint16_t _sfb(uint16_t v);
/** @deprecated Use builtin_mlx16_sfb instead */
static inline uint16_t _sfb(uint16_t v)
{
    return builtin_mlx16_sfb(v);
}

__attribute__((deprecated, always_inline))
static inline int16_t _fsb32(uint32_t v);
/** @deprecated Use builtin_mlx16_fsb32 instead */
static inline int16_t _fsb32(uint32_t v)
{
    return builtin_mlx16_fsb32(v);
}

__attribute__((deprecated, always_inline))
static inline uint32_t _sfb32(uint16_t v);
/** @deprecated Use builtin_mlx16_sfb32 instead */
static inline uint32_t _sfb32(uint16_t v)
{
    return builtin_mlx16_sfb32(v);
}

__attribute__((deprecated, always_inline))
static inline int16_t mulI16_I16byU16(int16_t multiplicand, uint16_t multiplier);
/** @deprecated Use mulI16hi_I16byU16 instead */
static inline int16_t mulI16_I16byU16(int16_t multiplicand, uint16_t multiplier)
{
    return mulI16hi_I16byU16(multiplicand, multiplier);
}

__attribute__((deprecated, always_inline))
static inline int32_t mulI32_I32byI16(int32_t multiplicand, int16_t multiplier);
/** @deprecated Use mulI32lo_I32byI16 instead */
static inline int32_t mulI32_I32byI16(int32_t multiplicand, int16_t multiplier)
{
    return mulI32lo_I32byI16(multiplicand, multiplier);
}

__attribute__((deprecated, always_inline))
static inline int32_t mulI32_I32byU16(int32_t multiplicand, uint16_t multiplier);
/** @deprecated Use mulI32lo_I32byU16 instead */
static inline int32_t mulI32_I32byU16(int32_t multiplicand, uint16_t multiplier)
{
    return mulI32lo_I32byU16(multiplicand, multiplier);
}

__attribute__((deprecated, always_inline))
static inline uint32_t mulU32_U32byU16(uint32_t multiplicand, uint16_t multiplier);
/** @deprecated Use mulU32lo_U32byU16 instead */
static inline uint32_t mulU32_U32byU16(uint32_t multiplicand, uint16_t multiplier)
{
    return mulU32lo_U32byU16(multiplicand, multiplier);
}

__attribute__((deprecated, always_inline))
static inline uint16_t divU16_U16byU16(uint16_t n, uint16_t d);
/** @deprecated Use divU16hi_U16byU16 instead */
static inline uint16_t divU16_U16byU16(uint16_t n, uint16_t d)
{
    return divU16hi_U16byU16(n, d);
}

__attribute__((deprecated, always_inline))
static inline int16_t divI16_I16byI16(int16_t n, int16_t d);
/** @deprecated Use divI16_I16byI16 instead */
static inline int16_t divI16_I16byI16(int16_t n, int16_t d)
{
    return divI16hi_I16byI16(n, d);
}

__attribute__((deprecated, always_inline))
static inline int16_t divI16_I16byU16(int16_t n, uint16_t d);
/** @deprecated Use divI16_I16byU16 instead */
static inline int16_t divI16_I16byU16(int16_t n, uint16_t d)
{
    return divI16hi_I16byU16(n, d);
}

__attribute__((deprecated, always_inline))
static inline int16_t  mulI16_I16byI16(int16_t multiplicand, int16_t multiplier);
/** @deprecated Use mulI16hi_I16byI16 instead */
static inline int16_t  mulI16_I16byI16(int16_t multiplicand, int16_t multiplier)
{
    return mulI16hi_I16byI16(multiplicand, multiplier);
}

__attribute__((deprecated, always_inline))
static inline uint16_t mulU16_U16byU16(uint16_t multiplicand, uint16_t multiplier);
/** @deprecated Use mulU16hi_U16byU16 instead */
static inline uint16_t mulU16_U16byU16(uint16_t multiplicand, uint16_t multiplier)
{
    return mulU16hi_U16byU16(multiplicand, multiplier);
}

#endif /* MATHLIB_DEPRECATED_H_ */
