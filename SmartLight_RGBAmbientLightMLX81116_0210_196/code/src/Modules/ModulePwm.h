/*

 * ledpwm.h
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#ifndef _MODULE_PWM_H_
#define _MODULE_PWM_H_

#include "../system/SystemConfig.h"
#include "../Algorithm/Algorithm.h"

typedef struct{
  uint16_t LedRGB[NUMBOFSINGLELEDS];
}SLEDPwmSet;

void modulePwmInit(void);
void modulePwmSet(SPWMParams *ouputPwm);
void modulePwmLedSpecialMask(uint8_t mask);

#endif /* CODE_SRC_DRIVERS_LEDPWM_H_ */
