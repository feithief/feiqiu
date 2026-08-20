/**
 *****************************************************************************
 * @brief   pal i2c communication file.
 *
 * @file    pal_i2c_comm.h
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

#ifndef __PAL_I2C_COMM_H__
#define __PAL_I2C_COMM_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Callback for all peripherals which supports I2C features for master mode */
typedef void (*i2c_master_callback_t)(void);

/**
  * @brief  i2c type enumeration
  */
typedef enum
{
    I2C_BUS_0 = 0,
    I2C_BUS_MAX,
} i2c_bus_e;

/**
  * @brief  defines the operation mode of the i2c pal
  */
typedef enum
{
    I2C_PAL_STANDARD_MODE      = 0x0U,   /* Standard-mode (Sm), 100 kbit/s        */
    I2C_PAL_FAST_MODE          = 0x1U,   /* Fast-mode (Fm), 400 kbit/s            */
} i2c_operating_mode_e;

/**
  * @brief  i2c master struct define
  */
typedef struct
{
    uint16_t slave_addr;                 /* Slave address, 7-bit or 10-bit        */
    bool is_10_bit_addr;                 /* Selects 7-bit or 10-bit slave address */
    i2c_operating_mode_e operating_mode; /* I2C Operating mode                    */
    i2c_master_callback_t callback;
} i2c_master_t;

typedef struct
{
    uint16_t slave_addr;
} i2c_control_context_t;

/**
  * @brief  i2c opt
  */
void pal_i2c_master_init(i2c_bus_e bus, const i2c_master_t *config);
void pal_i2c_master_send_data(i2c_bus_e bus, uint8_t *tx_buff, uint32_t tx_size);
void pal_i2c_master_receive_data(i2c_bus_e bus, uint8_t *rx_buff, uint32_t rx_size);
void pal_i2c_deinit(i2c_bus_e bus);

#ifdef __cplusplus
}
#endif
#endif /* __PAL_I2C_COMM_H__ */