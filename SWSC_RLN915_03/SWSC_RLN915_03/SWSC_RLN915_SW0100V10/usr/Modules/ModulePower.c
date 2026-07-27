/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModulePower.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file contains function make MCU entering sleep mode.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

#include "ModulePower.h"
#include "pmu_device.h"
#include "SystemStatus.h"
/**
*@details   Initialize ELMOS' power module
*
*@retval    None.
*/
void modulePowerInit(void)
{
  //pmu init
    SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY    = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.TRIM_ACCESS_KEY        = 0x0EU;
    SYSCTRLA_REG_PMU_ACCESS_KEY.PMU_ACCESS_KEY          = 0x0AU;
    /* Init set BOR voltage level for cpu low voltage safety*/
    PMU_BORInit(BOR_1V5_THRS_1328mV, BOR_3V3_THRS_2998mV);
    /* Disable wake up timer */
    PMU_WakeTimerInit(WAKEUP_TIMEER_DISABLE, WAKEUP_TIMEER_INTERVAL_32768ms);
    
    SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY    = 0x00U;
}

/**
*@details   Make MCU entering sleep mode
*
*@retval    None.
*/
void modulePowerSleep()
{
  (void)ls_clr_go_to_sleep_flag();

  PMU_EnterDeepSleepMode();   //if the fun work, need lin message to wake up. 
  
}

