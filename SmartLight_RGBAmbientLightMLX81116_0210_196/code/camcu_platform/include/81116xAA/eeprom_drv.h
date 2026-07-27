/**
 * @file
 * @brief EEPROM support library functions
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
 * EEPROM driver. Used to manage the dedicated EEPROM type.
 */

#ifndef EEPROM_DRV_H
#define EEPROM_DRV_H

#include "compiler_abstraction.h"
#include "eeprom_map.h"
#include "flash_cfg.h"
#include "io.h"

#ifdef DBG_FPGA
#include "dbg_tools.h"
#endif

#ifdef EEP_USE_DIRECT_SETTINGS

#ifndef EE_EEP_WR_TIMING_VALUE
/* Use default value to initialize the EEPROM. This definition should exist outside the library */
#warning "EE_EEP_WR_TIMING_VALUE is not set. Using default value"
#define EE_EEP_WR_TIMING_VALUE                          0x3B1Du
#endif

#ifndef EE_EEP_ER_TIMING_VALUE
/* Use default value to initialize the EEPROM. This definition should exist outside the library */
#warning "EE_EEP_ER_TIMING_VALUE is not set. Using default value"
#define EE_EEP_ER_TIMING_VALUE                          0x3B1Du
#endif

#ifndef EE_EEP_PROG_CYCLE_VALUE
/* Use default value to initialize the EEPROM. This definition should exist outside the library */
#warning "EE_EEP_PROG_CYCLE_VALUE is not set. Using default value"
#define EE_EEP_PROG_CYCLE_VALUE                            0x000Fu
#endif

#ifndef EE_EEP_READ_TIMING_VALUE
/* Use default value to initialize the EEPROM. This definition should exist outside the library */
#warning "EE_EEP_READ_TIMING_VALUE is not set. Using default value"
#define EE_EEP_READ_TIMING_VALUE                            0x000Fu
#endif


#ifndef EE_EEP_WAIT_STATES_VALUE
/* Use default value to initialize the EEPROM. This definition should exist outside the library */
#warning "EE_EEP_WAIT_STATES_VALUE is not set. Using default value"
#define EE_EEP_WAIT_STATES_VALUE                          0x0F00u
#endif


#else /* EEP_USE_DIRECT_SETTINGS */
/* EEPROM Timing parameters, in microseconds: ( 100 us step for write-time, 10us step for erase-time )*/
#ifndef EEP_WRITE_TIME
#warning "EEP_WRITE_TIME is not set. Using default value"
#define EEP_WRITE_TIME                                  6000u
#endif

#ifndef EEP_PREWRITE_TIME
#warning "EEP_PREWRITE_TIME is not set. Using default value"
#define EEP_PREWRITE_TIME                                  300u
#endif

#ifndef EEP_ERASE_TIME
#warning "EEP_ERASE_TIME is not set. Using default value"
#define EEP_ERASE_TIME                                  300u
#endif

#ifndef EEP_PROGRAM_CYCLES /* EEP_PROGRAM_CYCLES - cycles quantity for write, -1 (value 0..31)*/
#warning "EEP_PROGRAM_CYCLES is not set. Using default value"
#define EEP_PROGRAM_CYCLES                              16u
#endif

#ifndef EEP_WAIT_STATES /* EEP_WAIT_STATES - EEPROM Wait-states to read, -1 (value 0..15)*/
#warning "EEP_WAIT_STATES is not set. Using default value"
#define EEP_WAIT_STATES                                 15u
#endif

/* Driver settings constraints' checks */
#if (EEP_WRITE_TIME<100) || (EEP_WRITE_TIME>6300)
#error EEPROM_WRITE_TIME cannot be less than 100uS and more then 6300uS!!!
#endif

#if (EEP_PREWRITE_TIME<100) || (EEP_PREWRITE_TIME>1270)
#error EEP_PREWRITE_TIME cannot be less than 10uS and more then 1270uS!!!
#endif

#if (EEP_ERASE_TIME<100) || (EEP_ERASE_TIME>1270)
#error EEPROM_ERASE_TIME cannot be less than 10uS and more then 1270uS!!!
#endif

#if (EEP_PROGRAM_CYCLES>31)
#error EEP_PROGRAM_CYCLES cannot be more than 31!!!
#endif

#if (EEP_WAIT_STATES>15)
#error EEP_WAIT_STATES cannot be more than 15!!!
#endif


/* Internal declarations */
#define EEPROM_PREWRITE_TIME_INT    ( (uint16_t)( EEP_PREWRITE_TIME/10u - 1 ) )
#define EEPROM_WRITE_TIME_INT       ( (uint16_t)( EEP_WRITE_TIME/100u - 1 ) )
#define EEPROM_ERASE_TIME_INT       ( (uint16_t)( EEP_ERASE_TIME/10u - 1 ) )

#endif /* EEP_USE_DIRECT_SETTINGS */

/* EEPROM NVM Mode Register (R1_EE) configuration for EEPROM main or configuration array selection */
#define EE_R1_SEL_MASK      0x0F00u
#define EE_R1_SEL_MA_CFG    0x0100u
#define EE_R1_SEL_CS_CFG    0x0200u

/* EEPROM buffer size, in words, should be 16bit size!!!          */
/* Note: EEPROM_DATA_SIZE should be 1, 2, 4, 8 ... word's size!!! */
#define EEPROM_DATA_SIZE ((uint16_t)4u)
#define EEPROM_DATA_SIZE_MSK (~((EEPROM_DATA_SIZE<<1)-1))

#define EEPROM_WE_KEY_VALUE 0x07u



/* functions prefix which manages the library's functions memory mapping */
#ifndef EEPROM_PREFIX
#define EEPROM_PREFIX
#endif

extern volatile uint16_t EEPROM_CFG_REGS[];

/** EEPROM-write process current state */
typedef enum {
    eepIdle=0,          /**< Write is not executed */
    eepBusy=0xFF,       /**< Write is in active phase */
} EEPROM_Result_t;

/* Convenient macroses to check and wait some dedicated FLASH state */

/** EEPROM_WAIT_STATUS waits the EEPROM for the certain mode expected
 * @note the macros perform the infinite loop. Therefore AWD acknowledge should be handled accordingly
 */
#define EEPROM_WAIT_STATUS(status)                    do {;} while (IO_GET(EEPROM_FLASH, EE_STATUS)!=status)

/** EEPROM_WAIT_NOT_STATUS waits the EEPROM for the certain mode should be changed
 * @note the macros perform the infinite loop. Therefore AWD acknowledge should be handled accordingly
 */
#define EEPROM_WAIT_NOT_STATUS(status)                do {;} while (IO_GET(EEPROM_FLASH, EE_STATUS)==status)

#define EEPROM_CRC_Check(offs, size)                nvram_CalcCRC(offs, size)
void EEPROM_TurnOn(void);
void EEPROM_Init(void);
void EEPROM_Configure(void);

#ifndef UNITTEST
/** EEPROM_SetConfigured changes the EEPROM's mode to the configured state
 *
 * The function is called when the EEPROM was inited and configured. The configured state protects some settings for the EEPROM
 */
STATIC INLINE void EEPROM_SetConfigured(void)
{
    /* NOTE: EE_CONFIGURED should not be set to 1  before all EEPROM CS setting are not loaded into IO ports */
    IO_SET(EEPROM_FLASH, EE_CONFIGURED, 1);
    while ( IO_GET(EEPROM_FLASH, EE_BUSY) != 0u ) {/* Wait until the EEPROM will be "configurable" TODO: What if not? */
    }
}


/** EEPROM_SetTimings copies the timings for the EEPROM for write/erase/prog_cycle operations from the specified EEPROM settings
 *
 * Function is called when the EEPROM was inited and configured. The configured state protects some settings for the EEPROM.
 * Leaves the EEPROM wait_states
 */
INLINE void EEPROM_SetTimings(void)
{
#ifdef DBG_FPGA
    writeln("EEtms");
#endif
    IO_HOST(EEPROM_FLASH, EE_PRE_WR_TIME) = EE_EEP_WR_TIMING_VALUE;
    IO_HOST(EEPROM_FLASH, EE_ER_TIME) = EE_EEP_ER_TIMING_VALUE;
    IO_HOST(EEPROM_FLASH, EE_PROGRAM_CYCLE) = EE_EEP_PROG_CYCLE_VALUE;
    IO_HOST(EEPROM_FLASH,
            EE_WAIT_STATES) =
        (EE_EEP_READ_TIMING_VALUE &
         ~(IO_MASK(EEPROM_FLASH,
                   EE_WAIT_STATES))) |
        ((uint16_t)IO_GET(EEPROM_FLASH, EE_WAIT_STATES) << IO_OFFSET(EEPROM_FLASH, EE_WAIT_STATES));
}



/** EEPROM_SetRxRegisters copies the EE_R1, EE_R2 registers for the EEPROM from the specified EEPROM settings */
INLINE void EEPROM_SetRxRegisters(void)
{
    register uint16_t tmp; /* EEPROM KEY should be written always before each write operation */
#ifdef DBG_FPGA
    writeln("EErg");
#endif
    /* Configuring the EEPROM registers */ /* Remove the R1_EE << EEPROM */
    tmp = EE_FL_R1_EE;
    IO_SET(EEPROM_FLASH, EE_WE_KEY, EEPROM_WE_KEY_VALUE); /* TODO: Optimise the request if possible, to avoid the rea-modify-write operation */
    EEPROM_CFG_REGS[1] = tmp;
    tmp = EE_FL_R2_EE;
    IO_SET(EEPROM_FLASH, EE_WE_KEY, EEPROM_WE_KEY_VALUE);
    EEPROM_CFG_REGS[2] = tmp;
}
#else
void EEPROM_SetTimings(void);
void EEPROM_SetRxRegisters(void);
void EEPROM_SetConfigured(void);
#endif
#endif

