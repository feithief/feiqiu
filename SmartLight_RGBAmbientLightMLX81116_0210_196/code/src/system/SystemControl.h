/*
 * SystemControl.h
 *
 *  Created on: 2020Äê1ÔÂ1ÈÕ
 *      Author: SH1705016
 */

#ifndef SYSTEM_CONTROL_H_
#define _SYSTEM_CONTROL_H_

#include "SystemConfig.h"
#include "ModuleComm.h"
#include "Algorithm.h"
#include "ModuleAdc.h"

extern SLEDVoltages currentLedVoltage;

extern uint16_t systemStatus[];

extern uint16_t ledCalData[];

void systemControlInit(void);

void systemProtocolControl(SCommMessage *Message, SColorParams *params);

void slaveStatusBufferUpdate(void);

void LedVoltageInit(SLEDVoltages *LedVoltage);

#endif /* CODE_SRC_SYSTEM_SYSTEMCONTROL_H_ */
