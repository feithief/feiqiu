/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "adc11.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/
/** \brief Initialize the ADC11 module
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t ADC11_init(void)
{
  /*{44210B09-2F0D-42e0-83E3-9C71632BEE6A}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_CODE_MODULE_DISABLED_IN_CW;
#if ((ADC11_CFG_1 & ADC11_CFG_1_ADC_ENABLE_Msk) == ADC11_CFG_1_ADC_ENABLE_Msk)
  u8_returnCode = ERR_LOG_SUCCESS;
  ADC11->CFG_0.reg = (uint32_t) ADC11_CFG_0;
  ADC11->IEN.reg = (uint32_t) ADC11_IEN;
  ADC11->CFG_1.reg = (uint32_t) ADC11_CFG_1;
#if (UC_DEVICE == 403012)
  ADC11->PWROFFLD.reg = (uint32_t) ADC11_PWROFFLD;
#endif
#endif
  return u8_returnCode;
}

/** \brief Apply gain and offset to a 11-bit ADC value
 * \param  u32_adcValue ADC value to be compensated
 * \param  s32_gain Gain to be applied
 * \param  s8_offset Offset to be applied
 * \return uint32_t Compensated value
 *
 */
uint32_t ADC11_applyGainAndOffset_11bit(uint32_t u32_adcValue, sint32 s32_gain, sint8 s8_offset)
{
  sint32 s32_temp;

  if (s32_gain != 0)
  {
    /* compensate gain and adapt to offset scale */
    /* gain is scaled with factor 128, offset is scaled with factor 2 */
    s32_temp = ((sint32)u32_adcValue * s32_gain) / 64;
    s32_temp += (sint32)s8_offset;
    s32_temp = s32_temp / 2;

    /* check for overflow and underflow */
    if (s32_temp > 0x7FF)
    {
      s32_temp = 0x7FF;
    }

    if (s32_temp < 0)
    {
      s32_temp = 0;
    }
  }
  else
  {
    /* device not trimmed - skip procedure */
    s32_temp = (sint32)u32_adcValue;
  }

  return (uint32_t)s32_temp;
}

/** \brief Apply gain and offset to a 12-bit ADC value
 * \param  u32_adcValue ADC value to be compensated
 * \param  s32_gain Gain to be applied
 * \param  s8_offset Offset to be applied
 * \return uint32_t Compensated value
 *
 */
uint32_t ADC11_applyGainAndOffset_12bit(uint32_t u32_adcValue, sint32 s32_gain, sint8 s8_offset)
{
  sint32 s32_temp;

  if (s32_gain != 0)
  {
    /* compensate gain and adapt to offset scale */
    /* gain is scaled with factor 128, offset and adc value are scaled with factor 2 */
    s32_temp = ((sint32)u32_adcValue * s32_gain) / 128;
    s32_temp += (sint32)s8_offset;

    /* check for overflow and underflow */
    if (s32_temp > 0xFFF)
    {
      s32_temp = 0xFFF;
    }

    if (s32_temp < 0)
    {
      s32_temp = 0;
    }
  }
  else
  {
    /* device not trimmed - skip procedure */
    s32_temp = (sint32)u32_adcValue;
  }

  return (uint32_t)s32_temp;
}

/** \brief Compensate ADC11 reading with gain and offset stored in Flash,
 *         in order to have a more precise result on the reading of the
 *         Temperature Sensor.
 *         Evaluation of gain and offset is done by Infineon.
 *         Gain and offset are available for the ADC channel for the
 *         Temperature Sensor result.
 * \param  u32_adcValue Value to be compensated
 * \return uint32_t Compensated value
 *
 */
uint32_t ADC11_compensateTsResult(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  sint32 s32_gain;
  sint8 s8_offset;
  s32_gain = (sint32)ADC11_CAL->CAL8.bit.LVDTS_PCSW_GAIN;
  s8_offset = (sint8)ADC11_CAL->CAL9.bit.LVDTS_PCSW_OFFS;
  u32_temp = ADC11_applyGainAndOffset_11bit(u32_adcValue, s32_gain, s8_offset);
  return u32_temp;
}

/** \brief Convert ADC11 reading of temperature sensor (11 bit value) to
 *         milli celsius.
 * \param  u32_adcValue Value to be converted
 * \return sint32 Converted value
 * \note   The resolution of the temperature sensor is about 0.6 mV per bit,
 *         which translates to about 0.238 °C per 2 bits in a range from
 *         -63.750 °C to +226.237 °C
 *
 */
sint32 ADC11_convertTsResult_mC(uint32_t u32_adcValue)
{
  sint64 s64_temp;
  s64_temp = (sint64)u32_adcValue * (sint64)ADC11_SCALE_TS;
  /* Ensure accurate rounding during division */
  s64_temp += (ADC11_11BIT_RESOLUTION >> 1U);
  s64_temp = s64_temp / (sint64)ADC11_11BIT_RESOLUTION;
  s64_temp = ((s64_temp - 100) * 2375 / 10) - 40000;
  return (sint32)s64_temp;
}

/** \brief Get the temperature sensor result in milli degree celsius
 *
 * \return sint32 Temperature sensor result in milli degree celsius
 * \note   Before getting the result, check valid flag
 *
 */
sint32 ADC11_getTsResult_mC(void)
{
  uint32_t u32_temp;
  sint32 s32_temp;
  u32_temp = ADC11_compensateTsResult(ADC11->TS_RESULT.bit.TS_VALUE);
  s32_temp = ADC11_convertTsResult_mC(u32_temp);
  return s32_temp;
}

/** \brief Start the conversion of a channel by software
 *
 * \param e_channel channel number \ref ADC11_CFG_1_ADC_CHNR_Enum
 * \return uint8_t 0: success, !0: error codes
 *
 */
uint8_t ADC11_startChConversion(ADC11_CFG_1_ADC_CHNR_Enum e_channel)
{
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;

  if (e_channel < ADC11_CHNR_MAX)
  {
    /* Select the channel */
    ADC11->CFG_1.bit.ADC_CHNR = (uint8_t)e_channel;
    /* Trigger ADC conversion */
    ADC11->CFG_1.bit.ADC_SOC = 1;
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  return u8_returnCode;
}

/** \brief Get the 12-bit value of the ADC11 digital converted value of selected channel
 *
 * \param  e_channel     digital channel number \ref ADC11_CFG_1_ADC_CHNR_Enum
 * \param  u16p_digValue pointer to 12-bit ADC11 digital converted value of selected channel
 * \return uint8_t 0: success, !0: error codes
 * \note   Before getting the result, check valid flag
 *
 */
uint8_t ADC11_getChResult(uint16_t *u16p_digValue, ADC11_CFG_1_ADC_CHNR_Enum e_channel)
{
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;

  /* Check input parameter validity */
  if ((PtrRangeCheck(u16p_digValue) == true) && ((uint8_t)e_channel == ADC11->RESULT.bit.ADC_CHNR))
  {
    *u16p_digValue = (uint16_t)ADC11->RESULT.bit.ADC_VALUE;
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  return u8_returnCode;
}
