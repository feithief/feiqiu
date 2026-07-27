/*
 * system_main.c
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#include "Algorithm.h"
#include "systemControl.h"
#include "ModuleAdc.h"
#include "ModuleComm.h"
#include "ModuleFlash.h"
#include "ModulePwm.h"
#include "ModuleWatchdog.h"
#include "ModuleTemprature.h"
#include "SystemMain.h"
#include "SystemStatus.h"
#include <atomic.h>

SColorParams currentColorSets;

void system_Init(void)
{
  uint16_t index;

  /*Initialize watch dog*/
  moduleWDTInit();
  moduleWDTStart();
  moduleWDTFeed();
  /*Initialize flash*/
  moduleFlashInit();
  /*Initialize ADC*/
  moduleAdcInit();
  /*Initialize PWM*/
  modulePwmInit();
  /*Initialize communication*/
  moduleCommInit();
  /*Initialize Temprature*/
  moduleTempratureInit();

  /*Initialize System Status*/
  systemStatusInit();
  /*Initialize System Control*/
  systemControlInit();

  /*Initialize Algorithm*/
  for (index = 0; index < RGBLEDMAX; index++)
  {
    algoUpdateRedCIE(index, savedConfig.redx[index], savedConfig.redy[index], savedConfig.redY[index]);
    algoUpdateGreenCIE(index, savedConfig.greenx[index], savedConfig.greeny[index], savedConfig.greenY[index]);
    algoUpdateBlueCIE(index, savedConfig.bluex[index], savedConfig.bluey[index], savedConfig.blueY[index]);
  }

  for (index = 0; index < RGBLEDMAX; index++)
  {
    currentColorSets.intensity[index] = 0;
    currentColorSets.colorU[index] = 0;
    currentColorSets.colorV[index] = 0;
    currentColorSets.ledTemprature[index] = 0;
    currentColorSets.tempAdjust[index] = 0;
  }

}

SPWMParams ouputPwm;
void system_Main_Loop(void)
{
  uint16_t ledIndex;
  //SLEDPwmSet pwmParam;
  SCommMessage message;

  moduleWDTFeed();

  systemStatusUpdate();

  if (flashFlag == EFlashUpdateProcessing)
  {
    ENTER_SECTION(ATOMIC_KEEP_MODE);
    flashFlag = EFlashUpdateFree;
    EXIT_SECTION();

    /*update LED R/G/B light attributes*/
    for (ledIndex = 0; ledIndex < RGBLEDMAX; ledIndex++)
    {
      algoUpdateRedCIE(ledIndex, savedConfig.redx[ledIndex], savedConfig.redy[ledIndex], savedConfig.redY[ledIndex]);
      algoUpdateGreenCIE(ledIndex, savedConfig.greenx[ledIndex], savedConfig.greeny[ledIndex], savedConfig.greenY[ledIndex]);
      algoUpdateBlueCIE(ledIndex, savedConfig.bluex[ledIndex], savedConfig.bluey[ledIndex], savedConfig.blueY[ledIndex]);
    }
    moduleFlashSave();
  }
  else
  {
    /* check if new Melibu data has been received */
    if ((moduleCommFlagUpdated() == btrue) && (moduleCommGetData(&message) == btrue))
    {
      moduleCommFlagClear();

      systemProtocolControl(&message, &currentColorSets);
    }

//	  systemProtocolControl(&message, &currentColorSets);

    for (ledIndex = 0; ledIndex < RGBLEDMAX; ledIndex++)
    {
      currentColorSets.ledTemprature[ledIndex] = moduleTempratureGetLed(ledIndex);
      currentColorSets.tempAdjust[ledIndex] = 100 << DINTENSITY_TEMP_ADJUST_SHIFT_LEVEL;
    }

    algoGetPWMOutput(&currentColorSets, &ouputPwm);

    modulePwmSet(&ouputPwm);
  }
}
