/***************************************************************************//**
 * @file    Lin_EnvDataSizes.h
 *
 * @creator SBAI
 * @created 06.04.2017
 *
 * @brief
 *
 * @purpose
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 *
 * $Id: Lin_EnvDataSizes.h 8305 2021-02-03 13:29:35Z poe $
 *
 * $Revision: 8305 $
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
#ifndef LIN_ENVDATASIZES_H_
#define LIN_ENVDATASIZES_H_

/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/
#include "el_types.h"

/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/
#if defined(EL_ARCH_H430)
#include "Lin_EnvDataSizes_H430.h"
#elif defined(EL_ARCH_ACM0)
#include "Lin_EnvDataSizes_ACM0.h"
#else
#error "No architecture defined!"
#endif

#endif /* LIN_ENVDATASIZES_H_ */

/*! @} ELMOS_LIN_DRIVER */
