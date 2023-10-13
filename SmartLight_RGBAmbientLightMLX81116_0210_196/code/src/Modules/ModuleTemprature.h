/*

 * temperature.h
 *
 *  Created on: 2019Äê12ÔÂ29ÈÕ
 *      Author: SH1705016
 */

#ifndef _MODULE_TEMPERATURE_H_
#define _MODULE_TEMPERATURE_H_

#include "../system/SystemConfig.h"
#include "ModuleAdc.h"

void moduleTempratureInit(void);
//void moduleTempratureUpdate(void);
int16_t moduleTempratureGetMcu(void);
int16_t moduleTempratureGetLed(uint16_t ledNum);
void moduleTempratureUpdate(SLEDVoltages *LedVoltage);

#endif /* CODE_SRC_DRIVERS_MODULETEMPERATURE_H_ */
