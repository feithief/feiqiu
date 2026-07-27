/******************************************************************************
*                _  _               ___             _                         *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*    ihr GmbH                                                                 *
*    Airport Boulevard B210                                                   *
*    77836 Rheinmünster - Germany                                             *
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
********************     Workfile:      lin_main.c       **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver Protocol Layer                            *
*  FILE-DESCRIPTION:     Main routines for LIN driver                         *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 72          $                                 *
*       Responsible:     B.Roegl                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2015-12-04#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_main.c
@brief Implementation of the Main Functionality inside the LIN Driver
*/

/* ===========================================================================
 *  Header files
 * ==========================================================================*/

#include "genLinConfig.h"
#include "lin_hal.h"
#include "lin_slave_task.h"
#include "lin_driver_api.h"

#ifdef LIN_COOKED_API
#include "lin_diag_tp.h"
#endif /* end #ifdef LIN_COOKED_API */

#ifdef LIN_RAW_API
#include "lin_diag_raw.h"
#endif /* end #ifdef LIN_RAW_API */

#if ((!defined(LIN_DIAG_ENABLE)) && (!defined(LIN_USER_DEF_DIAG_ENABLE)))
#include "lin_config.h"
#endif /* end #ifndef LIN_DIAG_ENABLE */

#include "lin_main.h"
#include "ModuleFlash.h"

/* ===========================================================================
 *  Global Variables
 * ==========================================================================*/

l_u8              g_lin_frame_index;
l_u8              g_lin_active_pid;
t_Lin_Frame_Ctrl  g_lin_frame_ctrl[LIN_NUMBER_OF_FRAMES];  /* init with const from the config file */
t_lin_diag_frame  g_lin_diag_mreq;
t_lin_diag_frame  g_lin_diag_sresp;
t_lin_prod_id     g_lin_prod_id;                           /* init with const from the config file */
t_lin_status_word g_lin_status_word;
l_irqmask         g_lin_irqState;
t_lin_rx_ctrl     g_lin_rx_ctrl;
l_u8              g_lin_frame_data_size;
#if ((!defined(LIN_USER_DEF_DIAG_ENABLE)) || defined(LIN_USER_DEF_DIAG_MANDSERV_ENABLE))
l_u16             g_lin_tp_timeoutcounter;
#endif /* end #if (!defined(LIN_USER_DEF_DIAG_ENABLE)) || defined(LIN_USER_DEF_DIAG_MANDSERV_ENABLE) */

#if ((!defined(LIN_DIAG_ENABLE)) && (!defined(LIN_USER_DEF_DIAG_ENABLE)))
l_u8              g_lin_diag_config_active;
#endif /* #if (!defined(LIN_DIAG_ENABLE)) && (!defined(LIN_USER_DEF_DIAG_ENABLE)) */
#ifdef LIN_USER_DEF_DIAG_ENABLE
l_u8              g_lin_user_diag_active;
#endif /* end #ifdef LIN_USER_DEF_DIAG_ENABLE */
#ifdef J2602_SLAVE
t_lin_diag_frame  j2602_config_frame;
t_j2602_status    g_j2602_status;
l_u8              j2602_broadcast_bytes[2][J2602_DNN_NUMBER_OF_BYTES];
#ifndef GM_SPEC_ADAPTION
l_u8              j2602_statusbyte = 0x00u;
#endif /* #ifndef GM_SPEC_ADAPTION */
#endif /* end #ifdef J2602_SLAVE */

/* ===========================================================================
 *  Functions
 * ==========================================================================*/

/* ---------------------------------------------------------------------------
 *  void lin_main_init(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Initializes driver, calls HAL-init
   @param  void
   @retval void
*/
void lin_main_init(void)
{
   l_u8  loc_bytecounter = 0u;
   l_u8  loc_framecounter = 0u;

   /* init the Hardware */
   lin_hal_init();

#ifdef LIN_COOKED_API
   /* init diagnostic functions with transport protocol support */
   (void)lin_tp_init();
#endif /* end #ifdef LIN_COOKED_API */
#ifdef LIN_RAW_API
   /* init diagnostic functions with raw data support */
   (void)lin_diag_raw_init();
#endif /* end #ifdef LIN_RAW_API */

#if ((!defined(LIN_USER_DEF_DIAG_ENABLE)) || defined(LIN_USER_DEF_DIAG_MANDSERV_ENABLE))
   g_lin_tp_timeoutcounter = 0u;
#endif /* end #if (!defined(LIN_USER_DEF_DIAG_ENABLE)) || defined(LIN_USER_DEF_DIAG_MANDSERV_ENABLE) */

   /* initialise general data settings */
#ifdef LIN_INITIAL_NAD
   g_lin_prod_id.NAD = LIN_INITIAL_NAD;
   g_lin_prod_id.Initial_NAD = LIN_INITIAL_NAD;
#else
   g_lin_prod_id.NAD = l_callback_Get_Initial_NAD();
   g_lin_prod_id.Initial_NAD = l_callback_Get_Initial_NAD();
#endif /* end #ifdef LIN_INITIAL_NAD */
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1) || (defined J2602_PROTOCOL))
   g_lin_prod_id.Supplier_lo = savedConfig.ProductID[0];
   g_lin_prod_id.Supplier_hi = savedConfig.ProductID[1];
   g_lin_prod_id.Function_lo = savedConfig.ProductID[2];
   g_lin_prod_id.Function_hi = savedConfig.ProductID[3];
   g_lin_prod_id.Variant = savedConfig.ProductID[4];
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */

   g_lin_frame_index       = 0xFFu;
   g_lin_active_pid        = 0x0u;
   g_lin_status_word.reg   = 0u;
   g_lin_rx_ctrl.reg       = 0u;
   g_lin_irqState          = 0u;
   g_lin_frame_data_size   = 0u;

   /* initialize frame array*/
   for(loc_framecounter = 0u; loc_framecounter < LIN_NUMBER_OF_FRAMES; loc_framecounter++)
   {
#if ((defined LIN_ENABLE_ASSIGN_FRAME_ID) || (defined LIN_ENABLE_RBI_RD_MSG_ID_PID))
      for(loc_bytecounter = 0u; loc_bytecounter < 5u; loc_bytecounter++)
#else
      for(loc_bytecounter = 0u; loc_bytecounter < 3u; loc_bytecounter++)
#endif /* end #ifdef LIN_ENABLE_ASSIGN_FRAME_ID */
      {
          switch(SlaveNodeNAD)
          {
            case 0x01U: /* Slave NAD of AM_RGB_Slave_01 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT01[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x02U: /* Slave NAD of AM_RGB_Slave_02 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT02[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x03U: /* Slave NAD of AM_RGB_Slave_03 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT03[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x04U: /* Slave NAD of AM_RGB_Slave_04 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT04[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x05U: /* Slave NAD of AM_RGB_Slave_05 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT05[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x06U: /* Slave NAD of AM_RGB_Slave_06 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT06[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x07U: /* Slave NAD of AM_RGB_Slave_07 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT07[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x08U: /* Slave NAD of AM_RGB_Slave_08 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT08[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x09U: /* Slave NAD of AM_RGB_Slave_09 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT09[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x0AU: /* Slave NAD of AM_RGB_Slave_10 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT10[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x0BU: /* Slave NAD of AM_RGB_Slave_11 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT11[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x0CU: /* Slave NAD of AM_RGB_Slave_12 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT12[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x0DU: /* Slave NAD of AM_RGB_Slave_13 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT13[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x0EU: /* Slave NAD of AM_RGB_Slave_14 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT14[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x0FU: /* Slave NAD of AM_RGB_Slave_15 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT15[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x10U: /* Slave NAD of AM_RGB_Slave_16 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT16[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x11U: /* Slave NAD of AM_RGB_Slave_17 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT17[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x12U: /* Slave NAD of AM_RGB_Slave_18 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT18[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x13U: /* Slave NAD of AM_RGB_Slave_19 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT19[loc_framecounter].reg[loc_bytecounter];
              break;
            case 0x14U: /* Slave NAD of AM_RGB_Slave_20 */
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT20[loc_framecounter].reg[loc_bytecounter];
              break;
            default:
              g_lin_frame_ctrl[loc_framecounter].reg[loc_bytecounter] = const_LIN_FRAME_CTRL_INIT01[loc_framecounter].reg[loc_bytecounter];
              break;
          }
      }
   }

   /* initialize internal diagnostic buffers */
   for(loc_bytecounter = 0u; loc_bytecounter < 8u; loc_bytecounter++)
   {
      g_lin_diag_mreq.byte[loc_bytecounter]   = 0xFFu;
      g_lin_diag_sresp.byte[loc_bytecounter]  = 0xFFu;
#ifdef J2602_SLAVE
      j2602_config_frame.byte[loc_bytecounter] = 0u;
#endif /* end #ifdef J2602_SLAVE */
   }

#if ((!defined(LIN_DIAG_ENABLE)) && (!defined(LIN_USER_DEF_DIAG_ENABLE)))
   g_lin_diag_config_active = 0x0u;
#endif /* #if (!defined(LIN_DIAG_ENABLE)) && (!defined(LIN_USER_DEF_DIAG_ENABLE)) */
#ifdef LIN_USER_DEF_DIAG_ENABLE
   g_lin_user_diag_active = 0x0u;
#endif /* end #ifdef LIN_USER_DEF_DIAG_ENABLE */

   /* Initialize l_LinData with default data from the LDF file */
   for(loc_bytecounter = 0u; loc_bytecounter < (LIN_NUMBER_OF_FRAMES * 8u); loc_bytecounter++)
   {
      l_LinData.dataBytes[loc_bytecounter] = const_LinData.dataBytes[loc_bytecounter];
   }
#ifdef J2602_SLAVE
   g_j2602_status.reg[0] = 0u;
   /* Initialize buffers for broadcast message bytes */
   for(loc_bytecounter = 0u; loc_bytecounter < J2602_DNN_NUMBER_OF_BYTES; loc_bytecounter++)
   {
       j2602_broadcast_bytes[0][loc_bytecounter] = 0u;
       j2602_broadcast_bytes[1][loc_bytecounter] = 0u;
   }
#ifndef GM_SPEC_ADAPTION
   j2602_statusbyte = 0x00u;
#endif /* #ifndef GM_SPEC_ADAPTION */
   j2602_hal_target_reset (0u);
#endif /* end #ifdef J2602_SLAVE */
}

/* ---------------------------------------------------------------------------
 *  void l_get_byte_array(l_u8 start[], l_u8 count, l_u8 destination[])
 * --------------------------------------------------------------------------*/
/**
   @brief  Fetch a byte array
   @pre    LIN driver initialized
   @param  l_u8 start        source buffer
   @param  l_u8 count        number of bytes to be copied
   @param  l_u8 destination  target buffer
   @retval void
*/
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: Part of API, configuration dependent usage in signal access macros, MISRA violation can be issued. */
void l_get_byte_array(l_u8 start[], l_u8 count, l_u8 destination[])
{
   l_u8 loc_i;
   for(loc_i = 0u; loc_i < count; loc_i++)
   {
      destination[loc_i] = start[loc_i];
   }
}

/* ---------------------------------------------------------------------------
 *  void l_set_byte_array(l_u8 start[], l_u8 count, const l_u8 source[])
 * --------------------------------------------------------------------------*/
/**
   @brief  Set a byte array
   @pre    LIN driver initialized
   @param  l_u8 start        target buffer
   @param  l_u8 count        number of bytes to be copied
   @param  l_u8 source       source buffer
   @retval void
*/
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: Part of API, configuration dependent usage in signal access macros, MISRA violation can be issued. */
void l_set_byte_array(l_u8 start[], l_u8 count, const l_u8 source[])
{
   l_u8 loc_i;
   for(loc_i = 0u; loc_i < count; loc_i++)
   {
      start[loc_i] = source[loc_i];
   }
}

