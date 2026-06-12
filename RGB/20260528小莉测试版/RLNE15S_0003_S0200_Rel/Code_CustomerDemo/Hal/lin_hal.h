/******************************************************************************
*                _  _               ___             _                         *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*    ihr GmbH                                                                 *
*    Airport Boulevard B210                                                   *
*    77836 Rheinmünster - Germany                                             *
*    http://www.ihr.de                                                        *
*    Phone +49(0) 7229-18475-0                                                *
*    Fax   +49(0) 7229-18475-11                                               *
*                                                                             *
*******************************************************************************
*                                                                             *
* (c) Alle Rechte bleiben bei IHR GmbH, auch fuer den Fall von Schutzrechts-  *
* anmeldungen. Jede Verfuegungsbefugnis, wie Kopier- und Weitergaberecht      *
* bleibt bei IHR GmbH.                                                        *
*                                                                             *
* (c) All rights reserved by IHR GmbH including the right to file             *
* industrial property rights. IHR GmbH retains the sole power of              *
* disposition such as reproduction or distribution.                           *
*                                                                             *
********************     Workfile:      lin_hal.h        **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  Infineon Hardware Abstraction Layer                  *
*  FILE-DESCRIPTION:     All Hardware Access Routines for LIN Driver          *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 1737        $                                 *
*       Responsible:     B.Roegl                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2023-02-22#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_hal.h
@brief Header file of lin_hal.c
*/

#ifndef LIN_HW_DRIVER_H /* to interpret header file only once */
#define LIN_HW_DRIVER_H

/* ===========================================================================
 *  Header Files
 * ==========================================================================*/

#include "variants.h"
#include "device.h"
#include "genLinConfig.h"

/* ===========================================================================
 *  Constants
 * ==========================================================================*/

#ifndef LIN_FRAME_TIMEOUT
   #define LIN_FRAME_TIMEOUT       LIN_AUTOBAUD_TIMEOUT
#endif /* ifndef LIN_FRAME_TIMEOUT */

/* All UART control and access registers*/
#define LIN_UART_RX_BUFFER    (LIN->UART_RXBUF.bit.RXDATA)
#define LIN_UART_TX_BUFFER    (LIN->UART_TXBUF.bit.TXDATA)
#define LIN_UART_CONTROL      (LIN->UART_SCON.reg)
#define LIN_UART_MODE0        (LIN->UART_SCON.bit.SM0)
#define LIN_UART_MODE1        (LIN->UART_SCON.bit.SM1)
#define LIN_UART_RUN          (LIN->UART_SCON.bit.REN)
#define LIN_UART_9BITREC      (LIN->UART_RXBUF.bit.RB8)
#define LIN_UART_9BITTRANS    (LIN->UART_TXBUF.bit.TB8)

#define LIN_UART_BCON_PRE     (LIN->UART_BCON.bit.BR_PRE)
#define LIN_UART_BCON_FDSEL   (LIN->UART_BCON.bit.BG_FD_SEL)
#define LIN_UART_BCON_BG      (LIN->UART_BCON.bit.BG_BR_VALUE)
#define LIN_UART_BCON_RUN     (LIN->UART_BCON.bit.BR_R)

#define LIN_UART_IR_SET_EN    (CPU->NVIC_ISER0.bit.SETENA)
#define LIN_UART_IR_CLR_EN    (CPU->NVIC_ICER0.bit.CLRENA)
#define LIN_UART_IR_PENDING   (CPU->NVIC_ISPR0.bit.SETPEND)
#define LIN_UART_IR_PRIO      (CPU->NVIC_IPR0.bit.PRI_N2)
#define LIN_UART_IR_EN        (LIN->LIN_IEN.bit.RIEN)
#define LIN_UART_IRTX_EN      (LIN->LIN_IEN.bit.TIEN)

#define LIN_UART_STATUS       (LIN->LIN_IS.reg)
#define LIN_UART_CLR_STATUS   (LIN->LIN_ISC.reg)
#define LIN_UART_BR_SYN_DET   (LIN->UART_LINCON.bit.BGSEL)
#define LIN_UART_BR_EN        (LIN->UART_LINCON.bit.BREN)
#define LIN_UART_ERRSYNEN     (LIN->LIN_IEN.bit.ERRSYNEN)
#define LIN_UART_EOFSYNEN     (LIN->LIN_IEN.bit.EOFSYNEN)

#define LIN_UART_CLR_BRK      (LIN->LIN_ISC.bit.BRKCLR)
#define LIN_UART_CLR_EOFSYN   (LIN->LIN_ISC.bit.EOFSYNCLR)
#define LIN_UART_CLR_ERRSYN   (LIN->LIN_ISC.bit.ERRSYNCLR)

#ifdef AUTOBAUD_ENABLED
   /* Timer 2 is needed for baudrate synchronization when using UART 1 */
   #define LIN_TIMER_PRE                    (LIN->T2_MOD.bit.T2PRE)
   #define LIN_TIMER_PRE_EN                 (LIN->T2_MOD.bit.PREN)
   #define LIN_TIMER_COUNTER_EN             (LIN->T2_MOD.bit.DCEN)
   #define LIN_TIMER_EDGE_SELECT            (LIN->T2_MOD.bit.T2REGS)
   #define LIN_TIMER_EXT_START              (LIN->T2_MOD.bit.T2RHEN)
   #define LIN_TIMER_CAP_EDGE_SELECT        (LIN->T2_MOD.bit.EDGESEL)
   #define LIN_TIMER_CAP_REL                (LIN->T2_CON.bit.CP_RL2)
   #define LIN_TIMER_T_C_SEL                (LIN->T2_CON.bit.C_T2)
   #define LIN_TIMER_EXT_EN                 (LIN->T2_CON.bit.EXEN2)
   #define LIN_TIMER_EXT_IR_EN              (LIN->LIN_IEN.bit.EXF2EN)
   #define LIN_TIMER_INT_IR_EN              (LIN->LIN_IEN.bit.TF2EN)
   #define LIN_TIMER_RUN                    (LIN->T2_CON.bit.TR2)
   #define LIN_TIMER_HIGH                   (LIN->T2_CNT.bit.T2H)
   #define LIN_TIMER_LOW                    (LIN->T2_CNT.bit.T2L)
   #define LIN_TIMER_CAP_H                  (LIN->T2_RC.bit.RCH2)
   #define LIN_TIMER_CAP_L                  (LIN->T2_RC.bit.RCL2)
   #define LIN_TIMER_EX_IR_CLR_FLAG         (LIN->LIN_ISC.bit.EXF2CLR)
   #define LIN_TIMER_OVERFLOW_IR_CLR_FLAG   (LIN->LIN_ISC.bit.TF2CLR)
#endif /* end #ifdef AUTOBAUD_ENABLED */

#define LINST_MASK_SYNBRK       0x4030u
#define LINST_FLAG_EOFSYN       0x20u
#define LINST_FLAG_ERRSYN       0x10u
#define LINST_FLAG_BRK          0x4000u
#define LIN_IR_SET_EN_MASK      4u
#define LIN_IR_SET_PEN_MASK     4u

#define BREAK_FIELD_k  0x0u
#define SYNC_FIELD_k    0x55u

#ifndef CHECKSUM_NEXT
   #define CHECKSUM_NEXT 2u
#endif

#define lin_hal_tx_char(x)            (LIN_UART_TX_BUFFER = (x))
#define lin_hal_is_byte_received()    (lin_hal_status_g.byte_received == L_SET)
#define lin_hal_reset_byte_received() (lin_hal_status_g.byte_received = L_RESET)

#define lin_hal_set_rx_pid(pid)       (lin_temp_buffer.frame.PID = (pid))
#define lin_hal_set_tx_pid(pid)       (lin_temp_buffer.frame.PID = (pid))

#if (defined J2602_PROTOCOL)
/**
   @brief  This is the J2602 Statusbyte
   @note   This byte is divided into two parts, the Application Information
           Field (Bit 0-4) and the  Error Field (Bit 5-7)
   @par
         Error Field Err[2:0]
         These bits report the four defined Error States that have been seen
         by the Slave node since an error state has been last reported to the
         Master node. The four states are "sticky" which means that they are
         retained until they are successfully reported to the Master node in a
         message without any detected errors, after which they are automatically
         cleared. Only one state may be reported at a time. There is an inherent
         hierarchy to the states, with the highest latched state reported first
         Bit 7 also serves the purpose of the Response_Error bit from LIN Rev.2.0
         Section 6.3
         Err2   Err1   Err0   Fault State               Priority
         0      0      0      No Detected Fault         0
         0      0      1      Reset                     1
         0      1      0      Reserved                  2
         0      1      1      Reserved                  3
         1      0      0      Data Error                4
         1      0      1      Checksum Error            5
         1      1      0      Byte Field Framing Error  6
         1      1      1      ID Parity Error           7 (highest)
   @par
         No Detected Fault
         A slave node shall indicate this state whenever none of the detectable fault
         states are active. This is the default state of the LIN device.
   @par
         Reset
         A slave node shall set this state upon interruption and resumption of power,
         after a watchdog timeout, or after receipt of a Reset Command. Note that for
         those devices that require configuration and store the configuration information
         in volatile memory, this state indicates that the device is currently unconfigured
         and requires configuration. For those devices that use non-volatile memory for
         configuration information storage, then the state indicates configuration is
         required the first time the part is powered on and only indicates a reset from then
         on.
   @par
         Data Error
         A slave or master node that is transmitting a bit on the bus shall also monitor the
         bus. A Data Error shall be detected when the bit or bye that is received is different
         from the bit or byte value that is transmitted.
         A slave node that is receiving shall detect a Data Error when the Data in the fixed
         form Sync Byte is received incorrectly, i.e. is not $55. A slave node that performs
         autobauding shall detect this error but is not required to set the error bit.
   @par
         Checksum Error
         A Checksum Error shall be detected if the inverted modulo-256 sum over all received
         data bytes and the protected identifier (when using enhanced checksum) and the received
         checksum byte field does not result in $FF.
   @par
         Byte Field Framing Error
         The receiver shall detect a Byte Field Framing Error if the ninth bit after a valid start
         bit is dominant.
   @par
         ID Parity Error
         The receiver shall detect an ID Parity Error if the received ID parity (bits 6 & 7) does
         not match the ID parity calculated from the equations in the LIN 2.0 Protocol Specification
         based on the received identifier (bits 0-5)
   @par
         Application Information Field (Bits 4-0)
   @par
         The bit APINFO 4 shall be used to indicate when the application requires attention from the
         Master Device. This shall be indicated by setting te bit to "1".
         When a Reset state is indicated by the Error Field when this bit is "1" is shall
         indicate that the device needs to be configured.
         The action taken by the Master when this bit is "1" and no Reset state ist indicated
         shall be documented (e.g. NCF, datasheet,...) and may include loading a special
         schedule table which queries the Application as to its status.
         The lower four bits of the application information field (APINFO[3:0]) are not
         explicitly defined, since their structure will be dependent on the specific application
         and implementation of the slave node. This field may be implemented as discrete status
         bits, a state encoded bit field, or a combination of the two. The clearing mechanism shall
         be documented (e.g. in the NCF, datasheet, ...)
         A value of zero (00000b) indicates that no application information currently needs to be
         reported. All other encodings are
*/
#ifndef J_2602_STATUSBYTE
#define J_2602_STATUSBYTE   1

#define J2602_PID_ERROR         0x10u /* 0b111 */
#define J2602_FRAMING_ERROR     0x08u /* 0b110 */
#define J2602_CHECKSUM_ERROR    0x04u /* 0b101 */
#define J2602_DATA_ERROR        0x02u /* 0b100 */
#define J2602_RESET_STATUS      0x01u /* 0b001 */
#endif /* end #ifndef J_2602_STATUSBYTE */
#endif /* (defined J2602_PROTOCOL) */

/* ===========================================================================
 *  Structures
 * ==========================================================================*/


/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

void lin_hal_wait_for_break(void);
void l_hal_cyclic_call(void);
void l_frame_timeout_control(void);
void l_hal_txchecksum_calculation(void);
void l_hal_rx_state_machine(void);
void lin_hal_ISR(void);
void lin_hal_tx_wake_up(void);
void lin_hal_init_uart(void);

extern void lin_hal_init (void);
extern void lin_hal_rx_response (l_u8 l_type, l_u8 l_len);
extern void lin_hal_tx_response (l_u8 l_type, const l_u8 l_data[], l_u8 l_len);

#ifdef AUTOBAUD_ENABLED
void lin_hal_T2_ISR(void);
void l_autobaud_control(void);
void l_hal_baudrate_control(void);
#else
#define l_autobaud_control()  do {} while(0)
#define l_hal_baudrate_control()   do { } while (0u)
#endif /* #ifdef AUTOBAUD_ENABLED */

#if (defined J2602_PROTOCOL)
#ifdef GM_SPEC_ADAPTION
void lin_hal_set_resp_error (l_u8 j2602_fault);
#else
static void lin_hal_set_resp_error (l_u8 j2602_fault);
#endif /* #ifdef GM_SPEC_ADAPTION */
#else
/* Prototype for LIN protocols */
void lin_hal_set_resp_error (void);
#endif /* end #if (defined J2602_PROTOCOL) */

#if ((defined LIN_MASTER)||(defined J2602_MASTER))
void lin_hal_tx_header (l_u8 l_pid);
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */

/* ===========================================================================
 *  Variables
 * ==========================================================================*/

extern LIN_hal_stat_reg_t lin_hal_status_g;
extern t_lin_temp_frame_buffer lin_temp_buffer;
extern LIN_SLAVE_STATE_t lin_slave_state_g;
extern l_u8 lin_ab_timeout_timer;
extern l_u8 lin_frm_timeout_timer;
extern t_lin_timeout_ctrl lin_timeout_ctrl;
extern l_u8 l_txchk_byte_nr;
void ff(void);
extern volatile l_u8 lin_ab_t2_state;

#ifdef AUTOBAUD_ENABLED
extern volatile l_u16 lin_temp_word;
#endif /* end #ifdef AUTOBAUD_ENABLED */

#endif /* end #ifndef LIN_HW_DRIVER_H */
