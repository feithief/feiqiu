/***************************************************************************//**
 * @file   Lin_Basictypes.h
 *
 * @creator      RPY
 * @created      2013/10/16
 * @sdfv         Automotive Spice or Elmos Flow or Demo Flow
 *
 * @brief  Definitions of basic data types for the Elmos LIN Driver.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 *
 * $Id$
 *
 * $Revision$
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
#ifndef LIN_BASICTYPES_H_
#define LIN_BASICTYPES_H_

/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/
#include "el_types.h"

#include "Lin_Version.h"
#include "LinDrvImp_CompilationConfig.h"

/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/
#if defined(EL_ARCH_H430) && defined(EL_COMPILER_IAR)
#include "Lin_Basictypes_H430_IAR.h"
#elif defined(EL_ARCH_ACM0) && defined(EL_COMPILER_KEIL)
#include "Lin_Basictypes_ACM0_Keil.h"
#elif defined(EL_ARCH_ACM0) && defined(EL_COMPILER_IAR)
#include "Lin_Basictypes_ACM0_IAR.h"
#else
#error "No architecture or compiler defined!"
#endif


#endif /* LIN_BASICTYPES_H_ */
/*! @} */ /* ELMOS_LIN_DRIVER */
