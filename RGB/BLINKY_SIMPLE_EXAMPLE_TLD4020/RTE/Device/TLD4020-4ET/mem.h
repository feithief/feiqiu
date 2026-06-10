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
* \file     mem.h
*
* \brief    Memory low level access library
*
* \version  V1.0.4
* \date     07. Jul 2025
*
*/

/** \addtogroup MEM_api
*  @{
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
** V1.0.2: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.3: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V1.0.4: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

#if !defined(_MEM_H_)
#define _MEM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "mem_defines.h"

uint8_t MEM_init(void);


#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_MEM_H_)*/
