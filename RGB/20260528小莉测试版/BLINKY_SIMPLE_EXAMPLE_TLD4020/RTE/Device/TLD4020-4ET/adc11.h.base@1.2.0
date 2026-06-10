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
* \file     adc11.h
*
* \brief    Analog to Digital Converter low level access library
*
* \version  V1.2.0
* \date     23. Oct 2025
*
*/

/** \addtogroup ADC11_api
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
** V1.0.2: 2023-11-29, VO:   [ATVCOMETLL-149] Added ADC ConfigSector comp     **
**                           func for LIN shunt current reading               **
** V1.0.3: 2024-07-11, VO:   [ATVCOMETLL-118] Fixed MISRA violations          **
** V1.0.4: 2024-07-15, VO:   [ATVCOMETLL-87] Added functions to handle ADC    **
**                           conversion                                       **
** V1.0.5: 2024-10-16, VO:   [ATVCOMETLL-141] Added ADC ConfigSector comp     **
**                           func for remaining ADC channels                  **
** V1.0.6: 2024-11-19, VO:   [ATVCOMETLL-170] Fixed MISRA violation           **
** V1.0.7: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V1.0.8: 2025-01-08, VO:   [ATVCOMETLL-177] Fixed doxy comments             **
** V1.0.9: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V1.1.0: 2025-02-25, VO:   [ATVCOMETLL-185] Fixed compensation functions    **
** V1.1.1: 2025-04-02, VO:   [ATVCOMETLL-145] Moved comp funcs to according   **
**                           modules. Added ADC related comp funcs            **
** V1.1.1: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.1.2: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.1.3: 2025-08-14, VO:   [ATVCOMETLL-189] Add channel abstraction defines **
** V1.1.4: 2025-09-08, VO:   [ATVCOMETLL-223] Fix formatting                  **
** V1.1.5: 2025-10-02, VO:   [ATVCOMETLL-226] Fix resolution macros and       **
**                                            inaccurate rounding             **
** V1.2.0: 2025-10-23, KP:   [ATVCOMETLL-217] Added support for TLD4030_12ES  **
*******************************************************************************/

#if !defined(_ADC11_H_)
#define _ADC11_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "adc11_defines.h"

/*******************************************************************************
**                          Global Type Declarations                          **
*******************************************************************************/
/** \brief Struct for ADC11 post calibration data at sector 0, page 22 */
typedef struct
{
  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFD84) sector0_page22.ADC_CAL0 */
    struct
    {
      __IM  uint32_t HV_PCSW_GAIN      :  8;     /*!< [7..0] Post calibration Gain of ADC channel for VS */
      uint32_t                         : 24;
    } bit;
  } CAL0;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFD88) sector0_page22.ADC_CAL1 */
    struct
    {
      __IM  uint32_t  HV_PCSW_OFFS      :  8;    /*!< [7..0] Post calibration Offset of ADC channel for VS */
      uint32_t                          : 24;
    } bit;
  } CAL1;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFD8C) sector0_page22.ADC_CAL2 */
    struct
    {
      __IM  uint32_t HVFWD_PCSW_GAIN   :  8;     /*!< [7..0] Post calibration Gain of ADC channels for V_forward LEDx */
      uint32_t                         : 24;
    } bit;
  } CAL2;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFD90) sector0_page22.ADC_CAL3 */
    struct
    {
      __IM  uint32_t  HVFWD_PCSW_OFFS   :  8;    /*!< [7..0] Post calibration Offset of ADC channels for V_forward LEDx */
      uint32_t                          : 24;
    } bit;
  } CAL3;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFD94) sector0_page22.ADC_CAL4 */
    struct
    {
      __IM  uint32_t MV_PCSW_GAIN      :  8;     /*!< [7..0] Post calibration Gain of ADC channels for GPIOx */
      uint32_t                         : 24;
    } bit;
  } CAL4;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFD98) sector0_page22.ADC_CAL5 */
    struct
    {
      __IM  uint32_t  MV_PCSW_OFFS      :  8;    /*!< [7..0] Post calibration Offset of ADC channels for GPIOx */
      uint32_t                          : 24;
    } bit;
  } CAL5;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFD9C) sector0_page22.ADC_CAL6 */
    struct
    {
      __IM  uint32_t LVAA_PCSW_GAIN    :  8;     /*!< [7..0] Post calibration Gain of ADC channels for diff for LIN AutoAddressing */
      uint32_t                         : 23;
      __IM  uint32_t LVAA_PCSW_CHK     :  1;     /*!< [31..31] Post calibration guard, diff for LIN AutoAddressing check */
    } bit;
  } CAL6;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFDA0) sector0_page22.ADC_CAL7 */
    struct
    {
      __IM  uint32_t  LVAA_PCSW_OFFS    :  8;    /*!< [7..0] Post calibration Offset of ADC channels for diff for LIN AutoAddressing */
      uint32_t                          : 24;
    } bit;
  } CAL7;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFDA4) sector0_page22.ADC_CAL8 */
    struct
    {
      __IM  uint32_t LVDTS_PCSW_GAIN   :  8;     /*!< [7..0] Post calibration Gain of ADC channel for Temperature sensor */
      uint32_t                         : 24;
    } bit;
  } CAL8;

  union
  {
    __IM  uint32_t reg;                          /*!< (@ 0x11FFFDA8) sector0_page22.ADC_CAL9 */
    struct
    {
      __IM  uint32_t  LVDTS_PCSW_OFFS   :  8;    /*!< [7..0] Post calibration Offset of ADC channel for Temperature sensor */
      uint32_t                          : 24;
    } bit;
  } CAL9;
} ADC11_CAL_Type;


/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/
#ifndef UNIT_TESTING_CMT
#define ADC11_CAL_BASE            0x11FFFD84
#define ADC11_CAL                 ((ADC11_CAL_Type*) ADC11_CAL_BASE)
#else
extern ADC11_CAL_Type             *ADC11_CAL;
#endif /* UNIT_TESTING_CMT */

/** \brief ADC11 channel abstraction */
#define ADC11_CH_VS               ADC11_CFG_1_ADC_CHNR_CH_0
#if (UC_DEVICE == 40203)
#define ADC11_CH_VFWD_LED0        ADC11_CFG_1_ADC_CHNR_CH_1
#define ADC11_CH_VFWD_LED1        ADC11_CFG_1_ADC_CHNR_CH_2
#define ADC11_CH_VFWD_LED2        ADC11_CFG_1_ADC_CHNR_CH_3
#define ADC11_CH_GPIO0            ADC11_CFG_1_ADC_CHNR_CH_4
#define ADC11_CH_GPIO1            ADC11_CFG_1_ADC_CHNR_CH_5
#define ADC11_CH_LINAA1           ADC11_CFG_1_ADC_CHNR_CH_7
#define ADC11_CH_LINAA2           ADC11_CFG_1_ADC_CHNR_CH_8
#define ADC11_CH_TS               ADC11_CFG_1_ADC_CHNR_CH_9
/** \brief ADC11 maximum channel number */
#define ADC11_CHNR_MAX            ADC11_CFG_1_ADC_CHNR_CH_9
#elif (UC_DEVICE == 40204)
#define ADC11_CH_VFWD_LED0        ADC11_CFG_1_ADC_CHNR_CH_1
#define ADC11_CH_VFWD_LED1        ADC11_CFG_1_ADC_CHNR_CH_2
#define ADC11_CH_VFWD_LED2        ADC11_CFG_1_ADC_CHNR_CH_3
#define ADC11_CH_VFWD_LED3        ADC11_CFG_1_ADC_CHNR_CH_4
#define ADC11_CH_GPIO0            ADC11_CFG_1_ADC_CHNR_CH_6
#define ADC11_CH_GPIO1            ADC11_CFG_1_ADC_CHNR_CH_7
#define ADC11_CH_LINAA1           ADC11_CFG_1_ADC_CHNR_CH_9
#define ADC11_CH_LINAA2           ADC11_CFG_1_ADC_CHNR_CH_10
#define ADC11_CH_TS               ADC11_CFG_1_ADC_CHNR_CH_11
/** \brief ADC11 maximum channel number */
#define ADC11_CHNR_MAX            ADC11_CFG_1_ADC_CHNR_CH_11
#elif (UC_DEVICE == 403012)
#define ADC11_CH_VSL              ADC11_CFG_1_ADC_CHNR_CH_1
#define ADC11_CH_VSL_OUT0         ADC11_CFG_1_ADC_CHNR_CH_2
#define ADC11_CH_VSL_OUT1         ADC11_CFG_1_ADC_CHNR_CH_3
#define ADC11_CH_VSL_OUT2         ADC11_CFG_1_ADC_CHNR_CH_4
#define ADC11_CH_VSL_OUT3         ADC11_CFG_1_ADC_CHNR_CH_5
#define ADC11_CH_VSL_OUT4         ADC11_CFG_1_ADC_CHNR_CH_6
#define ADC11_CH_VSL_OUT5         ADC11_CFG_1_ADC_CHNR_CH_7
#define ADC11_CH_VSL_OUT6         ADC11_CFG_1_ADC_CHNR_CH_8
#define ADC11_CH_VSL_OUT7         ADC11_CFG_1_ADC_CHNR_CH_9
#define ADC11_CH_VSL_OUT8         ADC11_CFG_1_ADC_CHNR_CH_10
#define ADC11_CH_VSL_OUT9         ADC11_CFG_1_ADC_CHNR_CH_11
#define ADC11_CH_VSL_OUT10        ADC11_CFG_1_ADC_CHNR_CH_12
#define ADC11_CH_VSL_OUT11        ADC11_CFG_1_ADC_CHNR_CH_13
#define ADC11_CH_GPIO0            ADC11_CFG_1_ADC_CHNR_CH_14
#define ADC11_CH_GPIO1            ADC11_CFG_1_ADC_CHNR_CH_15
#define ADC11_CH_GPIO3           ADC11_CFG_1_ADC_CHNR_CH_16
#define ADC11_CH_GPIO4           ADC11_CFG_1_ADC_CHNR_CH_17
#define ADC11_CH_LINAA1           ADC11_CFG_1_ADC_CHNR_CH_18
#define ADC11_CH_LINAA2           ADC11_CFG_1_ADC_CHNR_CH_19
#define ADC11_CH_TS               ADC11_CFG_1_ADC_CHNR_CH_20
#define ADC11_CH_VREF_POFF        ADC11_CFG_1_ADC_CHNR_CH_21
/** \brief ADC11 maximum channel number */
#define ADC11_CHNR_MAX            ADC11_CFG_1_ADC_CHNR_CH_21
#endif
/** \brief ADC11 full scale voltage for VS in mV */
#define ADC11_SCALE_VS            (39104u)
/** \brief ADC11 full scale voltage for VFWD in mV */
#define ADC11_SCALE_VFWD_7V5      (7531u)
/** \brief ADC11 full scale voltage for VFWD in mV */
#define ADC11_SCALE_VFWD_4V5      (4534u)
/** \brief ADC11 full scale current for LIN AA in uA */
#define ADC11_SCALE_LINAA         (51200u)
/** \brief ADC11 full scale voltage for GPIO in mV */
#define ADC11_SCALE_GPIO          (5046u)
/** \brief ADC11 full scale voltage for TS in mV */
#define ADC11_SCALE_TS            (1222u)
/** \brief ADC11 resolution mask for 11 bit values */
#define ADC11_11BIT_RESOLUTION    (0x800u)
/** \brief ADC11 resolution mask for 12 bit values */
#define ADC11_12BIT_RESOLUTION    (0x1000u)

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/
uint8_t ADC11_init(void);
uint32_t ADC11_applyGainAndOffset_11bit(uint32_t u32_adcValue, sint32 s32_gain, sint8 s8_offset);
uint32_t ADC11_applyGainAndOffset_12bit(uint32_t u32_adcValue, sint32 s32_gain, sint8 s8_offset);
uint32_t ADC11_compensateTsResult(uint32_t u32_adcValue);
sint32 ADC11_convertTsResult_mC(uint32_t u32_adcValue);
sint32 ADC11_getTsResult_mC(void);
uint8_t ADC11_startChConversion(ADC11_CFG_1_ADC_CHNR_Enum e_channel);
uint8_t ADC11_getChResult(uint16_t *u16p_digValue, ADC11_CFG_1_ADC_CHNR_Enum e_channel);
INLINE uint8_t ADC11_getAdcReadySts(void);

/*******************************************************************************
**                        Global Function Definitions                         **
*******************************************************************************/
/** \brief Get ADC ready flag
 *
 * \return uint8_t ADC ready flag
 */
INLINE uint8_t ADC11_getAdcReadySts(void)
{
  return (uint8_t)ADC11->RESULT.bit.ADC_READY;
}


#ifdef __cplusplus
}
#endif

/** @}*/

#endif /*!defined(_ADC11_H_)*/
