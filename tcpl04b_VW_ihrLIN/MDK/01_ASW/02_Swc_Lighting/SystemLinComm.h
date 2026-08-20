#ifndef _SYSTEMLINCOMM_H_
#define _SYSTEMLINCOMM_H_

#include "SystemType.h"

/** @file
*
* @defgroup SYSTEM_LIN_COMM LIN communication module.
* @{
*
* @ingroup RGB_SYSTEM
*
* @brief This module manage LIN common communication functions.
*
* @details This module manage LIN common communication functions.
*/

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

/**
*@details   LIN common communication initialization.
*
*@retval    None.
*/
void systemLinCommInit(void);

/**
*@details   LIN common communication initialization.
*
*@params[in] newNad new single address.
*
*@retval    None.
*/
void systemLinCommChangeNad(uint8_t newNad);

/**
*@details   MELEXIS' LIN handler.
*
*@retval    None.
*/
void systemLinCommHandler(void);

/**
*@details   update LIN slave signal.
*
*@retval    if LIN signal has been updated.
*/
bool_t systemLinCommSignalUpdate(void);
/**
*@details   update LIN slave response signal.
*
*@retval    None.
*/
void systemLinCommRespUpdate(void);

/**
*@details   system LIN communication module prepare to enter sleep.
*
*@retval    None.
*/
void systemLinCommSleep(void);

/** @} */

#endif
