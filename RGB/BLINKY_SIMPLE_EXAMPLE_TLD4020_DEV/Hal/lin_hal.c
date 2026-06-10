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
********************     Workfile:      lin_hal.c        **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  Infineon Hardware Abstraction Layer                  *
*  FILE-DESCRIPTION:     All Hardware Access Routines for LIN Driver          *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 1744        $                                 *
*       Responsible:     B.Roegl                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2023-02-22#$                                 *
*                                                                             *
******************************************************************************/

/**
@file lin_hal.c
@brief Microcontroller specific implementation of the LIN Driver communication
*/

/* ===========================================================================
 *  Header files
 * ==========================================================================*/

#include "genLinConfig.h"
#include "lin_main.h"
#include "lin_slave_task.h"
#include "lin_driver_api.h"
#include "lin_type.h"
#include "lin_hal.h"
#ifdef LIN_ENABLE_AA_BSM_B5
#include "lin_autoaddressing.h"
#include "ModuleFlash.h"
#include "SystemLinComm.h"
#endif /* ifdef LIN_ENABLE_AA_BSM_B5 */
/* ===========================================================================
 *  Global Variables
 * ==========================================================================*/

LIN_hal_stat_reg_t lin_hal_status_g;
LIN_SLAVE_STATE_t lin_slave_state_g;
t_lin_temp_frame_buffer lin_temp_buffer;
l_u8 lin_ab_timeout_timer;
l_u8 lin_frm_timeout_timer;
t_lin_timeout_ctrl lin_timeout_ctrl;
l_u8 l_txchk_byte_nr;

static l_u8 lin_hal_rx_data_guc;
static l_u8 uc_frame_lin_version;
static l_u8 temp_data_size_uc;
static LIN_TX_DATA_t lin_tx_data_g;
static LIN_FLAGS_t lin_flag_g;
static l_u16 l_chk_sum_ui;

volatile l_u8 lin_ab_t2_state;

#ifdef AUTOBAUD_ENABLED
volatile l_u16 lin_temp_word;
static volatile l_u16 lin_synch_time;
#endif /* end #ifdef AUTOBAUD_ENABLED */

#if ((defined LIN_MASTER)||(defined J2602_MASTER))
static volatile t_lin_master_ctrl lin_masterctrl;
static l_u8 guc_frame_id;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */

/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

static void lin_hal_tx_checksum(l_u8 checksum);
static void lin_hal_tx_byte(l_u8 lin_hal_bytetoTX);
static void l_hal_tx_state_machine(void);

#ifdef AUTOBAUD_ENABLED
static void lin_hal_brreg_calc(void);
static void lin_hal_init_T2 (void);
#endif /* end #ifdef AUTOBAUD_ENABLED */

#if ((defined LIN_MASTER)||(defined J2602_MASTER))
static void l_hal_master_send_sync (void);
static void l_hal_master_send_pid (void);
#else
#define l_hal_master_send_sync()               do { } while (0u)
#define l_hal_master_send_pid()                do { } while (0u)
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */


static int16_t LinHa_Aa_GetShuntCurrent(void);

static void LinHa_Aa_WaitForRisingEdge(void);

static void LinHa_Aa_ConfigOffsetPhase(void);

static void LinHa_Aa_ConfigPreSelectionPhase(void);

static void LinHa_Aa_ConfigSelectionPhase(void);

static void LinHa_Aa_Stop(void);

/* ===========================================================================
 *  Functions
 * ==========================================================================*/

/* ---------------------------------------------------------------------------
 *  void lin_hal_init (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Initializes the HAL layer and UART / SCI
   @param  void
   @retval void
*/
void lin_hal_init (void)
{
   l_u8 loc_i                       = 0u;
   lin_hal_status_g.byte_received   = 0u;
   lin_hal_status_g.ferror          = 0u;
   lin_hal_status_g.res0            = 0u;
   lin_hal_rx_data_guc              = 0u;
   uc_frame_lin_version             = 0u;
   temp_data_size_uc                = L_RESET;
   lin_tx_data_g.checksum           = 0u;
   lin_tx_data_g.data_puc           = 0u;
   lin_tx_data_g.index_uc           = 0u;
   lin_tx_data_g.length_c           = 0u;
   lin_tx_data_g.old_data_uc        = 0u;
   lin_flag_g.res0                  = 0u;
   lin_flag_g.tx_finished           = 0u;
   lin_flag_g.tx_next               = 0u;
   lin_ab_timeout_timer             = 0u;
   lin_frm_timeout_timer            = 0u;
   l_chk_sum_ui                     = 0u;
   l_txchk_byte_nr                  = 0u;
   lin_timeout_ctrl.byte            = 0u;
   for(loc_i = 0u; loc_i < 11u; loc_i++)
   {
      lin_temp_buffer.byte[loc_i] = 0u;
   }
   lin_timeout_ctrl.flag.sys_init  = 1u;
   lin_hal_init_uart();
   lin_timeout_ctrl.flag.sys_init  = 0u;
   lin_ab_t2_state = 0u;
   lin_slave_state_g = idle;
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
   lin_masterctrl.byte = 0u;
   guc_frame_id = 0u;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
   return;
}

#ifdef J2602_SLAVE
/* ---------------------------------------------------------------------------
 *  void j2602_hal_target_reset (l_u8 reset)
 * --------------------------------------------------------------------------*/

/**
   @brief  This function resets the controller after receiving a reset request.
   @pre    LIN driver initialized
   @param  reset  Sets reset flag every time, but reset all errors if zero, keeps all errors if non-zero
   @retval void
 */
void j2602_hal_target_reset (l_u8 reset) {
#ifndef GM_SPEC_ADAPTION
   /* Deleting all errors and set the status to RESET */
   if (reset)
   {
      g_j2602_status.flags.errorstate |= J2602_RESET_STATUS;
   }
   else
   {
      g_j2602_status.flags.errorstate = J2602_RESET_STATUS;
   }
#endif /* #ifdef GM_SPEC_ADAPTION */
   /* Setting of the response data is made when the response header was received
    * The response header will be sent only if the targeted reset was called, so
    * there is no need to make a difference between broadcast and target reset
    * here.
    */
   return;
}
#endif /* end #ifdef J2602_SLAVE */

#if ((defined LIN_MASTER)||(defined J2602_MASTER))
/* ---------------------------------------------------------------------------
 *  void lin_hal_tx_header (l_u8 l_pid)
 * --------------------------------------------------------------------------*/
/**
   @brief  Starts sending the header on the bus
   @pre    LIN driver initialized
   @param  l_pid           PID of the frame
   @retval void
*/
void lin_hal_tx_header (l_u8 l_pid)
{
   /* Send dummy data to UART to send Break */
   LIN_UART_BCON_RUN =     0u; /*lint !e923*/
   LIN_UART_BCON_FDSEL =   BREAK_BRG_FD_SEL; /*lint !e923*/
   LIN_UART_BCON_BG =      BREAK_BRG_VAL; /*lint !e923*/
   LIN_UART_BCON_RUN = 1u; /*lint !e923*/
   LIN_UART_9BITTRANS = 0u; /*lint !e923*/
   lin_hal_tx_char(BREAK_FIELD_k); /*lint !e923*/
   lin_masterctrl.flag.break_transm = 1u;
   guc_frame_id = l_pid;
}


/* ---------------------------------------------------------------------------
 *  void l_hal_master_send_sync (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  After break the master shall send the sync field.
   @pre    LIN driver initialized
   @param  void
   @retval void
 */
static void l_hal_master_send_sync (void)
{
   if ((lin_hal_rx_data_guc == 0u) && (lin_masterctrl.flag.break_transm == 1u))
   {
      /* Send Sync Field data to UART to transmit Sync Field */
      LIN_UART_BCON_RUN = 0u;
      LIN_UART_BCON_FDSEL = BRG_FD_SEL;
      LIN_UART_BCON_BG = BRG_VAL;
      LIN_UART_BCON_RUN = 1u;
      lin_hal_tx_char(SYNC_FIELD_k);
      lin_masterctrl.flag.syncf_transm = 1u;
      lin_masterctrl.flag.break_transm = 0u;
      lin_slave_state_g = break_received;
   }
}


/* ---------------------------------------------------------------------------
 *  void l_hal_master_send_pid (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  After receiving sync field, start PID
   @pre    LIN driver initialized
   @param  void
   @retval void
 */
static void l_hal_master_send_pid (void)
{
   if (lin_masterctrl.flag.syncf_transm == 1u)
   {
      if (lin_hal_rx_data_guc == 0x55u)
      {
         lin_hal_tx_char(guc_frame_id);
         lin_slave_state_g = sync_received;
      }
      else
      {
         lin_slave_state_g = idle;
      }
      lin_masterctrl.flag.syncf_transm = 0u;
   }
   else
   {
      lin_hal_status_g.byte_received = L_SET;
   }

}
#endif /* end #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */

/* ---------------------------------------------------------------------------
 *  static void lin_hal_tx_byte(l_u8 lin_hal_bytetoTX)
 * --------------------------------------------------------------------------*/
/**
   @brief  Transmits one byte on bus
   @pre    LIN driver initialized
   @param  lin_hal_bytetoTX           pointer to frame buffer
   @retval void
*/
static void lin_hal_tx_byte(l_u8 lin_hal_bytetoTX)
{
   /* Store value for receive check later */
   lin_tx_data_g.old_data_uc = lin_hal_bytetoTX;

   /* send data to uart, for transmit */
   /* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
   * Reason: HW access through library */
   lin_hal_tx_char(lin_hal_bytetoTX); /*lint !e923*/

   /* make configuration for tx next data  */
   lin_tx_data_g.length_c--;
   /* check if next field is CRC */
   if(!lin_tx_data_g.length_c)
   {
      lin_flag_g.tx_next = CHECKSUM_NEXT;
   }
   else
   {
      lin_flag_g.tx_next = L_SET;
   }
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_tx_response (l_u8 l_type, const l_u8 l_data[], l_u8 l_len)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function prepares the driver for transmitting a response frame.
   @pre    LIN driver initialized
   @param  l_u8 l_type   LIN type (1.3, 2.0, or 2.1)
   @param  l_u8 l_data[] Databuffer for frame data
   @param  l_u8 l_len    Length information of frame
   @retval void
*/
void lin_hal_tx_response (l_u8 l_type, const l_u8 l_data[], l_u8 l_len)
{
   l_u16 ui_I = 0u;
   l_u8 uc_temp_sum = 0u;

   lin_tx_data_g.index_uc = 0u;
   lin_slave_state_g = tx_data;
   lin_tx_data_g.length_c = l_len;
   g_lin_frame_data_size = l_len;
   if(l_type == LIN_2_X)
   {
      /* For LIN 2.x use PID & Data for CRC */
      l_chk_sum_ui = lin_temp_buffer.frame.PID;
   }
   else
   {
      l_chk_sum_ui = 0u;
   }
   g_lin_irqState = l_sys_irq_disable();
   /* Copy data from main buffer to the tx buffer */
   for(ui_I = 0u; ui_I < l_len; ui_I++)
   {
      lin_temp_buffer.frame.DataBuffer[ui_I] = l_data[ui_I];
   }
   l_sys_irq_restore(g_lin_irqState);

   l_txchk_byte_nr = lin_tx_data_g.length_c - 1u;
   l_chk_sum_ui += lin_temp_buffer.frame.DataBuffer[l_txchk_byte_nr];
   /* if sum is bigger than 16 bit */
   uc_temp_sum = (l_u8)(l_chk_sum_ui >> 8);
   l_chk_sum_ui = l_chk_sum_ui + uc_temp_sum;
   l_chk_sum_ui &= 0xFFu;
   /* frame with only 1 databyte */
   if (l_txchk_byte_nr == 0u)
   {
      lin_tx_data_g.checksum = (l_u8)(~l_chk_sum_ui);
   }
#ifdef J2602_SLAVE
   /* SYNCH Field and valid PID is received. Reset this flag for next frame. */
   if(lin_hal_status_g.isr_rec_sync == 1u)
   {
      lin_hal_status_g.isr_rec_sync = 0u;
   }
#endif /* #ifdef J2602_SLAVE */

   /* start transmit of data fields */
   lin_hal_tx_byte(lin_temp_buffer.frame.DataBuffer[0]);
   lin_temp_buffer.frame.BufferIndex = 1u; /* Index of next data to transmit */
   lin_timeout_ctrl.flag.transm_ongoing = 1u;
   return;
}

/* ---------------------------------------------------------------------------
 *  static void l_hal_tx_state_machine (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function controls the transmission of frame data and checksum
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
static void l_hal_tx_state_machine (void)
{
   /* tx CRC of frame */
   if (lin_flag_g.tx_next == CHECKSUM_NEXT)
   {
      lin_hal_tx_checksum(lin_tx_data_g.checksum);
   }
   /* tx next byte of frame */
   if (lin_flag_g.tx_next == L_SET)
   {
      /* Set state machine to transmit data */
      lin_slave_state_g = tx_data;
      lin_hal_tx_byte(lin_temp_buffer.frame.DataBuffer[lin_temp_buffer.frame.BufferIndex]);
      lin_temp_buffer.frame.BufferIndex ++; /* Index of next data to transmit */
   }
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_tx_wake_up(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function transmits a wake-up pulse on the bus
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: Part of API, if not used MISRA violation will be issued. */
void lin_hal_tx_wake_up(void)
{
   l_u8 loc_data = 0u;

   /* Send dummy data to UART to send Break */
#ifdef LIN_PROTOCOL_VERSION_1_3
   loc_data = 0x80;
#else
   loc_data = 0xe0u;
#endif /* end #ifdef LIN_1_3 */
   /* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
    * Reason: HW access through library */
   lin_hal_tx_char(loc_data); /*lint !e923*/
}

/* ---------------------------------------------------------------------------
 *  void l_hal_txchecksum_calculation(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function calculates the checksum of a publish frame step-wise
           for each byte that is actually transmitted.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void l_hal_txchecksum_calculation(void)
{
   l_u8 uc_temp_sum;

   l_txchk_byte_nr--;
   /*  l_chk_sum_ui += l_txchk_data_p[l_txchk_byte_nr]; */
   l_chk_sum_ui += lin_temp_buffer.frame.DataBuffer[l_txchk_byte_nr];
   /* if sum is bigger than 16 bit */
   uc_temp_sum = (l_u8)(l_chk_sum_ui >> 8);
   l_chk_sum_ui = l_chk_sum_ui + uc_temp_sum;
   l_chk_sum_ui &= 0xFFu;
   if (l_txchk_byte_nr == 0u)
   {
      lin_tx_data_g.checksum = (l_u8)~l_chk_sum_ui;
      l_chk_sum_ui = 0u;
   }
}

/* ---------------------------------------------------------------------------
 *  static void lin_hal_tx_checksum(l_u8 checksum)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function transmits the checksum on the bus.
   @pre    LIN driver initialized
   @param  l_u8 checksum  The checksum of the frame.
   @retval void
*/
static void lin_hal_tx_checksum(l_u8 checksum)
{
   /* set next state in state machine */
   lin_slave_state_g = tx_checksum;

   /* send data to uart, for transmit */
   /* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
   * Reason: HW access through library */
   lin_hal_tx_char(checksum); /*lint !e923*/

   /* store data for receive check later */
   lin_tx_data_g.old_data_uc = checksum;
   /* reset tx state finish transmit of frame */
   lin_flag_g.tx_next = L_RESET;
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_rx_response (l_u8 l_type, l_u8 l_len)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function prepares the driver for receiving a subscribe frame.
   @pre    LIN driver initialized
   @param  l_u8 l_type  The LIN protocol version of the frame.
   @param  l_u8 l_len   The length information of the frame.
   @retval void
*/
void lin_hal_rx_response (l_u8 l_type, l_u8 l_len)
{
   g_lin_frame_data_size = l_len;
   temp_data_size_uc = g_lin_frame_data_size;
   uc_frame_lin_version = l_type;
   lin_slave_state_g = rx_data;
   lin_timeout_ctrl.flag.frame = 1u;
   lin_frm_timeout_timer = 0u;
#ifdef J2602_SLAVE
   /* SYNCH Field and valid PID is received. Reset this flag for next frame. */
   if(lin_hal_status_g.isr_rec_sync == 1u)
   {
      lin_hal_status_g.isr_rec_sync = 0u;
   }
#endif /* #ifdef J2602_SLAVE */
}

/* ---------------------------------------------------------------------------
 *  void l_hal_rx_state_machine (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function steps through the states for receiving frame headers
           and frame data.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void l_hal_rx_state_machine (void)
{
   l_u8 uc_temp_sum = 0u;
   /* next computations depend on receive task state (state machine) */
   switch(lin_slave_state_g)
   {
      /* only valid next data is break/sync */
      case idle:
         break;

      /* if break received next data is sync field */
      case break_received:				 
#ifndef AUTOBAUD_ENABLED
         /* check if sync field else invalid_field error */
         if (lin_hal_rx_data_guc == SYNC_FIELD_k)
         {
            lin_slave_state_g = sync_received;
         }
         else
         {
#ifdef J2602_SLAVE
            /* J2602 slave without baudrate synchronization shall cause an
               error bit set if the sync field is inconsistent. */
            if(lin_hal_status_g.isr_rec_sync == 0u)
            {
               lin_timeout_ctrl.flag.recept_started = 1u;
               lin_hal_set_resp_error(J2602_DATA_ERROR);
            }
#endif /* end #ifdef J2602_SLAVE */
            lin_hal_wait_for_break();
         }
#else
#ifdef LIN_ENABLE_AA_BSM_B5
          /* check if sync field else invalid_field error */
         //if ((lin_hal_rx_data_guc == SYNC_FIELD_k)&&(L_SET == (LinAa_GetActive())))
         if ((lin_hal_rx_data_guc == SYNC_FIELD_k)&&(E_StateMachineStatusIdle != (LIN_AutoAddr_Get_Status())))
#else
          /* check if sync field else invalid_field error */
         if (lin_hal_rx_data_guc == SYNC_FIELD_k)
#endif
         {
            lin_slave_state_g = sync_received;
         }
#endif /* end #ifndef AUTOBAUD_ENABLED */
         break;

      /* if sync field received next data is PID field */
      case sync_received:
         /* after sync field a pid field was received */
         /* reset lin_state_after receive of pid (Protocoll layer will set for further rx/tx action) */
         lin_slave_state_g = idle;
         /* Header of new frame received. Checksum calculation must be started again */
         l_txchk_byte_nr = 0u;
         /* check if pid is valid and if its tx or rx frame */
         lin_slave_task_rx_pid(lin_hal_rx_data_guc);
         break;

      /* if it is rx frame, then data field comes next */
      case rx_data:
         /* store received data and check for type of next field */
         lin_temp_buffer.frame.DataBuffer[(g_lin_frame_data_size - temp_data_size_uc)] = lin_hal_rx_data_guc;
         lin_timeout_ctrl.flag.frame = 1u;
         lin_timeout_ctrl.flag.transm_ongoing = 1u;
         lin_timeout_ctrl.flag.recept_started = 1u;
         lin_frm_timeout_timer = 0u;

         /* calculate temporary checksum (step wise after each byte) */
         if (g_lin_frame_data_size == temp_data_size_uc)
         {
            if (uc_frame_lin_version == LIN_2_X)
            {
               /* For LIN 2.x use PID & Data for CRC */
               l_chk_sum_ui = lin_temp_buffer.frame.PID;
            }
            else
            {
               l_chk_sum_ui = 0u;
            }
         }
         l_chk_sum_ui += lin_temp_buffer.frame.DataBuffer[(g_lin_frame_data_size - temp_data_size_uc)];
         /* if sum is bigger than 16 bit */
         uc_temp_sum = (l_u8) (l_chk_sum_ui >> 8);
         l_chk_sum_ui = l_chk_sum_ui + uc_temp_sum;
         l_chk_sum_ui &= 0xFFu;

         /* get number of data still to receive */
         temp_data_size_uc--;

         /* if no more data to receive got to rx_checksum state */
         if (temp_data_size_uc == 0u)
         {
            lin_slave_state_g = rx_checksum;
         }
         break;

         /* if last data was received next is crc field */
      case rx_checksum:
         /* Store crc data and check if crc is okay */
         lin_temp_buffer.frame.reserved = lin_hal_rx_data_guc;
         l_chk_sum_ui = ((l_u16)(~l_chk_sum_ui)) & 0x00FFu;
#ifdef SUPPORT_TWO_BAUDRATES
         /* reset to higher baudrate if not yet set to a fixed value
            to be able to detect the lower one */
         l_hal_reset_to_high_baudrate();
#endif /* #ifdef SUPPORT_TWO_BAUDRATES */
         /* check if checksum was faulty */
         if (lin_hal_rx_data_guc != (l_chk_sum_ui))
         {
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1))
            lin_hal_set_resp_error();
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
#ifdef J2602_PROTOCOL
            lin_hal_set_resp_error(J2602_CHECKSUM_ERROR);
#endif /* end #ifdef J2602_PROTOCOL */
         }
         else
         {
//#ifdef LIN_ENABLE_AA_BSM_B5
#if 1
            /* notify upper layer about received frame. */
					  if((lin_temp_buffer.frame.DataBuffer[0] == 0x7F) 
							&&(lin_temp_buffer.frame.DataBuffer[1] == 0x06) 
						  &&(lin_temp_buffer.frame.DataBuffer[2] == 0xB5)
						  &&(lin_temp_buffer.frame.DataBuffer[3] == 0xFF)
						  &&(lin_temp_buffer.frame.DataBuffer[4] == 0x7F)		
						  &&(lin_temp_buffer.frame.DataBuffer[5] == D_AUTO_ADDR_STEP1_START_AA)
						  &&(lin_temp_buffer.frame.DataBuffer[6] == 0x02)
						  &&(lin_temp_buffer.frame.DataBuffer[7] == 0xFF)								
						  )
						{
//							extern uint32_t debug_first_addr;
							if (savedConfig.pART != Slave_Address_eol)
							{
                     LIN_AutoAddr_Start();
							}
//							debug_first_addr = 0xff;
						}
#endif
						
            lin_slave_task_rx_data();
         }
         lin_timeout_ctrl.flag.frame = 0u;
         lin_timeout_ctrl.flag.transm_ongoing = 0u;
         lin_timeout_ctrl.flag.recept_started = 0u;
         lin_frm_timeout_timer = 0u;
         /* Set LIN slave state to idle to wait for next frame */
         lin_hal_wait_for_break();
         break;

      case tx_checksum:
         /* check if transmission has crashed */
         if (lin_hal_rx_data_guc != lin_tx_data_g.old_data_uc)
         {
            /* TX Response ERROR */
            lin_flag_g.tx_next = L_RESET;
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1))
            lin_hal_set_resp_error();
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
#ifdef J2602_PROTOCOL
            lin_hal_set_resp_error(J2602_DATA_ERROR);
#endif /* end #ifdef J2602_PROTOCOL */
         }
         else
         {
            /* if tx was okay, finish tx.  */
            lin_slave_task_tx_data();
         }
#ifdef SUPPORT_TWO_BAUDRATES
         /* reset to higher baudrate if not yet set to a fixed value
            to be able to detect the lower one */
         l_hal_reset_to_high_baudrate();
#endif /* #ifdef SUPPORT_TWO_BAUDRATES */
         lin_hal_wait_for_break();
         /* Reset transmission ongoing flag */
         lin_timeout_ctrl.flag.transm_ongoing = 0u;
         break;
      default:
         /* Impossible Slave State... */
         lin_hal_wait_for_break();
         break;
      }
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_ISR(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Interrupt service routine for receive interrupt. Can be also used for
           polling (then it should be called once each bit).
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
void lin_hal_ISR(void)
{
#ifdef AUTOBAUD_ENABLED
   l_u8 temp_linst = 0u;
#endif /* end #ifdef AUTOBAUD_ENABLED */
   g_lin_irqState = l_sys_irq_disable();
   /* get values from SCI */
   lin_hal_rx_data_guc = (l_u8) LIN_UART_RX_BUFFER; /*lint !e923*/
   lin_hal_status_g.stopbit = (l_u8) LIN_UART_9BITREC; /*lint !e923*/
#ifdef AUTOBAUD_ENABLED
   temp_linst = (l_u8) LIN_UART_STATUS; /*lint !e923*/
   LIN_UART_CLR_STATUS = (LIN_UART_CLR_STATUS | LINST_MASK_SYNBRK); /*lint !e923*/
#endif /* end #ifdef AUTOBAUD_ENABLED */
   l_sys_irq_restore(g_lin_irqState);

#ifdef LIN_PROTOCOL_VERSION_2_1
   g_lin_status_word.flag.bus_activity = L_SET;
#endif /* end #ifdef LIN_PROTOCOL_VERSION_2_1 */

#ifdef AUTOBAUD_ENABLED
#ifdef LIN_ENABLE_AA_BSM_B5
   /* Detect Break characters, configure and start T2 to start on next falling edge (startbit of Synch-field) */
   if (((temp_linst & LINST_MASK_SYNBRK) == 0u) && (lin_hal_rx_data_guc == 0u) 
      && (lin_hal_status_g.stopbit == 0u)&&(E_StateMachineStatusIdle == (LIN_AutoAddr_Get_Status())))
#else
   /* Detect Break characters, configure and start T2 to start on next falling edge (startbit of Synch-field) */
   if (((temp_linst & LINST_MASK_SYNBRK) == 0u) && (lin_hal_rx_data_guc == 0u) && (lin_hal_status_g.stopbit == 0u))
#endif
   {
      lin_ab_t2_state = 1u;
      lin_ab_timeout_timer = 0u;
      /* Start baudrate measurement */
      g_lin_irqState = l_sys_irq_disable();
      LIN_UART_EOFSYNEN = 1u; /*lint !e923*/
      LIN_UART_ERRSYNEN = 1u; /*lint !e923*/
      LIN_UART_BR_SYN_DET =   BG_SEL; /*lint !e923*/
      LIN_UART_BR_EN = 0u; /*lint !e923*/
      LIN_UART_BR_EN = 1u; /*lint !e923*/
      LIN_UART_CLR_STATUS = (LIN_UART_CLR_STATUS | LINST_MASK_SYNBRK); /*lint !e923*/
      lin_hal_init_T2();
      l_sys_irq_restore(g_lin_irqState);
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1))
      lin_hal_set_resp_error();
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
#ifdef J2602_PROTOCOL
      lin_hal_set_resp_error(J2602_FRAMING_ERROR);
#endif /* end #ifdef J2602_PROTOCOL */
      lin_hal_wait_for_break();
      lin_slave_state_g = break_received;
      /* After a break is received, frame timeout flag and frame timeout timer shall be reset */
      lin_timeout_ctrl.flag.frame = 0u;
      lin_frm_timeout_timer= 0u;
   }
   /* Break and End of Synch Field detected */
   else if ((temp_linst & LINST_MASK_SYNBRK) == LINST_FLAG_EOFSYN)
   {
      /* IRQ not active */
   }
   /* Break detected w/o End of Synch Field */
   else if ((temp_linst & LINST_MASK_SYNBRK) == LINST_FLAG_ERRSYN)
   {
      /* IRQ not active */	 
   }
   /* End of Synch Field detected w/o Break */
   else if ((temp_linst & LINST_MASK_SYNBRK) == LINST_FLAG_BRK)
   {
      /* IRQ not active */ 
   }
   /* received a normal byte without framing error */
   else
#endif /* #ifdef AUTOBAUD_ENABLED */
   {
      /* Detected a framing error */
      if (lin_hal_status_g.stopbit == 0u)
      {
         if (lin_slave_state_g >= rx_data)
         {
            lin_flag_g.tx_next = L_RESET;
#if ((defined(LIN_PROTOCOL_VERSION_2_0)) || (defined(LIN_PROTOCOL_VERSION_2_1)) || (defined(J2602_PROTOCOL)))
            if ((g_lin_frame_index >= LIN_NUMBER_OF_FRAMES) || ((g_lin_frame_ctrl[g_lin_frame_index].frame.frame_type.transfer_type & 0x01u) == 0u))
            {
               /* FERR received in the slot intended for receiving or transmitting the
                * first databyte of a frame, set flag to have response error bit set correctly */
               lin_timeout_ctrl.flag.recept_started = 1u;
            }
#if (defined(LIN_SLAVE))
            lin_hal_set_resp_error();
#endif /* end #if (defined(LIN_SLAVE)) */
#if (defined(J2602_SLAVE))
            lin_hal_set_resp_error(J2602_FRAMING_ERROR);
#endif /* end #if (defined(J2602_SLAVE)) */
#endif /* end #if ((defined(LIN_PROTOCOL_VERSION_2_0)) || (defined(LIN_PROTOCOL_VERSION_2_1)) || (defined(J2602_PROTOCOL))) */
         }
#ifndef AUTOBAUD_ENABLED
         /* Sync Break */
         lin_hal_wait_for_break();
         if ( lin_hal_rx_data_guc == 0u)
         {
            lin_slave_state_g = break_received;
         }
#else
          /* Sync Break */
#ifdef LIN_ENABLE_AA_BSM_B5
         //if((L_SET == (LinAa_GetActive())))
         if((E_StateMachineStatusIdle != (LIN_AutoAddr_Get_Status())))
         {
            lin_hal_wait_for_break();
            if ( lin_hal_rx_data_guc == 0u)
            {
               lin_slave_state_g = break_received;
            }    
         } 
#endif
#endif /* #ifndef AUTOBAUD_ENABLED */
      }
      else
      {
         /* byte received */
         if (lin_slave_state_g == tx_data)
         {
            if (lin_hal_rx_data_guc != lin_tx_data_g.old_data_uc)
            {
               /* TX Data error */
               lin_slave_state_g = idle;
               lin_flag_g.tx_next = L_RESET;
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1))
                     lin_hal_set_resp_error();
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
#ifdef J2602_PROTOCOL
                     lin_hal_set_resp_error(J2602_DATA_ERROR);
#endif /* end #ifdef J2602_PROTOCOL */
            }
            else
            {
               /* if tx was okay, send next field */
               l_hal_tx_state_machine();
            }
         }
         else
         {
            if (lin_slave_state_g != idle)
            {
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
               l_hal_master_send_pid(); /*lint !e923*/
#else
               lin_hal_status_g.byte_received = L_SET;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
            }
            else
            {
               /* no data field */
               lin_slave_state_g = idle;
               /* During header transmission baudrate is changed, break is transmitted
                * as normal 0x00 data byte but state is still idle. */
               l_hal_master_send_sync(); /*lint !e923*/
            }
         }
      }
   }

   /* LIN Status Register shall be initialized again */
   LIN_UART_BR_EN = 0u; /*lint !e923*/
   LIN_UART_BR_EN = 1u; /*lint !e923*/
   LIN_UART_CLR_STATUS = (LIN_UART_CLR_STATUS | LINST_MASK_SYNBRK); /*lint !e923*/
   
   return;
}

#ifdef AUTOBAUD_ENABLED
/* ---------------------------------------------------------------------------
 *  void lin_hal_T2_ISR(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Interrupt service routine for autobaud timer interrupt. Can be also used for
           polling (then it should be called once each bit).
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
void lin_hal_T2_ISR(void)
{
   if (lin_ab_t2_state == 5u)
   {
      LIN_TIMER_RUN = 0u; /*lint !e923*/
      LIN_TIMER_EXT_IR_EN = 0u; /*lint !e923*/
      LIN_TIMER_EXT_EN = 0u; /*lint !e923*/
      lin_ab_t2_state = 0u;
      lin_synch_time = (l_u16) ((l_u16)((l_u16)LIN_TIMER_CAP_H << 8u) | ((l_u16)LIN_TIMER_CAP_L)); /*lint !e923*/
      /* (MISRA-C:2004 13.7/R, e506) Constant value Boolean
       * Reason: Configuration value from genLinConfig.h */
      lin_hal_brreg_calc(); /*lint !e923, !e506*/
   }
   else
   {
      lin_ab_t2_state++;
   }
}


/* ---------------------------------------------------------------------------
 *  void l_autobaud_control (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Timeout Control for baudrate synchronization
   @pre    LIN driver initialized
   @param  void
   @retval void
 */
void l_autobaud_control(void)
{
   if(lin_ab_t2_state != 0u)
   {
      lin_ab_timeout_timer++;
      if(lin_ab_timeout_timer >= LIN_AUTOBAUD_TIMEOUT)
      {
         lin_slave_state_g = idle;
         lin_hal_init_uart();
         lin_ab_t2_state = 0u;
         lin_ab_timeout_timer = 0u;
      }
   }
}


/* ---------------------------------------------------------------------------
 *  void lin_hal_init_T2 (void)
 * --------------------------------------------------------------------------*/

/**
   @brief   Initializes timer 2 for baud rate synchronisation
   @pre     LIN driver initialized
   @param   void
   @retval  void
 */
static void lin_hal_init_T2 (void)
{
   LIN_UART_BCON_RUN = 0u; /*lint !e923*/          /* stop baudrate generator */

   /* CON */
   LIN_TIMER_RUN = 0u; /*lint !e923*/              /* stop timer for reinit */
   LIN_TIMER_CAP_REL = 1u; /*lint !e923*/
   LIN_TIMER_T_C_SEL = 0u; /*lint !e923*/
   LIN_TIMER_EXT_EN = 1u; /*lint !e923*/           /* External Events enabled */
   /* MOD */
   LIN_TIMER_PRE = 0u; /*lint !e923*/
   LIN_TIMER_PRE_EN = 1u; /*lint !e923*/
   LIN_TIMER_COUNTER_EN = 0u; /*lint !e923*/
   LIN_TIMER_EDGE_SELECT = 0u; /*lint !e923*/
   LIN_TIMER_EXT_START = 1u; /*lint !e923*/
   LIN_TIMER_CAP_EDGE_SELECT = 0u; /*lint !e923*/
   /* CNT */
   LIN_TIMER_HIGH = 0u; /*lint !e923*/             /* reset the count value to '0' */
   LIN_TIMER_LOW = 0u; /*lint !e923*/
   /* CON1 */
   LIN_TIMER_EXT_IR_EN = 1u; /*lint !e923*/        /* Interrupt for external events enabled */
   LIN_TIMER_INT_IR_EN = 0u; /*lint !e923*/
   /* RC */
   LIN_TIMER_CAP_H = 0u; /*lint !e923*/
   LIN_TIMER_CAP_L = 0u; /*lint !e923*/
   /* ICLR */
   LIN_TIMER_EX_IR_CLR_FLAG = 0u; /*lint !e923*/
   LIN_TIMER_OVERFLOW_IR_CLR_FLAG = 0u; /*lint !e923*/

   LIN_UART_CLR_STATUS = (LIN_UART_CLR_STATUS | LINST_MASK_SYNBRK); /*lint !e923*/
   lin_ab_timeout_timer = 0u;
}


/* ---------------------------------------------------------------------------
 *  static void lin_hal_brreg_calc(void)
 * --------------------------------------------------------------------------*/
/**
   @brief   Calculates the values for baud rate generator registers based on timer 2
            input capture
   @pre     LIN driver initialized
   @param   void
   @retval  void
 */
static void lin_hal_brreg_calc(void)
{
   lin_synch_time = (l_u16)(((l_u16)(lin_synch_time >> 2u)) >> BRG_PRE);

   LIN_UART_BCON_FDSEL = (l_u8) (((l_u8)(lin_synch_time & 0x1Fu)) >> ((BRG_PRE == 0u)?1u:0u)); /*lint !e923*/

   lin_synch_time = (l_u16) (lin_synch_time >> 5u);

   if (lin_synch_time >= MIN_BDREG)
   {
      if (lin_synch_time <= MAX_BDREG)
      {
         LIN_UART_BCON_BG = (uint32_t)((uint32_t)lin_synch_time & 0x7FFu); /*lint !e923*/
#ifdef J2602_SLAVE
         lin_hal_status_g.isr_rec_sync = 1u;
#endif /* end #ifdef J2602_SLAVE */
         lin_slave_state_g = sync_received;
      }
      else
      {
         lin_ab_timeout_timer = LIN_AUTOBAUD_TIMEOUT + 1u;
         lin_ab_t2_state = 7u;
      }
   }
   else
   {
      lin_ab_timeout_timer = LIN_AUTOBAUD_TIMEOUT + 1u;
      lin_ab_t2_state = 7u;
   }

   LIN_UART_BCON_RUN = 1u; /*lint !e923*/
}


/* ---------------------------------------------------------------------------
 *  void l_hal_baudrate_control (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Control function for baud rate
   @pre    LIN driver initialized
   @param  void
   @retval void
 */
void l_hal_baudrate_control(void)
{
   /* (MISRA-C:2004 11.1/R, 11.3/A e923) cast from unsigned int to pointer
    * Reason: Access to HW registers, multiple times in this function. */
   /*lint -save -e923*/
   /* do not try to check the baudrate setting here, reading the register BGL1/BGL2 will return the current timer value, not the timer reload value! */
   /* if the transmission is disturbed by a dominant pulse, a TX timeout will occur in the LIN transceiver and the transceiver will automatically switch to receive only mode.
      In order to recover from this error, the error flag is cleared. The same applies for overtemperature and overcurrent errors. */  
   if (( LIN->LIN_IS.bit.TXD_TMOUT_STS != 0u) || ( LIN->LIN_IS.bit.OT_STS != 0u) || ( LIN->LIN_IS.bit.OC_IS != 0u))
   {
      LIN->LIN_ISC.bit.OC_IS_CLR = 1u;
      LIN->LIN_ISC.bit.OT_IS_CLR = 1u;
      LIN->LIN_ISC.bit.TXD_TMOUT_IS_CLR = 1u;
      LIN->LIN_ISC.bit.OT_STS_CLR = 1u;
      LIN->LIN_ISC.bit.TXD_TMOUT_STS_CLR = 1u;
   }
   /*lint -restore*/
}
#endif /* end #ifdef AUTOBAUD_ENABLED */

/* ---------------------------------------------------------------------------
 *  void lin_hal_init_uart(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Initializes the UART / SCI
   @param  void
   @retval void
*/
/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
void lin_hal_init_uart(void)
{
   /* Reset UART to initial values */
   LIN_UART_CONTROL =      0x00u; /*lint !e923*/

   /* Set UART to 8bit shift UART */
   LIN_UART_MODE0 =        0u; /*lint !e923*/
   LIN_UART_MODE1 =        1u; /*lint !e923*/

   /* Set Baudrate Generator and enable */
   LIN_UART_BCON_PRE =     BRG_PRE; /*lint !e923*/

   if (BRG_FD_SEL > 1u)
   {
      LIN_UART_BCON_FDSEL =   (BRG_FD_SEL - 1u); /*lint !e923*/
   } 
   else
   {
      LIN_UART_BCON_FDSEL =   BRG_FD_SEL;
   }

   LIN_UART_BCON_BG =      BRG_VAL; /*lint !e923*/
   LIN_UART_BCON_RUN =     1u; /*lint !e923*/

   /* Set Limits for Baudrate detection */
#ifdef AUTOBAUD_ENABLED
   /* LIN_UART_STATUS =       0x40u; */
   LIN_UART_BR_EN = 1u; /*lint !e923*/
   LIN_UART_BR_SYN_DET = 0u; /*lint !e923*/
   LIN_UART_ERRSYNEN = 1u; /*lint !e923*/
   LIN_UART_EOFSYNEN = 1u; /*lint !e923*/

   LIN_UART_CLR_STATUS =   (LIN_UART_CLR_STATUS | LINST_MASK_SYNBRK); /*lint !e923*/
#else
   /* LIN_UART_STATUS =       0x01u; */
   LIN_UART_BR_EN = 0u; /*lint !e923*/
   LIN_UART_BR_SYN_DET = 0u; /*lint !e923*/
   LIN_UART_ERRSYNEN = 0u; /*lint !e923*/
   LIN_UART_EOFSYNEN = 0u; /*lint !e923*/

   LIN_UART_CLR_STATUS =   (LIN_UART_CLR_STATUS | LINST_MASK_SYNBRK); /*lint !e923*/
#endif /* end #ifndef AUTOBAUD_ENABLED */
   LIN_UART_BR_SYN_DET =   BG_SEL; /*lint !e923*/
#ifdef AUTOBAUD_ENABLED
   LIN_UART_BR_EN = 0u; /*lint !e923*/
   LIN_UART_BR_EN = 1u; /*lint !e923*/
   LIN_UART_CLR_STATUS =   (LIN_UART_CLR_STATUS | LINST_MASK_SYNBRK); /*lint !e923*/
#endif /* end #ifndef AUTOBAUD_ENABLED */

   /* Enable UART reception */
   LIN_UART_RUN =          1u; /*lint !e923*/

#ifndef POLLING_MODE
   /* UART interrupt enable */
   LIN_UART_IR_SET_EN =    (LIN_UART_IR_SET_EN | LIN_IR_SET_EN_MASK); /*lint !e923*/
   LIN_UART_IR_EN =        1u; /*lint !e923*/
   LIN_UART_IRTX_EN =      0u; /*lint !e923*/
   LIN_UART_IR_PENDING =   LIN_IR_SET_PEN_MASK; /*lint !e923*/
   LIN_UART_IR_PRIO =      LIN_INTERRUPT_PRIORITY; /*lint !e923*/
#endif /* ifndef POLLING_MODE*/

#ifdef AUTOBAUD_ENABLED

   LIN_TIMER_PRE =         0u; /*lint !e923*/  /* T2CLK = PCLK (20MHz) */
   LIN_TIMER_PRE_EN =      1u; /*lint !e923*/  /* Timer2 Prescaler is used (T2PRE) */
   LIN_TIMER_COUNTER_EN =  0u; /*lint !e923*/  /* UpDown Counter function disabled */
   LIN_TIMER_EDGE_SELECT = 0u; /*lint !e923*/  /* Timer2 starts on external falling edge */
   LIN_TIMER_EXT_START =   1u; /*lint !e923*/  /* Timer2 starts on external event */
   LIN_TIMER_CAP_EDGE_SELECT = 0u; /*lint !e923*/

   LIN_TIMER_CAP_REL =     1u; /*lint !e923*/  /* Capture Mode */
   LIN_TIMER_T_C_SEL =     0u; /*lint !e923*/  /* Timer function */
   LIN_TIMER_EXT_EN =      0u; /*lint !e923*/  /* External Events disabled */

   /* set Timer2 interrupts */
   LIN_TIMER_EXT_IR_EN =   0u; /*lint !e923*/  /* Interrupt for external events disabled */
   LIN_TIMER_INT_IR_EN =   0u; /*lint !e923*/  /* Interrupt for Timer2 overflow disabled */

   /* Timer2 is needed for baudrate synchronization */
   LIN_UART_IR_SET_EN =    (LIN_UART_IR_SET_EN | LIN_IR_SET_EN_MASK); /*lint !e923*/
#endif /* end #ifdef AUTOBAUD_ENABLED */
}

#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1) || (defined J2602_PROTOCOL))
/* ---------------------------------------------------------------------------
 *  void lin_hal_set_resp_error ()
 * --------------------------------------------------------------------------*/

/**
   @brief  Set Response Error flag and Statusword Update correctly dependent
           of current driver state.
   @note   Response error is set latest at beginning of a new synch break - if
           not timed out before via ld_task(). Flags are set again earliest at
           PID recognition.
   @param  void
   @retval void
 */
#if (defined J2602_PROTOCOL)
#ifdef GM_SPEC_ADAPTION
void lin_hal_set_resp_error (l_u8 j2602_fault)
#else
static void lin_hal_set_resp_error (l_u8 j2602_fault)
#endif /* #ifdef GM_SPEC_ADAPTION */
#else
/* Prototype for LIN protocols */
void lin_hal_set_resp_error (void)
#endif /* end #if defined J2602_PROTOCOL */
{
   /* Protect setting of variables in different contexts */
   g_lin_irqState = l_sys_irq_disable();
#ifndef GM_SPEC_ADAPTION
   if ((lin_timeout_ctrl.flag.recept_started != 0u) || (lin_timeout_ctrl.flag.transm_ongoing != 0u))
#else
   /* GM specific J2602 adaption needs also the PID errors shown in the response error flag */
   if ((lin_timeout_ctrl.flag.recept_started != 0u)
         || (lin_timeout_ctrl.flag.transm_ongoing != 0u)
         || ((g_j2602_status.flags.errorstate & 0x10) != 0u))
#endif /* #ifndef GM_SPEC_ADAPTION */
   {
#ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE
      if(g_lin_rx_ctrl.flag.evttrgfrm == 0u)
#endif /* #ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE */
      {
#if ((defined(J2602_PROTOCOL)) && (!(defined(GM_SPEC_ADAPTION))))
         /* J2602 errors are assembled in a variable for assembling the statusword when required for
          * transmission of a designated frame containing the status byte.
          * If receiving a broadcast frame ignore errors. */
         if (lin_timeout_ctrl.flag.recept_started == 1u)
         {
            if ((g_lin_active_pid != 0x78u) && (g_lin_active_pid != 0xBAu)
               && (g_lin_active_pid != 0x39u) && (g_lin_active_pid != 0xFBu))
            {
               g_j2602_status.flags.errorstate |= j2602_fault;
            }
         }
         else
         {
            /* If - Else is necessary because after reception of the first byte during a subscribe frame
             * the transm_ongoing flag is also set to indicate that a first data byte of a frame has been
             * received.
             * If a frame was being transmitted, indicate that the status byte was not successfully transmitted. */
            if (lin_timeout_ctrl.flag.transm_ongoing == 1u)
            {
               g_j2602_status.flags.errorstate |= j2602_fault;
               g_j2602_status.flags.status_byte_transm = 0u;
            }
         }
#endif /* end #ifndef J2602_PROTOCOL */
#ifdef GM_SPEC_ADAPTION
         /* Set errors except for broadcast frames. */
         if ((g_lin_active_pid != 0x78u) && (g_lin_active_pid != 0xBAu)
            && (g_lin_active_pid != 0x39u) && (g_lin_active_pid != 0xFBu))
         {
            LIN_STATUS_BYTE(1u);
         }
#endif /* end #ifdef GM_SPEC_ADAPTION */
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1))
         l_Set_Response_Error_Flag();
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
      }
      if (g_lin_status_word.flag.last_pid != 0x0u)
      {
         g_lin_status_word.flag.overrun = L_SET;
      }
      g_lin_status_word.flag.last_pid = g_lin_active_pid;
      g_lin_status_word.flag.error_in_resp = L_SET;
   }
   lin_timeout_ctrl.flag.recept_started = 0u;
   lin_timeout_ctrl.flag.transm_ongoing = 0u;
#ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE
   /* Reset flag to indicate an event triggered frame (receive or transmit) */
   g_lin_rx_ctrl.flag.evttrgfrm = L_RESET;
#endif /* #ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE */
   l_sys_irq_restore(g_lin_irqState);
}
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 || defined J2602_PROTOCOL */


/* ---------------------------------------------------------------------------
 *  void l_frame_timeout_control (void)
 * --------------------------------------------------------------------------*/
/**
   @brief   Timeout control of frames
   @note    This enables the driver to go into an idle state if for a long time
            no further byte was received.
   @pre     LIN driver initialized
   @param   void
   @retval  void
 */
void l_frame_timeout_control(void)
{
   if(lin_timeout_ctrl.flag.frame != 0u)
   {
      lin_frm_timeout_timer++;
      
      if(lin_frm_timeout_timer >= LIN_FRAME_TIMEOUT)
      {
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1))
         lin_hal_set_resp_error();
#else
#if (defined GM_SPEC_ADAPTION)
         if ((lin_timeout_ctrl.flag.recept_started != 0u) || (lin_timeout_ctrl.flag.transm_ongoing != 0u))
         {
            g_j2602_status.flags.errorstate = 1u;
         }   
         
         lin_hal_set_resp_error();
#else
#ifdef J2602_PROTOCOL
         lin_hal_set_resp_error(J2602_DATA_ERROR);
#endif /* #ifdef J2602_PROTOCOL */
#endif /* end #if defined GM_SPEC_ADAPTION */
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */

         lin_hal_wait_for_break();   
      } 
   }
#ifdef J2602_SLAVE
   if (((g_j2602_status.flags.errorstate & 0x10u) != 0u) && (lin_timeout_ctrl.flag.transm_ongoing == 0u))
   {
      lin_hal_set_resp_error(J2602_PID_ERROR);
   }
#endif /* end #ifdef J2602_SLAVE */
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_wait_for_break(void)
 * --------------------------------------------------------------------------*/

/**
   @brief   Call for other modules to set driver into idle state, waiting for
            next frame header
   @pre     LIN driver initialized
   @param   void
   @retval  void
 */
void lin_hal_wait_for_break(void)
{
   lin_slave_state_g = idle;
   lin_timeout_ctrl.flag.frame = 0u;
   lin_frm_timeout_timer = 0u;
}


/* ---------------------------------------------------------------------------
 *  void l_hal_cyclic_call (void)
 * --------------------------------------------------------------------------*/
/**
   @brief   l_cyclic_com_task implementation
   @note    Responsible for interpretation of received byte, storage in
            appropriate buffers and PID recognition.
   @pre     LIN driver initialized
   @param   void
   @retval  void
 */
void l_hal_cyclic_call(void)
{
   if(lin_hal_is_byte_received())
   { 
      lin_hal_reset_byte_received();
      l_hal_rx_state_machine();
   }
   if (l_txchk_byte_nr > 0u)
   { 
      l_hal_txchecksum_calculation();
   }
}
#if 0
#ifdef LIN_ENABLE_AA_BSM_B5
/**
   @brief      Stop Auto Addressing
*/
static void LinHa_Aa_Stop(void)
{
   LIN->AA_CTRL.reg = AA_CTRL_STOP_RESET;
   LIN->AA_CTRL.bit.ERR_CODE_CLR = 1u;
   LIN->LIN_IEN.bit.AA_ERR_IEN = 0u;
   LIN->LIN_IEN.bit.AA_MEAS_RDY_IEN = 0u;
}
/**
   @brief      Configures Auto Addressing Offset Phase
*/
static void LinHa_Aa_ConfigOffsetPhase(void)
{
   LIN->AA_CTRL.bit.ERR_CODE_CLR       = 1u;    
   LIN->LIN_ISC.reg = (LIN_LIN_ISC_AA_ERR_CLR_Msk | LIN_LIN_ISC_AA_MEAS_RDY_CLR_Msk);
   
   if (L_RESET == LinAa_GetAddressed())
	{
      LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlOffsetPhaseActive;
      LIN->LIN_IEN.bit.AA_ERR_IEN = 1u;
      LIN->LIN_IEN.bit.AA_MEAS_RDY_IEN = 1u;
   }
   else
   {
      LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlOffsetPhaseInactive;
      
      LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlEndingPhase;
   }

   LinHa_Aa_WaitForRisingEdge();
   
   error = eEsNoError;
}

/**
   @brief      Configures Auto Addressing Pre-Selection Phase
*/
static void LinHa_Aa_ConfigPreSelectionPhase(void)
{
   if (L_RESET == LinAa_GetAddressed())
	{
      LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlPreselPhase;
   }
   else
   {
		 
   }
}

/**
   @brief      Configures Auto Addressing Selection Phase
*/
static void LinHa_Aa_ConfigSelectionPhase(void)
{
   int16_t iShuntDiffPreselOff;
   
   if (L_RESET == LinAa_GetAddressed())
	{
//      iShuntDiffPreselOff = (uint16_t)(abs((int32_t)iShuntPreselPhase - (int32_t)iShuntOffsetPhase));
       iShuntDiffPreselOff = (int16_t)(((int32_t)iShuntPreselPhase - (int32_t)iShuntOffsetPhase));     
      if ((iShuntDiffPreselOff < D_PRE_DIFF_THRESHOLD_POS) && (iShuntDiffPreselOff > D_PRE_DIFF_THRESHOLD_NEG))
      {
         LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlSelPhaseActive;
      }
      else
      {
         LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlSelPhaseInactive;
         
         LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlEndingPhase;
         LIN->LIN_IEN.bit.AA_MEAS_RDY_IEN = 0u;
      }
   }
   else
   {
   }
}

/**
   @brief      Finish Auto Addressing
*/
void LinHa_Aa_Finish(void)
{
   LinHa_Aa_Stop();
   LIN->T2_CON.bit.EXEN2   = 0u;
   LIN->LIN_IEN.bit.EXF2EN = 0u;
}
/**
   @brief      Getter for shunt current from hardware
   @par        Read the accumulated measurement results from register bit fields
               and build the actual shunt current value
   @see        Infineon LIN Auto Addressing Information p.8                          
*/
static int16_t LinHa_Aa_GetShuntCurrent(void)
{
   int16_t retVal;
   int16_t shuntAcc1Val = (int16_t)LIN->AA_ACC_RES_1.bit.ACC_RES_1;
   int16_t shuntAcc2Val = (int16_t)LIN->AA_ACC_RES_2.bit.ACC_RES_2;
   retVal = ((shuntAcc1Val-shuntAcc2Val) >> (LIN->AA_CTRL.bit.ISENSE_MEAS_NUMBER));
   retVal = ADC11_compensateLinAutoAddrDiff(retVal);
   return retVal;
}

/**
   @brief      Configures Timer2 for LIN rising edge (end of break)
   @par        Configure Timer2 to generate an interrupt at the
               next rising edge that signals the end of the break field.
*/
static void LinHa_Aa_WaitForRisingEdge(void)
{
   LIN->T2_MOD.bit.EDGESEL = 1u;    /* Select rising edge */
   LIN->LIN_IEN.bit.EXF2EN = 1u;    /* Enable EXF2 interrupt */
}

/**
   @brief      Handle Auto Addressing error
*/
void LinHa_Aa_ErrorReceived(void)
{
   uint8_t aaError;
   
   aaError  = (uint8_t)LIN->AA_STATUS.bit.ERR_CODE;
   
   if (0u != ( aaError & AA_BREAK_ERR_MASK ))
   {
      error = eEsBreakError;
   }
   else if (0u != ( aaError & AA_ADC_ERR_MASK ))
   {
      error = eEsAdcError;
   }
   else
   {
      error = eEsDefaultError;
   }
   
   errorPosition = (uint8_t)LIN->AA_CTRL.bit.LIN_AA_PH;
   
   LinHa_Aa_Stop();
}
/**
   @brief      Initializes Auto Addressing procedure
*/
void LinHa_Aa_Init(void)
{
   error = eEsNoError;
   
   ADC11->CFG_1.bit.ADC_ENABLE = 1u;
   ADC11->CFG_0.bit.ADC_OVERS_CFG = 3u;
   
   LIN->AA_CTRL.reg                    = 0u;
   LIN->AA_CTRL.bit.LIN_AA_EN          = 1u;    
   LIN->AA_CTRL.bit.FSM_RST            = 1u;    
   LIN->AA_CTRL.bit.ERR_CODE_CLR       = 1u;    
   
   LIN->T2_MOD.reg = 0x10u;
   LIN->T2_CON.reg = 0x09u;
   LIN->LIN_ISC.reg = AA_INIT_INT_CLR_MASK; 
   LIN->LIN_IEN.bit.EXF2EN = 1u;            
   
   LIN->AA_FW_CTRL.bit.TO_BE_ADDR = 0u;
   LIN->AA_FW_CTRL.bit.ALRDY_ADDR = 0u;
}
/**
   @brief      Handle Timer2 falling or rising edge for Auto Addressing
*/
void LinHa_Aa_EdgeReceived(void)
{
   uint32_t edgeSelect = LIN->T2_MOD.bit.EDGESEL;
   uint32_t aaErrInt = LIN->LIN_IS.bit.AA_ERR;
   uint32_t aaError  = LIN->AA_STATUS.bit.ERR_CODE;
   
   if (LIN_FALLING_EDGE_SEL == edgeSelect)
   {
      LinHa_Aa_ConfigOffsetPhase();
   }
   else if (LIN_RISING_EDGE_SEL == edgeSelect)
   {
      
      if (0u != ( aaErrInt | aaError ))
      {
         LinHa_Aa_ErrorReceived();
      }
      else
      {
         LinHa_Aa_Stop();
      }
      
#ifdef AUTOBAUD_ENABLED     
      LIN->T2_MOD.bit.EDGESEL = 0u;
#endif 
   }
   else
   {

   }
}

/**
   @brief      Handle completed Auto Addressing measurement
*/
void LinHa_Aa_MeasureReady(void)
{
   uint32_t aaPhase = LIN->AA_CTRL.bit.LIN_AA_PH;
   int16_t iShuntDiffSelOff;
   int16_t  iShuntSelPhase;
   
   switch (aaPhase)
   {
      case 1u: 
         iShuntOffsetPhase = LinHa_Aa_GetShuntCurrent();

         LinHa_Aa_ConfigPreSelectionPhase();
         break;
      
      case 2u:
         iShuntPreselPhase = LinHa_Aa_GetShuntCurrent();

         LinHa_Aa_ConfigSelectionPhase();
         break;
      
      case 3u:
         iShuntSelPhase = LinHa_Aa_GetShuntCurrent();
         
         iShuntDiffSelOff = (int16_t)(((int32_t)iShuntSelPhase - (int32_t)iShuntOffsetPhase));
      
         if ((iShuntDiffSelOff < D_SEL_DIFF_THRESHOLD_POS) && (iShuntDiffSelOff > D_SEL_DIFF_THRESHOLD_NEG))
         {
//            LIN->AA_FW_CTRL.bit.TO_BE_ADDR = 1u;
            LinAa_SetLastInChain(L_SET);     
         }
         else
         {
//            LIN->AA_FW_CTRL.bit.TO_BE_ADDR = 0u;
            LinAa_SetLastInChain(L_RESET);
         }
         
         LIN->AA_CTRL.reg = LinCfg_GetHalAaCfgPtr()->data.aaCtrlEndingPhase;
         LIN->LIN_IEN.bit.AA_ERR_IEN = 0u;
         LIN->LIN_IEN.bit.AA_MEAS_RDY_IEN = 0u;
         break;
      default:
         /* Not used, continue */
         break;
   }
}
/**
   @brief      Sets or resets AA addressed
*/
void LinHa_Aa_SetAddressed(l_bool val)
{
   if (val == L_SET)
   {
      LIN->AA_FW_CTRL.bit.ALRDY_ADDR = 1u;
   }
   else if (val == L_RESET)
   {
      LIN->AA_FW_CTRL.bit.ALRDY_ADDR = 0u;
   }
   else
   {
      
   }
}
#endif /* ifdef LIN_ENABLE_AA_BSM_B5 */
#endif

/*Songjm added for auto-addressing*/
#ifdef LIN_ENABLE_AA_BSM_B5
#include "lin_autoaddressing.h"

void lin_hal_aa_meas_isr(void)
{
   LIN->LIN_ISC.bit.AA_MEAS_RDY_CLR = 1u; /*lint !e923*/
   LIN_AutoAddr_Measure_ISR();
}

void lin_hal_aa_err_isr(void)
{
   LIN->LIN_ISC.bit.AA_ERR_CLR = 1u; /*lint !e923*/
   LIN_AutoAddr_Error_ISR();
}

void lin_hal_exf2_isr(void)
{
  LIN->LIN_ISC.bit.EXF2CLR = 1u;

  if (E_StateMachineStatusIdle != (LIN_AutoAddr_Get_Status()))
  {
		if (savedConfig.pART != Slave_Address_eol)
		{
      LIN_AutoAddr_Edge_ISR();
		}
  }
  else
  {
#ifdef AUTOBAUD_ENABLED
      /* ISR for baud rate measurement with input capture */
      l_ifc_t2();
#endif /* end #ifdef AUTOBAUD_ENABLED */
  }
}

//void lin_hal_init_autoaddressing(void);
#endif
/*add end*/
