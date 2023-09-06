/***************************************************************************//**
 * @file      el_helper.h
 *
 * @creator   sbai
 * @created   06.10.2014
 *
 * @brief     Provides helper macros
 *
 * @purpose
 *
 * Provides helper functions for Elmos's CPU APIs and development
 * on ELMOS'S intelligent ICs.
 *
 * $Id: el_helper.h 30 2014-11-12 13:20:06Z sbai $
 *
 * $Revision: 30 $
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions:
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

#ifndef EL_HELPER_H_
#define EL_HELPER_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"

#if defined ( __ICC430__ )
  /* for IAR H430 compiler */
  #include "el_helper_H430_IAR.h"
  
#elif defined ( __ICCARM__ )
  /* for ARM IAR compiler and CM0 */
  #include "el_helper_ACM0_IAR.h"
  
#elif defined ( __CC_ARM )
  /* for Keil compiler V5 armcc and CM0 */
  #include "el_helper_ACM0_Keil.h"
  
#elif defined ( __GNUC__ )
  #if defined ( CORTEX_M23 )
    /* for compiler GNUC (Keil arm clang V6) and CM23 */
    #include "el_helper_ACM23_GNUC.h"
  #elif defined ( CORTEX_M0 )
    /* for compiler GNUC (Keil arm clang V6) and CM0 */
    #error "not yet existing"
  #else
    #error "Please define CPU_TYPE in preprocessor defines!"
  #endif
  
#else
  #error "Unsupported compiler type"

#endif

#endif /* EL_HELPER_H_ */

