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
//#define DDIMMING_TIME_SLICE_PERIOD          250//250ms
static uint32_t CDimmingTimeSlicePeriod = 250;

/*record last mode, initial value referring to LDF file*/
static uint8_t oldMode = keine_Sonderfunktion;
/*timing tick record*/
static tick_t tickRecord = 0;

/*Crash and actuator use, blink flag*/
static bool_t intensitylastStatus = bfalse;
/*Show-room mode and dimming use, new show room mode color flag*/
static bool_t newShowroomColor = bfalse;

#define DMAX_SHOWROOM_MODE_COLOR_AMOUNT     65

/*The color list below is from our customer*/
const uint8_t showroomColorSet[DMAX_SHOWROOM_MODE_COLOR_AMOUNT][3]={
  {0  , 0  , 255}, {0  , 33 , 255}, {0  , 54 , 255}, {0  , 81 , 255}, {1  , 112, 254},
  {0  , 147, 254}, {0  , 193, 255}, {0  , 255, 254}, {0  , 254, 197}, {0  , 255, 152},
  {0  , 255, 115}, {0  , 255, 82 }, {0  , 254, 55 }, {0  , 255, 0  }, {12 , 254, 0  },
  {36 , 255, 0  }, {78 , 255, 0  }, {121, 255, 0  }, {171, 255, 0  }, {228, 254, 0  },
  {254, 221, 0  }, {254, 163, 0  }, {254, 126, 0  }, {255, 95 , 0  }, {255, 72 , 0  },
  {255, 53 , 0  }, {255, 34 , 0  }, {255, 9  , 0  }, {254, 0  , 0  }, {254, 0  , 0  },
  {254, 0  , 0  }, {254, 0  , 0  }, {254, 0  , 0  }, {255, 0  , 8  }, {255, 0  , 17 },
  {255, 0  , 28 }, {255, 0  , 40 }, {255, 0  , 54 }, {254, 0  , 72 }, {255, 0  , 93 },
  {254, 0  , 117}, {255, 0  , 147}, {255, 0  , 147}, {255, 19 , 130}, {254, 38 , 111},
  {254, 63 , 92 }, {255, 88 , 72 }, {254, 112, 51 }, {255, 133, 33 }, {255, 157, 9  },
  {254, 191, 10 }, {254, 223, 30 }, {255, 247, 45 }, {228, 254, 64 }, {203, 255, 83 },
  {181, 255, 105}, {166, 255, 130}, {156, 254, 156}, {147, 255, 209}, {125, 234, 254},
  {87 , 185, 255}, {59 , 142, 254}, {38 , 112, 254}, {18 , 82 , 255}, {2  , 59 , 255}
};

/**
*@details   Controls color change every DMODE_SHOWROOM_COLOR_CHANGE_PERIOD period.total 65 colors.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
static void __systemModeShowroom(SColorParams *inputColor)
{
  static uint16_t colorIndex;
  /*initial show room mode*/
  if (oldMode != Showroommodus)
  {
    colorIndex = 0;
    tickRecord = moduleClockGet() + DMODE_SHOWROOM_COLOR_CHANGE_PERIOD;
    newShowroomColor = btrue;
  }
  /*if should change to next color*/
  if (DTickAfter(moduleClockGet(), tickRecord))
  {
    colorIndex++;
    if (colorIndex >= DMAX_SHOWROOM_MODE_COLOR_AMOUNT)
      colorIndex = 0;

    tickRecord += DMODE_SHOWROOM_COLOR_CHANGE_PERIOD;
    newShowroomColor = btrue;
  }

  inputColor->colorR = showroomColorSet[colorIndex][0];
  inputColor->colorG = showroomColorSet[colorIndex][1];
  inputColor->colorB = showroomColorSet[colorIndex][2];

  inputColor->colorR = inputColor->colorR<<8;
  inputColor->colorG = inputColor->colorG<<8;
  inputColor->colorB = inputColor->colorB<<8;
}

/**
*@details   Controls color change every DMODE_SHOWROOM_COLOR_CHANGE_PERIOD period.total 65 colors.
*
*@params[in] mode controls if it should initialization.
*
*@params[in] period input and output Color signal @SColorParams
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
static void __doBlink(uint8_t mode, tick_t period, SColorParams *inputColor)
{
  /*Initialize timing*/
  if (oldMode != mode)
  {
    tickRecord = moduleClockGet() + period;
    intensitylastStatus = btrue;
  }

  /*time to change intensity*/
  if (DTickAfter(moduleClockGet(), tickRecord))
  {
    if (intensitylastStatus == btrue)
      intensitylastStatus = bfalse;
    else
      intensitylastStatus = btrue;
    tickRecord += period;
  }

  if (intensitylastStatus == bfalse)
    inputColor->intensity = 0;
}

/**
*@details   Controls crash mode 1s on and 1s off.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
static void __systemModeCrash(SColorParams *inputColor)
{
  __doBlink(Crash, DMODE_ACTUATOR_BLINK_TIME_PERIOD, inputColor);
}

/**
*@details   Controls actuator mode 0.5s on and 0.5s off.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
static void __systemModeActuator(SColorParams *inputColor)
{
  __doBlink(Stellgliedtest, DMODE_CRASH_BLINK_TIME_PERIOD, inputColor);
}

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
static void __systemIntensityDimming(tick_t currentTick, uint8_t mode, SColorParams *inputColor)
{
  uint32_t temp;
  static tick_t intensityTotaltick;
  static tick_t intensityStartTick, intensityTickCount;

  /*initialize some variant*/
  if (sysStatus.newLinSignal == btrue)
  {
    /*When switching current system mode, or mode is not normal, we stop dimming immediately.*/
    /*intensity dimming only works at normal mode. */
    if ((mode == oldMode) &&
        (mode == keine_Sonderfunktion))
    {
      intensityStartTick = currentTick;
      intensityTickCount = 0;

      if (sysLin.BCM_RGB_Bewertung_Dimmzeit == Dimmzeit_absolut)
      {
        uint32_t tempCalculate = sysLin.BCM_RGB_Dimmrampe * (CDimmingTimeSlicePeriod);

        if (currentY > inputColor->intensity)
          tempCalculate = tempCalculate*(currentY - inputColor->intensity);
        else
          tempCalculate = tempCalculate*(inputColor->intensity - currentY);

        intensityTotaltick = (tick_t)(tempCalculate/(uint32_t)(100<<DINTENSITY_ADJUST_SHIFT_LEVEL));
      }
      else
        intensityTotaltick = sysLin.BCM_RGB_Dimmrampe * (CDimmingTimeSlicePeriod);

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
static void __systemColorDimming(tick_t currentTick, uint8_t mode, SColorParams *inputColor)
{
  uint32_t temp;
  static tick_t colorTotalTick;
  static tick_t colorStartTick, colorTickCount;

  if ((sysStatus.newLinSignal == btrue) || (newShowroomColor == btrue))
  {
    /*When switching current system mode, or mode is not normal nor show-room, we stop dimming immediately.*/
    /*color dimming only works at normal or show-room mode. */
    if ((mode == oldMode) &&
        ((mode == keine_Sonderfunktion) || (mode == Showroommodus)))
    {
      colorStartTick = currentTick;
      colorTickCount = 0;

      colorTotalTick = sysLin.BCM_RGB_Dimmrampe * (CDimmingTimeSlicePeriod);

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
  oldMode = keine_Sonderfunktion;
}

void systemControl(SColorParams *inputColor)
{
  tick_t currentTick = moduleClockGet();

  /*Sleep flag has set, preparing dimming*/
  if (sysStatus.sleepFlag == ESystemSleepPrepare)
  {
    sysStatus.sleepFlag = ESystemSleepProcessing;
    sysLin.BCM_RGB_Intensitaet = 0;

    if (sysStatus.errFlag == ESystemErrorNone)
      sysLin.BCM_RGB_Dimmrampe = 2;
    else
      sysLin.BCM_RGB_Dimmrampe = 0;

    sysLin.BCM_RGB_Bewertung_Dimmzeit = Dimmzeit_relativ ;
    sysStatus.newLinSignal = btrue;
  }

  /*initialize some control signals*/
  inputColor->factor = savedConfig.factor;
  inputColor->intensity = sysLin.BCM_RGB_Intensitaet<<DINTENSITY_ADJUST_SHIFT_LEVEL;
  inputColor->ledTemprature = moduleTempLedGet();

  /*show room mode do not need color signals from LIN.*/
  if (sysLin.BCM_RGB_Sonderfunktion != Showroommodus)
  {
    inputColor->colorR =  sysLin.BCM_RGB_Rot;
    inputColor->colorG =  sysLin.BCM_RGB_Gruen;
    inputColor->colorB =  sysLin.BCM_RGB_Blau;
    /*fix rgb signals*/
    algoGetFixedRGB(inputColor);
  }

  /*choose mode*/
  switch(sysLin.BCM_RGB_Sonderfunktion)
  {
    case Stellgliedtest://actuator test mode
      __systemModeActuator(inputColor);
      break;
    case Showroommodus:
      __systemModeShowroom(inputColor);
      break;
    case Crash:
      __systemModeCrash(inputColor);
      break;
    case keine_Sonderfunktion:
    default:
      break;
  }

  /*Do dimming*/
  //CDimmingTimeSlicePeriod = (sysLin.BCM_RGB_Dimmzeit_Zeitbasis == 0)?50:250;
  CDimmingTimeSlicePeriod = 250;
  __systemIntensityDimming(currentTick, sysLin.BCM_RGB_Sonderfunktion, inputColor);
  __systemColorDimming(currentTick, sysLin.BCM_RGB_Sonderfunktion, inputColor);
  sysStatus.newLinSignal = bfalse;
  newShowroomColor = bfalse;

  /*We use oldMode to notify that new mode has activated.*/
  oldMode = sysLin.BCM_RGB_Sonderfunktion;

  /*Do temperature protection*/
  systemTempDerating(inputColor);

  /*Dimming finished ,now enter sleep mode*/
  if ((sysStatus.sleepFlag == ESystemSleepProcessing)&&
      (inputColor->intensity == 0))
    sysStatus.sleepFlag = ESystemSleepSleep;
}
