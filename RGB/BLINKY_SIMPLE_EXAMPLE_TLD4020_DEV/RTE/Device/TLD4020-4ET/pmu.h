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
* \file     pmu.h
*
* \brief    Power Management Unit low level access library
*
* \version  V1.0.5
* \date     07. Jul 2025
*
*/

/** \addtogroup PMU_api
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
** V1.0.2: 2024-07-11, VO:   [ATVCOMETLL-118] Fixed MISRA violations          **
** V1.0.3: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.4: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V1.0.5: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

#if !defined(_PMU_H_)
#define _PMU_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "pmu_defines.h"

void PMU_countWatchdog(void);
uint8_t PMU_init(void);
uint8_t PMU_initWatchdog(void);
uint8_t PMU_serviceWatchdog(void);
uint8_t PMU_serviceWatchdogSOW(void);
void PMU_stopWatchdog(void);


#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_PMU_H_)*/
