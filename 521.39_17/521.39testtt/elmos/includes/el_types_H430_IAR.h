/***************************************************************************//**
 * @file        el_types_H430_IAR.h
 *
 * @creator     sbai
 * @created     06.10.2014
 *
 * @brief       Provides all basic data type definitions
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
 ******************************************************************************
 *
 * Demo Code Usage Restrictions\:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 ******************************************************************************/

#ifndef EL_TYPES_H430_IAR_H_
#define EL_TYPES_H430_IAR_H_

/* ===================================== */
/* arch and compiler defines             */
/* ===================================== */

#define EL_ARCH_H430               

#define EL_COMPILER_IAR            


/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/

#ifdef NO_STDINT

  typedef __UINT8_T_TYPE__  uint8_t;
  typedef __UINT16_T_TYPE__ uint16_t;
  typedef __UINT32_T_TYPE__ uint32_t;
  #pragma language=save
  #pragma language=extended    
  typedef __UINT64_T_TYPE__ uint64_t;
  #pragma language=restore  
  
  typedef __INT8_T_TYPE__    int8_t;
  typedef __INT16_T_TYPE__   int16_t;
  typedef __INT32_T_TYPE__   int32_t;
  #pragma language=save
  #pragma language=extended  
  typedef __INT64_T_TYPE__   int64_t;
  #pragma language=restore

  
  typedef __INTPTR_T_TYPE__   intptr_t;
  typedef __UINTPTR_T_TYPE__ uintptr_t;
#else
  #include <stdint.h>  // int IAR include not passible, since no LIB selected. 
#endif

#include <stddef.h>

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define __NOP() asm("nop")

/***************************************************************************//**
 * Logical
 ******************************************************************************/
#define TRUE  ( (bool_t)1u )
#define FALSE ( (bool_t)0u )
  

/***************************************************************************//**
 * Inlining
 ******************************************************************************/
#ifndef   __INLINE
  #define __INLINE                  inline
#endif
  
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE           static inline
#endif  
  
#ifndef __NOINLINE
  #define __NOINLINE                _Pragma("inline=never")
#endif  
  

/***************************************************************************//**
 * Root placement
 ******************************************************************************/  
#ifndef __USED  
  #define __USED                    __root
#endif

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

#endif /* EL_TYPES_H430_IAR_H_ */






