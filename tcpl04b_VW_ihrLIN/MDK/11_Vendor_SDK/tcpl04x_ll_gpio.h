/**
 *****************************************************************************
 * @brief   gpio driver header file.
 *
 * @file    tcpl04x_ll_gpio.h
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

#ifndef __TCPL04X_LL_GPIO_H__
#define __TCPL04X_LL_GPIO_H__

#include "tcpl04x_ll_def.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  gpio pin enumeration
 */
typedef enum
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,   //LED0
    GPIO_PIN_7,   //LED1
    GPIO_PIN_8,   //LED2
    GPIO_PIN_MAX,
} gpio_pin_e;

/**
 * @brief  gpio mode enumeration
 */
typedef enum
{
    GPIO_MODE_OUT_PP = 0,
    GPIO_MODE_OUT_OD,
    GPIO_MODE_IN_PP,
    GPIO_MODE_IN_OD,
} gpio_mode_e;

/**
 * @brief  gpio pull mode enumeration
 */
typedef enum
{
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
} gpio_pull_mode_e;

/**
 * @brief  gpio pull down type enumeration
 */
typedef enum
{
    GPIO_PULLDOWN_SWANDHW = 0,  /*!< GPIO Pull down is controlled by Outx_pd and hardware together*/
    GPIO_PULLDOWN_SW_ONLY,          /*!< GPIO Pull down is controlled by Outx_pd only*/
} gpio_pull_down_type;

/**
 * @brief  gpio trigger type enumeration
 */
typedef enum
{
    GPIO_TRIGGER_NULL = 0,
    GPIO_TRIGGER_LOW_LEVEL,           /*interrupt is active as long as GPIO is in low level*/
    GPIO_TRIGGER_FALLING_EDGE,        /*interrupt is active only on the falling edge*/
    GPIO_TRIGGER_HIGH_LEVEL,          /*interrupt is active as long as GPIO is in high level*/
    GPIO_TRIGGER_RISING_EDGE,         /*interrupt is active only on the rising edge*/
    GPIO_TRIGGER_RISING_FALLING_EDGE  /*both rising and falling edge will trigger interrupt*/
} gpio_trigger_flag_e;

/**
 * @brief  gpio trigger type enumeration GPIO0 and GPIO1为高速
*   GPIO    AFIO_0  AFIO_1      AFIO_2      AFIO_3      AFIO_4  AFIO_5      AFIO_6  AFIO_7
*   GPIO0   SWCLK   GPIO0       LED_S       LIN0_TX_T   NULL    NULL        NULL    NULL
*   GPIO1   SWDIO   GPIO1       PWM_CH3     NULL        NULL    NULL        NULL    NULL
*   GPIO2   GPIO2   DEBUG_TXD   PWM_CH0     NULL        NULL    NULL        NULL    NULL
*   GPIO3   GPIO3   DEBUG_TXD   PWM_CH1     NULL        NULL    LIN1_RXD    NULL    NULL
*   GPIO4   GPIO4   NULL        PWM_CH2     NULL        NULL    LIN1_TXD    NULL    NULL
*   GPIO5   GPIO5   LIN1_UART_1 NULL        NULL        NULL    LIN1_TXD    NULL    NULL
*   GPIO6   LED0    PWM_CH0     DEBUG_TXD   GPIO6       NULL    NULL        NULL    NULL
*   GPIO7   LED1    PWM_CH1     LIN0_RXD    GPIO7       NULL    NULL        NULL    NULL
*   GPIO8   LED2    PWM_CH2     LIN0_TXD    GPIO8       NULL    NULL        NULL    NULL
*/
typedef enum
{
    AFIO_MUX_0 = 0,
    AFIO_MUX_1,
    AFIO_MUX_2,
    AFIO_MUX_3,
    AFIO_MUX_4,
    AFIO_MUX_5,
    AFIO_MUX_6,
    AFIO_MUX_7,
} gpio_afio_mux_e;

/**
  * @defgroup GPIO_Configuration struct
  */
typedef struct
{
    gpio_pin_e gpio_pin;                   /* GPIO PIN  @ref gpio_pin_e */
    gpio_mode_e mode;                      /* GPIO mode @ref gpio_mode_e */
    gpio_pull_mode_e pull_mode;            /* GPIO Pull mode @ref gpio_pull_mode_e */
    gpio_pull_down_type pull_down_type;    /* pull-down type @ref gpio_pull_down_type */
    gpio_afio_mux_e afio;                  /* alternate function @ref gpio_afio_mux_e */
    gpio_trigger_flag_e trigger_flag;      /* @ref gpio_trigger_flag_e */
} gpio_config_t;

void ll_gpio_deinit(void);
void ll_gpio_init(gpio_config_t *config, ISR_FUNC_CALLBACK callback);
bool ll_gpio_read(gpio_pin_e gpio_pin);
void ll_gpio_output(gpio_pin_e gpio_pin, bool state);
void ll_gpio_toggle(gpio_pin_e gpio_pin);
void ll_gpio_isr_enable(gpio_pin_e gpio_pin, bool enable);
bool ll_gpio_interrupt_flag_get(gpio_pin_e gpio_pin);
void ll_gpio_interrupt_clear(gpio_pin_e gpio_pin);
void ll_gpio_afio_config(gpio_pin_e gpio_pin, gpio_afio_mux_e afio_mux);
void ll_gpio_ano_lpm_config(gpio_pin_e gpio_pin, bool keep_high);
void ll_gpio_reset_enable(bool enable);

#if defined(__cplusplus)
}
#endif
#endif /* __TCPL04X_LL_GPIO_H__ */
