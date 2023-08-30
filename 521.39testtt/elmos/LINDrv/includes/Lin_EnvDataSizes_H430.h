/***************************************************************************//**
 * @file    Lin_EnvDataSizes_H430.h
 *
 * @creator SBAI
 * @created 06.04.2017
 *
 * @brief
 *
 * @purpose
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 *
 * $Id: Lin_EnvDataSizes_H430.h 8455 2021-03-01 19:44:50Z poe $
 *
 * $Revision: 8455 $
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 ******************************************************************************/
#ifndef LIN_ENVDATASIZES_H430_H_
#define LIN_ENVDATASIZES_H430_H_

/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/
#include "LinDrvImp_CompilationConfig.h"
#include "Lin_DevEnvDataSizes.h"


/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/

/* ****************************************************************************/
/*                               LOOKUP LAYER                                 */
/* ****************************************************************************/
#define LINDRV_LOOKUP_FIX_ENVIRONMENT_DATA_SZE 162u                 /**< LIN LOOKUP Layer environment data size */
#define LIN_LOOKUP_FIX_FRM_DESC_ENV_DATA_SZE(FRAME_CNT) (FRAME_CNT) /**< LIN LOOKUP Layer per'Frame Description List' environment data size */

#define LINDRV_LOOKUP_SEQ_ENVIRONMENT_DATA_SZE 34u                  /**< LIN LOOKUP Layer environment data size w/o lookup table (64*2 byte) */
#define LIN_LOOKUP_SEQ_FRM_DESC_ENV_DATA_SZE(FRAME_CNT) (FRAME_CNT) /**< LIN LOOKUP Layer per'Frame Description List' environment data size */

#if LINLOOKUP_USE_FIX_LOOKUP == 1

#define LINDRV_LOOKUP_ENVIRONMENT_DATA_SZE LINDRV_LOOKUP_FIX_ENVIRONMENT_DATA_SZE                     /**< LIN LOOKUP Layer environment data size */
#define LIN_LOOKUP_FRM_DESC_ENV_DATA_SZE(FRAME_CNT) LIN_LOOKUP_FIX_FRM_DESC_ENV_DATA_SZE(FRAME_CNT)   /**< LIN LOOKUP Layer per'Frame Description List' environment data size */

#elif LINLOOKUP_USE_SEQ_LOOKUP == 1

#define LINDRV_LOOKUP_ENVIRONMENT_DATA_SZE LINDRV_LOOKUP_SEQ_ENVIRONMENT_DATA_SZE                     /**< LIN LOOKUP Layer environment data size w/o lookup table (64*2 byte) */
#define LIN_LOOKUP_FRM_DESC_ENV_DATA_SZE(FRAME_CNT) LIN_LOOKUP_SEQ_FRM_DESC_ENV_DATA_SZE(FRAME_CNT)   /**< LIN LOOKUP Layer per'Frame Description List' environment data size */

#endif

/* ****************************************************************************/
/*                                PROTO LAYER                                 */
/* ****************************************************************************/
#define LINPROTOIMP_ENVIRONMENT_DATA_SZE(MSG_BUF_SZE)         ((44u)+ (MSG_BUF_SZE))  /**< LIN PROTO Layer environment data size */

#define LINPROTOIMP_PER_FRM_DESC_LST_ENV_DATA_SZE(FRAME_CNT)  (LIN_LOOKUP_FRM_DESC_ENV_DATA_SZE(FRAME_CNT)) /**< LIN PROTO Layer per 'Frame Description List' environment data size */

/* ****************************************************************************/
/*                               TRANS LAYER                                  */
/* ****************************************************************************/
#if ( ( LINTRANSIMP_MULTI_PDU_SUPPORT_REQ == 1u ) || ( LINTRANSIMP_MULTI_PDU_SUPPORT_RESP == 1u ) )
 #define LINTRANSIMP_MULTI_PDU_ENV_OVERHEAD 2u
#else
 #define LINTRANSIMP_MULTI_PDU_ENV_OVERHEAD 0u
#endif 

#if ( LINTRANSIMP_INTERM_RESP_SUPPORT == 1u ) 
 #define LINTRANSIMP_INTERM_RESP_ENV_OVERHEAD 6u
#else
 #define LINTRANSIMP_INTERM_RESP_ENV_OVERHEAD 0u
#endif 

#if ( LINTRANSIMP_PER_LIST_NAD_CHECK_SUPPORT == 1u )
 #define LINTRANSIMP_PER_LIST_NAD_CHECK_ENV_OVERHEAD 1u
#else
 #define LINTRANSIMP_PER_LIST_NAD_CHECK_ENV_OVERHEAD 0u
#endif 

#if ( LINTRANSIMP_MANUAL_TRIGGER_SUPPORT == 1u )
 #define LINTRANSIMP_MANUAL_TRIGGER_ENV_OVERHEAD 4u
#else
 #define LINTRANSIMP_MANUAL_TRIGGER_ENV_OVERHEAD 0u
#endif 

/**< LIN TRANS Layer environment data size. */
#define LINTRANSIMP_ENVIRONMENT_DATA_SZE(MSG_BUF_SZE,REQ_FRM_LEN,INTERM_RESP_LEN) \
  ( ( 115u + LINTRANSIMP_MULTI_PDU_ENV_OVERHEAD + LINTRANSIMP_INTERM_RESP_ENV_OVERHEAD + LINTRANSIMP_MANUAL_TRIGGER_ENV_OVERHEAD ) + \
  ( ( (LINTRANSIMP_MAX_SID_DESCRIPTION_LISTS) ) * ( 4u + (2u * LINTRANSIMP_PER_LIST_NAD_CHECK_ENV_OVERHEAD) ) ) + ( ( (LINTRANSIMP_MAX_CALLBACK_TABLES) ) * 4u ) + \
  ( (MSG_BUF_SZE) + (REQ_FRM_LEN) + (INTERM_RESP_LEN) ) ) \

/* ****************************************************************************/
/*                                DIAG LAYER                                  */
/* ****************************************************************************/
#if LINDIAG_SUP_SNPD == 1 && LINDIAG_SUP_DATASTG == 1
  #define LINDIAGIMP_ENVIRONMENT_DATA_SZE (42u +4+ (LINDIAG_MAX_RBI_TBL_CNT * 2u) + (LINDIAG_MAX_SUPPORTED_TIMER * 10u))  /**< LIN DIAG Layer environment data size. */
#elif LINDIAG_SUP_SNPD == 0 && LINDIAG_SUP_DATASTG == 1
  #define LINDIAGIMP_ENVIRONMENT_DATA_SZE (32u +4+ (LINDIAG_MAX_RBI_TBL_CNT * 2u) + (LINDIAG_MAX_SUPPORTED_TIMER * 10u))  /**< LIN DIAG Layer environment data size. */
#elif LINDIAG_SUP_SNPD == 1 && LINDIAG_SUP_DATASTG == 0
  #define LINDIAGIMP_ENVIRONMENT_DATA_SZE (38u +4+ (LINDIAG_MAX_RBI_TBL_CNT * 2u) + (LINDIAG_MAX_SUPPORTED_TIMER * 10u))  /**< LIN DIAG Layer environment data size. */
#else
  #define LINDIAGIMP_ENVIRONMENT_DATA_SZE (28u +4+ (LINDIAG_MAX_RBI_TBL_CNT * 2u) + (LINDIAG_MAX_SUPPORTED_TIMER * 10u))  /**< LIN DIAG Layer environment data size. */
#endif

/* ****************************************************************************/
/*                                UDS LAYER                                  */
/* ****************************************************************************/
#ifdef LINUDS_USE_UDS_LAYER
#if LINUDS_SUP_SNPD == 1 && LINUDS_SUP_DATASTG == 1
  #define LINUDSIMP_ENVIRONMENT_DATA_SZE (50u + (LINUDS_MAX_RBI_TBL_CNT * 2u) + (LINUDS_MAX_DID_TBL_CNT * 2u) + (LINUDS_MAX_ROUTINEID_TBL_CNT * 2u) + (LINUDS_MAX_SUPPORTED_TIMER * 12u))
#elif LINUDS_SUP_SNPD == 0 && LINUDS_SUP_DATASTG == 1
  #define LINUDSIMP_ENVIRONMENT_DATA_SZE (40u + (LINUDS_MAX_RBI_TBL_CNT * 2u) + (LINUDS_MAX_DID_TBL_CNT * 2u) + (LINUDS_MAX_ROUTINEID_TBL_CNT * 2u) + (LINUDS_MAX_SUPPORTED_TIMER * 12u))
#elif LINUDS_SUP_SNPD == 1 && LINUDS_SUP_DATASTG == 0
  #define LINUDSIMP_ENVIRONMENT_DATA_SZE (44u + (LINUDS_MAX_RBI_TBL_CNT * 2u) + (LINUDS_MAX_DID_TBL_CNT * 2u) + (LINUDS_MAX_ROUTINEID_TBL_CNT * 2u) + (LINUDS_MAX_SUPPORTED_TIMER * 12u))
#else
  #define LINUDSIMP_ENVIRONMENT_DATA_SZE (36u + (LINUDS_MAX_RBI_TBL_CNT * 2u) + (LINUDS_MAX_DID_TBL_CNT * 2u) + (LINUDS_MAX_ROUTINEID_TBL_CNT * 2u) + (LINUDS_MAX_SUPPORTED_TIMER * 12u))
#endif
#endif

#endif /* LIN_ENVDATASIZES_H430_H_ */

/*! @} ELMOS_LIN_DRIVER */
