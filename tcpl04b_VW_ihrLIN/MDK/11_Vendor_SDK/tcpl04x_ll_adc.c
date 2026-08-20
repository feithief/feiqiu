/**
 *****************************************************************************
 * @brief   adc driver source file.
 *
 * @file    tcpl04x_ll_adc.c
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

/*
 * ============================================================================
 * 本地可审阅副本说明
 * ============================================================================
 * 上游文件：platform/drivers/tcpl04x_ll_driver/tcpl04x_ll_adc.c
 * 上游 SHA-256：79A6B8C7A9EEA1A3683011356EA24F005DF8C7D9C8878B2454532201D7864DA0
 * 功能改动：无。本副本只增加中文硬件数据流/生命周期审阅注释。
 *
 * 为什么保留原厂实现：TCPL04B Datasheet V1.0 证明了 ADC 的硬件能力，
 * 但没有公开完整的寄存器编程序列。为避免臆造未公开的硅片控制顺序，底层
 * 语句保持原厂一致；tools/verify.ps1 会在删除注释和空白后比较两份源码，
 * 防止“加注释”意外变成“改原厂逻辑”。
 *
 * ADC 是 input-only 外设，唯一有效载荷路径为：
 *   TEMP/VBAT 模拟源 -> 输入选择/模拟前端 -> SAR 转换状态机
 *   -> 8 深度硬件 FIFO -> CPU 读取 FIFO_DATA -> RAM samples[]。
 * 时钟、复位、软件 Start、IRQ 和状态位均为控制/通知侧路，不是第二条数据流。
 *
 * 本精简工程实际使用的调用链：
 *   初始化：上层先打开 PCLK/FCLK -> ll_adc_deinit -> 输入/参数/状态机/FIFO
 *           配置 -> 最后开放软件运行门
 *   运行：ll_adc_channel_start -> channel_config -> offset/gain/input/FIFO
 *                              -> IRQ gate off -> software Start
 *                              -> FIFO polling -> RAM buffer
 *
 * 这里采用用户可见 H/S/I 初始化分类，不把工作数据流误写成初始化顺序：
 *   H01 寄存器访问/转换时钟；H02 复位并进入 Stop/配置状态；H03 模拟输入路径；
 *   H04 模拟前端/触发/采样配置；H05 通道/采样保持/SAR；H06 结果 FIFO；
 *   H07 软件运行门/Start；H08 BUSY/FIFO 完成反馈。
 *   S01 RAM 请求/配置；S02 RAM 结果/补偿/工程量；S03 CPU/调度所有权。
 *   I01 NVIC；I02 ADC 事件门；I03 CPU 回调/ISR 交接对象。
 * 运行期注释沿真实工作数据流书写，不另建一套可见运行编号。
 */

#include "tcpl04x_ll_adc.h"

#define LL_ADC_BUSY_TIMEOUT_CYCLES    (DEFAULT_SYSTEM_CLOCK / 1000U)

/****************************************/
/* H08：检查硬件空闲或结果完成反馈      */
/****************************************/
static bool ll_adc_idle_wait(void)
{
    uint32_t timeout = LL_ADC_BUSY_TIMEOUT_CYCLES;

    /*
     * H08：CHNL_STATUS.ADC_BUSY 是工作状态下转换状态机的忙反馈。
     * 做什么：CPU 在改通道、增益和采样次数前等待 ADC_BUSY 变为 0。
     * 现象：若一直为 1，本函数在有限循环后返回 false，禁止无界卡死。
     * 本质原因：模拟前端和转换状态机仍在消费当前配置，此时覆盖配置可能让
     *           同一批样本混用新旧通道或增益。
     */
    while (ADC->CHNL_STATUS_F.ADC_BUSY)
    {
        if (timeout-- == 0U)
        {
            return false;
        }
    }

    return true;
}

#define ADC_ISR_FLAG       (0x1FUL)

#define TP_REF_VALUE                    1223UL

const uint16_t vcr_value[ADC_VCR_MAX] = {236, 314, 394, 437, 552, 631, 710, 789};
uint16_t vref_value[ADC_VREF_MAX] = {2500, 2000, 1500};

static uint32_t adc_isr_flag = 0;
bool adc_isr_enable = false;
static ISR_FUNC_CALLBACK adc_isr_callback = NULL;

typedef struct
{
    float tos_value;
    float k_value;
} temp_trim_value_t;

typedef struct
{
    float a;
    float b;
    float c;
} vaon_coef_t;

typedef struct
{
    uint16_t gain;
    int16_t offset;
} adc_ch_trim_t;

typedef struct
{
    uint16_t tpref_v;
    uint16_t temp_c;
} tpref_ta_trim_t;

typedef struct
{
    tpref_ta_trim_t tpref_ta[3];            /* [0]low [1]normal [2]high */
    float vref_slope_low;
    float vref_intercept_low;
    float vref_slope_high;
    float vref_intercept_high;
} vref_coef_t;

temp_trim_value_t vtemp_temp_value[2];
vaon_coef_t vaon_coef_value;
adc_ch_trim_t vf_ch_trim_value[3];
adc_ch_trim_t aon_ch_trim_value;
vref_coef_t vref_coef_value;

/********************************************************
** \brief   ll_adc_clk_config
**
** \param   ll_clk_config_t*    config
**
** \retval  None
*********************************************************/
/****************************************/
/* H01：原厂ADC时钟寄存器映射           */
/****************************************/
static void ll_adc_clk_config(ll_clk_config_t *config)
{
    /* H01：CRG 时钟/复位寄存器受保护，先开写保护门。 */
    CRG_CONFIG_UNLOCK();

    /*************************************************/
    /* ADC doesn't have an option to select its fclk */
    /* the default and only fclk for ADC is RC48MHz  */
    /*************************************************/
    /*
     * PCLK_EN_ADC：打开 CPU 到 ADC 寄存器组的总线时钟。
     * 不做现象：CPU 对 CTRL/FIFO/状态寄存器的访问不能形成有效外设操作。
     * 本质原因：寄存器接口时钟门关闭；这不等于启动一次 ADC 转换。
     */
    CRG->ADC_CLKRST_CTRL_F.PCLK_EN_ADC = true;

    /*
     * FCLK_EN_ADC：打开 SAR ADC 自动采样/转换逻辑的功能时钟。
     * 不做现象：寄存器可能可写，但软件 Start 后转换状态机没有时钟可运行。
     * 本质原因：PCLK 只服务寄存器访问，FCLK 才驱动外设内部工作。
     */
    CRG->ADC_CLKRST_CTRL_F.FCLK_EN_ADC = true;

    /* FCLK_DIV_ADC：设置 ADC 功能时钟分频，决定内部状态机的工作时间基准。 */
    CRG->ADC_CLKRST_CTRL_F.FCLK_DIV_ADC = config->fclk_div;

    /* H01：时钟配置完成后立即重新关闭 CRG 写保护门。 */
    CRG_CONFIG_LOCK();
}

/********************************************************
** \brief   ll_adc_isr_config
**
** \param   ll_isr_config_t*      config
** \param   ISR_FUNC_CALLBACK     callback
**
** \retval  None
*********************************************************/
static void ll_adc_isr_config(ll_isr_config_t *config, ISR_FUNC_CALLBACK callback)
{
    ADC->IMR |= ADC_ISR_FLAG;
    adc_isr_enable = false;

    if (config->isr_enable)
    {
        adc_isr_flag = config->isr & ADC_ISR_FLAG;

        adc_isr_callback = callback;
        NVIC_SetPriority(ADC_IRQn, config->priority);
    }
    else
    {
        adc_isr_flag = 0;
        adc_isr_callback = NULL;
    }
}

/********************************************************
** \brief   ll_adc_trim_value_load
**
** \param   None
**
** \retval  None
*********************************************************/
static void ll_adc_trim_value_load(void)
{
    TEST_CONFIG_UNLOCK();

    TEST->ADC_TRIM0 = 0x74371F1D;
    TEST->ADC_TRIM1 = 0xFA007C;
    TEST->ADC_TRIM2 = 0x2EF0177;
    TEST->ADC_TRIM3 = 0xB0404F8;
    TEST->ADC_TRIM4 = 0x16010B04;
    TEST->ADC_TRIM5 = 0x108020ff;
    TEST->ADC_TRIM6 = 0x3F402100;
    TEST->ADC_TRIM7 = 0x7900;

    TEST_CONFIG_LOCK();
}

/********************************************************
** \brief   ll_adc_calibration_load
**
** \param   trim_value_t*       vbat_trim
** \param   temp_trim_value_t*  temp_trim
** \param   vaon_coef_t*        vaon_coef
**
** \retval  None
*********************************************************/
static void ll_adc_calibration_load(temp_trim_value_t *temp_trim, vaon_coef_t *vaon_coef, vref_coef_t *vref_coef)
{
    uint32_t trim_version = TRIM->BASIC_INFO;

    if (((trim_version & 0xFF) > 0x03) || (trim_version & 0xFF) == 0x03 && (((trim_version >> 8) & 0x3F) >= 0x01))
    {
        /* vtemp trim */
        volatile TRIM_VALUE_ADC_TSENSOR_FIELD_T *tsensor_trim[] =
        {
            &TRIM->ADC_TSENSOR0_F,
            &TRIM->ADC_TSENSOR1_F,
        };

        for (uint8_t i = 0; i < 2; i++)
        {
            temp_trim[i].tos_value = (tsensor_trim[i]->TOS != 0xFFFF) ? tsensor_trim[i]->TOS : 0x5000;
            temp_trim[i].k_value = (tsensor_trim[i]->K != 0xFFFF) ? tsensor_trim[i]->K : 0x0B00;
            temp_trim[i].k_value /= 16;
        }

        /* tpref */
        uint32_t tpref[3] = {0};
        volatile TRIM_VALUE_TPREF_TA_FIELD_T *tpref_ta[] =
        {
            &TRIM->TPREF_TA_LOW_F,
            &TRIM->TPREF_TA_NORMAL_F,
            &TRIM->TPREF_TA_HIGH_F
        };

        volatile TRIM_VALUE_VAON_TSENSOR_FIELD_T *vaon_tsensor[] =
        {
            &TRIM->VAON_TLOW_TSENSOR1_F,
            &TRIM->VAON_TROOM_TSENSOR1_F,
            &TRIM->VAON_THIGH_TSENSOR1_F
        };

        for (uint8_t i = 0; i < 3; i++)
        {
            tpref[i] = ((uint32_t)(tpref_ta[i]->TPREF) * 1000) >> 17;
            vref_coef->tpref_ta[i].tpref_v = (tpref[i] == 0) ? TP_REF_VALUE : tpref[i];
            vref_coef->tpref_ta[i].temp_c = vaon_tsensor[i]->TCODE;
        }

        float x1 = vref_coef->tpref_ta[0].temp_c;
        float x2 = vref_coef->tpref_ta[1].temp_c;
        float y1 = vref_coef->tpref_ta[0].tpref_v;
        float y2 = vref_coef->tpref_ta[1].tpref_v;
        float delta_x = x2 - x1;
        float delta_y = y2 - y1;
        vref_coef->vref_slope_low = delta_y / delta_x;
        vref_coef->vref_intercept_low = y2 - vref_coef->vref_slope_low * x2;

        x1 = vref_coef->tpref_ta[1].temp_c;
        x2 = vref_coef->tpref_ta[2].temp_c;
        y1 = vref_coef->tpref_ta[1].tpref_v;
        y2 = vref_coef->tpref_ta[2].tpref_v;
        delta_x = x2 - x1;
        delta_y = y2 - y1;
        vref_coef->vref_slope_high = delta_y / delta_x;
        vref_coef->vref_intercept_high = y2 - vref_coef->vref_slope_high * x2;

        /* vaon */
        int32_t aon_value;
        aon_value = TRIM->VAON_COEF_A;
        vaon_coef->a =  /*5.0 **/ aon_value / 68719476.736 / 4096.0 / 4096.0;
        aon_value = TRIM->VAON_COEF_B;
        vaon_coef->b = /*5.0 * */ aon_value / 268435.456 / 4096.0;
        aon_value = TRIM->VAON_COEF_C;
        vaon_coef->c = /* 5.0 **/ aon_value / 4.096;
    }
    else
    {
        temp_trim[0].tos_value =  temp_trim[0].k_value = 1;
        temp_trim[1].tos_value =  temp_trim[1].k_value = 1;
        vref_coef->vref_slope_low = vref_coef->vref_intercept_low = 1;
        vref_coef->vref_slope_high = vref_coef->vref_intercept_high = 1;
        vaon_coef->a = vaon_coef->b = vaon_coef->c = 1;
    }
}

/********************************************************
** \brief   ll_adc_vf_calibration_load
**
** \param   adc_ch_trim_t*     vf_ch_trim
** \param   adc_ch_trim_t*     aon_ch_trim
**
** \retval  ll_status_e
*********************************************************/
static ll_status_e ll_adc_vf_calibration_load(adc_ch_trim_t *vf_ch_trim, adc_ch_trim_t *aon_ch_trim)
{
    volatile TRIM_VALUE_ADC_COEF_FIELD_T *led_trim[] =
    {
        &TRIM->ADC_DIFF_VDD5_LED0_F,
        &TRIM->ADC_DIFF_VDD5_LED1_F,
        &TRIM->ADC_DIFF_VDD5_LED2_F,
    };

    for (uint8_t i = 0; i < 3; i++)
    {
        vf_ch_trim[i].gain = led_trim[i]->B;
        vf_ch_trim[i].offset = ((int16_t)(led_trim[i]->A << 2)) >> 2;
    }

    aon_ch_trim->gain = TRIM->ADC_IO2_IO4_F.B;
    aon_ch_trim->offset = ((int16_t)(TRIM->ADC_IO2_IO4_F.A << 2)) >> 2;

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_scan_channel_config
**
** \param   adc_scan_channel_e  scan_channel
** \param   adc_scan_config_t*  config
**
** \retval  None
*********************************************************/
static void ll_adc_scan_channel_config(adc_scan_channel_e scan_channel, adc_scan_config_t *config)
{
    switch (scan_channel)
    {
        case ADC_SCAN_CHANNEL_0:
            ADC->CTRL_SCAN01_F.SCAN_CHNL_0 = config->scan_channel_cfg;
            break;

        case ADC_SCAN_CHANNEL_1:
            ADC->CTRL_SCAN01_F.SCAN_CHNL_1 = config->scan_channel_cfg;
            break;

        case ADC_SCAN_CHANNEL_2:
            ADC->CTRL_SCAN23_F.SCAN_CHNL_2 = config->scan_channel_cfg;
            break;

        case ADC_SCAN_CHANNEL_3:
            ADC->CTRL_SCAN23_F.SCAN_CHNL_3 = config->scan_channel_cfg;
            break;

        case ADC_SCAN_CHANNEL_4:
            ADC->CTRL_SCAN45_F.SCAN_CHNL_4 = config->scan_channel_cfg;
            break;

        case ADC_SCAN_CHANNEL_5:
            ADC->CTRL_SCAN45_F.SCAN_CHNL_5 = config->scan_channel_cfg;
            break;

        case ADC_SCAN_CHANNEL_6:
            ADC->CTRL_SCAN67_F.SCAN_CHNL_6 = config->scan_channel_cfg;
            break;

        case ADC_SCAN_CHANNEL_7:
            ADC->CTRL_SCAN67_F.SCAN_CHNL_7 = config->scan_channel_cfg;
            break;

        default:
            break;
    }
}

/********************************************************
** \brief   ll_adc_deinit
**
** \param   None
**
** \retval  void
*********************************************************/
void ll_adc_deinit(void)
{
/****************************************/
/* I02：屏蔽并清除ADC外设事件门         */
/****************************************/
    /*
     * H02 Stop/配置状态；同时整理 I02/I03：撤销旧事件、复位 ADC 状态机、清空软件
     * 回调所有权，使后续配置从确定状态开始。
     *
     * 做什么：清事件 -> 执行硬件复位脉冲 -> 屏蔽事件 -> 清软件影子状态。
     * 现象：不执行时，旧 FIFO/状态/中断可能被新一轮初始化误认为新结果。
     * 本质原因：硬件寄存器、FIFO、IRQ 门和软件回调是四类不同状态，仅清
     *           C 变量或仅写复位位都不能证明另外三类状态已清理。
     *
     * 注意：TCPL04B Datasheet 未公开下面“复位前先把 IMR[4:0] 写 0”的
     * 设计原因；这里忠实保留原厂顺序，不把未知原因包装成通用原理。
     */

    /* ICR[4:0]：向五类 ADC 事件清除位写 1，撤销复位前残留的硬件事件。 */
    ADC->ICR |= 0x1F;

    /* IMR[4:0]：按原厂复位前序列写 0；最终屏蔽状态在复位后重新建立。 */
    ADC->IMR &= ~0x1F;

/****************************************/
/* I03：清理CPU回调与ISR交接对象        */
/****************************************/
    /* 软件影子：声明 ADC IRQ 业务路径当前不拥有事件处理权。 */
    adc_isr_enable = false;

/****************************************/
/* H02：复位ADC并进入停止配置状态       */
/****************************************/
    /* CRG 的 RST_ADC 位受保护；只在复位脉冲期间打开写保护门。 */
    CRG_CONFIG_UNLOCK();

    /* RST_ADC=1：断言 ADC 硬件复位，停止转换状态机并恢复外设复位状态。 */
    CRG->ADC_CLKRST_CTRL_F.RST_ADC = 1;

    /* 两个 NOP 给总线写入/复位信号跨时钟域传播留下原厂规定的间隔。 */
    __NOP();
    __NOP();

    /* RST_ADC=0：释放复位；只是允许后续配置，不代表 ADC 已经 Start。 */
    CRG->ADC_CLKRST_CTRL_F.RST_ADC = 0;

    /* 两个 NOP 给复位释放传播留下原厂规定的间隔。 */
    __NOP();
    __NOP();

    /* 复位脉冲完成，重新关闭 CRG 写保护门。 */
    CRG_CONFIG_LOCK();

    /* IMR[4:0]=1：屏蔽全部 ADC 外设事件源，先关生产者一侧通知门。 */
    ADC->IMR |= ADC_ISR_FLAG;

    /* ICR[4:0]=1：再次清除复位/释放过程可能形成的硬件事件。 */
    ADC->ICR |= ADC_ISR_FLAG;

    /* 软件事件选择掩码归零；以后只有显式配置的事件才可打开。 */
    adc_isr_flag = 0;

    /* 释放回调所有权，防止 ADC IRQ 误调用上一次配置留下的函数地址。 */
    adc_isr_callback = NULL;

    /*
     * 本函数不直接关闭 NVIC ADC_IRQn；它依靠外设 IMR 屏蔽源头并清 ICR。
     * 本工程的轮询路径随后还会调用 ll_adc_isr_enable(false) 关闭 NVIC 门。
     */
}

/********************************************************
** \brief   ll_adc_init
**
** \param   adc_config_t*       config
** \param   ISR_FUNC_CALLBACK   callback
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_init(adc_config_t *config, ISR_FUNC_CALLBACK callback)
{
    ll_adc_deinit();

    ll_adc_clk_config(&config->clk_cfg);

    ll_adc_trim_value_load();

    ll_adc_vf_calibration_load(vf_ch_trim_value, &aon_ch_trim_value);

    ll_adc_calibration_load(vtemp_temp_value, &vaon_coef_value, &vref_coef_value);

    ADC->CTRL0_F.TRIG_EN = config->trig_mode;

    ADC->CTRL0_F.FIFO_THRHLD = config->trig_num - 1;
    ADC->CTRL0_F.I_SEL = 1;

    ADC->CTRL0_F.SW_ADC_EN = true;
    ADC->CTRL0_F.AUTO_ADC_EN = false;

    /* adc ctrl1: */
    ADC->CTRL1_F.SW_CONT_MODE = 0; //continuous mode
    ADC->CTRL1_F.SCAN_CHNL_NUM = 0; //scan channel num, N +1
    ADC->CTRL1_F.CHNL_SAMP_NUM =  config->trig_num - 1;

    ADC->CTRL1_F.DIV_EN = 1;

    /* adc ctrl2: default*/
    if (DEFAULT_SYSTEM_CLOCK < 48000000UL)
    {
        ADC->CTRL2_F.SAMP_CYCLE = 0x03 >> 1;
        ADC->CTRL2_F.INIT_CYCLE = 0x1E0 >> 1;
    }
    else
    {
        ADC->CTRL2_F.SAMP_CYCLE = 0x0F;
        ADC->CTRL2_F.INIT_CYCLE = 0x320;
    }

    /* ADC fifo clr */
    ll_adc_fifo_clear();

    ll_adc_isr_config(&config->isr_cfg, callback);

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_led_init
**
** \param   None
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_led_current_init(void)
{
    ADC->CTRL_ANA_F.ADC_CTRL |= 0x8000; //bit15

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_backup_channel_config
**
** \param   None
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_backup_channel_config(void)
{
    uint32_t reg_val;

    CRG_CONFIG_UNLOCK();
    CRG->RESERVED_T_CLKRST_CTRL_F.RST_RESERVED_T = 0;
    CRG->RESERVED_T_CLKRST_CTRL_F.PCLK_EN_RESERVED_T = 1;
    CRG->RESERVED_T_CLKRST_CTRL_F.FCLK_EN_RESERVED_T = 1;
    CRG_CONFIG_LOCK();

    TEST_CONFIG_UNLOCK();
    TEST->TEST_RESERVED_T_TRIM = TRIM->RESERVED0;
    TEST_CONFIG_LOCK();

    /* ex adc enable */
    *((volatile uint32_t *)&reg_val) = *((volatile uint32_t *)(0x40009014));
    reg_val |=  0x01;
    *((volatile uint32_t *)0x40009014) = *((volatile uint32_t *)(&reg_val));
    /* ex cfg mode */
    *((volatile uint32_t *)&reg_val) = *((volatile uint32_t *)(0x40009028));
    reg_val |=  0x01;
    *((volatile uint32_t *)0x40009028) = *((volatile uint32_t *)(&reg_val));

    *((volatile uint32_t *)&reg_val) = *((volatile uint32_t *)(0x4000902C));
    reg_val |=  0x04;
    *((volatile uint32_t *)0x4000902C) = *((volatile uint32_t *)(&reg_val));

    for (adc_channel_e i = 0; i < 3; i++)
    {
        vf_ch_trim_value[i].gain = TRIM->ADC_GAIN_OS_F.SARADC_GAIN;
        vf_ch_trim_value[i].offset = TRIM->ADC_GAIN_OS_F.SARADC_OFFSET;
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_vf_scan_config
**
** \param   adc_channel_e   channel
** \param   adc_cfg_t*      cfg
** \param   uint8_t         scan_num
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_vf_scan_config(adc_channel_e channel, adc_cfg_t *cfg, uint8_t scan_num)
{
    /* adc ctrl0: */
    ADC->CTRL0_F.FIFO_THRHLD = scan_num - 1;
    /* adc ctrl1: */
    ADC->CTRL1_F.SW_CONT_MODE = 0; //continuous mode
    ADC->CTRL1_F.SCAN_CHNL_NUM = scan_num - 1; //scan channel num, N +1
    ADC->CTRL1_F.CHNL_SAMP_NUM =  0;
    /* adc ctrl2: */
    ADC->CTRL2_F.INIT_CYCLE = 0x0f;

    adc_scan_config_t scan_config =
    {
        .scan_config_bit.pga_bypass = cfg->adc_pag_bypass,
        .scan_config_bit.pga_bufn_bypass = cfg->adc_buf_bypass,
        .scan_config_bit.pga_bufp_bypass = cfg->adc_buf_bypass,
        .scan_config_bit.pga_gain = cfg->gain,
    };

    if (ADC_CHANNEL_IO4_GND == channel)
    {
        scan_config.scan_config_bit.channel = ADC_CHANNEL_IO4_GND;
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_2, &scan_config);
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_3, &scan_config);
        scan_config.scan_config_bit.channel = ADC_CHANNEL_IO2_GND;
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_4, &scan_config);
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_5, &scan_config);
        scan_config.scan_config_bit.channel = ADC_CHANNEL_RESERVED_T;
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_6, &scan_config);
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_7, &scan_config);
    }
    else
    {
        scan_config.scan_config_bit.channel = channel;
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_2, &scan_config);
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_3, &scan_config);
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_4, &scan_config);
        ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_5, &scan_config);
    }

    scan_config.scan_config_bit.channel = ADC_CHANNEL_TEMP1;
    scan_config.scan_config_bit.pga_bypass = false;
    scan_config.scan_config_bit.pga_bufn_bypass = false;
    scan_config.scan_config_bit.pga_bufp_bypass = false;
    scan_config.scan_config_bit.pga_gain = ADC_GAIN_X16;
    ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_0, &scan_config);

    scan_config.scan_config_bit.channel = ADC_CHANNEL_TAO_TEST;
    scan_config.scan_config_bit.pga_bypass = true;
    scan_config.scan_config_bit.pga_bufn_bypass = true;
    scan_config.scan_config_bit.pga_bufp_bypass = false;
    scan_config.scan_config_bit.pga_gain = ADC_GAIN_X1;
    ll_adc_scan_channel_config(ADC_SCAN_CHANNEL_1, &scan_config);

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_gain_config
**
** \param   adc_channel_e   channel
** \param   adc_cfg_t*      cfg
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_gain_config(adc_channel_e channel, adc_cfg_t *cfg)
{
    if (channel > ADC_CHANNEL_MAX)
    {
        return LL_PARAM_INVALID;
    }

/****************************************/
/* H04：模拟前端、参考与增益配置        */
/****************************************/
    /*
     * H04 模拟前端配置。
     * 做什么：建立参考电压、共模、输入缓冲和 PGA 增益/旁路路径。
     * 现象：配置错误时原始码会饱和、比例错误或噪声增大。
     * 本质原因：模拟信号在进入 SAR 前先经过这些真实硬件开关和放大级。
     */

    /* VCR_EN：决定是否从输入中引入/扣除可编程 VCR 基准。 */
    ADC->CTRL0_F.VCR_EN = cfg->vcr_enable;

    if (cfg->vcr_enable)
    {
        /* VCR_SEL：仅在 VCR 路径开启时选择具体 VCR 电平。 */
        ADC->CTRL0_F.VCR_SEL = cfg->vcr;
    }

    /* VCM_SEL：选择模拟前端共模工作点。 */
    ADC->CTRL0_F.VCM_SEL = cfg->vcm;

    /* PGA_GAIN_SEL：PGA 启用且未旁路时选择 1X~16X；旁路时强制 0 档。 */
    ADC->CTRL1_F.PGA_GAIN_SEL = (cfg->adc_pag_en && !cfg->adc_pag_bypass) ? cfg->gain : 0;

    /* VREF_SEL：选择 SAR 满量程参考电压 1.5V/2.0V/2.5V。 */
    ADC->CTRL1_F.VREF_SEL = cfg->vref;

    /* VREFBUF_EN：打开 ADC 内部参考电压缓冲，给转换核心提供稳定 VREF。 */
    ADC->CTRL0_F.VREFBUF_EN = true;

    /* BUFP_EN/BUFN_EN：同时控制差分正、负输入缓冲放大级。 */
    ADC->CTRL1_F.BUFP_EN = cfg->adc_buf_en;
    ADC->CTRL1_F.BUFN_EN = cfg->adc_buf_en;

    /* BUFP_BP/BUFN_BP：选择信号经过输入缓冲还是直接旁路。 */
    ADC->CTRL1_F.BUFP_BP = cfg->adc_buf_bypass;
    ADC->CTRL1_F.BUFN_BP = cfg->adc_buf_bypass;

    /* PGA_EN/PGA_BP：分别控制 PGA 供能门和信号旁路门，两者不是同一开关。 */
    ADC->CTRL1_F.PGA_EN = cfg->adc_pag_en;
    ADC->CTRL1_F.PGA_BP = cfg->adc_pag_bypass;

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_isr_enable
**
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_isr_enable(bool enable)
{
/****************************************/
/* I03：设置CPU回调交接软件状态         */
/****************************************/
    /* I03：软件影子只记录 IRQ 交接策略，不会自行改变硬件通知路径。 */
    adc_isr_enable = enable;

/****************************************/
/* I02：配置ADC外设事件门               */
/****************************************/
    /* ICR：先清全部旧事件。 */
    ADC->ICR |= ADC_ISR_FLAG;

/****************************************/
/* I01：配置NVIC的ADC中断入口           */
/****************************************/
    /* 清 CPU 入口处的旧挂起。 */
    NVIC_ClearPendingIRQ(ADC_IRQn);

    if (enable)
    {
        /* IMR 位为 0 表示放行所选外设事件，随后才打开 NVIC CPU 入口。 */
        ADC->IMR &= ~adc_isr_flag;
        NVIC_EnableIRQ(ADC_IRQn);
    }
    else
    {
        /* 轮询模式：先在外设侧屏蔽事件，再关闭 NVIC ADC 入口。 */
        ADC->IMR |= adc_isr_flag;
        NVIC_DisableIRQ(ADC_IRQn);
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_channel_offset_load
**
** \param   adc_channel_e       channel
**
** \retval  ll_status_e
*********************************************************/
static void ll_adc_channel_offset_load(adc_channel_e channel)
{
    /* TRIM.BASIC_INFO：读取晶圆/修调版本，决定哪些工厂校准字段可信。 */
    uint32_t trim_version = TRIM->BASIC_INFO;

    /* ADC_CAL_CTRL 属于受保护模拟系统配置，写入前打开写保护门。 */
    ASYSCFG_CONFIG_UNLOCK();

    /* change offset & gain */
    if ((TRIM->BASIC_INFO_F.DIE_VER > 0x03) || TRIM->BASIC_INFO_F.DIE_VER == 0x03 && (TRIM->BASIC_INFO_F.TRIM_VER >= 0x01))
    {
        switch (channel)
        {
            case ADC_CHANNEL_VBAT:
                /*
                 * VBAT/VS 通道：B 写入数字增益校正 ADC_OUT_CAL，A 写入
                 * ADC_OUT_OFFSET。较旧修调版本没有专用值时退回单位增益/零偏。
                 */
                if ((TRIM->BASIC_INFO_F.DIE_VER > 0x03) || TRIM->BASIC_INFO_F.DIE_VER == 0x03 && (TRIM->BASIC_INFO_F.TRIM_VER >= 0x02))
                {
                    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_VS_DIV_F.B;
                    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_VS_DIV_F.A;
                }
                else
                {
                    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = 0x1000;
                    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = 0;
                }

                break;

            case ADC_CHANNEL_VC0:
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_LED0_F.B;
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_LED0_F.A;
                break;

            case ADC_CHANNEL_VC1:
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_LED1_F.B;
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_LED1_F.A;
                break;

            case ADC_CHANNEL_VC2:
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_LED2_F.B;
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_LED2_F.A;
                break;

            case ADC_CHANNEL_VPN0:
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_DIFF_VDD5_LED0_F.B;
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_DIFF_VDD5_LED0_F.A;
                break;

            case ADC_CHANNEL_VPN1:
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_DIFF_VDD5_LED1_F.B;
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_DIFF_VDD5_LED1_F.A;
                break;

            case ADC_CHANNEL_VPN2:
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_DIFF_VDD5_LED2_F.B;
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_DIFF_VDD5_LED2_F.A;
                break;

            default:
                /* TEMP 等普通通道使用 SAR ADC 公共工厂增益/偏移校准。 */
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_GAIN_OS_F.SARADC_GAIN;
                ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_GAIN_OS_F.SARADC_OFFSET;
                break;
        }
    }
    else
    {
        /* 无可证明的修调区时采用单位增益(0x1000)和零偏移，禁止读垃圾值。 */
        ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = 0x1000;
        ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = 0;
    }

    /* 校准写入完成后立即恢复 ASYSCFG 写保护。 */
    ASYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   ll_adc_channel_config
**
** \param   adc_channel_e   channel
** \param   adc_cfg_t*      cfg
** \param   uint8_t         smp_num
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_channel_config(adc_channel_e channel, adc_cfg_t *cfg, uint8_t smp_num)
{
/****************************************/
/* H05：选择通道并配置采样转换状态机    */
/****************************************/
    /* 参数门：通道、VREF 和 FIFO 深度都必须落在芯片定义的范围内。 */
    if ((channel >= ADC_CHANNEL_MAX) || (NULL == cfg) ||
        (cfg->vref >= ADC_VREF_MAX) || (0U == smp_num) || (smp_num > 8U))
    {
        return LL_PARAM_INVALID;
    }

    /* H08 -> H04/H05：先取得空闲确认，再覆盖当前硬件事务配置。 */
    if (!ll_adc_idle_wait())
    {
        return LL_TIMEOUT;
    }

    /* 先把所选物理源对应的工厂增益/偏移接到结果校准路径。 */
    ll_adc_channel_offset_load(channel);

    /* 再配置 VREF、共模、输入缓冲和 PGA 路径。 */
    ll_adc_gain_config(channel, cfg);

    /* FIFO_THRHLD=N-1：第 N 个结果入 FIFO 时达到阈值条件。 */
    ADC->CTRL0_F.FIFO_THRHLD = smp_num - 1;

    /* SW_CONT_MODE=0：一次软件请求完成后停止，不连续重复生产样本。 */
    ADC->CTRL1_F.SW_CONT_MODE = 0;

    /* SCAN_CHNL_NUM=0：本事务只转换 IN_SEL 指定的一个通道。 */
    ADC->CTRL1_F.SCAN_CHNL_NUM = 0;

    /* CHNL_SAMP_NUM=N-1：同一输入源连续转换 N 次。 */
    ADC->CTRL1_F.CHNL_SAMP_NUM =  smp_num - 1;

    /* IN_SEL：把 TEMP/VBAT 等目标模拟源接到采样保持/PGA 输入端。 */
    ADC->CTRL1_F.IN_SEL = channel;

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_vf_channel_config
**
** \param   adc_channel_e       channel
** \param   adc_cfg_t*          cfg
** \param   uint8_t             scan_num
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_vf_channel_config(adc_channel_e channel, adc_cfg_t *cfg, uint8_t scan_num)
{
    if ((channel >= ADC_CHANNEL_MAX) || (NULL == cfg) ||
        (cfg->vref >= ADC_VREF_MAX) || (0U == scan_num) || (scan_num > 8U))
    {
        return LL_PARAM_INVALID;
    }

    if (!ll_adc_idle_wait())
    {
        return LL_TIMEOUT;
    }

    ll_adc_gain_config(channel, cfg);

    TEST_CONFIG_UNLOCK();

    TEST->TEST_ANA_CTRL_F.TESTMUX_TAO_SEL = 2;
    TEST->TEST_ANA_CTRL_F.TEST_AON = 1;
    TEST->TEST_ANA_CTRL_F.TESTMUX_TO_ADC_EN = 1;
    TEST->TEST_ANA_CTRL_F.TESTMUX_SEL_BUF = 1;
    TEST->TEST_ANA_CTRL_F.TESTMUX_BUF_EN = 1;
    TEST->TEST_ANA_CTRL_F.TESTMUX_TAO_EN = 1;

    TEST_CONFIG_LOCK();

    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = 0x1000;
    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = 0;

    ASYSCFG_CONFIG_LOCK();

    ll_adc_vf_scan_config(channel, cfg, scan_num);
    return LL_OK;
}

/********************************************************
** \brief   ll_adc_select_channel
**
** \param   adc_channel_e   channel
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_select_channel(adc_channel_e channel)
{
    // if (channel >= ADC_CHANNEL_MAX)
    // {
    //     return LL_PARAM_INVALID;
    // }

    // ADC->CTRL1_F.IN_SEL = channel;

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_fifo_length_get
**
** \param   None
**
** \retval  uint8_t
*********************************************************/
uint8_t ll_adc_fifo_length_get(void)
{
/****************************************/
/* H06：读取ADC硬件结果FIFO状态         */
/****************************************/
    /*
     * H06/H08：FIFO_STATE[3:0] 是 8 深度结果 FIFO 当前有效条目数，
     * 这里只观察结果存储量，不搬运有效载荷，也不代表 CPU 已取得样本。
     */
    return (ADC->FIFO_STATE & 0x0F);
}

/********************************************************
** \brief   ll_adc_fifo_get
**
** \param   None
**
** \retval  uint16_t
*********************************************************/
/****************************************/
/* 工作数据流02：FIFO到RAM [WORKFLOW]   */
/****************************************/
uint16_t ll_adc_fifo_get(int16_t *buffer, uint16_t length)
{
    /* 先快照 FIFO 硬件条目数；后续每读一次 FIFO_DATA，硬件弹出一个条目。 */
    uint8_t len = ll_adc_fifo_length_get();

    if (len >= length)
    {
        /* 若旧结果多于调用者容量，先丢弃最旧的多余条目，保留最新 length 个。 */
        for (uint8_t i = 0; i < len - length; i++)
        {
            buffer[0] = ADC->FIFO_DATA;
        }
    }

    /* 实际搬运数取“调用者容量”和“硬件已有条目数”的较小值。 */
    len = length > len ? len : length;

    for (uint8_t i = 0; i < len; i++)
    {
        /*
         * 工作数据流：H06 FIFO_DATA -> CPU -> S02 buffer[i] RAM。
         * 数据手册规定结果为 14 位有符号码；左移 2 再算术右移 2 完成符号扩展。
         */
        buffer[i] = ((int16_t)(ADC->FIFO_DATA << 2)) >> 2; /* 0x3FFF */
    }

    return len;
}

/********************************************************
** \brief   ll_adc_fifo_clear
**
** \param   None
**
** \retval  uint16_t
*********************************************************/
uint16_t ll_adc_fifo_clear(void)
{
    uint32_t reg_val = 0;
    (void)(&reg_val);

    /* H06：读取复位/上一事务遗留的 FIFO 有效条目数。 */
    uint16_t len = ll_adc_fifo_length_get();

    for (uint8_t i = 0; i < len; i++)
    {
        /* FIFO 无独立 flush 位；逐次读 FIFO_DATA，让硬件读指针消费全部旧结果。 */
        reg_val = ADC->FIFO_DATA;
    }

    return len;
}

/********************************************************
** \brief   ll_adc_softwart_start
**
** \param   bool            enable
**
** \retval  None
*********************************************************/
/****************************************/
/* H07：映射软件启动请求门              */
/****************************************/
void ll_adc_softwart_start(bool enable)
{
    if (enable)
    {
        /*
         * H07 Start 请求：SW_START=1 把已配置事务交给 ADC 状态机。
         * 写 1 只表示发出请求；完成由 FIFO 条目数/忙状态证明。
         */
        ADC->CTRL0_F.SW_START = true;
    }
    else
    {
        /* SW_START=0 撤销软件启动电平；不等价于硬件复位。 */
        ADC->CTRL0_F.SW_START = false;
    }
}

/********************************************************
** \brief   ll_adc_gain_value_get
**
** \param   adc_gain_e      gain
** \param   uint8_t*        value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_gain_value_get(adc_gain_e gain, uint8_t *value)
{
    if (gain >= ADC_GAIN_MAX)
    {
        return LL_PARAM_INVALID;
    }

    *value = gain + 1;

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_vcr_value_get
**
** \param   adc_vcr_e       vcr
** \param   uint16_t*       value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_vcr_value_get(adc_vcr_e vcr, uint16_t *value)
{
    if (vcr >= ADC_VCR_MAX)
    {
        return LL_PARAM_INVALID;
    }

    *value = vcr_value[vcr];

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_vref_value_get
**
** \param   adc_vref_e      vref
** \param   uint16_t*       value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_vref_value_get(adc_vref_e vref, uint16_t *value)
{
    if (vref >= ADC_VREF_MAX)
    {
        return LL_PARAM_INVALID;
    }

    *value = vref_value[vref];

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_temp_calculate_func
**
** \param   temp_channel_e  channel
** \param   int16_t         code
** \param   adc_cfg_t*      cfg
** \param   int16_t*        value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_temp_calculate_func(temp_channel_e channel, int16_t code, adc_cfg_t *cfg,  int16_t *value)
{
    uint16_t vref;

    if (NULL == cfg || channel >= TEMP_CHANNEL_MAX)
    {
        return LL_PARAM_INVALID;
    }

    *value = (code * 2.5 - vtemp_temp_value[channel].tos_value) / vtemp_temp_value[channel].k_value - 273.15;

    /* vref coef */
    if (code <= vref_coef_value.tpref_ta[1].temp_c)
    {
        vref = (uint16_t)(vref_coef_value.vref_slope_low * code +
                          vref_coef_value.vref_intercept_low + 0.5);
    }
    else
    {
        vref = (uint16_t)(vref_coef_value.vref_slope_high * code +
                          vref_coef_value.vref_intercept_high + 0.5);
    }

    vref_value[0] = (uint16_t)(2500.0 * vref / TP_REF_VALUE + 0.5);
    vref_value[1] = (uint16_t)(2000.0 * vref / TP_REF_VALUE + 0.5);
    vref_value[2] = (uint16_t)(1500.0 * vref / TP_REF_VALUE + 0.5);

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_vbat_calculate_func
**
** \param   int16_t         code
** \param   int16_t*        value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_vbat_calculate_func(int16_t code, int16_t *value)
{
    return LL_OK;
}

/********************************************************
** \brief   ll_adc_volt_calculate_func
**
** \param   int16_t         code
** \param   adc_cfg_t*      cfg
** \param   int16_t*        value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_volt_calculate_func(int16_t code, adc_cfg_t *cfg, int16_t *value)
{
    if (NULL == cfg)
    {
        return LL_PARAM_INVALID;
    }

    int16_t code_val = code;
    uint16_t vref, vcr;
    uint8_t gain;

    if (cfg->adc_pag_en && !cfg->adc_pag_bypass)
    {
        ll_adc_gain_value_get(cfg->gain, &gain);
    }
    else
    {
        gain = 1;
    }

    if (cfg->vcr_enable)
    {
        ll_adc_vcr_value_get(cfg->vcr, &vcr);
    }
    else
    {
        vcr = 0;
    }

    ll_adc_vref_value_get(cfg->vref, &vref);

    code_val = ((int16_t)(code << 2)) >> 2;

    *value = cfg->ratio * ((code_val * vref) / (gain * 8192.f) + vcr);

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_vf_calculate_func
**
** \param   adc_channel_e   channel
** \param   int16_t*        buffer
** \param   int16_t*        value
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_vf_calculate_func(adc_channel_e channel, int16_t *buffer, int16_t *value)
{
    uint8_t index;
    int32_t aon_t_code;
    int32_t aon_v_code;
    int32_t led_v_code;
    float vaon;

    led_v_code = buffer[0];
    aon_t_code = buffer[1];
    aon_v_code = buffer[2];
    index = channel - ADC_CHANNEL_VPN0;

    aon_v_code = (aon_v_code - aon_ch_trim_value.offset) * aon_ch_trim_value.gain;
    aon_t_code = (aon_t_code - aon_ch_trim_value.offset) * aon_ch_trim_value.gain;
    led_v_code = (led_v_code - vf_ch_trim_value[index].offset) * vf_ch_trim_value[index].gain;

    vaon = vaon_coef_value.a * aon_t_code * aon_t_code + vaon_coef_value.b * aon_t_code + vaon_coef_value.c;
    *value = 0.5 + vaon * led_v_code  / aon_v_code;

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_channel_start
**
** \param   adc_channel_e   channel
** \param   adc_cfg_t*      cfg
** \param   int16_t*        buffer
** \param   uint8_t         trig_num
**
** \retval  ll_status_e
*********************************************************/
/****************************************/
/* 工作数据流01：一次ADC事务 [WORKFLOW] */
/****************************************/
ll_status_e ll_adc_channel_start(adc_channel_e channel, adc_cfg_t *cfg, int16_t *buffer, uint8_t trig_num)
{
    ll_status_e st = LL_OK;
    uint16_t timeout = 50;

    /* 工作控制侧路：经 H08 空闲门后配置 H04/H05 和本次结果数量。 */
    ll_adc_channel_config(channel, cfg, trig_num);

    /*
     * 原厂实现审计项：这里没有接收 ll_adc_channel_config() 的返回值。
     * 本工程调用者保证参数合法；若 ADC_BUSY 等待超时，后续 FIFO 有界等待
     * 通常仍会把事务判为 LL_ERROR，但会丢失更精确的 LL_TIMEOUT 原因。
     * 本地副本为保持原厂可执行逻辑不变暂不修改，后续若要修复必须作为
     * 独立功能变更和台架回归处理，不能混在“补注释”提交中。
     */
    // ll_adc_select_channel(channel);

    /* I02/I01：本事务采用轮询，关闭外设事件门和 NVIC 通知侧路。 */
    ll_adc_isr_enable(false);

    /* H07：发出软件 Start，请求 H05 开始采样和 SAR 转换。 */
    ll_adc_softwart_start(true);

    /*
     * H08：等待 H06 FIFO 恰好具有 trig_num 个结果，作为完成反馈。
     * 每轮含原厂 5*NOP 延时，并由 timeout 限制总等待次数，避免硬件故障
     * 造成无界阻塞。这里轮询的是“结果已存入硬件 FIFO”，不是搬运完成。
     */
    while (trig_num != ll_adc_fifo_length_get())
    {
        if (timeout-- <= 0)
        {
            st = LL_ERROR;
            break;
        }

        for (uint8_t i = 0; i < 100; i++) //10us
        {
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
        }
    }

    /* 工作数据流：从 H06 FIFO_DATA 搬到 S02 RAM；数量不足则事务失败。 */
    if (trig_num != ll_adc_fifo_get(buffer, trig_num))
    {
        st = LL_ERROR;
    }

    return st;
}

/********************************************************
** \brief   ll_adc_it_start
**
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_it_start(bool enable)
{
    ll_adc_fifo_clear();
    ll_adc_isr_enable(true);

    ll_adc_softwart_start(true);

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_tsensor_enable
**
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
/****************************************/
/* H03：映射片内温度差分输入门          */
/****************************************/
ll_status_e ll_adc_tsensor_enable(bool enable)
{
    if (enable)
    {
        /*
         * H03：内部温度传感器输出是差分模拟量。
         * INT0_SNS_EN、INT1_SNS_EN 分别接通差分两端到 ADC 内部输入网络。
         * 这里只打开输入物理门，不选择通道、不产生转换结果。
         */
        ADC->CTRL1_F.INT0_SNS_EN = true;
        ADC->CTRL1_F.INT1_SNS_EN = true;
    }
    else
    {
        /* 同时断开差分两端，防止只关一侧留下不完整模拟路径。 */
        ADC->CTRL1_F.INT0_SNS_EN = false;
        ADC->CTRL1_F.INT1_SNS_EN = false;
    }

    return LL_OK;
}

/********************************************************
** \brief   ll_adc_lin_snpd_enable
**
** \param   lin_snpd_type_e     type
** \param   bool                enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_adc_lin_snpd_enable(lin_snpd_type_e type, bool enable)
{
    ll_adc_deinit();

    ll_adc_trim_value_load();

    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_CAL = TRIM->ADC_GAIN_OS_F.SARADC_GAIN;
    ASYSCFG->ADC_CAL_CTRL_F.ADC_OUT_OFFSET = TRIM->ADC_GAIN_OS_F.SARADC_OFFSET;

    ASYSCFG_CONFIG_LOCK();

    PWM->LED_CTRL_F.LED_LDO5V_EN = 1;
    /* adc ctrl0: */
    ADC->CTRL0_F.VREFBUF_EN = 1;
    ADC->CTRL0_F.TRIG_EN = (LIN_SNPD_TYPE_STEPS_SW4 <= type) ? \
                           TRIG_SOFTWARE : TRIG_LIN;
    ADC->CTRL0_F.SW_ADC_EN = false;
    ADC->CTRL0_F.AUTO_ADC_EN = true;
    ADC->CTRL0_F.FIFO_THRHLD = 0;

    /* adc ctrl1: */
    ADC->CTRL1_F.IN_SEL = ADC_CHANNEL_LIN;
    ADC->CTRL1_F.SW_CONT_MODE = false;

    if (LIN_SNPD_TYPE_STEPS_SW2 == type)
    {
        ADC->CTRL1_F.CHNL_SAMP_NUM = 8 - 1;
    }
    else
    {
        ADC->CTRL1_F.CHNL_SAMP_NUM = 0;
    }

    ADC->CTRL1_F.SCAN_CHNL_NUM = 0;

    if (LIN_SNPD_TYPE_STEPS_2 == type)
    {
        ADC->CTRL2_F.INIT_CYCLE = 0x3EF >> 1;
    }
    else
    {
        ADC->CTRL2_F.INIT_CYCLE = 0xF0 >> 1;
    }

    /* path setting */
    ADC->CTRL1_F.BUFN_BP = true;
    ADC->CTRL1_F.BUFP_BP = true;
    ADC->CTRL1_F.BUFN_EN = false;
    ADC->CTRL1_F.BUFP_EN = false;
    ADC->CTRL1_F.PGA_BP = true;
    ADC->CTRL1_F.PGA_EN = false;

    ADC->CTRL1_F.PGA_GAIN_SEL = ADC_GAIN_X1;
    ADC->CTRL1_F.DIV_EN = false;

    /* ADC fifo clr */
    ll_adc_fifo_clear();

    return LL_OK;
}

/********************************************************
** \brief   ll_bias_control_enable
**
** \param   bool            enable
**
** \retval  ll_status_e
*********************************************************/
ll_status_e ll_bias_control_enable(bool enable)
{
    return LL_OK;
}

/********************************************************
** \brief   ADC_IRQHandler
**
** \param   None
**
** \retval  None
*********************************************************/
void ADC_IRQHandler(void)
{
    uint32_t isr = ADC->ISR & ADC_ISR_FLAG;

    if (isr)
    {
        if (NULL != adc_isr_callback)
        {
            adc_isr_callback(isr);
        }

        ADC->ICR |= isr;
    }
}
