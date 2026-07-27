/**
 * @file
 * @brief Provides a Software Interrupt trigger
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

#ifndef ITC_H_
#define ITC_H_

#include <stdbool.h>
#include "io.h"
#include <compiler_abstraction.h>
#include "itc_helper.h"

/* API */
/* === Implementation ====================================================== */
/** Request a software interrupt */
static INLINE
void Itc_SwIntRequest (void)
{
    IO_SET(MLX16, SWI, 1);
}

#endif /* ITC_H_  */
