/**
 * @file
 * @brief API definition for the bl_melibus module
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
 * @details
 * MeLiBus bootloader
 */

#ifndef BL_MELIBUS_H
#define BL_MELIBUS_H

#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "hal_melibus.h"
#include "memory_map.h"
#include "static_assert.h"

typedef enum __attribute__((packed)) BlMelCmd_e {
    /* Protocol-defined commands */
    MEL_BL_CMD_WRITE_FLASH = 0x01,
    MEL_BL_CMD_WRITE_EEPROM = 0x02,
    MEL_BL_CMD_WRITE_RAM = 0x04,
    MEL_BL_CMD_MOVE_POINTER = 0x08,
    MEL_BL_CMD_FLASH_CHECK = 0x14,
    MEL_BL_CMD_RESET = 0x11,
    MEL_BL_CMD_ERASE_FLASH = 0x81,
    MEL_BL_CMD_ERASE_FLASH_CS = 0x71,
    MEL_BL_CMD_REQUEST_STATUS = 0x18,

    MEL_BL_CMD_READ_FLASH = 0x12,
    MEL_BL_CMD_READ_EEPROM = 0x21,
    MEL_BL_CMD_READ_RAM = 0x22,
    MEL_BL_CMD_WRITE_KEY = 0x41,
    MEL_BL_CMD_EXECUTE_RAM = 0xB1,

    MEL_BL_CMD_AA = 0xFC,
    MEL_BL_CMD_EPM = 0xFE
} BlMelCmd_t;

typedef enum __attribute__((packed)) BlStatus_e {
    /* Protocol-defined statuses */
    MEL_BL_STATUS_IDLE = 0x00,
    MEL_BL_STATUS_CMD_ERROR = 0x99,

    MEL_BL_STATUS_WRITE_FLASH = 0x01,
    MEL_BL_STATUS_WRITE_FLASH_OK = 0x91,
    MEL_BL_STATUS_WRITE_EEPROM_OK = 0x02,
    MEL_BL_STATUS_WRITE_RAM = 0x04,
    MEL_BL_STATUS_MOVE_POINTER = 0x08,
    MEL_BL_STATUS_FLASH_CHECK_OK = 0x14,
    MEL_BL_STATUS_RESET = 0x11,
    MEL_BL_STATUS_ERASE_FLASH_OK = 0x81,
    MEL_BL_STATUS_ERASE_FLASH_CS_OK = 0x71,
    MEL_BL_STATUS_REQUEST_STATUS = 0x18,

    MEL_BL_STATUS_READ_FLASH = 0x12,
    MEL_BL_STATUS_READ_EEPROM = 0x21,
    MEL_BL_STATUS_READ_RAM = 0x22,
    MEL_BL_STATUS_WRITE_KEY = 0x41,
    MEL_BL_STATUS_EXECUTE_RAM = 0xB1,

    MEL_BL_STATUS_FL_WRITE_ERROR = 0xE1,
    MEL_BL_STATUS_EEPROM_WRITE_ERROR = 0xE2,
    MEL_BL_STATUS_FL_ERASE_ERROR = 0xE3,
    MEL_BL_STATUS_FL_CS_ERASE_ERROR = 0xE4,
    MEL_BL_STATUS_FL_CS_WRITE_ERROR = 0xE5,
    MEL_BL_STATUS_FLASH_CHECK_ERROR = 0xE6,

    MEL_BL_STATUS_FL_WRITE_BUSY_BUF_EMPTY = 0xF0,
    MEL_BL_STATUS_FL_WRITE_BUSY_BUF_FULL = 0xF1,
    MEL_BL_STATUS_EEP_WRITE_BUSY = 0xF2,
    MEL_BL_STATUS_FL_ERASE_BUSY = 0xF3,
    MEL_BL_STATUS_FL_CS_ERASE_BUSY = 0xF4,
    MEL_BL_CMD_FLASH_CHECK_BUSY = 0xF5,

    MEL_BL_STATUS_START_AA = 0xFB,
    MEL_BL_STATUS_END_AA = 0xFC,

    MEL_BL_STATUS_CMD_EMPTY_FLASH = 0xFD,
    MEL_BL_STATUS_CMD_EPM = 0xFE

} BlStatus_t;

/* MeLiBu RX message data size in words per commands */
#define MEL_MGS_DATA_HEADER_SIZE     1u
#define MEL_CMD_WRITE_FLASH_DATA_SIZE  8u
#define MEL_CMD_WRITE_FLASH_SIZE    (MEL_CMD_WRITE_FLASH_DATA_SIZE + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_WRITE_EEPROM_SIZE   (4u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_WRITE_RAM_SIZE      (8u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_MOVE_POINTER_SIZE   (2u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_FLASH_CHECK_SIZE    (0u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_RESET_SIZE          (0u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_ERASE_FLASH_SIZE    (2u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_ERASE_FLASH_CS_SIZE (2u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_REQUEST_STATUS_SIZE (0u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_READ_FLASH_SIZE     (0u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_READ_EEPROM_SIZE    (0u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_READ_RAM_SIZE       (0u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_WRITE_KEY_SIZE      (4u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_EXECUTE_RAM_SIZE    (0u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_AA_SIZE             (1u + MEL_MGS_DATA_HEADER_SIZE)
#define MEL_CMD_EPM_SIZE            (2u + MEL_MGS_DATA_HEADER_SIZE)

/* An option for definitions and structures to revert the bytes sequence in the word. */
#define BYTESWAPPED

#ifdef BYTESWAPPED

/* Erase FLASH command pattern: '0x24, 0x01, 0x81, 0x04'*/
#define FL_ERASE_CMD_PATTERN_FIRST_WORD     0x2401u
#define FL_ERASE_CMD_PATTERN_SECOND_WORD    0x8104u

#else
/* Erase FLASH command pattern: '0x24, 0x01, 0x81, 0x04'*/
#define FL_ERASE_CMD_PATTERN_FIRST_WORD     0x0124u
#define FL_ERASE_CMD_PATTERN_SECOND_WORD    0x0481u

#endif /* BYTESWAPPED */

/* Default MeLiBus ID for boot loader mode */
#define BL_MEL_DEFAULT_ADDRESS          0x3Fu
#if (FPLL<=16000)
#define BL_MEL_DEFAULT_BAUDRATE_CFG     0u
#else
#define BL_MEL_DEFAULT_BAUDRATE_CFG     1u
#endif

/* Ticks number to wait to acknowledge being sent */
#define WAIT_SEND_ACK_TICKS      ((uint16_t)(((uint32_t)FPLL * 15u)/1000u/4u)) /* Estimated ticks count for 15us delay */

typedef union {
    uint8_t str[8];
    uint16_t word[4];
} BlMelVersion_t;

/* FLASH settings */
#define FL_PAGE_COUNT       (MEM_FLASH_SIZE/MEM_FLASH_PAGE_SIZE)
#define FLASH_COMMAND_KEYS  (MEM_RAM_START+MEM_RAM_SIZE-0x40)
#define EEPROM_COMMAND_KEY  (FLASH_COMMAND_KEYS + 6u)
#define FL_DEFAULT_PAGE_NUMBER  1
#define FL_CS_PAGE_NUMBER   (FL_PAGE_COUNT+0u)

#define BL_EE_PAGE_SIZE             (8u)
#define BL_RAM_PAGE_SIZE            (16u)

/* MeliBus RX message data structure */
#define BL_MEL_RX_DATA_WORD_SIZE    8u
#define CMD_HEADER_MASK             0x07u
typedef struct BlMelMsgRxData_s {
#ifndef BYTESWAPPED
    uint8_t blCmdHeaderCnt : 3; /* Boot loader command header counter */
    uint8_t blCmdHeaderReserved : 5; /* Reserved bits in the header */
    BlMelCmd_t blCmd; /* Boot loader command byte */
#else
    BlMelCmd_t blCmd; /* Boot loader command byte */
    uint8_t blCmdHeaderCnt : 3; /* Boot loader command header counter */
    uint8_t blCmdHeaderReserved : 5; /* Reserved bits in the header */
#endif
    uint16_t blRxData[BL_MEL_RX_DATA_WORD_SIZE]; /* Boot loader data field */
} BlMelMsgRxData_t;

/* MeliBus boot loader RX buffer structure */
typedef struct BlMelRxBuf_s {
    MelMsgId_t id; /* Message ID field (16 bits size) */
    BlMelMsgRxData_t data; /* Boot loaded message data field (18 bytes) */
    uint8_t dummy[18]; /* Full size of RX buffer must be 38 bytes to receive message with maximum length */
} BlMelRxBuf_t;

/* Boot loader low-level communication status byte structure */
typedef union {
    struct {
        uint8_t blMelTxMsgCnt : 2; /* Slave transmit message counter (only for Flash read command) */
        uint8_t blMelCmdError : 1; /* MeliBu command error flag */
        uint8_t blMelCommParityError : 1; /* MeLiBu communication message parity error detected by MeLiBu HW  */
        uint8_t blMelCommCrcError : 1; /* MeLiBu communication message CRC error detected by MeLiBu HW */
        uint8_t blMelCommBreakErrors : 1; /* MeLiBu communication WRONG_BREAK or BREAK_IN_FRAME errors detected by MeLiBu HW */
        uint8_t blMelCommTxTimeout : 1; /* MeLiBu communication bus TX dominant timeout detected by MeLiBu HW */
        uint8_t blMelResetFlag : 1; /* MeliBu boot loader reset flag */
    } s;
    uint8_t u;
} BlMelStatus_t;

/* MeliBus boot loader TX buffer structure */
#define BL_MEL_TX_DATA_WORD_SIZE    8u
typedef struct BlMelTxBuf_s {
    BlMelStatus_t blMelStatus; /* Boot loader  low-level communication status bytes */
    BlStatus_t blStatus; /* Boot loader status bytes */
    uint16_t blTxData[BL_MEL_TX_DATA_WORD_SIZE]; /* Boot loader TX data field */
    uint8_t dummy[18]; /* Full size of TX buffer must be 36 bytes to transmit message with maximum length */
} BlMelTxBuf_t;

/* FW protection key structure */
typedef struct BlMelProtectionKey_s {
    uint32_t keyA;
    uint32_t keyB;
} BlMelProtectionKey_t;

typedef struct BlMelInternalFlags_s {
    uint16_t blFlashWriteCmdMsgCnt : 3; /* Boot loader write flash command messages counter */
    uint16_t blCleanMemorizedErrorFlags : 1;
    uint16_t blSetAAoutPinFlag : 1;
    uint16_t dummyFlags : 11;
} BlMelInternalFlags_t;

/* MeliBus boot loader parameters structure */
typedef struct BlMelParams_s {
    BlMelRxBuf_t blMelRxBufA; /* Boot loader MeLiBus receive message buffer A */
    BlMelRxBuf_t blMelRxBufB; /* Boot loader MeLiBus receive message buffer B */
    BlMelTxBuf_t blMelTxBuf; /* Boot loader MeLiBus transmit */
    uint16_t blMelFlashWriteCmdBuffer[MEM_FLASH_PAGE_SIZE / sizeof(uint16_t)]; /* Boot loader MeLiBus flash write command data buffer */
    uint16_t blMelEepromWriteCmdBuffer[BL_EE_PAGE_SIZE / sizeof(uint16_t)]; /* Boot loader MeLiBus EEPROM write command data buffer */
    uint16_t* blMelTxDataPtr;
    uint16_t blMelTxDataSize;
    int16_t blMelCurrentFlashPage; /* signed value to store the FLASH CS sector page number as -1 */
    uint16_t blMelCurrentEepromOffset;
    uint16_t blMelCurrentRamOffset;
    BlMelProtectionKey_t blMelProtectionKey;
    BlMelInternalFlags_t blMelInternalFlags;
} BlMelParams_t;

typedef void (*BLMelFunc_t)(void); /* functions' typedef to call by cast */

/*
 * Global variables
 */
extern volatile BlMelParams_t blMelParams;

/*
 * Boot loader functions
 */
extern MelResult_t BlMel_Init(void); /* Function creates and initiates all its settings, initiates the peripherals needed. It also subscribes and calls the callbacks, such as 'ByteReceived' */
extern void BlMel_Tick(void);

#endif /* BL_MELIBUS_H */
