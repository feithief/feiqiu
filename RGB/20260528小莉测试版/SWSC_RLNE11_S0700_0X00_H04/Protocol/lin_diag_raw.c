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
********************     Workfile:      lin_diag_raw.c   **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver Protocol Layer                            *
*  FILE-DESCRIPTION:     All Routines for LIN Diagnostic Raw Driver           *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 561         $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2018-03-16#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_diag_raw.c
@brief All Routines for LIN Diagnostic Raw Driver
 */
/* ===========================================================================
 *  Header files
 * ==========================================================================*/

#include "genLinConfig.h"

#ifdef LIN_RAW_API

#include "lin_type.h"
#include "lin_main.h"
#include "lin_config.h"
#include "lin_driver_api.h"
#include "lin_diag_raw.h"

/* ===========================================================================
 *  Global Variables
 * ==========================================================================*/

static l_u8 lin_diag_raw_rx_queue[(8u * LIN_DIAG_RAW_RX_QUEUE_LENGTH) + 1u];
static l_u8 lin_diag_raw_tx_queue[(8u * LIN_DIAG_RAW_TX_QUEUE_LENGTH) + 1u];
static l_u8 lin_diag_raw_rx_write_idx;
static l_u8 lin_diag_raw_rx_read_idx;
#if ((defined LIN_PROTOCOL_VERSION_2_0) || ((defined LIN_PROTOCOL_VERSION_2_1) && (!defined LIN_PROTOCOL_VERSION_2_2)))
l_u8 lin_diag_raw_tx_write_idx;
l_u8 lin_diag_raw_tx_read_idx;
#else
static l_u8 lin_diag_raw_tx_write_idx;
static l_u8 lin_diag_raw_tx_read_idx;
#endif /* #if ((defined LIN_PROTOCOL_VERSION_2_0) || ((defined LIN_PROTOCOL_VERSION_2_1) && (!defined LIN_PROTOCOL_VERSION_2_2))) */

/* ===========================================================================
 *  Functions
 * ==========================================================================*/

static l_u8 lin_diag_raw_increase_index(l_u8 index,l_u8 queue_length);
static void lin_diag_raw_push_rx_queue(const l_u8* const data);
static void lin_raw_receive_frame(void);

/* ---------------------------------------------------------------------------
 *  l_u8 lin_diag_raw_init(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function initializes the RAW transport layer for LIN diagnostics.
   @pre    LIN driver initialized
   @param  void
   @retval Zero If the initialization failed
   @retval Non-zero If the initialization succeeded
*/
l_u8 lin_diag_raw_init(void)
{
   lin_diag_raw_rx_write_idx  = 0u;
   lin_diag_raw_rx_read_idx   = 0u;
   lin_diag_raw_tx_write_idx  = 0u;
   lin_diag_raw_tx_read_idx   = 0u;
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
#ifndef LIN_PROTOCOL_VERSION_1_3
   g_lin_config_status = LD_SERVICE_IDLE;
   g_lin_tp_timeoutcounter = 0u;
#endif /* ifndef LIN_PROTOCOL_VERSION_1_3 */
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
   return 1u;
}

/* ---------------------------------------------------------------------------
 *  l_u8 lin_diag_raw_increase_index(l_u8 index,l_u8 queue_length);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function increases a queue index of the RAW transport layer.
   @pre    LIN driver initialized
   @param  l_u8 index                  Index which should be increased
   @param  l_u8 queue_length           Size of queue
   @retval Increased index
*/
static l_u8 lin_diag_raw_increase_index(l_u8 index,l_u8 queue_length)
{
   l_u8 result_idx = index;

   result_idx += 8u;
   if(result_idx >= queue_length)
   {
      result_idx = 0u;
   }
   return result_idx;
}


/* ---------------------------------------------------------------------------
 *  l_u8 ld_raw_rx_status(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function returns the current state of the RX queue of the
           RAW transport layer.
   @pre    LIN driver initialized
   @param  void
   @retval LD_NO_DATA        The queue is empty.
   @retval LD_DATA_AVAILABLE The queue contains data to be fetched.
   @retval LD_RECEIVE_ERROR  LIN protocol error occurred during the transfer
*/
l_u8 ld_raw_rx_status(void)
{
   l_u8 ret_val = 0u;
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
   if((g_lin_status_word.flag.last_pid == 0x7Du) && (0u != g_lin_status_word.flag.error_in_resp))
#else
   if((g_lin_status_word.flag.last_pid == 0x3Cu) && (0u != g_lin_status_word.flag.error_in_resp))
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
   {
      ret_val = LD_RECEIVE_ERROR;
   }
   else
   {
      if(lin_diag_raw_rx_write_idx == lin_diag_raw_rx_read_idx)
      {
         ret_val = LD_NO_DATA;
      }
      else
      {
         if(lin_diag_raw_rx_write_idx != lin_diag_raw_rx_read_idx)
         {
            ret_val = LD_DATA_AVAILABLE;
         }
         else
         {
            /* case should never been reached */
            ret_val = 0xFFu;
         }
      }
   }
   return ret_val;
}

/* ---------------------------------------------------------------------------
 *  l_u8 ld_raw_tx_status(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function returns the current state of the TX queue of the
           RAW transport layer.
   @pre    LIN driver initialized
   @param  void
   @retval LD_QUEUE_EMPTY     The queue is empty.
   @retval LD_QUEUE_AVAILABLE The queue contains data but has slots free for more data.
   @retval LD_QUEUE_FULL      The queue contains data and no more data can be put into the queue.
   @retval lD_TRANSMIT_ERROR  LIN protocol error occurred during the transfer
*/
l_u8 ld_raw_tx_status(void)
{
   l_u8 ret_val = 0u;

#if ((defined LIN_MASTER)||(defined J2602_MASTER))
   if((g_lin_status_word.flag.last_pid == 0x3Cu) && (0u != g_lin_status_word.flag.error_in_resp))
#else
   if((g_lin_status_word.flag.last_pid == 0x7Du) && (0u != g_lin_status_word.flag.error_in_resp))
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
   {
      ret_val = LD_TRANSMIT_ERROR;
   }
   else
   {
      if(lin_diag_raw_tx_write_idx == lin_diag_raw_tx_read_idx)
      {
         ret_val = LD_QUEUE_EMPTY;
      }
      else
      {
         if(lin_diag_raw_increase_index(lin_diag_raw_tx_write_idx, (8u*LIN_DIAG_RAW_TX_QUEUE_LENGTH)) == lin_diag_raw_tx_read_idx)
         {
            ret_val = LD_QUEUE_FULL;
         }
         else
         {
            if(lin_diag_raw_tx_write_idx != lin_diag_raw_tx_read_idx)
            {
               ret_val = LD_DATA_AVAILABLE;
            }
            else
            {
               /* case should never been reached */
               ret_val = 0xFFu;
            }
         }
      }
   }
   return ret_val;
}

/* ---------------------------------------------------------------------------
 *  void lin_diag_raw_push_rx_queue(const l_u8* const data);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function puts a frame in the RX queue of the RAW transport layer.
   @pre    LIN driver initialized
   @param  l_u8* data   pointer to the whole frame which was received
   @retval void
*/
static void lin_diag_raw_push_rx_queue(const l_u8* const data)
{
   l_u8 loc_i;
   /* (MISRA-C:2004 11.4/A, e927) A cast should not be performed between a pointer to object type and a different pointer to object type
    * Reason: make the pointer (req. by LIN specification) accessible as array. */
   const l_u8 (*lin_diag_raw_tmp_arr)[] = (const l_u8 (*)[])data; /*lint !e927*/

   for(loc_i = 0u; loc_i < 8u; loc_i++)
   {
      lin_diag_raw_rx_queue[lin_diag_raw_rx_write_idx + loc_i] = (*lin_diag_raw_tmp_arr)[loc_i];
   }
   lin_diag_raw_rx_write_idx = lin_diag_raw_increase_index(lin_diag_raw_rx_write_idx, (8u*LIN_DIAG_RAW_RX_QUEUE_LENGTH));
#ifndef LIN_PROTOCOL_VERSION_1_3
   /* Reset timeout if a slave response was received */
   g_lin_tp_timeoutcounter = 0u;
#endif /* ifndef LIN_PROTOCOL_VERSION_1_3 */
}

/* ---------------------------------------------------------------------------
 *  void ld_get_raw(l_u8* const data);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function fetches a frame from the RX queue of the RAW transport layer.
   @pre    LIN driver initialized, data in queue
   @param  l_u8* data   pointer to databuffer for the frame data
   @retval void
*/
void ld_get_raw(l_u8* const data)
{
   l_u8 loc_i;
   /* (MISRA-C:2004 11.4/A, e927) A cast should not be performed between a pointer to object type and a different pointer to object type
    * Reason: make the pointer (req. by LIN specification) accessible as array. */
   l_u8 (*lin_diag_raw_tmp_arr)[] = (l_u8 (*)[])data; /*lint !e927*/

   for(loc_i = 0u; loc_i < 8u; loc_i++)
   {
      /* Possible access / creation of out-of-bounds pointer (8 beyond end of data) by operator '['
       * Reason: This code part is created to only copy dedicated bytes from frame queue to frame buffer, will not
       * go out of bounds. */
      (*lin_diag_raw_tmp_arr)[loc_i] = lin_diag_raw_rx_queue[lin_diag_raw_rx_read_idx + loc_i]; /*lint !e661, !e662*/
   }
   lin_diag_raw_rx_read_idx = lin_diag_raw_increase_index(lin_diag_raw_rx_read_idx, (8u*LIN_DIAG_RAW_RX_QUEUE_LENGTH));
}

/* ---------------------------------------------------------------------------
 *  void ld_put_raw(const l_u8* const data);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function puts a frame in the TX queue of the RAW transport layer.
   @pre    LIN driver initialized
   @param  l_u8* data   pointer to the whole frame which should be transmitted
   @retval void
*/
void ld_put_raw(const l_u8* const data)
{
   l_u8 loc_i;
   /* (MISRA-C:2004 11.4/A, e927) A cast should not be performed between a pointer to object type and a different pointer to object type
    * Reason: make the pointer (req. by LIN specification) accessible as array. */
   const l_u8 (*lin_diag_raw_tmp_arr)[] = (const l_u8 (*)[])data; /*lint !e927*/

   /* Possible out of bounds access: The necessary space for putting data into queue is checked
    * prior to this function, so enough memory space is available. */
   for(loc_i = 0u; loc_i < 8u; loc_i++)
   {
      /* Possible access / creation of out-of-bounds pointer (8 beyond end of data) by operator '['
       * Reason: This code part is created to only copy dedicated bytes from frame queue to frame buffer, will not
       * go out of bounds. */
      lin_diag_raw_tx_queue[lin_diag_raw_tx_write_idx + loc_i]= (*lin_diag_raw_tmp_arr)[loc_i]; /*lint !e661, !e662*/
   }
   lin_diag_raw_tx_write_idx = lin_diag_raw_increase_index(lin_diag_raw_tx_write_idx, (8u*LIN_DIAG_RAW_TX_QUEUE_LENGTH));
   j2602_allow_slaveresponse();
#ifndef LIN_PROTOCOL_VERSION_1_3
   /* Reset timeout if a master request is being put into queue */
   g_lin_tp_timeoutcounter = 0u;
#endif /* ifndef LIN_PROTOCOL_VERSION_1_3 */
}

/* ---------------------------------------------------------------------------
 *  void lin_diag_raw_pop_tx_queue(l_u8* data);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function fetches a frame from the TX queue of the RAW transport layer.
   @note   The data of the buffer is immediatelly copied into the transfer buffer
           (slave response structure), therefore no separate buffer is necessary for
           keeping data consistent.
   @pre    LIN driver initialized, data in queue
   @param  l_u8* data   pointer to databuffer for the frame data
   @retval void
*/
l_u8* lin_diag_raw_pop_tx_queue(void)
{
   l_u8* ret_ptr;

   ret_ptr = &lin_diag_raw_tx_queue[lin_diag_raw_tx_read_idx];
   lin_diag_raw_tx_read_idx = lin_diag_raw_increase_index(lin_diag_raw_tx_read_idx, (8u*LIN_DIAG_RAW_TX_QUEUE_LENGTH));
#ifndef LIN_PROTOCOL_VERSION_1_3
   /* Reset timeout if a is being sent on the bus */
   g_lin_tp_timeoutcounter = 0u;
#endif /* ifndef LIN_PROTOCOL_VERSION_1_3 */
   return ret_ptr;
}

/* ---------------------------------------------------------------------------
 *  void lin_raw_receive_frame(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  This function will handle a received diagnostic frame.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
static void lin_raw_receive_frame(void)
{
#ifdef J2602_SLAVE
   l_u8 ret_val;
#endif /* end #ifdef J2602_SLAVE */

#if ((defined LIN_MASTER)||(defined J2602_MASTER))
#ifndef LIN_PROTOCOL_VERSION_1_3
   /* check if config */
   if (((g_lin_diag_sresp.frame.PCI & 0xF0u) == SINGLE_FRAME)
         && (((g_lin_diag_sresp.frame.type.SF.SID >= 0xF0u)
         &&   (g_lin_diag_sresp.frame.type.SF.SID <= 0xF7u))
         ||  (g_lin_diag_sresp.frame.type.SF.SID == 0x7Fu)))
   {
      if ((g_lin_config_status != LD_SERVICE_IDLE) && (g_lin_config_status != LD_SERVICE_ERROR))
      {
         g_lin_config_status = LD_RESPONSE_FINISHED;
      }
   }
   else
   {
      /* fill raw api rx queue */
      lin_diag_raw_push_rx_queue(&g_lin_diag_sresp.frame.NAD);
   }
#else
   lin_diag_raw_push_rx_queue(&g_lin_diag_sresp.frame.NAD);
#endif /* end #ifndef LIN_PROTOCOL_VERSION_1_3 */
#else /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
   /* clear pending slave responses if target NAD is not functional NAD */
#ifdef LIN_PROTOCOL_VERSION_2_1
   if(LIN_DIAG_RECEIVE_FRAME.frame.NAD != LIN_FUNCTIONAL_NAD)
   {
      lin_diag_raw_tx_read_idx = lin_diag_raw_tx_write_idx;
   }
#else
   lin_diag_raw_tx_read_idx = lin_diag_raw_tx_write_idx;
#ifdef J2602_SLAVE
   /* If a previous master request had allowed a slave response, but that response was never fetched
    * a slave response must be forbidden - if it is eventually allowed, this flag will be set later. */
   g_j2602_status.flags.sresp = 0u;
   g_j2602_status.flags.tx_resp = 0u;
#endif /* end #ifdef J2602_SLAVE */
#endif /* end #ifdef LIN_PROTOCOL_VERSION_2_1 */

   /* check if NAD and other data are valid for node configuration services */
   if (((LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.NAD) ||
        (LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_BROADCAST_NAD) ||
        (LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.Initial_NAD)) &&
       ((LIN_DIAG_RECEIVE_FRAME.frame.PCI & 0xF0u) == SINGLE_FRAME) &&
       ((LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID >= 0xB0u) && (LIN_DIAG_RECEIVE_FRAME.frame.type.SF.SID <= 0xB7u)))
   {
      lin_handle_config();
   }
   else
   {
      /* check if NAD is valid for forwarding the request to application */
      if ((LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.NAD) || (LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_BROADCAST_NAD))
      {
         /* fill raw api rx queue */
         lin_diag_raw_push_rx_queue(&LIN_DIAG_RECEIVE_FRAME.frame.NAD);
      }
#ifdef LIN_PROTOCOL_VERSION_2_1
      if(LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_FUNCTIONAL_NAD)
      {
         lin_diag_raw_push_rx_queue(&LIN_DIAG_RECEIVE_FRAME.frame.NAD);
      }
#endif
      /* check if goto sleep cmd was received */
      if(LIN_DIAG_RECEIVE_FRAME.frame.NAD == 0x0u)
      {
         /* PID and successful transfer is not needed here */
         g_lin_status_word.flag.goto_sleep = L_SET;
      }
#ifdef J2602_SLAVE
      /* NAD does not fit, every other case is handled via RAW queues */
      ret_val = j2602_callback_configuration_request(LIN_DIAG_RECEIVE_FRAME.byte, LIN_DIAG_SEND_FRAME.byte);
      if (ret_val == LD_RESPONSE)
      {
         j2602_allow_slaveresponse();
      }
#endif /*end #ifdef J2602_SLAVE */
   }
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
}

/* ---------------------------------------------------------------------------
 *  ld_raw_task(void);
 * --------------------------------------------------------------------------*/
/**
   @brief  Cyclic diagnostic task for RAW transport layer.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void ld_raw_task(void)
{
#ifdef J2602_SLAVE
   l_u8 ret_val;
#endif /*end #ifdef J2602_SLAVE */
#ifndef LIN_PROTOCOL_VERSION_1_3
   l_u8 lin_raw_queue_status;

   /* timeout added */
   lin_raw_queue_status = ld_raw_tx_status();
   if((lin_raw_queue_status == LD_DATA_AVAILABLE) || (lin_raw_queue_status == LD_QUEUE_FULL))
   {
      g_lin_tp_timeoutcounter++;
      if(g_lin_tp_timeoutcounter>LIN_TP_COUNTER_MAX)
      {
         lin_diag_raw_tx_read_idx = lin_diag_raw_tx_write_idx;
      }
   }
   else
   {
      g_lin_tp_timeoutcounter = 0u;
   }
#endif /* end #ifndef LIN_PROTOCOL_VERSION_1_3 */

   if(g_lin_rx_ctrl.flag.rx_diag == 1u)
   {
      g_lin_rx_ctrl.flag.rx_diag = 0u;
      lin_raw_receive_frame();
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
}

#endif /* end #ifdef LIN_RAW_API */
