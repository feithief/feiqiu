#ifndef _SYSTEMCONTROL_H_
#define _SYSTEMCONTROL_H_
#include "Algorithm.h"

/** @file
*
* @defgroup SYSTEM_CONTROL system control module.
* @{
*
* @ingroup RGB_SYSTEM
*
* @brief This module controls RGB LIN module.
*
* @details This module controls RGB LIN module.
*/

extern uint8_t Mode;

/**
*@details   Controls color.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
void systemControl(SColorParams *inputColor);

/**
*@details   Control module prepare to enter sleep.
*
*@retval    None.
*/
void systemControlSleep(void);

/** @} */

#endif
