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

/*timing macros*/
#define DMODE_ACTUATOR_BLINK_TIME_PERIOD    500 //500ms
#define DMODE_CRASH_BLINK_TIME_PERIOD       1000//500ms
#define DMODE_SHOWROOM_COLOR_CHANGE_PERIOD  250 //250ms

#define DDIMMING_TIME_SLICE_PERIOD          20//20ms


/*timing tick record*/
//static tick_t tickRecord = 0;

/*Crash and actuator use, blink flag*/
//static bool_t intensitylastStatus = bfalse;
/*Show-room mode and dimming use, new show room mode color flag*/
static bool_t newShowroomColor = bfalse;





static uint16_t currentY = 0, originY = 0;
static bool_t intensityDimmingFlag = bfalse;
/**
*@details   Controls intensity dimming. Intensity dimming only activate at normal mode.
*
*@params[in&out] currentTick current system tick.
*
*@params[in&out] mode current system mode.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
static void __systemIntensityDimming(tick_t currentTick, SColorParams *inputColor)
{
  uint32_t temp;
  static tick_t intensityTotaltick;
  static tick_t intensityStartTick, intensityTickCount;

  /*initialize some variant*/
  if (sysStatus.newLinSignal == btrue)
  {
    /*When switching current system mode, or mode is not normal, we stop dimming immediately.*/
    /*intensity dimming only works at normal mode. */
    if (1)
    {
      //uint32_t tempCalculate = sysLin.RampTime_LIN10 * (DDIMMING_TIME_SLICE_PERIOD);
      intensityStartTick = currentTick;
      intensityTickCount = 0;

      intensityTotaltick = sysLin.RampTime_LIN10 * (DDIMMING_TIME_SLICE_PERIOD);

      originY = currentY;
      intensityDimmingFlag = btrue;
    }
    else
      intensityDimmingFlag = bfalse;
  }

  if ((DTickAfter(currentTick, (intensityStartTick + intensityTotaltick))) ||
      (intensityTotaltick == 0))
  {
    intensityDimmingFlag = bfalse;
  }

  if (intensityDimmingFlag == btrue)
  {
    if (currentTick > intensityStartTick)
      intensityTickCount = currentTick - intensityStartTick;
    else
      intensityTickCount = DMAX_SYSTEM_TICK - (intensityStartTick - currentTick) + 1;

    /*When intensityDimmingtick ==0 , TEMP divide 0 will not be equals to zero, so the comparison symbol must be '<'.*/
    if (intensityTickCount < intensityTotaltick)
    {
      if (originY > inputColor->intensity)
      {
        temp = (uint32_t)(originY - inputColor->intensity)*(uint32_t)(intensityTotaltick - intensityTickCount);
        temp = temp/(uint32_t)intensityTotaltick;
        inputColor->intensity += (uint16_t)temp;
      }
      else
      {
        temp = (uint32_t)(inputColor->intensity - originY)*(uint32_t)(intensityTotaltick - intensityTickCount);
        temp = temp/(uint32_t)intensityTotaltick;
        inputColor->intensity -= (uint16_t)temp;
      }
    }
  }

  currentY = inputColor->intensity;
}

static uint16_t currentR = 0, currentG = 0, currentB = 0;
static uint16_t originR = 0, originG = 0, originB = 0;
static bool_t colorDimmingFlag = bfalse;
static bool_t intensityDimmingFlag2 = bfalse;
static uint8_t StepWY=0;
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

static void __systemColorChange(tick_t currentTick,  SColorParams *inputColor)
{
  uint32_t temp;
  static tick_t colorTotalTick;
  static tick_t colorStartTick, colorTickCount;

  static tick_t intensityTotaltick;
  static tick_t intensityStartTick, intensityTickCount;

  if (sysStatus.newLinSignal == btrue)
  {
    /*When switching current system mode, or mode is not normal nor show-room, we stop dimming immediately.*/
    /*color dimming only works at normal or show-room mode. */
    if (1)
    {
      colorStartTick = currentTick;
      colorTickCount = 0;
      colorTotalTick = sysLin.RampTime_LIN10 * (DDIMMING_TIME_SLICE_PERIOD);

      /*When switching current system mode, or mode is not normal, we stop dimming immediately.*/
	  /*intensity dimming only works at normal mode. */
	  intensityStartTick = currentTick;
	  intensityTickCount = 0;
	  StepWY = 0;
	  originY = currentY;
	  intensityTotaltick = sysLin.RampTime_LIN10 * (DDIMMING_TIME_SLICE_PERIOD)/2;
	  intensityDimmingFlag2 = btrue;

      /*Our customer requires that when intensity = 0 , color dimming will start from target color to target color.*/
      if (inputColor->intensity == 0)
      {
        originR = inputColor->colorR;
        originG = inputColor->colorG;
        originB = inputColor->colorB;
      }
      else
      {
        originR = currentR;
        originG = currentG;
        originB = currentB;
      }

      colorDimmingFlag = btrue;
    }
    else
      colorDimmingFlag = bfalse;
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

      if (originR > inputColor->colorR)
      {
        temp = (uint32_t)(originR - inputColor->colorR)*(uint32_t)(colorTotalTick - colorTickCount);
        temp = temp/(uint32_t)colorTotalTick;
        inputColor->colorR += (uint16_t)(originR - inputColor->colorR);
      }
      else
      {
        temp = (uint32_t)(inputColor->colorR - originR)*(uint32_t)(colorTotalTick - colorTickCount);
        temp = temp/(uint32_t)colorTotalTick;
        inputColor->colorR -= (uint16_t)(inputColor->colorR - originR);
      }

      if (originG > inputColor->colorG)
      {
        temp = (uint32_t)(originG - inputColor->colorG)*(uint32_t)(colorTotalTick - colorTickCount);
        temp = temp/(uint32_t)colorTotalTick;
        inputColor->colorG += (uint16_t)(originG - inputColor->colorG);
      }
      else
      {
        temp = (uint32_t)(inputColor->colorG - originG)*(uint32_t)(colorTotalTick - colorTickCount);
        temp = temp/(uint32_t)colorTotalTick;
        inputColor->colorG -= (uint16_t)(inputColor->colorG - originG);
      }

      if (originB > inputColor->colorB)
      {
        temp = (uint32_t)(originB - inputColor->colorB)*(uint32_t)(colorTotalTick - colorTickCount);
        temp = temp/(uint32_t)colorTotalTick;
        inputColor->colorB += (uint16_t)(originB - inputColor->colorB);
      }
      else
      {
        temp = (uint32_t)(inputColor->colorB - originB)*(uint32_t)(colorTotalTick - colorTickCount);
        temp = temp/(uint32_t)colorTotalTick;
        inputColor->colorB -= (uint16_t)(inputColor->colorB - originB);
      }
    }

    currentR = inputColor->colorR;
    currentG = inputColor->colorG;
    currentB = inputColor->colorB;


  if (intensityDimmingFlag2 == btrue)
     {
       if (currentTick > intensityStartTick)
         intensityTickCount = currentTick - intensityStartTick;
       else
         intensityTickCount = DMAX_SYSTEM_TICK - (intensityStartTick - currentTick) + 1;

       if (intensityTickCount < intensityTotaltick)
       {
         switch(StepWY)
         {
         case 0:  //0-->100
           temp = (uint32_t)(inputColor->intensity - originY)*(uint32_t)(intensityTotaltick - intensityTickCount);
           temp = temp/(uint32_t)intensityTotaltick;
           inputColor->intensity -= (uint16_t)temp;
           break;
         case 1:  //100-->0
           temp = (uint32_t)(originY - inputColor->intensity)*(uint32_t)(intensityTotaltick - intensityTickCount);
           temp = temp/(uint32_t)intensityTotaltick;
           inputColor->intensity += (uint16_t)temp;
           break;
         case 2:  //0-->100
           temp = (uint32_t)(inputColor->intensity - originY)*(uint32_t)(intensityTotaltick - intensityTickCount);
           temp = temp/(uint32_t)intensityTotaltick;
           inputColor->intensity -= (uint16_t)temp;
           break;
         case 3:
           break;
         }
       }
     }
     currentY = inputColor->intensity; 

     if ((DTickAfter(currentTick, (intensityStartTick + intensityTotaltick))) ||
         (intensityTotaltick == 0))
     {
       intensityStartTick = currentTick;
       originY = currentY;
       switch(StepWY)
       {
       case 0: //case0 complete
         StepWY = 1;
         sysLin.Brightness_LIN10 = 0;
         break;
       case 1:
         StepWY = 2;
         sysLin.Brightness_LIN10 = 100;
         break;
       case 2: //case2 complete
         StepWY = 3;
         Mode = 0;
         break;
       }
     }

}


static void __systemColorDimming(tick_t currentTick, SColorParams *inputColor)
{
  uint32_t temp;
  static tick_t colorTotalTick;
  static tick_t colorStartTick, colorTickCount;

  if ((sysStatus.newLinSignal == btrue) || (newShowroomColor == btrue))
  {
    /*When switching current system mode, or mode is not normal nor show-room, we stop dimming immediately.*/
    /*color dimming only works at normal or show-room mode. */
    if (1)
    {
      colorStartTick = currentTick;
      colorTickCount = 0;

      colorTotalTick = sysLin.RampTime_LIN10 * (DDIMMING_TIME_SLICE_PERIOD);

      /*Our customer requires that when intensity = 0 , color dimming will start from target color to target color.*/
      if (inputColor->intensity == 0)
      {
        originR = inputColor->colorR;
        originG = inputColor->colorG;
        originB = inputColor->colorB;
      }
      else
      {
        originR = currentR;
        originG = currentG;
        originB = currentB;
      }

      colorDimmingFlag = btrue;
    }
    else
      colorDimmingFlag = bfalse;
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

    if (originR > inputColor->colorR)
    {
      temp = (uint32_t)(originR - inputColor->colorR)*(uint32_t)(colorTotalTick - colorTickCount);
      temp = temp/(uint32_t)colorTotalTick;
      inputColor->colorR += (uint16_t)temp;
    }
    else
    {
      temp = (uint32_t)(inputColor->colorR - originR)*(uint32_t)(colorTotalTick - colorTickCount);
      temp = temp/(uint32_t)colorTotalTick;
      inputColor->colorR -= (uint16_t)temp;
    }

    if (originG > inputColor->colorG)
    {
      temp = (uint32_t)(originG - inputColor->colorG)*(uint32_t)(colorTotalTick - colorTickCount);
      temp = temp/(uint32_t)colorTotalTick;
      inputColor->colorG += (uint16_t)temp;
    }
    else
    {
      temp = (uint32_t)(inputColor->colorG - originG)*(uint32_t)(colorTotalTick - colorTickCount);
      temp = temp/(uint32_t)colorTotalTick;
      inputColor->colorG -= (uint16_t)temp;
    }

    if (originB > inputColor->colorB)
    {
      temp = (uint32_t)(originB - inputColor->colorB)*(uint32_t)(colorTotalTick - colorTickCount);
      temp = temp/(uint32_t)colorTotalTick;
      inputColor->colorB += (uint16_t)temp;
    }
    else
    {
      temp = (uint32_t)(inputColor->colorB - originB)*(uint32_t)(colorTotalTick - colorTickCount);
      temp = temp/(uint32_t)colorTotalTick;
      inputColor->colorB -= (uint16_t)temp;
    }
 }

  currentR = inputColor->colorR;
  currentG = inputColor->colorG;
  currentB = inputColor->colorB;
}


/**
*@details   Control module prepare to enter sleep.
*
*@retval    None.
*/
void systemControlSleep(void)
{
  currentR = 0;
  currentG = 0;
  currentB = 0;
  currentY = 0;
}

uint8_t Mode;
void systemControl(SColorParams *inputColor)
{
  tick_t currentTick = moduleClockGet();

  /*Sleep flag has set, preparing dimming*/
  if (sysStatus.sleepFlag == ESystemSleepPrepare)
  {
    sysStatus.sleepFlag = ESystemSleepProcessing;
    sysLin.Brightness_LIN10 = 0;

    if (sysStatus.errFlag == ESystemErrorNone)
      sysLin.RampTime_LIN10 = 25;
    else
      sysLin.RampTime_LIN10 = 0;

//    sysLin.BCM_RGB_Bewertung_Dimmzeit = Dimmzeit_relativ ;
    sysStatus.newLinSignal = btrue;
  }

  /*initialize some control signals*/
  /********************ygh add*********************/
  
  inputColor->factor = savedConfig.Lfactor[sysLin.VehicleTypeInfo];
  
//    if(sysLin.VehicleTypeInfo == 0)
//        inputColor->factor = savedConfig.Lfactor[4];
  inputColor->intensity = (sysLin.Brightness_LIN10>>1)<<DINTENSITY_ADJUST_SHIFT_LEVEL;
  inputColor->ledTemprature = moduleTempLedGet();


    inputColor->colorR = sysLin.RGB_R_LIN10;
    inputColor->colorG = sysLin.RGB_G_LIN10;
    inputColor->colorB = sysLin.RGB_B_LIN10;
    algoGetFixedRGB(inputColor);

  /*choose mode*/
  switch(Mode)
  {
    case ChangeClolor:
      __systemColorChange(currentTick, inputColor);
      //__systemColorChanging(currentTick, inputColor);
      break;
    default:
      break;
  }

  /*Do dimming*/
//  if ((sysStatus.sleepFlag == ESystemSleepProcessing) || (sysLin.RampTime_LIN10 == Showroommodus))
//  {
//    CDimmingTimeSlicePeriod = 250;
//  }
//  else
//  {
//    if (savedConfig.platform == 0)
//    {
//      CDimmingTimeSlicePeriod = (sysLin.BCM_RGB_Dimmzeit_Zeitbasis == 0)?50:250;
//    }
//    else
//    {
//      CDimmingTimeSlicePeriod = 250;
//    }
//  }

  __systemIntensityDimming(currentTick, inputColor);
  __systemColorDimming(currentTick,  inputColor);
  sysStatus.newLinSignal = bfalse;

  /*We use oldMode to notify that new mode has activated.*/
  //oldMode = 0;

  /*Do temperature protection*/
  systemTempDerating(inputColor);

  /*Dimming finished ,now enter sleep mode*/
  if ((sysStatus.sleepFlag == ESystemSleepProcessing)&&
      (inputColor->intensity == 0))
    sysStatus.sleepFlag = ESystemSleepSleep;
}
