/**
 *****************************************************************************
 * @brief   pal osp header file.
 *
 * @file    pal_osp.h
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

#ifndef __PAL_OSP_H__
#define __PAL_OSP_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CFG_SUPPORT_OSP_DMA
#define CFG_SUPPORT_OSP_DMA                       (1)
#endif /* CFG_SUPPORT_OSP_DMA */

#define OSP_INT_DMA_TX_DONE_FLAG       (DMA_INT_TX_DONE << DMA_INT_OFFSET)
#define OSP_INT_DMA_RX_DONE_FLAG       (DMA_INT_RX_DONE << DMA_INT_OFFSET)


#define OSP_FRAME_LENGTH        (12U) /* OSP frame length: 3 bytes header + 8 bytes data + 1 byte crc */
/**
 * @brief OSP INT FLAG
 */

/**
  * @brief  osp bus enumeration
  */
typedef enum
{
    OSP_BUS_0 = 0,
    OSP_BUS_MAX,
} osp_bus_e;

/**
  * @brief  lin mode enumeration
  */
typedef ll_osp_mode_e osp_mode_e;
typedef ll_osp_direction_e osp_direction_e;

/**
 * @brief  osp msg frame struct
 */
typedef struct
{
    uint8_t data[OSP_FRAME_LENGTH];  /**< preamble 4 * bits, Fixed to 0xA0 */
    /**< addr 10 * bits, BROADCAST:0x000, addr:0x001~0x3EF, RESERVED:0x3F0~0x3FR-RESERVED, INIT_VALUE:0x3FF */
    /**< psi 3 * bits, 0x0-0 ,0x1-1 0x2-2 0x3-3, 0x4-4, 0x5-reserved, 0x6-6, 0x7-8 */
    /**< cmd 7 * bits */
    uint8_t length;
    osp_direction_e direction;
} osp_msg_packet_t  __attribute__((aligned(4)));


/**
 * @brief  osp msg frame struct
 */
typedef struct
{
    uint16_t addr;       /**< 10 * bits, BROADCAST:0x000, addr:0x001~0x3EF, RESERVED:0x3F0~0x3FR-RESERVED, INIT_VALUE:0x3FF */
    uint8_t length;     /**< 3 * bits, 0x0-0 ,0x1-1 0x2-2 0x3-3, 0x4-4, 0x5-reserved, 0x6-6, 0x7-8 */
    uint8_t cmd;        /**< 7 * bits */
    uint8_t msg[8];     /**< 8 * bytes dlength */
    osp_direction_e direction;
    // uint8_t crc;
    bool ready;
} osp_msg_frame_t __attribute__((aligned(4)));


void pal_osp_init(osp_bus_e bus, osp_mode_e mdoe, ISR_FUNC_CALLBACK callback);
void pal_osp_dma_tx_config(osp_bus_e bus, ISR_FUNC_CALLBACK callback);
void pal_osp_deinit(osp_bus_e bus);
bool pal_osp_transmit(osp_bus_e bus, osp_msg_packet_t *packet);
bool pal_osp_dma_transmit(osp_bus_e bus, osp_msg_packet_t *packet);
bool pal_osp_receive(osp_bus_e bus, osp_msg_packet_t *packet);
bool pal_osp_dma_receive(osp_bus_e bus, osp_msg_packet_t *packet);
bool pal_osp_fifo_read(osp_bus_e bus, osp_msg_packet_t *packet);
bool pal_osp_dma_message_get(osp_bus_e bus,  osp_msg_packet_t *packet);
bool pal_osp_message_head_get(osp_bus_e bus, osp_msg_frame_t *frame);
#ifdef __cplusplus
}
#endif
#endif /*__PAL_OSP_H__*/
