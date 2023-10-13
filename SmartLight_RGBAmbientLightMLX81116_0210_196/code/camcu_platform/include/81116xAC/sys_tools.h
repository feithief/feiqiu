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

#ifndef SYS_TOOLS_H
#define SYS_TOOLS_H

#if 0
#define INSTRSIM_TST /* TODO: should be disabled for the output !!! */
#warning INSTRSIM_TST is ENABLED!!!
#endif

#include "io.h"
#include <stdint.h>
#include "compiler_abstraction.h"
#include "atomic.h"
#include <static_assert.h>

#ifdef FPGA_DEBUG_OUTPUT
#include "dbg_tools.h"
#endif

#define COMPILE_CONFIG_ID_81200             (1u)
#define COMPILE_CONFIG_ID_81116             (2u)
#define COMPILE_CONFIG_ID_81330             (3u)
#define COMPILE_CONFIG_ID_81113             (4u)
#define COMPILE_CONFIG_ID_81130             (5u)
#define COMPILE_CONFIG_ID_81332             (6u)
#define COMPILE_CONFIG_ID_81113_LIN         (7u)
#define COMPILE_CONFIG_ID_81114             (8u)
#define COMPILE_CONFIG_ID_81344             (9u)
#define COMPILE_CONFIG_ID_MAX               (COMPILE_CONFIG_ID_81344)

ASSERT(COMPILE_CONFIG_ID <= COMPILE_CONFIG_ID_MAX);

/* ----------------------------------------------------------------------------
 * MLX16 helper functions
 *
 * NOTE: stack - should be in a register
 */

#ifndef UNITTEST
#define SET_STACK(stack)        \
    do  {                           \
        __asm__ __volatile__ (      \
            "mov S, %[oper]"        \
            :                       \
            : [oper] "r" (stack)    \
            : "S"                   \
            );                          \
    }   while(0)

#else
void SET_STACK(uint16_t* address);
#endif

#ifndef UNITTEST
/*
 * Clears M register
 */
STATIC INLINE void SYS_clearCpuStatus (void)
{
    __asm__ __volatile__ (
        "mov A, #0" "\n\t"
        "mov M, A"  "\n\t"
        :
        :
        : "A"
        );
}
#else
void SYS_clearCpuStatus (void);
#endif

/* section in the first half of the flash */
#define __MLX_TEXT__ __attribute__((section(".mlx_text")))

/* section in the ROM */
#define __ROM_MLX_TEXT__  __attribute__((section(".rom_mlx_text")))

/* section in the MLX4 "RESERVED" ROM */
#define __ROM_MLX_TEXT_RESERVE__ __attribute__((section(".rom_reserve")))

/* ram fixed data for CAMCU data purposes */
#define __RAM_CAMCU__ __attribute__((section(".ram_bss_loader")))


/* ----------------------------------------------------------------------------
 * Blocking delays
 *
 */
#define DELAY(loops)            \
    __asm__ __volatile__ (      \
        "mov  X, %[cnt]\n\t"    \
        "djnz X,."              \
        :                       \
        : [cnt] "ri" (loops)    \
        : "X"                   \
        )


/*-----------------------------------------------------------------------------
 * Delays for 'us' microseconds
 */
#define DELAY_US(us)    DELAY((FPLL * (uint32_t)(us) + 2000u) / 4000u)


/*
 * Blocking delay for 'msec' milliseconds
 *
 * NOTES:
 *  1. Macro assumes 4 clocks per instruction cycle. However, in certain cases
 *     instruction cycle is extended by 1 or 2 additional clocks. In these
 *     cases accuracy of MSEC_DELAY macro is not guaranteed:
 *      1.1 in case of MLX4 activity the instruction cycle is equal
 *          to 5-6 clocks
 *      1.2 in some products the instruction cycle can be configured to be
 *          equal to 5 clocks
 */
STATIC INLINE void MSEC_DELAY(int16_t msec)
{
    int16_t i;
    for(i = 0; i < msec; i++) {
        __asm__ __volatile__ (
            "mov X, %[cnt]\n\t"
            "djnz X,."
            :
            : [cnt] "i" (FPLL/4)
            : "X");
    }
}

/*#define __SYS_ENTER_PROTECTED_MODE  __asm__ __volatile__ ("call fp2:0x00")*/

#define LOOP_FOREVER()  __asm__ __volatile__ ("jmp .")

/* API */
STATIC INLINE void Sys_ResetCpu (void);

/* === Implementation ====================================================== */
/**
    Reset CPU by software
 */
#ifndef UNITTEST
STATIC INLINE
void Sys_ResetCpu (void)
{
    IO_SET(RST_CTRL, SOFT_RESET, 1);    /* Reset happens immediately after writing SOFT_RESET bit */
}
#endif

/* Debug information output macro */
#ifdef FPGA_DEBUG_OUTPUT
#define DEBUG_OUTPUT(info)         \
    do {                           \
        SynLabel(info);            \
    } while(0)
#elif defined(UTESTS_DEBUG_OUTPUT)
#define DEBUG_OUTPUT(info)         \
    do {                           \
        printf_small(#info "\n");  \
    } while(0)
#else
#define DEBUG_OUTPUT(info)
#endif

#endif

