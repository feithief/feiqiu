/**
 * @file
 * @brief Simple Timer hardware support library
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

#ifndef STIMERLIB_H_
#define STIMERLIB_H_

#include <stdint.h>
#include "compiler_abstraction.h"
#include "io.h"

/** Simple timer clock's mode definitions */
typedef enum {
    STIMER_DISABLE_CLOCK = 0u,      /**< Simple timer is turned off */
    STIMER_CPU_CLOCK = 1u,          /**< Simple timer counts on the CPU clocks frequency */
    STIMER_1US_CLOCK = 2u,          /**< Simple timer runs on 1us period */
    STIMER_100US_CLOCK = 3u,        /**< Simple timer runs on 100us period */
} STimerClk_t;


#define _STIMER_SET_MODE(U, ST_MODE) \
    IO_SET(U, MODE, ST_MODE)

#define _STIMER_SET_VALUE(U, ST_VALUE) \
    IO_SET(U, VALUE, ST_VALUE)

#define _STIMER_INIT(U, ST_MODE, ST_VALUE) \
    do { _STIMER_SET_MODE(U, ST_MODE);   \
         _STIMER_SET_VALUE(U, ST_VALUE); } while (0)

#define _STIMER_GET_CURRENT(U) \
    IO_GET(U, CURRENT)


/** STIMER_SET_MODE sets the simple timer's mode
 * @param[in] mode new Simple Timer's mode
 */
STATIC INLINE void STIMER_SET_MODE( STimerClk_t mode )
{
    _STIMER_SET_MODE(STIMER, mode);
}

/** STIMER_SET_VALUE sets the simple timer's mode
 * @param[in] value is new 14-bit timer value
 */
STATIC INLINE void STIMER_SET_VALUE( uint16_t value )
{
    _STIMER_SET_VALUE(STIMER, value);
}

/** STIMER_INIT sets the simple timer's mode and value
 * @param[in] mode new Simple Timer's mode
 * @param[in] value represents the period value
 */
STATIC INLINE void STIMER_INIT( STimerClk_t mode, uint16_t value )
{
    _STIMER_INIT(STIMER, mode, value);
}

/** STIMER_GET_CURRENT Returns the current timer's value
 * @return 14bit timer's counter value
 */
STATIC INLINE uint16_t STIMER_GET_CURRENT(void)
{
    return _STIMER_GET_CURRENT(STIMER);
}

#define _STIMER_INT_DISABLE( unit )     IO_SET(MLX16, unit ## _ITC, 0U)
#define _STIMER_INT_ENABLE( unit )         \
    do  {   IO_SET(MLX16, unit ## _PEND, 1u); \
            IO_SET(MLX16, unit ## _ITC, 1u); } while(0u)

/** STIMER_INT_DISABLE Disables the simple timer's interrupt */
STATIC INLINE void STIMER_INT_DISABLE(void)
{
    _STIMER_INT_DISABLE( STIMER );
}

/** STIMER_INT_ENABLE Enables the simple timer's interrupt */
STATIC INLINE void STIMER_INT_ENABLE(void)
{
    _STIMER_INT_ENABLE( STIMER );
}

#endif /* STIMERLIB_H_ */
