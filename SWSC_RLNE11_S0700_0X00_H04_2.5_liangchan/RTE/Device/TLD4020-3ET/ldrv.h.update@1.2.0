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
* \file     ldrv.h
*
* \brief    Power Stage low level access library
*
* \version  V1.2.0
* \date     10. Oct 2025
*
*/

/** \addtogroup LDRV_api
*  @{
*/

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** VO           Vanessa Ongaro                                                **
** JO           Julia Ott                                                     **
*******************************************************************************/


/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-08-07, VO:   [ATVCOMETLL-119] Added init of PWM_SYNCH reg     **
** V1.0.2: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.3: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.4: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V1.0.5: 2025-02-25, VO:   [ATVCOMETLL-117] Added pwm dc setter functions   **
** V1.0.6: 2025-04-02, VO:   [ATVCOMETLL-145] Added comp funcs and getters    **
**                           for diagnostics                                  **
** V1.0.7: 2025-04-29, VO:   [ATVCOMETLL-190] Fix MISRA violations            **
** V1.0.8: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.9: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.1.0: 2025-08-14, VO:   [ATVCOMETLL-189] Adjusted init sequence          **
** V1.1.1: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
** V1.1.2: 2025-10-06, VO:   [ATVCOMETLL-226] Fix inaccurate rounding         **
** V1.2.0: 2025-10-10, VO:   [ATVCOMETLL-217] Added support for TLD4030_12ES  **
*******************************************************************************/

#if !defined(_LDRV_H_)
#define _LDRV_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "ldrv_defines.h"
#include "gpio.h"

/*******************************************************************************
**                          Global Type Declarations                          **
*******************************************************************************/
/** \enum LDRV_CHANNEL
 *  \brief This enum lists the LDRV channels
 */
typedef enum LDRV_CHANNEL
{
  LDRV_CHANNEL_0 = 0u,
  LDRV_CHANNEL_1 = 1u,
  LDRV_CHANNEL_2 = 2u,
#if ((UC_DEVICE == 40204) || (UC_DEVICE == 403012))
  LDRV_CHANNEL_3 = 3u,
#endif
#if (UC_DEVICE == 403012)
  LDRV_CHANNEL_4 = 4u,
  LDRV_CHANNEL_5 = 5u,
  LDRV_CHANNEL_6 = 6u,
  LDRV_CHANNEL_7 = 7u,
  LDRV_CHANNEL_8 = 8u,
  LDRV_CHANNEL_9 = 9u,
  LDRV_CHANNEL_10 = 10u,
  LDRV_CHANNEL_11 = 11u
#endif
} tLDRV_CHANNEL;

/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/
#if (UC_DEVICE == 40203)
#define LDRV_CHNR_MAX       LDRV_CHANNEL_2
#elif (UC_DEVICE == 40204)
#define LDRV_CHNR_MAX       LDRV_CHANNEL_3
#else
#define LDRV_CHNR_MAX       LDRV_CHANNEL_11
#endif

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/
uint8_t LDRV_init(void);
uint8_t LDRV_setPwmDutyCycle(tLDRV_CHANNEL channel, uint16_t u16_duty_cycle);
uint8_t LDRV_setPwmDutyCycle_percent(tLDRV_CHANNEL channel, uint8_t u8_duty_cycle);
uint8_t LDRV_setPwmDutyCycle_milliPercent(tLDRV_CHANNEL channel, uint32_t u32_duty_cycle);
uint32_t LDRV_compensateDiagVsResult(uint32_t u32_adcValue);
uint32_t LDRV_compensateDiagVfwdResult(uint32_t u32_adcValue);
uint32_t LDRV_compensateDiagGpioResult(uint32_t u32_adcValue);
#if (UC_DEVICE == 403012)
uint32_t LDRV_compensateDiagVslResult(uint32_t u32_adcValue);
uint32_t LDRV_compensateDiagVslVfwdResult(uint32_t u32_adcValue);
#endif
uint32_t LDRV_convertDiagVsResult_mV(uint32_t u32_adcValue);
uint32_t LDRV_convertDiagVfwdResult_mV(uint32_t u32_adcValue);
uint32_t LDRV_convertDiagGpioResult_mV(uint32_t u32_adcValue);
#if (UC_DEVICE == 403012)
uint32_t LDRV_convertDiagVslResult_mV(uint32_t u32_adcValue);
uint32_t LDRV_convertDiagVslVfwdResult_mV(uint32_t u32_adcValue);
#endif
uint8_t LDRV_getDiagVs_mV(uint32_t *u32_diag_vs_mv);
uint8_t LDRV_getDiagVfwd_mV(uint32_t *u32_diag_vfwd_mv, tLDRV_CHANNEL channel);
uint8_t LDRV_getDiagGpio_mV(uint32_t *u32_diag_gpio_mv, tGPIO_NO gpio);
#if (UC_DEVICE == 403012)
uint8_t LDRV_getDiagVsl_mV(uint32_t *u32_diag_vsl_mv);
uint8_t LDRV_getDiagVslVfwd_mV(uint32_t *u32_diag_vsl_vfwd_mv);
#endif


#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_LDRV_H_)*/
