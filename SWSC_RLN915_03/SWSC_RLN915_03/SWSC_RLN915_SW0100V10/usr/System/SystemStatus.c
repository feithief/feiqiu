/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    SystemStatus.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file maintains system status.
  *Others:      None.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "SystemStatus.h"
#include "ModuleTemperature.h"
#include "ModuleBattery.h"
#include "ModuleLed.h"
#include "ModuleWatchdog.h"


/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/*******************************************************************************
*@brief This variant shows system status.
*@Note  It's dangerous to make this external, only for smaller code size.
*******************************************************************************/
volatile SSystemStatus sysStatus;

/*******************************************************************************
*@brief This variant shows current LIN control signals.
*@Note  It's dangerous to make this external, only for smaller code size.
*******************************************************************************/
SLinSignals sysLin;


/* ============================================================================ */
/* --------------------------- Application Functions--------------------------- */
/* ============================================================================ */
/*******************************************************************************
*@details   Initialize status management.
*
*@retval    None.
*******************************************************************************/
void systemStatusInit(void)
{
#ifdef DLIN_COMM_NO_SLEEP_WHITE_LIGHT
  sysLin.CtrlList_LIN10 = 0;
  sysLin.Brightness_LIN10 = 200;
  sysLin.RGB_R_LIN10 = 175;
  sysLin.RGB_G_LIN10 = 255;
  sysLin.RGB_B_LIN10 = 64;
  sysLin.RampTime_LIN10 = 0;
  sysLin.BCM_RGB_Sonderfunktion = 0;
  sysLin.unused52_0 = 0;
  sysLin.unused58_0 = 0;
  sysStatus.errFlag = ESystemErrorNone;
  sysStatus.newLinSignal = btrue;
  sysStatus.sleepFlag = ESystemSleepAwake;
#else
  sysLin.CtrlList_LIN10 = 0;
  sysLin.Brightness_LIN10 = 0;
  sysLin.RGB_R_LIN10 = 0;
  sysLin.RGB_G_LIN10 = 0;
  sysLin.RGB_B_LIN10 = 0;
  sysLin.RampTime_LIN10 = 0;
  sysLin.VehicleTypeInfo = 0;
  sysStatus.errFlag = ESystemErrorNone;
  sysStatus.newLinSignal = bfalse;
  sysStatus.sleepFlag = ESystemSleepAwake;
#endif
}

/*******************************************************************************
*@details   Update system status(Battery, temperature, LIN, LED&System status...).
*
*@retval    None.
*******************************************************************************/
void systemStatusUpdate(void)
{
  /*Process Battery*/

  if (moduleBatUpdateStatus() != EBatStatusNormal)
    sysStatus.errFlag |= ESystemErrorBatError;
  else
    sysStatus.errFlag &= ~ESystemErrorBatError;

  
  /*Process Led open short detection*/
  moduleWatchdogFeed();
  moduleLedUpdateStatus();

  /*Process temperature*/
  moduleTempUpdate();

  /*if new LIN signal received, try to power on and detect led for once*/
  if ((sysStatus.errFlag & ESystemErrorLedError) &&
      (sysStatus.newLinSignal == btrue))
  {
    moduleLedResetDetection();
  }
}
