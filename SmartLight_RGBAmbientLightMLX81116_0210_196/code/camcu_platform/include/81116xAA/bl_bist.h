/**
 * @file
 * @brief Reset BIST codes
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

#ifndef BL_BIST_H
#define BL_BIST_H

#include <syslib.h>
#include <io.h>
#include <lib_bist_config.h>

#define C_CHIP_CODE(x, y, z) \
    ( (uint16_t)( (((uint16_t)(x)-(uint16_t)'@')<<10) | (((uint16_t)(y)-(uint16_t)'@')<<5) | \
                  ((uint16_t)(z)-(uint16_t)'@') ) )

#define C_CHIP_HEADER                       (0x8000U | C_CHIP_CODE('M','L','X')) /** Defines a 16-bit value as a KEY in RAM, represented as a 'useful data' */
#define C_CHIP_STATE_COLD_START             0x0000U
#define C_CHIP_STATE_WATCHDOG_RESET         C_CHIP_CODE('W','D','R')    /** Watchdog reset (Warm Start) */
#define C_CHIP_STATE_FATAL_RECOVER_ENA      C_CHIP_CODE('F','R','E')    /** Fatal Recovery Enabled */
#define C_CHIP_STATE_FATAL_CRASH_RECOVERY   C_CHIP_CODE('F','C','R')    /** Fatal Crash Recovery */
#define C_CHIP_STATE_LIN_LDR_CMD_RESET      C_CHIP_CODE('L','C','R')    /** LIN Loader Command Reset */
#define C_CHIP_STATE_LIN_LDR_PROG_RESET     C_CHIP_CODE('L','P','R')    /** LIN Loader Enter Programming mode Reset */
#define C_CHIP_STATE_LIN_CMD_EPM            C_CHIP_CODE('L','C','E')    /** LIN Command (chip) Enter-PPM-Mode reset */

#ifndef UNITTEST
#define BIST_DATA_OFFSET 0x17F8
#else
#define BIST_DATA_OFFSET 0x37F8
#endif /* UNITTEST */

/** bistHeader specifies the constant signature for the data in RAM. This sign is used to validate the data below */
extern volatile uint16_t bistHeader    __attribute__((addr(BIST_DATA_OFFSET+0x00u)));
/** bistResetInfo specifies the command for the bootloader to perform the Warm reset, Cold reset or Enter programming-mode */
extern volatile uint16_t bistResetInfo __attribute__((addr(BIST_DATA_OFFSET+0x02u)));
/** bistError reserved for the application. */
extern volatile uint16_t bistError     __attribute__((addr(BIST_DATA_OFFSET+0x04u)));
/** bistErrorInfo reserved for the application. */
extern volatile uint16_t bistErrorInfo __attribute__((addr(BIST_DATA_OFFSET+0x06u)));

#endif
