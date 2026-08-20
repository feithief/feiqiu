/**
 *****************************************************************************
 * @brief   pal uart communication header file.
 *
 * @file    pal_uart.h
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

#ifndef __PAL_UART_H__
#define __PAL_UART_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief UART INT FLAG
 */
#define UART_INT_RX_1BYTE_FLAG           (SCI_INT_RX_1BYTE_DONE >> SCI_INT_OFFSET)
#define UART_INT_RX_DONE_FLAG            (SCI_INT_RX_DONE >> SCI_INT_OFFSET)
#define UART_INT_RX_FIFO_FULL_FLAG       (SCI_INT_RX_FIFO_FULL >> SCI_INT_OFFSET)
#define UART_INT_RX_FIFO_OVF_FLAG        (SCI_INT_RX_FIFO_OVF_ERR >> SCI_INT_OFFSET)
#define UART_INT_TX_DONE_FLAG            (SCI_INT_TX_DONE >> SCI_INT_OFFSET)
#define UART_INT_TX_FIFO_EMPTY_FLAG      (SCI_INT_TX_FIFO_EMPTY >> SCI_INT_OFFSET)
#define UART_INT_TX_1BYTE_FLAG           (SCI_INT_TX_1BYTE_DONE >> SCI_INT_OFFSET)


/**
  * @brief  uart bus enumeration
  */
typedef enum
{
    UART_BUS_0 = 0,
#if (defined(__TCPL03X__) || defined(__TCPL04X__) || defined( __TCPL08X__))
    UART_BUS_1,
#endif
    UART_BUS_MAX,
} uart_bus_e;


void pal_uart_init(uart_bus_e bus, uint32_t baudrate, ISR_FUNC_CALLBACK callback);
void pal_uart_deinit(uart_bus_e bus);
void pal_uart_transmit(uart_bus_e bus, uint8_t *buffer, uint8_t length);
void pal_uart_receive(uart_bus_e bus, uint8_t *buffer, uint8_t length);
#ifdef __cplusplus
}
#endif
#endif /*__PAL_UART_H__*/