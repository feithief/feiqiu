/**
 * @file
 * @brief The flash firmware premain routines.
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
 * @details This file contains the implementations of the flash firmware premain routine.
 */

#include <stdint.h>
#include <atomic.h>
#include <trimming.h>
#include <mem_checks.h>
#include <eeprom_map.h>
#include "fw_startup.h"

/*-----------------------------------------------------------------------------
 * Macro definitions
 */

/*-----------------------------------------------------------------------------
 * Function prototypes
 */

__attribute__((weak)) void fw_low_level_init(void);
STATIC INLINE void set_Clock_Speed(void);


/*-----------------------------------------------------------------------------
 * Functions
 */

/** Application specific premain routines.
 * Initiates clocks, wait_states to it's optimal states for the application.
 * @note Current default implementation is empty and can be overwritten by application implementation.
 */
void fw_low_level_init(void)
{
    /* trim area */
    if(nvram_CalcCRC((uint16_t *)EE_TRIM_AREA_START, EE_TRIM_AREA_SIZE>>1) == NVRAM_CORRECT_CRC) {
        IO_HOST(TRIM1_LC, LC0_TRIM)=(EE_APP_TRIM23 & 0x0FFF);
        IO_HOST(TRIM2_LC, LC2_TRIM)=(EE_APP_TRIM24 & 0x0FFF);
        IO_HOST(TRIM3_LC, LC4_TRIM)=(EE_APP_TRIM25 & 0x0FFF);
        IO_HOST(TRIM4_LC, LC6_TRIM)=(EE_APP_TRIM26 & 0x0FFF);
        IO_HOST(TRIM5_LC, LC8_TRIM)=(EE_APP_TRIM27 & 0x0FFF);
        IO_HOST(TRIM6_LC, LC10_TRIM)=(EE_APP_TRIM28 & 0x0FFF);
#if defined (__MLX81116xAA__)
        IO_HOST(TRIM_MISC, TR_TAU)=(EE_APP_TRIM29 & 0x1FFF);
#else
        IO_HOST(TRIM_MELIBUS, TR_TAU)=(EE_APP_TRIM22 & 0x1FFF);
        IO_HOST(TRIM_MISC, UNUSED)=(EE_APP_TRIM29 & 0x1FFF);
#endif
    } else {
        /* load default values */
    }
}

/** Set the PLL clock speed according the user configuration
 * This function will configure the PLL clock speed according to the user
 * selected configuration as in Makefile.configure.mk. Six different values
 * are supported as these are the values for which we have trimming data in
 * the eeprom calibration section.
 */
STATIC INLINE void set_Clock_Speed(void)
{
    RC_Settings_t tmp;
    uint8_t ac_sel = 0u;

#if FPLL == 12000
    tmp.u = EE_MS_TRIM6_VALUE;
    ac_sel = 1u;
#elif FPLL == 14000
    tmp.u = EE_MS_TRIM7_VALUE;
    ac_sel = 1u;
#elif FPLL == 16000
    tmp.u = EE_MS_TRIM8_VALUE;
    ac_sel = 1u;
#elif FPLL == 24000
    tmp.u = EE_MS_TRIM6_VALUE;
    ac_sel = 0u;
#elif FPLL == 28000
    tmp.u = EE_MS_TRIM7_VALUE;
    ac_sel = 0u;
#elif FPLL == 32000
    tmp.u = EE_MS_TRIM8_VALUE;
    ac_sel = 0u;
#else
    #warning "Clock Speed not valid, use 32MHz"
#endif

    SetSystemSpeed(tmp, ac_sel);
}

/**
 * Function premain() is invoked by start-up code just before calling main().
 * At this point all runtime-initialization is done thus variables/objects
 * get their initial values.
 */
void fw_premain(void)
{
    fw_low_level_init();            /* optionally call more low level initialization */

    if (IO_GET(PORT_MISC_IN, RSTAT) == 0u)
    {
        /* this is a cold start */
        fw_ram_section_init();      /* RAM initialization */
    }

    set_Clock_Speed();              /* set clock speed */
    builtin_mlx16_set_priority(7);  /* system mode, the lowest priority: 7 */
}

/* EOF */