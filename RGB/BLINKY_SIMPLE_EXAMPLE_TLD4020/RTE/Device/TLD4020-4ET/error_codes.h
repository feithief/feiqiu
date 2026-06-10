/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
* \file     error_codes.h
*
* \brief    Error codes definition
*
* \version  V1.0.4
* \date     08. Sep 2025
*
* \note
*/

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** JO           Julia Ott                                                     **
** VO           Vanessa Ongaro                                                **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.2: 2025-02-24, JO:   [ATVCOMETLL-165] Rework error codes              **
** V1.0.3: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.0.4: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
*******************************************************************************/

#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/

/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/
/** \brief Error codes definition
 *
 * \note BSL errors are defines in error codes struct from Firmware \ref status_t
 *
 */
#define ERR_LOG_SUCCESS                     (0x00)  /**< No Error */
#define ERR_LOG_ERROR                       (0x01)  /**< Standard Error */
#define ERR_LOG_CODE_MODULE_DISABLED_IN_CW  (0x02)  /**< One or more modules disabled in Config Wizard */
#define ERR_LOG_CODE_TIMEOUT                (0x04)  /**< Timeout */
#define ERR_LOG_CODE_PARAM_INVALID          (0x08)  /**< Invalid parameter */
#define ERR_LOG_CODE_PARAM_OUT_OF_RANGE     (0x10)  /**< Parameter out of range */
#define ERR_LOG_CODE_NO_TRIGGER             (0x20)  /**< No watchdog trigger */
#define ERR_LOG_CODE_SOW_DISABLED           (0x40)  /**< No watchdog Short Open Window activated */

#endif /* _ERROR_CODES_H_ */

