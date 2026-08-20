/**
 *****************************************************************************
 * @brief   gpio driver source file.
 *
 * @file    tcpl04x_ll_gpio.c
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

#include "tcpl04x_ll_gpio.h"

static ISR_FUNC_CALLBACK gpio_isr_callback = NULL;
/********************************************************
** \brief   ll_gpio_clk_config
**
** \param   None
**
** \retval  None
*********************************************************/
static void ll_gpio_clk_config(void)
{
    CRG_CONFIG_UNLOCK();

    CRG->GPIO_CLKRST_CTRL_F.FCLK_EN_GPIO = 1;
    CRG->GPIO_CLKRST_CTRL_F.PCLK_EN_GPIO = 1;
    CRG->PINMUX_CLKRST_CTRL_F.PCLK_EN_PINMUX = 1;

    CRG_CONFIG_LOCK();
}

/********************************************************
** \brief   ll_gpio_isr_config
**
** \param   gpio_trigger_flag_e     flag
** \param   gpio_pin_e              gpio_pin
**
** \retval  None
*********************************************************/
static void ll_gpio_isr_config(gpio_trigger_flag_e flag, gpio_pin_e gpio_pin)
{
    switch (flag)
    {
        case GPIO_TRIGGER_FALLING_EDGE:
            GPIO->INT_POL_SEL_F.INT_POL_SEL &= ~(1 << gpio_pin);
            GPIO->INT_TYP_SEL_F.INT_TYP_SEL |= 1 << gpio_pin;
            break;

        case GPIO_TRIGGER_LOW_LEVEL:
            GPIO->INT_POL_SEL_F.INT_POL_SEL &= ~(1 << gpio_pin);
            GPIO->INT_TYP_SEL_F.INT_TYP_SEL &= ~(1 << gpio_pin);
            break;

        case GPIO_TRIGGER_RISING_EDGE:
            GPIO->INT_POL_SEL_F.INT_POL_SEL |= 1 << gpio_pin;
            GPIO->INT_TYP_SEL_F.INT_TYP_SEL |= 1 << gpio_pin;
            break;

        case GPIO_TRIGGER_HIGH_LEVEL:
            GPIO->INT_POL_SEL_F.INT_POL_SEL |= 1 << gpio_pin;
            GPIO->INT_TYP_SEL_F.INT_TYP_SEL &= ~(1 << gpio_pin);
            break;

        case GPIO_TRIGGER_RISING_FALLING_EDGE:
            GPIO->INT_EDGE_EN_F.INT_EDGE_EN |= 1 << gpio_pin;
            break;

        default:
            break;
    }
}

/********************************************************
** \brief   ll_gpio_deinit
**
** \param   None
**
** \retval  None
*********************************************************/
void ll_gpio_deinit(void)
{
    CRG_CONFIG_UNLOCK();

    CRG->GPIO_CLKRST_CTRL_F.RST_GPIO = 1 ;
    __NOP();
    __NOP();
    CRG->GPIO_CLKRST_CTRL_F.RST_GPIO = 0 ;
    __NOP();
    __NOP();

    CRG_CONFIG_LOCK();
}

/********************************************************
** \brief   ll_gpio_init
**
** \param   gpio_config_t*      config
** \param   ISR_FUNC_CALLBACK   callback
**
** \retval  None
*********************************************************/
void ll_gpio_init(gpio_config_t *config, ISR_FUNC_CALLBACK callback)
{
    uint32_t reg_val = 0;
    uint32_t reg = (config->gpio_pin < GPIO_PIN_6) ? PINMUX_IO0_CFG_ADDR : PINMUX_LED0_CFG_ADDR;

    ll_gpio_clk_config();

    if (config->mode == GPIO_MODE_IN_PP || config->mode == GPIO_MODE_IN_OD)
    {
        GPIO->OUTEN_CLR_F.OUTEN_CLR |= 1 << config->gpio_pin;
    }
    else if (config->mode == GPIO_MODE_OUT_PP || config->mode == GPIO_MODE_OUT_OD)
    {
        GPIO->OUTEN_SET_F.OUTEN_SET |= 1 << config->gpio_pin;
    }

    reg_val = PINMUX_IO0_CFG_IO0_SRC_SEL_SET(config->afio);

    if (config->pull_mode == GPIO_PULL_DOWN)
    {
        reg_val |= PINMUX_IO0_CFG_IO0_PD_SET(1);
    }
    else if (config->pull_mode == GPIO_PULL_UP)
    {
        reg_val |= PINMUX_IO0_CFG_IO0_PU_SET(1);
    }

    reg_val  |= PINMUX_IO0_CFG_IO0_PULL_SEL_SET(config->pull_down_type);

    /*open drain bit10 and open source bit11 will be set in a single step*/
    if (config->mode == GPIO_MODE_IN_OD || config->mode == GPIO_MODE_OUT_OD)
    {
        reg_val |= PINMUX_IO0_CFG_IO0_OD_SET(1);
    }

    reg += ((config->gpio_pin % GPIO_PIN_6) * 4);

    *((volatile uint32_t *)(reg)) = reg_val;

    if (config->trigger_flag)
    {
        ll_gpio_isr_config(config->trigger_flag, config->gpio_pin);
        GPIO->INT_CLR = 0xFFFFFFFF;
        GPIO->INT_RAW_STATUS = 0xFFFFFFFF;
        gpio_isr_callback = callback;
        NVIC_SetPriority(GPIO_IRQn, 2);
    }
}

/********************************************************
** \brief   ll_gpio_read
**
** \param   gpio_pin_e  gpio_pin
**
** \retval  bool        true for false
*********************************************************/
bool ll_gpio_read(gpio_pin_e gpio_pin)
{
    return (GPIO->DATAIN_F.DATAIN & (1 << gpio_pin));
}

/********************************************************
** \brief   ll_gpio_output
**
** \param   gpio_pin_e  gpio_pin
** \param   bool        state:true-H_LEVEL, false-L_LEVEL
**
** \retval  None
*********************************************************/
void ll_gpio_output(gpio_pin_e gpio_pin, bool state)
{
    if (state)
    {
        GPIO->DATAOUT_F.DATAOUT |= 1 << gpio_pin;
    }
    else
    {
        GPIO->DATAOUT_F.DATAOUT &= ~(1 << gpio_pin);
    }
}

/********************************************************
** \brief   ll_gpio_toggle
**
** \param   gpio_pin_e  gpio_pin
**
** \retval  None
*********************************************************/
void ll_gpio_toggle(gpio_pin_e gpio_pin)
{
    GPIO->DATAOUT_F.DATAOUT ^= 1 << gpio_pin;
}

/********************************************************
** \brief   ll_gpio_isr_enable
**
** \param   gpio_pin_e  gpio_pin
** \param   bool        enable
**
** \retval  None
*********************************************************/
void ll_gpio_isr_enable(gpio_pin_e gpio_pin, bool enable)
{
    NVIC_ClearPendingIRQ(GPIO_IRQn);

    if (!enable)
    {
        GPIO->INT_MSK_F.INT_MSK |= 1 << gpio_pin;
        NVIC_DisableIRQ(GPIO_IRQn);
    }
    else
    {
        GPIO->INT_MSK_F.INT_MSK &= ~(1 << gpio_pin);
        NVIC_EnableIRQ(GPIO_IRQn);
    }
}

/********************************************************
** \brief   ll_gpio_interrupt_flag_get
**
** \param   gpio_pin_e  gpio_pin
**
** \retval  bool
*********************************************************/
bool ll_gpio_interrupt_flag_get(gpio_pin_e gpio_pin)
{
    return (GPIO->INT_STATUS_F.INT_STATUS & (1 << gpio_pin));
}

/********************************************************
** \brief   ll_gpio_interrupt_clear
**
** \param   gpio_pin_e  gpio_pin
**
** \retval  None
*********************************************************/
void ll_gpio_interrupt_clear(gpio_pin_e gpio_pin)
{
    GPIO->INT_CLR_F.INT_CLR |= 1 << gpio_pin;
}

/********************************************************
** \brief   ll_gpio_afio_config
**
** \param   gpio_pin_e          gpio_pin
** \param   gpio_afio_mux_e     afio_mux
**
** \retval  None
*********************************************************/
void ll_gpio_afio_config(gpio_pin_e gpio_pin, gpio_afio_mux_e afio_mux)
{
    uint32_t reg_val = 0;
    uint32_t reg = (gpio_pin < GPIO_PIN_6) ? PINMUX_IO0_CFG_ADDR : PINMUX_LED0_CFG_ADDR;

    reg += (gpio_pin % GPIO_PIN_6) * 4 ;

    reg_val = *((volatile uint32_t *)(reg));

    reg_val &= ~(7 << PINMUX_IO0_CFG_IO0_SRC_SEL_SHIFT);
    reg_val |= afio_mux;
    *((volatile uint32_t *)(reg)) = reg_val;

    /* For tcpl04b */
    if (GPIO_PIN_0 == gpio_pin && AFIO_MUX_4 == afio_mux)
    {
        TEST->TEST_LOCK = 0x76543210;
        TEST->TEST_BUS_CTRL_F.TOP_TP_HBYTE_SEL = 0x0;
        TEST->TEST_BUS_CTRL_F.TOP_TP_HNIBBLE_SEL = 0x1;
        TEST->TEST_BUS_CTRL_F.TOP_TP_IP_SEL = 0x2;
        TEST->TEST_BUS_CTRL_F.IP_TP_SEL = 0x3;
        TEST->TEST_BUS_CTRL_F.TOP_TP_OUT_SEL = 0x2;
        TEST->TEST_BUS_CTRL_F.TOP_TP_EN = 0x1;
        TEST->TEST_LOCK = 0xFEDCBA98;
    }
}

/********************************************************
** \brief   This function configures an output gpio to keep in a specified
**          output fixed value during low power mode
**
** \param   gpio_pin_e          gpio_pin
** \param   bool                keep_high : true-high, false-low
**
** \retval  None
*********************************************************/
void ll_gpio_ano_lpm_config(gpio_pin_e gpio_pin, bool keep_high)
{
    /*only GPIO1,2,3,4 are supported with AON IO*/
    if (gpio_pin < GPIO_PIN_1 || gpio_pin > GPIO_PIN_4)
    {
        return;
    }

    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->AON_IO_CTRL_F.AON_IO_OUT_SEL = gpio_pin - 1;
    ASYSCFG->AON_IO_CTRL_F.AON_IO_OUT_VAL = keep_high;
    ASYSCFG->AON_IO_CTRL_F.AON_IO_OUT_EN = 1;

    ASYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   This function enables or disabled Pad Reset function on GPIO4
**
** \param   bool     enable
**
** \retval  None
*********************************************************/
void ll_gpio_reset_enable(bool enable)
{
    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->IO4_RST_CTRL = enable ? 0x12345678 : 0xAA55AA55;

    ASYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   GPIO_IRQHandler
**
** \param   None
**
** \retval  None
*********************************************************/
void GPIO_IRQHandler(void)
{
    uint32_t isr = GPIO->INT_STATUS_F.INT_STATUS;

    if (gpio_isr_callback)
    {
        gpio_isr_callback(isr);
    }

    GPIO->INT_CLR_F.INT_CLR |= isr; /* clear the enable flag */
}
