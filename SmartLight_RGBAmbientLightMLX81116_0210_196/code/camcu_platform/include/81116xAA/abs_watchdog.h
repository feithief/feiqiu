/**
 * @file
 * @brief Brief description of file for doxygen
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
 * @ingroup module_name
 *
 * @details
 */

#ifndef ABS_WATCHDOG_H
#define ABS_WATCHDOG_H

#include <stdint.h>
#include <stdbool.h>

#include <io.h>
#include <static_assert.h>
#include <compiler_abstraction.h>

/* API */
STATIC INLINE void AbsWatchdog_Restart (void);
STATIC INLINE bool AbsWatchdog_IsWindowOpen (void);


/* === Implementation ========================================================= */
#ifndef UNITTEST
/**
    Restart (acknowledge) absolute watchdog
 */
STATIC INLINE void AbsWatchdog_Restart (void)
{
    IO_SET(AWD, ACK, 1);    /* shall be compiled into bit-access instruction 'setb io:XX.n' */
}


/**
    Return true if absolute watchdog window is open

    @return Return true if absolute watchdog window is open
 */
STATIC INLINE bool AbsWatchdog_IsWindowOpen (void)
{
#if 1
    return IO_GET(AWD, WIN_OPEN);   /* check WIN_OPEN using bit access ('mov C, io:XX.n' provided by IO_GET) */
#else
    return (IO_HOST(AWD, WIN_OPEN) & IO_MASK(AWD, WIN_OPEN)) != 0;  /* check WIN_OPEN using word access (word-read is not implemented in HW) */
#endif

}
#endif /* UNITTEST */

#endif /* ABS_WATCHDOG_H */
