#include "Adc.h"

#include "pal_led.h"
#include "tcpl04x_ll_adc.h"
#include "tcpl04x_ll_led.h"
#include "tcpl04x_ll_pwm.h"

/**
 * @file Tcpl04bAdc.c
 * @brief TCPL04B ADC 的 MCAL（微控制器抽象层）实现。
 *
 * 本文件只负责“如何驱动 ADC 硬件”和“如何把原始码换算成工程量”，
 * 不负责 100 ms 调度、跨模块共享或应用策略；这些职责属于 ModuleAdc。
 *
 * 工作状态下的硬件有效载荷数据流（ADC 只有输入/结果方向）：
 *   片内温度传感器 / VBAT 监测源
 *       -> ADC 通道选择与模拟前端
 *       -> 采样保持及模数转换
 *       -> ADC 结果 FIFO
 *       -> 厂商 LL 轮询取数
 *       -> samples[] 软件栈缓冲
 *       -> 4 次平均、整数标定
 *       -> 摄氏温度(0.1 degC) / 电池电压(mV)
 *
 * CPU 控制路径（不是 ADC 有效载荷）：
 *   CPU -> 时钟/复位、TRIM、触发方式和采样参数寄存器
 *       -> 软件触发 -> ADC 状态机开始转换。
 *
 * 当前实现采用同步轮询：ll_adc_channel_start() 关闭 ADC 中断，发出软件
 * 触发，等待 FIFO 达到 4 个结果，再把结果读入 samples[]。因此本驱动没有
 * ADC ISR、DMA 或异步队列；上层不得把中断路径误认为采样数据路径。
 *
 * 上述路径描述的是 ADC 正在工作时，样本实际经过哪些硬件和 RAM；它不是
 * 初始化函数调用顺序。H/S/I 只是附着在这些工作节点旁的“准备分类”：
 *
 *   H01 寄存器访问与转换时钟         H05 通道选择、采样保持与 SAR 状态机
 *   H02 Stop/配置状态                H06 ADC 硬件结果 FIFO
 *   H03 TEMP/VBAT 模拟输入路径       H07 软件运行门与 Start 请求
 *   H04 模拟前端/触发/采样参数       H08 BUSY/FIFO 数量完成反馈
 *
 *   S01 RAM 请求与通道配置对象       S03 100 ms 调度、CPU/API 所有权
 *   S02 原始样本、补偿状态和工程量缓存
 *
 *   I01 NVIC ADC 入口                I02 ADC 外设事件屏蔽/清除门
 *   I03 CPU 回调/ISR 软件交接对象
 *
 * 本工程使用轮询，所以 I01/I02/I03 的初始化结果是“全部安全关闭”，而不是
 * 删除中断节点或把 IRQ 误画成样本路径。运行过程直接按上面的工作数据流解释。
 *
 * 板级边界：本文件使用芯片内部 TEMP/VBAT 通道。VBAT 的实际可测范围、
 * 外部分压及精度仍须以 TCPL04B 手册和目标板原理图/实测为最终依据。
 */

/****************************************/
/* S01：RAM请求与通道配置对象           */
/****************************************/
/* RAM 请求数量与硬件每通道采样数量统一为 4。 */
#define ADC_SAMPLE_COUNT                (4U)
#define ADC_NOMINAL_VREF_MV             (2500L)
#define ADC_TPREF_NOMINAL               (1223L)
#define ADC_TEMP_DEFAULT_TOS            (0x5000L)
#define ADC_TEMP_DEFAULT_K              (0x0B00L)
#define ADC_LED_CHANNEL_COUNT            (3U)
#define ADC_LED_PN_SAMPLE_COUNT          (6U)
#define ADC_LED_VF_SAMPLE_COUNT          (4U)

typedef enum
{
    ADC_LED_DIAG_IDLE = 0,
    ADC_LED_DIAG_WAIT_PWM,
    ADC_LED_DIAG_WAIT_ADC
} Adc_LedDiagStateType;

/*
 * 两个 RAM 配置对象分别映射 TEMP/VBAT 输入及模拟前端参数。
 * 这些 adc_cfg_t 字段直接描述硬件通道倍率、增益、共模和参考电压，所有权
 * 属于 MCAL。上层只能索取工程量，不能直接修改这些硬件参数。
 */
static adc_cfg_t s_temp_cfg =
    {1U, false, true, true, false, false,
     ADC_VCR_SEL_236_7, ADC_GAIN_X16, ADC_VCM_SEL_205, ADC_VREF_2500};

static adc_cfg_t s_battery_cfg =
    {20U, false, true, true, true, false,
     ADC_VCR_SEL_236_7, ADC_GAIN_X2, ADC_VCM_SEL_205, ADC_VREF_2500};

/* TCPL04B VPN measurement front-end, from the vendor TCPL04B meas profile. */
static adc_cfg_t s_led_vf_cfg =
    {5U, true, false, true, true, false,
     ADC_VCR_SEL_437_45, ADC_GAIN_X1, ADC_VCM_SEL_205, ADC_VREF_2500};

/****************************************/
/* S02：原始样本与补偿状态RAM           */
/****************************************/
/*
 * 最近一次温度通道原始码仅供 VBAT 参考电压温漂补偿使用。
 * 它不是对外发布的温度值；ModuleAdc 另行拥有工程量缓存。
 */
static int16_t s_last_temperature_code;
static volatile Adc_LedDiagStateType s_led_diag_state = ADC_LED_DIAG_IDLE;
static uint8_t s_led_active_mask;
static uint8_t s_led_next_channel;
static uint8_t s_led_current_channel;
static uint8_t s_led_current_hw_channel;
static uint32_t s_led_current_pwm_flag;
static uint8_t s_led_hw_channel[ADC_LED_CHANNEL_COUNT];
static int16_t s_led_voltage_mv[ADC_LED_CHANNEL_COUNT];
static bool s_led_voltage_valid[ADC_LED_CHANNEL_COUNT];
static const uint32_t s_led_pwm_clear_flag[ADC_LED_CHANNEL_COUNT] =
{
    PWM_INT_CH0_PWM_CLR,
    PWM_INT_CH1_PWM_CLR,
    PWM_INT_CH2_PWM_CLR
};

static void Adc_StartNextLedChannel(void);

static void Adc_AbortLedCycle(void)
{
    if (s_led_diag_state == ADC_LED_DIAG_WAIT_PWM)
    {
        (void)ll_pwm_isr_flag_enable(LL_PWM_BUS_0,
                                     s_led_current_pwm_flag, false);
    }
    else if (s_led_diag_state == ADC_LED_DIAG_WAIT_ADC)
    {
        (void)ll_adc_isr_enable(false);
        (void)ll_led_diagnose_enable(
            (ll_led_channel_e)s_led_current_hw_channel, false);
        (void)ll_adc_fifo_clear();
    }
    s_led_active_mask = 0U;
    s_led_diag_state = ADC_LED_DIAG_IDLE;
}

static bool Adc_SetupLedChannel(uint8_t channel)
{
    adc_channel_e adcChannel;

    s_led_current_channel = channel;
    s_led_current_hw_channel = s_led_hw_channel[channel];
    adcChannel = (adc_channel_e)(ADC_CHANNEL_VPN0 +
                                 s_led_current_hw_channel);
    s_led_current_pwm_flag =
        s_led_pwm_clear_flag[s_led_current_hw_channel];

    if (ll_adc_vf_channel_config(adcChannel, &s_led_vf_cfg,
                                 ADC_LED_PN_SAMPLE_COUNT) != LL_OK)
    {
        return false;
    }
    s_led_diag_state = ADC_LED_DIAG_WAIT_PWM;
    (void)ll_pwm_isr_flag_enable(LL_PWM_BUS_0,
                                 s_led_current_pwm_flag, true);
    return true;
}

static void Adc_StartNextLedChannel(void)
{
    while (s_led_next_channel < ADC_LED_CHANNEL_COUNT)
    {
        uint8_t channel = s_led_next_channel;
        s_led_next_channel++;
        if ((s_led_active_mask & (uint8_t)(1U << channel)) != 0U)
        {
            if (Adc_SetupLedChannel(channel))
            {
                return;
            }
        }
    }
    s_led_diag_state = ADC_LED_DIAG_IDLE;
    s_led_active_mask = 0U;
}

static void Adc_LedIrqCallback(uint32_t isr)
{
    int16_t samples[ADC_LED_PN_SAMPLE_COUNT];
    int16_t vfData[3];
    int32_t sum;
    int16_t voltage = 0;
    uint8_t index;

    if ((s_led_diag_state != ADC_LED_DIAG_WAIT_ADC) ||
        ((isr & ADC_INT_FIFO_RDY_FLAG) == 0U))
    {
        return;
    }

    (void)ll_adc_isr_enable(false);
    (void)ll_led_diagnose_enable(
        (ll_led_channel_e)s_led_current_hw_channel, false);

    if (ll_adc_fifo_get(samples, ADC_LED_PN_SAMPLE_COUNT) ==
        ADC_LED_PN_SAMPLE_COUNT)
    {
        sum = 0;
        for (index = 2U; index < ADC_LED_PN_SAMPLE_COUNT; index++)
        {
            sum += samples[index];
        }
        vfData[0] = (int16_t)(sum / ADC_LED_VF_SAMPLE_COUNT);
        vfData[1] = samples[0];
        vfData[2] = samples[1];

        /* Zero is a valid short-circuit observation, not a sampling failure. */
        if ((vfData[0] > 0) && (vfData[2] != 0))
        {
            (void)ll_adc_vf_calculate_func(
                (adc_channel_e)(ADC_CHANNEL_VPN0 +
                                s_led_current_hw_channel),
                vfData, &voltage);
            voltage = (int16_t)((int32_t)voltage * s_led_vf_cfg.ratio);
        }
        s_led_voltage_mv[s_led_current_channel] = voltage;
        s_led_voltage_valid[s_led_current_channel] = true;
    }

    s_led_diag_state = ADC_LED_DIAG_IDLE;
    Adc_StartNextLedChannel();
}

/*
 * Strong callback selected by pal_led_init().  It starts ADC sampling at the
 * selected channel's PWM clear event, so low duty-cycle LEDs are not sampled
 * at an arbitrary point in their off phase.
 */
void pwm_callback_handle(uint32_t isr)
{
    if ((s_led_diag_state == ADC_LED_DIAG_WAIT_PWM) &&
        ((isr & s_led_current_pwm_flag) != 0U))
    {
        (void)ll_pwm_isr_flag_enable(LL_PWM_BUS_0,
                                     s_led_current_pwm_flag, false);
        (void)ll_led_diagnose_enable(
            (ll_led_channel_e)s_led_current_hw_channel, true);
        s_led_diag_state = ADC_LED_DIAG_WAIT_ADC;
        if (ll_adc_it_start(true) != LL_OK)
        {
            (void)ll_led_diagnose_enable(
                (ll_led_channel_e)s_led_current_hw_channel, false);
            s_led_diag_state = ADC_LED_DIAG_IDLE;
            Adc_StartNextLedChannel();
        }
    }
}

/* 有符号整数四舍五入，避免引入浮点运行库。denominator 由调用者保证非零。 */
/****************************************/
/* 工作数据流04：整数标定辅助 [WORKFLOW] */
/****************************************/
static int32_t Adc_DivideRounded(int32_t numerator, int32_t denominator)
{
    if (numerator >= 0)
    {
        return (numerator + (denominator / 2)) / denominator;
    }
    return (numerator - (denominator / 2)) / denominator;
}

/****************************************/
/* 工作数据流01：取得RAM样本 [WORKFLOW] */
/****************************************/
static bool Adc_ReadAverage(adc_channel_e channel,
                                    adc_cfg_t *config,
                                    int16_t *average)
{
    /* S02：RAM 临时结果缓冲，不是 H06 ADC 硬件 FIFO，也不会跨调用保存。 */
    int16_t samples[ADC_SAMPLE_COUNT];
    int32_t sum = 0;
    uint8_t index;

    /*
     * 工作数据流：
     *   1. LL 配置指定通道；
     *   2. LL 关闭 ADC IRQ 并发出软件触发；
     *   3. LL 轮询 FIFO，超时或结果数不足时返回 LL_ERROR；
     *   4. 成功时把 4 个硬件结果搬入 samples[]。
     *
     * 因为这是一条同步调用链，返回 true 时 samples[] 已完整，返回 false 时
     * average 保持不变。这里没有“先返回、稍后由中断补数据”的行为。
     */
    if (s_led_diag_state != ADC_LED_DIAG_IDLE)
    {
        Adc_AbortLedCycle();
    }

    if ((average == NULL) ||
        (LL_OK != ll_adc_channel_start(channel, config, samples, ADC_SAMPLE_COUNT)))
    {
        return false;
    }

    /* 工作数据流：CPU 对 S02 中的 4 个原始码求平均，再进入数字标定。 */
    for (index = 0U; index < ADC_SAMPLE_COUNT; index++)
    {
        sum += samples[index];
    }
    *average = (int16_t)(sum / (int32_t)ADC_SAMPLE_COUNT);
    return true;
}

/****************************************/
/* 工作数据流02：选择有效标定 [WORKFLOW] */
/****************************************/
static bool Adc_HasTemperatureTrim(void)
{
    uint32_t version = TRIM->BASIC_INFO;
    uint32_t major = version & 0xFFU;
    uint32_t minor = (version >> 8U) & 0x3FU;

    /* 芯片版本达到厂商规定门槛后，才允许读取相应温度/VREF 工厂校准区。 */
    return (major > 0x03U) || ((major == 0x03U) && (minor >= 0x01U));
}

/*
 * S02 数字补偿：根据最近一次温度通道码，在低温/室温或室温/高温两个工厂
 * 标定点之间线性插值，估算当前 ADC 参考电压。校准不可用时退回 2500 mV
 * 标称值。此函数只做数字补偿，不会再次启动 ADC。
 */
/****************************************/
/* 工作数据流03：补偿参考电压 [WORKFLOW] */
/****************************************/
static int32_t Adc_GetVrefMv(int16_t temperature_code)
{
    int32_t x1;
    int32_t x2;
    int32_t y1;
    int32_t y2;
    int32_t interpolated;

    if (!Adc_HasTemperatureTrim())
    {
        return ADC_NOMINAL_VREF_MV;
    }

    if (temperature_code <= (int16_t)TRIM->VAON_TROOM_TSENSOR1_F.TCODE)
    {
        x1 = TRIM->VAON_TLOW_TSENSOR1_F.TCODE;
        x2 = TRIM->VAON_TROOM_TSENSOR1_F.TCODE;
        y1 = ((int32_t)TRIM->TPREF_TA_LOW_F.TPREF * 1000L) >> 17U;
        y2 = ((int32_t)TRIM->TPREF_TA_NORMAL_F.TPREF * 1000L) >> 17U;
    }
    else
    {
        x1 = TRIM->VAON_TROOM_TSENSOR1_F.TCODE;
        x2 = TRIM->VAON_THIGH_TSENSOR1_F.TCODE;
        y1 = ((int32_t)TRIM->TPREF_TA_NORMAL_F.TPREF * 1000L) >> 17U;
        y2 = ((int32_t)TRIM->TPREF_TA_HIGH_F.TPREF * 1000L) >> 17U;
    }

    if (y1 == 0L)
    {
        y1 = ADC_TPREF_NOMINAL;
    }
    if (y2 == 0L)
    {
        y2 = ADC_TPREF_NOMINAL;
    }
    if (x1 == x2)
    {
        return ADC_NOMINAL_VREF_MV;
    }

    interpolated = y1 + ((y2 - y1) * ((int32_t)temperature_code - x1)) / (x2 - x1);
    return (ADC_NOMINAL_VREF_MV * interpolated + (ADC_TPREF_NOMINAL / 2L)) /
           ADC_TPREF_NOMINAL;
}

void Adc_Init(void)
{
    adc_config_t initConfig =
    {
        .clk_cfg = { .fclk_div = 1U },
        .isr_cfg =
        {
            .isr = ADC_INT_FIFO_RDY_FLAG,
            .isr_enable = true,
            .priority = 2U
        },
        .trig_num = ADC_SAMPLE_COUNT,
        .trig_mode = TRIG_SOFTWARE
    };
/****************************************/
/* H01：先打开寄存器访问与转换时钟      */
/****************************************/
    /*
     * 这是所有 ADC 寄存器操作的硬件前置条件，必须位于 ll_adc_deinit()
     * 以及任何 ADC->... 访问之前：PCLK 接通 CPU 到 ADC 寄存器接口，
     * FCLK 给采样/转换状态机提供工作时钟。两者都不等于启动一次转换。
     * CRG 写保护只在修改时钟门期间打开，写完立即重新锁定。
     */
    CRG_CONFIG_UNLOCK();

    /* PCLK_EN_ADC=1：CPU 随后才能可靠访问 ADC 的控制、状态和 FIFO 寄存器。 */
    CRG->ADC_CLKRST_CTRL_F.PCLK_EN_ADC = true;

    /* FCLK_EN_ADC=1：允许 ADC 内部采样保持和 SAR 状态机在 Start 后运行。 */
    CRG->ADC_CLKRST_CTRL_F.FCLK_EN_ADC = true;

    /* FCLK_DIV_ADC=1：沿用 TCPL04B 原厂驱动的 ADC 功能时钟分频配置。 */
    CRG->ADC_CLKRST_CTRL_F.FCLK_DIV_ADC = 1U;
    CRG_CONFIG_LOCK();

/****************************************/
/* H02：停止硬件并进入可配置状态        */
/****************************************/
    /*
     * H01 已保证 ADC 寄存器可访问。现在清事件并产生硬件复位脉冲，把 ADC
     * 放回已知停止态，防止配置模拟前端或 FIFO 时仍有旧转换在运行。
     * 真正的单次 Start 只发生在运行期 ll_adc_channel_start()。
     */
    ll_adc_deinit();

/****************************************/
/* I01：关闭并清除NVIC的ADC入口         */
/****************************************/
    /*
     * 轮询架构不使用 ADC IRQ，显式关闭 CPU 的 ADC 中断入口并清
     * pending。ll_adc_deinit() 已完成 I02 外设事件屏蔽/清除和 I03 回调清空。
     * 三处同时准备，才不会把旧事件送进一个已经失去所有权的回调。
     */
    NVIC_DisableIRQ(ADC_IRQn);
    NVIC_ClearPendingIRQ(ADC_IRQn);

/****************************************/
/* H03：接通片内温度模拟输入路径        */
/****************************************/
    /* 使能温度传感器差分输入的两端；这里只接通路径，不启动转换。 */
    (void)ll_adc_tsensor_enable(true);

/****************************************/
/* H04：配置模拟前端与采样规则          */
/****************************************/
    /*
     * 装载 TCPL04B 厂商驱动采用的 ADC 模拟核心修调值。
     * 这些常量是芯片相关硬件配置，不是应用算法参数；迁移到其他芯片时
     * 必须重新由目标芯片手册/厂商驱动证明，禁止原样照搬。
     */
    TEST_CONFIG_UNLOCK();

    /*
     * ADC_TRIM0..7：八个 SAR ADC 模拟核心修调字。
     * 做什么：按 TCPL04B 原厂 LL 装入经过硅片验证的模拟修调序列。
     * 现象：缺失或错误时，ADC 增益、偏移、稳定时间或线性度可能偏离规格。
     * 本质原因：这些位直接作用于模拟核心，不是应用层数字标定参数。
     *
     * 证据边界：公开 Datasheet V1.0 未定义每个 TRIM 子位对应的晶体管级
     * 功能，因此下面只标明“修调字序号”，禁止臆造更细的物理解释。
     */

    /* 模拟核心修调字 0：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM0 = 0x74371F1DUL;

    /* 模拟核心修调字 1：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM1 = 0x00FA007CUL;

    /* 模拟核心修调字 2：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM2 = 0x02EF0177UL;

    /* 模拟核心修调字 3：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM3 = 0x0B0404F8UL;

    /* 模拟核心修调字 4：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM4 = 0x16010B04UL;

    /* 模拟核心修调字 5：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM5 = 0x108020FFUL;

    /* 模拟核心修调字 6：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM6 = 0x3F402100UL;

    /* 模拟核心修调字 7：原厂值，子字段物理含义未公开。 */
    TEST->ADC_TRIM7 = 0x00007900UL;
    TEST_CONFIG_LOCK();

    /*
     * 以下寄存器只建立工作条件，不在此处产生一批有效载荷。
     * - 软件触发：转换由 Read...() 调用发起；
     * - FIFO 阈值/通道采样数：与 ADC_SAMPLE_COUNT=4 对齐；
     * - 单通道、非连续、非自动：每次 API 调用形成一个边界清晰的事务；
     * - 采样周期/初始化周期：给模拟前端稳定和采样保持留出硬件时间。
     */
    /* TRIG_EN：只接受 CPU 软件触发，不接受 Timer/PWM/Tinywork 硬件触发。 */
    ADC->CTRL0_F.TRIG_EN = TRIG_SOFTWARE;

    /* FIFO_THRHLD=3：第 4 个结果进入硬件 FIFO 时达到阈值。寄存器采用 N-1。 */
    ADC->CTRL0_F.FIFO_THRHLD = ADC_SAMPLE_COUNT - 1U;

    /* I_SEL=1：沿用原厂 TCPL04B 模拟核心电流档；公开 Datasheet 未说明细分含义。 */
    ADC->CTRL0_F.I_SEL = 1U;

/****************************************/
/* H05：配置通道采样与SAR状态机         */
/****************************************/
    /* SW_CONT_MODE=0：一次软件事务完成后停止，不自动连续重启。 */
    ADC->CTRL1_F.SW_CONT_MODE = 0U;

    /* SCAN_CHNL_NUM=0：单通道模式，每次只转换 IN_SEL 选择的物理源。 */
    ADC->CTRL1_F.SCAN_CHNL_NUM = 0U;

    /* CHNL_SAMP_NUM=3：同一通道每次事务产生 4 个结果，寄存器采用 N-1。 */
    ADC->CTRL1_F.CHNL_SAMP_NUM = ADC_SAMPLE_COUNT - 1U;

    /* DIV_EN=1：打开原厂 ADC 内部分频/时序门；公开 Datasheet 未给出位级公式。 */
    ADC->CTRL1_F.DIV_EN = 1U;

    /* SAMP_CYCLE=0x0F：设置采样保持窗口；值来自 48 MHz 原厂初始化分支。 */
    ADC->CTRL2_F.SAMP_CYCLE = 0x0FU;

    /* INIT_CYCLE=0x320：设置模拟前端/转换启动稳定周期；值来自原厂初始化。 */
    ADC->CTRL2_F.INIT_CYCLE = 0x320U;

/****************************************/
/* H06：清理ADC硬件结果FIFO             */
/****************************************/
    /*
     * 清除复位前可能残留的 FIFO 结果，防止旧结果冒充首批新样本。
    */
    (void)ll_adc_fifo_clear();

/****************************************/
/* H07：最后打开软件控制运行门          */
/****************************************/
    /*
     * H03~H06 已经完整建立输入、转换和结果存放条件，至此才允许软件 Start。
     * SW_ADC_EN 只是开放运行门；当前函数不写 SW_START，不产生有效样本。
     */
    ADC->CTRL0_F.SW_ADC_EN = true;

    /* AUTO_ADC_EN=0：关闭 Tinywork/硬件自主路径，避免后台产生额外样本。 */
    ADC->CTRL0_F.AUTO_ADC_EN = false;

    /*
     * Register the ADC completion callback and load the TCPL04B VPN/AON
     * calibration coefficients needed by LED forward-voltage conversion.
     * Runtime TEMP/VBAT transactions still use the bounded polling API.
     */
    (void)ll_adc_init(&initConfig, Adc_LedIrqCallback);
    (void)ll_adc_tsensor_enable(true);
    (void)ll_adc_isr_enable(false);

    s_led_diag_state = ADC_LED_DIAG_IDLE;
    s_led_active_mask = 0U;
    s_led_next_channel = 0U;
    for (uint8_t index = 0U; index < ADC_LED_CHANNEL_COUNT; index++)
    {
        s_led_voltage_mv[index] = 0;
        s_led_voltage_valid[index] = false;
        s_led_hw_channel[index] = index;
    }
}

/****************************************/
/* 工作数据流05：温度到工程量 [WORKFLOW] */
/****************************************/
bool Adc_ReadMcuTempDeciC(int16_t *deci_celsius)
{
    int16_t code;
    int32_t tos = ADC_TEMP_DEFAULT_TOS;
    int32_t k = ADC_TEMP_DEFAULT_K;
    int32_t temperature;

    /* 工作数据流：TEMP 源 -> H05 采样/转换 -> H06 FIFO -> S02 四点原始码。 */
    if ((deci_celsius == NULL) ||
        !Adc_ReadAverage(ADC_CHANNEL_TEMP, &s_temp_cfg, &code))
    {
        return false;
    }

    /* S02：先保存原始温度码，供紧随其后的 VBAT VREF 补偿使用。 */
    s_last_temperature_code = code;

    /* 工作数据流：优先使用芯片工厂 TOS/K；无有效修调时使用厂商默认系数。 */
    if (Adc_HasTemperatureTrim())
    {
        if (TRIM->ADC_TSENSOR0_F.TOS != 0xFFFFU)
        {
            tos = TRIM->ADC_TSENSOR0_F.TOS;
        }
        if (TRIM->ADC_TSENSOR0_F.K != 0xFFFFU)
        {
            k = TRIM->ADC_TSENSOR0_F.K;
        }
    }
    if (k == 0L)
    {
        return false;
    }

    /*
     * 工作数据流：按数据手册/厂商公式把原始码换算成 0.1 degC。
     * 全程整数运算，避免为两个慢速测量引入浮点库。
     */
    temperature = Adc_DivideRounded((((int32_t)code * 5L) - (2L * tos)) * 80L, k) -
                  2732L;
    *deci_celsius = (int16_t)temperature;
    return true;
}

/****************************************/
/* 工作数据流06：VBAT到工程量 [WORKFLOW] */
/****************************************/
bool Adc_ReadBatteryMv(int16_t *millivolts)
{
    int16_t code;
    int32_t signed_code;
    int32_t vref_mv;
    int32_t voltage_mv;

    /* 工作数据流：VBAT 源 -> H05 采样/转换 -> H06 FIFO -> S02 四点原始码。 */
    if ((millivolts == NULL) ||
        !Adc_ReadAverage(ADC_CHANNEL_VBAT, &s_battery_cfg, &code))
    {
        return false;
    }

    /* 工作数据流：ADC 有效结果为 14 位有符号码，显式完成符号扩展。 */
    signed_code = ((int16_t)(code << 2U)) >> 2U;

    /*
     * S02 数字补偿：使用上一次温度采样估算 VREF，再结合 VBAT 通道 ratio=20
     * 换算为 mV。ModuleAdc 因此必须先读温度、再读 VBAT。
     */
    vref_mv = Adc_GetVrefMv(s_last_temperature_code);
    voltage_mv = (20L * signed_code * vref_mv + 4096L) / 8192L;
    *millivolts = (int16_t)voltage_mv;
    return true;
}

bool Adc_StartLedVoltageCycle(uint8_t active_mask)
{
    uint8_t *channelMux = NULL;
    uint8_t index;

    if ((active_mask & ((1U << ADC_LED_CHANNEL_COUNT) - 1U)) == 0U)
    {
        Adc_AbortLedCycle();
        return true;
    }
    if (s_led_diag_state != ADC_LED_DIAG_IDLE)
    {
        return false;
    }

    pal_led_channel_mux_get(LED_CHANNEL_0, &channelMux);
    if (channelMux == NULL)
    {
        return false;
    }
    for (index = 0U; index < ADC_LED_CHANNEL_COUNT; index++)
    {
        s_led_hw_channel[index] = channelMux[index];
    }

    s_led_active_mask =
        (uint8_t)(active_mask & ((1U << ADC_LED_CHANNEL_COUNT) - 1U));
    s_led_next_channel = 0U;
    Adc_StartNextLedChannel();
    return true;
}

bool Adc_GetLedForwardVoltageMv(uint8_t channel, int16_t *millivolts)
{
    if ((channel >= ADC_LED_CHANNEL_COUNT) || (millivolts == NULL) ||
        !s_led_voltage_valid[channel])
    {
        return false;
    }
    *millivolts = s_led_voltage_mv[channel];
    return true;
}
