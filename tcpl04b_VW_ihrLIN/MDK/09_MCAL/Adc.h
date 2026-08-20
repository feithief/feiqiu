#ifndef SWSC_TCPL04B_ADC_H
#define SWSC_TCPL04B_ADC_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief 初始化 ADC 硬件，但不在后台持续采样。
 *
 * H 类硬件准备：先打开寄存器访问/转换时钟 -> 复位并停止 -> 接通输入 ->
 * 配置模拟前端、触发、通道和采样状态机 -> 清 FIFO -> 最后开放软件运行门。
 * I 类中断准备：屏蔽/清除 ADC 事件，关闭并清除 NVIC ADC 入口，清空回调。
 * 调用后 ADC 处于“已配置、等待软件请求”状态。
 *
 * @note 只允许系统初始化阶段调用；寄存器所有权属于 ADC MCAL。
 */
void Adc_Init(void);

/**
 * @brief 同步采集 VBAT 监测通道并换算为毫伏。
 * @param[out] millivolts 成功时写入电池电压，单位 mV。
 * @return true：4 个结果完整且换算成功；false：空指针、轮询超时或结果不足。
 * @note 本接口会触发硬件并阻塞到本次转换结束，不应在 ISR 中调用。
 * @note VREF 温漂补偿依赖最近一次温度通道原始码，应先调用温度读取接口。
 */
bool Adc_ReadBatteryMv(int16_t *millivolts);

/**
 * @brief 同步采集片内温度通道并换算为 0.1 摄氏度。
 * @param[out] deci_celsius 成功时写入温度；250 表示 25.0 摄氏度。
 * @return true：4 个结果完整且标定换算成功；false：参数或硬件采样失败。
 * @note 本接口会触发硬件并阻塞到本次转换结束，不应在 ISR 中调用。
 */
bool Adc_ReadMcuTempDeciC(int16_t *deci_celsius);

/**
 * @brief 启动一轮与 PWM 同步的 RGB LED 正向压降采样。
 * @param active_mask bit0/bit1/bit2 分别表示 R/G/B 当前需要检测。
 * @return true：本轮已启动或没有活动通道；false：上一轮仍在执行。
 * @note 采样链路完全位于 MCAL/PWM/ADC 中断中，上层不接触芯片通道号。
 */
bool Adc_StartLedVoltageCycle(uint8_t active_mask);

/**
 * @brief 读取最近一次成功的 LED 正向压降结果。
 * @param channel 逻辑通道 0=R、1=G、2=B。
 * @param[out] millivolts 正向压降，单位 mV。
 * @return true：该通道至少完成过一次有效采样；false：无有效结果。
 */
bool Adc_GetLedForwardVoltageMv(uint8_t channel, int16_t *millivolts);

#endif
