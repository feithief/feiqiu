/***************************************************************************//**
 * @file			LinTrans_Types.h
 *
 * @creator		sbai
 * @created		25.03.2015
 *
 * @brief  		Definitions of basic data types for the 'LIN TRANS Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup TRANS TRANS Layer
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

#ifndef LINTRANS_TYPES_H_
#define LINTRANS_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinBus_Types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINTRANSIF_GOTOSLEEP_NAD  0x00u /**< NAD of a 'Go to sleep' request. (See also: LIN 2.2a Specification - Chapter 4.2.3.2 NAD) */
#define LINTRANSIF_FUNCTIONAL_NAD 0x7Eu /**< Functional NAD. (See also: LIN 2.2a Specification - Chapter 4.2.3.2 NAD) */
#define LINTRANSIF_BROADCAST_NAD  0x7Fu /**< Broadcast NAD. (See also: LIN 2.2a Specification - Chapter 4.2.3.2 NAD) */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/
typedef Lin_Error_t  LinTransIf_Error_t;        /**< Generic data types for TRANS layer error codes. */

typedef Lin_uint8_t  LinTransIf_NAD_t;          /**< Data type for the 'Node Address' (NAD)
                                                     (See also: LIN 2.2a Specification - Chapter 4.2.3.2 NAD) */
typedef Lin_uint8_t  LinTransIf_SID_t;          /**< Data type for SID (Service Identifier)
                                                     (See also: LIN 2.2a Specification - Chapter 3.2.1.5 SID) */

typedef Lin_pvoid_t      LinTransIf_pGenericEnvData_t;    /**< Generic pointer to environment data of the TRANS layer module. */
typedef Lin_EnvDataSze_t LinTransIf_EnvDataSze_t;         /**< LIN TRANS layer data type for the environment data length. */
typedef Lin_pvoid_t      LinTransIf_pGenericImpCfgData_t; /**< Generic pointer to configuration parameter of the specific TRANS layer implementation */
typedef Lin_cpvoid_t     LinTransIf_cpGenericImpCfgData_t; /**< Generic pointer to const configuration parameter of the specific TRANS layer implementation */
typedef Lin_pvoid_t      LinTransIf_pGenericCbCtxData_t;  /**< Pointer to TRANS callback context data. */

typedef Lin_uint8_t               LinTransIf_Data_t;       /**< Basic TRANS data type. */
typedef LinTransIf_Data_t*        LinTransIf_pData_t;      /**< Pointer to LinTransIf_Data_t. */
typedef const LinTransIf_Data_t*  LinTransIf_cpData_t;     /**< Pointer to constant LinTransIf_Data_t. */
typedef LinTransIf_Data_t**       LinTransIf_ppData_t;     /**< Pointer-Pointer to LinTransIf_Data_t. */
typedef const LinTransIf_Data_t** LinTransIf_cppData_t;     /**< Pointer-Pointer to const LinTransIf_Data_t. */

typedef Lin_BufLength_t          LinTransIf_BufLength_t;  /**< TRANS layer data type of length values */
typedef LinTransIf_BufLength_t*  LinTransIf_pBufLength_t; /**< Pointer to LinTransIf_BufLength_t */

typedef Lin_uint16_t             LinTransIf_PDUMsgLen_t;  /**< Typedef for the length of a PDU. */
typedef LinTransIf_PDUMsgLen_t*  LinTransIf_pPDUMsgLen_t; /**< Typedef of an pointer to the length of a PDU. */

typedef LinBusIf_Tick_t LinTransIf_Tick_t;                /**< Redefinition of the BUS Layer 'LinBusIf_Tick_t'. */

typedef Lin_uint32_t         LinTransIf_Timeout_t;        /**< Type for basic timeout values. */
typedef LinTransIf_Timeout_t LinTransIf_NasTimeout_t;     /**< Type for N_As timeout values. */
typedef LinTransIf_Timeout_t LinTransIf_NcrTimeout_t;     /**< Type for N_Cr timeout values. */

typedef Lin_uint8_t  LinTransIf_SIDProcessingHint_t;      /**< A user defined value which will be passed to the SID-Callbaack to indicate various calling condition (primarily used for manual triggered processing ) */

/* Transport layer internaly used processing hint: */
#define  LINTRANSIF_SID_PROC_HINT_TRANSPORT                   0u  /**< LIN driven processing (this one is issued to any non-manual processing callback call within the tranport layer) */
#define  LINTRANSIF_SID_PROC_HINT_GENERIC_MANUAL_TRIGGER      1u  /**< This indicats any not further specified manual trigger. */

/* Suggested user level processing hints: */ 
#define  LINTRANSIF_SID_PROC_HINT_UPDATE_INTERMEDIATE_RESP    2u  /**< Application may indicate with this hint value to the callback to update an intermediate response */
#define  LINTRANSIF_SID_PROC_HINT_MAKE_FINAL_RESPONSE         3u  /**< Application may indicate with this hint value to the callback to build the final response */
#define  LINTRANSIF_SID_PROC_HINT_CANCEL_RESPONSE             4u  /**< Application may indicate with this hint value to the callback to cancel any response */
#define  LINTRANSIF_SID_PROC_HINT_USER                        5u  /**< All other hint values up to 0xFF are free for user usage and likely may vary between callbacks  */
  
/***************************************************************************//**
 * @brief LIN TRANS layer error enumerator
 *
 * @misra{M3CM Dir-10.1. - PRQA Msg 4521,
 * The Elmos LIN Driver defines areas of error codes for every module and sub-areas between
 * general defined interface error codes and implementation specific error codes. To link
 * between this areas some arithmetic offset calculation has to be done between different
 * enum types.,
 * Conflicts in  signedness.,
 * Always make sure unsigned values ate defined and used.}
 *
 ******************************************************************************/
// PRQA S 4521 ++
enum LinTransIf_eErrorCodes
{
  LinTransIf_ERR_NO_ERROR                        = Lin_NO_ERROR,               /**< No error at all. */
  LinTransIf_ERR_INIT                            = Lin_ERROR_AREA_TRANS + 0u,  /**< Error at TRANS layer module initialization. */
  LinTransIf_ERR_MULTI_PDU_NOT_SUPPORTED         = Lin_ERROR_AREA_TRANS + 1u,  /**< Multi PDU request received, but not supported. */
  LinTransIf_ERR_INTERMEDIATE_RESP_NOT_SUPPORTED = Lin_ERROR_AREA_TRANS + 2u,  /**< Intermediate responses not supported. */
  LinTransIf_ERR_UNKNOWN_PCI_TYPE                = Lin_ERROR_AREA_TRANS + 3u,  /**< Unknown PCI type. */
  LinTransIf_ERR_UNEXPECTED_CF                   = Lin_ERROR_AREA_TRANS + 4u,  /**< Unexpected 'Continues Frame'. */
  LinTransIf_ERR_FRAME_CNT                       = Lin_ERROR_AREA_TRANS + 5u,  /**< Wrong frame count in Multi-PDU. */
  LinTransIf_ERR_BUFFER_TOO_SMALL                = Lin_ERROR_AREA_TRANS + 6u,  /**< Buffer to small to receive message. */
  LinTransIf_ERR_NAS_TIMEOUT                     = Lin_ERROR_AREA_TRANS + 7u,  /**< N_As timeout occurred. */
  LinTransIf_ERR_NCR_TIMEOUT                     = Lin_ERROR_AREA_TRANS + 8u,  /**< N_Cr timeout occurred. */
  LinTransIf_ERR_NO_RESPONSE_DATA                = Lin_ERROR_AREA_TRANS + 9u,  /**< SID callback indicates "WAITING FOR RESPONSE", but did not provide any response data */
  LinTransIf_ERR_NOT_ENOUGH_DATA                 = Lin_ERROR_AREA_TRANS + 10u, /**< Less data to send, then in 'Init Response' passed. */
  LinTransIf_ERR_TOO_MUCH_DATA                   = Lin_ERROR_AREA_TRANS + 11u, /**< More data to send, then in 'Init Response' passed. */
  LinTransIf_ERR_INVALID_SF_PDU                  = Lin_ERROR_AREA_TRANS + 12u, /**< Invalid single frame PDU. */
  LinTransIf_ERR_INVALID_FF_PDU                  = Lin_ERROR_AREA_TRANS + 13u, /**< Invalid first frame PDU. */
  LinTransIf_ERR_INVALID_SID_CB_RET_VAL          = Lin_ERROR_AREA_TRANS + 14u, /**< Invalid SID callback return value. */
  LinTransIf_ERR_TASK_TOO_SLOW                   = Lin_ERROR_AREA_TRANS + 15u, /**< Task invocation too slow. Some events may be missed. */
  LinTransIf_ERR_REQUEST_ABORTED                 = Lin_ERROR_AREA_TRANS + 16u, /**< Next request received while receiving or processing the previuos one  */
  LinTransIf_ERR_RESPONSE_ABORTED                = Lin_ERROR_AREA_TRANS + 17u, /**< Next request received while transmiting or processing the response of previuos one   */
  LinTransIf_ERR_WRONG_STATE                     = Lin_ERROR_AREA_TRANS + 18u, /**< Internal state error. i.e. can't init/add response data any more since the transport layer has changed the state ( i.e. the next request has arrived in the meanwhile and the buffer is needed there.) */
  LinTransIf_ERR_TRANSPORT_ERROR                 = Lin_ERROR_AREA_TRANS + 19u, /**< Transport layer has reported an error. Will be used in SID processing callbacs, once the actual error is obscured, due to postponed processing. */
  
  LinTransIf_ERR_IMPL_ERROR_AREA                 = Lin_ERROR_AREA_TRANS + (LIN_ERROR_AREA_SIZE/2u) /**< Any additional implementation specific error codes start here. */
};
// PRQA S 4521 --

typedef enum LinTransIf_eErrorCodes LinTransIf_eErrorCodes_t; /**< Typedef of LinTransIf_eErrorCodes. */

/***************************************************************************//**
 * @brief Enumerator to tell the TRANS layer module how to proceed with the SID.
 ******************************************************************************/
enum LinTransIf_eSIDReaction
{
  LinTransIf_SIDRA_IGNORED                    = 0x00,  /**< Ignore SID. */
  LinTransIf_SIDRA_SUCCESSFULLY_PROC          = 0x01,  /**< Successfully processed, no response*/
  LinTransIf_SIDRA_SEND_RESPONSE              = 0x02,  /**< Successfully processed with final response */
  LinTransIf_SIDRA_SEND_INTERMEDIATE_RESPONSE = 0x03,  /**< Successfully processed but only intermediate response available. */
  
  LinTransIf_SIDRA_NO_CALLBACK_FNC_FOUND      = 0xFF   /**< No callback found. (internal use) */
};

typedef enum LinTransIf_eSIDReaction LinTransIf_eSIDReaction_t; /**< Typedef of LinTransIf_eSIDReaction. */

/***************************************************************************//**
 * @brief Enumerator about how to proceed with a received NAD.
 ******************************************************************************/
enum LinTransIf_eCheckNADResult
{
  LinTransIf_ChkNADRes_Unknown = 0, /**< Unknown/Default return value. */
  LinTransIf_ChkNADRes_Accept  = 1, /**< Accept NAD. */
  LinTransIf_ChkNADRes_Decline = 2  /**< Decline NAD. */
};

typedef enum LinTransIf_eCheckNADResult LinTransIf_eCheckNADResult_t; /**< Typedef of LinTransIf_eCheckNADResult. */

/***************************************************************************//**
 * @brief TRANS layer communication status enumerator.
 ******************************************************************************/
enum LinTransIf_eComStatus
{
  LinTransIf_ComStatus_IDLE                       = 0, /**< Idle. */
  LinTransIf_ComStatus_RECEIVING_REQEUST          = 1, /**< Receiving a request. */
  LinTransIf_ComStatus_WAITING_FOR_RESPONSE       = 2, /**< Waiting for a response transmission. */
  LinTransIf_ComStatus_TRANSMITTING_RESPONSE      = 3, /**< Transmitting a response. */
  LinTransIf_ComStatus_INVALID                    = 4  /**< Invalid/Unknown status. */
};

typedef enum LinTransIf_eComStatus LinTransIf_eComStatus_t; /**< Typedef of LinTransIf_eComStatus. */

/***************************************************************************//**
 * @brief TRANS layer timeout type enumerator.
 ******************************************************************************/
enum LinTransIf_eTimeoutType
{
  LinTransIf_Timeout_NAS = 0, /**< N_As timeout. */
  LinTransIf_Timeout_NCR = 1, /**< N_Cr timeout. */
};

typedef enum LinTransIf_eTimeoutType LinTransIf_eTimeoutType_t; /**< Typedef of LinTransIf_eTimeoutType. */

/***************************************************************************//**
 * @brief Enumerator about who to proceed with a received NAD.
 ******************************************************************************/
enum LinTransIf_eSIDType
{
  LinTransIf_SIDType_INVALID  = 0, /**< Invalid/Unknown/Default return value. */
  LinTransIf_SIDType_REQUEST  = 1, /**< SID related to request processing. */
  LinTransIf_SIDType_RESPONSE = 2  /**< SID related to response processing. */
};

typedef enum LinTransIf_eSIDType LinTransIf_eSIDType_t; /**< Typedef of LinTransIf_eSIDType. */


struct         LinTransIf_sInterfaceFunctions;                                      /**< Forward declaration of LIN TRANS layer interface functions. */
typedef struct LinTransIf_sInterfaceFunctions    LinTransIf_sInterfaceFunctions_t;  /**< Typedef of LinTransIf_sInterfaceFunctions. */
typedef        LinTransIf_sInterfaceFunctions_t* LinTransIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinTransIf_sInterfaceFunctions. */
typedef const  LinTransIf_sInterfaceFunctions_t* LinTransIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinTransIf_sInterfaceFunctions. */

struct         LinTransIf_sResponseSubInterfaceFunctions;                                                 /**< Forward declaration of LIN TRANS layer response construction interface functions. */
typedef struct LinTransIf_sResponseSubInterfaceFunctions    LinTransIf_sResponseSubInterfaceFunctions_t;  /**< Typedef of LinTransIf_sResponseSubInterfaceFunctions. */
typedef        LinTransIf_sResponseSubInterfaceFunctions_t* LinTransIf_pResponseSubInterfaceFunctions_t;  /**< Typedef of pointer to LinTransIf_sResponseSubInterfaceFunctions. */
typedef const  LinTransIf_sResponseSubInterfaceFunctions_t* LinTransIf_cpResponseSubInterfaceFunctions_t; /**< Typedef of constant pointer to LinTransIf_sResponseSubInterfaceFunctions. */

struct         LinTransIf_sCallbackFunctions;                                     /**< Forward declaration of LIN TRANS layer callback functions. */
typedef struct LinTransIf_sCallbackFunctions    LinTransIf_sCallbackFunctions_t;  /**< Typedef of LinTransIf_sCallbackFunctions. */
typedef        LinTransIf_sCallbackFunctions_t* LinTransIf_pCallbackFunctions_t;  /**< Typedef of pointer to LinTransIf_sCallbackFunctions. */
typedef const  LinTransIf_sCallbackFunctions_t* LinTransIf_cpCallbackFunctions_t; /**< Typedef of constant pointer to LinTransIf_sCallbackFunctions. */

struct         LinTransIf_sThis;                        /**< Forward declaration of LIN TRANS layer This-Pointer. */
typedef struct LinTransIf_sThis    LinTransIf_sThis_t;  /**< Typedef for LinTransIf_sThis. */
typedef        LinTransIf_sThis_t* LinTransIf_pThis_t;  /**< Typedef of pointer to LinTransIf_sThis. */
typedef const  LinTransIf_sThis_t* LinTransIf_cpThis_t; /**< Typedef of const pointer to LinTransIf_sThis. */

struct         LinTransIf_sResponseSubInterfaceThis;                                            /**< Forward declaration of LIN TRANS layer This-Pointer. */
typedef struct LinTransIf_sResponseSubInterfaceThis    LinTransIf_sResponseSubInterfaceThis_t;  /**< Typedef for LinTransIf_sResponseSubInterfaceThis. */
typedef        LinTransIf_sResponseSubInterfaceThis_t* LinTransIf_pResponseSubInterfaceThis_t;  /**< Typedef of pointer to LinTransIf_sResponseSubInterfaceThis. */
typedef const  LinTransIf_sResponseSubInterfaceThis_t* LinTransIf_cpResponseSubInterfaceThis_t; /**< Typedef of pointer to LinTransIf_sResponseSubInterfaceThis. */

struct         LinTransIf_sInitParam;                            /**< Forward declaration of LIN TRANS layer initialization parameter struct. */
typedef struct LinTransIf_sInitParam    LinTransIf_sInitParam_t; /**< Typedef of LinTransIf_sInitParam */
typedef        LinTransIf_sInitParam_t* LinTransIf_pInitParam_t; /**< Typedef of pointer to LinTransIf_sInitParam */

typedef void * LinTransIf_SIDCbCtx_t; /**< Pointer to SID callback context data. */

struct         LinTransIf_sSIDDescription;                                  /**< Forward declaration of LIN TRANS layer 'SID Description' struct. */
typedef struct LinTransIf_sSIDDescription    LinTransIf_sSIDDescription_t;  /**< Typedef of LinTransIf_sSIDDescription. */
typedef        LinTransIf_sSIDDescription_t* LinTransIf_pSIDDescription_t;  /**< Typedef of pointer to LinTransIf_sSIDDescription */
typedef const  LinTransIf_sSIDDescription_t* LinTransIf_cpSIDDescription_t; /**< Typedef of constant pointer to LinTransIf_sSIDDescription. */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINTRANS_TYPES_H_ */

/*! @} */ /* TRANS */
/*! @} */ /* ELMOS_LIN_DRIVER */
