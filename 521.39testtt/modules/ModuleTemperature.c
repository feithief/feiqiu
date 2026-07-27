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


/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */
/*Defines temperature update period. Update function will capture temperature 
    every 10ms and make average every DTEMP_UPDATE_CYCLE_PERIOD(250ms), 
      and then make a filter((old*3+new)/4)*/
#define DTEMP_UPDATE_CYCLE_PERIOD   250

#define DTEMP_MCU_FILTER_LEVEL          2
#define DTEMP_LED_FILTER_LEVEL          16
#define DLED_CURRENT_MAX                21
#define DTEMP_LED_FILTER2_LEVEL          20
/*the variable is the coefficient of temperature-ADC Line.
    ADC = -3.959T + 1699.875
point(125,1205) is on the function graph*/
#define TemperCoeff 3959


/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/*This variant shows current LED junction temperature.*/
static int16_t ledTemperature;
static int16_t mcuTemperature;
static int16_t tempJunctionFilter = 0, tempLedFilter = 0, tempLedFilter2 = 0;

static bool_t initializationFlag = bfalse;
static tick_t updateTick = 0;


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
}

static int16_t moduleTempGetRaw()
{
  int32_t rawVal;
  rawVal=(uint32_t)ReturnADCBuffer(ADC_CH_VTEMP);
  if(rawVal>=4090) rawVal=4090;
  if(rawVal<=10) rawVal=10;
  
  rawVal=(rawVal-1205)*1000;
  rawVal=rawVal/TemperCoeff;
  rawVal=125-rawVal;
  return (int16_t)rawVal;
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
    
    //tempJunctionFilter = jointTemperature*DTEMP_MULTIPLIED_RATIO;
    
    pRLed = (int32_t)moduleLedGetPWM(ELedChannelRed)*(int32_t)moduleLedGetVoltage(ELedChannelRed)/PWM_PERIOD;
    pGLed = (int32_t)moduleLedGetPWM(ELedChannelGreen)*(int32_t)moduleLedGetVoltage(ELedChannelGreen)/PWM_PERIOD;
    pBLed = (int32_t)moduleLedGetPWM(ELedChannelBlue)*(int32_t)moduleLedGetVoltage(ELedChannelBlue)/PWM_PERIOD;
    
    //savedConfig.R_ADC = moduleLedGetVoltage(ELedChannelRed);
    //savedConfig.G_ADC = moduleLedGetVoltage(ELedChannelGreen);
    //savedConfig.B_ADC = moduleLedGetVoltage(ELedChannelBlue);
    
    pRLed = pRLed*(int32_t)savedConfig.RthermolRatio*DLED_CURRENT_MAX;
    pGLed = pGLed*(int32_t)savedConfig.GthermolRatio*DLED_CURRENT_MAX;
    pBLed = pBLed*(int32_t)savedConfig.BthermolRatio*DLED_CURRENT_MAX;
    
    /*Chip 12v, 12mA*/
    pMCU  = (int32_t)moduleLedGetPWM(ELedChannelRed)*(int32_t)(moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelRed));
    pMCU += (int32_t)moduleLedGetPWM(ELedChannelGreen)*(int32_t)(moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelGreen));
    pMCU += (int32_t)moduleLedGetPWM(ELedChannelBlue)*(int32_t)(moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelBlue));
    
    if (moduleBatGetVoltage() >= 12000)
        ratioAdjusting = ((int16_t)(moduleBatGetVoltage() - 12000))/1000*savedConfig.RatingHigherVoltage/10;
    else
        ratioAdjusting = ((int16_t)(12000 - moduleBatGetVoltage()))/1000*savedConfig.RatingLowerVoltage/10;
    
    pMCU = pMCU/PWM_PERIOD*(savedConfig.MCUthermolRatio + ratioAdjusting);
    pMCU = pMCU*(int32_t)DLED_CURRENT_MAX + 10*moduleBatGetVoltage()*savedConfig.MCUthermolRatio;
    //  pMCU = pMCU/PWM_PERIOD*(int32_t)(debugthermal + ratioAdjusting);
    //  pMCU = pMCU*DLED_CURRENT_MAX + 10*(int32_t)moduleBatGetVoltage()*(int32_t)debugthermal;
    
    /*XXXXRatio/100 = XXXXRatio in percentage.*/
    /*Every temperature multiplied by DTEMP_MUTIPLIED_RATIO.*/
    tempTemperature = (int32_t)tempJunctionFilter -
        (pMCU - pRLed - pGLed - pBLed)/(int32_t)1000000*(int32_t)DTEMP_MULTIPLIED_RATIO;
    
    tempTemperature += 10*DTEMP_MULTIPLIED_RATIO;
    
    if (initializationFlag == bfalse)
        tempLedFilter = tempTemperature;
    else
    {
        tempLedFilter2 = (tempLedFilter*(DTEMP_LED_FILTER_LEVEL-1) + tempTemperature)/DTEMP_LED_FILTER_LEVEL;
        tempLedFilter = (tempLedFilter*(DTEMP_LED_FILTER2_LEVEL-1) + tempLedFilter2)/DTEMP_LED_FILTER2_LEVEL;
    }
    
    
    
    /*we update temperature every DTEMP_UPDATE_CYCLE_PERIOD MS*/
    if ((DTickAfter(moduleClockGet(), updateTick + DTEMP_UPDATE_CYCLE_PERIOD)) || (initializationFlag == bfalse))
    {
        updateTick = moduleClockGet();
        
        //    debugTemp = tempLedFilter;
        //    debugJunction = tempJunctionFilter;
        
        ledTemperature = tempLedFilter;
        mcuTemperature = tempJunctionFilter;
        
        //    savedConfig.Reserved[0]=(uint16_t)ledTemperature;
        //    savedConfig.Reserved[1]=(uint16_t)mcuTemperature;
        //    savedConfig.Reserved[2]=(uint16_t)AD_data_temperatrue;
    }
    initializationFlag = btrue;
}

/**
*@details   Get current LED temperature.
*
*@retval    current LED temperature.
*/
int16_t moduleTempLedGet(void)
{
  
  moduleTempUpdate();
  return ledTemperature;
}

/**
*@details   Get current MCU temperature.
*
*@retval    current MCU temperature.
*/
int16_t moduleTempMcuGet(void)
{ 
  return mcuTemperature;
}
