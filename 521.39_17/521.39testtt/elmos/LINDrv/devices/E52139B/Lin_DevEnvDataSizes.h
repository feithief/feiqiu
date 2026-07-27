/***************************************************************************//**
 * @file    Lin_EnvDataSizes.h
 *
 * @creator POE
 * @created 2021.11.18
 *
 * @brief
 *
 * @purpose
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup LIN_E52139B E52139B
 * @{
 *
 * $Id: Lin_DevEnvDataSizes.h 9420 2021-11-21 16:08:02Z poe $
 *
 * $Revision: 9420 $
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
#ifndef LIN_DEVENVDATASIZES_H_
#define LIN_DEVENVDATASIZES_H_

/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/
#include "LinDrvImp_CompilationConfig.h"

/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/

/* ****************************************************************************/
/*                                 BUS LAYER                                  */
/* ****************************************************************************/

#if LINBUSIMP_ENABLE_AUTO_ADDR == 1u
  #if LINBUSIMP_ENABLE_AUTO_ADDR_DEBUG == 1u
    #define LINBUSIMP_ENVIRONMENT_DATA_SZE    (114)
  #else
    #define LINBUSIMP_ENVIRONMENT_DATA_SZE    (110)
  #endif
#else
  #define LINBUSIMP_ENVIRONMENT_DATA_SZE      (76)
#endif

#endif /* LIN_DEVENVDATASIZES_H_ */

/*! @} */ /* E52139B */
/*! @} */ /* ELMOS_LIN_DRIVER */
