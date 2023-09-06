/***************************************************************************//**
 * @file      LinDrvImp_CompilationConfig.h
 *
 * @creator   poe
 * @created   05.03.2021
 *
 * @brief     This file configures the features of the LIN driver  
 *            for this application.
 *
 * @addtogroup SAMPLE_PROJECT Sample Project
 * @{
 * @addtogroup COMPILE_CONF Compilation Config
 * @{
 * 
 * $Id: LinDrvImp_CompilationConfig.h 9597 2022-01-19 14:32:10Z poe $
 *
 * $Revision: 9597 $
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
 * Disclaimer:
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

#ifndef LINDRVIMP_COMPILATIONCONFIG_H_
#define LINDRVIMP_COMPILATIONCONFIG_H_

#include "LinDrvImp_ROMCompilationConfig.h"


#define LINDRVIMP_USE_SNPD_PROXY (1u)

#define LINSNPDPROXYLED_EXT_IFFUN_STRCT_ACCESS (1u)

/***************************************************************************//**
 * Control access to LIN Driver modules via the global interface functions
 * structs.
 ******************************************************************************/
#define LINDIAGIMP_EXT_IFFUN_STRCT_ACCESS               1u


/* ****************************************************************************/
/*                               LIN DIAG LAYER                              */
/* ****************************************************************************/
#define LINDIAG_USE_DIAG_LAYER               1u

#define LINDIAG_MAX_PHYADR_RDBI_LST_LEN      8u
#define LINDIAG_MAX_FUNADR_RDBI_LST_LEN      3u

#define LINDIAG_MAX_SUPPORTED_TIMER          3u

#define LINDIAG_SUP_ASSIGN_NAD               1u   /* LIN Diagnostic Service - Assign NAD - 0xB0 */
#define LINDIAG_SUP_ASSIGN_FRAME_IDENTIFIER  1u   /* LIN Diagnostic Service - Assign frame identifier - 0xB1 */
#define LINDIAG_SUP_READ_BY_ID               1u   /* LIN Diagnostic Service - Read by Identifier - 0xB2 */
#define LINDIAG_SUP_CONDITIONAL_CHANGE_NAD   1u   /* LIN Diagnostic Service - Conditional Change NAD - 0xB3 */
#define LINDIAG_SUP_DATA_DUMP                0u   /* LIN Diagnostic Service - Data Dump - 0xB4 */
#define LINDIAG_SUP_SAVE_CONFIGURATION       0u   /* LIN Diagnostic Service - Save Configuration - 0xB6 */
#define LINDIAG_SUP_ASSIGN_FRAME_ID_RANGE    1u   /* LIN Diagnostic Service - Assign frame identifier range - 0xB7 */

#define LINDIAG_SUP_DATASTG                  0u
#define LINDIAG_SUP_SNPD                     1u   /* Auto-Addressing (SNPD) */


#define LINDIAG_INIT_DEFAULT_VALUES          1u

#if (LINDIAG_INIT_DEFAULT_VALUES == 1)
#define LINDIAG_DEFAULT_NAD                  1u
#define LINDIAG_DEFAULT_SERIAL_NUMBER        0x55AA669944u

  #if defined(EL_DEVICE_M52139C) || defined(EL_DEVICE_E52139A)
    #define LINDIAG_DEFAULT_SUPPLIERID           0x23u
    #define LINDIAG_DEFAULT_FUNCTIONID           0x0521u
    #define LINDIAG_DEFAULT_VARIANTID            0x39u
  #endif
#endif

#define LINDIAG_MAX_RBI_TBL_CNT              3u



#endif /* LINDRVIMP_COMPILATIONCONFIG_H_ */

/*! @} */ /* M52139C */
/*! @} */ /* ELMOS_LIN_DRIVER */
