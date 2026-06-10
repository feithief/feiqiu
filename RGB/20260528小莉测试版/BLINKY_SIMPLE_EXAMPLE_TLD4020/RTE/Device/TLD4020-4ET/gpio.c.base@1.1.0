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
#include "gpio.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize the GPIO module
 *
 * \return uint8_t 0: success, !0: error codes
 */
uint8_t GPIO_init(void)
{
  /*{0C84DE37-A85C-4f81-9C1D-BF362506B9EF}*/
  uint8_t u8_returnCode;
  u8_returnCode = ERR_LOG_SUCCESS;  /* config can not be disabled in CW */
  GPIO->OUT.reg = (uint32_t) GPIO_OUT;
  GPIO->PADCFG0_DRVCFG.reg = (uint32_t) GPIO_PADCFG0_DRVCFG;
  GPIO->PADCFG1_DRVCFG.reg = (uint32_t) GPIO_PADCFG1_DRVCFG;
#if (UC_DEVICE == 403012)
  GPIO->PADCFG3_DRVCFG.reg = (uint32_t) GPIO_PADCFG3_DRVCFG;
  GPIO->PADCFG4_DRVCFG.reg = (uint32_t) GPIO_PADCFG4_DRVCFG;
#endif
  GPIO->PDISC.reg = (uint32_t) GPIO_PDISC;
  GPIO->IEN.reg = (uint32_t) GPIO_IEN;
  return u8_returnCode;
}

