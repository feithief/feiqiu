/***************************************************************************//**
 * @file     io_e52139a.h
 *
 * @creator  poe
 * @created  06.10.2014
 *
 * @brief    Provides all basic data type definitions
 *
 * @purpose
 *
 * Provides all basic data type definitions for Elmos's CPU APIs and development
 * on ELMOS'S intelligent ICs.
 *
 * $Id: io_e52139a.h 8389 2021-02-23 09:41:13Z poe $
 *
 * $Revision: 8389 $
 *
 ******************************************************************************/
 
#ifndef IO_E52139A_H_
#define IO_E52139A_H_

#include "el_types.h"

/* ****************************************************************************/
/* ************************ ARCH DEPENDENT INCLUDES ***************************/
/* ****************************************************************************/

#if defined ( EL_COMPILER_KEIL )
  
  /* use Keil package provided headers */
  

#elif defined ( EL_COMPILER_IAR )

  /* use local modified version for IAR */ 
  #include "io_e52139a_IAR.h"
//  #include "el_regs_m52139c_IAR.h"

#elif defined ( EL_COMPILER_GCC )

  /* use local modified version for IAR */
  #include "io_e52139a_IAR.h"
  //#include "el_regs_m52139c_IAR.h"

#else
  #error "Unsupported compiler type"	

#endif
	

#endif /* IO_E2139A_H_ */






