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
#include "SystemStatus.h"
#include "IoHwAb_Temperature.h"
#include "IoHwAb_Battery.h"
#include "IoHwAb_Led.h"
#include "SystemLinComm.h"

/**@brief This variant shows system status.
 * It's dangerous to make this external, only for smaller code size.*/
SSystemStatus sysStatus;
/**@brief This variant shows current LIN control signals.
 * It's dangerous to make this external, only for smaller code size.*/
SLinSignals sysLin;

/**
*@details   Initialize status management.
*
*@retval    None.
*/
void systemStatusInit(void)
{
#ifdef DLIN_COMM_NO_SLEEP_WHITE_LIGHT
  sysLin.BCM_RGB_Adressierungsart = 0;
  sysLin.BCM_RGB_Bewertung_Dimmzeit = 0;
  sysLin.BCM_RGB_Blau = 100;
  sysLin.BCM_RGB_Dimmrampe = 0;
  sysLin.BCM_RGB_Dimmzeit_Zeitbasis = 0;
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
  sysLin.BCM_RGB_Dimmzeit_Zeitbasis = 0;
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

/**
*@details   Update system status(Battery, temperature, LIN, LED&System status...).
*
*@retval    None.
*/
void systemStatusUpdate(void)
{
  /*Process Battery*/
  if (IoHwAb_Battery_UpdateStatus() != EBatStatusNormal)
    sysStatus.errFlag |= ESystemErrorBatError;
  else
    sysStatus.errFlag &= ~ESystemErrorBatError;

  /*Process Led open short detection*/
  IoHwAb_Led_UpdateStatus();

  /*Process temperature*/
  IoHwAb_Temperature_Update();

  /*get current new LIN signal*/
  /*Sometimes LIN control signal will update less then DSYSTEM_EVENT_LOOP_PERIOD, we need get update*/
  /*LIN signal out side the main loop. Delete here*/
  //systemLinCommSignalUpdate();

  /*if new LIN signal received, try to power on and detect led for once*/
  if ((sysStatus.errFlag & ESystemErrorLedError) &&
      (sysStatus.newLinSignal == btrue))
  {
    IoHwAb_Led_ResetDetection();
  }
}
