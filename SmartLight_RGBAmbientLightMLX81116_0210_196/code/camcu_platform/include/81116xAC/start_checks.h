/**
 * @file
 * @brief ROM, RAM, EEPROM memory startup checks library
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
 * @ingroup Startup
 *
 * @details
 */

#ifndef START_CHECKS_H
#define START_CHECKS_H

#include <syslib.h>
#include <plib.h>
#include "sys_tools.h"

void HAL_BIST_romTest(void);
void HAL_BIST_ramTest(void);
void CheckEEPROM(void);
#ifndef UNITTEST
void _fatal (void) __attribute__((noreturn));
#else
void _fatal (void);
void Clear_EE_SingleBitErrorFlags(void);
#endif


#endif /* START_CHECKS_H */

