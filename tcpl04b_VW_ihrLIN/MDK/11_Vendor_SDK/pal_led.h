/**
 *****************************************************************************
 * @brief   pal led header file.
 *
 * @file    pal_led.h
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

#ifndef __PAL_LED_H__
#define __PAL_LED_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*LED_FUNC_CALLBACK)(uint32_t);

/**
  * @brief  rgb channel struct
  */
typedef struct
{
    uint8_t rgb[LED_TYPE_MAX];
} rgb_channel_t;

/**
  * @brief  led ctrl status struct
  */
typedef struct
{
    uint8_t static_sample_valid : 1;
    uint8_t light_sts : 1;
    uint8_t reserved : 6;
} led_ch_status_t;

/**
  * @brief  led control instance struct
  */
typedef struct
{
#if CFG_SUPPORT_CURRRENT_EX
    rgb_channel_t mux_channel;
#endif
    rgb_channel_t channel;
    union
    {
        uint8_t ch_status;
        led_ch_status_t ch_status_bits;
    };

#if (CFG_SUPPORT_MULTIPLEX_LED && CFG_MULTIPLEX_SWITCH_SW)
    gpio_pin_e led_sw_gpio;
#endif
} led_control_context_t;

void pal_led_init(void);
void pal_led_enable(led_channel_e channel, bool enable);
void pal_led_break(led_channel_e channel, bool enable);
void pal_led_duty_cycle_set(led_channel_e channel, uint16_t *duty_cycle);
void pal_led_duty_cycle_get(led_channel_e channel, uint16_t *duty_cycle);
void pal_led_current_set(led_channel_e channel, uint8_t *current);
void pal_led_current_get(led_channel_e channel, uint8_t *current);
void pal_led_static_pnvolt_set(led_channel_e channel, bool enable);
void pal_led_channel_mux_get(led_channel_e channel, uint8_t **channel_mux);
void pal_led_channel_switch(led_channel_e channel);
void pal_led_hsw_count_value_get(uint16_t *value);

extern led_control_context_t led0_ctrl_instance;

#ifdef __cplusplus
}
#endif
#endif /*__PAL_LED_H__*/

