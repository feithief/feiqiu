/**
 *****************************************************************************
 * @brief   pwm driver source file.
 *
 * @file    tcpl04x_ll_pwm.c
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

#include "tcpl04x_ll_pwm.h"

static ISR_FUNC_CALLBACK pwm_isr_callback = NULL;

/********************************************************
** \brief   ll_pwm_clk_config
**
** \param   ll_clk_config_t*    config
**
** \retval  None
*********************************************************/
static void ll_pwm_clk_config(ll_clk_config_t *config)
{
    CRG_CONFIG_UNLOCK();

    CRG->PWM_CLKRST_CTRL_F.PCLK_EN_PWM = 1;
    CRG->PWM_CLKRST_CTRL_F.FCLK_EN_PWM = 1;
    CRG->PWM_CLKRST_CTRL_F.FCLK_SEL_PWM = config->clk_source;
    CRG->PWM_CLKRST_CTRL_F.FCLK_DIV_PWM = config->fclk_div;

    CRG_CONFIG_LOCK();
}

/********************************************************
** \brief   ll_pwm_isr_config
**
** \param   ll_isr_config_t*    config
** \param   ISR_FUNC_CALLBACK   callback
**
** \retval  None
*********************************************************/
static void ll_pwm_isr_config(ll_isr_config_t *config, ISR_FUNC_CALLBACK callback)
{
    PWM->ICR |= PWM_INIT_ALL_FLAG;

    if (config->isr_enable)
    {
        PWM->IMR &= ~(config->isr & PWM_INIT_ALL_FLAG);
        pwm_isr_callback = callback;
        NVIC_SetPriority(PWM_IRQn, config->priority);
    }
    else
    {
        PWM->IMR = PWM_INIT_ALL_FLAG;
        pwm_isr_callback = NULL;
    }
}

/********************************************************
** \brief   ll_pwm_isr_flag_enable
**
** \param   ll_pwm_bus_e    bus
** \param   uint32_t        isr
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_isr_flag_enable(ll_pwm_bus_e bus, uint32_t isr, bool enable)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    PWM->ICR |= PWM_INIT_ALL_FLAG;

    if (enable)
    {
        // PWM->IMR = PWM_INIT_ALL_FLAG;
        PWM->IMR &= ~(isr & PWM_INIT_ALL_FLAG);  //enable isr_flag
    }
    else
    {
        PWM->IMR |= (isr & PWM_INIT_ALL_FLAG);   //disable isr_flag
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_isr_enable
**
** \param   ll_pwm_bus_e    bus
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_isr_enable(ll_pwm_bus_e bus, bool enable)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    PWM->ICR |= PWM_INIT_ALL_FLAG;
    NVIC_ClearPendingIRQ(PWM_IRQn);

    if (enable)
    {
        NVIC_EnableIRQ(PWM_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(PWM_IRQn);
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_isr_clear
**
** \param   ll_pwm_bus_e    bus
** \param   uint32_t        flag
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_isr_clear(ll_pwm_bus_e bus, uint32_t flag)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    PWM->ICR |= flag;

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_isr_flag_get
**
** \param   ll_pwm_bus_e    bus
** \param   uint32_t*       flag
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_isr_flag_get(ll_pwm_bus_e bus, uint32_t *flag)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    *flag = PWM->ISR;

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_enable
**
** \param   ll_pwm_bus_e    bus
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_enable(ll_pwm_bus_e bus, bool enable)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    PWM->CTRL_F.CH_SYNC_EN = (enable ? 1 : 0);
    PWM->CNT_CTRL_F.CNT0_EN = (enable ? 1 : 0);

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_channel_threshold_config
**
** \param   ll_pwm_channel_e   channel
** \param   uint16_t        threshold_h
** \param   uint16_t        threshold_l
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_channel_threshold_config(ll_pwm_channel_e channel, uint16_t threshold_h, uint16_t threshold_l)
{
    if (channel >= PWM_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    /* channel*4 start from PWM->CH0_PWM_CFG */
    PWM_CH0_PWM_CFG_FIELD_T *pwm_reg  = (PWM_CH0_PWM_CFG_FIELD_T *)(PWM_CH0_PWM_CFG_ADDR + (channel << 2));

    pwm_reg->LT0 = threshold_h ? threshold_l : threshold_l + 1;
    pwm_reg->HT0 = threshold_h;

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_channel_period_config
**
** \param   ll_pwm_channel_e   channel
** \param   uint16_t        period
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_channel_period_config(ll_pwm_channel_e channel, uint16_t period)
{
    if (channel >= PWM_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    PWM->CNT_CFG_F.PERIOD0 = period;

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_high_threshold_get
**
** \param   ll_pwm_channel_e   channel
** \param   uint16_t*       value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_high_threshold_get(ll_pwm_channel_e channel, uint16_t *value)
{
    if (channel >= PWM_CHANNEL_MAX || value == NULL)
    {
        return LL_ERROR;
    }


    PWM_CH0_PWM_CFG_FIELD_T *pwm_reg  = (PWM_CH0_PWM_CFG_FIELD_T *)(PWM_CH0_PWM_CFG_ADDR + (channel << 2));

    *value = pwm_reg->HT0;

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_channel_counter_get
**
** \param   ll_pwm_channel_e   channel
** \param   uint16_t*       value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_channel_counter_get(ll_pwm_channel_e channel, uint16_t *value)
{
    if (channel >= PWM_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_break_set
**
** \param   ll_pwm_bus_e    bus
** \param   bool            enable
**
** \retval  None
*********************************************************/
void ll_pwm_break_set(ll_pwm_bus_e bus, bool enable)
{
    PWM->CTRL_F.PWM_STP = enable;
}

/********************************************************
** \brief   ll_pwm_status_get
**
** \param   ll_pwm_channel_e   channel
** \param   uint8_t*        value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_status_get(ll_pwm_channel_e channel, uint8_t *value)
{
    if (channel >= PWM_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    *value = PWM->STATUS  & 0x3F;

    return LL_OK;
}

/********************************************************
** \brief   ll_led_exchannel_enable
**
** \param   ll_pwm_bus_e        bus
** \param   bool                enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_exchannel_enable(ll_pwm_bus_e bus, bool enable)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    PWM->CH_CTRL_F.CH3_EN = (enable ? 1 : 0);
    PWM->CNT_CTRL_F.CNT1_EN = (enable ? 1 : 0);
    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_deinit
**
** \param   ll_pwm_bus_e        bus
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_deinit(ll_pwm_bus_e bus)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    CRG_CONFIG_UNLOCK();
    CRG->PWM_CLKRST_CTRL_F.RST_PWM = 1;
    __NOP();
    __NOP();
    CRG->PWM_CLKRST_CTRL_F.RST_PWM = 0;
    __NOP();
    __NOP();
    CRG_CONFIG_LOCK();

    PWM->IMR |= PWM_INIT_ALL_FLAG;
    PWM->ICR |= PWM_INIT_ALL_FLAG;

    pwm_isr_callback = NULL;

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_init
**
** \param   ll_pwm_bus_e        bus
** \param   pwm_config_t*       config
** \param   ISR_FUNC_CALLBACK   callback
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_init(ll_pwm_bus_e bus, pwm_config_t *config, ISR_FUNC_CALLBACK callback)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    ll_pwm_deinit(bus);

    ll_pwm_clk_config(&config->clk_cfg);

    if (config->pwm_sync_mode != PWM_CH_SYNC_MODE_NULL)
    {
        PWM->CTRL_F.CH_SYNC_EN = 1;
        PWM->CTRL_F.CH_SYNC_SEL = config->pwm_sync_mode;
    }
    else
    {
        PWM->CTRL_F.CH_SYNC_EN = 0;
    }

    PWM->CTRL_F.AUTO_LD_EN = 1;
    PWM->CTRL_F.BRK_EN = 1;
    PWM->CTRL_F.PWM_SW_EN = false;

    // ADC->CTRL0_F.VREFBUF_EN = true;  /* 5V out same as tcpl01x adc bias  */
    ll_pwm_isr_config(&config->isr_cfg, callback);

    return LL_OK;
}

/********************************************************
** \brief   ll_pwm_count_config
**
** \param   ll_pwm_count_index_e        index
** \param   ll_pwm_count_config_t*    config
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_count_config(ll_pwm_count_index_e index, ll_pwm_count_config_t *config)
{
    if (index >= PWM_COUNT_INDEX_MAX)
    {
        return LL_ERROR;
    }


    if (config->sync_mode != PWM_COUNT_SYNC_MODE_NULL)
    {
        PWM->CTRL_F.SYNC_EN = config->sync_mode;
    }

    switch (index)
    {
        case PWM_COUNT_INDEX_0:   /* pwm cnt0 control reg */

            PWM->CNT_CTRL_F.CNT0_MODE = config->count_mode;
            PWM->CNT_CTRL_F.CNT0_ONE_SHOT_EN = config->one_shot_enable;;
            /* Enable by cnt0_en =1 and sync_en=1 */
            PWM->CNT_CTRL_F.CNT0_EN_MODE = 0;
            PWM->CNT_CFG_F.PERIOD0 = config->period;

            break;

        case PWM_COUNT_INDEX_1:   /* pwm cnt1 control reg */
            PWM->CNT_CTRL_F.CNT1_MODE = config->count_mode;
            PWM->CNT_CTRL_F.CNT1_ONE_SHOT_EN = config->one_shot_enable;;
            /* Enable by cnt0_en =1 and sync_en=1 */
            PWM->CNT_CTRL_F.CNT1_EN_MODE = 0;
            PWM->CNT_CFG_F.PERIOD1 = config->period;
            break;

        default:
            break;
    }

    return LL_OK;
}


/********************************************************
** \brief   ll_pwm_channel_config
**
** \param   ll_pwm_channel_e            channel
** \param   ll_pwm_channel_config_t*    config
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_pwm_channel_config(ll_pwm_channel_e channel, ll_pwm_channel_config_t *config)
{
    if (channel >= PWM_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    /* CH CNT_SEL */
    PWM->CH_CTRL &= ~(0x1UL << (channel + 16));
    PWM->CH_CTRL |= config->cnt_source << (channel + 16);

    /* CH_PTY */
    PWM->CH_CTRL &= ~(0x1UL << (channel + 4));
    PWM->CH_CTRL |= config->polarity << (channel + 4);

    PWM->CH_CTRL &= ~(0x3UL << ((channel << 1) + 8));
    PWM->CH_CTRL |= (config->out_mode << ((channel << 1) + 8));

    PWM->CH_CTRL |= (1UL << channel);

    return LL_OK;
}


/********************************************************
** \brief   ll_led_switch_init
**
** \param   ll_pwm_bus_e        bus
** \param   uint16_t            period
** \param   pwm_polarity_e      polarity
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_exchannel_init(ll_pwm_bus_e bus, uint16_t period, pwm_polarity_e polarity)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    PWM->CNT_CTRL_F.CNT1_MODE = PWM_COUNT_MODE_UP;
    PWM->CNT_CTRL_F.CNT1_ONE_SHOT_EN = 0;
    PWM->CNT_CTRL_F.CNT1_EN_MODE = 1; /* Enable by cnt1_en =1 and sync_en=1 */
    PWM->CNT_CFG_F.PERIOD1 = period;

    PWM->CH_CTRL_F.CH3_CNT_SEL = 1; /* sel counter1 */
    PWM->CH_CTRL_F.CH3_PTY = polarity;

    PWM->CH_CTRL_F.CH3_MODE = PWM_MODE_INDEPENDENT;

    PWM->CH3_PWM_CFG_F.HT3 = period >> 2;
    PWM->CH3_PWM_CFG_F.LT3 = 0;

    return LL_OK;
}

/********************************************************
** \brief   ll_led_switch_init
**
** \param   ll_pwm_bus_e        bus
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_switch_init(ll_pwm_bus_e bus)
{
    if (bus >= LL_PWM_BUS_MAX)
    {
        return LL_ERROR;
    }

    PWM->CTRL_F.PWM_SW_EN = 1; /* PWM software enable */
    ll_gpio_afio_config(GPIO_PIN_0, AFIO_MUX_2);

    ll_led_enable(true);
    ll_pwm_enable(bus, true);

    return LL_OK;
}

/********************************************************
** \brief   PWM_IRQHandler
**
** \param   None
**
** \retval  None
*********************************************************/
void PWM_IRQHandler(void)
{
    uint32_t isr = PWM->ISR;

    if (isr & PWM_INIT_ALL_FLAG)
    {
        if (NULL != pwm_isr_callback)
        {
            pwm_isr_callback(isr);
        }

        PWM->ICR |= isr;
    }
}
