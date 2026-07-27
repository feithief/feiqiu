#ifndef _SYSTEMCONTROL_H_
#define _SYSTEMCONTROL_H_
#include "Algorithm.h"
#define Einzeladresse  0  /**<Using single address.*/
#define Gruppenadresse 1  /**<Using group address.*/

#define keine_Sonderfunktion 0     /**<Normal mode.*/
#define Crash                1     /**<Crash mode.*/
#define Showroommodus        2     /**<Showroom mode.*/
#define Stellgliedtest       3     /**<Actuator mode.*/

#define Dimmzeit_absolut 0  /**<Absolute dimming.*/
#define Dimmzeit_relativ 1  /**<Relative dimming.*/

#define init          0  /**<error flag init.*/
#define nicht_erlaubt 1  /**<error flag reserved.*/
#define kein_Fehler   2  /**<error flag no error.*/
#define Fehler        3  /**<error flag error.*/
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
