#ifndef _SYSTOOL_H_
#define _SYSTOOL_H_



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
#define DTickAfter(a, b) ((int16_t)((int16_t)(b)-(int16_t)(a))<=0)

/** @} */


/***************************************************************************//**
 * Logical
 ******************************************************************************/
#define TRUE  1u
#define FALSE 0u

#define btrue 1
#define bfalse 0

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

#ifdef NO_STDINT
  typedef unsigned char  uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned long  uint32_t;
#else
  #include <stdint.h>  // int IAR include not passible, since no LIB selected. 
#endif

#include <stddef.h>

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/


/***************************************************************************//**
 * Logical
 ******************************************************************************/
#define TRUE  1u
#define FALSE 0u

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
typedef float float32_t; /**> 32 bit type */
typedef double float64_t; /**> 64 bit type */

/***************************************************************************//**
 * Bitfield-types (according to MISRA-C)
 ******************************************************************************/
typedef unsigned int  bitfield_t;

/***************************************************************************//**
 * Special data types
 ******************************************************************************/
typedef struct bits
{
  unsigned char b0 :1;
  unsigned char b1 :1;
  unsigned char b2 :1;
  unsigned char b3 :1;
  unsigned char b4 :1;
  unsigned char b5 :1;
  unsigned char b6 :1;
  unsigned char b7 :1;
} BITS;

typedef union bit_char
{
  uint8_t val;
  BITS b;
} BIT_CHAR;

typedef struct bit_2char
{
  BIT_CHAR highbyte;
  BIT_CHAR lowbyte;
} BIT_2CHAR;

typedef struct bit_3char
{
  BIT_CHAR highbyte;
  BIT_CHAR midbyte;
  BIT_CHAR lowbyte;
} BIT_3CHAR;

typedef struct bit_4char
{
  BIT_2CHAR highword;
  BIT_2CHAR lowword;
} BIT_4CHAR;

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* EL_TYPES_H_ */
















#endif


