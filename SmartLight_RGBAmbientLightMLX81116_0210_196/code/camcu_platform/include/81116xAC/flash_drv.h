/**
 * @file
 * @brief FLASH write support and initialization functions library
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git 6d04ab96
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
 * FLASH driver. Used to manage the dedicated FLASH type.
 */

#ifndef FLASH_DRV_H
#define FLASH_DRV_H

#include "flash_defines.h"

typedef enum {
    FL_BIST_FAILED = 0,    /* Occurs when the Flash BIST has failed */
    FL_BIST_OK             /* The BIST is OK */
} fd_FlashBistRes_t;


void FLASH_TurnOn(void);                                                                         /* Enables the FLASH shell and waits a state */
void FLASH_Init(void);                                                                           /* Initiates the FLASH with timings  */

#if (FL_EE_HAS_WRITE_FUNC!=0u)
FLASH_Result_t FLASH_Write( void );
#endif /* FL_EE_HAS_WRITE_FUNC */

extern fd_FlashBistRes_t FLASH_BistTest(uint16_t pages_number);
extern void FLASH_BistPrepare(uint16_t pages_number);
extern uint16_t FLASH_BistRun(uint16_t pages_number);
extern void FLASH_SetR3(uint16_t NewValue);

extern void FLASH_eventHandler(void);

/* Legacy function declaration. The old name is FLASH_BistTest(). */
extern fd_FlashBistRes_t FLASH_BistTestAuto(void);

#endif /* FLASH_DRV_H */

/* EOF */
