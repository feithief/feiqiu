/**
 * @file
 * @brief Describes the global variables and fields in FLASH and RAM to be used during the startup code and interrupts handling
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

#ifndef BL_DEFS_H
#define BL_DEFS_H

/*
 * User functions prototypes
 * These function shall be defined in user application. And they will be called from the function pointers' table
 */


#ifndef BL_PREFIX
#define BL_PREFIX                   /** Defines the prefix for each bootloader function */
#endif

#include <stdint.h>
#include "version.h"
#include "memory_map.h"

/** Possible values for ml_driver_mode */
typedef enum {
    blm_AppMode    = 0x00U,     /**< Application mode sign. */
    blm_LoaderMode = 0x58U,     /**< Bootloader mode sign. Should differ from 0x55 or 0xAA which can be left after the RAM BIST */
    blm_EmptyFlashMode = 0xA7U  /**< Empty FLASH mode sign. Should differ from 0x55 or 0xAA which can be left after the RAM BIST */
} bl_Mode_t;

/** Simple function type */
typedef void (*main_t)(void);

/**
 * Firmware KEYS and definitions
 *
 **/
//#define CAMCU_VERSION_32  ((uint32_t)0x01010100ul)  /* Incremental 32bit value to define the ROM's revision */

/** FW_FLASH_KEY specifies the FW SIGN value offset within the FLASH content */
extern uint16_t FW_FLASH_KEY __attribute__((addr(MEM_FLASH_START + 0x0006u)));
/** FW_FLASH_BIST_PAGES_COUNT specifies the FLASH pages count to be tested by the FLASH BIST during the startup */
extern uint16_t FW_FLASH_BIST_PAGES_COUNT __attribute__((addr(MEM_FLASH_START + 0x0026u))); /* The latest word in the OPERR handler shows how much FLASH pages should be used for the FLASH signature check */
/** FW_FAST_CHECK the condition specifying the FW SIGN is present in the FLASH or not. Allows to quickly identify the FLASH content. */
#define FW_FAST_CHECK()      (FW_FLASH_KEY == (FLASH_FW_PATTERN ^ PRODUCT_VERSION_16))

void bl_start(void);

#endif

