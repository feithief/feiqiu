/***************************************************************************//**
 * @file		LinBus_Types.h
 *
 * @creator		sbai
 * @created		25.03.2015
 *
 * @brief  		Definitions of basic data types for the 'LIN BUS Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup BUS BUS Layer
 * @{
 *
 * $Id$
 *
 * $Revision$
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

#ifndef LINBUS_TYPES_H_
#define LINBUS_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/
typedef Lin_pvoid_t      LinBusIf_pGenericEnvData_t;       /**< Generic pointer to environment data of the BUS layer module */
typedef Lin_cpvoid_t     LinBusIf_cpGenericEnvData_t;      /**< Generic const pointer to environment data of the BUS layer module */
typedef Lin_pvoid_t      LinBusIf_pGenericImpCfgData_t;    /**< Generic pointer to configuration parameter of the specific BUS layer implementation */
typedef Lin_cpvoid_t     LinBusIf_cpGenericImpCfgData_t;   /**< Generic const pointer to configuration parameter of the specific BUS layer implementation */
typedef Lin_EnvDataSze_t LinBusIf_EnvDataSze_t;            /**< Type for bus layer environment data */
typedef Lin_pvoid_t      LinBusIf_pGenericCbCtxData_t;     /**< Pointer to BUS callback context data */

typedef Lin_Error_t  LinBusIf_Error_t;                     /**< Mapping for bus error codes to generic error type */

typedef Lin_pGenericInterfaceFunctions_t LinBusIf_pGenericInterfaceFunctions_t; /**< Layer-specific redefinition of 'Lin_pGenericInterfaceFunctions_t' */

typedef Lin_uint8_t       LinBusIf_Data_t;                 /**< Basic BUS data type */
typedef LinBusIf_Data_t*  LinBusIf_pData_t;                /**< Pointer to LinBusIf_Data_t */
typedef LinBusIf_Data_t** LinBusIf_ppData_t;               /**< Pointer-Pointer to LinBusIf_Data_t */

typedef Lin_BufLength_t          LinBusIf_BufLength_t;     /**< BUS layer data type of buffer length values. */
typedef LinBusIf_BufLength_t* LinBusIf_pBufLength_t;       /**< Pointer to LinBusIf_BufLength_t */

typedef       Lin_uint8_t          LinBusIf_PID_t;         /**< Data type for PID representation */
typedef       Lin_uint8_t          LinBusIf_FrameID_t;     /**< Data type for frame ID representation */
typedef       LinBusIf_FrameID_t*  LinBusIf_pFrameID_t;    /**< Pointer to data type for frame ID representation */
typedef const LinBusIf_FrameID_t*  LinBusIf_cpFrameID_t;   /**< Const pointer to data type for frame ID representation */
typedef       LinBusIf_FrameID_t** LinBusIf_ppFrameID_t;   /**< Pointer-pointer to data type for frame ID representation */
typedef const LinBusIf_FrameID_t** LinBusIf_cppFrameID_t;  /**< Const pointer-pointer to data type for frame ID representation */

typedef Lin_uint32_t LinBusIf_Baudrate_t;                  /**< Data type configured baud rate */

typedef Lin_uint16_t LinBusIf_BreakLen_t;                  /**< Data type for break length representation. */ 

typedef Lin_uint32_t    LinBusIf_Tick_t;                   /**< Data type for tick count representation */
typedef LinBusIf_Tick_t LinBusIf_TimeOut_t;                /**< Data type for timeout values */


struct         LinBusIf_sInterfaceFunctions;                                     /**< Forward declaration of LIN BUS layer interface functions. */
typedef struct LinBusIf_sInterfaceFunctions     LinBusIf_sInterfaceFunctions_t;  /**< Typedef for LinBusIf_sInterfaceFunctions. */
typedef        LinBusIf_sInterfaceFunctions_t*  LinBusIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinBusIf_sInterfaceFunctions. */
typedef const  LinBusIf_sInterfaceFunctions_t*  LinBusIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinBusIf_sInterfaceFunctions. */

struct         LinBusIf_sCallbackFunctions;                                     /**< Forward declaration of LIN BUS layer callback functions. */
typedef struct LinBusIf_sCallbackFunctions     LinBusIf_sCallbackFunctions_t;   /**< Typedef for LinBusIf_sCallbackFunctions. */
typedef        LinBusIf_sCallbackFunctions_t*  LinBusIf_pCallbackFunctions_t;   /**< Typedef of pointer to LinBusIf_sCallbackFunctions. */
typedef const  LinBusIf_sCallbackFunctions_t*  LinBusIf_cpCallbackFunctions_t;  /**< Typedef of constant pointer to LinBusIf_sCallbackFunctions. */

struct         LinBusIf_sThis;                     /**< Forward declaration of LIN BUS layer This-Pointer. */
typedef struct LinBusIf_sThis    LinBusIf_sThis_t; /**< Typedef for LinBusIf_sThis. */
typedef        LinBusIf_sThis_t* LinBusIf_pThis_t; /**< Typedef of pointer to LinBusIf_sThis. */

struct         LinBusIf_sInitParam;                          /**< Forward declaration of LIN BUS layer initialization parameter struct. */
typedef struct LinBusIf_sInitParam    LinBusIf_sInitParam_t; /**< Typedef for LinBusIf_sInitParam */
typedef        LinBusIf_sInitParam_t* LinBusIf_pInitParam_t; /**< Typedef of pointer to LinBusIf_sInitParam */

/***************************************************************************//**
 * @brief LIN BUS CRC type.
 *
 * Enumerator for LIN CRC calculation methods.
 ******************************************************************************/
 enum LinBusIf_eCrcType
 {
   LinBusIf_CRC_Extended = 0,         /**< Extended LIN CRC calculation (LIN Specification >= 2.0) */
   LinBusIf_CRC_Classic  = 1          /**< Classic LIN CRC calculation (LIN Specification < 2.0)   */
 };

 typedef enum LinBusIf_eCrcType    LinBusIf_eCrcType_t;  /**< Typedef of LinBusIf_eCrcType */
 typedef      LinBusIf_eCrcType_t* LinBusIf_pCrcType_t;  /**< Typedef of pointer to LinBusIf_eCrcType_t */

/***************************************************************************//**
 * @brief LIN BUS state enumeration
 ******************************************************************************/
enum LinBusIf_eState
{
  LinBusIf_STATE_OFFLINE           = 0,  /**< Bus is offline, i.e. after shutdown */
  LinBusIf_STATE_SLEEP             = 1,  /**< Bus is in sleep mode. may wakeup on next Rx line event */
  LinBusIf_STATE_IDLE              = 2,  /**< Bus is waiting for a valid break and sync (performs autobaud measurement) */
  LinBusIf_STATE_RECEIVING_DATA    = 3,  /**< Bus is receiving a LIN frame. */
  LinBusIf_STATE_TRANSMITING_DATA  = 4   /**< Bus is transmitting a LIN frame. */
};

typedef enum LinBusIf_eState LinBusIf_eState_t; /**< Typedef of LinBusIf_eState. */

/***************************************************************************//**
 * @brief LIN BUS layer error enumerator
 *
 * @misra{M3CM Dir-10.1. - PRQA Msg 4521,
 * The Elmos LIN Driver defines areas of error codes for every module and sub-areas between
 * general defined interface error codes and implementation specific error codes. To link
 * between this areas some arithmetic offset calculation has to be done between different
 * enum types.,
 * Conflicts in  signedness.,
 * Always make sure unsigned values ate defined and used.}
 ******************************************************************************/
// PRQA S 4521 ++
enum LinBusIf_eErrorCodes
{
  LinBusIf_ERROR_NO_ERROR              = Lin_NO_ERROR,             /**< No error at all. */

  LinBusIf_ERR_INIT                    = Lin_ERROR_AREA_BUS + 0u,   /**< Initialization error */
  LinBusIf_ERR_HW_MISSMATCH            = Lin_ERROR_AREA_BUS + 1u,   /**< LIN driver has been configured for a different SCI version than present (i.e. missing features, not every implementation can detect this error) */
  LinBusIf_ERR_HW_FAIL                 = Lin_ERROR_AREA_BUS + 2u,   /**< Hardware failure i.e. TxD timeout detected */

  LinBusIf_ERR_SYNC_FAIL               = Lin_ERROR_AREA_BUS + 3u,   /**< Corrupted or invalid sync byte received */
  LinBusIf_ERR_PID_PARITY              = Lin_ERROR_AREA_BUS + 4u,   /**< Parity error in PID */
  
  LinBusIf_ERR_BUS_COLLISION           = Lin_ERROR_AREA_BUS + 5u,   /**< Bus collision detected */
  LinBusIf_ERR_FRAMING_ERROR           = Lin_ERROR_AREA_BUS + 6u,   /**< Received data expose framing error */
  LinBusIf_ERR_RX_OVERRUN              = Lin_ERROR_AREA_BUS + 7u,   /**< Reciever service too slow, some data are lost */

  LinBusIf_ERR_BUFFER_ERROR            = Lin_ERROR_AREA_BUS + 8u,   /**< Data buffer setup incorrectly */

  LinBusIf_ERR_HEADER_TIMEOUT          = Lin_ERROR_AREA_BUS + 9u,   /**< Message header time out. */
  LinBusIf_ERR_MESSAGE_TIMEOUT         = Lin_ERROR_AREA_BUS + 10u,  /**< Message content time out. */
  LinBusIf_ERR_MESSAGE_TIMEOUT_NO_DATA = Lin_ERROR_AREA_BUS + 11u,  /**< Message content time out but no message data received at all */

  LinBusIf_ERR_CRC                     = Lin_ERROR_AREA_BUS + 12u,  /**< LIN CRC error */

  LinBusIf_ERR_UNEXPECTED_SLEEP        = Lin_ERROR_AREA_BUS + 13u,  /**< */
  LinBusIf_ERR_UNEXPECTED_SHUTDOWN     = Lin_ERROR_AREA_BUS + 14u,  /**< */

  LinBusIf_ERR_SHORT_DETECTED          = Lin_ERROR_AREA_BUS + 15u,  /**< Will inidcate detection of a short (to VBAT) condition, if supported in particular BUS implementation. Depending on HW may be accompanied by a LinBusIf_ERR_BUS_COLLISION as well*/

  LinBusIf_ERR_CONCURRENT_MESSAGE      = Lin_ERROR_AREA_BUS + 16u,  /**< Will indicate a message reception has been aborted by a concurrent message header */
  LinBusIf_ERR_UNEXPECTED_CURR_DETECTED= Lin_ERROR_AREA_BUS + 17u,  /**< Will indicate a measured current imprinted on the bus after being addressed  */
  LinBusIf_ERR_NEG_CURR_DETECTED       = Lin_ERROR_AREA_BUS + 18u,  /**< Will indicate a measured negative current indicating a possible switch of LIN M & S connection.  */
  
  LinBusIf_ERR_IMPL_ERROR_AREA         = Lin_ERROR_AREA_BUS + (LIN_ERROR_AREA_SIZE/2u) /**< Any additional implementation specific errors start here. */  
};
// PRQA S 4521 --

typedef enum LinBusIf_eErrorCodes LinBusIf_eErrorCodes_t; /**< Typedef of pointer to LinBusIf_eErrorCodes */

/***************************************************************************//**
 * @brief An enumeration indicating how to proceed with a measured baud rate value.
 ******************************************************************************/
enum LinBusIf_eMeasAction
{
  LinBusIf_MEASACT_DISMISS           = 0,    /**< The measured baud rate should be ignored. Continue with the previous value. */
  LinBusIf_MEASACT_RESET             = 1,    /**< The measured baud rate should be ignored. Continue with the expected (nominal) baud rate value. */
  LinBusIf_MEASACT_UPDATE            = 2,    /**< The measured baud rate should be used as the new default baud rate value. */
  LinBusIf_MEASACT_UPDATE_NOMINAL    = 3,     /**< The measured baud rate should be used as the new nominal baud rate (recalculates timings, higher computing effort). */
  LinBusIf_MEASACT_ABORT             = 4    /**< The measured baud rate is too much off, stop further processing, and wait for next break. */
} ;

typedef enum LinBusIf_eMeasAction LinBusIf_eMeasAction_t; /**< Typedef of pointer to LinBusIf_eMeasAction */

/***************************************************************************//**
 * @brief An enumeration indicating how to proceed according to a received PID
 ******************************************************************************/
enum LinBusIf_eFrameIdAction
{
  LinBusIf_PIDACT_IGNORE   = 0,    /**< Invalid or unknown frame ID. (Bus driver continues to wait for next break) */
  LinBusIf_PIDACT_RECEIVE  = 1,    /**< This frame ID indicates a message body to be received. 'rxtx__buf' and 'buf_len' need to be filled accordingly. */
  LinBusIf_PIDACT_TRANSMIT = 2,    /**< This frame ID indicates a message body to be transmitted. 'rxtx__buf' and 'buf_len' need to be filled accordingly. */
} ;

typedef enum LinBusIf_eFrameIdAction LinBusIf_eFrameIdAction_t; /**< Typedef of pointer to LinBusIf_eFrameIdAction */

/****************************************************************************//**
 * @brief An enumeration which indicates how to proceed after a message reception.
 ******************************************************************************/
enum LinBusIf_eMsgAction
{
  LinBusIf_MSGACT_CONTINUNE         = 0,    /**< The LIN driver should continue regular operation. */
  LinBusIf_MSGACT_SLEEP             = 1,    /**< The message implies go to sleep. */
};

typedef enum LinBusIf_eMsgAction LinBusIf_eMsgAction_t; /**< Typedef of pointer to LinBusIf_eMsgAction */

/***************************************************************************//**
 * @brief An enumeration which indicates how to proceed in case of an timeout.
 ******************************************************************************/
enum LinBusIf_eIdleAction
{
  LinBusIf_IDLEACT_CONTINUNE         = 0,    /**< The LIN driver should continue in wake state regardless of timeout. */
  LinBusIf_IDLEACT_SLEEP             = 1,    /**< Go to sleep. */
};

typedef enum LinBusIf_eIdleAction LinBusIf_eIdleAction_t; /**< Typedef of pointer to LinBusIf_eIdleAction */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINBUS_TYPES_H_ */

/*! @} *//* BUS */
/*! @} *//* ELMOS_LIN_DRIVER */

