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
SSystemStatus sysStatus;

/*******************************************************************************
*@brief This variant shows current LIN control signals.
*@Note  It's dangerous to make this external, only for smaller code size.
*******************************************************************************/
SLinSignals sysLin, currentLinBusMsg;


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
  sysLin.BCM_RGB_Adressierungsart = 0;
  sysLin.BCM_RGB_Bewertung_Dimmzeit = 0;
  sysLin.BCM_RGB_Blau = 100;
  sysLin.BCM_RGB_Dimmrampe = 0;
  sysLin.BCM_RGB_Gruen = 255;
  sysLin.BCM_RGB_Gueltigkeit = 0;
  sysLin.BCM_RGB_Intensitaet = 100;
  sysLin.BCM_RGB_Normierung_Intensitaet = 0;
  sysLin.BCM_RGB_Rot = 175;
  sysLin.BCM_RGB_Slave_Adresse = 0;
  sysLin.BCM_RGB_Sonderfunktion = 0;
  sysLin.unused52_0 = 0;
  sysLin.unused59_0 = 0;
  sysStatus.errFlag = ESystemErrorNone;
  sysStatus.newLinSignal = btrue;
  sysStatus.sleepFlag = ESystemSleepAwake;
#else
  sysLin.BCM_RGB_Adressierungsart = 0;
  sysLin.BCM_RGB_Bewertung_Dimmzeit = 0;
  sysLin.BCM_RGB_Blau = 0;
  sysLin.BCM_RGB_Dimmrampe = 0;
  sysLin.BCM_RGB_Gruen = 0;
  sysLin.BCM_RGB_Gueltigkeit = 0;
  sysLin.BCM_RGB_Intensitaet = 0;
  sysLin.BCM_RGB_Normierung_Intensitaet = 0;
  sysLin.BCM_RGB_Rot = 0;
  sysLin.BCM_RGB_Slave_Adresse = 0;
  sysLin.BCM_RGB_Sonderfunktion = 0;
  sysLin.unused52_0 = 0;
  sysLin.unused59_0 = 0;
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
