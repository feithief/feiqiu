/**
 *****************************************************************************
 * @brief   pal meas def header file.
 *
 * @file    pal_meas.h
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

#ifndef __PAL_MEAS_DEF_H__
#define __PAL_MEAS_DEF_H__

#include "pal_meas.h"
#include "pal_led.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *
 */
#define MEAS_VF_FILTER_LEN      (40U)

#if defined (__TCPL01X__)
#define LED_MEAS_SERIAL_MAX     (2u)
#define MEAS_VF_CHANNEL_MX      (LED_CHANNEL_MAX)
#elif defined (__TCPL03X__) || defined (__TCPL04X__)
#define LED_MEAS_SERIAL_MAX     (1u)
#define MEAS_VF_CHANNEL_MX      (LED_CHANNEL_MAX)
#elif defined (__TCPL08X__)
#define LED_MEAS_SERIAL_MAX     (1u)
#define MEAS_VF_CHANNEL_MX      (0)
#endif

/**
 * @brief default led serial num
 */
#define DEFAULT_LED_SERIAL_NUM  LED_MEAS_SERIAL_1

/**
 * @brief led ceil floor calue define
 */
#ifndef LED_R_CEIL_VALUE
#define LED_R_CEIL_VALUE        (2020)
#endif //LED_R_CEIL_VALUE
#ifndef LED_R_FLOOR_VALUE
#define LED_R_FLOOR_VALUE       (1585)
#endif //LED_R_FLOOR_VALUE

#ifndef LED_G_CEIL_VALUE
#define LED_G_CEIL_VALUE        (3204)
#endif //LED_G_CEIL_VALUE
#ifndef LED_G_FLOOR_VALUE
#define LED_G_FLOOR_VALUE       (2217)
#endif //LED_G_FLOOR_VALUE

#ifndef LED_B_CEIL_VALUE
#define LED_B_CEIL_VALUE        (3032)
#endif //LED_B_CEIL_VALUE
#ifndef LED_B_FLOOR_VALUE
#define LED_B_FLOOR_VALUE       (2415)
#endif //LED_B_FLOOR_VALUE

typedef bool (*meas_init)(led_channel_e);
typedef bool (*meas_gain_config)(led_channel_e);
typedef bool (*meas_voltage_code_get)(meas_volt_type_e, int16_t *);
typedef bool (*meas_volt_calc_func)(meas_volt_type_e, int16_t, int16_t *);
typedef bool (*meas_pn_voltage_get)(led_channel_e);
typedef bool (*meas_pn_calc_func)(led_channel_e, led_type_e, int16_t *, int16_t *);
typedef bool (*meas_pn_status_reflash)(led_channel_e);
typedef bool (*meas_pn_process)(led_channel_e, uint32_t);
typedef bool (*meas_pn_suspend)(led_channel_e);
typedef bool (*meas_pn_resume)(led_channel_e);
typedef bool (*meas_pn_acquire)(led_channel_e);
typedef bool (*meas_pn_monitor)(led_channel_e);
typedef bool (*meas_pn_status_get)(led_channel_e, led_vf_status_type_e, uint8_t *);
typedef bool (*meas_pn_status_set)(led_channel_e, led_vf_status_type_e, uint8_t);

typedef struct meas_manager_instance_t_
{
    bool (*meas_init)(led_channel_e);
    bool (*meas_gain_config)(led_channel_e);
    bool (*meas_voltage_code_get)(meas_volt_type_e, int16_t *);
    bool (*meas_volt_calc_func)(meas_volt_type_e, int16_t, int16_t *);
    bool (*meas_pn_voltage_get)(led_channel_e);
    bool (*meas_pn_calc_func)(led_channel_e, led_type_e, int16_t *, int16_t *);
    bool (*meas_pn_status_reflash)(led_channel_e);
    bool (*meas_pn_process)(led_channel_e, uint32_t);
    bool (*meas_pn_suspend)(led_channel_e);
    bool (*meas_pn_resume)(led_channel_e);
    bool (*meas_pn_acquire)(led_channel_e);
    bool (*meas_pn_monitor)(led_channel_e);
    bool (*meas_pn_status_get)(led_channel_e, led_vf_status_type_e, uint8_t *);
    bool (*meas_pn_status_set)(led_channel_e, led_vf_status_type_e, uint8_t);
} meas_manager_instance_t;

#ifdef __cplusplus
}
#endif
#endif /*__PAL_MEAS_DEF_H__*/
