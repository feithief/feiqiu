#ifndef SWSC_MODULE_ADC_H
#define SWSC_MODULE_ADC_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief 完成 S 类 RAM/所有权准备，初始化 ADC MCAL，并采集首轮快照。
 * @note 由系统初始化统一调用一次；应用模块不应重复初始化硬件。
 */
void IoHwAb_Adc_Init(void);

/**
 * @brief ADC 的 100 ms 周期入口：先温度、后 VBAT，并更新模块缓存。
 * @note 这是当前设计中唯一的周期采样入口；不得并发重入。
 */
void IoHwAb_Adc_MainFunction100ms(void);

/**
 * @brief 从 ModuleAdc RAM 缓存取得最近一次有效电池电压。
 * @param[out] millivolts 电池电压，单位 mV。
 * @return true：已复制有效快照；false：空指针或最近一次采样无效。
 * @note 不触发 ADC、不等待硬件。
 */
bool IoHwAb_Adc_GetBatteryMv(int16_t *millivolts);

/**
 * @brief 从 ModuleAdc RAM 缓存取得最近一次有效芯片温度。
 * @param[out] deci_celsius 温度，单位 0.1 摄氏度。
 * @return true：已复制有效快照；false：空指针或最近一次采样无效。
 * @note 不触发 ADC、不等待硬件。
 */
bool IoHwAb_Adc_GetMcuTempDeciC(int16_t *deci_celsius);

#endif
