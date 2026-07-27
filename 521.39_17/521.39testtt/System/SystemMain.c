/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    SystemMain.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file maintains system main initialization and loop.
  *Others:      None.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
                     V01: we will watch LIN signal update status out side main loop.
                          modified by Leo, Song. 2018.1.12
**********************************************************************************/


/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "SystemMain.h"
#include "SystemType.h"
#include "ModuleWatchdog.h"
#include "ModuleClock.h"
#include "genLinConfig.h"
//#include "lin_driver_api.h"
#include "lin_app.h"
#include "ModuleAdc.h"
#include "SystemStatus.h"
#include "ModuleTemperature.h"
#include "Algorithm.h"
#include "ModuleFlash.h"
#include "ModuleLed.h"
#include "ModuleBattery.h"
#include "SystemControl.h"
#include "SystemLinComm.h"
#include "ModulePower.h"  //sleep related
#include "CommonInterruptHandler.h"
#include "LinDrvImp_Config.h"
#include "main.h"
#include "io_e52139b_IAR.h"


#include "LinDiag_Implementation.h"
/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */

/* ============================================================================ */
/* ----------------------------SYSTEM GLOBALE VARIABLES ----------------------- */
/* ============================================================================ */
/*Record last main loop execute tick.*/
static tick_t systemEventTick = 0;
static tick_t linEventTick = 0;


/********************************************************************************
*@details   Initialize all system modules.
*
*@retval    None.
********************************************************************************/
void systemInit(void)
{
    /* init sys_state module & and IRQ If */
    
    /*Basic clock initialize(including VIC Initial), should be run at the first of functional initialization*/
    moduleClockInit();
    
    modulePowerInit();
    
    /*watch-dog initialize and feed once*/
    moduleWatchdogInit();
    moduleWatchdogFeed();
    
    
    /*EEPROM Initial, it may cause watch dog reset if put it behind*/
    romIf_MainInterface->Interface_Get(ROMIF_MTP_ACCESS, (romIf_cpGenericInterface_t *) &EEImp_IfFuns, LIN_NULL);
    romIf_MainInterface->Interface_Get(ROMIF_FLASH_ACCESS, (romIf_cpGenericInterface_t *) &FlashImp_IfFuncs, LIN_NULL);
    moduleFlashInit(); //just for debug
    
    
    /*ADC initial, should run before Battery/Power initial*/
    moduleAdcInit();
    
    /*not used, perhaps to be defined*/
    moduleTempInit();
    
    /*In fact, it is PWM initial*/
    moduleLedInit();
    
    /* init LIN driver */
    if(LinDrvImp_Init() != LIN_TRUE)
    {
        while(1); //等看门狗复位
    }
    //systemLinCommChangeNad(savedConfig.singleAddr);  // 在初始化中以调用
    SystemLinCommInit();
    
    /*Update battery status. if other status/flags are initial value, it represents battery initialization*/
    moduleBatInit();
    
    /*Give most of the modules flags initial status value*/
    systemStatusInit();
    
    /*Algorithm initialize, set core color parameters initial values*/
    algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
    algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
    algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
    
    /*it provides the mainloop 5ms to initialize*/
    systemEventTick = moduleClockGet() + DSYSTEM_EVENT_LOOP_PERIOD;
}


/********************************************************************************
*@details   System main loop function.
*
*@retval    None.
*********************************************************************************/
void systemMainLoop(void)
{
  bool_t LINFlag = bfalse, tickFlag = bfalse;
  
  if (flashFlag == EFlashUpdateProcessing)
  {
    moduleWatchdogFeed();
    /*if single address temporarily change to 0xA0, we do not save it*/
    if (savedConfig.singleAddr != lastSavedSingalAdress)
    {
      if(savedConfig.singleAddr!=0)
      {
        systemLinCommChangeNad(savedConfig.singleAddr);
        lastSavedSingalAdress = savedConfig.singleAddr;
      }
    }
    
    /*update LED R/G/B light attributes*/
    algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
    algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
    algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
    if (!(flashFlag & EFlashUpdateLocked))
        moduleFlashSave();
    while( btrue==GetEEPROMBusyState())
    {
      moduleWatchdogFeed();
    }
    flashFlag = EFlashUpdateFree;
  }
  else //Flash/EEPROM process is over
  {
    /*System Tick enables the system run every 'systemEventTick' time*/
    if (DTickAfter(moduleClockGet(), systemEventTick))
      tickFlag = btrue;
    else tickFlag = bfalse;
    
    /*IHR rules that LIN_Task must run before any other IHR functions or ReadWrite operations within a loop*/
    if (DTickAfter(moduleClockGet(), linEventTick))
    {
      linEventTick += LIN_TASK_CYCLE_MS;
      LinDrvImp_Task();
      /* protocol layer task */
      LinDrvImp_ProtoIfFuns->Task((LinProtoIf_pGenericEnvData_t) LinDrvImp_ProtoEnvData);
      /* transport layer task */
      LinDrvImp_TransIfFuns->Task((LinTransIf_pGenericEnvData_t) LinDrvImp_TransEnvData);
    }

    if (tickFlag == btrue)
    {
      SColorParams inputColor;
      SPWMParams outputPWM;
      
      //LIN_Sleep_Status();//delete
      
      /*Update LIN signals*/      
      dint();
      LINFlag = systemLinCommSignalUpdate();
      eint();
    
      /*Update tick. */
      systemEventTick += DSYSTEM_EVENT_LOOP_PERIOD;
            
      if ((LINFlag == btrue) || (tickFlag == btrue))
      {       
        /*get current system status*/
        systemStatusUpdate();

        /*get control signal*/
        systemControl(&inputColor);
                    
        
        //sysStatus.errFlag = ESystemErrorNone;
        /*if error occurred, turn off led power, otherwise normal control*/
        if (sysStatus.errFlag == ESystemErrorNone)
        {
          /*get output PWM*/
          algoGetPWMOutput(&inputColor, &outputPWM);

          /*update PWM*/
          moduleLedUpdatePWM(outputPWM.PWM_R, outputPWM.PWM_G, outputPWM.PWM_B);           
        }
        /*System has malfunction*/
        else  
        {
          inputColor.intensity = 0;
          moduleLedPowerOff();                          
        } //if (sysStatus.errFlag == ESystemErrorNone)     
              
        /*update response signals*/
        dint();
        systemLinCommRespUpdate();
        eint();

        /*enter into real sleep mode now*/
        if (sysStatus.sleepFlag == ESystemSleepSleep)
        {
          systemLinCommSleep();
          systemControlSleep();
          moduleLedSleep();
          modulePowerSleep();
        }
 
      } //if(LINFlag == btrue)
      else//if(LINFlag == bfalse)
      {
        moduleWatchdogFeed();
      }             
    }//if (tickFlag == btrue)
       
    moduleWatchdogFeed();
    
  }//if (flashFlag != EFlashUpdateProcessing)
}
