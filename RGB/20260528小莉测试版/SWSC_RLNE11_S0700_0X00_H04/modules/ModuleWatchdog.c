/***************************************************************************//**
 * @file      mod_wdog.c
 *
 * @creator   poe
 * @created   2021.05.04 
 *
 * @brief     Example for WDOG usage.
 *
 * @purpose   Simple implementation example for usage of the WDOG module.
 *            
 * 
 *
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
 * $Id: mod_wdog.c 8917 2021-05-20 10:29:54Z poe $
 *
 * $Revision: 8917 $
 *
 ******************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/

#include "ModuleWatchdog.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define WDOG_PASS              (0xA500u)
#define WDOG_WINDOW_DEFAULT    (  0x0Fu)
#define WDOG_WINDOW_VALUE      (0x8000u) //actual window counter value is 2^WDOG_WINDOW value, (2^F=0x8000)
/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FORWARD DECLARATIONS ****************************/
/* ****************************************************************************/

/* Watchdog initialization. */
bool_t moduleWatchdogInit(void)
{
  /* Set default return value. */
  bool_t retVal = TRUE;

  /* 239+1 => 100KHz. */
  WDOG_PRESCALER  =   239u; 
  /* Set interval to 0.5s. */
  WDOG_RELOAD     = 49999u; 
  /* Enable WDog window. */
  WDOG_WINDOW    |=  E_WDOG_WINDOW_ENABLE; 
  
  /* Enable WDog software reset. */
  SYS_STATE_RESET_ENABLE |= E_SYS_STATE_RESET_ENABLE_WATCHDOG |E_SYS_STATE_RESET_ENABLE_WATCHDOG_ZERO | E_SYS_STATE_RESET_ENABLE_WATCHDOG_WINDOW;
    
  return retVal;
}

/* Enable Watchdog. */
void wdog_Start(void)
{
  /* Enable Watchdog. */
  WDOG_CONTROL = WDOG_PASS | E_WDOG_CONTROL_RUN_ENABLE; 
}

/* Kick the Watchdog. */
void moduleWatchdogFeed(void)
{
  /* Is wdog counter inside wdog window? */
  if ( WDOG_COUNTER < WDOG_WINDOW_VALUE)
  {
    /* Kick the Watchdog. */
    WDOG_CONTROL = WDOG_PASS | E_WDOG_CONTROL_RESTART; 
  }
}