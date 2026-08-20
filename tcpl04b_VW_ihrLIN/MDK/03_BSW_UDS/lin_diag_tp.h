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
********************     Workfile:      lin_diag_tp.h    **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver Protocol Layer                            *
*  FILE-DESCRIPTION:     All defines for lin_diag_tp.c                        *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 40          $                                 *
*       Responsible:     B.Roegl                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2015-10-16#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_diag_tp.h
@brief Header file of lin_diag_tp.c

   Defines and declaration for lin_diag_tp.c
*/


#include "genLinConfig.h"
#ifndef LIN_DIAG_TP_H /* to interpret header file only once */
#define LIN_DIAG_TP_H

#ifdef LIN_COOKED_API

/* ===========================================================================
 *  Header Files
 * ==========================================================================*/

/* ===========================================================================
 *  Constants
 * ==========================================================================*/

#define LIN_FF_DATA_LEN                 5u
#define LIN_CF_DATA_LEN                 6u
#define LIN_SF_DATA_LEN                 6u
#define LIN_TP_MAX_LEN                  4095u
/* Value of 1000 may cause early timeout therefore the value was increased to 1050 */
#define LIN_TP_TIMEOUT_MS               1050u
#define LIN_TP_COUNTER_MAX              (LIN_TP_TIMEOUT_MS / LIN_TASK_CYCLE_MS)
#define lin_tp_clear_timeout()          (g_lin_tp_timeoutcounter = 0u)
#define lin_tp_inc_timeout_counter()    (g_lin_tp_timeoutcounter++)

/* ===========================================================================
 *  Structures
 * ==========================================================================*/

/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

l_u8 lin_tp_init(void);
void lin_tp_tx_part_complete(void);
void ld_tp_task(void);
 void ld_receive_message(l_u16* const length,l_u8* const data);
void ld_send_message(l_u16 length, const l_u8* const data);
/* ===========================================================================
 *  Variables
 * ==========================================================================*/

extern t_lin_tp_tx_rx_ctrl  g_lin_tp_tx_rx_ctrl;
extern t_lin_tp_status      g_lin_tp_stat;

#endif /* end #ifdef LIN_COOKED_API */
#endif /* end #ifndef LIN_DIAG_TP_H */
