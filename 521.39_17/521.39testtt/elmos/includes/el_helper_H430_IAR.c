/***************************************************************************//**
 * @file        el_helper_H430_IAR.c
 *
 * @creator		sbai
 * @created		17.06.2015
 *
 * @brief       Provides helper macros
 *
 * @purpose
 *
 * Provides helper functions for Elmos's CPU APIs and development
 * on ELMOS'S intelligent ICs.
 *
 * $Id: $
 *
 * $Revision: $
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

/* ****************************************************************************/
/* ******************** Helix QAC, MISRA Rule Suppression ***************************/
/* ****************************************************************************/

/* PRQA S 0883 EOF #justification: This is a helper c-file that is included once. 
                   There is no need for include guard here. To include c-file is 
                   an absolute exceptional case here. */
/* Rule: 0883 MISRA r Dir-4.10
   Circumstances: Include file code is not protected against repeated inclusion
   Potential consequences: This file can only be included over the el_helper.h. So there should be no potential negative
                   consequences to suppress rule here.
   Alternative safety assurance: none */ 

/* PRQA S 1338 EOF #justification: This is a helper file. Purpose of this file is to pass parameters to helper functions and then
                   do some copying, filling, camparing or erasing. */
/* Rule: 1338 MISRA a Rule-17.8
   Circumstances: The value of a function parameter is being modified.
   Potential consequences: Only programmars that are familiar with c-language will use this helper functions. There are not negative
                   consequences to suppress rule here.
   Alternative safety assurance: none */ 

/* PRQA S 3243 EOF #justification: It is a good practice to declare a function inline in a c-file and extern in a
                   header file to give the compiler the possibility to inline function outside of the
                   containing module. On this way a compiler like the Keil compiler, which supports
                   "multi-file compilation", is able to inline generic functions, i.e. like memory
                   write/copy/etc functions, to other modules.*/
/* Rule: 3243 MISRA r Rule-8.10
   Circumstances: An inline function is being declared as an external definition.
   Potential consequences: The compiler will at least warn you about an unexpected behavior or just don't inline the
                           function.
   Alternative safety assurance: none */ 

/* PRQA S 1503 EOF #justification: This is a helper file and is part of a library bundle - the functions are not intended
 *                                 to be all used in one single project and is distributed into different projects as well. */

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_helper.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ******************** FORWARD DECLARATIONS / PROTOTYPES *********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FUNCTION DEFINITIONS ****************************/
/* ****************************************************************************/

/***************************************************************************//**
 * TODO: Concrete implementation description.
 *
 ******************************************************************************/
inline void el_CopyBytes(const uint8_t *src, uint8_t *dest, uint16_t srclen)
{
  while(srclen>0u)  
  { 
    *dest = *src;
    --srclen;
    dest++;
    src++;
  }
}

/***************************************************************************//**
 * TODO: Concrete implementation description.
 *
 ******************************************************************************/
inline void el_FillBytes(const uint8_t value, uint8_t *dest, uint16_t destlen)
{
  while(destlen>0u)
  {
    *dest = value;  
    --destlen;
    dest++;
  }
}

/***************************************************************************//**
 * TODO: Concrete implementation description.
 *
 ******************************************************************************/
// justification(3240): This is a helper file, helper files are used all over the project to ease our implementation
// PRQA S 3240 ++
// justification(1503): Following function is helper function to fill words, it will be used in project soon
// PRQA S 1503 ++
inline void el_FillWords(const uint16_t value, uint16_t *dest, uint16_t destlen)
// PRQA S 3240 --
// PRQA S 1503 --
{
  while(destlen>0u)  
  { 
    *dest++=value;  
	--destlen; 
  }
}

/***************************************************************************//**
 * TODO: Concrete implementation description.
 *
 ******************************************************************************/
/* PRQA S 1532 ++ #justification: this is a function with library character and intended to be used outside of this project. */
inline void el_CopyAndFillBytes(const uint8_t *src, uint8_t *dest, uint16_t srclen, const uint8_t value, uint16_t destlen)
{
  destlen-=srclen; // TODO: if once some calling context will be unknown, eventuall check if destlen>srclen

  while(srclen>0u)
  {
    *dest = *src;
    --srclen;
    dest++;
    src++;
  }

  while(destlen>0u)
  {
    *dest = value;
    --destlen;
    dest++;
  }

}
/* PRQA S 1532 -- */ 
/***************************************************************************//**
 * TODO: Concrete implementation description.
 *
 ******************************************************************************/
/* returns true, if arrays are equal */
/* PRQA S 1532 ++ #justification: this is a function with library character and intended to be used outside of this project. */
bool_t el_CompareBytes(const uint8_t * a, const uint8_t * b, uint16_t size)
{
  bool_t retVal = TRUE;
  while (0u < size)
  {
    size--;
    if (*a != *b)
    {
      retVal = FALSE;
    }
    a++;
    b++;
  }
  return retVal;
}
/* PRQA S 1532 -- */
