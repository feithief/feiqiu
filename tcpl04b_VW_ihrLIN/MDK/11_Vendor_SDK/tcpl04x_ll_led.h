/**
 *****************************************************************************
 * @brief   led driver header.
 *
 * @file    tcpl04x_ll_led.h
 * @author
 * @date    2024.04.20
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */
#ifndef __TCPL04X_LL_LED_H__
#define __TCPL04X_LL_LED_H__

#include "tcpl04x_ll_def.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define LL_LED_CHANNEL_MAX             (30)
/**
 * @brief  ll pwm channel enumeration
 */
typedef uint8_t ll_led_channel_e;

/**
  * @brief  led fall sr time enumeration
  */
typedef enum
{
    LED_FALL_SR_TIME_81NS = 0,
    LED_FALL_SR_TIME_200NS,
    LED_FALL_SR_TIME_400NS,
    LED_FALL_SR_TIME_660NS,
    LED_FALL_SR_TIME_MAX,
} led_fall_sr_time_e;

/**
  * @brief  led rise sr time enumeration
  */
typedef enum
{
    LED_RISE_SR_TIME_116NS = 0,
    LED_RISE_SR_TIME_200NS,
    LED_RISE_SR_TIME_400NS,
    LED_RISE_SR_TIME_630NS,
    LED_RISE_SR_TIME_MAX,
} led_rise_sr_time_e;

/**
  * @brief  led driver current enumeration
  */
typedef enum
{
    LED_DRIVER_5MA = 0,
    LED_DRIVER_10MA,
    LED_DRIVER_15MA,
    LED_DRIVER_20MA,
    LED_DRIVER_25MA,
    LED_DRIVER_30MA,
    LED_DRIVER_35MA,
    LED_DRIVER_40MA,
    LED_DRIVER_45MA,
    LED_DRIVER_MAX,
} ll_led_driver_current_e;

/**
  * @brief  led diag current enumeration
  */
typedef enum
{
    LED_DIAG_MAX,
} led_diag_current_e;

void ll_led_init(void);
ll_status_e ll_led_driver_current_config(ll_led_channel_e channel, ll_led_driver_current_e current);
ll_status_e ll_led_driver_current_get(ll_led_channel_e channel, ll_led_driver_current_e *current);
ll_status_e ll_led_diag_current_config(ll_led_channel_e channel, led_diag_current_e current);
ll_status_e ll_led_diagnose_enable(ll_led_channel_e channel, bool enable);
ll_status_e ll_led_enable(bool enable);
ll_status_e ll_led_break_set(bool enable);
ll_status_e ll_led_diag_current_config(ll_led_channel_e channel, led_diag_current_e current);
ll_status_e ll_led_fall_rise_time_set(ll_led_channel_e channel, led_rise_sr_time_e rise_time, led_fall_sr_time_e fall_time);

#if defined(__cplusplus)
}
#endif
#endif /* __TCPL04X_LL_LED_H__ */
