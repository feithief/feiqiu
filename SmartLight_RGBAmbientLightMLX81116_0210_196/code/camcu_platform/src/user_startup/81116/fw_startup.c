/**
 * @file
 * @brief The flash firmware startup routines.
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
 * @details This file contains the implementations of the flash firmware startup routine.
 */

#include <stdint.h>
#include <sys_tools.h>
#include <memory_map.h>
#include "fw_startup.h"


/*-----------------------------------------------------------------------------
 * Linker symbols (these objects are not created in the memory)
 */

extern uint16_t fw_stack;


/*-----------------------------------------------------------------------------
 * Externals
 */

extern int main(void);


/*-----------------------------------------------------------------------------
 * Functions
 */

/** Start function
 * The function defines stack, executes low level initialization of the hardware,
 * C-runtime initialization and executes main function of the application
 *
 * \note:
 *  1. This function is called by Reset vector. System mode of the COU with highest
 *     priority (mov UPR, #0) is set before calling start()
 *
 *  2. Function directly changes the CPU stack pointer, thus creating any local
 *     variables/objects inside this function is FORBIDDEN!
 */
void fw_start(void)
{
    /* Need to run the RAM-test before STACK init */
    SET_STACK(&fw_stack);
    SYS_clearCpuStatus();   /* Initialize M register.
                             * Note that UPR register (== M[11:8]) was already
                             * initialized during execution of the reset vector
                             * (see JMPVECTOR macro)
                             */

    fw_premain();           /* Don't make this functions static to be sure its local variable will not apear before the SP assignment */

    __asm__ ("JMP _main\n\t" ::); /* jump to the main function */
}

/* EOF */
