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
 * \file     main.c
 *
 * \brief    Blinky LED example
 *
 * \version  V1.0.1
 * \date     14. Aug 2025
 *
 * \note
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** VO           Vanessa Ongaro                                                **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2025-07-24, VO:   Initial version                                  **
** V1.0.1: 2025-08-14, VO:   [ATVCOMETLL-220] Use LDRV channel enum values    **
*******************************************************************************/

/*******************************************************************************
**                                  Abstract                                  **
********************************************************************************
** Blinky LED example                                                         **
********************************************************************************
** Within the endless main loop a tick count is incremented. If the tick      **
** reached 100000 the RGB LED colour is changed. Then the tick count is       **
** reinitialized to 0.                                                        **
*******************************************************************************/

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "device.h"

/*******************************************************************************
**                        Global Constant Declarations                        **
*******************************************************************************/

/*******************************************************************************
**                          Global Type Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/

/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/

sint32 main(void)
{
  uint8_t u8_return_code;
  sint32 s32_counter;
  sint8 s8_ledIndex;
  sint8 s8_onState;

  /* Main watchdog service */
  (void)PMU_serviceWatchdog();

  /* Initialization of hardware modules based on Config Wizard configuration */
  u8_return_code = Device_init();

  if (u8_return_code != ERR_LOG_SUCCESS)
  {
    /* Place your code here to handle an initialization error */
    for (;;)
      ;
  }

  /*****************************************************************************
  ** Place your application code here                                         **
  *****************************************************************************/
  #define MAX_BRIGHTNESS 0x0FFF
  #define MIN_BRIGHTNESS 0x0000
  #define MAX_COUNTER    100000
  #define RED_LED        LDRV_CHANNEL_0
  #define BLUE_LED       LDRV_CHANNEL_1
  #define GREEN_LED      LDRV_CHANNEL_2
  #define WHITE_LED      LDRV_CHANNEL_3

  s32_counter = 0;
  s8_ledIndex = 0;
  s8_onState = 0;

  for (;;)
  {
    /* Main watchdog service */
    (void)PMU_serviceWatchdog();
        
    if (s32_counter++ > MAX_COUNTER)
    {
      if (s8_onState == 0)
      {
        LDRV->PWM_DC_CFG0.reg = (s8_ledIndex == RED_LED) ? MAX_BRIGHTNESS : MIN_BRIGHTNESS;
        LDRV->PWM_DC_CFG1.reg = (s8_ledIndex == BLUE_LED) ? MAX_BRIGHTNESS : MIN_BRIGHTNESS;
        LDRV->PWM_DC_CFG2.reg = (s8_ledIndex == GREEN_LED) ? MAX_BRIGHTNESS : MIN_BRIGHTNESS;
        LDRV->PWM_DC_CFG3.reg = (s8_ledIndex == WHITE_LED) ? MAX_BRIGHTNESS : MIN_BRIGHTNESS;
        
        if (++s8_ledIndex > WHITE_LED)
          s8_ledIndex = RED_LED;

        s8_onState = 1;
      }
      else
      {
        LDRV->PWM_DC_CFG0.reg = MIN_BRIGHTNESS;
        LDRV->PWM_DC_CFG1.reg = MIN_BRIGHTNESS;
        LDRV->PWM_DC_CFG2.reg = MIN_BRIGHTNESS;
        LDRV->PWM_DC_CFG3.reg = MIN_BRIGHTNESS;
        s8_onState = 0;
      }
      
      s32_counter = 0;
    }
  }
}

