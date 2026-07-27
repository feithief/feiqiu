/**
 * @file
 * @brief The flash firmware startup definitions.
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
 * @details This file contains the definitions of the flash firmware startup module.
 */

#ifndef FW_STARTUP_H_
#define FW_STARTUP_H_


/*-----------------------------------------------------------------------------
 * Function prototypes
 */

extern void fw_start(void);
extern void _fatal(void);
extern void fw_ram_section_init(void);
extern void fw_premain(void);

#endif /* FW_STARTUP_H_ */
