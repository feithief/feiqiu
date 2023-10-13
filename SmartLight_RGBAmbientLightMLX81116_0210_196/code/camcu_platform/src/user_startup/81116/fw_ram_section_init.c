/**
 * @file
 * @brief The RAM section initialization routines.
 * @internal
 *
 * @copyright (C) 2018 Melexis N.V.
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
 * @addtogroup fw_user_startup
 *
 * @details This file contains the implementations of the RAM section initialization routine.
 */

#include <stdint.h>
#include <bl_tools.h>
#include "fw_startup.h"


/*-----------------------------------------------------------------------------
 * Linker symbols (these objects are not created in the memory)
 */

extern uint16_t _bss_dp_start;
extern uint16_t _bss_dp_end;

extern uint16_t _bss_start;
extern uint16_t _bss_end;

extern uint16_t _data_load_start;

extern uint16_t _data_dp_start;
extern uint16_t _data_dp_end;

extern uint16_t _data_start;
extern uint16_t _data_end;

/*-----------------------------------------------------------------------------
 * Functions
 */

/**
 * C runtime initialization. Initializes .bss and .data RAM sections.
 * @note Function should NOT be static and/or inlined otherwise space
 * on stack for local variables r and w could be allocated before calling
 * SET_STACK() in start().
 */
void fw_ram_section_init(void)
{
    uint16_t *w;
#if !defined (RAM_APPLICATION)
    uint16_t *r;
#endif /* !RAM_APPLICATION */

    /* clear .bss and .dp.bss sections */
    for (w = &_bss_dp_start; w < &_bss_dp_end; ) {
        *w++ = 0;
    }

    for (w = &_bss_start; w < &_bss_end; ) {
        *w++ = 0;
    }

#if !defined (RAM_APPLICATION)  /* skip ROM-to-RAM loading for RAM application */
    /* initialize .data and .dp.data sections */
    r = &_data_load_start;      /* copy data from rom */

    for (w = &_data_dp_start; w < &_data_dp_end; ) {
        *w++ = *r++;
    }

    for (w = &_data_start; w < &_data_end; ) {
        *w++ = *r++;
    }
#endif /* !RAM_APPLICATION */

}

/* EOF */
