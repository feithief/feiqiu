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
* \file     cmsis_misra.h
*
* \brief    CMSIS Intrinsics access
*
* \version  V1.0.2
* \date     07. Jul 2025
*
* \note This file violates [MISRA Rule 20] and [MISRA Rule 71]
*/

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** JO           Julia Ott                                                     **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.2: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

#ifndef _CMSIS_MISRA_H
#define _CMSIS_MISRA_H

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "types.h"
#include "core_cm23.h"

/*******************************************************************************
**                           Unit Test Declarations                           **
*******************************************************************************/

#if defined(TESTING)

sint32 CMSIS_Irq_Dis(void);
void CMSIS_Irq_En(void);
void CMSIS_NOP(void);
void CMSIS_WFE(void);
void CMSIS_SEV(void);

#else

/*******************************************************************************
**                     Global Inline Function Declarations                    **
*******************************************************************************/

/** \brief Access to the CMSIS intrinsic __disable_irq().
 *  \note This function violates [MISRA Rule 20] and [MISRA Rule 71]
 */
__STATIC_INLINE sint32 CMSIS_Irq_Dis(void);

/** \brief Access to the CMSIS intrinsic __enable_irq().
 *  \note This function violates [MISRA Rule 20] and [MISRA Rule 71]
 */
__STATIC_INLINE void CMSIS_Irq_En(void);

/** \brief Access to the CMSIS intrinsic __NOP().
 *  \note This function violates [MISRA Rule 20] and [MISRA Rule 71]
 */
__STATIC_INLINE void CMSIS_NOP(void);

/** \brief Access to the CMSIS intrinsic __WFE().
 *  \note This function violates [MISRA Rule 20] and [MISRA Rule 71]
 */
__STATIC_INLINE void CMSIS_WFE(void);

/** \brief Access to the CMSIS intrinsic __SEV().
 *  \note This function violates [MISRA Rule 20] and [MISRA Rule 71]
 */
__STATIC_INLINE void CMSIS_SEV(void);


/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/

__STATIC_INLINE sint32 CMSIS_Irq_Dis(void)
{
  /* violation: Symbol '__disable_irq' undeclared, assumed to return int [MISRA Rule 20], [MISRA Rule 71]*/
  /* violation: call to function '__disable_irq()' not made in the presence of a prototype [MISRA Rule 71] */
  __disable_irq();
  return 0;
}

__STATIC_INLINE void CMSIS_Irq_En(void)
{
  /* violation: Symbol '__enable_irq' undeclared, assumed to return int [MISRA Rule 20], [MISRA Rule 71]*/
  /* violation: call to function '__enable_irq()' not made in the presence of a prototype [MISRA Rule 71] */
  __enable_irq();
}

__STATIC_INLINE void CMSIS_NOP(void)
{
  /* violation: Symbol '__nop' undeclared, assumed to return int [MISRA Rule 20], [MISRA Rule 71]*/
  /* violation: call to function '__nop()' not made in the presence of a prototype [MISRA Rule 71] */
  __NOP();
}

__STATIC_INLINE void CMSIS_WFE(void)
{
  /* violation: Symbol '__wfe' undeclared, assumed to return int [MISRA Rule 20], [MISRA Rule 71]*/
  /* violation: call to function '__wfe()' not made in the presence of a prototype [MISRA Rule 71] */
  __WFE();
}

__STATIC_INLINE void CMSIS_SEV(void)
{
  /* violation: Symbol '__sev' undeclared, assumed to return int [MISRA Rule 20], [MISRA Rule 71]*/
  /* violation: call to function '__sev()' not made in the presence of a prototype [MISRA Rule 71] */
  __SEV();
}

#endif


#endif /*_CMSIS_MISRA_H*/
