/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemType.h
 * Project:		RLNE15S
 * Description:	This header file defines the basic data types and macros used 
 *              across the system modules. It includes tick type definitions, 
 *              logical data types, and special data types for embedded system 
 *              development. The file provides compatibility with Elmos's CPU 
 *              APIs for intelligent ICs.
 * Note(s):	    - This module defines a basic tick comparison macro for time 
 *                management and a boolean type for logical operations.
 *              - The `bool_t` type is defined for clarity and consistency 
 *                across the codebase.
 * Update Record:	
 * Version     	Date     	 Modified by       Changes Made
 * 00.01      2024.11.28      Genhua Yang      Initial version
 ************************************************************************
 */

#ifndef SYSTOOL_H
#define SYSTOOL_H



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
#define DMAX_SYSTEM_TICK  (0xFFFFu)/**<system tick type max value.*/

/**@brief This macro is used to comparing two ticks if a tick exceed or equal b tick.*/
#define DTickAfter(a, b) ((int16_t)((int16_t)(b)-(int16_t)(a))<=0)

/** @} */


/***************************************************************************//**
 * Logical
 ******************************************************************************/




/***************************************************************************//**
 * @file			el_types.h
 *
 * @creator		sbai
 * @created		06.10.2014
 *
 * @brief  		Provides all basic data type definitions
 *
 * @purpose
 *
 * Provides all basic data type definitions for Elmos's CPU APIs and development
 * on ELMOS'S intelligent ICs.
 *
 * $Id: el_types.h 30 2014-11-12 13:20:06Z sbai $
 *
 * $Revision: 30 $
 *
 ******************************************************************************/

#ifndef EL_TYPES_H_
#define EL_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include <stdint.h>  // int IAR include not passible, since no LIB selected. 
#include <stddef.h>
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/



#define btrue 1
#define bfalse 0

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * Memory class and type definitions
 ******************************************************************************/

/***************************************************************************//**
 * Boolean data type
 ******************************************************************************/
typedef uint8_t bool_t;

/***************************************************************************//**
 * Floating point data - seldom here...
 ******************************************************************************/


/***************************************************************************//**
 * Bitfield-types (according to MISRA-C)
 ******************************************************************************/

/***************************************************************************//**
 * Special data types
 ******************************************************************************/








/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* EL_TYPES_H_ */
















#endif


