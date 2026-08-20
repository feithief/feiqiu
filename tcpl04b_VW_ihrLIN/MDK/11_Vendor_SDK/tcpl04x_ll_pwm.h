/**
 *****************************************************************************
 * @brief   pwm driver header.
 *
 * @file    tcpl04x_ll_pwm.h
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
 * <b>&copy; Copyright (c) 2026 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */
#ifndef __TCPL04X_LL_PWM_H__
#define __TCPL04X_LL_PWM_H__

#include "tcpl04x_ll_def.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define PWM_INT_CH0_CNT_MAX         PWM_IMR_CNT0_OVF_INT_MSK_MASK               //Mask PWM channel0 counter max overflow interrupt
#define PWM_INT_CH0_PWM_SET         PWM_IMR_CH0_PWM_SET_INT_MSK_MASK            //Mask PWM channel0 PWM output set interrupt
#define PWM_INT_CH0_PWM_CLR         PWM_IMR_CH0_PWM_CLR_INT_MSK_MASK            //Mask PWM channel0 PWM output clear interrupt
#define PWM_INT_CH1_CNT_MAX         PWM_IMR_CNT0_OVF_INT_MSK_MASK               //Mask PWM channel1 counter max overflow interrupt
#define PWM_INT_CH1_PWM_SET         PWM_IMR_CH1_PWM_SET_INT_MSK_MASK            //Mask PWM channel1 PWM output set interrupt
#define PWM_INT_CH1_PWM_CLR         PWM_IMR_CH1_PWM_CLR_INT_MSK_MASK            //Mask PWM channel1 PWM output clear interrupt
#define PWM_INT_CH2_CNT_MAX         PWM_IMR_CNT0_OVF_INT_MSK_MASK               //Mask PWM channel2 counter max overflow interrupt
#define PWM_INT_CH2_PWM_SET         PWM_IMR_CH2_PWM_SET_INT_MSK_MASK            //Mask PWM channel2 PWM output set interrupt
#define PWM_INT_CH2_PWM_CLR         PWM_IMR_CH2_PWM_CLR_INT_MSK_MASK

#define PWM_INT_CH3_CNT_MAX         PWM_IMR_CNT1_OVF_INT_MSK_MASK                //Mask PWM channel2 counter max overflow interrupt
#define PWM_INT_CH3_PWM_SET         PWM_IMR_CH3_PWM_SET_INT_MSK_MASK            //Mask PWM channel2 PWM output set interrupt
#define PWM_INT_CH3_PWM_CLR         PWM_IMR_CH3_PWM_CLR_INT_MSK_MASK

#define PWM_INIT_FLAG   (PWM_INT_CH0_PWM_CLR | PWM_INT_CH0_CNT_MAX | \
                              PWM_INT_CH1_PWM_CLR | PWM_INT_CH1_CNT_MAX | \
                              PWM_INT_CH2_PWM_CLR | PWM_INT_CH2_CNT_MAX)

#define PWM_INT_CH_PWM_MAX          (PWM_INT_CH0_CNT_MAX)
#define PWM_INIT_ALL_FLAG     (0xFFFFUL)

/**
  * @brief  ll sci bus enumeration
  */
typedef enum
{
    LL_PWM_BUS_0 = 0,
    LL_PWM_BUS_MAX,
} ll_pwm_bus_e;

/**
  * @brief  ll pwm channel enumeration
  */
typedef enum
{
    PWM_CHANNEL_0 = 0,
    PWM_CHANNEL_1,
    PWM_CHANNEL_2,
    PWM_CHANNEL_3,
    PWM_CHANNEL_MAX
} ll_pwm_channel_e;

/* pwm channel config */
/**
 * @brief  ll pwm channel sel enumeration
 */
typedef enum
{
    PWM_CNT_SOURCE_CNT0 = 0,
    PWM_CNT_SOURCE_CNT1,
    PWM_CNT_SOURCE_MAX
} ll_pwm_cnt_source_e;
/**
  * @brief  ll pwm channel enumeration
  */
typedef enum
{
    PWM_MODE_DOUBLE_PERIOD = 0,
    PWM_MODE_INDEPENDENT,       //independent mode
    PWM_MODE_SOFTWARE,          //Software mode
    PWM_MODE_FIXED,
    PWM_MODE_TYPE_MAX
} pwm_outmode_e;

/**
  * @brief  ll pwm channel enumeration
  */
typedef enum
{
    PWM_POLARITY_HIGH = 0,     //high level is valid
    PWM_POLARITY_LOW,
    PWM_POLARITY_MAX
} pwm_polarity_e;

/* pwm count config */
/**
 * @brief  ll pwm count index enumeration
 */
typedef enum
{
    PWM_COUNT_INDEX_0 = 0,
    PWM_COUNT_INDEX_1,
    PWM_COUNT_INDEX_MAX
} ll_pwm_count_index_e;

/**
  * @brief  led pwm counter mode enumeration
  */
typedef enum
{
    PWM_COUNT_MODE_UP = 0,
    PWM_COUNT_MODE_UP_DOWN,
    PWM_COUNT_MODE_MAX,
} ll_pwm_count_mode_e;

/**
 * @brief  led pwm sync mode enumeration
 */
typedef enum
{
    PWM_COUNT_SYNC_MODE_NULL = 0,                  /*!< no Synchronization*/
    PWM_COUNT_SYNC_MODE_CNT0_TO_CNT1,             /*!< Sync channel 0 and channel 1*/
    PWM_COUNT_SYNC_MODE_MAX,
} ll_pwm_count_sync_mode_e;
/**
  * @brief  led pwm sync mode enumeration
  */
typedef enum
{
    PWM_CH_SYNC_MODE_NULL = 0,/*!< no Synchronization */
    PWM_CH_SYNC_CH0_TO_CH1,   /*!< Sync channel 0~1 */
    PWM_CH_SYNC_CH0_TO_CH2,   /*!< Sync channel 0~2 */
    PWM_CH_SYNC_CH0_TO_CH3,   /*!< Sync channel 0~3 */
    PWM_CH_SYNC_MODE_MAX,
} ll_pwm_ch_sync_mode_e;

/**
 * @defgroup pwm count configurations structure
 */
typedef struct
{
    ll_pwm_count_mode_e count_mode;
    bool one_shot_enable;                               /*!< Eables one shot mode */
    ll_pwm_count_sync_mode_e sync_mode;
    uint16_t period;
} ll_pwm_count_config_t;

/**
 * @defgroup pwm channel configurations structure
 */
typedef struct
{
    ll_pwm_cnt_source_e cnt_source;     /*!< Specifies the count which will be the source for the current channel
                                          this parameter can be any value of @ref PWM_COUNT_Definitions */
    pwm_polarity_e      polarity;       /*!< Specifies the polarity of the channel This
                                          parameter can be any value of @ref PWM_CH_POLARITY_Definitions */
    pwm_outmode_e       out_mode;       /*!< Specifies the channel output mode, 1'b0 : output 1'b1 valid; 1'b1 : output 1'b0 valid
                                          This parameter can be any value of @ref PWM_CH_MODE_Definitions */
} ll_pwm_channel_config_t;

typedef struct
{
    ll_clk_config_t     clk_cfg;
    ll_isr_config_t     isr_cfg;
    ll_pwm_count_config_t count_config;
    ll_pwm_ch_sync_mode_e pwm_sync_mode;
} pwm_config_t;

ll_status_e ll_pwm_isr_flag_enable(ll_pwm_bus_e bus, uint32_t isr, bool enable);
ll_status_e ll_pwm_isr_enable(ll_pwm_bus_e bus, bool enable);
ll_status_e ll_pwm_isr_clear(ll_pwm_bus_e bus, uint32_t flag);
ll_status_e ll_pwm_isr_flag_get(ll_pwm_bus_e bus, uint32_t *flag);

ll_status_e ll_pwm_enable(ll_pwm_bus_e bus, bool enable);
ll_status_e ll_pwm_channel_threshold_config(ll_pwm_channel_e channel, uint16_t threshold_h, uint16_t threshold_l);
ll_status_e ll_pwm_channel_period_config(ll_pwm_channel_e channel, uint16_t period);
ll_status_e ll_pwm_high_threshold_get(ll_pwm_channel_e channel, uint16_t *value);
ll_status_e ll_pwm_channel_counter_get(ll_pwm_channel_e channel, uint16_t *value);
void ll_pwm_break_set(ll_pwm_bus_e bus, bool enable);
ll_status_e ll_pwm_status_get(ll_pwm_channel_e channel, uint8_t *value);
ll_status_e ll_led_exchannel_enable(ll_pwm_bus_e bus, bool enable);
ll_status_e ll_led_exchannel_init(ll_pwm_bus_e bus, uint16_t period, pwm_polarity_e polarity);
ll_status_e ll_led_switch_init(ll_pwm_bus_e bus);
ll_status_e ll_pwm_deinit(ll_pwm_bus_e bus);
ll_status_e ll_pwm_init(ll_pwm_bus_e bus, pwm_config_t *config, ISR_FUNC_CALLBACK callback);
ll_status_e ll_pwm_channel_config(ll_pwm_channel_e channel, ll_pwm_channel_config_t *config);
ll_status_e ll_pwm_count_config(ll_pwm_count_index_e index, ll_pwm_count_config_t *config);
#if defined(__cplusplus)
}
#endif
#endif /* __TCPL04X_LL_PWM_H__ */
