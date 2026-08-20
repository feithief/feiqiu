/**
 *****************************************************************************
 * @brief   pal led source file.
 *
 * @file    pal_led.c
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

#include "pal_led.h"
#include "pal_led_def.h"
#include "prj_led_pinmux_cfg.h"

/**
* @brief frequency of static acquisition for PN junction voltage
*/
#if (defined (__TCPL01X__) || defined (__TCPL03X__) || defined (__TCPL04X__))
#define LED_STATIC_SAMPLE_FREQ   0X4FFF
#define LED_PRIORITY_LEVEL       (2)
#elif defined (__TCPL08X__) /* __TCPL08X__ */
#if CFG_SUPPORT_MULTIPLEX_LED
#define LED_STATIC_SAMPLE_FREQ   0XFFFF
#define LED_PRIORITY_LEVEL       (1)
#else /* CFG_SUPPORT_MULTIPLEX_LED */
#define LED_STATIC_SAMPLE_FREQ   0X4FFF
#define LED_PRIORITY_LEVEL       (2)
#endif  /* !CFG_SUPPORT_MULTIPLEX_LED */
#endif /* __TCPL08X__ */

#if (defined (__TCPL08X__))
#if CFG_SUPPORT_MULTIPLEX_LED
#if (CFG_SUPPORT_MULTIPLEX_LED <= 2)
#define LED_DISCHARGE_ISR_FLAG      (PWM_INT_HSW_CLR(0) | PWM_INT_HSW_CLR(1) | PWM_INT_HSW_CLR(2) | PWM_INT_HSW_BLANK)
#else
#define LED_DISCHARGE_ISR_FLAG      (PWM_INT_HSW_CLR(0) | PWM_INT_HSW_CLR(1) | PWM_INT_HSW_CLR(2) |\
                                    PWM_INT_HSW_CLR(3) | PWM_INT_HSW_CLR(4) | PWM_INT_HSW_CLR(5) | \
                                    PWM_INT_HSW_BLANK)
#endif
#endif /* CFG_SUPPORT_MULTIPLEX_LED */
#endif /* __TCPL08X__ */

/**
* @brief LED driver frequency
*/
#define LED_DRIVER_PWM_FREQ      0XFFFF

#define HSW_CHANNEL_NUM          (CFG_SUPPORT_MULTIPLEX_LED + 1)

extern void  pwm_callback_handle(uint32_t isr);
extern void  hsw_callback_handle(uint32_t isr);


#if (defined (__TCPL08X__))
#if CFG_SUPPORT_MULTIPLEX_LED
/**
  * @brief HSW cycle configuration table
  */
const uint8_t hsw_cycle_table[] =
{
    0x00, 0x00, 0x01, 0x01, 0x02, 0x02, /* HSW: HSW0, HSW1, HSW2 */
    0x03, 0x03, 0x04, 0x04, 0x05, 0x05, /* HSW: HSW3, HSW4, HSW5 */
};

#endif /* CFG_SUPPORT_MULTIPLEX_LED */
#endif /* __TCPL08X__ */


led_control_context_t led_ctrl_context[CFG_SUPPORT_RGB_NUM];

__attribute__((weak)) void pwm_callback_handle(uint32_t isr)
{
    //do noting
}

#if defined (__TCPL08X__)
__attribute__((weak)) void hsw_callback_handle(uint32_t isr)
{

}

/********************************************************
** \brief   pal_led_hsw_init
**
** \param   None
**
** \retval  None
*********************************************************/
static void pal_led_hsw_init(void)
{
#if CFG_SUPPORT_MULTIPLEX_LED

    for (uint8_t i = 0; i < HSW_CHANNEL_NUM; i++)
    {
        ll_gpio_afio_config(hsw_pinmux_table[i].gpio_pin, hsw_pinmux_table[i].afio_mux); // LED_DRV_HSW
    }

    if (AFIO_MUX_0 == hsw_pinmux_table[0].afio_mux && AFIO_MUX_0 == hsw_pinmux_table[1].afio_mux && \
        AFIO_MUX_0 == hsw_pinmux_table[2].afio_mux)
    {
        ll_led_hsw_config(LED_HSW_MODE_HW);
    }

    /**
    * @brief led pwm config
    */
    ll_pwm_count_config_t count_config =
    {
        .count_mode = PWM_COUNT_MODE_UP,
        .one_shot_enable = false,
        .sync_mode = PWM_COUNT_SYNC_MODE_CNT0_TO_CNT1,
        .period = CFG_SUPPORT_MULTIPLEX_LED,
    };
    ll_pwm_count_config(PWM_COUNT_INDEX_1, &count_config);

    ll_pwm_channel_config_t channel_config =
    {
        .cnt_source = PWM_CNT_SOURCE_CNT1,
        .polarity = PWM_POLARITY_HIGH,
        .out_mode = PWM_MODE_INDEPENDENT,
    };
    uint32_t hsw_isr = 0;

    for (uint8_t i = 0; i < HSW_CHANNEL_NUM; i++)
    {
        ll_pwm_channel_config(hsw_pinmux_table[i].pwm_channel, &channel_config);
        ll_pwm_channel_threshold_config(hsw_pinmux_table[i].pwm_channel, \
                                        hsw_cycle_table[(i << 1) + 1], \
                                        hsw_cycle_table[i << 1]);
        hsw_isr |= PWM_INT_HSW_SET(hsw_pinmux_table[i].hsw_index);
    }

    /* fast discharge for auto ctrl mode */
    //hsw_isr |= LED_DISCHARGE_ISR_FLAG;

    ll_pwm_hsw_isr_config(LL_PWM_BUS_0, hsw_isr, hsw_callback_handle);
#else
#if CFG_SUPPORT_HSW_SMP

    for (led_type_e rgb = 0; rgb < LED_TYPE_MAX; rgb++)
    {
        ll_gpio_afio_config(hsw_pinmux_table[rgb].gpio_pin, hsw_pinmux_table[rgb].afio_mux); // LED_DRV_HSW
    }

    ll_led_hsw_config(LED_HSW_MODE_SW);
    ll_led_hsw_sw_enable(LED_MULT_HSW0_2);
#endif /* CFG_SUPPORT_HSW_SMP */
#endif /* CFG_SUPPORT_MULTIPLEX_LED */
}
#endif

/********************************************************
** \brief   pal_led_init
**
** \param   void
**
** \retval  None
*********************************************************/
void pal_led_init(void)
{
    /**
    * @brief led pwm config
    */
    pwm_config_t pwm_config =
    {
        .clk_cfg = {

            .clk_source = FCLK_SRC_HSI,
#if defined (__TCPL08X__)
#if CFG_SUPPORT_MULTIPLEX_LED
            .fclk_div = CRG_CLOCK_DIVIDER_1,
#else
            .fclk_div = CRG_CLOCK_DIVIDER_4,
#endif
#else
            .fclk_div = 2,
#endif

        },
        .isr_cfg = {
            .isr = PWM_INT_CH_PWM_MAX,
            .isr_enable = true,
            .priority = LED_PRIORITY_LEVEL,
        },

#if defined (__TCPL01X__) || defined (__TCPL03X__) || defined (__TCPL04X__) /* __TCPL03X__ ||  __TCPL04X__ */
        .pwm_sync_mode = PWM_CH_SYNC_CH0_TO_CH3,
#elif defined (__TCPL08X__)
        .pwm_sync_mode = PWM_CH_SYNC_CH0_TO_CH29,
#endif /* __TCPL08X__ */

        .count_config =
        {
            .count_mode = PWM_COUNT_MODE_UP,
            .one_shot_enable = false,
            .sync_mode = PWM_COUNT_SYNC_MODE_CNT0_TO_CNT1,
            .period = LED_DRIVER_PWM_FREQ,
        },
    };

    ll_pwm_init(LL_PWM_BUS_0, &pwm_config, pwm_callback_handle);
    ll_pwm_count_config(PWM_COUNT_INDEX_0, &pwm_config.count_config);

#if !((defined (__TCPL03X__) || defined (__TCPL04X__)) && (CM_COLOR_COMP_TYPE_EXT_TEMP == CFG_SUPPORT_COLOR_COMP_MODE))
    ll_led_init();
#endif

#if CFG_SUPPORT_MULTIPLEX_LED
#if defined (__TCPL08X__)
    ll_pwm_blank_time_config_t blank_cfg =
    {
        /* TODO: blank time define */
        .hs_cnt_max = 0x3FF,
        .hs_autoload_enabale = 1,
        .hs_hw_trig_enable = 1,
        .rgb_cnt_max = 0x3FF,
        .rgb_hw_trig_enable = 1,
    };
    ll_pwm_blank_time_config(LL_PWM_BUS_0, &blank_cfg);
#elif defined (__TCPL01X__) || defined (__TCPL03X__) || defined (__TCPL04X__)
#if CFG_MULTIPLEX_SWITCH_SW
    gpio_config_t led_sw_config =
    {
        .gpio_pin = (gpio_pin_e)CFG_MULTIPLEX_SWITCH_GPIO,
        .mode = GPIO_MODE_OUT_PP,
        .pull_mode = GPIO_PULL_UP,
        .pull_down_type = GPIO_PULLDOWN_SW_ONLY,
#if defined (__TCPL01X__)
        .afio = AFIO_MUX_0,
#elif defined (__TCPL03X__) || defined (__TCPL04X__)
        .afio = AFIO_MUX_1,
#endif
        .trigger_flag = GPIO_TRIGGER_NULL,
    };
    ll_gpio_init(&led_sw_config, NULL);
    ll_gpio_output(GPIO_PIN_0, false);
    ll_led_enable(true);
    ll_pwm_enable(LL_PWM_BUS_0, true);
#else
    ll_led_switch_init((ll_pwm_bus_e)channel);
#endif /* CFG_MULTIPLEX_SWITCH_SW */
#endif /* __TCPL01X__ || __TCPL03X__ || __TCPL04X__ */
#endif /* CFG_SUPPORT_MULTIPLEX_LED */

    ll_pwm_isr_enable(LL_PWM_BUS_0, true);
#if defined (__TCPL08X__)
    pal_led_hsw_init();
#endif
}

/********************************************************
** \brief   pal_led_register
**
** \param   led_channel_e               channel
**
** \retval  None
*********************************************************/
void pal_led_register(led_channel_e channel)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

#if ((defined (__TCPL03X__) || defined (__TCPL04X__)) && (CM_COLOR_COMP_TYPE_EXT_TEMP == CFG_SUPPORT_COLOR_COMP_MODE))
    ll_gpio_afio_config(GPIO_PIN_6, AFIO_MUX_3);
    ll_gpio_afio_config(GPIO_PIN_7, AFIO_MUX_3);
    ll_gpio_afio_config(GPIO_PIN_8, AFIO_MUX_3);
    PINMUX->LED0_CFG_F.LED0_ASW = 1;
    PINMUX->LED1_CFG_F.LED1_ASW = 1;
    PINMUX->LED2_CFG_F.LED2_ASW = 1;
#endif


#ifdef CFG_SUPPORT_LARGE_CURRENT
    /* PWM1-GPIO3, PWM2-GPIO0, PWM3-GPIO2 */
    ll_gpio_afio_config(GPIO_PIN_0, AFIO_MUX_4);
    ll_gpio_afio_config(GPIO_PIN_1, AFIO_MUX_2);
    ll_gpio_afio_config(GPIO_PIN_3, AFIO_MUX_2);
#endif

#if ((defined (__TCPL03X__) || defined (__TCPL04X__)) && CFG_SUPPORT_CURRRENT_EX)
    ctx->mux_channel.rgb[LED_R] = 1;
    ctx->mux_channel.rgb[LED_G] = 2;
    ctx->mux_channel.rgb[LED_B] = 0;
#endif

    ll_pwm_channel_config_t channel_config =
    {
        .cnt_source = PWM_CNT_SOURCE_CNT0,
        .polarity = PWM_POLARITY_HIGH,
        .out_mode = PWM_MODE_INDEPENDENT,
    };

    for (led_type_e rgb = 0; rgb < LED_TYPE_MAX; rgb++)
    {
        ctx->channel.rgb[rgb] = led_pinmux_table[channel][rgb].pwm_channel;

        ll_gpio_afio_config(led_pinmux_table[channel][rgb].gpio_pin, led_pinmux_table[channel][rgb].afio_mux);

        ll_pwm_channel_config((ll_pwm_channel_e)ctx->channel.rgb[rgb], &channel_config);

#ifdef __TCPL01X__
        ll_led_diag_current_config((ll_led_channel_e)ctx->channel.rgb[rgb], (led_diag_current_e)LED_DIAG_1000UA);
#endif
        ll_led_driver_current_config((ll_led_channel_e)ctx->channel.rgb[rgb], (ll_led_driver_current_e)CFG_LED_DRIVING_STEP);
    }

#if CFG_SUPPORT_LED_PARALLEL_NUM

    for (uint8_t i = 0; i < CFG_SUPPORT_LED_PARALLEL_NUM; i++)
    {
        uint8_t ex_channle = channel * CFG_SUPPORT_LED_PARALLEL_NUM + i;

        for (led_type_e rgb = 0; rgb < LED_TYPE_MAX; rgb++)
        {
            ll_gpio_afio_config(led_pinmux_table_ex[ex_channle + i][rgb].gpio_pin, led_pinmux_table_ex[ex_channle][rgb].afio_mux);
            ll_pwm_channel_config((ll_pwm_channel_e)led_pinmux_table_ex[ex_channle][rgb].pwm_channel, &channel_config);
            ll_led_driver_current_config((ll_led_channel_e)led_pinmux_table_ex[ex_channle][rgb].pwm_channel, (ll_led_driver_current_e)CFG_LED_DRIVING_STEP);
        }
    }

#endif

}

/********************************************************
** \brief   pal_led_current_set
**
** \param   led_channel_e               channel
** \param   uint8_t                     *current
**
** \retval  None
*********************************************************/
void pal_led_current_set(led_channel_e channel, uint8_t *current)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

    if (NULL == ctx)
    {
        return;
    }

    for (led_type_e rgb = LED_R; rgb < LED_TYPE_MAX; rgb++)
    {
        ll_led_driver_current_config((ll_led_channel_e)ctx->channel.rgb[rgb], (ll_led_driver_current_e)current[rgb]);
    }
}

/********************************************************
** \brief   pal_led_current_get
**
** \param   led_channel_e               channel
** \param   uint8_t*                    *current
**
** \retval  None
*********************************************************/
void pal_led_current_get(led_channel_e channel, uint8_t *current)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];
    ll_led_driver_current_e value;

    if (NULL == ctx)
    {
        return;
    }

    for (led_type_e rgb = LED_R; rgb < LED_TYPE_MAX; rgb++)
    {
        ll_led_driver_current_get((ll_led_channel_e)ctx->channel.rgb[rgb], &value);
        current[rgb] = value;
    }
}

/********************************************************
** \brief   pal_led_duty_cycle_set
**
** \param   led_channel_e               channel
** \param   uint16_t*                   duty_cycle
**
** \retval  None
*********************************************************/
void pal_led_duty_cycle_set(led_channel_e channel, uint16_t *duty_cycle)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

    if (NULL == ctx || NULL == duty_cycle)
    {
        return;
    }

#if !CFG_SUPPORT_MONO_COLOUR

    for (led_type_e rgb = LED_R; rgb < LED_TYPE_MAX; rgb++)
#else
    for (led_type_e rgb = LED_R; rgb < LED_TYPE_MAX - 2; rgb++) /* Mono-color only support R channel */
#endif
    {
        /* when lt > ht, duty cycle is 0 */
#ifdef CFG_SUPPORT_LARGE_CURRENT
        uint16_t ht = UINT16_MAX - duty_cycle[rgb];
        uint16_t lt = (ht == 0) ? 1 : 0;
#else
        uint16_t ht = duty_cycle[rgb];
        uint16_t lt = (ht == 0) ? 1 : 0;
#endif
        ll_pwm_channel_threshold_config((ll_pwm_channel_e)ctx->channel.rgb[rgb], ht, lt);
#if CFG_SUPPORT_LED_PARALLEL_NUM

        for (uint8_t i = 0; i < CFG_SUPPORT_LED_PARALLEL_NUM; i++)
        {
            uint8_t ex_channle = channel * CFG_SUPPORT_LED_PARALLEL_NUM + i;
            ll_pwm_channel_threshold_config((ll_pwm_channel_e)led_pinmux_table_ex[ex_channle][rgb].pwm_channel, ht, lt);
        }

#endif
    }

#if CFG_SUPPORT_MULTIPLEX_LED
#if CFG_MULTIPLEX_SWITCH_SW
    ll_gpio_output(GPIO_PIN_0, channel);
#endif /* CFG_MULTIPLEX_SWITCH_SW */
#endif /* CFG_SUPPORT_MULTIPLEX_LED */
}

/********************************************************
** \brief   pal_led_duty_cycle_get
**
** \param   led_channel_e               channel
** \param   uint16_t*                   duty_cycle
**
** \retval  None
*********************************************************/
void pal_led_duty_cycle_get(led_channel_e channel, uint16_t *duty_cycle)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

    if (NULL == ctx || NULL == duty_cycle)
    {
        return;
    }

    for (led_type_e rgb = LED_R; rgb < LED_TYPE_MAX; rgb++)
    {
        ll_pwm_high_threshold_get((ll_pwm_channel_e)ctx->channel.rgb[rgb], &duty_cycle[rgb]);
    }
}

/********************************************************
** \brief   pal_led_enable
**
** \param   led_channel_e               channel
** \param   bool                        enable
**
** \retval  None
*********************************************************/
void pal_led_enable(led_channel_e channel, bool enable)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

    if (NULL == ctx)
    {
        return;
    }

    // #if defined (__TCPL08X__)
    // #if !CFG_SUPPORT_MULTIPLEX_LED

    //     if (enable)
    //     {
    //         ll_led_hsw_sw_enable(LED_MULT_HSW0_2);
    //     }
    //     else
    //     {
    //         ll_led_hsw_sw_enable(LED_MULT_HSW_NULL);
    //     }

    // #endif
    // #endif

    ll_pwm_enable(LL_PWM_BUS_0, enable);
    ll_led_enable(enable);
    ctx->ch_status_bits.light_sts = enable;
}

/********************************************************
** \brief   pal_led_break
**
** \param   led_channel_e               channel
** \param   bool                        enable
**
** \retval  None
*********************************************************/
void pal_led_break(led_channel_e channel, bool enable)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

    if (NULL == ctx)
    {
        return;
    }

    //ll_pwm_break_set(LL_PWM_BUS_0, enable);
    //ll_led_break_set(enable);
    if (ctx->ch_status_bits.light_sts)
    {
        ll_pwm_enable(LL_PWM_BUS_0, !enable);
        ll_led_enable(!enable);
    }
}

/********************************************************
** \brief   pal_led_static_pnvolt_set
**
** \param   led_channel_e               channel
** \param   bool                        enable
**
** \retval  None
*********************************************************/
void pal_led_static_pnvolt_set(led_channel_e channel, bool enable)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

    if (NULL == ctx)
    {
        return;
    }

    if (enable != ctx->ch_status_bits.static_sample_valid)
    {
        ctx->ch_status_bits.static_sample_valid = enable;

        if (enable)
        {
            if (!ctx->ch_status_bits.light_sts)
            {

                for (led_type_e rgb = LED_R; rgb < LED_TYPE_MAX; rgb++)
                {
                    ll_pwm_channel_threshold_config((ll_pwm_channel_e)ctx->channel.rgb[rgb], 0, 0);
                }
            }
        }

        uint32_t pwm_freq = (enable && !ctx->ch_status_bits.light_sts) ? LED_STATIC_SAMPLE_FREQ : LED_DRIVER_PWM_FREQ;

        for (led_type_e rgb = LED_R; rgb < LED_TYPE_MAX; rgb++)
        {
            ll_pwm_channel_period_config((ll_pwm_channel_e)ctx->channel.rgb[rgb], pwm_freq);
        }

        if (!ctx->ch_status_bits.light_sts)
        {
            ll_led_enable(true);
            ll_pwm_enable(LL_PWM_BUS_0, true);
        }
    }
}

#if defined (__TCPL08X__)
/********************************************************
** \brief   pal_led_hsw_count_value_get
**
** \param   uint16_t*                   value
**
** \retval  None
*********************************************************/
void pal_led_hsw_count_value_get(uint16_t *value)
{
    ll_pwm_counter_value_get(PWM_CNT_SOURCE_CNT1, value);
}
#endif

/********************************************************
** \brief   pal_led_channel_mux_get
**
** \param   led_channel_e               channel
** \param   uint8_t**                   channel_mux
**
** \retval  None
*********************************************************/
void pal_led_channel_mux_get(led_channel_e channel, uint8_t **channel_mux)
{
    if (channel >= CFG_SUPPORT_RGB_NUM)
    {
        return;
    }

    led_control_context_t *ctx = &led_ctrl_context[channel];

    if (NULL == ctx)
    {
        return;
    }

#if ((defined (__TCPL03X__) || defined (__TCPL04X__)) && CFG_SUPPORT_CURRRENT_EX)
    *channel_mux = ctx->mux_channel.rgb;
#else

    *channel_mux = ctx->channel.rgb;
#endif
}

/********************************************************
** \brief   pal_led_channel_switch
**
** \param   led_channel_e               channel
**
** \retval  None
*********************************************************/
void pal_led_channel_switch(led_channel_e channel)
{

}

const ctrl_manager_instance_t led_ctrl_instance_def =
{
    .ctrl_register = pal_led_register,
    .ctrl_enable = pal_led_enable,
    .ctrl_break = pal_led_break,
    .ctrl_duty_set = pal_led_duty_cycle_set,
    .ctrl_duty_get = pal_led_duty_cycle_get,
    .ctrl_current_set = pal_led_current_set,
    .ctrl_current_get = pal_led_current_get,
    .ctrl_static_pnvolt_set = pal_led_static_pnvolt_set,
    .ctrl_channel_mux_get = pal_led_channel_mux_get,
};
