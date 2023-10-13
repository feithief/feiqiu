/*
 * ModuleBattery.c
 *
 *  Created on: 2020Äê1ÔÂ1ÈÕ
 *      Author: SH1705016
 */

#include "ModuleBattery.h"
#include "ModuleFlash.h"
#include "ModuleAdc.h"

static uint16_t currentVoltage = 5000;

void moduleBatteryInit(void)
{
  if (savedConfig.LedNumber == 1)
  {
    currentVoltage = 5000;
  }
  else
  {
    currentVoltage = 7000;
  }
}

/* ----------------------------------------------------------------------------
 * convert ADC value into mV for VS measurement
 */
static uint16_t calcVSmVolt(uint16_t adcval){
    uint16_t result = 0;

    /* VS_Voltage [mV] = (ADC_Val * 14 * ADC_Ref_Volt(2.5) / ADC_Resolution (1023)) * 1000 (mV)
     * -> 2.5 = 5/2 -> avoids float operations */
    result = adcval * 14;//ADCVSDIVIDER = 14
    result = (uint16_t)((uint32_t)((uint32_t)result * (uint32_t)5000) / (uint32_t)2046);

    return result;
}

void moduleBatteryUpdate(void)
{
  uint16_t tempValue = moduleAdcGetVs();

  currentVoltage = calcVSmVolt(tempValue);
}

uint16_t moduleBatteryGet(void)
{
  return currentVoltage;
}
