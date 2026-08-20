/**
 *****************************************************************************
 * @brief   utilities header file.
 *
 * @file    utilities.h
 * @author  AE/FAE team
 * @date    2024.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX2_VALUE_GET(x, y) ((x > y) ? x : y)

#define MIN2_VALUE_GET(x, y) ((x < y) ? x : y)

#define CLAMP_VALUE_GET(x, min, max)    ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define MAX3_VALUE_GET(x, y, z)         (MAX2_VALUE_GET(MAX2_VALUE_GET(x, y), z))

#define MIN3_VALUE_GET(x, y, z)         (MIN2_VALUE_GET(MIN2_VALUE_GET(x, y), z))

#define DIV_FIXED_POINT(x, y)           (((x)+((y)>>1)) / (y)) /* 定点除法 */

#define BIT32_VALUE_GET(x, y)           (((uint32_t)(x) >> (y)) & 0x01)
#define BIT32_VALUE_SET(x,y)            ((x) |= (uint32_t) (1UL << (y)))
#define BIT32_VALUE_CLR(x,y)            ((x) &= ((uint32_t) (~(1UL << (y)))))

#define BIT64_VALUE_GET(x, y)           (((uint64_t)(x) >> (y)) & 0x01)
#define BIT64_VALUE_SET(x,y)            ((x) |= (uint64_t) (1ULL << (y)))
#define BIT64_VALUE_CLR(x,y)            ((x) &= ((uint64_t) (~(1ULL << (y)))))

typedef enum
{
    PARITY_CALIB_MAKE,    /**< make parity bits */
    PARITY_CALIB_CHECK,   /**< check parity bits */
} parity_calib_type_e;

int16_t average_calculate_utils(int16_t *data, uint16_t length);
uint16_t crc16_calculate_func(uint16_t crc, const uint8_t *data, uint16_t len);
uint32_t crc32_calculate_func(uint32_t crc, const uint8_t *data, uint32_t len);
uint8_t checksum_calculate_func(uint8_t init_sum, const uint8_t *data, uint16_t length);
uint32_t endian_swap_func(uint8_t *data, uint16_t length);
void bit_invert_swap_func(void *bit_data, uint8_t bit_length);
uint8_t int_to_ascii_func(int32_t num, uint8_t *str);
uint8_t parity_calib_func(parity_calib_type_e type, uint16_t pid);
#ifdef __cplusplus
}
#endif

#endif /* __UTILITIES_H__ */
