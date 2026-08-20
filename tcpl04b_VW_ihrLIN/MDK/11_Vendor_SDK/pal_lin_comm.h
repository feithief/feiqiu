/**
 *****************************************************************************
 * @brief   pal lin communication header file.
 *
 * @file    pal_lin_comm.h
 * @author  AE/FAE team
 * @date    2024.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __PAL_LIN_COMM_H__
#define __PAL_LIN_COMM_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief LIN INT FLAG
 */
#define LIN_INT_RX_1BYTE_FLAG           (SCI_INT_RX_1BYTE_DONE >> SCI_INT_OFFSET)
#define LIN_INT_RX_DONE_FLAG            (SCI_INT_RX_DONE >> SCI_INT_OFFSET)
#define LIN_INT_RX_PID_DONE_FLAG        (SCI_INT_RX_PID_DONE >> SCI_INT_OFFSET)
#define LIN_INT_RX_CHKPTY_ERROR_FLAG    (SCI_INT_RX_PTY_CHK_ERR >> SCI_INT_OFFSET)
#define LIN_INT_RX_CHKSUM_ERROR_FLAG    (SCI_INT_RX_CHKSUM_ERR >> SCI_INT_OFFSET)
#define LIN_INT_STOP_BIT_ERROR_FLAG     (SCI_INT_RX_STP_ERR >> SCI_INT_OFFSET)
#define LIN_INT_RX_FIFO_FULL_FLAG       (SCI_INT_RX_FIFO_FULL >> SCI_INT_OFFSET)
#define LIN_INT_RX_FIFO_OVF_FLAG        (SCI_INT_RX_FIFO_OVF_ERR >> SCI_INT_OFFSET)
#define LIN_INT_TX_DONE_FLAG            (SCI_INT_TX_DONE >> SCI_INT_OFFSET)
#define LIN_INT_TX_FIFO_EMPTY_FLAG      (SCI_INT_TX_FIFO_EMPTY >> SCI_INT_OFFSET)

#define LIN_INT_BREAK_DET_FLAG          (SCI_INT_BRK_DET >> SCI_INT_OFFSET)
#define LIN_INT_SYNC_DET_FLAG           (SCI_INT_SYNC_DET >> SCI_INT_OFFSET)
#define LIN_INT_SYNC_VALUE_ERROR_FLAG   (SCI_INT_SYNC_VAL_ERR >> SCI_INT_OFFSET)
#define LIN_INT_TX_PID_DONE_FLAG        (SCI_INT_TX_PID_DONE >> SCI_INT_OFFSET)
#define LIN_INT_TX_RX_CONFLICT_FLAG     (SCI_INT_TX_RX_CONF >> SCI_INT_OFFSET)
#if (defined (__TCPL01X__) || defined (__TCPL03X__) || defined (__TCPL04X__))
#define LIN_INT_SLV_SELECT_FLAG         (SCI_INT_SLV_SELECTED >> SCI_INT_OFFSET)
#define LIN_INT_AUTOADDR_DONE_FLAG      (SCI_INT_AUTO_ADDR_DONE >> SCI_INT_OFFSET)
#elif defined (__TCPL08X__) /* defined (__TCPL08X__)  */
#define LIN_INT_SLV_SELECT_FLAG         (LIN_INT_SLV_SELECTED << LIN_INT_OFFSET)
#define LIN_INT_AUTOADDR_DONE_FLAG      (LIN_INT_AUTO_ADDR_DONE << LIN_INT_OFFSET)
#endif /* defined (__TCPL08X__) */
#define LIN_INT_TX_1BYTE_FLAG           (SCI_INT_TX_1BYTE_DONE >> SCI_INT_OFFSET)

#if defined (__TCPL01X__)
#define LIN_INT_RX_TIMEOUT_FLAG         (SCI_INT_RX_TIMEOUT >> SCI_INT_OFFSET)
#define LIN_INT_SLV_TX_BREAK_DONE_FLAG  (SCI_INT_SLV_TX_BRK_DONE << SCI_INT_OFFSET1)
#define LIN_INT_MP_MODE_ADDR_FLAG       (SCI_INT_MP_MODE_ADDR << SCI_INT_OFFSET1)
#define LIN_INT_TX_COL_DET_ERROR_FLAG   (SCI_INT_TX_COL_DET_ERR >> SCI_INT_OFFSET)
#define LIN_INT_IDLE_DET_FLAG           (SCI_INT_BUS_IDLE_DET << SCI_INT_OFFSET1) //bit=20
#define LIN_INT_WAKEUP_DET_FLAG         (SCI_INT_WAKE_UP_DET << SCI_INT_OFFSET1)
#define LIN_INT_AUTOADDR_START_FLAG     (SCI_INT_AUTO_ADDR_START << SCI_INT_OFFSET1)
#define LIN_INT_AUTOADDR_1BIT_FLAG      (SCI_INT_ADDR_1BIT << SCI_INT_OFFSET1)
#elif defined (__TCPL03X__) || defined (__TCPL04X__)
#define LIN_INT_SLV_TX_BREAK_DONE_FLAG  (SCI_INT_SLV_TX_BRK_DONE >> SCI_INT_OFFSET)
#define LIN_INT_MP_MODE_ADDR_FLAG       (SCI_INT_MP_MODE_ADDR >> SCI_INT_OFFSET)
#define LIN_INT_SHORT_TO_GND_FLAG       (SCI_INT_SHORT_TO_GND >> SCI_INT_OFFSET)
#define LIN_INT_RX_VALID_FLAG           (SCI_INT_RX_VALID >> SCI_INT_OFFSET)
#endif

/**
  * @brief  lin abort type enumeration
  */
typedef enum
{
    LIN_BUS_0 = 0,
#if (defined(__TCPL03X__) || defined(__TCPL04X__) || defined( __TCPL08X__))
    LIN_BUS_1,
#endif
    LIN_BUS_MAX,
} lin_bus_e;

/**
  * @brief  lin mode enumeration
  */
typedef enum
{
    LIN_MODE_SLV = 0,
    LIN_MODE_MASTER,
    LIN_MODE_MAX,
} lin_mode_e;

/**
  * @brief  lin parity enumeration
  */
typedef enum
{
    LIN_PARITY_MAKE,    /**< make parity bits */
    LIN_PARITY_CHECK,   /**< check parity bits */
} lin_parity_type_e;

/**
  * @brief  lin abort type enumeration
  */
typedef enum
{
    LIN_ABORT_TYPE_NULL = (0),
    LIN_ABORT_TYPE_TX = (0x01 << 0),
    LIN_ABORT_TYPE_RX = (0x01 << 1),
} lin_abort_type_e;

/**
  * @brief  lin read type enumeration
  */
typedef enum
{
    LIN_READ_TYPE_PID,
    LIN_READ_TYPE_FIFO,
} lin_read_type_e;

/**
  * @brief  lin read type enumeration
  */
typedef enum
{
    SWAA_STATUS_0,
    SWAA_STATUS_1,
    SWAA_STATUS_2,
    SWAA_STATUS_3,
    SWAA_STATUS_MAX,
} lin_swaa_status_type_e;

void pal_lin_init(lin_bus_e bus, lin_mode_e mode, uint32_t baudrate, ISR_FUNC_CALLBACK callback);
void pal_lin_deinit(lin_bus_e bus);
void pal_lin_rx_response(lin_bus_e bus, uint8_t pid, uint8_t *buffer, uint8_t msg_length);
bool pal_lin_tx_response(lin_bus_e bus, uint8_t pid, uint8_t *buffer, uint8_t msg_length);
void pal_lin_tx_4byte(lin_bus_e bus, uint8_t *buffer, uint8_t msg_length);
void pal_lin_tx_header(lin_bus_e bus, uint8_t pid);
void pal_lin_snpd_enter(lin_snpd_type_e type, uint16_t *aa_cur_th);
void pal_lin_snpd_exit(void);
void pal_lin_snpd_offset_set(bool sw_start);
void pal_lin_snpd_ready(void);
uint8_t pal_lin_parity_calib(lin_parity_type_e type, uint8_t pid);
uint8_t pal_lin_checksum_calib(uint8_t pid, uint8_t *buffer);
uint16_t pal_lin_aa_raw_code_get(int16_t *buffer, uint16_t length);
uint16_t pal_lin_swaa_raw_code_calc(int16_t *buffer, uint8_t length);
void pal_lin_swaa_steps_config(lin_bus_e bus, lin_swaa_status_type_e status);
void pal_lin_swaa_adc_sw_ctrl(bool en);
void pal_lin_swaa_config(lin_bus_e bus, bool en);
void pal_lin_swaa_ready_set(lin_bus_e bus);
void pal_lin_abort_handle(lin_bus_e bus, lin_abort_type_e type);
void pal_lin_read_byte(lin_bus_e bus, lin_read_type_e type, uint8_t *byte);
void pal_lin_autobaudrate_check(lin_bus_e bus);

#ifdef __cplusplus
}
#endif
#endif /*__PAL_LIN_COMM_H__*/
