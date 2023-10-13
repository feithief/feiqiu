/**
 * @file
 * @brief Absolute and intelligent watchdogs HW support library
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
 * Absolute and an intelligent watchdog timers support library
 */

#ifndef LIB_WDG_H
#define LIB_WDG_H

#include <stdint.h>
#include <syslib.h>

#include "io.h"

/* -------------- PUBLIC FUNCTIONS  -------------- */

/** Activates the Intelligent watchdog attention interrupt
 * @param[in] cDIV Iwd divider, [0..6]
 * @param[in] cWDT Iwd time window, [0..255]
 *
 * @note IWDT Period is (MCU_CLK)*2^(5+2*cDIV)
 * The ensemble {cDIV, cWDT} creates a timeout budget, equal to cWDT*Period (MCU_CLK)*2^(5+2*cDIV).
 */
STATIC INLINE void WDG_activateIwd(uint16_t cDIV, uint16_t cWDT)
{
    IO_SET(IWD, WIN_ENABLE, 1, WTG, 0u);
    IO_SET(IWD, DIV, cDIV, WDT, cWDT);
}

/** WDG_conditionalIwdRefresh Conditionally acknowledges the IWD
 *
 * @param cDIV Iwd divider, [0..6]
 * @param cWDT Iwd time window, [0..255]

 * @note IWDT Period is (MCU_CLK)*2^(5+2*cDIV)
 * The ensemble {cDIV, cWDT} creates a timeout budget, equal to cWDT*Period (MCU_CLK)*2^(5+2*cDIV).
 */
STATIC INLINE void WDG_conditionalIwdRefresh(uint16_t cDIV, uint16_t cWDT)
{
    if (IO_GET(IWD, WIN_OPEN)) {
        IO_SET(IWD, WTG, 0u);
        IO_SET(IWD, DIV, cDIV, WDT, cWDT);
    }
}

/** WDG_enableIwdIt enables the intelligent watchdog timer interrupt */
STATIC INLINE void WDG_enableIwdIt(void)
{
    /* enable IWD interrupt */
    IO_SET(MLX16, IWD_ATT_ITC, 1u);
}

/** WDG_disableIwdIt disables the intelligent watchdog timer interrupt */
STATIC INLINE void WDG_disableIwdIt(void)
{
    /* disable IWD interrupt */
    IO_SET(MLX16, IWD_ATT_ITC, 0u);
}

/** WDG_enableIwdWindow Enables the IWD windowed mode
 *
 * @note In the windowed mode the IWDG can be acknowledged only when its window is opened
 */
STATIC INLINE void WDG_enableIwdWindow(void)
{
    /* enable IWD interrupt */
    IO_SET(IWD, WIN_ENABLE, 1);
}

/** WDG_disableIwdWindow Disables the IWD windowed mode
 *
 * @note In the windowed mode the IWDG can be acknowledged only when its window is opened
 */
STATIC INLINE void WDG_disableIwdWindow(void)
{
    /* enable IWD interrupt */
    IO_SET(IWD, WIN_ENABLE, 0u);
}

/** WDG_conditionalAwdRefresh acknowledges the AWD timer if its window is opened */
STATIC INLINE void WDG_conditionalAwdRefresh(void)
{
    if (0 != IO_GET(AWD, WIN_OPEN)) {
        IO_SET(AWD, ACK, 1);
    }
}

/** Legacy definition, used in Mulan2/Mulan3 projects */
#define WDG_Manager() WDG_conditionalAwdRefresh()

#endif /* LIB_WDG_H */
