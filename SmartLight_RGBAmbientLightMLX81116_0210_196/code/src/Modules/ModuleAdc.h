/*

 * adc.h
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#ifndef _MODULE_ADC_H_
#define _MODULE_ADC_H_

#include "../system/SystemConfig.h"
#include <stdbool.h>

typedef struct{
  SSingleLEDChannel LedVoltage[RGBLEDMAX];
}SLEDVoltages;

void moduleAdcInit(void);
void moduleAdcLedEnable(uint16_t ledIndex, ESingleLedChannel ledChanel, uint16_t enable);
void moduleAdcUpdate(void);

uint16_t moduleAdcDataReady(void);
void moduleAdcDataClear(void);

int16_t moduleAdcGetTemperature(void);
uint16_t moduleAdcGetVs(void);
//SLEDVoltages moduleAdcGetLedVoltage(void);
void moduleAdcGetLedVoltage(SLEDVoltages *result);
void moduleLedVoltageUpdate(void);
SLEDVoltages moduleVoltageGetLed(void);
uint8_t getLedAdcSampleState(uint16_t led_index, ESingleLedChannel led_chanel);

#endif /* CODE_SRC_DRIVERS_ADC_H_ */
