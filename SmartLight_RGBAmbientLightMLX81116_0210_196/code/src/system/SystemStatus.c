#include "SystemStatus.h"

/*
 * systemStatus.c
 *
 *  Created on: 2019Äê12ÔÂ29ÈÕ
 *      Author: SH1705016
 */

#include "../Modules/moduleTemprature.h"
#include "../Modules/moduleBattery.h"
#include "../Modules/moduleAdc.h"
#include "SystemControl.h"

#define LOOP_CNT_GET_TEMPERATURE 300

void systemStatusInit(void)
{
	LedVoltageInit(&currentLedVoltage);
}

void systemStatusUpdate(void)
{
  static uint16_t Main_Loop_Cnt = 0;

  if (moduleAdcDataReady() == 1)
  {
    moduleAdcDataClear();

    moduleBatteryUpdate();
    moduleLedVoltageUpdate();
    if(Main_Loop_Cnt == LOOP_CNT_GET_TEMPERATURE)
	{
		moduleTempratureUpdate(&currentLedVoltage);
		Main_Loop_Cnt = 0;
	}

    slaveStatusBufferUpdate();

    Main_Loop_Cnt++;
  }
  else
  {}
}
