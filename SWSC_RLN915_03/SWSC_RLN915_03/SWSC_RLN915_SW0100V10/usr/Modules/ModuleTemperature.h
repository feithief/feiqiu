#ifndef _MODULETEMPERATURE_H_
#define _MODULETEMPERATURE_H_

#include "SystemType.h"
/** @file
*
* @defgroup RGB_TEMPERATURE_MODULE RGB temperature update module
* @{
*
* @ingroup BASIC_MODULES
*
* @brief This module maintains LED junction temperature.
*
* @details This module maintains LED junction temperature. The measure algorithm is from Melexis.
*/

/**@brief This macro defines temperature's accuracy.*/
#define DTEMP_MULTIPLIED_RATIO       10

/**
*@details   Initialize temperature detection.
*
*@warning   This function must be invoked before @moduleTempUpdate.
*
*@retval    None.
*/
void moduleTempInit(void);

/**
*@details   Temperature detection callback function, Currently, we do this every 250ms(experience time).
*           This function will update @ledTemperature value.
*
*@note      For more details, please refer to development document.
*
*@retval    None.
*/
void moduleTempUpdate(void);

/**
*@details   Get current LED temperature.
*
*@retval    current LED temperature.
*/
int16_t moduleTempLedGet(void);

/**
*@details   Get current MCU temperature.
*
*@retval    current MCU temperature.
*/
int16_t moduleTempMcuGet(void);
/** @} */

#endif
