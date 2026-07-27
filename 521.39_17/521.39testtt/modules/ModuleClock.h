/***************************************************************************//**
 * @file      mod_swtimer.h
 *
 * @creator   poe
 * @created   2021.05.04 
 *
 * @brief     Example for SW Timer usage.
 *
 * @purpose   Simple implementation example for usage of the swtimer module.
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
 * $Id: mod_swtimer.h 8848 2021-05-10 14:11:24Z poe $
 *
 * $Revision: 8848 $
 *
 ******************************************************************************/

#ifndef MOD_SWTIMER_H_
#define MOD_SWTIMER_H_

#include "SystemType.h"
/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "vic_Interface.h"
extern vic_cpInterfaceFunctions_t vic_IfFuns; /* VIC Interface ptr. */

bool_t moduleClockInit1(vic_cpInterfaceFunctions_t vicIf);
//void swtimer_StartTimer0(void);
void moduleClockISR(int sno);
bool_t moduleClockInit(void);
bool_t __moduleClockSysInit(void);


tick_t moduleClockGet(void);







#endif /* MOD_SWTIMER_H_ */
