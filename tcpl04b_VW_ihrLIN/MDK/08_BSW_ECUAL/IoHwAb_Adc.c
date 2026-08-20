#include "IoHwAb_Adc.h"
#include "Adc.h"

#include <stddef.h>

/**
 * @file ModuleAdc.c
 * @brief ADC 的 ECU Abstraction（设备/功能抽象）层。
 *
 * 分层边界：
 *   Tcpl04bAdc(MCAL)  拥有寄存器、通道配置、FIFO 轮询和原始码标定；
 *   ModuleAdc         拥有 100 ms 采样节拍、最新工程量及有效标志；
 *   温度/电池等上层  只通过 Getter 读取 RAM 快照，不直接触碰 ADC 硬件。
 *
 * 端到端软件数据流：
 *   100 ms 调度
 *     -> 温度同步采样（同时更新 VREF 补偿所需温度码）
 *     -> VBAT 同步采样
 *     -> s_* 工程量缓存 + s_*_valid
 *     -> 上层 Getter 读取一致的最近一次快照。
 *
 * 当前模块没有中断回调、DMA 完成回调和跨任务队列。缓存只在本模块写，
 * 上层只读，从而把硬件采样时序与应用读取时刻解耦。
 */

/****************************************/
/* S02：工程量与有效标志RAM缓存         */
/****************************************/
/* ModuleAdc 独占写权限，上层只能通过 Getter 读取。 */
static int16_t s_battery_mv;
static int16_t s_mcu_temp_deci_c;
static bool s_battery_valid;
static bool s_temperature_valid;

/****************************************/
/* S03：调度、CPU接口与模块所有权        */
/****************************************/
void IoHwAb_Adc_Init(void)
{
    /*
     * S02 软件状态准备：每次初始化都先撤销旧快照的有效性；不能依赖上电时
     * BSS 恰好清零，因为重新初始化时 RAM 中可能仍是上一轮有效数据。
     */
    s_battery_mv = 0;
    s_mcu_temp_deci_c = 0;
    s_battery_valid = false;
    s_temperature_valid = false;

    /* S03：系统初始化上下文取得 ADC 模块所有权，随后才开始 H/I 类准备。 */
    /* 先完成 ADC 的 Clock/Access -> Stop -> Configure -> Arm；此时尚无样本。 */
    Adc_Init();

    /*
     * 立即执行首轮同步采样，不必等第一个 100 ms 节拍。
     * 首轮成功前 Getter 通过 valid=false 拒绝发布未初始化的零值。
     */
    IoHwAb_Adc_MainFunction100ms();
}

/****************************************/
/* 工作数据流07：更新S02缓存 [WORKFLOW] */
/****************************************/
void IoHwAb_Adc_MainFunction100ms(void)
{
    /*
     * S03 调度入口拥有本轮同步采样，Getter 不得并发触发第二个硬件事务。
     *
     * 顺序是设计约束，不可随意交换：
     * 1. 温度采样更新 MCAL 内部的温度原始码；
     * 2. VBAT 换算立即使用该原始码补偿 ADC 参考电压温漂。
     *
     * 单项失败只清除对应 valid，保留另一项是否可用的独立状态。
     */
    s_temperature_valid = Adc_ReadMcuTempDeciC(&s_mcu_temp_deci_c);
    s_battery_valid = Adc_ReadBatteryMv(&s_battery_mv);
}

/****************************************/
/* 工作数据流08：发布电压快照 [WORKFLOW] */
/****************************************/
bool IoHwAb_Adc_GetBatteryMv(int16_t *millivolts)
{
    /* S03：Getter 只读 S02 RAM，不访问寄存器、不触发转换。 */
    if ((millivolts == NULL) || !s_battery_valid)
    {
        return false;
    }
    *millivolts = s_battery_mv;
    return true;
}

/****************************************/
/* 工作数据流09：发布温度快照 [WORKFLOW] */
/****************************************/
bool IoHwAb_Adc_GetMcuTempDeciC(int16_t *deci_celsius)
{
    /* S03：仅发布最近一次成功的 S02 快照；失败样本不会冒充有效数据。 */
    if ((deci_celsius == NULL) || !s_temperature_valid)
    {
        return false;
    }
    *deci_celsius = s_mcu_temp_deci_c;
    return true;
}
