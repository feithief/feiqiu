/**
 * @file
 * @brief Provides a set of functions used for the bootloaders and can be used in the Software Application in FLASH
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
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
 * @ingroup CAMCU_library
 *
 * @details
 */

#ifndef BL_TOOLS_H
#define BL_TOOLS_H

#include "compiler_abstraction.h"
#include "bl_defs.h"
#include "io.h"
#include "sys_tools.h"

void dataCopy64(uint16_t *src, uint16_t *dest, uint8_t size);
void dataFill16( uint16_t *dest,  uint16_t pattern, uint8_t size);
void MLX16_RESET(uint16_t reason);
void MLX16_RESET_SIGNED(uint16_t reason);
void SetRstatFlag(void);
void ResetRstatFlag(void);

/** dataCopy16 copies up to 16 words of data from src into dest area ascending
 * @param[in] *src source data to copy
 * @param[in] *dest destination for the copying
 * @param[in] size data size in 16bit words.
 * @warning The data size should be within the [1..15]. In case the size=0 or 16 - this call will copy 16*2 bytes
 */
static INLINE void dataCopy16(uint16_t *src, uint16_t *dest, uint8_t size)
{
    uint16_t *src_out;
    uint16_t *dest_out;

#if defined(__POLYSPACE__)
    (void)memcpy(dest, src, size);
#else
    __asm__ __volatile__
    (
        "mov Cx, %[SIZE]\n\t"
        "movsw  [%[DST]++],[%[SRC]++]\n\t"
        "djnz   Cx, .-2\n\t"
        : "=c" (src_out), "=c" (dest_out) /* clobbering by specifying as output */
        : [SRC] "c" (src), [DST] "c" (dest), [SIZE] "r" (size)
        : "memory", "M"/*if it means Cx as a part of M*/
    );
#endif
}

/** Legacy declaration */
#define dataCopy16_(src, dest, size)  dataCopy16(src, dest, size)

/** copy64Bit copies the 64 bit data per 16bit words from source to destination.
 * @param[in] *source source data to copy
 * @param[in] *dest destination for the copying
 * @note No additional checks were performed.
 */
static INLINE void copy64Bit( uint16_t *source,  uint16_t *dest)
{
    uint16_t *src_out;
    uint16_t *dest_out;

    __asm__ __volatile__ (                           /* Copy the data into the NVRAM buffer. DONT USE JUMPS HERE! */
        "movsw [%[dst]++],[%[src]++]\n\t"
        "movsw [%[dst]++],[%[src]++]\n\t"
        "movsw [%[dst]++],[%[src]++]\n\t"
        "movsw [%[dst]++],[%[src]++]\n\t"
        : "=c" (src_out), "=c" (dest_out)     /* clobbering by specifying as output */
        : [src] "c" ((uint16_t)source), [dst] "c" ((uint16_t)dest)
        );
}



/** dataCopy32_extended copies the data in 32bit words from the data above 64k addresses to the buffer
 *
 * Allows to copy the data placed in the addresses above 64kb
 *
 * @param[in] *src_extended_address source data to copy, address is >64kb
 * @param[in] *dest destination for the copying
 * @param[in] size the size of data to be copied in 32bit words
 * @note No additional checks were performed.
 */
static __attribute__((always_inline)) inline void dataCopy32_extended(uint32_t src_extended_address,
                                                                      uint16_t *dest,
                                                                      uint16_t size)
{
    uint16_t *src_out;
    uint16_t *dest_out;

    uint8_t page = src_extended_address >> 16;
    __asm__ __volatile__
    (
        "cpy16ext%=:\t"
        "mov R, %[PAGE]\n\t"
        "mov D, [%[SRC]++]\n\t"
        "mov [%[DST]++], D\n\t"
        "sub %[SIZE], #1\n\t"
        "jnz\tcpy16ext%=\n\t"
        : "=c" (src_out), "=c" (dest_out) /* clobbering by specifying as output */
        : [SRC] "c" ((uint16_t)src_extended_address), [DST] "c" (dest), [SIZE] "r" (size), [PAGE] "i" (page)
        : "memory", "M"/*if it means Cx as a part of M*/
    );
}


/** within_including_u32() checks wether the argument is inside the region or not.
 * @param[in] arg the value to be compared. (32bit)
 * @param[in] min the smallest value to fit in the region.
 * @param[in] max the beggest value to fit in the region.
 * @retval 1 the arg value is within the region.
 * @retval 0 the arg value is outside the region.
 */
static INLINE uint16_t within_including_u32(uint32_t arg, uint32_t min, uint32_t max)
{
    return ((arg<=max) && (arg>=min));
}


/** within_including_u16() checks wether the argument is inside the region or not.
 * @param[in] arg the value to be compared. (16bit)
 * @param[in] min the smallest value to fit in the region.
 * @param[in] max the beggest value to fit in the region.
 * @retval 1 the arg value is within the region.
 * @retval 0 the arg value is outside the region.
 */
static INLINE uint16_t within_including_u16(uint16_t arg, uint16_t min, uint16_t max)
{
    return ((arg<=max) && (arg>=min));
}

/** within_including_u8() checks wether the argument is inside the region or not.
 * @param[in] arg the value to be compared. (8bit)
 * @param[in] min the smallest value to fit in the region.
 * @param[in] max the beggest value to fit in the region.
 * @retval 1 the arg value is within the region.
 * @retval 0 the arg value is outside the region.
 */
static INLINE uint16_t within_including_u8(uint8_t arg, uint8_t min, uint8_t max)
{
    return ((arg<=max) && (arg>=min));
}

#define WITHIN_INCLUDING(arg, min, max)                 ((arg<=max) && (arg>=min)) /** The universal comparison macros, without any checks and constraints */

#endif

