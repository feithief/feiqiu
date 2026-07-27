/**
 * @file main.c
 * @brief The application main routines.
 * @internal revision V1.1
 *
 * @copyright (C) 2018 Melexis N.V.
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
 * @ingroup application
 *
 * @details
 * This file contains the implementations of the application main routine.
 *
 * @history
 * 1.0 initial release
 * 1.1 added auto-addressing + EEPROM handling + bootloader functionality  + Melibu ROM functions
 * 1.2 support MLX81116AAC + sleep mode
 */
#include "system/SystemMain.h"

/** Application main loop
 *
 * The application main loop. Chip and peripherals are initialized and
 * background routines will be handled in endless loop.
 * @return 0
 */
int main(void)
{
  system_Init();

  while (1)
  {
    system_Main_Loop();
  }

  return 0;
}

