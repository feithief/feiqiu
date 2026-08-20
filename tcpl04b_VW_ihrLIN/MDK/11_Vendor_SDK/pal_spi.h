/**
 *****************************************************************************
 * @brief   pal spi communication header file.
 *
 * @file    pal_spi.h
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

#ifndef __PAL_SPI_H__
#define __PAL_SPI_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief SPI INT FLAG
 */
#define SPI_INT_RX_FIFO_UF_FLAG             (SPI_IMR_RX_FIFO_UF_INT_MSK_MASK)
#define SPI_INT_RX_FIFO_OF_FLAG             (SPI_IMR_RX_FIFO_OF_INT_MSK_MASK)
#define SPI_INT_RX_FIFO_VLD_FLAG            (SPI_IMR_RX_FIFO_VLD_INT_MSK_MASK)
#define SPI_INT_TX_FIFO_UF_FLAG             (SPI_IMR_TX_FIFO_UF_INT_MSK_MASK)
#define SPI_INT_TX_FIFO_OF_FLAG             (SPI_IMR_TX_FIFO_OF_INT_MSK_MASK)
#define SPI_INT_TX_FIFO_FREE_FLAG           (SPI_IMR_TX_FIFO_FREE_INT_MSK_MASK)
#define SPI_INT_INS_RX_FINISH_FLAG          (SPI_IMR_INS_RX_FINISH_INT_MSK_MASK)
#define SPI_INT_CRC_ERR_FLAG                (SPI_IMR_CRC_ERR_INT_MSK_MASK)
#define SPI_INT_TX_CONF_ERR_FLAG            (SPI_IMR_TX_CONF_ERR_INT_MSK_MASK)


/**
 * @brief  spi bus enumeration
 */
typedef enum
{
    SPI_BUS_0,
    SPI_BUS_MAX,
} spi_bus_e;

/**
  * @brief  tcan mode enumeration
  */
typedef ll_spi_mode_e spi_mode_e;

void pal_spi_init(spi_bus_e bus, spi_mode_e mode, ISR_FUNC_CALLBACK callback);
void pal_spi_deinit(spi_bus_e bus);
void pal_spi_master_write(spi_bus_e bus, uint8_t *buffer, uint16_t length);
bool pal_spi_master_read(spi_bus_e bus, uint8_t *buffer, uint16_t length);


#ifdef __cplusplus
}
#endif
#endif /*__PAL_SPI_H__*/
