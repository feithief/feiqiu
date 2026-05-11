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
********************     Workfile:      LINApplication.h    *******************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver                                           *
*  FILE-DESCRIPTION:     Header for LIN application file                      *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 984         $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2019-04-11#$                                 *
*                                                                             *
******************************************************************************/
/**
@file   LINApplication.h
@brief  Header file for LINApplication.c
*/

#ifndef LINAPPLICATION_H_FILE
#define LINAPPLICATION_H_FILE

/* ===========================================================================
 *  Types
 * ==========================================================================*/

#ifdef LIN_MASTER
typedef enum e_lin_app_state
{
   wake_up = 0u,
   wait_after_wake_up = 1u,
   run_sched0 = 2u,
   run_sched1 = 3u,
   run_sched2 = 4u,
   wait_01 = 5u,
   run_config0 = 6u,
   run_config1 = 7u,
   run_config2 = 8u,
   run_diag_sf = 9u,
   run_diag_mf = 10u,
   result = 11u,
   l_idle = 255u
} LIN_APP_STATE_t;
#endif /* #ifdef LIN_MASTER */

/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

void LINAPP_Init(void);
#ifdef LIN_MASTER
void LinMaster_Task(void);
#endif /* #ifdef LIN_MASTER */
#ifdef LIN_SLAVE
void LinSlave_Task(void);
#endif /* #ifdef LIN_SLAVE */
#endif /* #ifndef LINAPPLICATION_H_FILE */
