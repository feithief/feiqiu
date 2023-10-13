/**
 * @file
 * @brief Amalthea Software Platform
 * @internal
 *
 * @copyright (C) 2015-2016 Melexis N.V.
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
 * @ingroup amalthea_platform
 *
 * @details
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include "compiler_abstraction.h"

/**
    Delay for specified number of 'nop' instructions

    @param nops [3..65535]  A number of 'nop' instructions for delay.

    @note
    The minimum value for the @p nops parameter is 3!
 */
static INLINE
void delay_nops (uint16_t nops)
{
    __asm__ __volatile__ (
        "add %[cnt], #-2 \n\t"  /* '-2' to compensate X register loading and 'add X, #-2' instruction */
        "djnz %[cnt], . \n\t"
        :
        : [cnt] "x" (nops)
    );
}

#endif /* DELAY_H_ */
