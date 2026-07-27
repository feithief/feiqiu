/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleClock.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains functions that maintains system tick.
  *Others:      None.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "ModuleClock.h"
#include "clock_device.h"
#include "systick_device.h"

#include "ModuleGPIO.h"   //Debug



/*brief This variant contains current system ticks.*/
tick_t currentTick = 0;

/* ============================================================================ */
/* ------------------------ Application Definition ---------------------------- */
/* ============================================================================ */


/********************************************************************************
*@details   system Tick interrupt service routing.
*
*@retval    None.
********************************************************************************/
//volatile uint8_t test = 0;
void moduleClockISR(void)
{
  currentTick+=1;
//  if(currentTick % 2 == 0)
//  {
//    GPIO_Set(GPIO_PORT_1, GPIO_HIGH);
//  }
//  else
//  {
//    GPIO_Set(GPIO_PORT_1, GPIO_LOW);
//  }
}

/********************************************************************************
*@details   Initialize clock module.
*
*@retval    None.
********************************************************************************/
void moduleClockInit(void)
{
  /* Init system clock */
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
  
  /* Init global timer engine for driving soft timer */
    //SysTick_Init(1000U * MAIN_CPU_CLOCK * 8, moduleClockISR);
    SysTick_Init(1000U * MAIN_CPU_CLOCK, moduleClockISR);
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* 把systick优先级设置最低*/
    /* 注意定时器1已经被ADC用掉了 */
}


/********************************************************************************
*@details   Get current clock tick(Basicly, we set 1 tick equals 1ms).
*
*@retval    Current tick.
********************************************************************************/
tick_t moduleClockGet(void)
{
  tick_t temp;
  temp = currentTick;
  return temp;
}

