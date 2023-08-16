#ifndef _MODULEWATCHDOG_H_
#define _MODULEWATCHDOG_H_

/** @file
*
* @defgroup RGB_WATCHDOG_MODULE RGB watch dog module
* @{
*
* @ingroup BASIC_MODULES
*
* @brief This module contains basic function of watch-dog.
*
* @details This module contains basic function of watch-dog.
*/

/**
*@details   Initialize watch dog hardware. Timeout time is 100ms for now.
*
*@warning   This function must be invoked before @moduleWatchdogFeed.
*
*@retval    None.
*/
void moduleWatchdogInit(void);

/**
*@details   Feed dog.
*
*@retval    None.
*/
void moduleWatchdogFeed(void);

/** @} */

#endif
