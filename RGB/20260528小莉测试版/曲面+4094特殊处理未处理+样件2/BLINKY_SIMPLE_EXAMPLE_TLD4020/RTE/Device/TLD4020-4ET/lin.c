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
#include "lin.h"
#include "adc11.h"

#if (LIN_UART_STD_EN == 1)
  /* Define __STRICT_ANSI__ for compatibility with ARMCC v5 compiler */
  #ifndef __STRICT_ANSI__
    #define __STRICT_ANSI__
  #endif
  #include <stdio.h>
#endif


/*******************************************************************************
**                                  Defines                                  **
*******************************************************************************/
#define LIN_BAUDRATE_MIN ((uint16_t)1221)
#define LIN_BAUDRATE_MAX ((uint32_t)1250000)

/*******************************************************************************
**                        Private Variable Definitions                        **
*******************************************************************************/

#if (LIN_UART_STD_EN == 1)
  #if defined (__CC_ARM) || defined (__ARMCC_VERSION)
    FILE __stdout;
    FILE __stdin;
  #endif
#endif

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/


/** \brief Initialize the LIN module
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t LIN_init(void)
{
  /*{E812B712-1339-4984-8922-AE33885B6C4A}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_CODE_MODULE_DISABLED_IN_CW;
#if (LIN_T2_CONFIG_EN == 1u)
  LIN->T2_CON.reg = (uint32_t) LIN_T2_CON;
  LIN->T2_MOD.reg = (uint32_t) LIN_T2_MOD;
  LIN->T2_RC.reg = (uint32_t) LIN_T2_RC;
  LIN->T2_CNT.reg = (uint32_t) LIN_T2_CNT;
#endif
#if (LIN_UART_CONFIG_EN == 1u)
  LIN->UART_SCON.reg = (uint32_t) LIN_UART_SCON;
  /* UART_BCON register shall be written only when UART_BCON.BR_R is 0 */
  LIN->UART_BCON.bit.BR_R = (uint8_t) LIN_UART_BCON_BR_R_Disable; /* This is needed in case of a reset type 5 */
  LIN->UART_BCON.reg = (uint32_t) (LIN_UART_BCON & ~LIN_UART_BCON_BR_R_Msk);
  LIN->UART_BCON.reg = (uint32_t) LIN_UART_BCON;
  /* UART_LINCON register shall be written only when UART_BCON.BREN is 0 */
  LIN->UART_LINCON.bit.BREN = (uint8_t) LIN_UART_LINCON_BREN_Disable; /* This is needed in case of a reset type 5 */
  LIN->UART_LINCON.reg = (uint32_t) (LIN_UART_LINCON & (~LIN_UART_LINCON_BREN_Msk));
  LIN->UART_LINCON.reg = (uint32_t) LIN_UART_LINCON;
#endif
#if ((LIN_LIN_CTRL & LIN_LIN_CTRL_EN_Msk) == LIN_LIN_CTRL_EN_Msk)
  /* in order to configure SM and HV_MODE bitfields of LIN_CTRL, LIN shall be put into sleep mode */
  LIN->LIN_CTRL.bit.MODE = (uint8_t)LIN_LIN_CTRL_MODE_LINSleepMode;
  LIN->LIN_CTRL.reg = (uint32_t) ((LIN_LIN_CTRL & ~LIN_LIN_CTRL_MODE_Msk) | ((uint8_t)LIN_LIN_CTRL_MODE_LINSleepMode << LIN_LIN_CTRL_MODE_Pos));
  LIN->LIN_CTRL.reg = (uint32_t) LIN_LIN_CTRL;
  LIN->LIN_IEN.reg = (uint32_t) LIN_LIN_IEN;
  LIN->WAKE_CNF.reg = (uint32_t) LIN_WAKE_CNF;
#endif
  /* Auto-addressing part is still missing in the ConfigWizard */
  /*LIN->AA_STATUS.reg = (uint32_t) LIN_AA_STATUS;*/
  /*LIN->AA_FW_CTRL.reg = (uint32_t) LIN_AA_FW_CTRL;*/
  /*LIN->AA_CTRL.reg = (uint32_t) LIN_AA_CTRL;*/
  /*LIN->AA_I_DIFF_TH.reg = (uint32_t) LIN_AA_I_DIFF_TH;*/
  /*LIN->AA_ACC_RES_1.reg = (uint32_t) LIN_AA_ACC_RES_1;*/
  /*LIN->AA_ACC_RES_2.reg = (uint32_t) LIN_AA_ACC_RES_2;*/
#if ( (LIN_T2_CONFIG_EN == 1u) || (LIN_UART_CONFIG_EN == 1u) || ((LIN_LIN_CTRL & LIN_LIN_CTRL_EN_Msk) == LIN_LIN_CTRL_EN_Msk) )
  u8_returnCode = ERR_LOG_SUCCESS;
#endif
  return u8_returnCode;
}

/** \brief Set the baudrate for UART
 *
 * \param u32_baudrate baudrate
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t LIN_setUartBaudrate(uint32_t u32_baudrate)
{
  /*{0BFFE6D1-D014-4de3-992E-802450F5C2FB}*/
  uint16_t u16_brval;
  uint8_t u8_returnCode;

  if ((u32_baudrate >= LIN_BAUDRATE_MIN ) && (u32_baudrate <= LIN_BAUDRATE_MAX ))
  {
    u16_brval = (uint16_t)((((uint32_t)LIN_UARTCLOCK_HZ) << 1u) / u32_baudrate);
    LIN->UART_BCON.bit.BR_R = (uint8_t) LIN_UART_BCON_BR_R_Disable;
    LIN->UART_BCON.bit.BG_BR_VALUE = (u16_brval >> 5u);
    LIN->UART_BCON.bit.BG_FD_SEL = (uint8_t) (u16_brval & (uint16_t)0x1F);
    LIN->UART_BCON.bit.BR_R = (uint8_t) LIN_UART_BCON_BR_R_Enable;
    u8_returnCode = ERR_LOG_SUCCESS;
  }
  else
  {
    u8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }

  return u8_returnCode;
}


/** \brief Get the baudrate for UART
 * \return uint32_t UART baudrate
 */
uint32_t LIN_getUartBaudrate(void)
{
  return (uint32_t)((((uint32_t)LIN_UARTCLOCK_HZ) << 1UL)  / ((1U << (uint8_t)LIN->UART_BCON.bit.BR_PRE) * (32 * LIN->UART_BCON.bit.BG_BR_VALUE + (LIN->UART_BCON.bit.BG_FD_SEL))));
}

/** \brief Compensate ADC reading with gain and offset stored in Flash, in order to have
 *         a more precise result on the LIN shunt current reading.
 *         Evaluation of gain and offset is done by Infineon.
 *         Gain and offset are available for the difference between the two ADC channels
 *         for LIN Auto Addressing.
 * \param  s32_adcDiffValue Difference value to be compensated
 * \return sint32 Compensated value
 *
 * \brief <b>Example</b><br>
 * \brief This example compensates the accumulated ADC results of Auto Addressing phase
 *        measure loop
 * ~~~~~~~~~~~~~~~{.c}
 * sint32 s32_var;
 * sint32 s32_var_comp;
 *
 * s32_var = ((sint32)LIN->AA_ACC_RES_1.bit.ACC_RES_1 - (sint32)LIN->AA_ACC_RES_2.bit.ACC_RES_2) / (sint32)(1u << LIN->AA_CTRL.bit.ISENSE_MEAS_NUMBER);
 * s32_var_comp = LIN_compensateAutoAddrResultDiff(s32_var);
 * ~~~~~~~~~~~~~~~
 */
sint32 LIN_compensateAutoAddrResultDiff(sint32 s32_adcDiffValue)
{
  sint32 s32_temp;
  sint32 s32_gain;
  sint8 s8_offset;
  s32_gain = (sint32)ADC11_CAL->CAL6.bit.LVAA_PCSW_GAIN;
  s8_offset = (sint8)ADC11_CAL->CAL7.bit.LVAA_PCSW_OFFS;

  if ((s32_gain != 0) && (ADC11_CAL->CAL6.bit.LVAA_PCSW_CHK != 0))
  {
    /* compensate gain and adapt to offset scale */
    /* gain is scaled with factor 128, offset and adc value are scaled with factor 2 */
    s32_temp = ((sint32)s32_adcDiffValue * s32_gain) / 128;
    /* compensate offset */
    s32_temp += (sint32)s8_offset;

    /* check for overflow (12 bit) and underflow */
    if (s32_temp > 4095)
    {
      s32_temp = 4095;
    }

    if (s32_temp < (-4096))
    {
      s32_temp = (-4096);
    }
  }
  else
  {
    /* device not trimmed - skip procedure */
    s32_temp = s32_adcDiffValue;
  }

  return s32_temp;
}

/** \brief Get the shunt current in uA for LIN auto addressing from the difference of
 *         ADC accumulated results.
 * \return uint32_t Shunt current in uA
 * \note   Before getting the result, check valid flag
 * \note   Scaling to uA valid with integrated shunt resistor only! Scaling not valid, if
 *         additional external shunt resistor is used.
 * \note   Positive current measurement only! Connect LIN_S and LIN_M according to user
 *         manual.
 *
 */
uint32_t LIN_getAutoAddrIShunt_uA(void)
{
  sint32 s32_aa_acc_result_diff;
  uint64_t u64_ishunt;
  /* Ignore MISRA Note 9033: Impermissible cast of composite expression: ISENSE_MEAS_NUMBER is known to be 2 bit wide */
  //lint --e{9033}
  s32_aa_acc_result_diff = ((sint32)LIN->AA_ACC_RES_1.bit.ACC_RES_1 - (sint32)LIN->AA_ACC_RES_2.bit.ACC_RES_2) / (sint32)(1u << LIN->AA_CTRL.bit.ISENSE_MEAS_NUMBER);
  s32_aa_acc_result_diff = LIN_compensateAutoAddrResultDiff(s32_aa_acc_result_diff);

  if (s32_aa_acc_result_diff < 0)
  {
    s32_aa_acc_result_diff = 0;
  }

  /* Ignore MISRA Warning 571: Suspicious cast: u64_ishunt is saturated to zero */
  //lint --e{571}
  u64_ishunt = (uint64_t)s32_aa_acc_result_diff * ADC11_SCALE_LINAA;
  /* Ensure accurate rounding during division */
  u64_ishunt += (ADC11_12BIT_RESOLUTION >> 1U);
  u64_ishunt = u64_ishunt / ADC11_12BIT_RESOLUTION;
  return (uint32_t)u64_ishunt;
}

#if (LIN_UART_STD_EN == 1)
/** \brief Put a character to the stdout
 *
 * \param s32_character Character to output
 * \return The written character
 */
sint32 stdout_putchar(sint32 s32_character)
{
  if (LIN_UART_CONFIG_EN == (1u) && (LIN_UART_STD_EN == 1))
  {
    /* Fill buffer */
    LIN_setUartTXbuffer((uint16_t) s32_character);

    /* Handle line feed case */
    if (s32_character == (sint32)'\n')
    {
      (void)PMU_serviceWatchdog();

      /* Wait until new line is sent out */
      while (LIN_isUartByteTransmitted() == false)
      {
        /* Wait for byte to be transmitted */
      }

      (void)PMU_serviceWatchdog();
      /* Fill buffer with carriage return */
      LIN_setUartTXbuffer((uint8_t) '\r');
    }

    /* Wait until character is sent out */
    while (LIN_isUartByteTransmitted() == false)
    {
      (void)PMU_serviceWatchdog();
    }
  }
  else
  {
  }

  return s32_character;
}

/** \brief Receive a character via stdin
 *
 * \return The written character
 */
sint32 stdin_getchar(void)
{
  sint32 s32_character = 0;

  if (LIN_UART_CONFIG_EN == (1u) && (LIN_UART_STD_EN == 1))
  {
    while (LIN_isUartByteReceived() == false)
    {
      /* Service the watchdog until the character is received */
      (void)PMU_serviceWatchdog();
    }

    /* Clear RX interrupt status and get character */
    s32_character = (sint32)LIN_getUartRXbuffer();
  }
  else
  {
  }

  return s32_character;
}

/** \brief  Write a character to a data stream
 */
int fputc(int ch, FILE *f __attribute__((unused)))
{
  return stdout_putchar(ch);
}


/** \brief  Receive a character from a data stream
 */
int fgetc(FILE *f __attribute__((unused)))
{
  return stdin_getchar();
}

#endif /* (LIN_UART_STD_EN == 1) */
