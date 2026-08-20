/**
 *****************************************************************************
 * @brief   pal_gpio header file.
 *
 * @file    pal_gpio.h
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
#ifndef __PAL_GPIO_H__
#define __PAL_GPIO_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

void pal_gpio_init(gpio_config_t *cfg, ISR_FUNC_CALLBACK callback);
bool pal_gpio_read(gpio_pin_e gpio_pin);
void pal_gpio_output(gpio_pin_e gpio_pin, bool state);
void pal_gpio_toggle(gpio_pin_e gpio_pin);
void pal_gpio_afio_config(gpio_pin_e gpio_pin, gpio_afio_mux_e afio_mux);
void pal_gpio_isr_enable(gpio_pin_e gpio_pin, bool enable);

#ifdef __cplusplus
}
#endif
#endif /*__PAL_GPIO_H__*/


