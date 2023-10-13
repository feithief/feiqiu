/**
 * @file
 * @brief Provides a set of functions used for the bootloaders and can be used in the Software Application in FLASH
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git 6d04ab96
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

#ifndef BIST_H
#define BIST_H

#include "syslib.h"
#include "compiler_abstraction.h"
#include "bl_defs.h"
#include "io.h"
#include "sys_tools.h"
#include "memory_map.h"

#define ROM_BIST_CFG_SEED             (0x000001UL)                  /** An initial SEED for the FLASH and ROM BIST checks */
#define cROM_BIST_KEY                 (0x11EBU)                     /** The HW BIST KEY used to run the BIST */


STATIC INLINE void _bist_set_and_run(uint32_t from, uint32_t to, uint8_t ecc_pos)
{
    IO_SET(ROM_BIST, ECC_POSITION, ecc_pos,
           BIST,            0u,
           SINGLE_RAMP,     1u,
           MASK_SIG_ERR,    0u);

    IO_SET(ROM_BIST, ADD_START_L,     (uint16_t)from);
    IO_SET(ROM_BIST, ADD_START_H,     (uint16_t)(from>>16));


    /* Safety concept revision: SW test, ROM BIST and other bandwidth consuming tests requires another strategy */
    IO_SET(ROM_BIST, ADD_STOP_L,       (uint16_t)to);
    IO_SET(ROM_BIST, ADD_STOP_H,       (uint16_t)(to>>16));

    IO_SET(ROM_BIST, SIG_EXPECTED_L, 0x0u);
    IO_SET(ROM_BIST, SIG_EXPECTED_H, 0x0u);

    IO_SET(ROM_BIST, SIG_RECEIVED_L, (uint16_t)ROM_BIST_CFG_SEED);
    IO_SET(ROM_BIST, SIG_RECEIVED_H, (uint16_t)(ROM_BIST_CFG_SEED>>16));
    /* Run the ROM bist */
    IO_SET(ROM_BIST, START_BIST, cROM_BIST_KEY);
    /* CPU will stop here until the BIST's finishes his job. */

    NOP(); /* Inserted to let the HW BIST to not to skip the signature check in FX */
    NOP();
}

/** RomBistTest starts the HW BIST-check for the ROM and compares the signature with the expected.
 * @param[in] from start data pointer (should be 32bit aligned).
 * @param[in] to end data pointer (should be 32bit aligned).
 * @param[in] signature the signature expected.
 * @retval 0 the check has failed
 * @retval 1 the test's passed.
 */
STATIC INLINE uint8_t RomBistTest(uint32_t from, uint32_t to, uint32_t signature)
{
    _bist_set_and_run(from, to, 3u); /* ECC position is absent for the ROM */
    if ( (IO_GET(ROM_BIST, SIG_RECEIVED_L) != ((uint16_t)signature) ) ||
         (IO_GET(ROM_BIST, SIG_RECEIVED_H) != ((uint16_t)(signature>>16))) ) {
        return 0u;
    } else {
        return 1u;
    }
}



/** FlashBistTest starts the HW BIST-check for the FLASH and compares the signature with the expected.
 * @param[in] from start data pointer (should be 32bit aligned).
 * @param[in] to end data pointer (should be 32bit aligned).
 * @param[in] signature the signature expected.
 * @retval 0 the check has failed
 * @retval 1 the test's passed.
 */
STATIC INLINE uint8_t FlashBistTest(uint32_t from, uint32_t to, uint32_t signature)
{
    _bist_set_and_run(from, to, 2); /* ECC position is 0b10 for 64bit FLASH */
    if ( (IO_GET(ROM_BIST, SIG_RECEIVED_L) != ((uint16_t)signature) ) ||
         (IO_GET(ROM_BIST, SIG_RECEIVED_H) != ((uint16_t)(signature>>16))) ) {
        return 0u;
    } else {
        return 1u;
    }
}

/** BL_BistFlashPages() FLASH page number for HW BIST check
 * returns the FW configuration - how much pages FLASH should be checked during the BIST check, starting from the page 0
 * @return number of FLASH pages
 */
INLINE uint16_t BL_BistFlashPages(void)
{
    if (FW_FAST_CHECK()!=0u) {
        return FW_FLASH_BIST_PAGES_COUNT;
    } else {
        return MEM_FLASH_SIZE/MEM_FLASH_PAGE_SIZE;
    }
}

#endif /* BIST_H */
