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

#include "tcpl04x_ll_led.h"

#define LL_LED_LDO_READY_TIMEOUT_CYCLES    (DEFAULT_SYSTEM_CLOCK / 100U)

static ll_status_e ll_led_ldo_ready_wait(void)
{
    uint32_t timeout = LL_LED_LDO_READY_TIMEOUT_CYCLES;

    while (PWM->LED_CTRL_F.LED_LDO_RDY == 0U)
    {
        if (timeout-- == 0U)
        {
            return LL_TIMEOUT;
        }
    }

    return LL_OK;
}

typedef union
{
    struct
    {
        uint32_t led_lc_diag_en     : 1 ;
        uint32_t led_lc_dispu_en    : 1 ;
        uint32_t reserved1          : 2 ;
        uint32_t led_lc_iout_sel    : 4 ;
        uint32_t led_lc_sel_tr      : 2 ;
        uint32_t led_lc_sel_tf      : 2 ;
        uint32_t reserved0          : 20;
    };
    uint32_t reg;
} pwm_led_lc_ctrl_reg_t;

/********************************************************
** \brief   ll_led_init
**
** \param   None
**
** \retval  None
*********************************************************/
void ll_led_init(void)
{
    /* TCPL04B LED diagnostics require the ADC reference buffer. */
    ADC->CTRL0_F.VREFBUF_EN = true;
}

/********************************************************
** \brief   ll_led_driver_current_config
**
** \param   ll_led_channel_e            channel
** \param   ll_led_driver_current_e        current
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_driver_current_config(ll_led_channel_e channel, ll_led_driver_current_e current)
{
    if (channel >= LL_LED_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    pwm_led_lc_ctrl_reg_t *regs[] =
    {
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC0_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC1_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC2_CTRL
    };

    regs[channel]->led_lc_dispu_en = 0;
    regs[channel]->led_lc_iout_sel = current;

    return LL_OK;
}

/********************************************************
** \brief   ll_led_driver_current_get
**
** \param   ll_led_channel_e            channel
** \param   ll_led_driver_current_e        *current
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_driver_current_get(ll_led_channel_e channel, ll_led_driver_current_e *current)
{
    if (channel >= LL_LED_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    pwm_led_lc_ctrl_reg_t *regs[] =
    {
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC0_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC1_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC2_CTRL
    };

    *current = regs[channel]->led_lc_iout_sel;

    return LL_OK;
}

/********************************************************
** \brief   ll_led_diag_current_config
**
** \param   ll_led_channel_e        channel
** \param   led_diag_current_e      current
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_diag_current_config(ll_led_channel_e channel, led_diag_current_e current)
{
    if (channel >= LL_LED_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_led_fall_rise_time_set
**
** \param   ll_led_channel_e        channel
** \param   led_rise_sr_time_e      rise_time
** \param   led_fall_sr_time_e      fall_time
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_fall_rise_time_set(ll_led_channel_e channel, led_rise_sr_time_e rise_time, led_fall_sr_time_e fall_time)
{
    if (channel >= PWM_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    pwm_led_lc_ctrl_reg_t *regs[] =
    {
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC0_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC1_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC2_CTRL
    };

    regs[channel]->led_lc_sel_tr = rise_time;
    regs[channel]->led_lc_sel_tf = fall_time;

    return LL_OK;
}

/********************************************************
** \brief   ll_led_diagnose_enable
**
** \param   ll_led_channel_e        channel
** \param   bool                    enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_led_diagnose_enable(ll_led_channel_e channel, bool enable)
{
    if (channel >= LL_LED_CHANNEL_MAX)
    {
        return LL_ERROR;
    }

    pwm_led_lc_ctrl_reg_t *regs[] =
    {
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC0_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC1_CTRL,
        (pwm_led_lc_ctrl_reg_t *) &PWM->LED_LC2_CTRL
    };

    regs[channel]->led_lc_diag_en = enable ? 1 : 0;

    return LL_OK;
}

/********************************************************
** \brief   ll_led_enable
**
** \param   bool            enable
**
** \retval  None
*********************************************************/
ll_status_e ll_led_enable(bool enable)
{
    if (!enable)
    {
        PWM->LED_CTRL_F.LED_EN = 0U;
        PWM->LED_CTRL_F.LED_LDO5V_EN = 0U;
        return LL_OK;
    }

    /* Gate the current sinks until the 5V LED supply is confirmed ready. */
    PWM->LED_CTRL_F.LED_EN = 0U;
    PWM->LED_CTRL_F.LED_LDO5V_EN = 1U;

    if (LL_OK != ll_led_ldo_ready_wait())
    {
        PWM->LED_CTRL_F.LED_LDO5V_EN = 0U;
        return LL_TIMEOUT;
    }

    PWM->LED_CTRL_F.LED_EN = 1U;
    return LL_OK;
}


/********************************************************
** \brief   ll_led_break_set
**
** \param   bool            enable
**
** \retval  None
*********************************************************/
ll_status_e ll_led_break_set(bool enable)
{
    return ll_led_enable(!enable);
}



