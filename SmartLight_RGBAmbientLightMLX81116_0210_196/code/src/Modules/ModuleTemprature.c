/*
 * temperature.c
 *
 *  Created on: 2019Äê12ÔÂ29ÈÕ
 *      Author: SH1705016
 */
#include "ModuleTemprature.h"
#include "ModuleAdc.h"
#include "ModuleFlash.h"
#include "ModulePwm.h"

static int16_t currentMcuTemp = 25;
static int16_t lastcurrentMcuTemp = 25;
static int16_t currentLedTemp[RGBLEDMAX];
static int16_t lastcurrentLedTemp[RGBLEDMAX] = {250, 250, 250, 250, 250, 250};

void moduleTempratureInit(void)
{
  uint16_t index;

  currentMcuTemp = 25;

  for(index  = 0; index < RGBLEDMAX; index++)
    currentLedTemp[index] = 25*DTEMP_MULTIPLIED_RATIO;
}

/* temperature offset value(ADC Value) at EE_TEMP_MID for linearization, [LSB], 16bit */
#define EE_OTEMP_CAL              (*(uint16_t *)(0x09B4))
/* TempMid stored in EEPROM is the real temperature of the first test in degC */
#define EE_TEMP_MID              (*(int8_t *)(0x09B6))
/* gain factor of temp sensor from -40°C to EE_TEMP_MID, [K/LSB] */
#define EE_GAIN_TEMP_LOW_CAL      (*(int8_t *)(0x09B8))
/* gain factor of temp sensor from EE_TEMP_MID to 125°C, [K/LSB] */
#define EE_GAIN_TEMP_HIGH_CAL     (*(int8_t *)(0x09B9))

#define TEMPGRADSCALE 128u

/* ----------------------------------------------------------------------------
 * convert ADC value into degree Celsius for internal temperature sensor
 */
static int16_t calcTemperature(uint16_t adcval){
    int16_t result = 0;

    if(adcval>EE_OTEMP_CAL) {
        /* negative temperature gradient needed */
        result = (int16_t)((int16_t)((int16_t)(adcval-EE_OTEMP_CAL)*(int16_t)(EE_GAIN_TEMP_LOW_CAL))/TEMPGRADSCALE);
        result = (int16_t)(EE_TEMP_MID - result);
    }
    else {
        /* positive temperature gradient needed */
        result = (int16_t)((int16_t)((int16_t)(EE_OTEMP_CAL-adcval)*(int16_t)(EE_GAIN_TEMP_HIGH_CAL))/TEMPGRADSCALE);
        result = (int16_t)(result + EE_TEMP_MID);
    }

    return result;
}

//int16_t redLedTemp[6] = {250, 250, 250, 250, 250, 250};
//int16_t greenLedTemp[6] = {250, 250, 250, 250, 250, 250};
//int16_t blueLedTemp[6] = {250, 250, 250, 250, 250, 250};
//
//int16_t redLedTempDifference[6] = {0};
//int16_t greenLedTempDifference[6] = {0};
//int16_t blueLedTempDifference[6] = {0};
//
//int16_t redLedVoltageDifference[6] = {0};
//int16_t greenLedVoltageDifference[6] = {0};
//int16_t blueLedVoltageDifference[6] = {0};
//
//int32_t slopeRed[RGBLEDMAX] = {-16098, -15866, -15866, -16115, -16098, -16605};
//int32_t slopeGreen[RGBLEDMAX] = {-34738, -34738, -34738, -34738, -34738, -34738};
//int32_t slopeBlue[RGBLEDMAX] = {-15814, -16150, -16185, -15422, -16294, -18142};

extern SLEDPwmSet pwmParam;
extern const uint16_t LEDCHANNELMAP[RGBLEDMAX*RGBLEDCHANNELS];
uint8_t flag_led_temperature_calculate_first_time = 1;
uint8_t flag_mcu_temperature_calculate_first_time = 1;
void moduleTempratureUpdate(SLEDVoltages *LedVoltage)
{
  uint16_t index;
  int16_t currentMcuAdc = 0;

  currentMcuAdc = moduleAdcGetTemperature();

  currentMcuTemp = calcTemperature(currentMcuAdc);

  if(flag_mcu_temperature_calculate_first_time == 1)
  {
	  lastcurrentMcuTemp = currentMcuTemp;
	  flag_mcu_temperature_calculate_first_time = 0;
  }
  else
  {
	  currentMcuTemp = (lastcurrentMcuTemp*4 + currentMcuTemp*3)/7;
	  lastcurrentMcuTemp = currentMcuTemp;
  }

  for (index = 0; index < RGBLEDMAX; index++)
  {
//	  if(
//		  (LedVoltage->LedVoltage[index].ledChannelData[1] > 400)
//		  &&(LedVoltage->LedVoltage[index].ledChannelData[1] < 3500)
//		  &&(LedVoltage->LedVoltage[index].ledChannelData[0] > 400)
//		  &&(LedVoltage->LedVoltage[index].ledChannelData[0] < 4500)
//		  &&(LedVoltage->LedVoltage[index].ledChannelData[2] > 400)
//		  &&(LedVoltage->LedVoltage[index].ledChannelData[2] < 4500)
//		)
//	  {
//		  if(getLedAdcSampleState(index, 1) == 1)
//		  {
//			  redLedVoltageDifference[index] = LedVoltage->LedVoltage[index].ledChannelData[1] - savedConfig.LedVoltage_25Celsius.LedVoltage[index].ledChannelData[1];
//			  redLedTempDifference[index] = ((int32_t)redLedVoltageDifference[index])*10000/slopeRed[index]*DTEMP_MULTIPLIED_RATIO;
//			  redLedTemp[index] = 250 + redLedTempDifference[index];
//			  redLedTemp[index] = (redLedTemp[index] > 1200)?1200:redLedTemp[index];
//			  redLedTemp[index] = (redLedTemp[index] < (-400))?(-400):redLedTemp[index];
//		  }
//		  if(getLedAdcSampleState(index, 0) == 1)
//		  {
//			  greenLedVoltageDifference[index] = LedVoltage->LedVoltage[index].ledChannelData[0] - savedConfig.LedVoltage_25Celsius.LedVoltage[index].ledChannelData[0];
//			  greenLedTempDifference[index] = ((int32_t)greenLedVoltageDifference[index])*10000/slopeGreen[index]*DTEMP_MULTIPLIED_RATIO;
//			  greenLedTemp[index] = 250 + greenLedTempDifference[index];
//			  greenLedTemp[index] = (greenLedTemp[index] > 1200)?1200:greenLedTemp[index];
//			  greenLedTemp[index] = (greenLedTemp[index] < (-400))?(-400):greenLedTemp[index];
//		  }
//		  if(getLedAdcSampleState(index, 2) == 1)
//		  {
//			  blueLedVoltageDifference[index] = LedVoltage->LedVoltage[index].ledChannelData[2] - savedConfig.LedVoltage_25Celsius.LedVoltage[index].ledChannelData[2];
//			  blueLedTempDifference[index] = ((int32_t)blueLedVoltageDifference[index])*10000/slopeBlue[index]*DTEMP_MULTIPLIED_RATIO;
//			  blueLedTemp[index] = 250 + blueLedTempDifference[index];
//			  blueLedTemp[index] = (blueLedTemp[index] > 1200)?1200:blueLedTemp[index];
//			  blueLedTemp[index] = (blueLedTemp[index] < (-400))?(-400):blueLedTemp[index];
//		  }
//
//		  currentLedTemp[index] = (redLedTemp[index] + greenLedTemp[index] + blueLedTemp[index])/3;

	  currentLedTemp[index] = currentMcuTemp -
		  (((int32_t)(5000 - LedVoltage->LedVoltage[index].ledChannelData[1]))*40/1000*176*pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 0]]/1000/32767 +
		   ((int32_t)(5000 - LedVoltage->LedVoltage[index].ledChannelData[0]))*40/1000*200*pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 1]]/1000/32767 +
		   ((int32_t)(5000 - LedVoltage->LedVoltage[index].ledChannelData[2]))*50/1000*205*pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 2]]/1000/32767);
	  currentLedTemp[index] = currentLedTemp[index]*9/10;
	  currentLedTemp[index] = currentLedTemp[index]*DTEMP_MULTIPLIED_RATIO;
//	  }
  }
  if(flag_led_temperature_calculate_first_time == 1)
  {
	  for (index = 0; index < RGBLEDMAX; index++)
	  {
		  lastcurrentLedTemp[index] = currentLedTemp[index];
	  }
	  flag_led_temperature_calculate_first_time = 0;
  }
  else
  {
	  for (index = 0; index < RGBLEDMAX; index++)
	  {
		  currentLedTemp[index] = (lastcurrentLedTemp[index]*4 + currentLedTemp[index]*3)/7;
		  lastcurrentLedTemp[index] = currentLedTemp[index];
	  }
  }
}

int16_t moduleTempratureGetMcu(void)
{
  return currentMcuTemp;
}

int16_t moduleTempratureGetLed(uint16_t ledNum)
{
  if (ledNum >= RGBLEDMAX)
    ledNum  = RGBLEDMAX-1;

  return currentLedTemp[ledNum];
}
