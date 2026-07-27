/**
 * @file
 * @brief Header file for functions specifically optimized for the MLX16 processor
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

#ifndef BUILTIN_MLX16_H
#define BUILTIN_MLX16_H

#include <stdint.h>
#include <mlx16_cfg.h>

static __attribute__((always_inline)) inline void      builtin_mlx16_disable_interrupts (void);
static __attribute__((always_inline)) inline void      builtin_mlx16_set_priority (uint8_t level);
static __attribute__((always_inline)) inline uint16_t  builtin_mlx16_get_status (void);
static __attribute__((always_inline)) inline void      builtin_mlx16_set_status (uint16_t status);

static __attribute__((always_inline)) inline void      builtin_mlx16_enter_user_mode (void);

static __attribute__((always_inline)) inline uint16_t  builtin_mlx16_fsb (uint16_t v);
static __attribute__((always_inline)) inline uint16_t  builtin_mlx16_sfb(uint16_t v);
static __attribute__((always_inline)) inline int16_t   builtin_mlx16_fsb32(uint32_t v);
static __attribute__((always_inline)) inline uint32_t  builtin_mlx16_sfb32(uint16_t v);

static __attribute__((always_inline)) inline void      builtin_mlx16_movb (uint8_t volatile *addr,
                                                                           uint8_t mask,
                                                                           uint8_t value);
static __attribute__((always_inline)) inline void      builtin_mlx16_nop (void);
static __attribute__((always_inline)) inline int16_t   builtin_mlx16_iavg(int16_t val1, int16_t val2);
static __attribute__((always_inline)) inline uint16_t  builtin_mlx16_uavg(uint16_t val1, uint16_t val2);
static __attribute__((always_inline)) inline int16_t   builtin_mlx16_q15(int32_t v);

/**
    Disable all interrupts

    Disables all interrupts by setting CPU priority to 0 (the highest).
    The CPU priority is located in M register -- MH[2:0].

    This function actually compiles into three lines of assembly, so there is no function
    call overhead.  However, the macro also implies a <i>memory barrier</i>
    which can cause additional loss of optimization.

    In order to implement atomic access to multi-byte objects,
    consider using the ENTER_SECTION(ATOMIC_KEEP_MODE) macro from <atomic.h>, rather than
    implementing them manually.
 */
static __attribute__((always_inline)) inline
void builtin_mlx16_disable_interrupts(void)
{
    __asm__ __volatile__ (
        "clrb MH.2\n\t"     /* clear Pr part only staring from msbit */
        "clrb MH.1\n\t"
        "clrb MH.0"
        ::: "memory"
        );
}


/**
    Set CPU priority (UPr register)

    Sets CPU priority to the level specified by the <b>level</b> parameter.
    All interrupts with level greater than specified CPU priority will be effectively
    disabled. Please note that the lower level corresponds to the higher priority
    with level 0 being the highest priority.

    This function implies a <i>memory barrier</i> which can cause additional loss of optimization.

    @param level [0-7]      CPU priority level
 */
static __attribute__((always_inline)) inline
void builtin_mlx16_set_priority (uint8_t level)
{
    __asm__ __volatile__ (
        "mov UPR, %[upr]"
        :
        :  [upr] "ri" (level)
        : "memory");
}


/**
    Get CPU status (M register) value

    @return M register value
 */
static __attribute__((always_inline)) inline
uint16_t builtin_mlx16_get_status (void)
{
    uint16_t status;

    __asm__ __volatile__ (
        "mov %[dest], M" "\n\t"
        : [dest] "=r" (status)
        :: "memory"
        );

    return status;
}

/**
    Set CPU status (M register)

    @param status   Value to be written into M register

 */
static __attribute__((always_inline)) inline
void builtin_mlx16_set_status (uint16_t status)
{
    __asm__ __volatile__ (
        "mov M, %[src]"
        :
        : [src] "r" (status)
        : "memory", "M"
        );
}


/**
    Switch CPU into User Mode

 */
static __attribute__((always_inline)) inline
void builtin_mlx16_enter_user_mode (void)
{
    __asm__ __volatile__ ("setb MH.3" ::: "memory");
}


/** FSB: Find position (index) of the first set bit starting from the MSbit.
 *
 *  Example usage:
 *
 *      - builtin_mlx16_fsb(0) = 0
 *      - builtin_mlx16_fsb(1) = 0
 *      - builtin_mlx16_fsb(0xFFFF) = 15
 *
 *  @param v    bit pattern
 *
 *  @return     index of the highest set bit; note that function returns 0
 *              if all bits are cleared:
 *
 *  @internal
 *  Uses MLX16's `FSB` instruction, if available
 *
 *  FSB     <dst>
 *          [A | X | Y]
 */
static __attribute__((always_inline)) inline
uint16_t builtin_mlx16_fsb(uint16_t v)
{
#if defined(HAS_MLX16_FSB_SFB_INSTRUCTIONS)
    __asm__ __volatile__ ("fsb %[res]" : [res] "=r" (v) : "0" (v));

    return v;
#else
    uint16_t result = 0u;

    if (v != 0u) {
        /* alt: first do a binary tree search */
        uint16_t one = 0x8000u;
        result = 15;

        while ((v & one) == 0u) {
            one >>= 1;
            result--;
        }
    } else {
        /* builtin_mlx16_fsb(0) is 0 to match MLX16 FSB instruction */
    }

    return result;
#endif /* HAS_MLX16_FSB_SFB_INSTRUCTIONS */
}

/** SFB: Setting the bit at position in the 16-bit register.
 *
 *  Example usage:
 *
 *      - builtin_mlx16_sfb(0) = 0x0001
 *      - builtin_mlx16_sfb(1) = 0x0002
 *      - builtin_mlx16_sfb(15) = 0x8000
 *
 *  @param[in]  v   The bit number of the bit that needs to be set
 *
 *  @return     A 16 bit integer with one bit set: the one with the bit number
 *                indicated by the argument.
 *
 *  @internal
 *  Uses MLX16's `SFB` instruction, if available
 *
 *  SFB     <dst>
 *          [A | X | Y]
 */
static __attribute__((always_inline)) inline
uint16_t builtin_mlx16_sfb(uint16_t v)
{
#if defined(HAS_MLX16_FSB_SFB_INSTRUCTIONS)
    __asm("sfb %[res]" : [res] "=b" (v) : "0" (v));
    return v;
#else
    uint16_t one = 1u;

    /* first a binary tree search */
    /* 2 level tree */
    /* total time, looping over all 16 values: 81us */
    one = 1u;
    if (v > 8u) {
        v -= 8u;
        one = 1u << 8u;
    }
    if (v > 4u) {
        v -= 4u;
        one <<= 4u;
    }
    while (0u != v--) {
        one <<= 1u;
    }

    return one;
#endif /* HAS_MLX16_FSB_SFB_INSTRUCTIONS */
}

/** FSB: Find position (index) of the first set bit starting from the MSbit
 *  in 32-bit integer
 *
 *  Example usage:
 *
 *      - builtin_mlx16_fsb32(0) = 0
 *      - builtin_mlx16_fsb32(1) = 0
 *      - builtin_mlx16_fsb32(0xFFFFFFFF) = 31
 *
 *  @param v    32-bit pattern
 *
 *  @return     index of the highest set bit; note that function returns 0
 *              if all bits are cleared:
 *
 *  @internal
 *  Uses MLX16's `FSB` instruction, if available
 *
 *  FSB     <dst>
 *          [A | X | Y]
 */
static __attribute__((always_inline)) inline
int16_t builtin_mlx16_fsb32(uint32_t v)
{
    uint16_t result;

    if ((v >> 16u) == 0u) {
        result = builtin_mlx16_fsb((uint16_t) v);
    } else {
        result = 16u + builtin_mlx16_fsb(((uint16_t) (v >> 16u)));
    }

    return (int16_t)result;
}

/** SFB: Setting the bit at position in the 32-bit register.
 *
 *  Example usage:
 *
 *      - builtin_mlx16_sfb32(0) = 0x0001
 *      - builtin_mlx16_sfb32(1) = 0x0002
 *      - builtin_mlx16_sfb32(31) = 0x80000000
 *
 *  @param[in]  v   The bit number of the bit that needs to be set
 *
 *  @return     A 32 bit integer with one bit set: the one with the bit number
 *              indicated by the argument.
 *
 *  @internal
 *  Uses MLX16's `SFB` instruction, if available
 *
 *  SFB     <dst>
 *          [A | X | Y]
 */
static __attribute__((always_inline)) inline
uint32_t builtin_mlx16_sfb32(uint16_t v)
{
    uint32_t result;

    if (v < 16u) {
        result = (uint32_t) builtin_mlx16_sfb(v);
    } else {
        result = ((uint32_t) builtin_mlx16_sfb(v - 16U)) << 16u;
    }

    return result;
}


/**
    MOVB: Change (set/clear) atomically one or several bits in the byte

    @param  addr    Pointer to the byte
    @param  mask    Bit mask to indicate target bits
    @param  value   Bit pattern to be written to target bits


    The function is atomic equivalent of the following expression:
    `*addr = (*addr & ~mask) | (value & mask)`

    @code{.c}
    uint8_t v = 0xA5;

    builtin_mlx16_movb(&v, 0x3C, 0x18);
    assert(v == 0x99);

    @endcode

    @internal
    MOVB  <dst>,     <src>
          [X | Y]    A | X | Y

    mask  = high_byte(src)
    value = low_byte(src)
    dst = (dst & ~mask) | (value & mask)
 */
static __attribute__((always_inline)) inline
void builtin_mlx16_movb(uint8_t volatile *addr, uint8_t mask, uint8_t value)
{
    uint16_t mask_and_value = ((uint16_t)mask << 8u) | value;

    __asm__ __volatile__ (
        "movb [%[addr]], %[mask_and_value]\n\t"
        :
        : [addr] "c" (addr), [mask_and_value] "r" (mask_and_value)
        : "memory"
        );
}


/**
    NOP: No operation

    Compiles into CPUs `nop` instruction
 */
static __attribute__((always_inline)) inline
void builtin_mlx16_nop(void)
{
    __asm__ __volatile__ ("nop" ::);
}



/** Calculating average of 2 unsigned 16-bit integers
 *
 *  In case the code is compiled for the MLX16, an algorithm is used for the
 *  calculation which prevents promotion to 32 bit, resulting in a faster
 *  implementation
 *
 *  @param[in]  val1    First unsigned 16 bit integer for average calculation
 *  @param[in]  val2    Second unsigned 16 bit integer for average calculation
 *  @return     The averaged value of the 2 input parameters
 */
static __attribute__((always_inline)) inline
uint16_t builtin_mlx16_uavg(uint16_t val1, uint16_t val2)
{
#if defined (__MLX16__) && (!defined(__COVERITY__) && !defined(__POLYSPACE__)) && !defined(MLX_FORCE_C_IMPLEMENTATION)
    uint16_t result;

    __asm__ (
        "add %0, %2\n\t"
        "rrc %0"
        : "=r" (result)
        : "0" (val1), "g" (val2) );

    return result;
#else
    uint32_t sum;
    __asm__ ("; dummy asm code to attract Coverity or Polyspace");

    sum = (uint32_t) val1 + val2;
    sum /= 2u;

    return (uint16_t) sum;
#endif /* __MLX16__ */
}

/** Calculating average of 2 signed 16-bit integers
 *
 *  In case the code is compiled for the MLX16, an algorithm is used for the
 *  calculation which prevents promotion to 32 bit, resulting in a faster
 *  implementation
 *
 *  @param[in]  val1    First signed 16 bit integer for average calculation
 *  @param[in]  val2    Second signed 16 bit integer for average calculation
 *  @return     The averaged value of the 2 input parameters
 */
static __attribute__((always_inline)) inline
int16_t builtin_mlx16_iavg(int16_t val1, int16_t val2)
{
#if defined(__MLX16__) && (!defined(__COVERITY__) && !defined(__POLYSPACE__)) && !defined(MLX_FORCE_C_IMPLEMENTATION)
    int16_t result;

    __asm__ (
        "add %0, %2\n\t"
        "jnv NoOverflow_%=\n\t"
        "rrc %0\n\t"
        "jmp Done_%=\n"
        "NoOverflow_%=:\n\t"
        "asr %0, #1\n"
        "Done_%=:"
        : "=r" (result)
        : "0" (val1), "g" (val2) );

    return result;
#else
    int32_t sum;
    __asm__ ("; dummy asm code to attract Coverity or Polyspace");

    sum = (int32_t) val1 + val2;
    sum /= 2;

    return (int16_t) sum;
#endif /* __MLX16__*/
}

/** Conversion of q30 to q15 number representation
 *
 *  Note: may result in stack reservation, but not all used :-(
 *
 *  @param[in]  v   q30 number to convert to q15
 *  @return     q15 result
 */
static __attribute__((always_inline)) inline
int16_t builtin_mlx16_q15(int32_t v)
{
#if defined(__MLX16__) && !defined(HAS_MLX16_COPROCESSOR) && (!defined(__COVERITY__) && !defined(__POLYSPACE__)) && \
    !defined(MLX_FORCE_C_IMPLEMENTATION)

    /* v >> 15 */
    int16_t result;

    __asm__ (
        "asl A\n\t"
        "rlc Y"
        : "=y" (result)
        : "b" (v) /* YA register */
        /* : "Y", "A"  */
        );

    return result;
#else
    __asm__ ("; dummy asm code to attract Coverity or Polyspace");

    uint32_t u32 = (uint32_t)v;     /* cast to unsigned to make MISRA compliant unsigned shift .. */
    u32 = u32 >> 15u;               /* extract bits [30:15]: xiiii-iiii-iiii-iiii-ixxx-xxxx-xxxx-xxxx */

    return (int16_t)u32;
#endif
}

#endif /* BUILTIN_MLX16_H */
