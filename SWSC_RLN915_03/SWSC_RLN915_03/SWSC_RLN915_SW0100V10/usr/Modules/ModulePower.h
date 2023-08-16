#ifndef _MODULEPOWER_H_
#define _MODULEPOWER_H_

#include "linStackTask.h"
/** @file
*
* @defgroup RGB_POWER_MODULE RGB power management
* @{
*
* @ingroup BASIC_MODULES
*
* @brief This module manage RGB module sleep mode.
*
* @details This module manage RGB module sleep mode.
*/

/**
*@details   Initialize ELMOS' power module
*
*@retval    None.
*/
void modulePowerInit(void);

/**
*@details   Make MCU entering sleep mode
*
*@retval    None.
*/
void modulePowerSleep();

/** @} */

#endif
