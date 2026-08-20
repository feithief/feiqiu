/******************************************************************************
*                _  _               ___             _                         *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*    ihr GmbH                                                                 *
*    Airport Boulevard B210                                                   *
*    77836 Rheinm¨¹nster - Germany                                             *
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
********************     Workfile:      lin_diag_tp.c    **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver Protocol Layer                            *
*  FILE-DESCRIPTION:     All Routines for LIN Diagnostic Transport Layer      *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 1359        $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2020-10-29#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_diag_tp.c
@brief All Routines for LIN Diagnostic Transport Layer
 */

/* ===========================================================================
 *  Header files
 * ==========================================================================*/

#include "genLinConfig.h"

#ifdef LIN_COOKED_API

#include "lin_type.h"
#include "lin_driver_api.h"
#include "lin_main.h"
#include "lin_config.h"
#include "lin_diag_tp.h"
#include "SystemStatus.h"
#include "SystemLinDiag.h"
#include "ModuleFlash.h"
/* ===========================================================================
 *  Global Variables
 * ==========================================================================*/

t_lin_tp_status      g_lin_tp_stat;
static l_u16         g_lin_tp_data_left;
static l_u16         g_lin_tp_rx_data_left;
static l_u16         g_lin_tp_rx_data_index;
static l_u8          g_lin_cf_msg_nbr;
static const l_u8    (*g_lin_tp_ptr_tx_data)[];
static l_u16         g_lin_tp_tx_data_index;
static l_u16*        g_lin_tp_ptr_rx_size;
static l_u8          (*g_lin_tp_ptr_rx_data)[];
t_lin_tp_tx_rx_ctrl  g_lin_tp_tx_rx_ctrl;
static l_u8          g_lin_tp_rx_preNAD;
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
static l_u8*         g_lin_tp_ptr_rx_nad;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */

/* ===========================================================================
 *  Functions
 * ==========================================================================*/

static void lin_tp_receive_frame(void);
static void lin_tp_send_cf(void);

/* ---------------------------------------------------------------------------
 *  l_u8 lin_tp_init(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function initializes the Cooked transport layer for LIN diagnostics.
   @pre    LIN driver initialized
   @param  void
   @retval Zero If the initialization failed
   @retval Non-zero If the initialization succeeded
*/
l_u8 lin_tp_init(void)
{
   g_lin_tp_stat.rx_stat            = LD_COMPLETED;
   g_lin_tp_stat.tx_stat            = LD_COMPLETED;
   g_lin_tp_data_left               = 0u;
   g_lin_tp_rx_data_left            = 0u;
   g_lin_tp_rx_data_index           = 0u;
   g_lin_cf_msg_nbr                 = 0u;
   g_lin_tp_ptr_tx_data             = (const l_u8 (*) [])0u;
   g_lin_tp_tx_data_index           = 0u;
   g_lin_tp_ptr_rx_size             = (l_u16 *)0u;
   g_lin_tp_ptr_rx_data             = (l_u8 (*) [])0u;
   g_lin_tp_tx_rx_ctrl.rx_cf_on     = 0u;
   g_lin_tp_tx_rx_ctrl.tx_on        = 0u;
   g_lin_tp_tx_rx_ctrl.rx_t_out_on  = 0u;
   g_lin_tp_tx_rx_ctrl.tx_t_out_on  = 0u;
   g_lin_tp_tx_rx_ctrl.rx_cf_num    = 0u;
   g_lin_tp_rx_preNAD               = 0u;

   g_lin_tp_timeoutcounter          = 0u;
   g_lin_rx_ctrl.flag.rx_multiframe_msg   = 0u;
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
   g_lin_tp_ptr_rx_nad = 0u;
#ifndef LIN_PROTOCOL_VERSION_1_3
   g_lin_config_status = LD_SERVICE_IDLE;
#endif /* end #ifndef LIN_PROTOCOL_VERSION_1_3 */
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
   return 1u;
}

/* ---------------------------------------------------------------------------
 *  void ld_receive_message(l_u16* const length,l_u8* const data);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function notifies the TP that the application prepares to receive
           a request.
   @note   The cooked transport layer does not include a data buffer within the
           driver. The application has to prepare a buffer. After receiving a
           request the data is stored in the buffer and the size pointer is
           adapted to contain the actual size of the diagnostic request.
   @pre    LIN driver initialized
   @param  l_u16* length    pointer to size information of length of buffer
   @param  l_u8*  data      pointer to buffer for receiving data
   @retval void
*/
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
void ld_receive_message(l_u16* const length, l_u8* const NAD, l_u8* const data)
#else
void ld_receive_message(l_u16* const length, l_u8* const data)
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
{
#if ((defined LIN_SLAVE)||(defined J2602_SLAVE))
   if(g_lin_tp_stat.rx_stat != LD_IN_PROGRESS)
   {
      g_lin_tp_stat.rx_stat = LD_IN_PROGRESS;
      g_lin_tp_ptr_rx_size = length;
      /* (MISRA-C:2004 11.4/A, e927) A cast should not be performed between a pointer to object type and a different pointer to object type
       * Reason: make the pointer (req. by LIN specification) accessible as array. */
      g_lin_tp_ptr_rx_data = (l_u8 (*)[])data; /*lint !e927*/
   }
#else
   if((g_lin_tp_stat.rx_stat == LD_COMPLETED) && (g_lin_tp_stat.tx_stat == LD_COMPLETED))
   {
      g_lin_tp_stat.rx_stat = LD_IN_PROGRESS;
      g_lin_tp_ptr_rx_size = length;
      /* (MISRA-C:2004 11.4/A, e927) A cast should not be performed between a pointer to object type and a different pointer to object type
       * Reason: make the pointer (req. by LIN specification) accessible as array. */
      g_lin_tp_ptr_rx_data = (l_u8 (*)[])data; /*lint !e927*/
      g_lin_tp_ptr_rx_nad = NAD;
   }
   else
   {
      g_lin_tp_stat.rx_stat = LD_FAILED;
   }
#endif /* #if ((defined LIN_SLAVE)||(defined J2602_SLAVE)) */
}

/* ---------------------------------------------------------------------------
 *  void lin_tp_send_cf(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function handles the transmission of subsequent frames of one
           multi-frame message
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
static void lin_tp_send_cf(void)
{
   l_u8  loc_i;
   g_lin_cf_msg_nbr++;
   g_lin_cf_msg_nbr %= 16u;

   /* Consecutive Frame Transmit */
#if ((defined LIN_SLAVE)||(defined J2602_SLAVE))
   LIN_DIAG_SEND_FRAME.frame.NAD = g_lin_prod_id.NAD;
#endif /* #if ((defined LIN_SLAVE)||(defined J2602_SLAVE)) */
   LIN_DIAG_SEND_FRAME.frame.PCI = (l_u8)(CONSECUTIVE_FRAME | g_lin_cf_msg_nbr);
   for(loc_i = 0u; loc_i < LIN_CF_DATA_LEN; loc_i++)
   {
      if(loc_i<g_lin_tp_data_left)
      {
         LIN_DIAG_SEND_FRAME.frame.type.CF.diag_data[loc_i] = (*g_lin_tp_ptr_tx_data)[loc_i + g_lin_tp_tx_data_index];
      }
      else
      {
         LIN_DIAG_SEND_FRAME.frame.type.CF.diag_data[loc_i] = 0xFFu;
         g_lin_tp_data_left = 0u;
      }
   }
   if(g_lin_tp_data_left)
   {
      g_lin_tp_data_left -= LIN_CF_DATA_LEN;
      g_lin_tp_tx_data_index += LIN_CF_DATA_LEN;
   }
}

/* ---------------------------------------------------------------------------
 *  void lin_tp_tx_part_complete(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function checks if after transmitting a frame the message is
           finished or another frame buffer has to be prepared.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void lin_tp_tx_part_complete(void)
{
   /* callback from slave_task_tx_data */
   /* clear timeout counter for transmit */
   lin_tp_clear_timeout();

   /* if last cf transmitted reset multiframe flag */
   if(g_lin_tp_data_left == 0u)
   {
      if(g_lin_tp_stat.tx_stat < LD_COMPLETED)
      {
         g_lin_tp_stat.tx_stat = LD_COMPLETED;
      }
      g_lin_tp_tx_rx_ctrl.tx_on = 0u;
      g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;
#ifdef J2602_SLAVE
      g_j2602_status.flags.sresp = 0u;
      g_j2602_status.flags.tx_resp = 0u;
#endif /* end #ifdef J2602_SLAVE */
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
      /* Masterrequest finished, start timer for slaveresponse */
      g_ld_timer_p2 = 0u;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
   }
   else
   {
      lin_tp_send_cf();
      j2602_allow_slaveresponse();
   }
}

/* ---------------------------------------------------------------------------
 *  void ld_send_message(l_u16 length, const l_u8* const data);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function will prepare the first frame of a diagnostic response.
   @pre    LIN driver initialized
   @param  l_u16 length       length information of data
   @param  l_u8* data       pointer to buffer for data to be sent
   @retval void
*/
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
void ld_send_message(l_u16 length, l_u8 NAD, const l_u8* const data)
#else
void ld_send_message(l_u16 length, const l_u8* const data)
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
{
   l_u8  loc_i = 0u;

   if(g_lin_rx_ctrl.flag.rx_multiframe_msg == L_SET)
   {
      g_lin_tp_tx_rx_ctrl.tx_on = 0u;
      g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;
      g_lin_tp_ptr_tx_data = (const l_u8 (*) [])0u;
      g_lin_tp_data_left = 0u;
   }
   else
   {
      if(length > LIN_TP_MAX_LEN)
      {
         /* set error in status */
         if(g_lin_tp_stat.tx_stat <= LD_COMPLETED)
         {
            g_lin_tp_stat.tx_stat = LD_FAILED;
         }
         g_lin_tp_tx_rx_ctrl.tx_on = 0u;
         g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;
      }
      else
      {
         /* Set status to proceeding */
         g_lin_tp_stat.tx_stat = LD_IN_PROGRESS;
         /* (MISRA-C:2004 11.4/A, e927) A cast should not be performed between a pointer to object type and a different pointer to object type
          * Reason: make the pointer (req. by LIN specification) accessible as array. */
         g_lin_tp_ptr_tx_data = (const l_u8 (*)[])data; /*lint !e927*/
         if(length > LIN_SF_DATA_LEN)
         {
            /* Start Multi Frame Transmit */
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
            LIN_DIAG_SEND_FRAME.frame.NAD = NAD;
#else
            LIN_DIAG_SEND_FRAME.frame.NAD = g_lin_prod_id.NAD;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
            LIN_DIAG_SEND_FRAME.frame.PCI = (l_u8)(FIRST_FRAME | (l_u8)(length>>8u));
            LIN_DIAG_SEND_FRAME.frame.type.FF.LEN = (l_u8)(length & 0x00FFu);
            for(loc_i = 0u; loc_i < LIN_FF_DATA_LEN; loc_i++)
            {
               LIN_DIAG_SEND_FRAME.frame.type.CF.diag_data[loc_i + 1u] = (*g_lin_tp_ptr_tx_data)[loc_i];
            }
            g_lin_tp_data_left  = length - LIN_FF_DATA_LEN;
            g_lin_tp_tx_data_index = LIN_FF_DATA_LEN;
            g_lin_cf_msg_nbr = 0u;
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
#ifdef LIN_PROTOCOL_VERSION_2_1
            if ((LIN_DIAG_SEND_FRAME.frame.NAD != LIN_FUNCTIONAL_NAD) && (LIN_DIAG_SEND_FRAME.frame.NAD != 0x00u))
#else
            if (LIN_DIAG_SEND_FRAME.frame.NAD != 0x00u)
#endif /* ifdef LIN_PROTOCOL_VERSION_2_1 */
            {
               g_lin_tp_rx_preNAD = LIN_DIAG_SEND_FRAME.frame.NAD;
            }
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
            ld_allow_response();
         }
         else
         {
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
            /* Save NAD for node configuration services prior to working through the routines below. */
            g_lin_tp_rx_preNAD = LIN_DIAG_SEND_FRAME.frame.NAD;
            /* Application cannot send a correct goto sleep without API therefore use
             * a most uncommon combination to signal that the data came from API */
            if ((NAD != 0u) || (length != 0u))
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
            {
               /* Single Frame Transmit */
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
               LIN_DIAG_SEND_FRAME.frame.NAD = NAD;
#else
               LIN_DIAG_SEND_FRAME.frame.NAD = g_lin_prod_id.NAD;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
               LIN_DIAG_SEND_FRAME.frame.PCI = (l_u8)(SINGLE_FRAME | (l_u8)length);
               for(loc_i = 0u; loc_i < LIN_SF_DATA_LEN; loc_i++)
               {
                  if(loc_i<length)
                  {
                     LIN_DIAG_SEND_FRAME.frame.type.CF.diag_data[loc_i] = (*g_lin_tp_ptr_tx_data)[loc_i];
                  }
                  else
                  {
                     LIN_DIAG_SEND_FRAME.frame.type.CF.diag_data[loc_i] = 0xFFu;
                  }
               }
            }
            ld_allow_response();
            g_lin_tp_data_left = 0u;
            /* Save NAD for checking if the response came from the correct slave */
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
#ifdef LIN_PROTOCOL_VERSION_2_1
            if (LIN_DIAG_SEND_FRAME.frame.NAD != LIN_FUNCTIONAL_NAD)
#endif /* ifdef LIN_PROTOCOL_VERSION_2_1 */
            {
               g_lin_tp_rx_preNAD = LIN_DIAG_SEND_FRAME.frame.NAD;
            }
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
         }
         j2602_allow_slaveresponse();
      }
   }
}

/* ---------------------------------------------------------------------------
 *  void lin_tp_receive_frame(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function handles a received diagnostic frame checks formatting
           configures for eventual consecutive frame receive etc.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
static void lin_tp_receive_frame(void)
{
   l_u8 loc_i;
   l_u16  loc_length;
#ifdef J2602_SLAVE
   l_u8 ret_val;
#endif /* end #ifdef J2602_SLAVE */

#if ((defined LIN_SLAVE)||(defined J2602_SLAVE))
   /* Stop Slave from sending further Slaveresponses from previous Masterrequests */
#ifdef LIN_PROTOCOL_VERSION_2_1
   if (LIN_DIAG_RECEIVE_FRAME.frame.NAD != LIN_FUNCTIONAL_NAD)
#endif /* ifdef LIN_PROTOCOL_VERSION_2_1 */
   {
      g_lin_tp_tx_rx_ctrl.tx_on = 0u;
      g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;
      /* clear timeout counter - can be done here as the previous slave response transmission is stopped. */
      lin_tp_clear_timeout();
      g_lin_tp_stat.tx_stat = LD_COMPLETED;
      /* In case a response with consecutive frames is disrupted by a node configuration service,
         the clean up (lin_tp_tx_part_complete) might send the remaining consecutive frames which shall
         not happen. Therefore set the remaining data to zero. */
      g_lin_tp_data_left = 0u;
#ifdef J2602_SLAVE
      /* If a previous master request had allowed a slave response, but that response was never fetched
       * a slave response must be forbidden - if it is eventually allowed, this flag will be set later. */
      g_j2602_status.flags.sresp = 0u;
      g_j2602_status.flags.tx_resp = 0u;
#endif /* end #ifdef J2602_SLAVE */
   }

   /* check if config */
   if (((LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.NAD) ||
        (LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_BROADCAST_NAD) ||
        (LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.Initial_NAD)) &&
       ((LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0xF0u) == SINGLE_FRAME) &&
#ifndef USER_DEFINED_B4_SERVICE
       ((LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID >= 0xB0u) && (LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID <= 0xB7u))
#else
       /* Accept only B0...B7 without B4 service here. */
       (((LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID >= 0xB0u) && (LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID <= 0xB3u)) ||
       ((LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID >= 0xB5u) && (LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID <= 0xB7u)))
#endif
      )
   {
      g_lin_rx_ctrl.flag.rx_multiframe_msg = L_RESET;
#ifndef LIN_PROTOCOL_VERSION_1_3
      lin_handle_config();
#endif /* #ifndef LIN_PROTOCOL_VERSION_1_3 */
   }
   else
   {
      if ((LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.NAD) || (LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_BROADCAST_NAD))
      {
         switch(LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0xF0u)
         {
            case(SINGLE_FRAME):
               /* check if appl. wants to receive a message
                * if so fill buffer and set length for appl.
                * else clear message and wait for next. */
               if((g_lin_tp_ptr_rx_size != 0u) && (g_lin_tp_ptr_rx_data != 0u) &&
                  ((LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu) <= 0x06u) && ((LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu) > 0u))
               {
                  if(*g_lin_tp_ptr_rx_size >= ((l_u16)LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x000Fu) )
                  {
                     *g_lin_tp_ptr_rx_size = ((l_u16)LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x000Fu);
                     for(loc_i = 0u; loc_i < (l_u8)(*g_lin_tp_ptr_rx_size); loc_i++)
                     {
                        (*g_lin_tp_ptr_rx_data)[loc_i] = LIN_DIAG_RECEIVE_FRAME.frame.type.CF.diag_data[loc_i];
                     }
                     if(g_lin_tp_stat.rx_stat < LD_COMPLETED)
                     {
                        g_lin_tp_stat.rx_stat = LD_COMPLETED;
                     }
                     g_lin_tp_tx_rx_ctrl.rx_cf_on = 0u;
                     g_lin_tp_tx_rx_ctrl.rx_t_out_on = 0u;
                     g_lin_rx_ctrl.flag.rx_multiframe_msg = L_RESET;
                  }
               }
#ifdef J2602_SLAVE
               else
               {
                  ret_val = j2602_callback_configuration_request(LIN_DIAG_RECEIVE_FRAME.byte, LIN_DIAG_SEND_FRAME.byte);
                  if (ret_val == LD_RESPONSE)
                  {
                     j2602_allow_slaveresponse();
                  }
               }
#endif /*end #ifdef J2602_SLAVE */
               break;

            case(FIRST_FRAME):
               /* Check if appl. wants to receive a message
                * if so fill buffer and config tp for cf receive
                * else clear message and wait for next. */
               loc_length = (l_u16)((l_u16)(((l_u16)LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x000Fu) << 8u) | (l_u16)LIN_DIAG_RECEIVE_FRAME.frame.type.FF.LEN);
               if(loc_length > LIN_SF_DATA_LEN)
               {
                  if((g_lin_tp_ptr_rx_size != 0u) && (g_lin_tp_ptr_rx_data != 0u))
                  {
                     if(*g_lin_tp_ptr_rx_size >= loc_length)
                     {
                        g_lin_rx_ctrl.flag.rx_multiframe_msg = L_SET;
                        *g_lin_tp_ptr_rx_size = loc_length;
                        for(loc_i = 0u; loc_i < LIN_FF_DATA_LEN; loc_i++)
                        {
                           (*g_lin_tp_ptr_rx_data)[loc_i] = LIN_DIAG_RECEIVE_FRAME.frame.type.CF.diag_data[loc_i + 1u];
                        }
                        /* Keep used NAD in mind */
                        g_lin_tp_rx_preNAD = LIN_DIAG_RECEIVE_FRAME.frame.NAD;
                        /* Configure CF receive.... */
                        g_lin_tp_rx_data_left = loc_length - LIN_FF_DATA_LEN;
                        g_lin_tp_tx_rx_ctrl.rx_cf_on = 1u;
                        g_lin_tp_tx_rx_ctrl.rx_cf_num = 1u;
                        g_lin_tp_tx_rx_ctrl.rx_t_out_on = 1u;
                        g_lin_tp_rx_data_index = LIN_FF_DATA_LEN;
                     }
                     else
                     {
#ifdef J2602_SLAVE
                        ret_val = j2602_callback_configuration_request(LIN_DIAG_RECEIVE_FRAME.byte, LIN_DIAG_SEND_FRAME.byte);
                        if (ret_val == LD_RESPONSE)
                        {
                           j2602_allow_slaveresponse();
                        }
                        else
#endif /*end #ifdef J2602_SLAVE */
                        {
                           /* buffer too small, set error in status */
                           g_lin_tp_stat.rx_stat = LD_FAILED;
                           g_lin_tp_tx_rx_ctrl.rx_cf_on = 0u;
                           g_lin_tp_tx_rx_ctrl.rx_cf_num = 0u;
                           g_lin_tp_tx_rx_ctrl.rx_t_out_on = 0u;
                        }
                     }
                  }
               }
               break;

            case(CONSECUTIVE_FRAME):
               /* check if tp wants to receive and if frame number is right
                * if so fill buffer */
               if(g_lin_tp_tx_rx_ctrl.rx_cf_on == 1u)
               {
                  if((LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_tp_rx_preNAD) && (g_lin_tp_tx_rx_ctrl.rx_cf_num == (LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu)))
                  {
                     /* clear timeout counter for receive */
                     lin_tp_clear_timeout();
                     /* check how many data to receive */
                     if(g_lin_tp_rx_data_left>LIN_CF_DATA_LEN)
                     {
                        loc_length = LIN_CF_DATA_LEN;
                        g_lin_tp_rx_data_left-=LIN_CF_DATA_LEN;
                        g_lin_tp_tx_rx_ctrl.rx_cf_on = 1u;
                        g_lin_tp_tx_rx_ctrl.rx_cf_num++;
                        g_lin_tp_tx_rx_ctrl.rx_cf_num = (l_u8)(g_lin_tp_tx_rx_ctrl.rx_cf_num % 16u);
                     }
                     else
                     {
                        loc_length = g_lin_tp_rx_data_left;
                        g_lin_tp_rx_data_left=0u;
                        g_lin_tp_tx_rx_ctrl.rx_cf_on = 0u;
                        g_lin_tp_tx_rx_ctrl.rx_cf_num = 0u;
                        g_lin_rx_ctrl.flag.rx_multiframe_msg = L_RESET;
                        if(g_lin_tp_stat.rx_stat < LD_COMPLETED)
                        {
                           g_lin_tp_stat.rx_stat = LD_COMPLETED;
                        }
                        g_lin_tp_tx_rx_ctrl.rx_cf_on = 0u;
                        g_lin_tp_tx_rx_ctrl.rx_t_out_on = 0u;
                     }
                     /* Verify that length is ALWAYS smaller or equal than reserved buffer size. */
                     if ((g_lin_tp_rx_data_index + loc_length) <= *g_lin_tp_ptr_rx_size)
                     {
                        for(loc_i = 0u; loc_i < loc_length; loc_i++)
                        {
                           (*g_lin_tp_ptr_rx_data)[g_lin_tp_rx_data_index + loc_i] = LIN_DIAG_RECEIVE_FRAME.frame.type.CF.diag_data[loc_i];
                        }
                        g_lin_tp_rx_data_index += loc_length;
                     }
                  }
                  else
                  {
                     if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
                     {
                        g_lin_tp_stat.rx_stat = LD_WRONG_SN;
                     }
                     g_lin_tp_tx_rx_ctrl.rx_cf_on = 0u;
                     g_lin_tp_tx_rx_ctrl.rx_t_out_on = 0u;
                  }
               }
#ifdef J2602_SLAVE
               else
               {
                  ret_val = j2602_callback_configuration_request(LIN_DIAG_RECEIVE_FRAME.byte, LIN_DIAG_SEND_FRAME.byte);
                  if (ret_val == LD_RESPONSE)
                  {
                     j2602_allow_slaveresponse();
                  }
               }
#endif /*end #ifdef J2602_SLAVE */
               break;
            default:
#ifdef J2602_SLAVE
               ret_val = j2602_callback_configuration_request(LIN_DIAG_RECEIVE_FRAME.byte, LIN_DIAG_SEND_FRAME.byte);
               if (ret_val == LD_RESPONSE)
               {
                  j2602_allow_slaveresponse();
               }
#endif /*end #ifdef J2602_SLAVE */
               break;
         } /*end switch */
      }
#ifdef LIN_PROTOCOL_VERSION_2_1
      /* Functional NAD requests are ignored when another diagnostic
       * communication is being received or transmitted,
       * Funcational NAD requests are always single frame requests
       */
      if (LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_FUNCTIONAL_NAD)
      {
         if ((g_lin_tp_tx_rx_ctrl.rx_cf_on == 0u) && (g_lin_tp_tx_rx_ctrl.tx_on == 0u))
         {
            if ((LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu) <= LIN_SF_DATA_LEN)
            {
               if ((g_lin_tp_ptr_rx_size != 0u) && (g_lin_tp_ptr_rx_data != 0u))
               {
                  *g_lin_tp_ptr_rx_size = ((l_u16)LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu);
                  for(loc_i = 0u; loc_i < (l_u8)(*g_lin_tp_ptr_rx_size); loc_i++)
                  {
                     (*g_lin_tp_ptr_rx_data)[loc_i] = LIN_DIAG_RECEIVE_FRAME.frame.type.CF.diag_data[loc_i];
                  }
                  if(g_lin_tp_stat.rx_stat < LD_COMPLETED)
                  {
                     g_lin_tp_stat.rx_stat = LD_COMPLETED;
                  }
                  g_lin_tp_tx_rx_ctrl.rx_cf_on = 0u;
                  g_lin_tp_tx_rx_ctrl.rx_t_out_on = 0u;
               }
            }
         }
      }
#endif /* end #ifdef LIN_PROTOCOL_VERSION_2_1 */
      if(LIN_DIAG_RECEIVE_FRAME.frame.NAD == 0x0u)
      {
         g_lin_status_word.flag.goto_sleep = L_SET;
//				if (sysStatus.sleepFlag == ESystemSleepAwake)
//						sysStatus.sleepFlag = ESystemSleepPrepare;
      }
#ifdef J2602_SLAVE
      ret_val = j2602_callback_configuration_request(LIN_DIAG_RECEIVE_FRAME.byte, LIN_DIAG_SEND_FRAME.byte);
      if (ret_val == LD_RESPONSE)
      {
         j2602_allow_slaveresponse();
      }
#endif /*end #ifdef J2602_SLAVE */
   }
#else /* #if ((defined LIN_SLAVE)||(defined J2602_SLAVE)) */
   /* Check if response came from the correct slave */
   if (g_lin_tp_rx_preNAD == LIN_DIAG_RECEIVE_FRAME.frame.NAD)
   {
      switch(LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0xF0u)
      {
         case(SINGLE_FRAME):
            if(g_lin_tp_tx_rx_ctrl.rx_cf_on == 1u)
            {
               if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
               {
                  g_lin_tp_stat.rx_stat = LD_WRONG_SN;
               }
               ld_response_failed();
            }
            else
            {
               if((LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu) <= LIN_SF_DATA_LEN )
               {
                  g_lin_rx_ctrl.flag.rx_multiframe_msg = L_RESET;
                  /* clear transmission request of TP and transmission buffer */
                  g_lin_tp_tx_rx_ctrl.tx_on = 0u;
                  g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;
                  /* check if config */
                  if(((LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID >= 0xF0u)
                          && (LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID <=0xF7u))
                          || (LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID == 0x7Fu))
                  {
#ifndef LIN_PROTOCOL_VERSION_1_3
                     if ((g_lin_config_status > LD_SERVICE_IDLE)
                             && (g_lin_config_status < LD_SERVICE_ERROR))
                     {
                        g_lin_config_status = LD_RESPONSE_FINISHED;
                     }
#endif /* end #ifndef LIN_PROTOCOL_VERSION_1_3 */

                     ld_response_finished();
                  }
                  else
                  {
                     /* check if appl. wants to receive a message and if buffer is big enough
                      * if so fill buffer and set length for appl.
                      * else clear message and wait for next. */
                     if((*g_lin_tp_ptr_rx_size >= ((l_u16)LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu))
                             && (g_lin_tp_ptr_rx_data != 0u)
                             && (g_lin_tp_ptr_rx_nad != 0u))
                     {
                        *g_lin_tp_ptr_rx_size = ((l_u16)LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu);
                        *g_lin_tp_ptr_rx_nad = LIN_DIAG_RECEIVE_FRAME.frame.NAD;
                        for(loc_i = 0u; loc_i < *g_lin_tp_ptr_rx_size; loc_i++)
                        {    /* SID will be the first databyte in the buffer */
                           (*g_lin_tp_ptr_rx_data)[loc_i] = LIN_DIAG_RECEIVE_FRAME.frame.type.CF.diag_data[loc_i];
                        }
                        ld_response_finished();
                     } /* end if */
                  } /* end else */
               } /* end if */
               else
               {
                  /* Reception failed */
                  if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
                  {
                     g_lin_tp_stat.rx_stat = LD_FAILED;
                  }
                  ld_response_failed();
               }
            }
            break;

         case(FIRST_FRAME):
            if(g_lin_tp_tx_rx_ctrl.rx_cf_on == 1u)
            {
               if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
               {
                  g_lin_tp_stat.rx_stat = LD_WRONG_SN;
               }
               ld_response_failed();
            }
            else
            {
               /* clear transmission request of TP */
               g_lin_tp_tx_rx_ctrl.tx_on = 0u;
               g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;
               /* Check if appl. wants to receive a message
                * if so fill buffer and config tp for cf receive
                * else clear message and wait for next. */
               loc_length = ((l_u16)(((l_u16)LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu) << 8u) | (l_u16)LIN_DIAG_RECEIVE_FRAME.frame.type.FF.LEN);
               if((loc_length > LIN_SF_DATA_LEN)
                       && (*g_lin_tp_ptr_rx_size >= loc_length)
                       && (g_lin_tp_ptr_rx_data != 0u)
                       && (g_lin_tp_ptr_rx_nad != 0u))
               {
                  g_lin_rx_ctrl.flag.rx_multiframe_msg = L_SET;
                  /* clear transmission request of TP */
                  g_lin_tp_tx_rx_ctrl.tx_on = 0u;
                  g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;

                  *g_lin_tp_ptr_rx_size = loc_length;
                  *g_lin_tp_ptr_rx_nad = LIN_DIAG_RECEIVE_FRAME.frame.NAD;
                  for(loc_i = 0u; loc_i < LIN_FF_DATA_LEN; loc_i++)
                  {
                     (*g_lin_tp_ptr_rx_data)[loc_i] = LIN_DIAG_RECEIVE_FRAME.frame.type.CF.diag_data[loc_i + 1u];
                  }
                  /* Configure CF receive */
                  g_lin_tp_rx_data_left = loc_length - LIN_FF_DATA_LEN;
                  g_lin_tp_tx_rx_ctrl.rx_cf_on = 1u;
                  g_lin_tp_tx_rx_ctrl.rx_cf_num = 1u;
                  g_lin_tp_tx_rx_ctrl.rx_t_out_on = 1u;
                  g_lin_tp_rx_data_index = LIN_FF_DATA_LEN;
                  /* reset the p2 timer */
                  g_ld_timer_p2 = 0xFFFFu;
               }
               else
               {
                  /* Reception failed */
                  if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
                  {
                     g_lin_tp_stat.rx_stat = LD_FAILED;
                  }
                  ld_response_failed();
               }
            }
         break;

         case(CONSECUTIVE_FRAME):
            /* check if tp wants to receive and if frame number is right
             * if so fill buffer */
            if(g_lin_tp_tx_rx_ctrl.rx_cf_on == 1u)
            {
               if(g_lin_tp_tx_rx_ctrl.rx_cf_num == (LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0x0Fu))
               {
                  /* check how many data to receive */
                  /* clear timeout counter for receive */
                  lin_tp_clear_timeout();
                  if(g_lin_tp_rx_data_left > LIN_CF_DATA_LEN)
                  {
                     loc_length = LIN_CF_DATA_LEN;
                     g_lin_tp_rx_data_left -= LIN_CF_DATA_LEN;
                     g_lin_tp_tx_rx_ctrl.rx_cf_on = 1u;
                     g_lin_tp_tx_rx_ctrl.rx_cf_num++;
                     g_lin_tp_tx_rx_ctrl.rx_cf_num %= 16u;
                  }
                  else
                  {
                     loc_length = g_lin_tp_rx_data_left;
                     g_lin_tp_rx_data_left = 0u;
                     g_lin_tp_tx_rx_ctrl.rx_cf_num = 0u;
                     ld_response_finished();
                  }
                  /* Verify that length is ALWAYS smaller or equal than reserved buffer size. */
                  if ((g_lin_tp_rx_data_index + loc_length) <= *g_lin_tp_ptr_rx_size)
                  {
                     for(loc_i = 0u; loc_i < loc_length; loc_i++)
                     {
                        (*g_lin_tp_ptr_rx_data)[g_lin_tp_rx_data_index + loc_i] = LIN_DIAG_RECEIVE_FRAME.frame.type.CF.diag_data[loc_i];
                     }
                     g_lin_tp_rx_data_index += loc_length;
                  }
               }
               else
               {
                  if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
                  {
                     g_lin_tp_stat.rx_stat = LD_WRONG_SN;
                  }
                  ld_response_failed();
               }
            }
            else
            {
               /* Reception failed */
               if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
               {
                  g_lin_tp_stat.rx_stat = LD_FAILED;
               }
               ld_response_failed();
            }
         break;

         default:
         break;
      } /* end switch */
   } /* end if NAD check */
   else
   {
      /* Reception failed - functional NAD request are not answered to
       *                  - Goto Sleeps are not answered to (0x00) */
      if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
      {
         g_lin_tp_stat.rx_stat = LD_FAILED;
      }
      ld_response_failed();
   } /* end else NAD check */
#endif /* #if ((defined LIN_SLAVE)||(defined J2602_SLAVE)) */
}

/* ---------------------------------------------------------------------------
 *  void ld_tp_task(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  Cyclic diagnostic task for cooked transport layer.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void ld_tp_task(void)
{
#ifdef J2602_SLAVE
   l_u8 ret_val;
#endif /*end #ifdef J2602_SLAVE */

   if(g_lin_rx_ctrl.flag.rx_diag == 1u)
   {
      g_lin_rx_ctrl.flag.rx_diag = 0u;
      lin_tp_receive_frame();
   }
#ifdef J2602_SLAVE
   if (g_lin_rx_ctrl.flag.rx_config == 1u)
   {
      g_lin_rx_ctrl.flag.rx_config = 0u;
      ret_val = j2602_callback_configuration_request(j2602_config_frame.byte, LIN_DIAG_SEND_FRAME.byte);
      if (ret_val == LD_RESPONSE)
      {
         j2602_allow_slaveresponse();
      }
   }
#endif /*end #ifdef J2602_SLAVE */
   if((g_lin_tp_tx_rx_ctrl.tx_t_out_on==1u) || (g_lin_tp_tx_rx_ctrl.rx_t_out_on==1u))
   {
      lin_tp_inc_timeout_counter();
      if(g_lin_tp_timeoutcounter > LIN_TP_COUNTER_MAX)
      {
         if(g_lin_tp_tx_rx_ctrl.tx_t_out_on)
         {
            if(g_lin_tp_stat.tx_stat <= LD_COMPLETED)
            {
               g_lin_tp_stat.tx_stat = LD_N_AS_TIMEOUT;
            }
            g_lin_tp_tx_rx_ctrl.tx_on = 0u;
            g_lin_tp_tx_rx_ctrl.tx_t_out_on = 0u;
         }
         if(g_lin_tp_tx_rx_ctrl.rx_t_out_on)
         {
            if(g_lin_tp_stat.rx_stat <= LD_COMPLETED)
            {
               g_lin_tp_stat.rx_stat = LD_N_CR_TIMEOUT;
            }
            g_lin_tp_tx_rx_ctrl.rx_cf_on = 0u;
            g_lin_tp_tx_rx_ctrl.rx_t_out_on = 0u;
         }
      }
   }
   else
   {
      lin_tp_clear_timeout();
   }
}

#endif /* #if ((defined LIN_SLAVE)||(defined J2602_SLAVE)) */

