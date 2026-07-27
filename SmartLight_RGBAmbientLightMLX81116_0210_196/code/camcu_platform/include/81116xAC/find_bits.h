/**
 * @file
 * @brief Header file for functions related to finding set bits in a word/byte
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

#ifndef FIND_BITS_H
#define FIND_BITS_H

#include <stdint.h>

/* Each return the position, zero-indexed from the right, or -1 for not found. */
int8_t find_first_one_left  (uint16_t search_word);
int8_t find_first_one_right (uint16_t search_word_in);
int8_t find_first_one_right_U8_fast(uint8_t x);
int8_t find_first_zero_left (uint16_t search_word);
int8_t find_first_zero_right    (uint16_t search_word);

#endif
