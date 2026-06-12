/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2022-2023, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * This software is provided by the copyright holders and contributors "as is" and any express or implied warranties,
 * including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are
 * disclaimed. In no event shall the copyright holder or contributors be liable  for any direct, indirect, incidental,
 * special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or
 * services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability,
 * whether in contract, strict liability,or tort (including negligence or otherwise) arising in any way out  of the
 * use of this software, even if advised of the possibility of such damage.
 *
 **********************************************************************************************************************/

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "lin.h"

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
#define LIN_BAUDRATE_MIN ((uint16)1221)
#define LIN_BAUDRATE_MAX ((uint32)1250000)
#define MESSAGE_SLEEP_TIMEOUT	6000U

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
 * \return sint8 0: success, <0: error codes
 */
sint8 LIN_init(void)
{
  /*{E812B712-1339-4984-8922-AE33885B6C4A}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_CODE_MODULE_DISABLED_IN_CW;
  
	#if (LIN_T2_CONFIG_EN == 1u)
    LIN->T2_CON.reg = (uint32) LIN_T2_CON;
    LIN->T2_MOD.reg = (uint32) LIN_T2_MOD;
    LIN->T2_RC.reg = (uint32) LIN_T2_RC;
    LIN->T2_CNT.reg = (uint32) LIN_T2_CNT;
  #endif
  
  #if (LIN_UART_CONFIG_EN == 1u)
    LIN->UART_SCON.reg = (uint32) LIN_UART_SCON;
    
    /* in order to change UART_BCON bitfields, BR_R has to be toggled */
    LIN->UART_BCON.bit.BR_R = LIN_UART_BCON_BR_R_Disable; /* This is need in case of a reset type 5 */
    LIN->UART_BCON.reg = (uint32) ((LIN_UART_BCON & ~LIN_UART_BCON_BR_R_Msk) | (LIN_UART_BCON_BR_R_Disable<<LIN_UART_BCON_BR_R_Pos));
    LIN->UART_BCON.reg = (uint32) LIN_UART_BCON;
    
    /* to initialize the sync break/synch detection logic, BREN has to be toggled */
    LIN->UART_LINCON.bit.BREN = LIN_UART_LINCON_BREN_Disable; /* This is need in case of a reset type 5 */
    LIN->UART_LINCON.reg = (uint32) ((LIN_UART_LINCON & ~LIN_UART_LINCON_BREN_Msk) | (LIN_UART_LINCON_BREN_Disable<<LIN_UART_LINCON_BREN_Pos));
    LIN->UART_LINCON.reg = (uint32) LIN_UART_LINCON;
  #endif
	
	
  #if ((LIN_LIN_CTRL & LIN_LIN_CTRL_EN_Msk) == LIN_LIN_CTRL_EN_Msk)
    /* in order to configure SM and HV_MODE bitfields of LIN_CTRL, LIN shall be put into sleep mode */
    LIN->LIN_CTRL.bit.MODE = LIN_LIN_CTRL_MODE_LINSleepMode;
    LIN->LIN_CTRL.reg = (uint32) ((LIN_LIN_CTRL & ~LIN_LIN_CTRL_MODE_Msk) | (LIN_LIN_CTRL_MODE_LINSleepMode<<LIN_LIN_CTRL_MODE_Pos));
    LIN->LIN_CTRL.reg = (uint32) LIN_LIN_CTRL;

    LIN->LIN_IEN.reg = (uint32) LIN_LIN_IEN;
    LIN->WAKE_CNF.reg = (uint32) LIN_WAKE_CNF;
  #endif  

  /* Auto-addressing part is still missing in the ConfigWizard */
  /*LIN->AA_STATUS.reg = (uint32) LIN_AA_STATUS;*/
  /*LIN->AA_FW_CTRL.reg = (uint32) LIN_AA_FW_CTRL;*/
  /*LIN->AA_CTRL.reg = (uint32) LIN_AA_CTRL;*/
  /*LIN->AA_I_DIFF_TH.reg = (uint32) LIN_AA_I_DIFF_TH;*/
  /*LIN->AA_ACC_RES_1.reg = (uint32) LIN_AA_ACC_RES_1;*/
  /*LIN->AA_ACC_RES_2.reg = (uint32) LIN_AA_ACC_RES_2;*/

  #if ( (LIN_T2_CONFIG_EN == 1u) || (LIN_UART_CONFIG_EN == 1u) || ((LIN_LIN_CTRL & LIN_LIN_CTRL_EN_Msk) == LIN_LIN_CTRL_EN_Msk) )
    s8_returnCode = ERR_LOG_SUCCESS;
  #endif

  return s8_returnCode;
	
} 

/** \brief Set the baudrate for UART
 *
 * \param u32_baudrate baudrate
 * \return sint8 0: success, <0: error codes
 */
sint8 LIN_setUartBaudrate(uint32 u32_baudrate)
{
  /*{0BFFE6D1-D014-4de3-992E-802450F5C2FB}*/

  uint16 u16_brval;
  sint8 s8_returnCode;

  if ((u32_baudrate >= LIN_BAUDRATE_MIN ) && (u32_baudrate <= LIN_BAUDRATE_MAX ))
  {
    u16_brval = (uint16)((((uint32)LIN_UARTCLOCK_HZ) << 1u) / u32_baudrate);
  
    LIN->UART_BCON.bit.BR_R = 0u;
    LIN->UART_BCON.bit.BG_BR_VALUE = ((uint32)u16_brval >> 5u);
    LIN->UART_BCON.bit.BG_FD_SEL = ((uint32)u16_brval & (uint32)0x1F);
    LIN->UART_BCON.bit.BR_R = 1u;
    
    s8_returnCode = ERR_LOG_SUCCESS;

  }
  else
  {
  	s8_returnCode = ERR_LOG_CODE_PARAM_OUT_OF_RANGE;
  }
  
  return s8_returnCode;
	
}


/** \brief Get the baudrate for UART
 * \return uint32 UART baudrate
 */
uint32 LIN_getUartBaudrate(void)
{
  return (uint32)((((uint32)LIN_UARTCLOCK_HZ)<<1 )  / ((1U << (uint8)LIN->UART_BCON.bit.BR_PRE) * (32 * LIN->UART_BCON.bit.BG_BR_VALUE + (LIN->UART_BCON.bit.BG_FD_SEL))));
}
extern l_u16 g_u16_BusIdleAutoSleepTimeOutCounter;
extern tick_t g_u16_BusIdleMsgSleepTimeOutCounter;
extern l_u16 g_u16_SleepTimeCnt;
static tick_t clockCompensation = 0;
void LIN_Task_1ms(void)
{
	g_u16_CurrentTick++;

	g_u16_BusIdleAutoSleepTimeOutCounter++;
	if (g_e_SleepType == E_SLEEP_TYPE_COMMAND)
	{
		g_u16_BusIdleMsgSleepTimeOutCounter++;
		if (g_u16_BusIdleMsgSleepTimeOutCounter >= MESSAGE_SLEEP_TIMEOUT)
		{
			if (g_st_SysStatus.e_SleepFlag == E_SYSTEM_SLEEP_AWAKE)
				g_st_SysStatus.e_SleepFlag = E_SYSTEM_SLEEP_PREPARE;
		}
	}
	if(GPIO->OUT.bit.P0 == 1)
		GPIO->OUT.bit.P0 = 0;
	else
		GPIO->OUT.bit.P0 = 1;
	
}



l_irqmask l_sys_irq_disable (void)
{
   /* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
    * Reason: HW access through library */
   l_irqmask retVal;
   
   /* Get NVIC LIN IRQ status and disable LIN IRQ */
   retVal = ((LIN_UART_IR_SET_EN) & LIN_IR_SET_EN_MASK); /*lint !e923*/
   LIN_UART_IR_CLR_EN = LIN_IR_SET_EN_MASK; /*lint !e923*/
   return retVal;
}

void l_sys_irq_restore (l_irqmask mask)
{
   /* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
    * Reason: HW access through library */
   /* Check if IRQ was disabled in last irq_disable call */
   if ( 0u != mask )
   {
      /* Re-enable LIN IRQ */
      LIN_UART_IR_SET_EN = (LIN_UART_IR_SET_EN | LIN_IR_SET_EN_MASK); /*lint !e923*/
   }
   else
   {
      /* Do not re-enable LIN IRQ */
   }
}

/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
void LIN_Enable_Transceiver(void)
{
   /* Set LIN normal mode */
   LIN->LIN_CTRL.bit.MODE = 3u; /*lint !e923*/

   /* Disable LIN transceiver wakeup */
   PMU->WAKECTRL.bit.EXT0 = 0u; /*lint !e923*/
}

/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: Part of API, if not used MISRA violation will be issued. */
void LIN_Disable_Transceiver(void)
{
}



/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: System ISR. */
void LIN_COM_Handler(void)
{
#ifdef AUTOBAUD_ENABLED
   if (LIN->LIN_IS.bit.EXF2 == 1u) /*lint !e923*/
   {
      /* ISR for baud rate measurement with input capture */
      l_ifc_t2();
      LIN->LIN_ISC.bit.EXF2CLR = 1u; /*lint !e923*/
   }
#endif /* end #ifdef AUTOBAUD_ENABLED */
   if (LIN->LIN_IS.bit.RI == 1u) /*lint !e923*/
   {
      l_ifc_rx();
      l_cyclic_com_task();
			
      /* If LIN version is < 2.1 special handling is necessary in the ISR to
       * detect bus disturbances or other communication. Bus activity is
       * signaled by the function l_ifc_read_status only from LIN 2.1 and
       * higher. */
      /* Some UARTs require special handling to detect bus stuck to
       * dominant level as the HW will generate continuously interrupts.
       * This should be prevented by the application engineer as the driver
       * cannot recognize such situations. */
      LIN->LIN_ISC.bit.RICLR = 1u; /*lint !e923*/
   }
#ifdef AUTOBAUD_ENABLED
   else if ((LIN->LIN_IS.reg & LINST_MASK_SYNBRK) != 0u) /*lint !e923*/
   {
      l_ifc_rx();
      l_cyclic_com_task();
      /* If LIN version is < 2.1 special handling is necessary in the ISR to
       * detect bus disturbances or other communication. Bus activity is
       * signaled by the function l_ifc_read_status only from LIN 2.1 and
       * higher. */
      /* Some UARTs require special handling to detect bus stuck to
       * dominant level as the HW will generate continuously interrupts.
       * This should be prevented by the application engineer as the driver
       * cannot recognize such situations. */
      /* LINST interrupt flags are cleared by Hardware Abstraction Layer */
   }
   else
   {
      /* unknown interrupt source */
   }
#endif /* end #ifdef AUTOBAUD_ENABLED */
}

/* Timer ISR */
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: System ISR. */


/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
void InitHw(void)
{
   /* LED settings are done in Config Wizard */

   /* variables initialization */
   LED_0 = 0xFFFF; /*lint !e923*/
   LED_1 = 0u; /*lint !e923*/
   LED_2 = 0u; /*lint !e923*/
 
   /* Set tranceiver into sleep state */
   LIN->LIN_CTRL.bit.MODE = 0u; /*lint !e923*/
   
   /* Wait till state is changed to sleep */
   while (LIN->LIN_CTRL.bit.MODE != 0u) /*lint !e923*/
   {
      /* Do nothing */
   }
   
   LIN->LIN_IEN.bit.OC_IEN = 0u; /*lint !e923*/
   LIN->LIN_IEN.bit.OT_IEN = 0u; /*lint !e923*/
   LIN->LIN_IEN.bit.TXD_TMOUT_IEN = 0u; /*lint !e923*/
   
   /* First set register without changing the mode to normal mode */
   /* EN = 1, TXD = 0, HV_MODE = 0, SM = 0 */
   LIN->LIN_CTRL.reg = (uint32)1u; /*lint !e923*/
   /* Now set transceiver to normal mode (MODE = 3) */
   LIN->LIN_CTRL.reg = (uint32)7u; /*lint !e923*/   
   
   /* TRX->IRQCLR.reg = 0u; */
   /* TRX->IRQS.reg=0u; */
   
   LIN->UART_LINCON.bit.BGSEL = 3u; /*lint !e923*/
   LIN->UART_LINCON.bit.BREN = 1u; /*lint !e923*/
   LIN->LIN_IEN.bit.ERRSYNEN = 0u; /*lint !e923*/
   LIN->LIN_IEN.bit.EOFSYNEN = 0u; /*lint !e923*/
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
    LIN_setUartTXbuffer((uint16) s32_character);
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
      LIN_setUartTXbuffer((uint8) '\r');
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
