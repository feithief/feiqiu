/**
 * @file
 * @brief Version information for each project
 * @internal
 *
 * @copyright (C) 2017 Melexis N.V.
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

#ifndef VERSION_H
#define VERSION_H

#ifndef __ASSEMBLER__
#include <stdint.h>
#include "eeprom_map.h"

/** Special unique pattern to define the product's version and HW revision.*/
#define PRODUCT_VERSION_16   EE_CHIP_ID_3
/** 32bit value uses only 16 LSB for the PRODUCT VERSION */
#define PRODUCT_VERSION_32   ((uint32_t)PRODUCT_VERSION_16)

/** Flash FW PATTERM used to code the expected FLASH FW KEY */
#define FLASH_FW_PATTERN     ((uint16_t)0xA55A)
/** Flash SW SIGN KEY is used as a 16bit key for FW in FLASH */
#define FLASH_SW_SIGNATURE   (FLASH_FW_PATTERN ^ PRODUCT_VERSION_16)

volatile const uint8_t * VERSION_getFwPltfVersionPtr(void);

#else /* __ASSEMBLER__ */

#endif /* __ASSEMBLER__ */

#endif /* VERSION_H */

