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
//#include "SystemStatus.h"
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

#define DTEMP_MCU_FILTER_LEVEL          8
#define DTEMP_LED_FILTER_LEVEL          16
#define DLED_CURRENT_MAX                21
//#define DTEMP_LED_FILTER2_LEVEL          20
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
  int32_t rawVal, Temp;
//  if(rawVal>=4090) rawVal=4090;		//delete		
//  if(rawVal<=10) rawVal=10;
	Temp = ReturnADCBuffer(ADC_CH_VTEMP);
  rawVal = -40 + (Temp * 1222 - 204800 ) * 190 * 5 / 4/ 2048000 ;
	
	
  //rawVal = -40 + ((ReturnADCBuffer(ADC_CH_VTEMP) * 1.222 / 2048) - 0.1) * 190 / 0.8 ;
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
void moduleTempUpdate(void)
{

	  int16_t jointTemperature = moduleTempGetRaw();

	   int16_t tempTemperature;
	   int32_t pMCU = 0, pRLed, pGLed, pBLed;
	   int16_t ratioAdjusting;

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

	   pRLed = pRLed*(int32_t)DLED_CURRENT_MAX;
	   pGLed = pGLed*(int32_t)DLED_CURRENT_MAX;
	   pBLed = pBLed*(int32_t)DLED_CURRENT_MAX;

	   pRLed = pRLed*(int32_t)savedConfig.RthermolRatio;
	   pGLed = pGLed*(int32_t)savedConfig.GthermolRatio;
	   pBLed = pBLed*(int32_t)savedConfig.BthermolRatio;

	   /*Chip 12v, 12mA*/
	   pMCU  = (int32_t)moduleLedGetPWM(ELedChannelRed)*(int32_t)(moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelRed));
	   pMCU += (int32_t)moduleLedGetPWM(ELedChannelGreen)*(int32_t)(moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelGreen));
	   pMCU += (int32_t)moduleLedGetPWM(ELedChannelBlue)*(int32_t)(moduleBatGetVoltage() - moduleLedGetVoltage(ELedChannelBlue));

	   if (moduleBatGetVoltage() >= 12000)
	     ratioAdjusting = ((int16_t)(moduleBatGetVoltage() - 12000))/1000*savedConfig.RatingHigherVoltage/10;
	   else
	     ratioAdjusting = ((int16_t)(12000 - moduleBatGetVoltage()))/1000*savedConfig.RatingLowerVoltage/10;

	   pMCU = pMCU/PWM_PERIOD*(ratioAdjusting + savedConfig.MCUthermolRatio);
	   pMCU = pMCU*(int32_t)DLED_CURRENT_MAX + 10*moduleBatGetVoltage()* savedConfig.MCUthermolRatio;
	 //  pMCU = pMCU/PWM_PERIOD*(int32_t)(debugthermal + ratioAdjusting);
	 //  pMCU = pMCU*DLED_CURRENT_MAX + 10*(int32_t)moduleBatGetVoltage()*(int32_t)debugthermal;

	   /*XXXXRatio/100 = XXXXRatio in percentage.*/
	   /*Every temperature multiplied by DTEMP_MUTIPLIED_RATIO.*/
	   tempTemperature = (int32_t)tempJunctionFilter -
	                   (pMCU - pRLed - pGLed - pBLed)/(int32_t)1000000*(int32_t)DTEMP_MULTIPLIED_RATIO;

	  // tempTemperature += 6*DTEMP_MULTIPLIED_RATIO;    //V03delete

	   if (initializationFlag == bfalse)
	     tempLedFilter = tempTemperature;
	   else
	   {
	 	  tempLedFilter = (tempLedFilter*(DTEMP_LED_FILTER_LEVEL-1) + tempTemperature)/DTEMP_LED_FILTER_LEVEL;
	   }

	   /*we update temperature every DTEMP_UPDATE_CYCLE_PERIOD MS*/
	   if ((DTickAfter(moduleClockGet(), updateTick + DTEMP_UPDATE_CYCLE_PERIOD)) || (initializationFlag == bfalse))
	   {
	     updateTick = moduleClockGet();
//			 if (updateTick > 60000)
//			 {
//					updateTick = 0;
//			 }
	 //    debugTemp = tempLedFilter;
	 //    debugJunction = tempJunctionFilter;
	     ledTemperature = tempLedFilter;
	     mcuTemperature = tempJunctionFilter;
	   }
	   initializationFlag = btrue;


}

/**
*@details   Get current LED temperature.
*
*@retval    current LED temperature.
*/
//extern uint16_t Debug;
int16_t moduleTempLedGet(void)
{
  
//  moduleTempUpdate();
//  return ledTemperature;			//delete
//	return Debug;
	return 250;
}

/**
*@details   Get current MCU temperature.
*
*@retval    current MCU temperature.
*/
int16_t moduleTempMcuGet(void)
{ 
//   return mcuTemperature;			//delete
//	return Debug;
	return 250;
}
int16_t moduleTempMcuGet_debug(void)
{ 
   return mcuTemperature;			//delete
//	return Debug;
//	return 250;
}