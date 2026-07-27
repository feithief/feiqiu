/***************************************************************************//**
 * @file      LinDrvImp_CompilationConfig_Example.h
 *
 * @creator   POE
 * @created   23.02.2021
 *
 * @brief     Compilation-changing settings of the Elmos LIN Driver.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup LIN_E52139A E52139A
 * @{
 *
 * $Id: LinDrvImp_CompilationConfig_Example.h 9395 2021-11-18 11:27:38Z poe $
 *
 * $Revision: 9395 $
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
 * Disclaimer:
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

#ifndef LINDRVIMP_COMPILATIONCONFIG_H_
#define LINDRVIMP_COMPILATIONCONFIG_H_

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINDRVIMP_AQR_MOD_SZE               0                 /**< Created to debug layer sizes. Here not necessary. */

#define LINDRVIMP_CHECK_DEFS_ENVDATA_SIZES  1                 /**< if set to (1) sizes defined for environment data of different LIN-Drv
                                                                   layers will be checked with static_assert during compile time */

/***************************************************************************//**
 * Control access to LIN Driver modules via the global interface functions
 * structs.
 ******************************************************************************/
#define LINBUSIMP_EXT_IFFUN_STRCT_ACCESS                1u
#define LINLOOKUPIMP_FIXED_EXT_IFFUN_STRCT_ACCESS       1u
#define LINLOOKUPIMP_SEQUENTIAL_EXT_IFFUN_STRCT_ACCESS  1u
#define LINPROTOIMP_EXT_IFFUN_STRCT_ACCESS              1u
#define LINTRANSIMP_EXT_IFFUN_STRCT_ACCESS              1u
#define LINDATASTGIMP_EXT_IFFUN_STRCT_ACCESS            1u
#define LINDIAGIMP_EXT_IFFUN_STRCT_ACCESS               1u
#define LINUDSIMP_EXT_IFFUN_STRCT_ACCESS                1u

/* ****************************************************************************/
/*                                LIN BUS LAYER                               */
/* ****************************************************************************/
#define LINBUSIMP_ENABLE_SEND_HEADER                1u        /**< Enable master functionality. (Send LIN frame header) */
#define LINBUSIMP_ENABLE_MEAS_DONE_CALLBACK         1u        /**< if enabled, bus invokes MeasDone callback to control baudrate updates. */
#define LINBUSIMP_ENABLE_AUTO_UPDATE_BAUDRATE       1u        /**< if enabled (and MeadDone() callback is not used, the measured baudrate will be used as starting point for next break and sync measurement. */
#define LINBUSIMP_ENABLE_AUTO_ADDR                  1u        /**< if enabled adds auto addressing support to this implementation */
#define LINBUSIMP_ENABLE_AUTO_ADDR_DEBUG            0u        /**< if enabled adds callbacks which dump ADC sample values during AA sequence */

#define LINBUSIMP_DEFAULT_IDLE_TIMEOUT              5000u     /**< idle timeout in ms */
#define LINBUSIMP_DEFAULT_WAKEUP_DURATION           2500u     /**< 2.5ms wakeup pulse duration (0.25 - 5ms ). */
#define LINBUSIMP_DEFAULT_DEBOUNCER_VALUE           2u        /**< default debouncer value */
#define LINBUSIMP_DEFAULT_SHORT_DEBOUNCER_VALUE     0u        /**< default debouncer value for short detection */
#define LINBUSIMP_DEFAULT_SEND_HEADER_BREAK_LEN     13u       /**< send header break length*/ 
#define LINBUSIMP_DEFAULT_TIMER_MESSAGE_TIMEOUT_EXT (5u*16u)  /**< 5 bit times ( * 16 ) added to cope with some header time measurement jitter. */
#define LINBUSIMP_DEFAULT_MAX_DATA_LEN              254u      /**< max. message data length (bytes) */

/* ****************************************************************************/
/*                             LIN DATA STG LAYER                             */
/* ****************************************************************************/

/* ****************************************************************************/
/*                                LOOKUP LAYER                                */
/* ****************************************************************************/
 #define LINLOOKUP_USE_FIX_LOOKUP  0u /**< If set to 1, use fixed lookup layer */
 #define LINLOOKUP_USE_SEQ_LOOKUP  1u /**< If set to 1, use sequential lookup layer */
 
 /* 2021-07-29, poe: Optimization
 * Moved from lookup interface */
 #define LINLOOKUP_INTERFACE_MAX_FRAME_DESCIPTION_LISTS  4u /**< Maximal supported 'Frame Description Lists'. @see LinProtoIf_sFrameDescription_t **/  
 /* 2021-07-29, poe: Optimization */
 
/* ****************************************************************************/
/*                               LIN PROTO LAYER                              */
/* ****************************************************************************/
#define LINPROTOIMP_SUPPORT_SIGNAL_LISTS           1u    
#define LINPROTOIMP_MIN_LIN_MSG_SZE                8u    /**< Minimal configurable size of a LIN frame. */
#define LINPROTOIMP_MAX_LIN_MSG_SZE                128u  /**< Maximal configurable size of a LIN frame. */
#define LINPROTO_INTERFACE_MAX_CALLBACK_TABLES     3u    /**< Maximal allowed count of callback tables which can be added to the PROTO layer. */

/* ****************************************************************************/
/*                               LIN TRANS LAYER                              */
/* ****************************************************************************/
#define LINTRANSIMP_MULTI_PDU_SUPPORT_REQ         1u  /**< Define for LIN TRANS Multi PDU support for the request reception. */
#define LINTRANSIMP_MULTI_PDU_SUPPORT_RESP        1u  /**< Define for LIN TRANS Multi PDU support for the response transmission. */

#define LINTRANSIMP_ABORT_ON_ERROR_REQ            1u  /**< If set to 1 the request is abortet if an BUS or PROTO error occures, otherwise the false frame is only skipped. */
#define LINTRANSIMP_ABORT_ON_ERROR_RESP           1u  /**< If set to 1 the respone is abortet if an BUS or PROTO error occures, otherwise the false frame is only skipped. */
#define LINTRANSIMP_ABORT_ON_NEW_DIAG_REQ         1u  /**< If set to 1 communication to other devices aborts request recetion. (neeeded for full LIN conformance */

#define LINTRANSIMP_INTERM_RESP_SUPPORT           1u  /**< If set to 1 transport layer supports race-condition free handling of intermediate responses. */
#define LINTRANSIMP_PER_LIST_NAD_CHECK_SUPPORT    1u  /**< If set to 1 transport layer supports race-condition free handling of intermediate responses. */

#define LINTRANSIMP_MANUAL_TRIGGER_SUPPORT        1u  /**< If set to 1 transport layer supports . */

#define LINTRANSIMP_MAX_SID_DESCRIPTION_LISTS     3u  /**< Max. number of SID description list which can be assigned to the transport layer */
#define LINTRANSIMP_MAX_CALLBACK_TABLES           2u  /**< Max. number of calback tables */

/* Runtime config defaults */ 
#define LINTRANSIMP_DEFAULT_ABRT_ON_NEW_DIAG_REQ  1u  /**< If this Flag is set to (1) the frame processing is abortet
                                                           when new diag-frame is received - see lin2.2-conformance-test 13.2.1*/


/* ****************************************************************************/
/*                               LIN DIAG LAYER                              */
/* ****************************************************************************/
#define LINDIAG_MAX_PHYADR_RDBI_LST_LEN      8u
#define LINDIAG_MAX_FUNADR_RDBI_LST_LEN      3u

#define LINDIAG_MAX_SUPPORTED_TIMER          3u

#define LINDIAG_SUP_ASSIGN_NAD               1u   /* LIN Diagnostic Service - Assign NAD - 0xB0 */
#define LINDIAG_SUP_ASSIGN_FRAME_IDENTIFIER  0u   /* LIN Diagnostic Service - Assign frame identifier - 0xB1 */
#define LINDIAG_SUP_READ_BY_ID               1u   /* LIN Diagnostic Service - Read by Identifier - 0xB2 */
#define LINDIAG_SUP_CONDITIONAL_CHANGE_NAD   1u   /* LIN Diagnostic Service - Conditional Change NAD - 0xB3 */
#define LINDIAG_SUP_SAVE_CONFIGURATION       1u   /* LIN Diagnostic Service - Save Configuration - 0xB6 */
#define LINDIAG_SUP_ASSIGN_FRAME_ID_RANGE    1u   /* LIN Diagnostic Service - Assign frame identifier range - 0xB7 */
#define LINDIAG_SUP_TARGETED_RESET           0u   /* LIN Diagnostic Service - TargetedReset - 0xB5 (if SNPD is also mapped to this service, it will take precedence)  */

#define LINDIAG_SUP_DATASTG                  0u
#define LINDIAG_SUP_SNPD                     1u   /* Auto-Addressing (SNPD) */
#define LINDIAG_INIT_DEFAULT_VALUES          1u

#if (LINDIAG_INIT_DEFAULT_VALUES == 1)
#define LINDIAG_DEFAULT_NAD                  1u
#define LINDIAG_DEFAULT_SERIAL_NUMBER        0x00000000u
#define LINDIAG_DEFAULT_SUPPLIERID           0x23u
#define LINDIAG_DEFAULT_FUNCTIONID           0x0117u
#define LINDIAG_DEFAULT_VARIANTID            0x22u
#endif

#define LINDIAG_MAX_RBI_TBL_CNT              3u

/* ****************************************************************************/
/*                                LIN UDS LAYER                               */
/* ****************************************************************************/
#define LINUDS_MAX_PHYADR_RDBI_LST_LEN      8u

#define LINUDS_MAX_SUPPORTED_TIMER          3u

#define LINUDS_SUP_ASSIGN_NAD               1u   /* LIN Diagnostic Service - Assign NAD - 0xB0 */
#define LINUDS_SUP_ASSIGN_FRAME_IDENTIFIER  0u   /* LIN Diagnostic Service - Assign frame identifier - 0xB1 */
#define LINUDS_SUP_READ_BY_ID               1u   /* LIN Diagnostic Service - Read by Identifier - 0xB2 */
#define LINUDS_SUP_CONDITIONAL_CHANGE_NAD   1u   /* LIN Diagnostic Service - Conditional Change NAD - 0xB3 */
#define LINUDS_SUP_DATA_DUMP                0u   /* LIN Diagnostic Service - Data Dump - 0xB4 */
#define LINUDS_SUP_SAVE_CONFIGURATION       0u   /* LIN Diagnostic Service - Save Configuration - 0xB6 */
#define LINUDS_SUP_ASSIGN_FRAME_ID_RANGE    1u   /* LIN Diagnostic Service - Assign frame identifier range - 0xB7 */

#define LINUDS_SUP_DATASTG                  0u
#define LINUDS_SUP_SNPD                     0u   /* Auto-Addressing (SNPD) */
#define LINUDS_INIT_DEFAULT_VALUES          0u

#if (LINUDS_INIT_DEFAULT_VALUES == 1)
#define LINUDS_DEFAULT_NAD                  1u
#define LINUDS_DEFAULT_SERIAL_NUMBER        0x01020304u
#define LINUDS_DEFAULT_SUPPLIERID           0x23u
#define LINUDS_DEFAULT_FUNCTIONID           0x0117u
#define LINUDS_DEFAULT_VARIANTID            0x22u
#endif

#define LINUDS_UDS_SUP_DIAG_SESSION_CTRL    1u   /* UDS - Diagnostic Session Control - 0x10 */
#define LINUDS_UDS_SUP_ECU_RESET            1u   /* UDS - ECU Reset - 0x11 */
#define LINUDS_UDS_SUP_SEC_ACCESS           1u   /* UDS - Security Access - 0x27 */
#define LINUDS_UDS_SUP_COMM_CTRL            1u   /* UDS - Communication Control - 0x28 */
#define LINUDS_UDS_SUP_TEST_PRESNT          1u   /* UDS - Tester Present - 0x3E */
#define LINUDS_UDS_SUP_RESP_ON_EVNT         1u   /* UDS - Response On Event  - 0x86 */
#define LINUDS_UDS_SUP_CTRL_DTC_SET         1u   /* UDS - Control DTC Setting - 0x85 */
#define LINUDS_UDS_SUP_RDBI                 1u   /* UDS - Read Data By Identifier - 0x22 */
#define LINUDS_UDS_SUP_WDBI                 1u   /* UDS - Write Data By Identifier - 0x2E */
#define LINUDS_UDS_SUP_CLR_DTC_INFO         1u   /* UDS - Clear Diagnostic Information - 0x14 */
#define LINUDS_UDS_SUP_READ_DTC_INFO        1u   /* UDS - Read DTC Information - 0x19 */
#define LINUDS_UDS_SUP_IO_CTRL_BY_ID        1u   /* UDS - Input Output Control By Identifier - 0x2F */
#define LINUDS_UDS_SUP_ROUTINE_CTRL         1u   /* UDS - Routine Control - 0x31 */
#define LINUDS_UDS_SUP_REQ_DWNLD            1u   /* UDS - Request Download - 0x34 */
#define LINUDS_UDS_SUP_REQ_UPLD             1u   /* UDS - RequestUpload - 0x35 */
#define LINUDS_UDS_SUP_TRANS_DATA           1u   /* UDS - Transfer Data - 0x36 */
#define LINUDS_UDS_SUP_REQ_TRANS_EXIT       1u   /* UDS - Request Transfer Exit - 0x37 */

#define LINUDS_MAX_RBI_TBL_CNT              3u
#define LINUDS_MAX_DID_TBL_CNT              3u
#define LINUDS_MAX_ROUTINEID_TBL_CNT        3u

#endif /* LINDRVIMP_COMPILATIONCONFIG_H_ */

/*! @} */ /* E52139A */
/*! @} */ /* ELMOS_LIN_DRIVER */
