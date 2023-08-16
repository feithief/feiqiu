/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleTemperature.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file contains functions update LED junction temperature.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "ModuleTemperature.h"
#include "ModuleAdc.h"
#include "ModuleClock.h"
#include "SystemStatus.h"
#include "ModuleBattery.h"
#include "ModuleLed.h"
#include "ModuleFlash.h"
#include "Algorithm.h"


/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */
/*Defines temperature update period. Update function will capture temperature 
    every 10ms and make average every DTEMP_UPDATE_CYCLE_PERIOD(250ms), 
      and then make a filter((old*3+new)/4)*/
#define DTEMP_UPDATE_CYCLE_PERIOD   50     // ms

#define DTEMP_MCU_FILTER_LEVEL          2
#define DTEMP_LED_FILTER_LEVEL          16



#define MEASURE_TEMP_GAIN_POS    14U
#define MEASURE_TEMP_GAIN        7208 /*16384*0.43994*/

coeffParam_t Temp_coeff;
/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/*This variant shows current LED junction temperature.*/
static int16_t ledTemperature = 35*DTEMP_MULTIPLIED_RATIO;
static int16_t mcuTemperature = 30*DTEMP_MULTIPLIED_RATIO;

static int16_t tempJunctionFilter = 0, tempLedFilter = 0;
static bool_t initializationFlag = bfalse;
static tick_t updateTick = 0;

//static int16_t MCULastTemperature = 25*DTEMP_MULTIPLIED_RATIO;
/* ============================================================================ */
/* ------------------------- Appication Functions ----------------------------- */
/* ============================================================================ */

/**
*@details   Initialize temperature detection.
*
*@warning   This function must be invoked before @moduleTempUpdate.
*
*@retval    None.
*/
void moduleTempInit(void)
{
  //ColorUpdateINITFlag = true;
  Temp_coeff.coefficient = MEASURE_TEMP_GAIN;
  Temp_coeff.offset = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*(int32_t)HWCFG_SFRS->TSENSOR_mV_25C;
}

static int16_t moduleTempGetRaw()
{
  uint32_t rawVal = 0;
  int32_t  sVolt;
  uint32_t uVolt;
  int16_t  volt;
  
  int32_t  stemp;
  uint32_t uTemp;
  uint8_t  negative = 0U;
    
  rawVal = moduleADCValue(ADC_CH_VTEMP);
  rawVal = rawVal * 2;
  if(rawVal != 0)
  {
    sVolt = adc_coeff[ADC_CH_VTEMP].coefficient * (int32_t)rawVal + adc_coeff[ADC_CH_VTEMP].offset;
    uVolt = (uint32_t)sVolt;
    uVolt = uVolt >> MEASURE_GAIN_POS; 
    volt = (int16_t)uVolt;                        //计算电压
    
    stemp = Temp_coeff.coefficient * volt + Temp_coeff.offset; //计算温度
    
    if (stemp < 0){
          stemp = -stemp;
          negative = 1U;
      }
    uTemp = (uint32_t)stemp;
    uTemp = uTemp >> MEASURE_TEMP_GAIN_POS;
    stemp = (int32_t)uTemp;
    if (negative != 0U){
        stemp = -stemp;
    }
  }
  else
  {
    stemp = 30;
  }
  return (int16_t)stemp;
}

/**
*@details   Temperature detection callback function, Currently, we do this every 250ms(experience time).
*           This function will update @ledTemperature value.
*
*@note      For more details, please refer to development document.
*
*@retval    None.
*/
//int16_t debugTemp, debugJunction;
void moduleTempUpdate(void)
{
  int16_t tempTemperature;
  int32_t pMCU = 0, pRLed, pGLed, pBLed;
  int16_t ratioAdjusting;
  int16_t jointTemperature =25;
  
  /*Get the current temperature*/
  jointTemperature = moduleTempGetRaw();
  
  /*Every temperature multiplied by DTEMP_MUTIPLIED_RATIO.*/
  if (initializationFlag == bfalse)
    tempJunctionFilter = jointTemperature*DTEMP_MULTIPLIED_RATIO;
  else
  {
    tempJunctionFilter = (tempJunctionFilter*(DTEMP_MCU_FILTER_LEVEL-1) + jointTemperature*DTEMP_MULTIPLIED_RATIO);
    tempJunctionFilter = tempJunctionFilter/DTEMP_MCU_FILTER_LEVEL;
  }

  tempJunctionFilter = jointTemperature*DTEMP_MULTIPLIED_RATIO;

  pRLed = 30*moduleLedGetPWM(ELedChannelRed)*moduleLedGetVoltage(ELedChannelRed)/PWM_PERIOD;
  pGLed = 30*moduleLedGetPWM(ELedChannelGreen)*moduleLedGetVoltage(ELedChannelGreen)/PWM_PERIOD;
  pBLed = 30*moduleLedGetPWM(ELedChannelBlue)*moduleLedGetVoltage(ELedChannelBlue)/PWM_PERIOD;

  pRLed = pRLed*savedConfig.RthermolRatio;
  pGLed = pGLed*savedConfig.GthermolRatio;
  pBLed = pBLed*savedConfig.BthermolRatio;

  /*Chip 12v, 12mA*/
  pMCU  = moduleLedGetPWM(ELedChannelRed)  * (moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelRed));
  pMCU += moduleLedGetPWM(ELedChannelGreen)* (moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelGreen));
  pMCU += moduleLedGetPWM(ELedChannelBlue) * (moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelBlue));

  if (moduleBatGetVoltage() >= 12000)
    ratioAdjusting = ((int16_t)(moduleBatGetVoltage() - 12000))/1000*savedConfig.RatingHigherVoltage/10;
  else
    ratioAdjusting = ((int16_t)(12000 - moduleBatGetVoltage()))/1000*savedConfig.RatingLowerVoltage/10;

  pMCU = pMCU/PWM_PERIOD*(savedConfig.MCUthermolRatio + ratioAdjusting);
  pMCU = pMCU*30 + 8*moduleBatGetVoltage()*savedConfig.MCUthermolRatio;

  /*XXXXRatio/100 = XXXXRatio in percentage.*/
  /*Every temperature multiplied by DTEMP_MUTIPLIED_RATIO.*/
  tempTemperature = (int32_t)tempJunctionFilter -
                  (pMCU - pRLed - pGLed - pBLed)/(int32_t)1000000*(int32_t)DTEMP_MULTIPLIED_RATIO;

  tempTemperature += 8*DTEMP_MULTIPLIED_RATIO;//3

  if (initializationFlag == bfalse)
    tempLedFilter = tempTemperature;
  else
    tempLedFilter = (tempLedFilter*(DTEMP_LED_FILTER_LEVEL-1) + tempTemperature)/DTEMP_LED_FILTER_LEVEL;

  initializationFlag = btrue;

  /*we update temperature every DTEMP_UPDATE_CYCLE_PERIOD MS*/
  if (DTickAfter(moduleClockGet(), updateTick + DTEMP_UPDATE_CYCLE_PERIOD))
  {
    updateTick = moduleClockGet();

//    debugTemp = tempLedFilter;
//    debugJunction = tempJunctionFilter;

    ledTemperature = tempLedFilter;
    mcuTemperature = tempJunctionFilter;
  }
}


/**
*@details   Get current LED temperature.
*
*@retval    current LED temperature.
*/
int16_t moduleTempLedGet(void)
{
  //Debug.T_LED = ledTemperature;
  return ledTemperature;
  //return 25*DTEMP_MULTIPLIED_RATIO;
}

/**
*@details   Get current MCU temperature.
*
*@retval    current MCU temperature.
*/
int16_t moduleTempMcuGet(void)
{ 
  //Debug.T_MCU = mcuTemperature;
  return mcuTemperature;
  //return 25*DTEMP_MULTIPLIED_RATIO;
}
