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

/**
*@details   LIN common communication initialization.
*
*@retval    None.
*/
void SystemLinCommInit(void);

/**
*@details   LIN common communication initialization.
*
*@params[in] newNad new single address.
*
*@retval    None.
*/
//void systemLinCommChangeNad(uint8_t newNad);  //∞·µΩ¡À LinDrvImp_Config.c÷–

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
