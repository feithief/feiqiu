/***************************************************************************//**
 * @file      LinDrvImp_ROMCompilationConfig.h
 *
 * @creator   sbai
 * @created   09.12.2016
 *
 * @brief     Compilation-changing settings of the Elmos LIN Driver for modules located in ROM.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup LIN_E52139A E52139A
 * @{
 *
 * $Id: $
 *
 * $Revision: $
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor AG provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
 * Elmos Semiconductor AG shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor AG. To the extend permitted by law 
 * Elmos Semiconductor AG hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 ******************************************************************************/

#ifndef LINDRVIMP_ROMCOMPILATIONCONFIG_H_
#define LINDRVIMP_ROMCOMPILATIONCONFIG_H_

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define BLDMAIN_USE_POWERCTRL_STANDBY (0u)


/****************************************************************************//**
 * LINDRVIMP
 *
 *******************************************************************************/
#define LINDRVIMP_CHECK_DEFS_ENVDATA_SIZES  0u
#define LINDRVIMP_AQR_MOD_SZE 0u

/***************************************************************************//**
 * Control access to LIN Driver modules via the global interface functions
 * structs.
 ******************************************************************************/
#define LINBUSIMP_EXT_IFFUN_STRCT_ACCESS                1u
#define LINLOOKUPIMP_SEQUENTIAL_EXT_IFFUN_STRCT_ACCESS  1u
#define LINPROTOIMP_EXT_IFFUN_STRCT_ACCESS              1u
#define LINTRANSIMP_EXT_IFFUN_STRCT_ACCESS              1u
#define LINDATASTGIMP_EXT_IFFUN_STRCT_ACCESS            1u
#define LINDIAGIMP_EXT_IFFUN_STRCT_ACCESS               1u
#define LINUDSIMP_EXT_IFFUN_STRCT_ACCESS                1u
#define FLASHACCESS_EXT_IFFUN_STRCT_ACCESS              1u
#define EEACCESS_EXT_IFFUN_STRCT_ACCESS                 1u
#define POWERCTRL_EXT_IFFUN_STRCT_ACCESS                1u
/* ****************************************************************************/
/*                                LIN BUS LAYER                               */
/* ****************************************************************************/
#define LINBUSIMP_ENABLE_SEND_HEADER                1u        /*< Enable master functionality. (Send LIN frame header) */
#define LINBUSIMP_ENABLE_MEAS_DONE_CALLBACK         1u        /**< if enabled, bus invokes MeasDone callback to control baudrate updates. */
#define LINBUSIMP_ENABLE_AUTO_UPDATE_BAUDRATE       1u        /**< if enabled (and MeadDone() callback is not used, the measured baudrate will be used as starting point for next break and sync measurement. */
#define LINBUSIMP_ENABLE_AUTO_ADDR                  1u        /**< if enabled adds auto addressing support to this implementation */
#define LINBUSIMP_ENABLE_AUTO_ADDR_DEBUG            0u        /**< if enabled adds callbacks which dump ADC sample values during AA sequence */

#if (LINBUSIMP_ENABLE_AUTO_ADDR == 0u)
 #warning "SNPD IS DISABLED!"
#endif

#if (LINBUSIMP_ENABLE_AUTO_ADDR_DEBUG == 1u)
  #warning "SNPD DEBUG OUTPUT ENABLED!"
#endif

#define LINBUSIMP_DEFAULT_IDLE_TIMEOUT              5000u     /*< idle timeout in ms */
#define LINBUSIMP_DEFAULT_WAKEUP_DURATION           2500u     /*< 2.5ms wakeup pulse duration (0.25 - 5ms ). */
#define LINBUSIMP_DEFAULT_DEBOUNCER_VALUE           2u        /**< default debouncer value */
#define LINBUSIMP_DEFAULT_SHORT_DEBOUNCER_VALUE     0u        /**< default debouncer value for short detection */
#define LINBUSIMP_DEFAULT_SEND_HEADER_BREAK_LEN     13u       /**< send header break length*/ 
#define LINBUSIMP_DEFAULT_TIMER_MESSAGE_TIMEOUT_EXT (5u*16u)  /**< 5 bit times ( * 16 ) added to cope with some header time measurement jitter. */
#define LINBUSIMP_DEFAULT_MAX_DATA_LEN              254u      /**< max. message data length (bytes) */
#define LINDIAG_MAX_PHYADR_RDBI_LST_LEN      8u
#define LINDIAG_MAX_FUNADR_RDBI_LST_LEN      3u

#define LINDIAG_MAX_SUPPORTED_TIMER          3u
#define LINDIAG_MAX_RBI_TBL_CNT              3u
/* ****************************************************************************/
/*                               LIN PROTO LAYER                              */
/* ****************************************************************************/
#define LINPROTOIMP_SUPPORT_SIGNAL_LISTS           1u
#define LINPROTOIMP_MIN_LIN_MSG_SZE                8u    /* Minimal configurable size of a LIN frame. */
#define LINPROTOIMP_MAX_LIN_MSG_SZE                128u  /* Maximal configurable size of a LIN frame. */
#define LINPROTOIMP_INTERFACE_MAX_CALLBACK_TABLES  3u    /**< Maximal allowed count of callback tables which can be added to the PROTO layer. */

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
/*                                LOOKUP LAYER                                */
/* ****************************************************************************/
 #define LINLOOKUP_USE_FIX_LOOKUP  0u
 #define LINLOOKUP_USE_SEQ_LOOKUP  1u


#endif /* LINDRVIMP_ROMCOMPILATIONCONFIG_H_ */

/*! @} */ /* M52139A */
/*! @} */ /* ELMOS_LIN_DRIVER */
