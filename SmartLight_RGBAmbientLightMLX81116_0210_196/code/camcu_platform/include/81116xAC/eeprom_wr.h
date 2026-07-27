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

#ifndef EEPROM_WR_DRV_H
#define EEPROM_WR_DRV_H

#include "compiler_abstraction.h"
#include "eeprom_map.h"
#include "eeprom_drv.h"
#include "flash_cfg.h"
#include "io.h"

#ifdef DBG_FPGA
#include "dbg_tools.h"
#endif

/* functions prefix which manages the library's functions memory mapping */
#ifndef EEPROM_PREFIX
#define EEPROM_PREFIX
#endif

/** EEPROM_Write_Mode_t defines the mode for the EEPROM write operation */
typedef enum {
    eepFullWrite=0x00u, /**< performs the complete XFAB flow (pre-write, erase, write steps) */
    eepWriteOnly=0x01u, /**< performs only write */
    eepPreWrErase=0x02u,/**< performs pre-write and erase */
    eepEraseOnly=0x03u, /**< performs only erase */
    eepPreWrite=0x04u   /**< performs only pre-write */
} EEPROM_Write_Mode_t;

/** The structure for the write EEPROM functions state-machine */
typedef struct {
    uint16_t address;                   /**< The EEPROM's address */
    uint16_t * data64bit;               /**< The data to write */
    uint16_t write_access_key;          /**< Write KEY, should be 0x07 before the write */
    EEPROM_Result_t write_state;        /**< The current write-operation state */
} EEPROM_WriteStruct_t;

/* Convenient macroses to check and wait some dedicated FLASH state */

/** EEPROM_WAIT_STATUS waits the EEPROM for the certain mode expected
 * @note the macros perform the infinite loop. Therefore AWD acknowledge should be handled accordingly
 */
/* Initiates the EEPROM with timings */
void EEPROM_WriteWord64_blocking( const uint16_t address, uint16_t *data64bit, uint16_t const write_acces_key );   /* Writes 64bits of data into the EEPROM */
/* Non-blocking mechanism implementation */
EEPROM_Result_t EEPROM_WriteWord64_nonblocking(EEPROM_WriteStruct_t *writeData );
/* "Common" (universal) implementation takes too much size. Should be used from the FW library. BL needs to use the 64bit alignment. */
void EEPROM_WriteData( uint16_t address, const uint16_t *data, uint8_t size16, uint16_t write_access_key );   /* Writes random data into the EEPROM, blocking, size's in 16bit words */

#endif /* EEPROM_WR_DRV_H */
