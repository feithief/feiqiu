/**
 * @file
 * @brief UART serial interface support library
 * @internal
 *
 * @copyright (C) 2017-2018 Melexis N.V.
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
 */

#ifndef LIB_UART_H
#define LIB_UART_H

#include <syslib.h>
#include "sys_tools.h"
#include <plib.h>
#include "itc_helper.h"
#include "io.h"
#include "lib_wdg.h"

/** Bits scrambler */
typedef enum {
    NRZIL = 0u, /**< UART, using NRZ, LSB first [bits 15 -> 0] */
    NRZIM,   /**< UART, using NRZ, MSB first [bits 0 -> 15] */
    MPE,     /**< UART, using Manchester Phase Encoding [bits 15, 7, 14, 6 ... 8, 0] Data is written in bits TRD[15:8], and TRD[7:0] must be set to the data value complemented by 1 */
    RZ,      /**< UART, using Return to Zero [bits 8, 0, 9, 1 ... 15, 7]. Data is written in bits TRD[15:8]. Unused bits TRD[7:0] must be set to 00h. */
} UART_BSC_ctrl_t;

/** Message length selector. Double buffer mechanism */
typedef enum {
    UART_MLS_8Bit = 0u,/**< 8 bit of data BSD[7:0] + 1 [stop] bit as BSD8 */
    UART_MLS_9Bit,     /**< 9 bit of data BSD[8:0] + 1 [stop] bit as BSD9 */
    UART_MLS_10Bit,    /**< 10 bit of data BSD[9:0] + 1 [stop] bit as BSD10 */
    UART_MLS_11Bit,    /**< 11 bit of data BSD[10:0] + 1 [stop] bit as BSD11 */
    UART_MLS_12Bit,    /**< 12 bit of data BSD[11:0] + 1 [stop] bit as BSD12 */
    UART_MLS_13Bit,    /**< 13 bit of data BSD[12:0] + 1 [stop] bit as BSD13 */
    UART_MLS_14Bit,    /**< 14 bit of data BSD[13:0] + 1 [stop] bit as BSD14 */
    UART_MLS_15Bit,    /**< 15 bit of data BSD[14:0] + 1 [stop] bit as BSD15 */
} UART_MLS_ctrl_t;


/** Selector and Control Register type definition */
typedef union {
    struct {
        uint16_t : 1; /**< reserved bit-field */
        uint16_t TRE : 1; /**< Transmitter enable */
        uint16_t REE : 1; /**< Receiver enable */
        uint16_t ISB : 1; /**< Transmit IDLE state */
        uint16_t : 1; /**< reserved bit-field */
        uint16_t LBE : 1; /**< LIN bit error. Set to 0 if TRE is asserted low. Read and clear bit, set to zero for UART */
        uint16_t LSE : 1; /**< LIN start error. Set to 0 if TRE is asserted low. Read and clear bit, set to zero for UART */
        uint16_t LTE : 1; /**< LIN time out error. Set to 0 if TRE is asserted low. Read and clear bit, set to zero for UART */
        UART_MLS_ctrl_t MLS : 3; /**< Message length selector */
        UART_BSC_ctrl_t BSC : 2; /**< Bit scrambler control */
        uint16_t LBC : 1; /**< LIN bit control. Double buffer mechanism */
        uint16_t LSC : 1; /**< LIN start bit control. Double buffer mechanism */
        uint16_t LDC : 1; /**< LIN duration control */
    } ctrl;
    uint16_t data;
} UART_ctrl_t;


/** Describes the statuses and error flags of UART */
typedef union {
    struct {
        uint16_t : 8; /**< reserved [7:0] */
        uint16_t TRO : 1; /**< Transmit register overrun. In LIN mode, bit set to 0 if read UCTRL. Set to 0 if TRE is asserted low. Read and clear bit */
        uint16_t TSB : 1; /**< Transmit shifter busy. In LIN mode, bit set to 0 if LSE = 1 or LBE = 1. Set to 0 if TRE is asserted low */
        uint16_t RSO : 1; /**< Receive shifter overrun. Set to 0 if REE is asserted low. Read and clear bit */
        uint16_t RSB : 1; /**< Receive shifter busy. Set to 0 if REE is asserted low */
        uint16_t RRF : 1; /**< Receive register full. Set to 0 if REE is asserted low */
        uint16_t NBR : 1; /**< Noisy bit reception. Set to 0 if REE is asserted low. Read and clear bit */
        uint16_t SBE : 1; /**< Stop bit error. Set to 0 if REE is asserted low. Read and clear bit */
    } ctrl;
    uint16_t data;
} UART_status_t;

/** UART_GetRate is a special function to calculate the UART BRRD's register value from the expected baudrate, assigned in bits per second
 * @param[in] freq - the baudrate expected to be set
 * Fbit = Fck / (16 * (UART_RATE+1))
 */
STATIC INLINE uint16_t UART_GetRate(uint32_t freq)
{
    return ((uint32_t)FPLL*1000/8)/((freq*2)-1);
}

#ifndef IO_UART__BRRD_GET
#warning The UART is not available while the UART library is included.
#endif /* UART IO PORTS check */


/** UART_SetBaudrate sets the baudrate for UART communication
 * @param[in] brrd_reg_value the baudrate calculated for the UART HW. Can be set as a result of UART_GetRate function
 */
STATIC INLINE void UART_SetBaudrate(uint16_t brrd_reg_value)
{
    IO_SET(UART, BRRD, brrd_reg_value);
}

/** UART_INIT sets the configuration for the UART and it's baudrate
 * @param[in] rate the baudrate calculated for the UART. Can be set as a result of UART_GetRate function
 * @param[in] cfg the UART's configuration. Can have UART enabled in its settings.
 *
 */
STATIC INLINE void UART_INIT(uint16_t rate, UART_ctrl_t cfg)
{
    IO_SET(UART, REE, 0u, TRE, 0u);
    UART_SetBaudrate(rate);
    IO_HOST(UART, LDC) = cfg.data;
}

/* Interrupts support */

/** UART_RRI_ENABLE enables the Receive Register full Interrupt */
STATIC INLINE void UART_RRI_ENABLE(void)
{
    Itc_Clear(UART_RR);
    Itc_Enable(UART_RR);
}

/** UART_RRI_DISABLE disables the Receive Register full Interrupt */
STATIC INLINE void UART_RRI_DISABLE(void)
{
    Itc_Disable(UART_RR);
}

/** UART_TRI_ENABLE enables the Transmit Register empty Interrupt */
STATIC INLINE void UART_TRI_ENABLE(void)
{
    Itc_Clear(UART_TR);
    Itc_Enable(UART_TR);
}

/** UART_TRI_DISABLE disables the Transmit Register empty Interrupt */
STATIC INLINE void UART_TRI_DISABLE(void)
{
    Itc_Disable(UART_TR);
}

/** UART_RSI_ENABLE enables the Receive Shifter overrun Interrupt */
STATIC INLINE void UART_RSI_ENABLE(void)
{
    Itc_Clear(UART_RS);
    Itc_Enable(UART_RS);
}

/** UART_RSI_DISABLE disables the Receive Shifter overrun Interrupt */
STATIC INLINE void UART_RSI_DISABLE(void)
{
    Itc_Disable(UART_RS);
}

/** UART_TSI_ENABLE enables the Transmitter Shifter empty Interrupt */
STATIC INLINE void UART_TSI_ENABLE(void)
{
    Itc_Clear(UART_TS);
    Itc_Enable(UART_TS);
}

/** UART_TSI_DISABLE disables the Transmitter Shifter empty Interrupt */
STATIC INLINE void UART_TSI_DISABLE(void)
{
    Itc_Disable(UART_TS);
}

/** UART_SBI_ENABLE enables the Stop Bit Error Interrupt */
STATIC INLINE void UART_SBI_ENABLE(void)
{
    Itc_Clear(UART_SB);
    Itc_Enable(UART_SB);
}

/** UART_SBI_DISABLE disables the Stop Bit Error Interrupt */
STATIC INLINE void UART_SBI_DISABLE(void)
{
    Itc_Disable(UART_SB);
}

/** UART_TEI_ENABLE enables the Transmit Register overrun Interrupt */
STATIC INLINE void UART_TEI_ENABLE(void)
{
    Itc_Clear(UART_TE);
    Itc_Enable(UART_TE);
}

/** UART_TEI_DISABLE disables the Transmit Register overrun Interrupt */
STATIC INLINE void UART_TEI_DISABLE(void)
{
    Itc_Disable(UART_TE);
}

/* Flags support */

/** UART_TX_BUSY returns the current TX busy flag */
STATIC INLINE uint16_t UART_TX_BUSY(void)
{
    return IO_GET(UART, TRB);
}

/** UART_RX_FULL returns the current RX full flag */
STATIC INLINE uint16_t UART_RX_FULL(void)
{
    return IO_GET(UART, RRF);
}



/* Data sending */

/** __UART_PUT_FRAME Puts data frame with start and stop bits to UART
 *
 * @param[in] length sets the data bit length
 * @param[in] x sets the data to send
 *
 * Notes:
 *  1. Functions UART_PUT_FRAME_xx are only applicable for UART_FORMAT_LSB
 *     format (defined during initialization, see UART_INIT)
 *
 */
STATIC INLINE void __UART_PUT_FRAME(uint16_t length, uint16_t x)
{
    IO_SET(UART, TRD, (x << 1u) | ((uint16_t)1u << (length + 1u)));
}

/** UART_PUT_FRAME_7N1 sends the data in 7N1 format
 *
 * @param[in] data 7 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_7N1(uint16_t data)
{
    __UART_PUT_FRAME(7, data);
}

/** UART_PUT_FRAME_8N1 sends the data in 8N1 format
 *
 * @param[in] data 8 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_8N1(uint16_t data)
{
    __UART_PUT_FRAME(8, data);
}

/** UART_PUT_FRAME_8N1M sends the data in 8N1 format, assuming MSB mode
 *
 * @param[in] data 8 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_8N1M(uint16_t data)
{
    IO_SET(UART, TRD, (data<<7) | 0x40u ); // for 9bit cfg
}

/** UART_PUT_FRAME_9N1 sends the data in 9N1 format
 *
 * @param[in] data 9 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_9N1(uint16_t data)
{
    __UART_PUT_FRAME(9, data);
}

/** UART_PUT_FRAME_10N1 sends the data in 10N1 format
 *
 * @param[in] data 10 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_10N1(uint16_t data)
{
    __UART_PUT_FRAME(10, data);
}


/** UART_PUT_FRAME_11N1 sends the data in 11N1 format
 *
 * @param[in] data 11 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_11N1(uint16_t data)
{
    __UART_PUT_FRAME(11, data);
}


/** UART_PUT_FRAME_12N1 sends the data in 12N1 format
 *
 * @param[in] data 12 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_12N1(uint16_t data)
{
    __UART_PUT_FRAME(12, data);
}


/** UART_PUT_FRAME_13N1 sends the data in 13N1 format
 *
 * @param[in] data 13 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_13N1(uint16_t data)
{
    __UART_PUT_FRAME(13, data);
}


/** UART_PUT_FRAME_14N1 sends the data in 14N1 format
 *
 * @param[in] data 14 bits to send
 */
STATIC INLINE void UART_PUT_FRAME_14N1(uint16_t data)
{
    __UART_PUT_FRAME(14, data);
}


/** UART_WAIT_TX waits till tranmit register will be ready to send the next data portion
 *
 * @note: Acknowledges the AWD
 */
STATIC void UART_WAIT_TX(void)
{
    while (IO_GET(UART, TRB)!=0u) {
        WDG_conditionalAwdRefresh();
    }
}


/** UART_PUT_BREAK_BLOCKING sends the data break
 *
 * @param[in] break_length number of bits of break
 *
 * The function configures the UART to send the break field and waits for its end (and restores the UART's configuration)
 */
STATIC INLINE void UART_PUT_BREAK_BLOCKING(UART_MLS_ctrl_t break_length)
{
    uint16_t tmp = IO_GET(UART, MLS); /* save the previous config */
    UART_WAIT_TX();
    IO_SET(UART, MLS, break_length); /** Creating the 10bit break */
    IO_SET(UART, TRD, 0x80 >> break_length);
    UART_WAIT_TX(); /** Wait for tranmitter register is ready to make sure we will set the shifter to the previous mode */
    IO_SET(UART, MLS, tmp);
}

/** UART_PUT_RAW sends the data into the TX output register
 *
 * @param data - is raw value without start-stop
 * @note:
 *  1. Function is applicable for all data formats:
 *     UART_FORMAT_LSB, UART_FORMAT_MSB, UART_FORMAT_MPE and
 *     UART_FORMAT_RZ
 */
STATIC INLINE void UART_PUT_RAW(uint16_t data)
{
    IO_SET(UART, TRD, data);
}

/* Read functions */

/** UART_GET returns the current RX buffer received
 *
 * @return RX register
 */
STATIC INLINE uint16_t UART_GET(void)
{
    return IO_GET(UART, RRD);
}

/*********************** Function from lm_uart, Mulan2/3 ******************************************/

/** UART_TX_ENABLE Enable UART unit for transmit.
 *
 */
STATIC INLINE void UART_TX_ENABLE(void)
{
    IO_SET(UART, TRE, 1u);
}


/** UART_TX_DISABLE Disable UART unit for transmit.
 *
 */
STATIC INLINE void UART_TX_DISABLE(void)
{
    IO_SET(UART, TRE, 0u);
}


/** UART_RX_ENABLE Enable UART unit for receive.
 *
 * Enable UART unit before starting the module configuration.
 */
STATIC INLINE void UART_RX_ENABLE(void)
{
    IO_SET(UART, REE, 1u);
}


/** UART_RX_DISABLE Disable UART unit for receive.
 *
 * Disable UART unit before starting the module configuration.
 */
STATIC INLINE void UART_RX_DISABLE(void)
{
    IO_SET(UART, REE, 0u);
}


/** UART_ALL_DISABLE Disable UART unit.
 *
 */
STATIC INLINE void UART_ALL_DISABLE(void)
{
    IO_SET(UART, TRE, 0u, REE, 0u);
}


/** UART_LIN_LDC_ENABLE Enable LIN Mode - Duration Control checking.
 *
 * Enable counting the number of bit, compare UART_LFD and UART_LFC.
 */
STATIC INLINE void UART_LIN_LDC_ENABLE(void) \
    {
    IO_SET(UART, LDC, 1u);
    }


/** UART_LIN_LDC_DISABLE Disable LIN Mode -     .
 *
 * Disable counting the number of bit, compare UART_LFD and UART_LFC.
 */
STATIC INLINE void UART_LIN_LDC_DISABLE(void)
{
    IO_SET(UART, LDC, 0u);
}


/** UART_LIN_LSC_ENABLE Enable LIN Mode - Start bit Control checking.
 *
 * Enable receive state verifying before sending message.
 */
STATIC INLINE void UART_LIN_LSC_ENABLE(void)
{
    IO_SET(UART, LSC, 1u);
}


/** UART_LIN_LSC_DISABLE Disable LIN Mode - Start bit Control checking.
 *
 * Disable receive state verifying before sending message.
 */
STATIC INLINE void UART_LIN_LSC_DISABLE(void)
{
    IO_SET(UART, LSC, 0u);
}


/** UART_LIN_LBC_ENABLE Enable LIN Mode - Bit bit Control checking.
 *
 * Enable input and output data equality.
 */
STATIC INLINE void UART_LIN_LBC_ENABLE(void)
{
    IO_SET(UART, LBC, 1u);
}


/** UART_LIN_LBC_DISABLE Disable LIN Mode - Bit Control checking.
 *
 * Disable input and output data equality.
 */
STATIC INLINE void UART_LIN_LBC_DISABLE(void)
{
    IO_SET(UART, LBC, 0u);
}

/** UART_RESET_REGISTER Enables the UART unit and resets UART Registers to 0.
 *
 * Resets UART unit to UART mode, LIN Master mode is disabled.
 */
STATIC INLINE void UART_RESET_REGISTER(void)
{
    IO_HOST(UART, TRE) = 0u;
}

/** UART_TX_FORMAT Select the format for data transmit.
 *
 * Configures the bit scrambler.
 * @param[in] format is one of formats specified by UART_BSC_ctrl_t enum
 */
STATIC INLINE void UART_TX_FORMAT(UART_BSC_ctrl_t format)
{
    IO_SET(UART, BSC, format);
}


/** UART_FRAME_LENGTH Select the UART frame length including start and stop bit.
 *
 * @param[in] len - is one of lengths specified by UART_MLS_ctrl_t enum
 */
STATIC INLINE void UART_FRAME_LENGTH(UART_MLS_ctrl_t len)
{
    IO_SET(UART, MLS, len);
}


#endif

