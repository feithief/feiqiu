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
* \file     gpio.h
*
* \brief    GPIO low level access library
*
* \version  V1.1.0
* \date     23. Oct 2025
*
*/

/** \addtogroup GPIO_api
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
** KP           Krina Panagou                                                 **
*******************************************************************************/


/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.2: 2024-03-19, SL:   [ATVCOMETLL-157] Fixed configuration of output   **
**                           init value                                       **
** V1.0.3: 2024-07-18, VO:   [ATVCOMETLL-150] Added missing init of PDISC.reg **
**                           Added GPIO INLINE functions to set output state  **
** V1.0.4: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.5: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V1.0.6: 2025-04-08, VO:   [ATVCOMETLL-117] Added enum for GPIO numbers     **
** V1.0.7: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.8: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.0.9: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
** V1.1.0: 2025-10-23, KP:   [ATVCOMETLL-217] Added support for TLD4030_12ES  **
*******************************************************************************/

#if !defined(_GPIO_H_)
#define _GPIO_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "gpio_defines.h"

/*******************************************************************************
**                          Global Type Declarations                          **
*******************************************************************************/
/** \enum GPIO_STATE
 *  \brief Enum for the GPIO Output bit
 *  \note You can use this type via 'tGPIO_STATE' or 'enum GPIO_STATE'
 */
typedef enum GPIO_STATE
{
  GPIO_STATE_HIGH =   0x00000001,
  GPIO_STATE_LOW =    0x00010000,
  GPIO_STATE_TOGGLE = 0x00010001
} tGPIO_STATE;

/** \enum GPIO_NO
 *  \brief This enum lists the available gpios
 */
typedef enum GPIO_NO
{
  GPIO_NO_0 = 0u,
  GPIO_NO_1 = 1u,
#if (UC_DEVICE == 403012)
  GPIO_NO_3 = 3u,
  GPIO_NO_4 = 4u
#endif
} tGPIO_NO;

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/
uint8_t GPIO_init(void);
INLINE void GPIO_setP00State(tGPIO_STATE e_value);
INLINE void GPIO_setP01State(tGPIO_STATE e_value);
#if (UC_DEVICE == 403012)
INLINE void GPIO_setP03State(tGPIO_STATE e_value);
INLINE void GPIO_setP04State(tGPIO_STATE e_value);
#endif


/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/
/** \brief Set Pin00 state, if Pin00 is configured as output
 *
 *  \param e_value Pin00 state
 */
INLINE void GPIO_setP00State(tGPIO_STATE e_value)
{
  GPIO->OMR0.reg = (uint32_t)e_value;
}

/** \brief Set Pin01 state, if Pin01 is configured as output
 *
 *  \param e_value Pin01 state
 */
INLINE void GPIO_setP01State(tGPIO_STATE e_value)
{
  GPIO->OMR0.reg = (uint32_t)e_value << GPIO_OMR0_PS1_Pos;
}

#if (UC_DEVICE == 403012)
/** \brief Set Pin03 state, if Pin03 is configured as output
 *
 *  \param e_value Pin03 state
 */
INLINE void GPIO_setP03State(tGPIO_STATE e_value)
{
  GPIO->OMR0.reg = (uint32_t)e_value << GPIO_OMR0_PS3_Pos;
}

/** \brief Set Pin04 state, if Pin04 is configured as output
 *
 *  \param e_value Pin04 state
 */
INLINE void GPIO_setP04State(tGPIO_STATE e_value)
{
  GPIO->OMR0.reg = (uint32_t)e_value << GPIO_OMR0_PS4_Pos;
}
#endif


#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_GPIO_H_)*/
