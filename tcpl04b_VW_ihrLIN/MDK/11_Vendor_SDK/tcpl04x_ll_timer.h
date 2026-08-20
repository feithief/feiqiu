/**
 *****************************************************************************
 * @brief   timer driver header.
 *
 * @file    tcpl04x_ll_timer.h
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

#ifndef __TCPL04X_LL_TIMER_H__
#define __TCPL04X_LL_TIMER_H__

#include "tcpl04x_ll_def.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
  * @brief  ll timer bus enumeration
  */
typedef enum
{
    LL_TIMER_BUS_0 = 0,
    LL_TIMER_BUS_MAX
} ll_timer_bus_e;


typedef struct
{
    ll_clk_config_t clk_cfg;
    ll_isr_config_t isr_cfg;
    uint16_t initial_value; /*!< Specifies the initial decrement alue, the timer will decremnt from this value.
                                This parameter can be any value between 0x00 and 0xFFFF */

    bool repeat_disable;    /*!< if loop is disabled, the timer will decrement N times
                                N is specified by loop_repeat_counts*/
    bool trigger_mode;
} timer_config_t;

ll_status_e ll_timer_deinit(ll_timer_bus_e bus);
ll_status_e ll_timer_init(ll_timer_bus_e bus, timer_config_t *config, ISR_FUNC_CALLBACK callback);
ll_status_e ll_timer_isr_enable(ll_timer_bus_e bus, bool enable);
ll_status_e ll_timer_isr_get(ll_timer_bus_e bus, uint8_t *status);
ll_status_e ll_timer_trig_enable(ll_timer_bus_e bus, bool enable);
ll_status_e ll_timer_enable(ll_timer_bus_e bus, bool enable);
ll_status_e ll_timer_counter_set(ll_timer_bus_e bus, uint16_t value);
ll_status_e ll_timer_counter_get(ll_timer_bus_e bus, uint16_t *value);

#if defined(__cplusplus)
}
#endif
#endif /* __TCPL04X_LL_TIMER_H__ */
