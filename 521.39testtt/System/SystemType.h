#ifndef _SYSTOOL_H_
#define _SYSTOOL_H_

#include "el_types.h"

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
typedef unsigned short int  tick_t;
#define DMAX_SYSTEM_TICK  (0xFFFF)/**<system tick type max value.*/

/**@brief This macro is used to comparing two ticks if a tick exceed or equal b tick.*/
#define DTickAfter(a, b) ((signed short)(b)-(signed short)(a)<=0)

/** @} */


/***************************************************************************//**
 * Logical
 ******************************************************************************/
//#define TRUE  1u
//#define FALSE 0u

#define btrue 1
#define bfalse 0











#endif


