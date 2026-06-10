/******************************************************************************
*                _  _               ___             _                         *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*    ihr GmbH                                                                 *
*    Airport Boulevard B210                                                   *
*    77836 Rheinm�nster - Germany                                             *
*    http://www.ihr.de                                                        *
*    Phone +49(0) 7229-18475-0                                                *
*    Fax   +49(0) 7229-18475-11                                               *
*                                                                             *
*******************************************************************************
*                                                                             *
* (c) Alle Rechte bleiben bei IHR GmbH, auch fuer den Fall von Schutzrechts-  *
* anmeldungen. Jede Verfuegungsbefugnis, wie Kopier- und Weitergaberecht      *
* bleibt bei IHR GmbH.                                                        *
*                                                                             *
* (c) All rights reserved by IHR GmbH including the right to file             *
* industrial property rights. IHR GmbH retains the sole power of              *
* disposition such as reproduction or distribution.                           *
*                                                                             *
********************     Workfile:      LINApplication.c    *******************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver                                           *
*  FILE-DESCRIPTION:     LIN application file                                 *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 1482        $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2021-09-20#$                                 *
*                                                                             *
******************************************************************************/
/**
@file   LINApplication.c
@brief  LIN application file
*/

#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "main.h"
#include "LINApplication.h"

static const l_u8 pidbuf[15] = {0x8b,0x4c,0x0d,0x8e,0xcf,0x50,0x11,0x92,0xd3,0x14,0x55,0xd6,0x97,0xd8,0x99};
void LINAPP_Init(void)
{
   /***************
   Init LIN
   ***************/
   (void)l_sys_init ();
}

#if (defined LIN_SLAVE)
void LinSlave_Task(void)
{
    //LED2_toggle();
   /* Variable for IRQ mask */
   static l_irqmask lin_irqState;
   /* Variable to count bus idle time */
   static l_u16 BusIdleTimeOutCounter = 0u;
   static l_u16 tmpStatus = 0u;
   static l_u8  messagecounter = 0u;
   BusIdleTimeOutCounter++;
   /* Every interrupt that contains either signal read (incl.
    * status flag management), signal write, driver status management,
    * driver tasks (incl. ld_task, cyclic_com_task, l_ifc_rx) should be
    * protected against interruption from other.
    */
   lin_irqState = l_sys_irq_disable();
   /* Call the cyclic task according to the configured value
    * (field "LIN Task Cycletime" in the LDC-Tool) */
   ld_task();
   /* for more detailed information */
   tmpStatus = l_ifc_read_status();
   l_sys_irq_restore(lin_irqState);

   /* If one of the status flags is set then there was something on the bus.
    * Reset counter and wake up. */
   if (tmpStatus & 0x00FFu)
   {
      LED0_setHigh();
      BusIdleTimeOutCounter = 0u;
   }

   /* After 4 seconds no communcation or other disturbances - go to sleep. */
   if (BusIdleTimeOutCounter >= 4000u)
   {
      BusIdleTimeOutCounter = 4000u; /* prevent overflow */
      LED0_setLow();
   }

   /* Toggle LED2 on successful transfer (reception or transmission of a frame)             */
   if (tmpStatus & 0x02u)
   {
      LED1_toggle();

      /* Every time the frame Demo_LED_Response was transmitted successfully
       * increase the message counter.
       * First two bits are parity, so ignore them and then compare to Message Identifier 0x0C */
      if ((tmpStatus & 0x3F00u) == 0x0C00u)
      {
         messagecounter++;
      }
   }

   /* Every interrupt that contains either signal read (incl.
    * status flag management), signal write, driver status management,
    * driver tasks (incl. ld_task, cyclic_com_task, l_ifc_rx) should be
    * protected against interruption from other.
    */
   lin_irqState = l_sys_irq_disable();

   /* Example on how to use signal access macros */
   /* Read signals received by subscribe frames */
//   if (l_flg_tst_SetLed())
//   {
//      l_flg_clr_SetLed();

//      if ( 0u != l_bool_rd_SetLed())
//      {
//         LED2_setHigh();
//      }
//      else
//      {
//         LED2_setLow();
//      }
//   }

   /* Write signals transmitted by publish frame */
//   l_u8_wr_MsgCounter_2(messagecounter);
   l_sys_irq_restore(lin_irqState);
}
#endif /* #if defined LIN_SLAVE */

/********************
 Example for user defined Callback functions from LIN driver
********************/

#ifdef LIN_ENABLE_RBI_RD_ECU_SN
/* Driver Callback to get Serial Number of ECU. */
/* Argument 1:  Output: pointer to data buffer for serial number  */
void l_callback_RBI_ReadEcuSerialNumber(l_u8 SerialnumberResult[])
{
   /* If read ECU serial number (read by ID service - ID 1) is active,
    * fill 4 bytes of data of the array for transmitting the correct
    * data in the response.
    */
   SerialnumberResult[0] = 0x08u;
   SerialnumberResult[1] = 0x15u;
   SerialnumberResult[2] = 0x47u;
   SerialnumberResult[3] = 0x11u;
}
#endif /* end #ifdef LIN_ENABLE_RBI_RD_ECU_SN */

/********************
 Example for user defined Callback functions from LIN driver
********************/

#ifdef LIN_ENABLE_RBI_USER_DEFINED
/* Driver Callback to get User defined answer for specific id. */
/* Argument 1: Input: ID from request */
/* Argument 2: Output: pointer to data buffer response (5 bytes of data) */
l_u8 l_callback_RBI_UserDefined(l_u8 id, l_u8 UserDefinedResult[])
{
   l_u8 retval = 0u;

   /* Assemble data and return data to LIN driver to transmit. Length of
    * response data different for different LIN standards:
    * LIN 2.0 / 2.1: Response data has to be filled with 5 bytes. Return value
    *                for positive response 1 to 5,
    *                for negative response = 0
    *                for no response > 5 (not recommended)
    * LIN 2.2: Allows response length to be configurable, so the return value
    *          is used as information how many databytes are transferred:
    *          for negative response = 0
    *          for positive response 1 to 5
    *                for no response > 5 (not recommended)
    *          This option can be activated by adding a define to genLinConfig.h:
    *          #define LIN_RBI_UDEF_2_2
    */
   switch (id)
   {
      case 32:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;

      case 33:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;

      case 34:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;

      case 35:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;

      default:
         break;
   }

   return retval;
}
#endif /* end #ifdef LIN_ENABLE_RBI_USER_DEFINED */


/********************
 Example for user defined Callback functions from LIN driver
********************/

#ifdef    LIN_ENABLE_DATA_DUMP
/* Driver Callback for data dump request. */
/* Argument 1: Input: pointer to data buffer with data from request */
/* Argument 2: Output: pointer to data buffer to make response */
void l_callback_DataDump(const l_u8 ReceivedData[], l_u8 TransmitData[])
{
   /* ReceivedData delivers the request data (5 bytes) and expects
    * the response data in the TransmitData (also 5 bytes)
    * All data bytes needs to be filled correctly.
    */
   TransmitData[0] = ReceivedData[0];
   TransmitData[1] = ReceivedData[1];
   TransmitData[2] = ReceivedData[2];
   TransmitData[3] = ReceivedData[3];
   TransmitData[4] = ReceivedData[4];
}
#endif /* end #ifdef LIN_ENABLE_DATA_DUMP */


#ifndef LIN_INITIAL_NAD
l_u8 l_callback_Get_Initial_NAD(void)
{
   /* Here you can set the intial NAD.    */
   return SlaveNodeNAD;
}
#endif /* end ifndef LIN_INITIAL_NAD */

#ifdef LIN_USER_DEF_DIAG_ENABLE
void ld_user_define_diagnostic(void)
{
   /* delete previous response upon receiving a new response */
#ifdef LIN_PROTOCOL_VERSION_2_1
   if (l_u8_rd_MasterReq0() !=  0x7Eu)
#endif /* ifdef LIN_PROTOCOL_VERSION_2_1 */
   {
      g_lin_user_diag_active = 0u;
   }

   /* Example Assign NAD, ignore Suppl ID, Func ID */
   if ((l_u8_rd_MasterReq0() ==  l_get_initial_NAD()) && (l_u8_rd_MasterReq1() == 0x06u) && (l_u8_rd_MasterReq2() == 0xB0u))
   {
      /* positive response */
      l_u8_wr_SlaveResp0(l_get_initial_NAD());
      l_u8_wr_SlaveResp1(0x01u);
      l_u8_wr_SlaveResp2(0xF0u);
      l_u8_wr_SlaveResp3(0xFFu);
      l_u8_wr_SlaveResp4(0xFFu);
      l_u8_wr_SlaveResp5(0xFFu);
      l_u8_wr_SlaveResp6(0xFFu);
      l_u8_wr_SlaveResp7(0xFFu);
      l_set_current_NAD(l_u8_rd_MasterReq7());
      g_lin_user_diag_active = 1u;
   }
}
#endif /* ifdef LIN_USER_DEF_DIAG_ENABLE */
/********************
 Example for user defined Callback functions from LIN driver
********************/
#if 0
#ifdef LIN_ENABLE_AA_BSM_B5
void l_callback_AA_assign_NAD_SNPD_start(void)
{
   /* Just an example code */
   /* Prepare HW for driver to execute AA if necessary */
   //PrepareHwForAutoAddressing();
    /* Turn off RGBs and disable LED Driver to unblock ADC */
   LED_0 = 0u;
   LED_1 = 0u;
   LED_2 = 0u;
   
   LDRV->MOD_CFG.bit.LD_ENABLE = 0u;  
   /* Save RGB config to restore it on l_callback_AA_assign_NAD_SNPD_finish if neccessary */
}
#include "ModuleFlash.h"
void l_callback_AA_assign_NAD_SNPD_storeNAD(void)
{
   /* Just an example code */
//	savedConfig.singleAddr = 	g_lin_prod_id.NAD;
//	l_set_current_NAD(g_lin_prod_id.NAD);
//	moduleFlashSave();
	
   /* Store new NAD in NVM if requested */
}
void systemInit(void);
void systemLinCommChangeNad(uint8_t newNad);
void l_callback_AA_assign_NAD_SNPD_finish(const l_u8 exitCode, const l_u8 NAD)
{
   (void)exitCode;
   //(void)NAD;
	SlaveNodeNAD = NAD;
	systemLinCommChangeNad(NAD);
	l_sys_init();
	LDRV->MOD_CFG.bit.LD_ENABLE = 1u;
//   LIN_CHANGE_PID_FRAME_Demo_Response2(pidbuf[NAD]);
	
//	systemInit();
   /* Restore HW config if required */
   //RestoreHwAfterAutoAddressing();
}
#endif /* ifdef LIN_ENABLE_AA_BSM_B5 */
#endif
