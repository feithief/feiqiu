/**
 *****************************************************************************
 * @brief   sci driver source file.
 *
 * @file    tcpl04x_ll_sci.c
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

#include "tcpl04x_ll_sci.h"

#define LL_SCI_FIFO_WAIT_TIMEOUT_CYCLES    (DEFAULT_SYSTEM_CLOCK / 1000U)

static bool ll_sci_tx_fifo_ready_wait(LIN_SCI_REG_TypeDef *sci_reg)
{
    uint32_t timeout = LL_SCI_FIFO_WAIT_TIMEOUT_CYCLES;

    while (sci_reg->STATUS_F.TX_FIFO_FULL)
    {
        if (timeout-- == 0U)
        {
            return false;
        }
    }

    return true;
}

__attribute__((unused))static ISR_FUNC_CALLBACK sci_isr_callback[LL_SCI_BUS_MAX] = {NULL};

#define LIN_ISR_FLAG       (0x7FFFFFUL)
#define LIN_CHECKSUM_USE_SW     0

static inline LIN_SCI_REG_TypeDef *get_sci_reg(ll_sci_bus_e bus)
{
    return (LIN_SCI_REG_TypeDef *)((LL_SCI_BUS_0 == bus) ? LIN_SCI_BASE_ADDR
                                   : LIN_SCI1_BASE_ADDR);
}

static inline CRG_LIN_SCI_CLKRST_CTRL_FIELD_T *get_crg_reg(ll_sci_bus_e bus)
{
    return (CRG_LIN_SCI_CLKRST_CTRL_FIELD_T
            *)((LL_SCI_BUS_0 == bus) ? CRG_LIN_SCI_CLKRST_CTRL_ADDR
               : CRG_LIN_SCI1_CLKRST_CTRL_ADDR);
}

/********************************************************
** \brief   ll_sci_clk_config
**
** \param   ll_sci_bus_e        bus
** \param   ll_clk_config_t*    config
**
** \retval  None
*********************************************************/
static void ll_sci_clk_config(ll_sci_bus_e bus, ll_clk_config_t *config)
{
    CRG_CONFIG_UNLOCK();

    CRG_LIN_SCI_CLKRST_CTRL_FIELD_T  *crg_reg = get_crg_reg(bus);

    /* lin baudrate change, need reset */
    crg_reg->RST_LIN_SCI = 1;
    crg_reg->RST_LIN_SCI = 0;

    crg_reg->PCLK_EN_LIN_SCI = 1;
    crg_reg->FCLK_EN_LIN_SCI = 1;
    crg_reg->FCLK_DIV_LIN_SCI = config->fclk_div;

    CRG_CONFIG_LOCK();
}

/********************************************************
** \brief   ll_sci_gpio_config
**
** \param   ll_sci_bus_e        bus
** \param   ll_sci_mode_e       mode
**
** \retval  ll_status_e
*********************************************************/
static ll_status_e ll_sci_gpio_config(ll_sci_bus_e bus, ll_sci_mode_e mode)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    if (LL_SCI_BUS_0 == bus)
    {
        if (SCI_MODE_UART == mode)
        {
            ll_gpio_afio_config(GPIO_PIN_7, AFIO_MUX_2);
            ll_gpio_afio_config(GPIO_PIN_8, AFIO_MUX_2);
        }

    }
    else
    {
        ll_gpio_afio_config(GPIO_PIN_3, AFIO_MUX_5);
        ll_gpio_afio_config(GPIO_PIN_4, AFIO_MUX_5);
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_sci_state_clear
**
** \param   ll_sci_bus_e            bus
** \param   ll_sci_clear_type_e     type
**
** \retval  None
*********************************************************/
void ll_sci_state_clear(ll_sci_bus_e bus, ll_sci_clear_type_e type)
{
    LIN_SCI_REG_TypeDef *sci_reg = NULL;

    if (!type || (bus >= LL_SCI_BUS_MAX))
    {
        return;
    }

    sci_reg = get_sci_reg(bus);

    if (type & SCI_CLEAR_TX_FIFO)
    {
        sci_reg->CTRL_F.TX_FIFO_CLR = 1;
    }

    if (type & SCI_CLEAR_RX_FIFO)
    {
        sci_reg->CTRL_F.RX_FIFO_CLR = 1;
    }

    if (type & SCI_CLEAR_TX_ABORT)
    {
        sci_reg->CTRL_F.TX_ABORT = 1;
    }

    if (type & SCI_CLEAR_RX_ABORT)
    {
        sci_reg->CTRL_F.RX_ABORT = 1;
    }
}

/********************************************************
** \brief   ll_sci_contrl_config
**
** \param   ll_sci_bus_e        bus
** \param   ll_sci_mode_e       mode
**
** \retval  None
*********************************************************/
static void ll_sci_contrl_config(ll_sci_bus_e bus, ll_sci_mode_e mode)
{
    uint32_t reg_val = 0;
    LIN_SCI_REG_TypeDef *sci_reg = get_sci_reg(bus);

    /* setup master send config */
    ll_sci_state_clear(bus, (ll_sci_clear_type_e)(SCI_CLEAR_TX_FIFO | SCI_CLEAR_RX_FIFO));

    if (SCI_MODE_UART == mode)
    {
        reg_val |= LIN_SCI_CTRL_GLB_EN_SET(1) | LIN_SCI_CTRL_TX_EN_SET(1) | LIN_SCI_CTRL_RX_EN_SET(1);
        reg_val &= (LIN_SCI_CTRL_TX_NUM_MODE_CLR & LIN_SCI_CTRL_TX_NUM_CLR);
        reg_val &= (LIN_SCI_CTRL_RX_NUM_MODE_CLR & LIN_SCI_CTRL_RX_NUM_CLR);
    }
    else
    {
        reg_val |= LIN_SCI_CTRL_GLB_EN_SET(1) | LIN_SCI_CTRL_RX_EN_SET(1) | LIN_SCI_CTRL_AUTO_BAUD_EN_SET(1);
#if !LIN_CHECKSUM_USE_SW
        reg_val |= LIN_SCI_CTRL_CHKSUM_EN_SET(1) | LIN_SCI_CTRL_CHKSUM_TYPE_SET(1) ; //crc
        reg_val |= LIN_SCI_CTRL_TX_NUM_MODE_SET(1) | LIN_SCI_CTRL_TX_NUM_SET(8);
#else
        reg_val |= LIN_SCI_CTRL_CHKSUM_EN_SET(0);
        reg_val |= LIN_SCI_CTRL_TX_NUM_MODE_CLR | LIN_SCI_CTRL_TX_NUM_SET(8);
#endif
        reg_val |= LIN_SCI_CTRL_RX_NUM_MODE_SET(1) | LIN_SCI_CTRL_RX_NUM_SET(8);
        reg_val |= LIN_SCI_CTRL_SHORT_GND_DET_EN_SET(1) | LIN_SCI_CTRL_BIT_ERR_DET_EN_SET(1);

        if (SCI_MODE_LIN_M == mode)
        {
            reg_val |= LIN_SCI_CTRL_MASTER_EN_SET(1);
        }
        else
        {
            reg_val |= LIN_SCI_CTRL_TX_WAIT_FIFO_VLD_EN_SET(1);
        }
    }

    sci_reg->CTRL = reg_val;
}

/********************************************************
** \brief   ll_sci_isr_config
**
** \param   ll_sci_bus_e          bus
** \param   ll_isr_config_t*      config
** \param   ISR_FUNC_CALLBACK     callback
**
** \retval  None
*********************************************************/
static void ll_sci_isr_config(ll_sci_bus_e bus, ll_isr_config_t *config, ISR_FUNC_CALLBACK callback)
{
    IRQn_Type irq = (LL_SCI_BUS_0 == bus) ? LINSCI_IRQn : LINSCI_UART_IRQn;
    LIN_SCI_REG_TypeDef *sci_reg = NULL;

    sci_reg = get_sci_reg(bus);

    sci_reg->ICR |= LIN_ISR_FLAG;

    if (config->isr_enable)
    {
        sci_reg->IMR &= ~(config->isr & LIN_ISR_FLAG);
        sci_isr_callback[bus] = callback;
        NVIC_SetPriority(irq, config->priority);
    }
    else
    {
        sci_reg->IMR |= LIN_ISR_FLAG;
    }
}

/********************************************************
** \brief   ll_sci_deinit
**
** \param   ll_sci_bus_e            bus
**
** \retval  None
*********************************************************/
void ll_sci_deinit(ll_sci_bus_e bus)
{
    IRQn_Type irq;
    CRG_LIN_SCI_CLKRST_CTRL_FIELD_T  *crg_reg;

    if (bus >= LL_SCI_BUS_MAX)
    {
        return;
    }

    crg_reg = get_crg_reg(bus);
    irq = (LL_SCI_BUS_0 == bus) ? LINSCI_IRQn : LINSCI_UART_IRQn;

    CRG_CONFIG_UNLOCK();

    crg_reg->RST_LIN_SCI = 1;
    crg_reg->RST_LIN_SCI = 0;

    CRG_CONFIG_LOCK();

    NVIC_ClearPendingIRQ(irq);
    NVIC_DisableIRQ(irq);

    sci_isr_callback[bus] = NULL;
}

/********************************************************
** \brief   ll_lin_snpd_enable
**
** \param   ll_lin_sbc_e            sbc
** \param   lin_snpd_type_e         type
** \param   bool                    ext_shunt_res
** \param   uint16_t*               cur_th
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_snpd_enable(ll_lin_sbc_e sbc, lin_snpd_type_e type, bool ext_shunt_res, uint16_t *cur_th)
{
    if (LL_LIN_SBC_0 != sbc)
    {
        return LL_ERROR;
    }

    /* ll_lin_aa_disable, need reset */
    LIN_SCI->ANALOG_CTRL = 4;
    ll_lin_rx_delay_set((ll_sci_bus_e)sbc, 0);

    /* Clk deviation time config, Pga ready time config:time base = 20.8ns */
    if (DEFAULT_SYSTEM_CLOCK < 48000000UL)
    {
        if (LIN_SNPD_TYPE_STEPS_2 == (type & 0x0F))
        {
            LIN_SCI->CLK_DEV_TIM_CFG_F.CLK_DEV_TIM = 0x3FF >> 1;
            LIN_SCI->PGA_RDY_TIM_CFG_F.PGA_RDY_TIM =  0xB20 >> 1;
        }
        else
        {
            LIN_SCI->CLK_DEV_TIM_CFG_F.CLK_DEV_TIM = 0x290 >> 1;
            LIN_SCI->CLK_DEV_TIM_CFG_F.CLK_DEV_TIM_DELTA = 0xF0 >> 1;
            LIN_SCI->PGA_RDY_TIM_CFG_F.PGA_RDY_TIM =  0xA50 >> 1;
        }
    }
    else
    {
        if (LIN_SNPD_TYPE_STEPS_2 == (type & 0x0F))
        {
            LIN_SCI->CLK_DEV_TIM_CFG_F.CLK_DEV_TIM = 0x3FF;
            LIN_SCI->PGA_RDY_TIM_CFG_F.PGA_RDY_TIM =  0xB20;
        }
        else
        {
            LIN_SCI->CLK_DEV_TIM_CFG_F.CLK_DEV_TIM = 0x290;
            LIN_SCI->CLK_DEV_TIM_CFG_F.CLK_DEV_TIM_DELTA = 0xF0;
            LIN_SCI->PGA_RDY_TIM_CFG_F.PGA_RDY_TIM =  0xA50;
        }
    }

    if (type >= LIN_SNPD_TYPE_STEPS_SW4) /* SW SNPD*/
    {
        LIN_SCI->ANALOG_CTRL_F.SW_LIN_NORMAL_MODE = 1;

        LIN_SCI->AUTO_ADDR_ANA_CFG = (LIN_SNPD_TYPE_STEPS_2 == (type & 0x0F)) ? 0x047 : 0x042;

        /* Auto addressing control */
        LIN_SCI->AUTO_ADDR_CTRL = LIN_SCI_AUTO_ADDR_CTRL_AUTO_ADDR_EN_SET(1) | \
                                  LIN_SCI_AUTO_ADDR_CTRL_AUTO_ADDR_ANA_EN_SET(1) | \
                                  LIN_SCI_AUTO_ADDR_CTRL_MEAS_STEP_SEL_SET(type);
        LIN_SCI->AUTO_ADDR_CTRL_F.SW_CTRL_EN = 1;
        LIN_SCI->AUTO_ADDR_SW_CTRL_F.SW_LIN_CKOSC = 0;
        LIN_SCI->AUTO_ADDR_SW_CTRL_F.SW_LIN_CKCONV = 0;
        LIN_SCI->AUTO_ADDR_SW_CTRL_F.SW_LIN_PU_RES_EN = 1;

        SYSCFG_CONFIG_UNLOCK();
        SYSCFG->PATCH_BIT_F.PATCH_BIT = 2;
        SYSCFG_CONFIG_LOCK();

    }
    else /* HW SNPD*/
    {
        if (LIN_SNPD_TYPE_STEPS_2 == type)
        {
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP1 = 0b10001;    /* 1.1mA */
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP2 = 0b11000;    /* 1.1mA */
        }
        else if (LIN_SNPD_TYPE_STEPS_3 == type)
        {
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP1 = 0b00001;    /*   1mA */
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP2 = 0b00111;    /*   4mA */
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP3 = 0b01111;    /*   8mA */
        }
        else
        {
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP1 = 0b00000;    /* 0.5mA */
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP2 = 0b00011;    /*   2mA */
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP3 = 0b01001;    /*   5mA */
            LIN_SCI->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP4 = 0b01111;    /*   8mA */
        }

        if (!ext_shunt_res)
        {
            uint32_t reg_val = TRIM->ADC_LINAA_F.LINAA_RS;

            if (reg_val != 0xFFF)
            {
                float cur_th_ratio = (float)reg_val / 800;

                for (uint8_t i = 0; i < 4; i++)
                {
                    cur_th[i] = (uint16_t)((uint32_t)cur_th[i] * cur_th_ratio);
                }
            }
        }

        /* Current threshold for test value 0x043 0x075, 0x0b9, 0x032 */
        LIN_SCI->CUR_TH1_F.CUR_TH_STEP1 = cur_th[0];
        LIN_SCI->CUR_TH1_F.CUR_TH_STEP2 = cur_th[1];
        LIN_SCI->CUR_TH2_F.CUR_TH_STEP3 = cur_th[2];
        LIN_SCI->CUR_TH2_F.CUR_TH_STEP4 = cur_th[3];

        /* Auto addressing analog config  */
        if (ext_shunt_res)
        {
            LIN_SCI->AUTO_ADDR_ANA_CFG = 0x04F;
        }
        else
        {
            LIN_SCI->AUTO_ADDR_ANA_CFG = (LIN_SNPD_TYPE_STEPS_2 == (type & 0x0F)) ? 0x047 : 0x042;
        }

        /* Auto addressing control */
        LIN_SCI->AUTO_ADDR_CTRL = LIN_SCI_AUTO_ADDR_CTRL_AUTO_ADDR_EN_SET(1) | \
                                  LIN_SCI_AUTO_ADDR_CTRL_AUTO_ADDR_ANA_EN_SET(1) | \
                                  LIN_SCI_AUTO_ADDR_CTRL_MEAS_STEP_SEL_SET(type);

        /* LIN_AA interrupt enable */
        LIN_SCI->ICR |= (SCI_INT_SLV_SELECTED | SCI_INT_AUTO_ADDR_DONE);
        LIN_SCI->IMR &= ~(SCI_INT_SLV_SELECTED | SCI_INT_AUTO_ADDR_DONE);

    }


    return LL_OK;
}

/********************************************************
** \brief   ll_lin_snpd_disable
**
** \param   ll_lin_sbc_e            sbc
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_snpd_disable(ll_lin_sbc_e sbc)
{
    if (LL_LIN_SBC_0 != sbc)
    {
        return LL_ERROR;
    }

    LIN_SCI->ICR |= (SCI_INT_SLV_SELECTED | SCI_INT_AUTO_ADDR_DONE);
    LIN_SCI->IMR |= (SCI_INT_SLV_SELECTED | SCI_INT_AUTO_ADDR_DONE);

    /* current_source_iset */
    LIN_SCI->CURRENT_SOURCE_ISET = 0;

    /* Current threshold */
    LIN_SCI->CUR_TH1 = 0;
    LIN_SCI->CUR_TH2 = 0;

    /* Clk deviation time config */
    LIN_SCI->CLK_DEV_TIM_CFG = 0;

    /* Pga ready time config */
    LIN_SCI->PGA_RDY_TIM_CFG = 0;

    /* Auto addressing analog config  */
    LIN_SCI->AUTO_ADDR_ANA_CFG = 0x04F;

    /* Auto addressing control */
    LIN_SCI->AUTO_ADDR_CTRL = 0x00;

    /* ll_lin_snpd_enable, need 4 & 3 */
    LIN_SCI->ANALOG_CTRL = 7;
    ll_lin_rx_delay_set((ll_sci_bus_e)sbc, 3);

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_swsnpd_ready_set
**
** \param   ll_lin_sbc_e            sbc
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_swsnpd_ready_set(ll_lin_sbc_e sbc)
{
    LIN_SCI->AUTO_ADDR_CTRL_F.SW_CTRL_EN = 0;
    return LL_OK;
}

/********************************************************
** \brief   ll_sci_init
**
** \param   ll_sci_bus_e            bus
** \param   sci_config_t*           config
** \param   ISR_FUNC_CALLBACK       callback
**
** \retval  None
*********************************************************/
void ll_sci_init(ll_sci_bus_e bus, sci_config_t *config, ISR_FUNC_CALLBACK callback)
{
    assert_param(IS_SCI_BUS(bus));
    assert_param(IS_SCI_MODE(config->mode));

    ll_sci_gpio_config(bus, config->mode);
    ll_sci_clk_config(bus, &config->clk_cfg);
    ll_sci_baudrate_config(bus, config->baudrate);

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    if (SCI_MODE_UART == config->mode)
    {
        sci_reg->CTRL_UART_F.MODE = 1;
        /* set uart stop 1 bit*/
        sci_reg->CTRL_UART_F.STP_BIT_SEL = 0;
        /* default disable mp mode*/
        // /* set uart MP mode */
        // LIN_SCI->CTRL_UART_F.MP_MODE_EN = 1;
        // LIN_SCI->CTRL_UART_F.MP_RX_ADDR_WR_EN = 1;
        // LIN_SCI->CTRL_UART_F.MP_TX_ADDR_DATA_SEL = 1;
        // /* set uart MP address */
        // LIN_SCI->RX_CFG_F.MP_SLAVE_ADDR = 0;
        // LIN_SCI->RX_CFG_F.MP_SLAVE_ADDR = 0xAA;
        // LIN_SCI->RX_CFG_F.MP_SLAVE_ADDR_MSK = 1;
    }
    else
    {
        sci_reg->CTRL_UART_F.MODE = 0;

        if (SCI_MODE_LIN_M == config->mode)
        {
            config->isr_cfg.isr |= SCI_INT_TX_PID_DONE;
        }
    }

    /* bit error check point sel:0@middle, 1@last */
    sci_reg->TX_CFG_F.CHK_PT_SEL = 1;

    if (config->baudrate > 19200UL)
    {
        /* 高速模式下需要关闭TX_RX检测 */
        config->isr_cfg.isr &= ~SCI_INT_TX_RX_CONF;
    }

    ll_sci_contrl_config(bus, config->mode);
    ll_sci_isr_config(bus, &config->isr_cfg, callback);
}

/********************************************************
** \brief   ll_sci_baudrate_config
**
** \param   ll_sci_bus_e    bus
** \param   uint32_t    baudrate
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_sci_baudrate_config(ll_sci_bus_e bus, uint32_t baudrate)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    uint32_t fclk, div = 0;
    uint32_t frac;
    uint32_t intr;
    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    /**************************************************************/
    /* (intr + frac) * 16 = fclk / baudrate                       */
    /* intr + frac = fclk / baudrate / 16 = fclk / baudrate >> 16 */
    /**************************************************************/
    /* CPU_INT_FAST_CLK_HZ, 48MHz */
    fclk = CPU_INT_FAST_CLK_HZ;

    div = fclk / baudrate;
    intr = div / 16;
    frac = div % 16;

    sci_reg->BAUD_CFG_F.BD_FRAC = frac & 0x0F;
    sci_reg->BAUD_CFG_F.BD_INTR = intr & 0x1FF;

    if (baudrate > 19200UL)
    {
        sci_reg->RX_FILTER_CFG_F.RX_FILTER_TIM = 8;
        sci_reg->ANALOG_CTRL = 0;
        ll_lin_rx_delay_set(bus, 0);
        TEST_CONFIG_UNLOCK();
        TEST->TEST_LIN_CTRL_F.LIN_EMC_FBNEG_EN = false;
        TEST_CONFIG_LOCK();
    }
    else
    {
        sci_reg->RX_FILTER_CFG_F.RX_FILTER_TIM = 200;
        sci_reg->ANALOG_CTRL = 7;
        ll_lin_rx_delay_set(bus, 3);
        TEST_CONFIG_UNLOCK();
        TEST->TEST_LIN_CTRL_F.LIN_EMC_FBNEG_EN = true;
        TEST_CONFIG_LOCK();
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_sci_isr_enable
**
** \param   ll_sci_bus_e    bus
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_sci_isr_enable(ll_sci_bus_e bus, bool enable)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    IRQn_Type irq = (LL_SCI_BUS_0 == bus) ? LINSCI_IRQn : LINSCI_UART_IRQn;

    NVIC_ClearPendingIRQ(irq);

    if (enable)
    {
        /* TODO: lin speed: 10ms need change priority? */
        NVIC_EnableIRQ(irq);
    }
    else
    {
        NVIC_DisableIRQ(irq);
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_rx_delay_set
**
** \param   ll_sci_bus_e    bus
** \param   uint8_t         count
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_rx_delay_set(ll_sci_bus_e bus, uint8_t count)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    if (LL_SCI_BUS_0 == bus)
    {
        TEST_CONFIG_UNLOCK();
        TEST->TEST_LIN_CTRL_F.LIN_RX_DELAY = count;
        TEST_CONFIG_LOCK();
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_wakeup_enable
**
** \param   ll_lin_sbc_e    sbc
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_wakeup_enable(ll_lin_sbc_e sbc, bool enable)
{
    if (LL_SCI_BUS_0 != sbc)
    {
        return LL_ERROR;
    }

    ll_wakeup_init(WAKEUP_SOUERCE_LIN, WAKEUP_TIME_5, WAKEUP_FILTER_3);

    ll_syscfg_isr_enable(ASYSCFG_INT_WAKEUP, enable);

    NVIC_SetPriority(AON_IRQn, 3);
    NVIC_EnableIRQ(AON_IRQn);

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_snpd_ready_set
**
** \param   ll_lin_sbc_e    sbc
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_snpd_ready_set(ll_lin_sbc_e sbc, bool enable)
{
    if (LL_LIN_SBC_0 != sbc)
    {
        return LL_ERROR;
    }

    LIN_SCI->AUTO_ADDR_CTRL_F.ADDR_ALREADY_FLAG = enable ? 1 : 0;

    return LL_OK;
}

/********************************************************
** \brief   ll_sci_transmit
**
** \param   ll_sci_bus_e   bus
** \param   uint8_t*        buffer
** \param   uint16_t        length
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_sci_transmit(ll_sci_bus_e bus, uint8_t *buffer, uint16_t length)
{
    if (bus >= LL_SCI_BUS_MAX || NULL == buffer || !length)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    for (uint16_t i = 0; i < length; i++)
    {
        if (!ll_sci_tx_fifo_ready_wait(sci_reg))
        {
            return LL_TIMEOUT;
        }

        sci_reg->TX_DATA = buffer[i];
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_sci_receive
**
** \param   ll_sci_bus_e    bus
** \param   uint8_t*        buffer
** \param   uint16_t        length
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_sci_receive(ll_sci_bus_e bus, uint8_t *buffer, uint16_t length)
{
    if (bus >= LL_SCI_BUS_MAX || NULL == buffer || !length)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    for (uint16_t i = 0; i < length; i++)
    {
        buffer[i] = sci_reg->RX_DATA;
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_transmit
**
** \param   ll_sci_bus_e        bus
** \param   uint8_t             pid
** \param   uint8_t*            buffer
** \param   uint16_t            length
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_transmit(ll_sci_bus_e bus, uint8_t pid, uint8_t *buffer, uint16_t length)
{
    if (bus >= LL_SCI_BUS_MAX || NULL == buffer || !length)
    {
        return LL_ERROR;
    }

#if !LIN_CHECKSUM_USE_SW
    ll_lin_checksum_e checksum_type = ((0x3C == pid) || (0x7D == pid)) ? LIN_CHECKSUM_CLASSIC : LIN_CHECKSUM_ENHANCED;
#endif

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    if (sci_reg->ISR & SCI_INT_RX_STP_ERR)
    {
        sci_reg->ICR |= SCI_INT_RX_STP_ERR;
        ll_sci_state_clear(bus, (ll_sci_clear_type_e)(SCI_CLEAR_TX_ABORT | SCI_CLEAR_RX_ABORT));
        return LL_COMM_ERROR;
    }

    sci_reg->CTRL_F.TX_EN = 0;

    /* clear tx fifo */
    ll_sci_state_clear(bus, SCI_CLEAR_TX_FIFO);

#if !LIN_CHECKSUM_USE_SW
    sci_reg->CTRL_F.CHKSUM_TYPE = (LIN_CHECKSUM_CLASSIC == checksum_type) ? 0 : 1;
    sci_reg->CTRL_F.TX_NUM = length;
#endif

    sci_reg->CTRL_F.TX_EN = 1;

    length = (length > 4) ? 4 : length;

    for (uint8_t i = 0; i < length; i++)
    {
        if (!ll_sci_tx_fifo_ready_wait(sci_reg))
        {
            return LL_TIMEOUT;
        }

        sci_reg->TX_DATA = buffer[i];
    }

#if LIN_CHECKSUM_USE_SW

    if (!ll_sci_tx_fifo_ready_wait(sci_reg))
    {
        return LL_TIMEOUT;
    }

    sci_reg->TX_DATA_F.TX_DATA = ll_lin_checksum_calib_func(pid, buffer, length);
#endif

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_receive
**
** \param   ll_sci_bus_e        bus
** \param   uint8_t             pid
** \param   uint8_t*            buffer
** \param   uint16_t            length
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_receive(ll_sci_bus_e bus, uint8_t pid, uint8_t *buffer, uint16_t length)
{
    if (bus >= LL_SCI_BUS_MAX || NULL == buffer || !length)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg = get_sci_reg(bus);

    sci_reg->CTRL_F.TX_EN = 0;
    sci_reg->CTRL_F.RX_EN = 1;

    ll_sci_state_clear(bus, SCI_CLEAR_RX_FIFO);

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_checksum_calib_func
**
** \param   uint8_t             pid
** \param   uint8_t*            buffer
** \param   uint16_t            length
**
** \retval  None
*********************************************************/
uint8_t ll_lin_checksum_calib_func(uint8_t pid, uint8_t *buffer, uint16_t length)
{
    uint16_t check_sum;

    /* 1. PID correspond to Master request and Slave response,
     * their checksum cal is classic the non-diagnostic frame is calculated in Enhanced
     */
    check_sum = ((0x3C == pid) || (0x7D == pid))  ? 0 : pid;

    for (uint8_t i = 0; i < length; i++)
    {

        check_sum += buffer[i];

        /* 2. to deal with the carry */
        if (check_sum > 0xFF)
        {
            check_sum -= 0xFF;
        }
    }

    /* 3. to reverse */
    return (uint8_t)(~check_sum);
}

/********************************************************
** \brief   ll_lin_pid_read
**
** \param   ll_sci_bus_e    bus
** \param   uint8_t*        pid
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_pid_read(ll_sci_bus_e bus, uint8_t *pid)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    *pid = sci_reg->RX_PID_F.RX_PID;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_read_byte
**
** \param   ll_sci_bus_e    bus
** \param   uint8_t*        byte
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_read_byte(ll_sci_bus_e bus, uint8_t *byte)
{
    if (bus >= LL_SCI_BUS_MAX || NULL == byte)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    *byte = sci_reg->RX_DATA_F.RX_DATA;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_auto_baudrate_read
**
** \param   ll_sci_bus_e    bus
** \param   uint32_t*       baud
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_auto_baudrate_read(ll_sci_bus_e bus, uint32_t *baud)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    *baud = sci_reg->AUTO_BAUD_VAL_F.AUTO_BD_INTR;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_baudrate_read
**
** \param   ll_sci_bus_e    bus
** \param   uint32_t*       baud
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_baudrate_read(ll_sci_bus_e bus, uint32_t *baud)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    *baud = sci_reg->BAUD_CFG_F.BD_INTR;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_ctrl_glben
**
** \param   ll_sci_bus_e    bus
** \param   bool            sw
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_ctrl_glben(ll_sci_bus_e bus, bool sw)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    sci_reg->CTRL_F.GLB_EN = sw;

    return LL_OK;
}

/********************************************************
** \brief   ll_sci_auto_baudrate
**
** \param   ll_sci_bus_e        bus
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_sci_auto_baudrate(ll_sci_bus_e bus)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    uint32_t baud = sci_reg->BAUD_CFG_F.BD_INTR;
    uint32_t auto_baud = sci_reg->AUTO_BAUD_VAL_F.AUTO_BD_INTR;
    uint32_t diff_baud = (auto_baud >= baud) ? (auto_baud - baud) : (baud - auto_baud);

    if (diff_baud > (baud * 14 / 100))
    {
        sci_reg->CTRL_F.GLB_EN = false;
        sci_reg->CTRL_F.GLB_EN = true;
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_ctrl_rx_abort
**
** \param   ll_sci_bus_e    bus
** \param   bool            sw
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_ctrl_rx_abort(ll_sci_bus_e bus, bool sw)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    sci_reg->CTRL_F.RX_ABORT = sw;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_ctrl_brk_tx
**
** \param   ll_sci_bus_e    bus
** \param   uint8_t         brk_num
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_ctrl_brk_tx(ll_sci_bus_e bus, uint8_t brk_num)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);


    sci_reg->BRK_SYNC_CFG_F.BRK_NUM = brk_num;
    sci_reg->CTRL_F.BRK_TX_TRIG = true;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_tx_header
**
** \param   ll_sci_bus_e    bus
** \param   uint8_t         pid
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_tx_header(ll_sci_bus_e bus, uint8_t pid)
{
    if (bus >= LL_SCI_BUS_MAX)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg(bus);

    sci_reg->CTRL_F.RX_EN = 0;
    sci_reg->TX_CFG_F.TX_PID = pid;
    sci_reg->BRK_SYNC_CFG_F.BRK_NUM = 0x0D; /* 13bit for break*/
    sci_reg->CTRL_F.BRK_TX_TRIG = true;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_sw_snpd_config
**
** \param   ll_lin_sbc_e    sbc
** \param   bool            osc
** \param   bool            conv
** \param   bool            pu_res
** \param   uint8_t         iset
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_sw_snpd_config(ll_lin_sbc_e sbc, bool osc, bool conv, bool pu_res, uint8_t iset)
{
    if (sbc >= LL_SCI_BUS_MAX  || LL_LIN_SBC_0 != sbc)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg((ll_sci_bus_e)sbc);

    sci_reg->AUTO_ADDR_SW_CTRL_F.SW_LIN_CKOSC     = osc;
    sci_reg->AUTO_ADDR_SW_CTRL_F.SW_LIN_CKCONV    = conv;
    sci_reg->AUTO_ADDR_SW_CTRL_F.SW_LIN_PU_RES_EN = pu_res;
    sci_reg->CURRENT_SOURCE_ISET_F.LIN_ISET_STEP1 = iset;

    return LL_OK;
}

/********************************************************
** \brief   ll_lin_sw_snpd_enable
**
** \param   ll_lin_sbc_e    bus
** \param   boolean         en
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_lin_sw_snpd_enable(ll_lin_sbc_e sbc, bool en)
{
    if (sbc >= LL_LIN_SBC_MAX  || LL_LIN_SBC_0 != sbc)
    {
        return LL_ERROR;
    }

    LIN_SCI_REG_TypeDef *sci_reg =  get_sci_reg((ll_sci_bus_e)sbc);

    sci_reg->AUTO_ADDR_CTRL_F.SW_CTRL_EN = en;

    return LL_OK;
}

/********************************************************
** \brief   SCI_IRQHandler
**
** \param   None
**
** \retval  None
*********************************************************/
void SCI_IRQHandler(void)
{
    uint32_t isr = LIN_SCI->ISR & LIN_ISR_FLAG;

    if (isr)
    {
        if (isr & SCI_INT_TX_DONE)
        {
            LIN_SCI->CTRL_F.TX_EN = 0; /* disable tx */
            LIN_SCI->CTRL_F.RX_EN = 1; /* enable rx */
        }

        if (isr & SCI_INT_SYNC_VAL_ERR)
        {
            LIN_SCI->CTRL_F.GLB_EN = false;
            LIN_SCI->CTRL_F.GLB_EN = true;
        }

        if (NULL != sci_isr_callback[0])
        {
            sci_isr_callback[0](isr);
        }

        LIN_SCI->ICR |= isr;
    }
}

/********************************************************
** \brief   UART_IRQHandler
**
** \param   None
**
** \retval  None
*********************************************************/
void UART_IRQHandler(void)
{
    uint32_t isr = LIN_SCI1->ISR & LIN_ISR_FLAG;

    if (isr)
    {
        if (NULL != sci_isr_callback[1])
        {
            sci_isr_callback[1](isr);
        }

        LIN_SCI1->ICR |= isr;
    }
}
