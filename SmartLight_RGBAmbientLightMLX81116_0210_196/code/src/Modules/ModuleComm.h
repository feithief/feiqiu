/*

 * comm.h
 *
 *  Created on: 2019Äê6ÔÂ9ÈÕ
 *      Author: SH1705016
 */

#ifndef _MODULE_COMM_H_
#define _MODULE_COMM_H_

#include "../system/SystemConfig.h"

typedef enum CommFunction
{
	ECommFunctionNone = 0x0,
	ECommFunctionLedColor,
	ECommFunctionCalLed,
	ECommFunctionCalData,
	ECommFunctionRequestCalData,
	ECommFunctionRequestLedStatus,
	ECommFunctionAutoAddressing,
	ECommFunctionEnterProgrammingMode,

}ECommFunction;

typedef struct CommMessage
{
  ECommFunction func;
  uint8_t reserved;
  uint16_t msgLength;
  uint8_t* data;
} SCommMessage;

void moduleCommInit(void);

bool_t moduleCommFlagUpdated(void);

void moduleCommFlagClear(void);

bool_t moduleCommGetData(SCommMessage *Message);

uint16_t Get_Transmit_Lock(void);

//void ClearMeliubuBuffer(void);

#endif /* CODE_SRC_DRIVERS_MODULECOMM_H_ */
