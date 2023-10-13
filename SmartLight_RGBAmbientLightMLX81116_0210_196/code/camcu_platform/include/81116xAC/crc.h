/**
 * @file
 * @brief Header file for various CRC calculation functions
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

#ifndef MATHLIB_CRC_H_
#define MATHLIB_CRC_H_

#include <stdint.h>

extern uint8_t  crc8  (uint8_t c, uint8_t seed);
extern uint16_t crc16 (uint8_t c, uint16_t seed);
extern uint32_t crc32 (uint8_t c, uint32_t seed);
extern uint16_t crc_ccitt(uint8_t c, uint16_t seed);

extern uint8_t  crc8_table256  (uint8_t c, uint8_t seed);
extern uint16_t crc16_table256 (uint8_t c, uint16_t seed);
extern uint32_t crc32_table256 (uint8_t c, uint32_t seed);

extern uint8_t  crc8_table16   (uint8_t c, uint8_t seed);
extern uint16_t crc16_table16  (uint8_t c, uint16_t seed);
extern uint32_t crc32_table16  (uint8_t c, uint32_t seed);

extern uint8_t  crc8_tabletwo16   (uint8_t c, uint8_t seed);
extern uint16_t crc16_tabletwo16  (uint8_t c, uint16_t seed);
extern uint32_t crc32_tabletwo16  (uint8_t c, uint32_t seed);

extern uint8_t  crc8_table256_data8  (const uint8_t  *data, uint16_t length, uint8_t init);
extern uint8_t  crc8_table256_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint16_t crc16_table256_data8 (const uint8_t  *data, uint16_t length, uint16_t init);
extern uint16_t crc16_table256_data16(const uint16_t *data, uint16_t length, uint16_t init);
extern uint32_t crc32_table256_data8 (const uint8_t  *data, uint16_t length, uint32_t init);
extern uint32_t crc32_table256_data16(const uint16_t *data, uint16_t length, uint32_t init);

extern uint8_t  crc8_table16_data8  (const uint8_t  *data, uint16_t length, uint8_t init);
extern uint8_t  crc8_table16_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint16_t crc16_table16_data8 (const uint8_t  *data, uint16_t length, uint16_t init);
extern uint16_t crc16_table16_data16(const uint16_t *data, uint16_t length, uint16_t init);
extern uint32_t crc32_table16_data8 (const uint8_t  *data, uint16_t length, uint32_t init);
extern uint32_t crc32_table16_data16(const uint16_t *data, uint16_t length, uint32_t init);

extern uint8_t  crc8_tabletwo16_data8  (const uint8_t  *data, uint16_t length, uint8_t init);
extern uint8_t  crc8_tabletwo16_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint16_t crc16_tabletwo16_data8 (const uint8_t  *data, uint16_t length, uint16_t init);
extern uint16_t crc16_tabletwo16_data16(const uint16_t *data, uint16_t length, uint16_t init);
extern uint32_t crc32_tabletwo16_data8 (const uint8_t  *data, uint16_t length, uint32_t init);
extern uint32_t crc32_tabletwo16_data16(const uint16_t *data, uint16_t length, uint32_t init);

/* reflected variants */
extern uint8_t  crc8r  (uint8_t c, uint8_t seed);
extern uint16_t crc16r (uint8_t c, uint16_t seed);
extern uint32_t crc32r (uint8_t c, uint32_t seed);

extern uint8_t  crc8r_table256  (uint8_t c, uint8_t seed);
extern uint16_t crc16r_table256 (uint8_t c, uint16_t seed);
extern uint32_t crc32r_table256 (uint8_t c, uint32_t seed);

extern uint8_t  crc8r_table16   (uint8_t c, uint8_t seed);
extern uint16_t crc16r_table16  (uint8_t c, uint16_t seed);
extern uint32_t crc32r_table16  (uint8_t c, uint32_t seed);

extern uint8_t  crc8r_tabletwo16   (uint8_t c, uint8_t seed);
extern uint16_t crc16r_tabletwo16  (uint8_t c, uint16_t seed);
extern uint32_t crc32r_tabletwo16  (uint8_t c, uint32_t seed);

extern uint8_t  crc8r_table256_data8  (const uint8_t  *data, uint16_t length, uint8_t init);
extern uint8_t  crc8r_table256_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint16_t crc16r_table256_data8 (const uint8_t  *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_table256_data16(const uint16_t *data, uint16_t length, uint16_t init);
extern uint32_t crc32r_table256_data8 (const uint8_t  *data, uint16_t length, uint32_t init);
extern uint32_t crc32r_table256_data16(const uint16_t *data, uint16_t length, uint32_t init);

extern uint8_t  crc8r_table16_data8  (const uint8_t  *data, uint16_t length, uint8_t init);
extern uint8_t  crc8r_table16_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint16_t crc16r_table16_data8 (const uint8_t  *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_table16_data16(const uint16_t *data, uint16_t length, uint16_t init);
extern uint32_t crc32r_table16_data8 (const uint8_t  *data, uint16_t length, uint32_t init);
extern uint32_t crc32r_table16_data16(const uint16_t *data, uint16_t length, uint32_t init);

extern uint8_t  crc8r_tabletwo16_data8  (const uint8_t  *data, uint16_t length, uint8_t init);
extern uint8_t  crc8r_tabletwo16_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint16_t crc16r_tabletwo16_data8 (const uint8_t  *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_tabletwo16_data16(const uint16_t *data, uint16_t length, uint16_t init);
extern uint32_t crc32r_tabletwo16_data8 (const uint8_t  *data, uint16_t length, uint32_t init);
extern uint32_t crc32r_tabletwo16_data16(const uint16_t *data, uint16_t length, uint32_t init);

extern uint8_t crc8r_AB (uint8_t c, uint8_t seed);
extern uint8_t crc8r_AB_table16 (uint8_t c, uint8_t seed);
extern uint8_t crc8r_AB_table16_data8 (const uint8_t *data, uint16_t length, uint8_t init);
extern uint8_t crc8r_AB_table16_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint8_t crc8r_AB_tabletwo16 (uint8_t c, uint8_t seed);
extern uint8_t crc8r_AB_tabletwo16_data8 (const uint8_t *data, uint16_t length, uint8_t init);
extern uint8_t crc8r_AB_tabletwo16_data16 (const uint16_t *data, uint16_t length, uint8_t init);
extern uint8_t crc8r_AB_table256 (uint8_t c, uint8_t seed);
extern uint8_t crc8r_AB_table256_data8 (const uint8_t *data, uint16_t length, uint8_t init);
extern uint8_t crc8r_AB_table256_data16 (const uint16_t *data, uint16_t length, uint8_t init);

extern uint16_t crc16r_A001 (uint8_t c, uint16_t seed);
extern uint16_t crc16r_A001_table16 (uint8_t c, uint16_t seed);
extern uint16_t crc16r_A001_table16_data8 (const uint8_t *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_A001_table16_data16 (const uint16_t *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_A001_tabletwo16 (uint8_t c, uint16_t seed);
extern uint16_t crc16r_A001_tabletwo16_data8 (const uint8_t *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_A001_tabletwo16_data16 (const uint16_t *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_A001_table256 (uint8_t c, uint16_t seed);
extern uint16_t crc16r_A001_table256_data8 (const uint8_t *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_A001_table256_data16 (const uint16_t *data, uint16_t length, uint16_t init);

extern uint16_t crc16r_8408 (uint8_t c, uint16_t seed);
extern uint16_t crc16r_8408_table256 (uint8_t c, uint16_t seed);
extern uint16_t crc16r_8408_table256_data8 (const uint8_t *data, uint16_t length, uint16_t init);
extern uint16_t crc16r_8408_table256_data16 (const uint16_t *data, uint16_t length, uint16_t init);

extern uint32_t crc32r_EDB88320 (uint8_t c, uint32_t seed);
extern uint32_t crc32r_EDB88320_table256 (uint8_t c, uint32_t seed);
extern uint32_t crc32r_EDB88320_table256_data8 (const uint8_t *data, uint16_t length, uint32_t init);
extern uint32_t crc32r_EDB88320_table256_data16 (const uint16_t *data, uint16_t length, uint32_t init);

extern uint8_t crc8_D (uint8_t c, uint8_t seed);
extern uint8_t crc8_D1 (uint8_t c, uint8_t seed);

extern uint16_t crc16_1021 (uint8_t c, uint16_t seed);
extern uint16_t crc16_8005 (uint8_t c, uint16_t seed);


#endif /* MATHLIB_CRC_H_ */
