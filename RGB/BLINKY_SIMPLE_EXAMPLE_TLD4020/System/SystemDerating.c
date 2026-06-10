#include "SystemDerating.h"
#include "ModuleTemperature.h"

#define DTempRange    8

static const int16_t tempMcuRange[DTempRange]={
  -40 * DTEMP_MULTIPLIED_RATIO,        /*Level 0*/
   113* DTEMP_MULTIPLIED_RATIO,        /*Level 1*/
   117* DTEMP_MULTIPLIED_RATIO,        /*Level 2*/
   121* DTEMP_MULTIPLIED_RATIO,        /*Level 3*/
   125* DTEMP_MULTIPLIED_RATIO,        /*Level 4*/
   129* DTEMP_MULTIPLIED_RATIO,        /*Level 5*/
   133* DTEMP_MULTIPLIED_RATIO,        /*Level 6*/
   137* DTEMP_MULTIPLIED_RATIO,        /*Level 7*/
};

static const int16_t tempLedRange[DTempRange]={
  -40 * DTEMP_MULTIPLIED_RATIO,        /*Level 0*/
   97 * DTEMP_MULTIPLIED_RATIO,        /*Level 1*/
   101 * DTEMP_MULTIPLIED_RATIO,        /*Level 2*/
   105 * DTEMP_MULTIPLIED_RATIO,        /*Level 3*/
   109 * DTEMP_MULTIPLIED_RATIO,        /*Level 4*/
   113* DTEMP_MULTIPLIED_RATIO,        /*Level 5*/
   117* DTEMP_MULTIPLIED_RATIO,        /*Level 6*/
   121* DTEMP_MULTIPLIED_RATIO,        /*Level 7*/
};

// static const uint8_t intensityLevel[DTempRange+1]={
//   100,  /*Level 0*/
//   90,  /*Level 1*/
//   70,   /*Level 2*/
//   50,   /*Level 3*/
//   30,   /*Level 4*/
//   20,   /*Level 5*/
//   10,   /*Level 6*/
//   0,    /*Level 7*/
//   0,    /*Level 7*/
// };
static const uint8_t intensityLevel[DTempRange+1]={
  100,  /*Level 0*/
  100,  /*Level 1*/
  100,   /*Level 2*/
  100,   /*Level 3*/
  100,   /*Level 4*/
  100,   /*Level 5*/
  100,   /*Level 6*/
  100,    /*Level 7*/
  0,    /*Level 7*/
};
//static const uint8_t tempLevelShift[DTempRange] =
//{
//  0, 0,   /*Level 0*/
//  1,      /*Level 1*/
//  2,      /*Level 2*/
//  3,      /*Level 3*/
//  4,      /*Level 4*/
//  5,      /*Level 5*/
//  6,      /*Level 6*/
//  7,      /*Level 7*/
//  7,      /*Level 7*/
//};

/*record current temperature protection level*/
static uint8_t tempProtectLevel = 0;
static uint8_t currentLedLevel = 0, currentMcuLevel = 0;

typedef enum{
  EDeratingSourceLed  = 0x1,
  EDeratingSourceMcu  = 0x2,
}EDeratingSource;

static EDeratingSource __systemDeratingUpdateLevel(void)
{
  int16_t temperatureLed = moduleTempLedGet();
  int16_t temperatureMcu = moduleTempMcuGet();

  if (temperatureLed <= tempLedRange[0])
    currentLedLevel = 0;
  else if (temperatureLed > tempLedRange[DTempRange-1])
    currentLedLevel = DTempRange-1;
  else
  {
		int16_t i;
    /*confirm current Led level*/
    for(i = 0;i < (DTempRange-1); i++)
    {
      /*prevent level from jumping back*/
      if ((temperatureLed >= (tempLedRange[i] + 1*DTEMP_MULTIPLIED_RATIO)) &&
          (temperatureLed < (tempLedRange[i+1] - 1*DTEMP_MULTIPLIED_RATIO)))
      {
        currentLedLevel = i;
        break;
      }
    }
  }

  if (temperatureMcu <= tempMcuRange[0])
    currentMcuLevel = 0;
  else if (temperatureMcu > tempMcuRange[DTempRange-1])
    currentMcuLevel = DTempRange-1;
  else
  {
		int16_t i;
    /*confirm current Led level*/
    for(i = 0;i < (DTempRange-1); i++)
    {
      /*prevent level from jumping back*/
      if ((temperatureMcu >= (tempMcuRange[i] + 1*DTEMP_MULTIPLIED_RATIO)) &&
          (temperatureMcu < (tempMcuRange[i+1] - 1*DTEMP_MULTIPLIED_RATIO)))
      {
        currentMcuLevel = i;
        break;
      }
    }
  }

  return (currentLedLevel >= currentMcuLevel)?EDeratingSourceLed:EDeratingSourceMcu;
}

static uint16_t currentAdjust = 100<<DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL;
/**
*@details   dimming change intensity, when temperature too high.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
void systemTempDerating(SColorParams *inputColor)
{
  static uint16_t targetIntensityAdjust;
  EDeratingSource source = EDeratingSourceLed;
  uint8_t currentLevel = 0;
  const int16_t* tempRange = tempLedRange;
  int16_t temperature = 0;
  uint16_t delta = 0;
  int32_t tempA, tempB;

  source = __systemDeratingUpdateLevel();

  if (source == EDeratingSourceLed)
  {
    currentLevel = currentLedLevel;
    tempRange = tempLedRange;
    temperature = moduleTempLedGet();
  }

  if (source == EDeratingSourceMcu)
  {
    currentLevel = currentMcuLevel;
    tempRange = tempMcuRange;
    temperature = moduleTempMcuGet();
  }

  /*calculate intensity*/
  if (intensityLevel[currentLevel] != intensityLevel[currentLevel+1])
  {
    tempA = intensityLevel[currentLevel] << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL;
    tempB = intensityLevel[currentLevel+1] << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL;

    if (temperature >= tempLedRange[currentLevel])
    {
      tempB = (tempA - tempB) * (temperature - tempRange[currentLevel]) / (tempRange[currentLevel+1] - tempRange[currentLevel]);
      if (tempA >= tempB)
        tempA = tempA - tempB;
      else
        tempA = 0;
    }
    else
    {
      tempA = tempA + (tempA - tempB) * (tempRange[currentLevel] - temperature) / (tempRange[currentLevel+1] - tempRange[currentLevel]);
      if (tempA >= (100<<DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL))
        tempA = (100<<DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL);
    }
    targetIntensityAdjust = tempA;
  }
  else
    targetIntensityAdjust = (intensityLevel[currentLevel]<<DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL);

  /*do dimming.*/
  if (currentAdjust < targetIntensityAdjust)
  {
    delta = (targetIntensityAdjust - currentAdjust)>>(DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL + 1);
    currentAdjust = currentAdjust + delta;
  }
  else if (currentAdjust > targetIntensityAdjust)
  {
    delta = (currentAdjust - targetIntensityAdjust)>>(DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL + 1);
    currentAdjust = currentAdjust - delta;
  }

  if (delta == 0)
    currentAdjust = targetIntensityAdjust;

  inputColor->tempAdjust = currentAdjust;
  tempProtectLevel = currentLevel;
}

/**
*@details   get current TEMPSTATE level.
*
*@retval    TEMPSTATE level.
*/
uint8_t systemDeratingGetTempstate(void)
{
  return tempProtectLevel;
}
