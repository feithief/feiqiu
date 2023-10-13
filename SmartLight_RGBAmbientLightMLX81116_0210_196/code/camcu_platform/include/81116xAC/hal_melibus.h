/**
 * @file
 * @brief API definition for the hal_melibus module
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
 * @details HAL and SW library for the Melibus HW interface, including the FW library
 *
 */

#ifndef HAL_MELIBUS_H
#define HAL_MELIBUS_H

#include <stdint.h>
#include <stdbool.h>
#include <syslib.h>
#include "io.h"

#if defined(HAS_HW_MELIBU_TRIM) && defined(HAS_HW_MELIBU_TRIM2)
#error "MELIBU_TRIM and MELIBU_TRIM2 cannot be both defined. Please check documentation"
#endif

#define NUMBER_OF_AAIN_SAMPLES   16u

/** MeliBus interrupts configuration structure
 *
 * Interrupts priority must be defined in range [0:3]
 * Interrupt priority encoding:
 *  configuration 0 -> Priority 3
 *  configuration 1 -> Priority 4
 *  configuration 2 -> Priority 5
 *  configuration 3 -> Priority 6
 *
 */
typedef struct MelInterruptConfiguration_s {
    uint16_t melErrorInterruptEnable : 1;
    uint16_t melFrameTransmittedInterruptEnable : 1;
    uint16_t melFrameHeaderReceivedInterruptEnable : 1;
    uint16_t melFrameReceivedInterruptEnable : 1;
    uint16_t melTxTimeoutInterruptEnable : 1;
    uint16_t melErrorInterruptPriority : 2;
    uint16_t melFrameTransmittedInterruptPriority : 2;
    uint16_t melFrameHeaderReceivedInterruptPriority : 2;
    uint16_t melFrameReceivedInterruptPriority : 2;
    uint16_t melTxTimeoutInterruptPriority : 2;
    uint16_t melUartErrorInterruptsEnable : 1;
} MelInterruptConfiguration_t;

/** MeliBus hardware configuration structure */
typedef struct MelConfiguration_s {
    uint16_t melBaudRate; /**< Baud rate configuration */
    uint16_t melAddress : 6; /**< Address to be compared in address */
    uint16_t melAddressMask : 6; /**< Mask all bits set when filtering for address */
    uint16_t melIdDummy : 4; /**< ID unused bits */
    uint16_t melDmaRda; /**< Read buffer address for buffer a */
    uint16_t melDmaRdb; /**< Read buffer address for buffer b */
    uint16_t melDmaTx; /**< Transmit buffer address */
    uint16_t melCtrlEn : 1; /**< MeliBus enable  0: block is not active (CPU has direct UART access),  1:block  is active  */
    /* Note: Melibus hardware should be disabled during configuration. Mel_Start function must be called to start MeLiBus HW */
    uint16_t melCtrlAckEn : 1; /**< Activate sending an ACK field */
    uint16_t melCtrlDisParity : 1; /**< Disable MeLiBus ID field parity check in hardware */
    uint16_t melCtrlDisCrc : 1; /**< Disable MeLiBus message CRC check in hardware */
    uint16_t melCtrlSelTxd : 2; /**< TX output multiplexer configuration */
    uint16_t melCtrlSelRxIn : 2; /**< Multiplexer in front of MeLiBus UART hardware RX input configuration */
    uint16_t melCtrlDummy : 8;
} MelConfiguration_t;

/** Melibus UART transmit output to Physical Layer configuration */
typedef enum MelSelTxdConfiguration_e {
    MEL_SEL_TX_OUTPUT = 0, /**< Connect Melibus physical output to UART transmit output */
    MEL_SEL_TX_AA_IN, MEL_SEL_TX_RXD, MEL_SEL_TX_0
} MelSelTxdConfiguration_t;

/** MeLiBus UART receive input from Physical Layer configuration */
typedef enum MelSelRxInConfiguration_e {
    MEL_SEL_RX_INPUT = 0, /**< Connect Melibus physical input to UART RX input */
    MEL_SEL_RX_AA_IN, MEL_SEL_RX_0, MEL_SEL_RX_1
} MelSelRxInConfiguration_t;

/** UART module Message length selector configuration */
typedef enum UartMlsConfiguration_e {
    UART_MLS_9_BITS = 0, /**< UART Message length 9 bits including start and stop bits */
    UART_MLS_10_BITS, /**< UART Message length 10 bits including start and stop bits */
    UART_MLS_11_BITS, /**< UART Message length 11 bits including start and stop bits */
    UART_MLS_12_BITS, /**< UART Message length 12 bits including start and stop bits */
    UART_MLS_13_BITS, /**< UART Message length 13 bits including start and stop bits */
    UART_MLS_14_BITS, /**< UART Message length 14 bits including start and stop bits */
    UART_MLS_15_BITS, /**< UART Message length 15 bits including start and stop bits */
    UART_MLS_16_BITS /**< UART Message length 16 bits including start and stop bits */
} UartMlsConfiguration_t;

/** UART module Bit scrambler selector configuration */
typedef enum UartBscConfiguration_e {
    UART_BSC_NRZI_L = 0, /**< UART transmit format NRZ, LSB first  */
    UART_BSC_NRZI_M, /**< UART transmit format NRZ, MSB first */
    UART_BSC_MPE, /**< UART transmit format Manchester Phase Encoding */
    UART_BSC_RZ /**< UART transmit format Return to Zero */
} UartBscConfiguration_t;

/** The common MeLiBus result type */
typedef enum MelResult_e {
    MEL_FAIL = 0, MEL_SUCCESS
} MelResult_t;

/** MeLiBus AA OUT pin configuration */
typedef enum MelSelAAOUTpinConfiguration_e {
    MEL_AAOUT_PIN_SEL_AAOUT_PORT = 0, /**< AA out pin driver is connected to AA_OUT port */
    MEL_AAOUT_PIN_SEL_AAIN_PIN, /**< AA out pin driver is connected to AA input pin signal */
    MEL_AAOUT_PIN_SEL_URX, /**< AA out pin driver is connected to MeLiBu PHY RX signal */
    MEL_AAOUT_PIN_SEL_UTX_DIG /**< AA out pin driver is connected to MeLiBu digital TX signal */
} MelSelAAOUTpinConfiguration_t;

/** MeliBus RX buffer valid status */
typedef enum MelRxBufValidStatus_e {
    MEL_VALID_RX_BUF_B = 0, /**< MeLiBus RX message buffer B is valid */
    MEL_VALID_RX_BUF_A /**< MeLiBus RX message buffer A is valid */
} MelRxBufValidStatus_t;

/** An option for definitions and structures to revert the bytes sequence in the word. */
#define BYTESWAPPED

#ifdef BYTESWAPPED
/* The 'deadbeaf' pattern for the enter-programming mode */
#define EPM_CMD_PATTERN_FIRST_WORD          0xDEADu
#define EPM_CMD_PATTERN_SECOND_WORD         0xBEEFu
#else
/* The 'deadbeaf' pattern for the enter-programming mode */
#define EPM_CMD_PATTERN_FIRST_WORD          0xADDEu
#define EPM_CMD_PATTERN_SECOND_WORD         0xEFBEu
#endif /* BYTESWAPPED */

#define BL_MEL_INSTR_EXT    7u
typedef union {
#ifndef BYTESWAPPED
    /** MeliBus message ID structure */
    struct {
        uint16_t funcSel : 1; /**< Function select: 0 - SubAdr/ InstrExt field is decoded by software; 1 - data length encoded by SubAdr */
        uint16_t slaveReceiveSel : 1; /**< Slave receive select: 0 - slave receive; 1 - slave transmit */
        uint16_t slaveAddress : 6; /**< Slave address */
        uint16_t p0 : 1; /**< Parity bit 0 */
        uint16_t p1 : 1; /**< Parity bit 1 */
        uint16_t subAdr : 3; /**< Sub address field */
        uint16_t instrExt : 3; /**< Instruction extension */
    }s;
#else
    struct {
        uint16_t p0 : 1; /**< Parity bit 0 */
        uint16_t p1 : 1; /**< Parity bit 1 */
        uint16_t subAdr : 3; /**< Sub address field */
        uint16_t instrExt : 3; /**< Instruction extension */
        uint16_t funcSel : 1; /**< Function select: 0 - SubAdr/ InstrExt field is decoded by software; 1 - data length encoded by SubAdr */
        uint16_t slaveReceiveSel : 1; /**< Slave receive select: 0 - slave receive; 1 - slave transmit */
        uint16_t slaveAddress : 6; /**< Slave address */
    } s;
#endif
    uint16_t u;
} MelMsgId_t;

/** MeliBus Enter programming mode RX buffer structure */
typedef struct MelEpmCmdRxBuf_s {
#ifndef BYTESWAPPED
    MelMsgId_t id; /**< Message ID field (16 bits size) */
    uint8_t cmdHeader; /**< EPM command header */
    uint8_t cmd; /**< EPM command byte */
    uint16_t epmCmdPatternFirstWord; /**< The 'deadbeaf' pattern for the enter-programming mode */
    uint16_t epmCmdPatternSecondWord; /**< The 'deadbeaf' pattern for the enter-programming mode */
    uint16_t dummyData[15]; /**< Full size of RX buffer must be 38 bytes to receive message with maximum length */
#else
    MelMsgId_t id; /**< Message ID field (16 bits size) */
    uint8_t cmd; /**< EPM command byte */
    uint8_t cmdHeader; /**< EPM command header */
    uint16_t epmCmdPatternFirstWord; /**< The 'deadbeaf' pattern for the enter-programming mode */
    uint16_t epmCmdPatternSecondWord; /**< The 'deadbeaf' pattern for the enter-programming mode */
    uint16_t dummyData[15]; /**< Full size of RX buffer must be 38 bytes to receive message with maximum length */
#endif
} MelEpmCmdRxBuf_t;

/** Function gets RX buffer valid status
 *
 *  @return  valid RX buffer: 0 = read buffer B, 1 = read buffer A is valid
 */
STATIC INLINE MelRxBufValidStatus_t Mel_GetValidRxBufferStatus(void)
{
    return (IO_GET(PORT_MELIBUS_STAT, RD_BUFFER_VALID));
}

/** Function gets valid RX buffer address
 *
 *  @return  valid RX buffer address
 */
STATIC INLINE uint16_t Mel_GetValidRxBufferAddress(void)
{
    if (Mel_GetValidRxBufferStatus() != MEL_VALID_RX_BUF_B) {
        /* Return MeLiBus Read buffer address for buffer A */
        return IO_GET(PORT_DMA_RDA, DMA_RDA);
    } else {
        /* Return MeLiBus Read buffer address for buffer B */
        return IO_GET(PORT_DMA_RDB, DMA_RDB);
    }
}

/*
 * Library functions
 */
extern void Mel_ConfigureInterrupts(
    const MelInterruptConfiguration_t interruptConfig);
extern MelResult_t Mel_Init(MelConfiguration_t* config);
extern void Mel_Start(void);
extern void Mel_Stop(void);
extern void Mel_Restart(void);
extern MelResult_t Mel_CheckEpmCmd(void);
extern MelResult_t Mel_GetUartTxAndRxErrorStatus(void);

/* Auto-addressing functions */
extern void Mel_SetAAoutPin(void);
extern void Mel_ClearAAoutPin(void);
extern uint16_t Mel_GetAAinStatus(void);
extern void Mel_SetNewNAD(uint16_t newNAD);

#endif /* HAL_MELIBUS_H */
