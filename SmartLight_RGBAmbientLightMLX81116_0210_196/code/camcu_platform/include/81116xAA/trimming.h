/**
 * @file
 * @brief ROM startup routines for the trimming
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git 5f2794fe
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

#ifndef TRIMMING_H
#define TRIMMING_H

#include <syslib.h>
#include <plib.h>
#include "sys_tools.h"
#include "eeprom_map.h"

/** Compile-time Wait-states and R1,R2,R2 EE and FL settings to be loaded before everything else */
#define ROM_R1_EE      (0x5366u)
#define ROM_R2_EE      (0x4000u)
#define ROM_WS_EE      (0x0Fu)
#define ROM_WS_FL      (0x07u)

/** EEPROM-based configuration description type */
typedef union RC_Settings_t {
    struct __attribute__((packed)) {
        uint16_t RCO_OSC : 10;              /**< 10 bits of data represent the RCO_OSC trimming value for certain frequency */
        uint16_t EE_WaitStatesP : 2;        /**< The Base EEPROM WaitStates value for this frequency */
        uint16_t FL_WaitStatesP : 2;        /**< The Base FLASH WaitStates value for this frequency */
        uint16_t FL_AcSelWaitOption : 1;    /**< Defines whether the FLASH Waitstates will be decreased by 1 in case of AcSel=1 option */
        uint16_t EE_AcSelWaitOption : 1;    /**< Defines whether the EEPROM Waitstates will be decreased by 1 in case of AcSel=1 option */
    };
    uint16_t u;
} RC_Settings_t;

/*
   Set of functions to set-up the trimming and IC's wait-states
 */
void TrimAnalog(void);
void SetWaitStates(waitstate_cfg_t trim_cfg);
void SetRomMemoryFlags(void);
void SetSystemSpeed(const RC_Settings_t trim_cfg, const uint8_t AcSel);

/** SetRcoAndAcSel sets the RCO32M trimming and AcSel bit in one scope
 * @param[in] rco takes the value for the RCO trimming. 10bits value is used.
 * @param[in] AcSel defined whether the divider by 2 will be used for the MCU clock. 1- the divider is used.
 */

INLINE void SetRcoAndAcSel(uint16_t rco, uint8_t AcSel)
{
    IO_SET(TRIM_RCO32M, TR_RCO32M_IN, rco);
    IO_SET(PORT_CLOCK_CTRL, AC_SEL, AcSel); /* Set the half-freq flag as it set */
}

#endif /* TRIMMING_H */

