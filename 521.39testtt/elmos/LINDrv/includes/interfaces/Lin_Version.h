/***************************************************************************//**
 * @file   Lin_Version.h
 *
 * @creator      RPY
 * @created      2014/03/07
 *
 * @brief  Version defines
 *
 * @purpose
 *
 * Version and history management
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

#ifndef LIN_VERSION_H_
#define LIN_VERSION_H_

/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/

// Numbering suggestion:
// 
// ++Major: Redesign / Major changes
// ++Minor: Backward incompatible API changes
// ++Build: Any change, small/internal bugfixes etc.

#define LIN_VERSION_MAJOR		        1UL   /**< Major version define. */
#define LIN_VERSION_MAJOR_STR	        "1"   /**< Major version string. */
 
#define LIN_VERSION_MINOR		        0UL   /**< Minor version define. */
#define LIN_VERSION_MINOR_STR	        "0"   /**< Minor version string. */

#define LIN_VERSION_BUILD		        17UL  /**< Build version define. */
#define LIN_VERSION_BUILD_STR	        "17"  /**< Build version string. */

#define LIN_VERSION_PROJECT         "LINDrv"  /**< Project name string. */
#define LIN_VERSION_AUTHOR          "Elmos Semiconductor SE" /**< Company name string. */

#define LIN_VERSION_YEAR            "2014" /**< Version year string. */

#define LIN_VERSION_NUMBER			((LIN_VERSION_MAJOR<<24)|(LIN_VERSION_MINOR<<16)|LIN_VERSION_BUILD) /**< Macro to build version number. */
#define LIN_VERSION_STRING			"v"  LIN_VERSION_MAJOR_STR "." LIN_VERSION_MINOR_STR "." LIN_VERSION_BUILD_STR /**< Complete version string. */
#define LIN_VERSION_STRING_LONG		LIN_VERSION_PROJECT " " LIN_VERSION_STRING " (c)" LIN_VERSION_YEAR " " LIN_VERSION_AUTHOR /**< Long version string. */

/* --------------------------------------------------------------------------------------------------------------- *
 * Date      Version    Who   Comment
 * YY/MM/DD  Ma.Mi.Bd
 * --------------------------------------------------------------------------------------------------------------- *
 * 14/08/01  1.0.14     RPY   Dedicated LinDrv versioning started. Initially kept in sync with LinDrvDemo v1.0.14
 * 14/08/05  1.0.15     RPY   Added LIN_GetBusStructSize(), LinBusImp_GetInfo() and LinBusImp_GetState()
                              Added Rx/TxDone Callback return param to control entrance of sleep mode.
 * 14/08/06  1.0.16     RPY   Included C&S modifications to support LIN 2.2 requirements. (also slightly changed API)
 * 14/09/19  1.0.17     SBAI  Removed local static variable for SCI status tracking and adapted LIN_bus module
 *                            to ESW department coding rules with Doxygen compatible comments
 * --------------------------------------------------------------------------------------------------------------- */

#endif /* LIN_VERSION_H_ */
/*! @} */ /* ELMOS_LIN_DRIVER */
