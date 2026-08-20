/**
 *****************************************************************************
 * @brief   led pinmux evk header file.
 *
 * @file    pinmux_cfg_evk.h
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

#ifndef __LED_PINMUX_CFG_H__
#define __LED_PINMUX_CFG_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief LED pinmux configuration table
  */
const led_pinmux_config_t led_pinmux_table[][3] =
{
#if 3 == CFG_SUPPORT_COLOR_COMP_MODE
    {{GPIO_PIN_2, AFIO_MUX_2, PWM_CHANNEL_0},   {GPIO_PIN_4, AFIO_MUX_2, PWM_CHANNEL_2},    {GPIO_PIN_3, AFIO_MUX_2, PWM_CHANNEL_1}},   /* led channel 1: R,G,B */
#else
    {{GPIO_PIN_6, AFIO_MUX_0, PWM_CHANNEL_0},   {GPIO_PIN_8, AFIO_MUX_0, PWM_CHANNEL_2},    {GPIO_PIN_7, AFIO_MUX_0, PWM_CHANNEL_1}},   /* led channel 1: R,G,B */
#endif
};

/**
  * @brief led mux table
  */
const uint8_t led_mux_table[] =
{
    0
};


#ifdef __cplusplus
}
#endif
#endif /* __LED_PINMUX_CFG_H__ */

