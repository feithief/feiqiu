#ifndef _SYSTOOL_H_
#define _SYSTOOL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "realplumLite.h"

#include "appConfig.h"

/** @file
*
* @defgroup SYSTEM_BASIC_TYPE system basic type.
* @{
*
* @brief This module defines system basic variant type.
*
* @details This module defines system basic variant type.
*/

/**@brief defines system tick type*/
typedef uint16_t  tick_t;
#define DMAX_SYSTEM_TICK  (0xFFFF)/**<system tick type max value.*/

/**@brief This macro is used to comparing two ticks if a tick exceed or equal b tick.*/
#define DTickAfter(a, b) ((int16_t)((int16_t)(b)-(int16_t)(a))<=0)

/** @} */


/***************************************************************************//**
 * Logical
 ******************************************************************************/
/**@brief defines boolean type*/
typedef unsigned char       bool_t;
#define btrue               1/**<boolean type true.*/
#define bfalse              0/**<boolean type false.*/


//#define TRUE  1u

//#define FALSE 0u











#endif


