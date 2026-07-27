/***************************************************************************//**
 * @file		mod_sleep.c
 *
 * @creator		wca
 * @created		2022.06.23
 *
 * @brief  		Top application layer with main() entry point.
 *
 * @purpose             Hardware and LIN driver intialization, 
 *                      top application layer.
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 * $Id: $
 *
 * $Revision:  $
 *
 ******************************************************************************/

#include "ModulePower.h"
#include "io_e52139b_IAR.h"


extern uint8_t GucSleepEn;

void sleepEnSet(void)
{
    GucSleepEn = 1;
}

void modulePowerInit(void)
{

}

void modulePowerSleep(void)
{
    if ( 0 != GucSleepEn ) {
        SYS_STATE_SLEEP_ENABLE |= E_SYS_STATE_SLEEP_ENABLE_SOFTWARE;
        SYS_STATE_POWER        |= E_SYS_STATE_POWER_SLEEP;
    }
}

