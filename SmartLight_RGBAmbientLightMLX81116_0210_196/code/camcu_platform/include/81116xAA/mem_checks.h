/**
 * @file
 * @brief Provides a checksum calculation functions
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git 5f2794fe
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


#ifndef MEM_CHECKS_H
#define MEM_CHECKS_H

#include <stdint.h>

#ifndef MEM_CHECKS_PREFIX
/** Defines memory region to be used for memory checks functions */
#define MEM_CHECKS_PREFIX __ROM_MLX_TEXT__
#endif

/** Initial value of the CRC16 CCITT check. 0xFFFF or 0x1D0F could be used. 0xFFFF is standard but 0x1D0F is used by SREC_CAT tool. */
#define CCITT_SEED            0x1D0F
/** Expected CRC result after calculation the checksum and included checksum-value within the region (checksum value should be adapted to have NVRAM_CORRECT_CRC result */
#define NVRAM_CORRECT_CRC     0x00FFu

uint16_t nvram_CalcCRC( const uint16_t *pu16BeginAddress, const uint16_t u16Length);

#endif /* MEM_CHECKS_H */


