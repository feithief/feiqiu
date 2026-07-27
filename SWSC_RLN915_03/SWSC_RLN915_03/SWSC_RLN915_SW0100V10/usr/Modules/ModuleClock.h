
#ifndef _MODULECLOCK_H_
#define _MODULECLOCK_H_

#include "SystemType.h"

/** @file
*
* @defgroup BASIC_CLOCK_MODULE basic clock module.
* @{
*
* @ingroup BASIC_MODULES
*
* @brief This module provide clock timing functions.
*
* @details This module provide clock timing functions. So many of our requirements can
*          use this time services to calculate time.
*/

/**
*@details   Initialize clock module.
*
*@retval    None.
*/
void moduleClockInit(void);

/**
*@details   Get current clock tick(We set 1 tick equals 1ms).
*
*@retval    Current tick.
*/
tick_t moduleClockGet(void);

/** @} */

#endif
