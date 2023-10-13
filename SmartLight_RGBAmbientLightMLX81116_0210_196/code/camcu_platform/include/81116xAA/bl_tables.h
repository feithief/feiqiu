/**
 * @file
 * @brief The RAM-content expected for the Bootloader and/or startup sequence
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
 * @ingroup Bootloaders
 *
 * @details
 */

#ifndef BL_TABLES_H
#define BL_TABLES_H

#include "bl_defs.h"

#ifndef BL_AWD_TIMEOUT
  #define BL_AWD_TIMEOUT               1000
  #warning "The default BL_AWD_TIMEOUT value is used. Use BL_AWD_TIMEOUT=0 to disable it."
#endif /* BL_AWD_TIMEOUT */

#define BL_TABLE_KEY            0x05154832
#define BL_TABLE_NAD            0x01

#ifndef MAX_RESET_COUNTER
#define MAX_RESET_COUNTER       10
  #warning "The default MAX_RESET_COUNTER value is used. Use MAX_RESET_COUNTER=0 to disable this feature."
#endif /* MAX_RESET_COUNTER */

/*
 * This structure defines a set of shared parameters, needed to be stored, re-inited and analized after the each IC reset.
 *
 * */
typedef struct {
    uint32_t key; /* key, which should be set to determine the data integrity. */
    uint16_t NAD;   /* Personal address. Used to store the NAD from Application mode. */
    bl_Mode_t prog_mode; /* programming-mode bit. When - the system is working in programming mode, i.e. data parser executes the BL-commands instead of Empty-flash's. */
    volatile uint16_t reset_counter; /* Calculates the 'BAD'-resets for the FW application. Turns to the "Empty-FLASH" mode if this counter reaches the limit. Should be resetted by the FW. */
#if (BL_AWD_TIMEOUT>0)
    uint16_t awd_counter;
#endif  /* BL_AWD_TIMEOUT */
} bl_table_t;


/* Shared structure for the ROM-based API and libraries. Shares the data between reset-states and Application vs Bootloader modes. The bist flags are also here... */
extern bl_table_t bl_table;

typedef uint16_t (*mel_callback_func_t)(void); /* Common callback function type definition */

#endif /* BL_TABLES_H */
