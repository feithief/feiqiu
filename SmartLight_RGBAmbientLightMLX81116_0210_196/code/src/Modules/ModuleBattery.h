/*

 * ModuleBattery.h
 *
 *  Created on: 2020Äê1ÔÂ1ÈÕ
 *      Author: SH1705016
 */

#ifndef _MODULE_BATTERY_H_
#define _MODULE_BATTERY_H_

#include "../system/SystemConfig.h"

void moduleBatteryInit(void);
void moduleBatteryUpdate(void);
uint16_t moduleBatteryGet(void);

#endif /* CODE_SRC_MODULES_MODULEBATTERY_H_ */
