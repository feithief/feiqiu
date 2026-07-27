#include "../Modules/ModulePower.h"

/*
 * power.c
 *
 *  Created on: 2019Äê6ÔÂ2ÈÕ
 *      Author: SH1705016
 */

#include <plib.h>
#include <atomic.h>
#include <delay.h>
#include <flash_defines.h>

void modulePowerEnterSleep(void)
{
  builtin_mlx16_disable_interrupts();       /* Disable the interrupts */
  if (IO_GET(ADC_SAR, STOP)!=1) { /* If ADC is not stopped, we need to stop it first */
      IO_SET(ADC_SAR, STOP, 1);
      DELAY(2);  /* Make sure we will have the ADC HW reaction */
      while (IO_GET(ADC_SAR, ABORTED )==0u) {
          /* Wait for the ADC will be aborted after the STOP command */
      }
  }
  IO_SET (PORT_MELIBUS_CTRL, MELIBUS_EN, 0u);
  WDG_conditionalAwdRefresh(); /* refresh the timers if they're used */
  while ( IO_GET(EEPROM_FLASH, EE_BUSY) != 0u ) {};
  while (( IO_GET(EEPROM_FLASH, FL_STATUS) == FLASH_STATUS_PAGE_PROGRAM ) ||/* Wait till FlASH is stopping program/erase */
         ( IO_GET(EEPROM_FLASH, FL_STATUS) == FLASH_STATUS_SECTOR_ERASE ) ) {};

  __asm__ ("HALT\n\t" :::);
}
