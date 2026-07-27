/***************************************************************************//**
 * @file      mod_wdog.h
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
 * $Id: mod_wdog.h 8917 2021-05-20 10:29:54Z poe $
 *
 * $Revision: 8917 $
 *
 ******************************************************************************/

#ifndef MOD_WDOG_H_
#define MOD_WDOG_H_

#include "el_helper.h"
/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FORWARD DECLARATIONS ****************************/
/* ****************************************************************************/

bool_t moduleWatchdogInit(void);
void wdog_Start(void);
void moduleWatchdogFeed(void);

#endif /* MOD_WDOG_H_ */
