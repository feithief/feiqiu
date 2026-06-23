/******************************************************************************
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
********************     Workfile:      lin_diag_raw.h   **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  LIN Driver Protocol Layer                            *
*  FILE-DESCRIPTION:     All defines for lin_diag_raw.c                       *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 651         $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2018-05-02#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_diag_raw.h
@brief Header file of lin_diag_raw.c

   Defines and declaration for lin_diag_raw.c
*/

#ifndef LIN_DIAG_RAW_H /* to interpret header file only once */
#define LIN_DIAG_RAW_H

#ifdef    LIN_RAW_API
/* ===========================================================================
 *  Constants
 * ==========================================================================*/

/* ===========================================================================
 *  Structures
 * ==========================================================================*/

typedef l_u8 t_lin_diag_raw_frame[8];

/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

l_u8 lin_diag_raw_init(void);
l_u8* lin_diag_raw_pop_tx_queue(void);
void ld_raw_task(void);

/* ===========================================================================
 *  Variables
 * ==========================================================================*/

#if ((defined LIN_PROTOCOL_VERSION_2_0) || ((defined LIN_PROTOCOL_VERSION_2_1) && (!defined LIN_PROTOCOL_VERSION_2_2)))
extern l_u8 lin_diag_raw_tx_write_idx;
extern l_u8 lin_diag_raw_tx_read_idx;
#endif /* #if ((defined LIN_PROTOCOL_VERSION_2_0) || ((defined LIN_PROTOCOL_VERSION_2_1) && (!defined LIN_PROTOCOL_VERSION_2_2))) */

#endif /* #ifdef LIN_RAW_API */
#endif /* end #ifndef LIN_DIAG_RAW_H */

