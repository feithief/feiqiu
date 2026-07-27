/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    SystemLinDiag.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file manage system function control.
  *Others:      None.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
                     2018.1.18
                     Separate color and intensity dimming from each other.
                     Delete initial mode.
**********************************************************************************/
#include "SystemControl.h"
#include "SystemStatus.h"
#include "SystemLinComm.h"
#include "SystemMain.h"
#include "ModuleTemperature.h"
#include "ModuleFlash.h"
#include "ModuleClock.h"
#include "SystemDerating.h"
#include "string.h"
#include "Algorithm.h"
#ifndef HANDWRITE_INTERPOLATE
#include "Dimming_Calculate.h"
#endif
/*timing macros*/

#define PWMFULL (32000)
static const uint32_t CDimmingTimeSlicePeriod = 50;

IntensityDimStatus intensityDimStatus = {
	.dimStatus = X00_DIMIDLE,
	.currentDimPriority = X00_DIMPRIORITY_IDLE,
	.actualIntensity = 0,
	.x_Diff_ms = 0,
	.lastActualIntensity = 0,
	.currentX = 0,
	.y_Diff = 0,
	.lastLoopSysTick = 0,
	.dimmingStateMachineInit = bfalse
};

void systemControl_reInitDimmingStateMachine(void)
{
	intensityDimStatus.dimStatus = X00_DIMIDLE;
	intensityDimStatus.currentDimPriority = X00_DIMPRIORITY_IDLE;
	intensityDimStatus.actualIntensity = 0;
	intensityDimStatus.x_Diff_ms = 0;
	intensityDimStatus.lastActualIntensity = 0;
	intensityDimStatus.currentX = 0;
	intensityDimStatus.y_Diff = 0;
	intensityDimStatus.lastLoopSysTick = moduleClockGet();
}

#define INTENSITYDIMMING_BITSHIFT 14u
static void IntensityDimminggetDimmingSpeed(SColorParams* inputColor, uint8_t dimRate)
{
	uint32_t totalDimTime = 0;
	if((savedConfig.brightness_factor & 0x80) == 0 || sysStatus.sleepFlag == ESystemSleepProcessing)
	{
		totalDimTime = CDimmingTimeSlicePeriod * dimRate;
	}
	else
	{
		/*speed dimming*/
		uint8_t diff = GetBrightnessDiffForSpeedBasedDimming();
		totalDimTime = (CDimmingTimeSlicePeriod * (uint32_t)dimRate * (uint32_t)diff) / (uint32_t)100u;
	}
	totalDimTime = totalDimTime > 100 ? totalDimTime : 100;
	uint32_t targetY = inputColor->intensity << INTENSITYDIMMING_BITSHIFT;
	uint32_t actualY = (uint32_t)intensityDimStatus.actualIntensity << INTENSITYDIMMING_BITSHIFT;
	if (actualY != targetY)
	{
		/*dimming is required*/
		intensityDimStatus.lastActualIntensity = actualY;
		intensityDimStatus.x_Diff_ms = ((uint32_t)4000 << INTENSITYDIMMING_BITSHIFT) / totalDimTime;

		if (actualY > targetY)
		{
			intensityDimStatus.dimStatus = X02_DIMIDOWN;
			intensityDimStatus.y_Diff = (actualY - targetY) / (uint32_t)4000u;
			intensityDimStatus.currentX = (uint32_t)4000 << INTENSITYDIMMING_BITSHIFT;
		}
		else
		{
			intensityDimStatus.dimStatus = X01_DIMUP;
			intensityDimStatus.y_Diff = (targetY - actualY) / (uint32_t)4000u;
			intensityDimStatus.currentX = 0u;
		}
	}
	else {
		/*dimming is not required*/
		intensityDimStatus.dimStatus = X00_DIMIDLE;
		intensityDimStatus.x_Diff_ms = 0;
		intensityDimStatus.y_Diff = 0;
	}
//	intensityDimStatus.lastLoopSysTick = moduleClockGet();
}

#ifdef HANDWRITE_INTERPOLATE
static uint32_t dataPoint[11][2] = { {0 * 40, 0 * 40}, {10 * 40, (3 * 40)/10}, {20 * 40, (17 * 40)/10}, {30 * 40, 5 * 40}, {40 * 40, 10 * 40}, {50 * 40, 18 * 40}, {60 * 40, 28 * 40}, {70 * 40, 41 * 40}, {80 * 40, 57 * 40}, {90 * 40, 77 * 40}, {100 * 40, 100 * 40} };
#if 1
static void PonintcCalibration(void)
{
	 dataPoint[1][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x1);
	 dataPoint[1][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y1h<<8) + (uint16_t)savedConfig.y1l));

	 dataPoint[2][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x2);
	 dataPoint[2][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y2h<<8) + (uint16_t)savedConfig.y2l));

	 dataPoint[3][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x3);
	 dataPoint[3][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y3h<<8) + (uint16_t)savedConfig.y3l));

	 dataPoint[4][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x4);
	 dataPoint[4][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y4h<<8) + (uint16_t)savedConfig.y4l));

	 dataPoint[5][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x5);
	 dataPoint[5][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y5h<<8) + (uint16_t)savedConfig.y5l));

	 dataPoint[6][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x6);
	 dataPoint[6][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y6h<<8) + (uint16_t)savedConfig.y6l));

	 dataPoint[7][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x7);
	 dataPoint[7][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y7h<<8) + (uint16_t)savedConfig.y7l));

	 dataPoint[8][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x8);
	 dataPoint[8][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y8h<<8) + (uint16_t)savedConfig.y8l));

	 dataPoint[9][0] = (uint32_t)((uint16_t)40 * (uint16_t)savedConfig.x9);
	 dataPoint[9][1] = (uint32_t)((uint16_t)4 * (((uint16_t)savedConfig.y9h<<8) + (uint16_t)savedConfig.y9l));
}
#endif
static uint32_t unlinearize(uint32_t linearBrightness)
 {
  int8_t section = 0;
  uint32_t interpolateY;
  PonintcCalibration();
  while (dataPoint[section][0] < linearBrightness)
  {
   section++;
  }
  if (section <= 0)
  {
   section = 1;
  }
  interpolateY = dataPoint[section - 1][1] + (((linearBrightness - dataPoint[section - 1][0]) * 10000) / (dataPoint[section][0] - dataPoint[section - 1][0]) * (dataPoint[section][1] - dataPoint[section - 1][1])) / 10000;
  return interpolateY;
 }
#endif
static void IntensityDimmingPropositionToFLUX(uint32_t* p_proposedCurrentIntensity)
{
	uint32_t interpolateY;
	uint8_t currentState = intensityDimStatus.dimStatus;
	if (currentState == X01_DIMUP)
	{
		/*dim up*/
		if (*p_proposedCurrentIntensity >= (uint32_t)4000 << INTENSITYDIMMING_BITSHIFT)
		{
			/*if proposed is >= than barrier, dimming finished*/
			*p_proposedCurrentIntensity = (uint32_t)4000 << INTENSITYDIMMING_BITSHIFT;
			intensityDimStatus.dimStatus = X00_DIMIDLE;
			intensityDimStatus.x_Diff_ms = 0;
			intensityDimStatus.currentDimPriority = X00_DIMPRIORITY_IDLE;
			*p_proposedCurrentIntensity = (uint32_t)4000 << INTENSITYDIMMING_BITSHIFT;
		}
	}
	else
	{
		/*dim down*/
		/*if proposed is <= than barrier, dimming finished*/
		if (*p_proposedCurrentIntensity == 0u)
		{
			intensityDimStatus.dimStatus = X00_DIMIDLE;
			intensityDimStatus.x_Diff_ms = 0u;
			intensityDimStatus.currentDimPriority = X00_DIMPRIORITY_IDLE;
			*p_proposedCurrentIntensity = 0u;
		}
	}
	intensityDimStatus.currentX = *p_proposedCurrentIntensity;
	//inputColor->intensity = LINtoFLUX((uint16_t)(*p_proposedCurrentIntensity / 10000));
#ifdef HANDWRITE_INTERPOLATE
	interpolateY = unlinearize(*p_proposedCurrentIntensity >> INTENSITYDIMMING_BITSHIFT);
#else
	Dimming_Calculate_Fun((uint16_t)(*p_proposedCurrentIntensity / 10000), &interpolateY);
#endif
	//interpolateY = (uint16_t)(*p_proposedCurrentIntensity / 10000);
	if (currentState == X01_DIMUP)
	{
		intensityDimStatus.actualIntensity = (uint16_t)((intensityDimStatus.lastActualIntensity + interpolateY * intensityDimStatus.y_Diff) >> INTENSITYDIMMING_BITSHIFT);
	}
	else
	{
		intensityDimStatus.actualIntensity = (uint16_t)((intensityDimStatus.lastActualIntensity - (4000 - interpolateY) * intensityDimStatus.y_Diff) >> INTENSITYDIMMING_BITSHIFT);
	}
}

///*warn*/
//extern uint16_t Debug_0100[10];
//uint32_t *a = &Debug_0100[0];
void __systemIntensityDimming(tick_t currentTick, SColorParams* inputColor, uint8_t dimRate)
{
	tick_t lastLoopSysTick;
	tick_t intensityTickCount;
	uint32_t proposedCurrentIntensity;

	if(intensityDimStatus.dimmingStateMachineInit == bfalse)
	{
		intensityDimStatus.dimmingStateMachineInit = btrue;
		systemControl_reInitDimmingStateMachine();
	}
	lastLoopSysTick = intensityDimStatus.lastLoopSysTick;
		if(sysStatus.sleepFlag == ESystemSleepProcessing && X03_DIMPRIORITY_SLEEP > intensityDimStatus.currentDimPriority)
		{
			/*No other dimming can stop sleep dimming*/
			inputColor->intensity = 0;
			IntensityDimminggetDimmingSpeed(inputColor, dimRate);
			intensityDimStatus.currentDimPriority = X03_DIMPRIORITY_SLEEP;
		}
		else if (sysStatus.newLinSignal == btrue && (X02_DIMPRIORITY_NORMAL >= intensityDimStatus.currentDimPriority || X01_DIMPRIORITY_WAKEUP > intensityDimStatus.currentDimPriority))
		{
			IntensityDimminggetDimmingSpeed(inputColor, dimRate);
			if(sysLin.FristLin == FristDiminging)
			{
				intensityDimStatus.currentDimPriority = X01_DIMPRIORITY_WAKEUP;
			}
			else
			{
				intensityDimStatus.currentDimPriority = X02_DIMPRIORITY_NORMAL;
			}
			/*recalculate dim status if new LIN signal is received*/
			/*Normal and sleep dimming can stop normal dimming*/
		}
		if (intensityDimStatus.dimStatus != X00_DIMIDLE)
		{
			/*dimming*/
			if (currentTick > lastLoopSysTick)
			{
				/*sysTick not overflow*/
				intensityTickCount = currentTick - lastLoopSysTick;
			}
			else
			{
				/*sysTick overflow*/
				intensityTickCount = DMAX_SYSTEM_TICK - (lastLoopSysTick - currentTick) + 1;
			}
//			a[0] = intensityTickCount;
//			a[1] = lastLoopSysTick;
//			a[2] = currentTick;
			if (intensityDimStatus.dimStatus == X01_DIMUP)
			{
				/*dim direction is UP*/
				proposedCurrentIntensity = (uint32_t)intensityTickCount * intensityDimStatus.x_Diff_ms + intensityDimStatus.currentX;

			}
			else
			{
				/*dim direction is DOWN*/
				proposedCurrentIntensity = intensityDimStatus.currentX - (uint32_t)intensityTickCount * intensityDimStatus.x_Diff_ms;
				if (proposedCurrentIntensity > intensityDimStatus.currentX)
				{
					/*below zero situation, error, reset intensity to 0*/
					proposedCurrentIntensity = 0;
				}
			}
//			a[3] = proposedCurrentIntensity;
			IntensityDimmingPropositionToFLUX(&proposedCurrentIntensity);
		}
		intensityDimStatus.lastLoopSysTick = currentTick;
		//inputColor->intensity = (uint16_t)(((uint32_t)((intensityDimStatus.actualIntensity * (uint32_t)Helligkeitsfaktor)) / (uint32_t)100) >> 8);
		inputColor->intensity = (uint32_t)intensityDimStatus.actualIntensity;
}

static uint16_t currentU = 0, currentV = 0;
static uint16_t originU = 0, originV = 0;
static uint32_t Lastintensity = 1;
static tick_t colorTotalTick;
static tick_t colorStartTick, colorTickCount;
 bool_t colorDimmingFlag = bfalse;
/**
*@details   Controls color dimming. Color dimming only activate at show-room and normal mode.
*
*@params[in&out] currentTick current system tick.
*
*@params[in&out] mode current system mode.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/

 void reinitColorDimming(void)
 {
	 currentU = 0;
	 currentV = 0;
	 originU = 0;
	 originV = 0;
	 Lastintensity = 1;
	 colorDimmingFlag = bfalse;
 }

 static void __systemColorDimming(tick_t currentTick, SColorParams *inputColor, uint8_t dimRate)
 {

   uint32_t temp;
   if ((sysStatus.newLinSignal == btrue))
   {
     /*When switching current system mode, or mode is not normal nor show-room, we stop dimming immediately.*/
     /*color dimming only works at normal or show-room mode. */

       colorStartTick = currentTick;
       colorTickCount = 0;
//       colorTotalTick = dimRate * (CDimmingTimeSlicePeriod);
		if((savedConfig.brightness_factor & 0x80) == 0 || sysStatus.sleepFlag == ESystemSleepProcessing)
		{
			colorTotalTick = CDimmingTimeSlicePeriod * dimRate;
		}
		else
		{
			/*speed dimming*/
			uint8_t diff = GetBrightnessDiffForSpeedBasedDimming();
			if(diff == 0)
			{
				colorTotalTick = CDimmingTimeSlicePeriod * dimRate;
			}
			else
			{
				colorTotalTick = (CDimmingTimeSlicePeriod * (uint32_t)dimRate * (uint32_t)diff) / (uint32_t)100u;
			}
		}
		colorTotalTick = colorTotalTick > 100 ? colorTotalTick : 100;

       /*Our customer requires that when intensity = 0 , color dimming will start from target color to target color.*/
//       if (inputColor->intensity == 0)
//       {
//
//         originU = inputColor->colorU;
//         originV = inputColor->colorV;
//       }
       if(Lastintensity == 0 )
       {
     	  originU = inputColor->colorU;
     	  originV = inputColor->colorV;
       }
       else
       {
         originU = currentU;
         originV = currentV;
       }
       colorDimmingFlag = btrue;
   }

   if ((DTickAfter(currentTick, (colorStartTick + colorTotalTick))) ||
       (colorTotalTick == 0))
   {
     colorDimmingFlag = bfalse;
   }

   if (colorDimmingFlag == btrue)
   {
     if (currentTick > colorStartTick)
       colorTickCount = currentTick - colorStartTick;
     else
       colorTickCount = DMAX_SYSTEM_TICK - (colorStartTick - currentTick) + 1;

     if (originU > inputColor->colorU)
     {
       temp = (uint32_t)(originU - inputColor->colorU)*(uint32_t)(colorTotalTick - colorTickCount);
       temp = temp/(uint32_t)colorTotalTick;
       inputColor->colorU += (uint16_t)temp;
     }
     else
     {
       temp = (uint32_t)(inputColor->colorU - originU)*(uint32_t)(colorTotalTick - colorTickCount);
       temp = temp/(uint32_t)colorTotalTick;
       inputColor->colorU -= (uint16_t)temp;
     }
     if (originV > inputColor->colorV)
     {
       temp = (uint32_t)(originV - inputColor->colorV)*(uint32_t)(colorTotalTick - colorTickCount);
       temp = temp/(uint32_t)colorTotalTick;
       inputColor->colorV += (uint16_t)temp;
     }
     else
     {
       temp = (uint32_t)(inputColor->colorV - originV)*(uint32_t)(colorTotalTick - colorTickCount);
       temp = temp/(uint32_t)colorTotalTick;
       inputColor->colorV -= (uint16_t)temp;
     }
   }
   currentU = inputColor->colorU;
   currentV = inputColor->colorV;
   Lastintensity = inputColor->intensity;
 }
/**
*@details   Control module prepare to enter sleep.
*
*@retval    None.
*/
void systemControlSleep(void)
{

}

/**
*@details   Control logic of LED lights.
*
*@retval    None.
*/

void systemControl(SColorParams *inputColor)
{
	  tick_t currentTick = moduleClockGet();
	  uint8_t realDimRate = 0;
	  uint8_t realIntensity = 0;

	  /*Sleep flag has set, preparing dimming*/
	  realDimRate = sysLin.RGB.BCM_RGB_Dimmrampe < 2 ? 2 : sysLin.RGB.BCM_RGB_Dimmrampe;
	  realIntensity = sysLin.RGB.BCM_RGB_Intensitaet;
	  if(sysStatus.newLinSignal == btrue)
	  {
		  /*now startup dimming is processed as normal dimming*/
		  if(sysLin.FristLin == FristDimingstart)
		  {
			  sysLin.FristLin = FristDiminging;
//			  realDimRate = 4;
		  }
	  }
	  if (sysStatus.sleepFlag == ESystemSleepPrepare)
	  {
	      sysStatus.sleepFlag = ESystemSleepProcessing;
	      realIntensity = 0;

	      if (sysStatus.errFlag == ESystemErrorNone)
	    	  realDimRate = 10;//dimming time 50ms*10
	      else
	    	  realDimRate = 2;

	      sysStatus.newLinSignal = btrue;
	  }
//  	  if(sysLin.FristLin == FristDimingstart)
//  	  {
////  		realIntensity = 0;
////  		  sysLin.BCM_RGB_Dimmrampe = 4;
//  		  sysLin.FristLin = FristDiminging;
//  		  sysStatus.newLinSignal = btrue;
//  	  }
//  	  else if(sysLin.FristLin == FristDiminging)
//  	  {
////  		  sysLin.BCM_RGB_Dimmrampe = 4;
//  	  }
  	  WitheBalanceTempInit();


	  inputColor->ledTemprature = moduleTempLedGet();
//		if (sysLin.RGB.BCM_RGB_Color_u != 0 && sysLin.RGB.BCM_RGB_Color_v != 0)
		{
			inputColor->colorU = 	sysLin.RGB.BCM_RGB_Color_u*5;
			inputColor->colorV = sysLin.RGB.BCM_RGB_Color_v*5;
		}
	  inputColor->intensity = (uint16_t)((uint32_t)realIntensity)*DINTENSITY_ADJUST_SHIFT;
	  if(inputColor->intensity > (uint32_t)4000u)
	  {
		  inputColor->intensity = (uint32_t)4000u;
	  }
	  if(sysLin.FristLin != FristDimingstart && sysLin.FristLin != FristDiminging )
	  {
				__systemColorDimming(currentTick,inputColor, realDimRate);
	  }
	  __systemIntensityDimming(currentTick, inputColor, realDimRate);
	  sysStatus.newLinSignal = bfalse;
	  /*Do temperature protection*/
	  systemTempDerating(inputColor);
	  /*Dimming finished ,now enter sleep mode*/
	  if ((sysStatus.sleepFlag == ESystemSleepProcessing)&& (inputColor->intensity == 0))
	  {
	  	    sysStatus.sleepFlag = ESystemSleepSleep;
	  }
	  else if((sysLin.FristLin == FristDiminging)
	  	        &&(X00_DIMIDLE == intensityDimStatus.dimStatus)
	            &&(inputColor->intensity!= 0))
	  {

	  	    sysLin.FristLin = FritstDimingStop;
	  }
}

