/**
 *****************************************************************************
 * @brief   pal tcan communication header file.
 *
 * @file    pal_tcan_comm.h
 * @author  AE/FAE team
 * @date    2026.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2026 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __PAL_TCAN_COMM_H__
#define __PAL_TCAN_COMM_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief TCAN INT FLAG
 */
#define TCAN_INT_RX_1BYTE_FLAG           (SCI_INT_RX_1BYTE_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_RX_DONE_FLAG            (SCI_INT_RX_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_RX_PID_DONE_FLAG        (SCI_INT_RX_PID_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_RX_CHKPTY_ERROR_FLAG    (SCI_INT_RX_PTY_CHK_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_RX_CHKSUM_ERROR_FLAG    (SCI_INT_RX_CHKSUM_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_STOP_BIT_ERROR_FLAG     (SCI_INT_RX_STP_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_RX_FIFO_FULL_FLAG       (SCI_INT_RX_FIFO_FULL >> SCI_INT_OFFSET)
#define TCAN_INT_RX_FIFO_OVF_FLAG        (SCI_INT_RX_FIFO_OVF_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_TX_DONE_FLAG            (SCI_INT_TX_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_TX_FIFO_EMPTY_FLAG      (SCI_INT_TX_FIFO_EMPTY >> SCI_INT_OFFSET)

#define TCAN_INT_BREAK_DET_FLAG          (SCI_INT_BRK_DET >> SCI_INT_OFFSET)
#define TCAN_INT_SYNC_DET_FLAG           (SCI_INT_SYNC_DET >> SCI_INT_OFFSET)
#define TCAN_INT_SYNC_VALUE_ERROR_FLAG   (SCI_INT_SYNC_VAL_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_TX_PID_DONE_FLAG        (SCI_INT_TX_PID_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_TX_RX_CONFLICT_FLAG     (SCI_INT_TX_RX_CONF >> SCI_INT_OFFSET)
#define TCAN_INT_TX_1BYTE_FLAG           (SCI_INT_TX_1BYTE_DONE >> SCI_INT_OFFSET)

#define TCAN_INT_RX_PID2_DONE_FLAG      (SCI_INT_RX_PID2_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_RX_PID2_PTY_ERR_FLAG   (SCI_INT_RX_PID2_PTY_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_RX_CRC_ERR_FLAG        (SCI_INT_RX_CRC_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_RX_ACK_DONE_FLAG       (SCI_INT_RX_ACK_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_RX_CMD_DONE_FLAG       (SCI_INT_RX_CMD_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_RX_CMD_PTY_ERR_FLAG    (SCI_INT_RX_CMD_PTY_ERR >> SCI_INT_OFFSET)
#define TCAN_INT_TX_ACK_DONE_FLAG       (SCI_INT_TX_ACK_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_TX_PID2_DONE_FLAG      (SCI_INT_TX_PID2_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_TX_CMD_DONE_FLAG       (SCI_INT_TX_CMD_DONE >> SCI_INT_OFFSET)
#define TCAN_INT_DMA_TX_DONE_FLAG       (DMA_INT_TX_DONE << DMA_INT_OFFSET)
#define TCAN_INT_DMA_RX_DONE_FLAG       (DMA_INT_RX_DONE << DMA_INT_OFFSET)

/**
 * @brief  tcan bus enumeration
 */
typedef enum
{
#if defined(__TCPL08X__)
    TCAN_BUS_0 = 1,
#endif
    TCAN_BUS_MAX,
} tcan_bus_e;

/**
  * @brief  tcan mode enumeration
  */
typedef enum
{
    TCAN_MODE_SLV = 0,
    TCAN_MODE_MASTER,
    TCAN_MODE_MAX,
} tcan_mode_e;

/**
  * @brief  tcan parity enumeration
  */
typedef enum
{
    TCAN_PARITY_MAKE,    /**< make parity bits */
    TCAN_PARITY_CHECK,   /**< check parity bits */
} tcan_parity_type_e;

/**
 * @brief  tcan abort type enumeration
 */
typedef enum
{
    TCAN_ABORT_TYPE_NULL = (0),
    TCAN_ABORT_TYPE_TX = (0x01 << 0),
    TCAN_ABORT_TYPE_RX = (0x01 << 1),
} tcan_abort_type_e;

/**
 * @brief  tcan pid read type enumeration
 */
typedef enum
{
    TCAN_READ_TYPE_PID,
    TCAN_READ_TYPE_FIFO,
} tcan_read_type_e;

/**
 * @brief  tcan msg frame struct
 */
typedef struct
{
    uint8_t msg[96];
    uint8_t addr;
    uint8_t cmd;
    uint8_t length;
} tcan_msg_frame_t __attribute__((aligned(4)));

void pal_tcan_init(tcan_bus_e bus, tcan_mode_e mode, uint32_t baudrate, ISR_FUNC_CALLBACK callback);
void pal_tcan_deinit(tcan_bus_e bus);
void pal_tcan_rx_response(tcan_bus_e bus, uint16_t pid, uint8_t *buffer, uint8_t msg_length);
bool pal_tcan_tx_response(tcan_bus_e bus, tcan_msg_frame_t *msg_frame);
void pal_tcan_tx_4byte(tcan_bus_e bus, uint8_t *buffer, uint8_t msg_length);
uint8_t pal_tcan_checksum_calib(uint16_t pid, uint8_t *buffer);
void pal_tcan_abort_handle(tcan_bus_e bus, tcan_abort_type_e type);
void pal_tcan_read_byte(tcan_bus_e bus, tcan_read_type_e type, uint8_t *byte);
void pal_tcan_autobaudrate_check(tcan_bus_e bus);
void pal_tcan_slv_addr_config(tcan_bus_e bus, uint8_t slv_addr);
void pal_tcan_dma_tx_config(tcan_bus_e bus, ISR_FUNC_CALLBACK callback);
bool pal_tcan_dma_transmit(tcan_bus_e bus, tcan_msg_frame_t *msg_frame);
bool pal_tcan_dma_response(tcan_bus_e bus, tcan_msg_frame_t *msg_frame);
bool pal_tcan_dma_receive(tcan_bus_e bus, tcan_msg_frame_t *msg_frame);

#ifdef __cplusplus
}
#endif
#endif /*__PAL_TCAN_COMM_H__*/
