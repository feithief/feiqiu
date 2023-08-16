/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file systemInit.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */

#include <systemInit.h>
#include <realplumLite.h>
#include <linSlaveTask.h>
#include <linStackTask.h>

void leds_driver_Init(void);
void gpios_init(void);
void pmu_init(void);

void leds_driver_Init(void)
{
    (void)PWM_SetLedCurrent();
    PWM_Init(PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX);
}


void gpios_init(void)
{
    /* set gpios to input with power up resistor */
    for (uint8_t i = (uint8_t)GPIO_INIT_PORT_1; i <= (uint8_t)GPIO_INIT_PORT_2; i++){
        GPIO_Init((GpioInitPort_t)i,GPIO_DIR_INPUT,GPIO_PULL_UP);
    }

}

void pmu_init(void)
{
    SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY    = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.TRIM_ACCESS_KEY        = 0x0EU;
    SYSCTRLA_REG_PMU_ACCESS_KEY.PMU_ACCESS_KEY          = 0x0AU;
    /* Init set BOR voltage level for cpu low voltage safety*/
    PMU_BORInit(BOR_1V5_THRS_1328mV, BOR_3V3_THRS_2998mV);
    /* Disable wake up timer */
    PMU_WakeTimerInit(WAKEUP_TIMEER_DISABLE, WAKEUP_TIMEER_INTERVAL_32768ms);
    
    SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY    = 0x00U;
}


void SYS_Init(void)
{
    /* Enable trim revise access enable*/
    HWCFG_TrimAccessUnlock();
    CRGA_SFRS->MODULERSTREQ = 0xFFU;
    /* Init system clock */
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
    pmu_init();
    /* Init global timer engine for driving soft timer */
    SysTick_Init(SOFT_TIMER_INTERVAL *1000U * MAIN_CPU_CLOCK, SoftTimer_ExpireCallback);
#if WATCH_DOG_EN == 1U
      WDTA_Enable(WDTA_INTERVAL_8S); /* 8s */
#endif
    /* Init gpios settings */
    gpios_init();
    /* Init LED current and PWM settings */
    leds_driver_Init();
    /*HWCFG_TrimAccessLockUntilReset();*/
    
    /* tasks init must be called before use. */
    TM_PostTask(TASK_ID_SOFT_TIMER);
    TM_PostTask(TASK_ID_SAFETY_MONITOR);
    TM_PostTask(TASK_ID_LINS);
    TM_PostTask(TASK_ID_PDS);
    TM_PostTask(TASK_ID_ADC_MEASURE);
    TM_PostTask(TASK_ID_COLOR_MIXING);
    TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    TM_PostTask(TASK_ID_APPL);
}



