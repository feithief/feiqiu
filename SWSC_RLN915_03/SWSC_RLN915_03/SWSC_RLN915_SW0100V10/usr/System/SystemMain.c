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
//#include "genLinConfig.h"
//#include "lin_driver_api.h"
//#include "lin_app.h"
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

#include "atomic.h"
#include "linSlaveTask.h"
#include "ModuleGPIO.h"   //Debug

/* ============================================================================ */
/* ----------------------------SYSTEM GLOBALE VARIABLES ----------------------- */
/* ============================================================================ */
/*Record last main loop execute tick.*/
static tick_t systemEventTick = 0;
static tick_t linEventTick = 0;
#define        LIN_TASK_CYCLE_MS       2u


extern void LinStack_TaskHandler(void);
extern const uint16_t cFactorList[VehicleNum];
extern const uint8_t cMimUserLList[VehicleNum];
/********************************************************************************
*@details   Initialize all system modules.
*
*@retval    None.
********************************************************************************/
void systemInit(void)
{
  /*Basic clock initialize(including VIC Initial), should be run at the first of functional initialization*/
  moduleClockInit();
  
  modulePowerInit();

  /*For VSenable GPIO 3*/
//  moduleGPIOInit();
  
  /*ADC initial, should run before Battery/Power initial*/
    moduleAdcInit();
  
  /*not used, perhaps to be defined*/
  moduleTempInit();
  
  /*In fact, it is PWM initial*/
  moduleLedInit();
  
  /*EEPROM Initial, it may cause watch dog reset if put it behind*/
  moduleFlashInit(); //just for debug
//  systemLinCommChangeNad(savedConfig.singleAddr);
  
  /*IHR functions, used for LIN Initial*/
  SystemLINInit();
  NVIC_SetPriority (LINS_IRQn, (1UL << __NVIC_PRIO_BITS) - 2UL);
  /*Update battery status. if other status/flags are initial value, it represents battery initialization*/
  moduleBatInit();
  
  /*Give most of the modules flags initial status value*/
  systemStatusInit();

  /*Algorithm initialize, set core color parameters initial values*/
  algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
  algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
  algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
  
  /*watch-dog initialize and feed once*/
  //moduleWatchdogInit();


  /*it provides the mainloop 5ms to initialize*/
  systemEventTick = moduleClockGet() + DSYSTEM_EVENT_LOOP_PERIOD;
}


/********************************************************************************
*@details   System main loop function.
*
*@retval    None.
*********************************************************************************/


//static uint32_t DebugCNT2 = 0;
//static uint32_t DebugCNT3 = 0;
void systemMainLoop(void)
{
  SFlashContent saveconfig_1;
    moduleFlashLoad1(&saveconfig_1);
    int p;
    for (p = 0; p<6; p++)
    {
        if(savedConfig.Lfactor[p] == 0 )
        {
            savedConfig.Lfactor[p] = saveconfig_1.Lfactor[p];
        }
    }
  bool_t LINFlag = bfalse, tickFlag = bfalse;
  
  
//  GPIO_Set(GPIO_PORT_3, GPIO_HIGH);
  if (flashFlag == EFlashUpdateProcessing)
  {
    moduleWatchdogFeed();
    /*if single address temporarily change to 0xA0, we do not save it*/
    if (savedConfig.singleAddr != lastSavedSingalAdress && savedConfig.singleAddr!=0)
    {
      systemLinCommChangeNad(savedConfig.singleAddr);
      lastSavedSingalAdress = savedConfig.singleAddr;
    }
    
    if(savedConfig.groupAddr == 0)  /* label group addr = 0, group addr = 1*/
    {
      savedConfig.groupAddr = 1;
    }
    
    /*update LED R/G/B light attributes*/
    algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
    algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
    algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
    
    
        moduleFlashSave();
    
    flashFlag = EFlashUpdateFree;
  }
  else //Flash/EEPROM process is over
  {
    
    /*System Tick enables the system run every 'systemEventTick' time*/
    if (DTickAfter(moduleClockGet(), systemEventTick))
    {
      tickFlag = btrue;
      systemEventTick += DSYSTEM_EVENT_LOOP_PERIOD;

    }
    else 
    {
      tickFlag = bfalse;
    }
    
    if (DTickAfter(moduleClockGet(), linEventTick))
    {
      linEventTick += LIN_TASK_CYCLE_MS;
      LinStack_TaskHandler();    // IHR LINTask()
    }
    
    if (tickFlag == btrue) 
    {  
      SColorParams inputColor;
      SPWMParams outputPWM;
     
      /*  LIN Sleep Status */
      __atomic_enter()     // no ;
      LINFlag = systemLinCommSignalUpdate();  
      __atomic_exit()      // no ;
                
      if ((LINFlag == btrue) || (tickFlag == btrue))  
      {        
        /*get current system status*/
        systemStatusUpdate();
             
        systemControl(&inputColor);
        
//        sysStatus.errFlag = ESystemErrorNone;
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
               
        __atomic_enter();
        /*update response signals*/
        systemLinCommRespUpdate();
       __atomic_exit();

        /*enter into real sleep mode now*/
        if (sysStatus.sleepFlag == ESystemSleepSleep)
        {
          systemLinCommSleep();
          systemControlSleep();
          moduleLedSleep();
          modulePowerSleep();
        }       
      } //if ((LINFlag == btrue) || (tickFlag == btrue))    
    }  //if (tickFlag == btrue)      
    else  //if (tickFlag == bfalse) 
    {
      moduleWatchdogFeed();  // for break pointd
    }
    
    moduleWatchdogFeed();
  } // not flashing
//  GPIO_Set(GPIO_PORT_3, GPIO_LOW);
}
