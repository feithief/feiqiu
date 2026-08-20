/**
 *****************************************************************************
 * @brief   pal meas header file.
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

#ifndef __PAL_MEAS_H__
#define __PAL_MEAS_H__

#include "pal_func_def.h"
#include "pal_meas_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CFG_SUPPORT_MEAS_INSTANNCE  1
#if CFG_SUPPORT_MEAS_INSTANNCE
#define MEAS_INSTANCE   static
#define MEAS_LED_CHANNEL_INVALID(channel) (false)
#else
#define MEAS_INSTANCE
#define MEAS_LED_CHANNEL_INVALID(channel) ((channel>=LED_CHANNEL_MAX) ? true:false)
#endif

#if defined (__TCPL01X__)
#define LED_VF_SAMP_CALC_NUM    (1u)
#define LED_VF_RANGE_MAX        (6u)
#elif defined(__TCPL03X__) || defined(__TCPL04X__)
#define LED_VF_SAMP_CALC_NUM    (3u)
#elif defined (__TCPL08X__)
#define LED_VF_SAMP_CALC_NUM    (1u)
#endif

/**
 * @brief meas volt type enumeration
 */
typedef enum
{
    MEAS_VOLT_TEMP = 0U,
    MEAS_VOLT_VBAT,
    MEAS_VOLT_PN,
    MEAS_VOLT_PN_0 = MEAS_VOLT_PN,
    MEAS_VOLT_PN_1,
    MEAS_VOLT_PN_2,
    MEAS_VOLT_MAX,
} meas_volt_type_e;

/**
 * @brief meas trig src enumeration
 */
typedef enum
{
    CH_TRIG_CNT_MAX,
    CH0_TRIG_PWM_CLR,
    CH1_TRIG_PWM_CLR,
    CH2_TRIG_PWM_CLR,
#if defined(__TCPL08X__)
    CH3_TRIG_PWM_CLR,
    CH4_TRIG_PWM_CLR,
    CH5_TRIG_PWM_CLR,
    CH6_TRIG_PWM_CLR,
    CH7_TRIG_PWM_CLR,
    CH8_TRIG_PWM_CLR,
    CH9_TRIG_PWM_CLR,
    CH10_TRIG_PWM_CLR,
    CH11_TRIG_PWM_CLR,
    CH12_TRIG_PWM_CLR,
    CH13_TRIG_PWM_CLR,
    CH14_TRIG_PWM_CLR,
    CH15_TRIG_PWM_CLR,
    CH16_TRIG_PWM_CLR,
    CH17_TRIG_PWM_CLR,
    CH18_TRIG_PWM_CLR,
    CH19_TRIG_PWM_CLR,
    CH20_TRIG_PWM_CLR,
    CH21_TRIG_PWM_CLR,
    CH22_TRIG_PWM_CLR,
    CH23_TRIG_PWM_CLR,
    CH24_TRIG_PWM_CLR,
    CH25_TRIG_PWM_CLR,
    CH26_TRIG_PWM_CLR,
    CH27_TRIG_PWM_CLR,
    CH28_TRIG_PWM_CLR,
    CH29_TRIG_PWM_CLR,
#endif
    CH_TRIG_SRC_MAX,
} trig_src_e;

/**
 * @brief meas vf status enumeration
 */
typedef enum
{
    LED_VF_DATA_STATUS = (0x01U << 0),          /* RGB PN volt data ready */
    LED_VF_SUSPEND_STATUS = (0x01U << 1),       /* PN volt sample flag */
    LED_VF_DYNAMIC_SAMP_STATUS = (0x01U << 2),  /* PN volt acquire flag */
    LED_VF_STATIC_SAMP_STATUS = (0x01U << 3),   /* PN volt static sample flag */
} led_vf_status_type_e;

/**
 * @brief meas vf channel status struct
 */
typedef union
{
    uint8_t status_byte;
    struct
    {
        uint8_t red_ready: 1;
        uint8_t green_ready: 1;
        uint8_t blue_ready: 1;
        uint8_t ch_index: 2;
        uint8_t reserved: 3;
    } bit;
} vf_channel_status_t;


/**
 * @brief meas vf status struct
 */
typedef struct
{
    uint8_t data_status : 1;
    uint8_t suspend_status : 1;
    uint8_t acquire_status : 1;
    uint8_t static_samp_status : 1;
    uint8_t reserved : 4;
} vf_sample_status_t;

/**
 * @brief meas vf sample context struct
 */
typedef struct
{
    trig_src_e trig_src;
    vf_channel_status_t ch_status;
    int16_t vf_code[LED_TYPE_MAX][LED_VF_SAMP_CALC_NUM]; /* TCPL03X 0-VF 1-AON_T 2-AON*/
    union
    {
        uint8_t vf_status;
        vf_sample_status_t vf_status_bit;
    };
#if defined (__TCPL01X__)
    uint8_t vf_range_index[LED_TYPE_MAX];
#endif
    uint32_t time_cnt;
} vf_sample_ctx_t;

/**
 * @brief led meas context struct
 */
typedef struct
{
    uint8_t *rgb_vf_mux;
    vf_sample_ctx_t vf_samp;
#if defined(__TCPL08X__)
#if CFG_SUPPORT_MULTIPLEX_LED || CFG_SUPPORT_HSW_SMP
    uint8_t meas_index;
#endif
#endif
} led_measure_context_t;

/**
 * @brief led rgb safty threshold struct
 */
typedef struct
{
    uint16_t ceil_value;
    uint16_t floor_value;
} rgb_safty_threshold_t;

/**
 * @brief led rgb sft adpat threshold struct
 */
typedef struct
{
    uint16_t value_l;
    uint16_t value_h;
} sft_adpat_value_t;

#if defined (__TCPL01X__)
/**
 * @brief led rgb vf auto range struct
 */
typedef struct
{
    uint16_t    min_voltage;
    uint16_t    max_voltage;
    adc_vcr_e   vcr;
} vf_adc_range_config_t;

/**
 * @brief led rgb vf auto range enumeration
 */
typedef enum
{
    VF_RANGE_01,
    VF_RANGE_02,
    VF_RANGE_03,
    VF_RANGE_04,
    VF_RANGE_05,
    VF_RANGE_06,
    VF_RANGE_MAX,
} vf_adc_range_e;
#endif


#if !CFG_SUPPORT_MEAS_INSTANNCE
bool led_meas_init(led_channel_e channel);
bool led_meas_gains_config(led_channel_e channel);
bool led_meas_voltage_code_get(meas_volt_type_e type, uint16_t *value);
bool led_meas_volt_calc_func(meas_volt_type_e type, uint16_t raw_code, int16_t *value);

bool led_meas_pn_voltage_get(led_channel_e channel);
bool led_meas_pn_calc_func(led_channel_e channel, led_type_e rgb, int16_t *value);
bool led_meas_pn_status_reflash(led_channel_e channel);
bool led_meas_pn_process(led_channel_e channel, uint32_t pwm_isr);
bool led_meas_pn_suspend(led_channel_e channel);
bool led_meas_pn_resume(led_channel_e channel);
bool led_meas_pn_acquire(led_channel_e channel);
bool led_meas_pn_monitor(led_channel_e channel);
bool led_meas_pn_status_get(led_channel_e channel, led_vf_status_type_e type, uint8_t *status);
bool led_meas_pn_status_set(led_channel_e channel, led_vf_status_type_e type, uint8_t status);
#endif

#ifdef __cplusplus
}
#endif
#endif /*__PAL_MEAS_H__*/
