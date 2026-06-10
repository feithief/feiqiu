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
#include "ldrv.h"
#include "adc11.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize the LDRV module
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t LDRV_init(void)
{
  /*{45CE83A7-839F-4ea4-8C52-91DBC6DEAB23}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_CODE_MODULE_DISABLED_IN_CW;
#if ((LDRV_MOD_CFG & LDRV_MOD_CFG_LD_ENABLE_Msk) == LDRV_MOD_CFG_LD_ENABLE_Msk)
  u8_returnCode = ERR_LOG_SUCCESS;
  LDRV->PWM_FREQ.reg = (uint32_t) LDRV_PWM_FREQ;
  LDRV->PWM_SYNCH.reg = (uint32_t) LDRV_PWM_SYNCH;
  LDRV->PWM_PHASE_CFG0.reg = (uint32_t) LDRV_PWM_PHASE_CFG0;
  LDRV->PWM_PHASE_CFG1.reg = (uint32_t) LDRV_PWM_PHASE_CFG1;
  LDRV->PWM_PHASE_CFG2.reg = (uint32_t) LDRV_PWM_PHASE_CFG2;
  LDRV->PWM_DC_CFG0.reg = (uint32_t) LDRV_PWM_DC_CFG0;
  LDRV->PWM_DC_CFG1.reg = (uint32_t) LDRV_PWM_DC_CFG1;
  LDRV->PWM_DC_CFG2.reg = (uint32_t) LDRV_PWM_DC_CFG2;
  LDRV->PWM_ISET_CFG0.reg = (uint32_t) LDRV_PWM_ISET_CFG0;
  LDRV->PWM_ISET_CFG1.reg = (uint32_t) LDRV_PWM_ISET_CFG1;
  LDRV->PWM_ISET_CFG2.reg = (uint32_t) LDRV_PWM_ISET_CFG2;
  LDRV->OSP_CFG.reg = (uint32_t) LDRV_OSP_CFG;
  LDRV->DIAG_CFG.reg = (uint32_t) LDRV_DIAG_CFG;
  LDRV->DIAG_PHASE_CFG0.reg = (uint32_t) LDRV_DIAG_PHASE_CFG0;
  LDRV->DIAG_PHASE_CFG1.reg = (uint32_t) LDRV_DIAG_PHASE_CFG1;
  LDRV->DIAG_PHASE_CFG2.reg = (uint32_t) LDRV_DIAG_PHASE_CFG2;
#if (UC_DEVICE == 40204) || (UC_DEVICE == 403012)
  LDRV->PWM_PHASE_CFG3.reg = (uint32_t) LDRV_PWM_PHASE_CFG3;
  LDRV->PWM_DC_CFG3.reg = (uint32_t) LDRV_PWM_DC_CFG3;
  LDRV->PWM_ISET_CFG3.reg = (uint32_t) LDRV_PWM_ISET_CFG3;
  LDRV->DIAG_PHASE_CFG3.reg = (uint32_t) LDRV_DIAG_PHASE_CFG3;
#endif
#if (UC_DEVICE == 403012)
  LDRV->PWM_PHASE_CFG4.reg = (uint32_t) LDRV_PWM_PHASE_CFG4;
  LDRV->PWM_DC_CFG4.reg = (uint32_t) LDRV_PWM_DC_CFG4;
  LDRV->PWM_ISET_CFG4.reg = (uint32_t) LDRV_PWM_ISET_CFG4;
  LDRV->DIAG_PHASE_CFG4.reg = (uint32_t) LDRV_DIAG_PHASE_CFG4;
  LDRV->PWM_PHASE_CFG5.reg = (uint32_t) LDRV_PWM_PHASE_CFG5;
  LDRV->PWM_DC_CFG5.reg = (uint32_t) LDRV_PWM_DC_CFG5;
  LDRV->PWM_ISET_CFG5.reg = (uint32_t) LDRV_PWM_ISET_CFG5;
  LDRV->DIAG_PHASE_CFG5.reg = (uint32_t) LDRV_DIAG_PHASE_CFG5;
  LDRV->PWM_PHASE_CFG6.reg = (uint32_t) LDRV_PWM_PHASE_CFG6;
  LDRV->PWM_DC_CFG6.reg = (uint32_t) LDRV_PWM_DC_CFG6;
  LDRV->PWM_ISET_CFG6.reg = (uint32_t) LDRV_PWM_ISET_CFG6;
  LDRV->DIAG_PHASE_CFG6.reg = (uint32_t) LDRV_DIAG_PHASE_CFG6;
  LDRV->PWM_PHASE_CFG7.reg = (uint32_t) LDRV_PWM_PHASE_CFG7;
  LDRV->PWM_DC_CFG7.reg = (uint32_t) LDRV_PWM_DC_CFG7;
  LDRV->PWM_ISET_CFG7.reg = (uint32_t) LDRV_PWM_ISET_CFG7;
  LDRV->DIAG_PHASE_CFG7.reg = (uint32_t) LDRV_DIAG_PHASE_CFG7;
  LDRV->PWM_PHASE_CFG8.reg = (uint32_t) LDRV_PWM_PHASE_CFG8;
  LDRV->PWM_DC_CFG8.reg = (uint32_t) LDRV_PWM_DC_CFG8;
  LDRV->PWM_ISET_CFG8.reg = (uint32_t) LDRV_PWM_ISET_CFG8;
  LDRV->DIAG_PHASE_CFG8.reg = (uint32_t) LDRV_DIAG_PHASE_CFG8;
  LDRV->PWM_PHASE_CFG9.reg = (uint32_t) LDRV_PWM_PHASE_CFG9;
  LDRV->PWM_DC_CFG9.reg = (uint32_t) LDRV_PWM_DC_CFG9;
  LDRV->PWM_ISET_CFG9.reg = (uint32_t) LDRV_PWM_ISET_CFG9;
  LDRV->DIAG_PHASE_CFG9.reg = (uint32_t) LDRV_DIAG_PHASE_CFG9;
  LDRV->PWM_PHASE_CFG10.reg = (uint32_t) LDRV_PWM_PHASE_CFG10;
  LDRV->PWM_DC_CFG10.reg = (uint32_t) LDRV_PWM_DC_CFG10;
  LDRV->PWM_ISET_CFG10.reg = (uint32_t) LDRV_PWM_ISET_CFG10;
  LDRV->DIAG_PHASE_CFG10.reg = (uint32_t) LDRV_DIAG_PHASE_CFG10;
  LDRV->PWM_PHASE_CFG11.reg = (uint32_t) LDRV_PWM_PHASE_CFG11;
  LDRV->PWM_DC_CFG11.reg = (uint32_t) LDRV_PWM_DC_CFG11;
  LDRV->PWM_ISET_CFG11.reg = (uint32_t) LDRV_PWM_ISET_CFG11;
  LDRV->DIAG_PHASE_CFG11.reg = (uint32_t) LDRV_DIAG_PHASE_CFG11;
#endif
  LDRV->IEN0.reg = (uint32_t) LDRV_IEN0;
  LDRV->IEN1.reg = (uint32_t) LDRV_IEN1;
  LDRV->MOD_CFG.reg = (uint32_t) LDRV_MOD_CFG;
#endif
  return u8_returnCode;
}

/** \brief Set the duty cycle of a PWM channel
 *
 * \param channel        LDRV channel \ref tLDRV_channel
 * \param u16_duty_cycle Duty cycle as register value
 * \return uint8_t 0: success, !0: error codes
 *
 */
uint8_t LDRV_setPwmDutyCycle(tLDRV_CHANNEL channel, uint16_t u16_duty_cycle)
{
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;

  switch (channel)
  {
    case LDRV_CHANNEL_0:
      LDRV->PWM_DC_CFG0.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_1:
      LDRV->PWM_DC_CFG1.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_2:
      LDRV->PWM_DC_CFG2.bit.PWM_DC = u16_duty_cycle;
      break;
#if (UC_DEVICE == 40204)||(UC_DEVICE == 403012)

    case LDRV_CHANNEL_3:
      LDRV->PWM_DC_CFG3.bit.PWM_DC = u16_duty_cycle;
      break;
#endif
#if (UC_DEVICE == 403012)

    case LDRV_CHANNEL_4:
      LDRV->PWM_DC_CFG4.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_5:
      LDRV->PWM_DC_CFG5.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_6:
      LDRV->PWM_DC_CFG6.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_7:
      LDRV->PWM_DC_CFG7.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_8:
      LDRV->PWM_DC_CFG8.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_9:
      LDRV->PWM_DC_CFG9.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_10:
      LDRV->PWM_DC_CFG10.bit.PWM_DC = u16_duty_cycle;
      break;

    case LDRV_CHANNEL_11:
      LDRV->PWM_DC_CFG11.bit.PWM_DC = u16_duty_cycle;
      break;
#endif

    default:
      u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
      break;
  }

  return u8_returnCode;
}

/** \brief Set the duty cycle of a PWM channel in percent
 *
 * \param channel       LDRV channel \ref tLDRV_channel
 * \param u8_duty_cycle Duty cycle in percent (0 .. 100)
 * \return uint8_t 0: success, !0: error codes
 *
 */
uint8_t LDRV_setPwmDutyCycle_percent(tLDRV_CHANNEL channel, uint8_t u8_duty_cycle)
{
  uint16_t pwm_reg_value;
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;
  pwm_reg_value = (uint16_t)(((uint32_t)u8_duty_cycle * (uint32_t)LDRV->PWM_FREQ.bit.PWM_MAX) / 100u);
  u8_returnCode = LDRV_setPwmDutyCycle(channel, pwm_reg_value);
  return u8_returnCode;
}

/** \brief Set the duty cycle of a PWM channel in milli percent
 *
 * \param channel        LDRV channel \ref tLDRV_channel
 * \param u32_duty_cycle Duty cycle in milli percent (0 .. 100,000)
 * \return uint8_t 0: success, !0: error codes
 *
 */
uint8_t LDRV_setPwmDutyCycle_milliPercent(tLDRV_CHANNEL channel, uint32_t u32_duty_cycle)
{
  uint16_t pwm_reg_value;
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;
  pwm_reg_value = (uint16_t)(((uint64)u32_duty_cycle * (uint64)LDRV->PWM_FREQ.bit.PWM_MAX) / 100000u);
  u8_returnCode = LDRV_setPwmDutyCycle(channel, pwm_reg_value);
  return u8_returnCode;
}

/** \brief Compensate ADC11 reading with gain and offset stored in Flash,
 *         in order to have a more precise result on the reading of VS.
 *         Evaluation of gain and offset is done by Infineon.
 *         Gain and offset are available for the ADC channel for LED Driver
 *         Diagnostic VS result.
 * \param  u32_adcValue Value to be compensated
 * \return uint32_t Compensated value
 *
 */
uint32_t LDRV_compensateDiagVsResult(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  sint32 s32_gain;
  sint8 s8_offset;
  s32_gain = (sint32)ADC11_CAL->CAL0.bit.HV_PCSW_GAIN;
  s8_offset = (sint8)ADC11_CAL->CAL1.bit.HV_PCSW_OFFS;
  u32_temp = ADC11_applyGainAndOffset_11bit(u32_adcValue, s32_gain, s8_offset);
  return u32_temp;
}

/** \brief Compensate ADC11 reading with gain and offset stored in Flash,
 *         in order to have a more precise result on the reading of LED
 *         Vforward.
 *         Evaluation of gain and offset is done by Infineon.
 *         Gain and offset are available for the ADC channels for all LED
 *         Driver Diagnostic VFWDx results.
 * \param  u32_adcValue Value to be compensated
 * \return uint32_t Compensated value
 *
 */
uint32_t LDRV_compensateDiagVfwdResult(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  sint32 s32_gain;
  sint8 s8_offset;
  s32_gain = (sint32)ADC11_CAL->CAL2.bit.HVFWD_PCSW_GAIN;
  s8_offset = (sint8)ADC11_CAL->CAL3.bit.HVFWD_PCSW_OFFS;
  u32_temp = ADC11_applyGainAndOffset_11bit(u32_adcValue, s32_gain, s8_offset);
  return u32_temp;
}

/** \brief Compensate ADC11 reading with gain and offset stored in Flash,
 *         in order to have a more precise result on the reading of GPIOx.
 *         Evaluation of gain and offset is done by Infineon.
 *         Gain and offset are available for the ADC channels for all LED
 *         Driver Diagnostic VGPIOx results.
 * \param  u32_adcValue Value to be compensated
 * \return uint32_t Compensated value
 *
 */
uint32_t LDRV_compensateDiagGpioResult(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  sint32 s32_gain;
  sint8 s8_offset;
  s32_gain = (sint32)ADC11_CAL->CAL4.bit.MV_PCSW_GAIN;
  s8_offset = (sint8)ADC11_CAL->CAL5.bit.MV_PCSW_OFFS;
  u32_temp = ADC11_applyGainAndOffset_11bit(u32_adcValue, s32_gain, s8_offset);
  return u32_temp;
}

#if (UC_DEVICE == 403012)

/** \brief Compensate ADC11 reading with gain and offset stored in Flash,
 *         in order to have a more precise result on the reading of VSL.
 *         Evaluation of gain and offset is done by Infineon.
 * \param  u32_adcValue Value to be compensated
 * \return uint32_t Compensated value
 *
 */
uint32_t LDRV_compensateDiagVslResult(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  sint32 s32_gain;
  sint8 s8_offset;
  s32_gain = (sint32)ADC11_CAL->CAL0.bit.HV_PCSW_GAIN;
  s8_offset = (sint8)ADC11_CAL->CAL1.bit.HV_PCSW_OFFS;
  u32_temp = ADC11_applyGainAndOffset_11bit(u32_adcValue, s32_gain, s8_offset);
  return u32_temp;
}

/** \brief Compensate ADC11 reading with gain and offset stored in Flash,
 *         in order to have a more precise result on the reading of VSL_VFWD.
 *         Evaluation of gain and offset is done by Infineon.
 * \param  u32_adcValue Value to be compensated
 * \return uint32_t Compensated value
 *
 */
uint32_t LDRV_compensateDiagVslVfwdResult(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  sint32 s32_gain;
  sint8 s8_offset;
  s32_gain = (sint32)ADC11_CAL->CAL0.bit.HV_PCSW_GAIN;
  s8_offset = (sint8)ADC11_CAL->CAL1.bit.HV_PCSW_OFFS;
  u32_temp = ADC11_applyGainAndOffset_11bit(u32_adcValue, s32_gain, s8_offset);
  return u32_temp;
}

#endif /* (UC_DEVICE == 403012) */

/** \brief Convert ADC11 reading of VS triggered by LDRV Diagnostics to mV
 *
 * \param  u32_adcValue Value to be converted
 * \return uint32_t Converted value
 * \note   The resolution of the VS voltage is about 19.1 mV per bit
 *
 */
uint32_t LDRV_convertDiagVsResult_mV(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  u32_temp = (uint32_t)u32_adcValue * ADC11_SCALE_VS;
  /* Ensure accurate rounding during division */
  u32_temp += (ADC11_11BIT_RESOLUTION >> 1U);
  u32_temp = u32_temp / ADC11_11BIT_RESOLUTION;
  return (uint32_t)u32_temp;
}

/** \brief Convert ADC11 reading of Vfwd triggered by LDRV Diagnostics to mV
 *
 * \param  u32_adcValue Value to be converted
 * \return uint32_t Converted value
 * \note   The resolution of the Vforward voltage depends on the selected
 *         fullscale range for differential conversion
 *         8V fullscale: 3.7 mV per bit
 *         4V fullscale: 2.2 mV per bit
 *
 */
uint32_t LDRV_convertDiagVfwdResult_mV(uint32_t u32_adcValue)
{
  uint32_t u32_temp;

  if (ADC11->CFG_0.bit.ADC_SEL_4V == (uint32_t)ADC11_CFG_0_ADC_SEL_4V_FS_4V)
  {
    u32_temp = (uint32_t)u32_adcValue * ADC11_SCALE_VFWD_4V5;
  }
  else
  {
    u32_temp = (uint32_t)u32_adcValue * ADC11_SCALE_VFWD_7V5;
  }

  /* Ensure accurate rounding during division */
  u32_temp += (ADC11_11BIT_RESOLUTION >> 1U);
  u32_temp = u32_temp / ADC11_11BIT_RESOLUTION;
  return (uint32_t)u32_temp;
}

/** \brief Convert ADC11 reading of GPIO triggered by LDRV Diagnostics to mV
 *
 * \param  u32_adcValue Value to be converted
 * \return uint32_t Converted value
 * \note   The resolution of the GPIO voltage is about 2.5 mV per bit
 *
 */
uint32_t LDRV_convertDiagGpioResult_mV(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  u32_temp = (uint32_t)u32_adcValue * ADC11_SCALE_GPIO;
  /* Ensure accurate rounding during division */
  u32_temp += (ADC11_11BIT_RESOLUTION >> 1U);
  u32_temp = u32_temp / ADC11_11BIT_RESOLUTION;
  return (uint32_t)u32_temp;
}

#if (UC_DEVICE == 403012)

/** \brief Convert ADC11 reading of VSL triggered by LDRV Diagnostics to mV
 *
 * \param  u32_adcValue Value to be converted
 * \return uint32_t Converted value
 * \note   The resolution of the VSL voltage is about 19.1 mV per bit (same as VS)
 *
 */
uint32_t LDRV_convertDiagVslResult_mV(uint32_t u32_adcValue)
{
  uint32_t u32_temp;
  u32_temp = (uint32_t)u32_adcValue * ADC11_SCALE_VS;
  /* Ensure accurate rounding during division */
  u32_temp += (ADC11_11BIT_RESOLUTION >> 1U);
  u32_temp = u32_temp / ADC11_11BIT_RESOLUTION;
  return (uint32_t)u32_temp;
}

/** \brief Convert ADC11 reading of VSL_VFWD triggered by LDRV Diagnostics to mV
 *
 * \param  u32_adcValue Value to be converted
 * \return uint32_t Converted value
 * \note   The resolution of the VSL_VFWD voltage depends on the selected
 *         fullscale range for differential conversion (4V or 8V)
 *
 */
uint32_t LDRV_convertDiagVslVfwdResult_mV(uint32_t u32_adcValue)
{
  uint32_t u32_temp;

  if (ADC11->CFG_0.bit.ADC_SEL_4V == (uint32_t)ADC11_CFG_0_ADC_SEL_4V_FS_4V)
  {
    u32_temp = (uint32_t)u32_adcValue * ADC11_SCALE_VFWD_4V5;
  }
  else
  {
    u32_temp = (uint32_t)u32_adcValue * ADC11_SCALE_VFWD_7V5;
  }

  /* Ensure accurate rounding during division */
  u32_temp += (ADC11_11BIT_RESOLUTION >> 1U);
  u32_temp = u32_temp / ADC11_11BIT_RESOLUTION;
  return (uint32_t)u32_temp;
}

#endif /* (UC_DEVICE == 403012) */

/** \brief Get the LDRV diagnostics result of VS in mV
 *
 * \param  u32_diag_vs_mv Pointer to store the result
 * \return uint8_t 0: success, !0: error codes
 * \note   Before getting the result, check valid flag
 *
 */
uint8_t LDRV_getDiagVs_mV(uint32_t *u32_diag_vs_mv)
{
  uint32_t u32_temp;
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;

  if (PtrRangeCheck(u32_diag_vs_mv) == true)
  {
    u32_temp = LDRV_compensateDiagVsResult(LDRV->DIAG_VS.bit.VS);
    u32_temp = LDRV_convertDiagVsResult_mV(u32_temp);
    *u32_diag_vs_mv = u32_temp;
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  return u8_returnCode;
}

/** \brief Get the LDRV diagnostics result of Vfwd in mV
 *
 * \param  u32_diag_vfwd_mv Pointer to store the result
 * \param  channel          LDRV channel \ref tLDRV_channel
 * \return uint8_t 0: success, !0: error codes
 * \note   Before getting the result, check valid flag
 *
 */
uint8_t LDRV_getDiagVfwd_mV(uint32_t *u32_diag_vfwd_mv, tLDRV_CHANNEL channel)
{
  uint8_t u8_returnCode;
  uint32_t u32_temp;
  u8_returnCode = ERR_LOG_SUCCESS;
  u32_temp = 0;

  if (PtrRangeCheck(u32_diag_vfwd_mv) == true)
  {
    switch (channel)
    {
      case LDRV_CHANNEL_0:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD0.bit.VFWD);
        break;

      case LDRV_CHANNEL_1:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD1.bit.VFWD);
        break;

      case LDRV_CHANNEL_2:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD2.bit.VFWD);
        break;
#if ((UC_DEVICE == 40204) || (UC_DEVICE == 403012))

      case LDRV_CHANNEL_3:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD3.bit.VFWD);
        break;
#endif
#if (UC_DEVICE == 403012)

      case LDRV_CHANNEL_4:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD4.bit.VFWD);
        break;

      case LDRV_CHANNEL_5:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD5.bit.VFWD);
        break;

      case LDRV_CHANNEL_6:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD6.bit.VFWD);
        break;

      case LDRV_CHANNEL_7:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD7.bit.VFWD);
        break;

      case LDRV_CHANNEL_8:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD8.bit.VFWD);
        break;

      case LDRV_CHANNEL_9:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD9.bit.VFWD);
        break;

      case LDRV_CHANNEL_10:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD10.bit.VFWD);
        break;

      case LDRV_CHANNEL_11:
        u32_temp = LDRV_compensateDiagVfwdResult(LDRV->DIAG_VFWD11.bit.VFWD);
        break;
#endif

      default:
        u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
        break;
    }
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  if (u8_returnCode == ERR_LOG_SUCCESS)
  {
    u32_temp = LDRV_convertDiagVfwdResult_mV(u32_temp);
    *u32_diag_vfwd_mv = u32_temp;
  }

  return u8_returnCode;
}

/** \brief Get the LDRV diagnostics result of GPIO in mV
 *
 * \param  u32_diag_gpio_mv Pointer to store the result
 * \param  gpio             Gpio \ref tGPIO_NO
 * \return uint8_t 0: success, !0: error codes
 * \note   Before getting the result, check valid flag
 *
 */
uint8_t LDRV_getDiagGpio_mV(uint32_t *u32_diag_gpio_mv, tGPIO_NO gpio)
{
  uint8_t u8_returnCode;
  uint32_t u32_temp;
  u8_returnCode = ERR_LOG_SUCCESS;
  u32_temp = 0;

  if (PtrRangeCheck(u32_diag_gpio_mv) == true)
  {
    switch (gpio)
    {
      case GPIO_NO_0:
        u32_temp = LDRV_compensateDiagGpioResult(LDRV->DIAG_VGPIO0.bit.VGPIO0);
        break;

      case GPIO_NO_1:
        u32_temp = LDRV_compensateDiagGpioResult(LDRV->DIAG_VGPIO1.bit.VGPIO1);
        break;
#if (UC_DEVICE == 403012)

      case GPIO_NO_3:
        u32_temp = LDRV_compensateDiagGpioResult(LDRV->DIAG_VGPIO3.bit.VGPIO3);
        break;

      case GPIO_NO_4:
        u32_temp = LDRV_compensateDiagGpioResult(LDRV->DIAG_VGPIO4.bit.VGPIO4);
        break;
#endif

      default:
        u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
        break;
    }
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  if (u8_returnCode == ERR_LOG_SUCCESS)
  {
    u32_temp = LDRV_convertDiagGpioResult_mV(u32_temp);
    *u32_diag_gpio_mv = u32_temp;
  }

  return u8_returnCode;
}

#if (UC_DEVICE == 403012)

/** \brief Get the LDRV diagnostics result of VSL in mV
 *
 * \param  u32_diag_vsl_mv Pointer to store the result
 * \return uint8_t 0: success, !0: error codes
 * \note   Before getting the result, check valid flag
 *
 */
uint8_t LDRV_getDiagVsl_mV(uint32_t *u32_diag_vsl_mv)
{
  uint8_t u8_returnCode;
  uint32_t u32_temp;
  u8_returnCode = ERR_LOG_SUCCESS;
  u32_temp = 0;

  if (PtrRangeCheck(u32_diag_vsl_mv) == true)
  {
    u32_temp = LDRV_compensateDiagVslResult(LDRV->DIAG_VSL.bit.VSL);
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  if (u8_returnCode == ERR_LOG_SUCCESS)
  {
    u32_temp = LDRV_convertDiagVslResult_mV(u32_temp);
    *u32_diag_vsl_mv = u32_temp;
  }

  return u8_returnCode;
}

/** \brief Get the LDRV diagnostics result of VSL_VFWD in mV
 *
 * \param  u32_diag_vsl_vfwd_mv Pointer to store the result
 * \return uint8_t 0: success, !0: error codes
 * \note   Before getting the result, check valid flag
 *
 */
uint8_t LDRV_getDiagVslVfwd_mV(uint32_t *u32_diag_vsl_vfwd_mv)
{
  uint8_t u8_returnCode;
  uint32_t u32_temp;
  u8_returnCode = ERR_LOG_SUCCESS;
  u32_temp = 0;

  if (PtrRangeCheck(u32_diag_vsl_vfwd_mv) == true)
  {
    u32_temp = LDRV_compensateDiagVslVfwdResult(LDRV->DIAG_VSL_VFWD.bit.VSL_VFWD);
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  if (u8_returnCode == ERR_LOG_SUCCESS)
  {
    u32_temp = LDRV_convertDiagVslVfwdResult_mV(u32_temp);
    *u32_diag_vsl_vfwd_mv = u32_temp;
  }

  return u8_returnCode;
}

#endif /* (UC_DEVICE == 403012) */