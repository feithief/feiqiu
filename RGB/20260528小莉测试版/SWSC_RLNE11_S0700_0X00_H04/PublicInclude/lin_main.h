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
********************     Workfile:      lin_main.h       **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver Protocol Layer                            *
*  FILE-DESCRIPTION:     All defines for lin_main.c                           *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 1313        $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2020-07-06#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_main.h
@brief Header file of lin_main.c
*/

#ifndef LIN_MAIN_H /* to interpret header file only once */
#define LIN_MAIN_H

#include "genLinConfig.h"

/* ===========================================================================
 *  Constants
 * ==========================================================================*/

#define PUBLISH                        1u
#define SUBSCRIBE                      0u

#ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE
   #define UPDATED_SIGNALS             2u
   #define FRAME_IS_EVENT_TRIGGERED    16u
#endif /* end #ifdef EVENT_TRIGGERED_FRAME_ENABLE */

#define UNCONDITIONAL                  0u
#define LIN_2_X                        20u
#define LIN_1_3                        13u
#define J_2602                         2u

#define LIN_PROTOCOL_MASK     0x80u

#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1) || (defined J2602_PROTOCOL))
   #define LIN_PROTOCOL_TYPE           20u
#else
   #define LIN_PROTOCOL_TYPE           13u
#endif

#if ((defined LIN_MASTER)||(defined J2602_MASTER))
#define LIN_DIAG_RECEIVE_FRAME   (g_lin_diag_sresp)
#define LIN_DIAG_SEND_FRAME      (g_lin_diag_mreq)
#else
#define LIN_DIAG_SEND_FRAME      (g_lin_diag_sresp)
#define LIN_DIAG_RECEIVE_FRAME   (g_lin_diag_mreq)
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
#define DIAG_FRAME_LENGTH              8u
#define SINGLE_FRAME                   ((l_u8)0x00)
#define FIRST_FRAME                    ((l_u8)0x10)
#define CONSECUTIVE_FRAME              ((l_u8)0x20)
#define NEG_RESPONSE_RSID              ((l_u8)0x7F)
#define RSID_OFFSET                    ((l_u8)0x40)
#define NRC_SubfunctionNotSupported    ((l_u8)0x12)
#define NRC_FunctionNotSupported       ((l_u8)0x10)

#ifdef SUPPORT_TWO_BAUDRATES
#ifndef SUP2BAUDRATES_FRAMECOUNT
#define SUP2BAUDRATES_NUMBEROFFRAMES   (2u)
#endif /* #ifdef SUPPORT_TWO_BAUDRATES */
#endif /* #ifndef SUP2BAUDRATES_FRAMECOUNT */



/* ===========================================================================
 *  Structures
 * ==========================================================================*/

/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

void lin_main_init(void);

/* ===========================================================================
 *  Variables
 * ==========================================================================*/

extern l_u8               g_lin_frame_index;
#if ((!defined LIN_MASTER) && (!defined J2602_MASTER))
extern t_lin_prod_id      g_lin_prod_id;
#endif /* #if ((!defined LIN_MASTER) && (!defined J2602_MASTER)) */
extern t_Lin_Frame_Ctrl   g_lin_frame_ctrl[LIN_NUMBER_OF_FRAMES];
extern t_lin_diag_frame   g_lin_diag_mreq;
extern t_lin_diag_frame   g_lin_diag_sresp;
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
extern l_u8   g_lin_config_buffer[3]; /* NAD, PCI, SID */
extern l_u8 (*g_lin_config_rbid_data)[]; /* Data pointer for read by ID service */
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */
extern t_lin_status_word  g_lin_status_word;
extern l_u8               g_lin_active_pid;
extern t_lin_rx_ctrl      g_lin_rx_ctrl;
extern l_irqmask          g_lin_irqState;
extern l_u8               g_lin_frame_data_size;
#if ((!defined(LIN_USER_DEF_DIAG_ENABLE)) || defined(LIN_USER_DEF_DIAG_MANDSERV_ENABLE))
extern l_u16              g_lin_tp_timeoutcounter;
#endif /* end #if (!defined(LIN_USER_DEF_DIAG_ENABLE)) || defined(LIN_USER_DEF_DIAG_MANDSERV_ENABLE) */
#if ((!defined(LIN_DIAG_ENABLE)) && (!defined(LIN_USER_DEF_DIAG_ENABLE)))
   extern l_u8            g_lin_diag_config_active;
#endif /* #if (!defined(LIN_DIAG_ENABLE)) && (!defined(LIN_USER_DEF_DIAG_ENABLE)) */
#ifdef LIN_USER_DEF_DIAG_ENABLE
   extern l_u8            g_lin_user_diag_active;
#endif /* end #ifdef LIN_USER_DEF_DIAG_ENABLE */
#if (defined J2602_PROTOCOL)
   extern t_lin_diag_frame   j2602_config_frame;
   extern t_j2602_status     g_j2602_status;
#endif /* (defined J2602_PROTOCOL) */
#ifdef J2602_SLAVE
   extern l_u8               j2602_broadcast_bytes[2][J2602_DNN_NUMBER_OF_BYTES];
#endif /* J2602_SLAVE */
#if ((defined LIN_MASTER)||(defined J2602_MASTER))
extern l_u8                g_nextScheduleFrmIdx;
#ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE
extern l_u8                g_ScheduleFrmIdx;
extern l_u16               g_ScheduleIdx;
#endif /* #ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE */
extern l_u16               g_nextScheduleIdx;
#ifndef LIN_PROTOCOL_VERSION_1_3
extern l_u8                g_lin_config_status;
#endif /* ifndef LIN_PROTOCOL_VERSION_1_3 */
extern l_u8                g_lin_zero_schedule;
extern l_u8                g_ld_NodeConfRSID;
extern l_u8                g_ld_NodeConfErr;
extern l_u16              g_ld_timer_p2;
#endif /* #if ((defined LIN_MASTER)||(defined J2602_MASTER)) */

#endif /* end #ifndef LIN_MAIN_H */
