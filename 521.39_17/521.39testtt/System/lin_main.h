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
*       Revision:        $Rev:: 43          $                                 *
*       Responsible:     B.Roegl                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2015-10-21#$                                 *
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

#ifndef L_SET
   #define L_SET                       1u
#endif /* end #ifdef L_SET */

#ifndef L_RESET
   #define L_RESET                     0u
#endif /* end #ifdef L_RESET */

#define PUBLISH                        1u
#define SUBSCRIBE                      0u

#ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE
   #define UPDATED_SIGNALS             2u
   #define EVENT_TRIGGERED             16u
#endif /* end #ifdef EVENT_TRIGGERED_FRAME_ENABLE */

#define UNCONDITIONAL                  0u
#define LIN_2_X                        20u
#define LIN_1_3                        13u
#define J_2602                         2u

#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1) || (defined J2602_PROTOCOL))
   #define LIN_PROTOCOL_TYPE           20u
#else
   #define LIN_PROTOCOL_TYPE           13u
#endif

#define DIAG_FRAME_LENGTH              8u
#define SINGLE_FRAME                   ((l_u8)0x00)
#define FIRST_FRAME                    ((l_u8)0x10)
#define CONSECUTIVE_FRAME              ((l_u8)0x20)
#define NEG_RESPONSE_RSID              ((l_u8)0x7F)
#define RSID_OFFSET                    ((l_u8)0x40)
#define NRC_SubfunctionNotSupported    ((l_u8)0x12)
#define NRC_FunctionNotSupported       ((l_u8)0x10)


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
extern t_lin_prod_id      g_lin_prod_id;
extern t_Lin_Frame_Ctrl   g_lin_frame_ctrl[LIN_NUMBER_OF_FRAMES];
extern t_lin_diag_frame   g_lin_diag_mreq;
extern t_lin_diag_frame   g_lin_diag_sresp;
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
#ifdef J2602_SLAVE
   extern t_lin_diag_frame   j2602_config_frame;
   extern t_j2602_status     g_j2602_status;
   extern l_u8               j2602_broadcast_bytes[2][J2602_DNN_NUMBER_OF_BYTES];
#endif /* end #ifdef J2602_SLAVE */

#endif /* end #ifndef LIN_MAIN_H */
