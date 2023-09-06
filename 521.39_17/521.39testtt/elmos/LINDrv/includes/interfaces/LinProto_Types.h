/***************************************************************************//**
 * @file			LinProto_Types.h
 *
 * @creator		sbai
 * @created		24.03.2015
 *
 * @brief  		Definitions of basic data types for the 'LIN PROTO Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup PROTO PROTO Layer
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

#ifndef LINPROTO_TYPES_H_
#define LINPROTO_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinBus_Types.h"
#include "LinLookup_Types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINPROTO_MAX_FRAME_LENGHT 0xffu /**< Absolute maximun frame length, actuall implementation may restrict frame length further */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/*                            Simple data types                               */
/* ****************************************************************************/
typedef LinBusIf_Baudrate_t LinProtoIf_Baudrate_t;   /**< @copydoc LinBusIf_Baudrate_t */
typedef LinBusIf_BreakLen_t LinProtoIf_BreakLen_t;   /**< @copydoc LinBusIf_BreakLen_t */
typedef Lin_Error_t LinProtoIf_Error_t;              /**< Typedef for PROTO layer error codes. */

typedef LinBusIf_eCrcType_t LinProtoIf_eCrcType_t;      /**< @copybrief LinBusIf_eCrcType */
typedef LinBusIf_pCrcType_t LinProtoIf_pCrcType_t;      /**< @copybrief LinBusIf_eCrcType */
#define LinProtoIf_CRC_Classic   LinBusIf_CRC_Classic   /**< @copydoc LinBusIf_CRC_Extended */
#define LinProtoIf_CRC_Extended  LinBusIf_CRC_Extended  /**< @copydoc LinBusIf_CRC_Classic */

typedef Lin_uint16_t LinProtoIf_MsgID_t;              /**< LIN frame ID lookup message ID data type */
#if LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1
typedef Lin_pvoid_t  LinProtoIf_pGenericSignalList_t; /**< Generic pointer to a 'Signal List' */
#else
typedef Lin_pvoid_t  LinProtoIf_pGenericFun_t;        /**< Generic pointer to a function to build or receiver a LIN frame. */
#endif /* LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1 */
typedef Lin_uint8_t  LinProtoIf_FrameLenght_t;        /**< Typedef for the LIN frame length. */

typedef Lin_pvoid_t      LinProtoIf_pGenericEnvData_t;               /**< Generic pointer to environment data of the PROTO layer module. */
typedef Lin_EnvDataSze_t LinProtoIf_EnvDataSze_t;                    /**< LIN Protocol layer data type for the environment data length. */
typedef Lin_pvoid_t      LinProtoIf_pGenericImpCfgData_t;            /**< Generic pointer to configuration parameter of the specific PROTO layer implementation. */
typedef Lin_pvoid_t      LinProtoIf_pGenericCbCtxData_t;             /**< Pointer to PROTO callback context data. */
typedef Lin_pvoid_t      LinProtoIf_pGenericFrameDescriptionList_t;  /**< Generic pointer to a PROTO layer 'Frame Description List'. */
typedef Lin_cpvoid_t     LinProtoIf_cpGenericFrameDescriptionList_t; /**< Generic constant pointer to a PROTO layer 'Frame Description List'. */
typedef Lin_pvoid_t      LinProtoIf_pGenericFrmDescLstEnvData_t;     /**< Generic pointer to environment data a PROTO layer 'Frame Description List'. */

typedef Lin_uint8_t               LinProtoIf_Data_t;   /**< Basic PROTO data type */
typedef LinProtoIf_Data_t*        LinProtoIf_pData_t;  /**< Pointer to LinProtoIf_Data_t */
typedef const LinProtoIf_Data_t*  LinProtoIf_cpData_t;  /**< Pointer to const LinProtoIf_Data_t */
typedef LinProtoIf_Data_t**       LinProtoIf_ppData_t; /**< Pointer-Pointer to LinProtoIf_Data_t */

typedef Lin_BufLength_t         LinProtoIf_BufLength_t;   /**< PROTO layer data type of buffer length values */
typedef LinProtoIf_BufLength_t* LinProtoIf_pBufLength_t;  /**< Pointer to LinProtoIf_BufLength_t */

typedef LinBusIf_Data_t       LinProtoIf_BusData_t;     /**< Local forward for Bus-Layer data type */
typedef LinBusIf_Data_t*      LinProtoIf_pBusData_t;    /**< Pointer to LinProtoIf_BusData_t */
typedef LinBusIf_Data_t**     LinProtoIf_ppBusData_t;   /**< Pointer-Pointer to LinProtoIf_BusData_t */

typedef LinBusIf_BufLength_t  LinProtoIf_BusBufLength_t;   /**< local forward of bus layer length values */
typedef LinBusIf_BufLength_t* LinProtoIf_pBusBufLength_t;  /**< Pointer to LinProtoIf_BusLength_t */

/***************************************************************************//**
 * @brief LIN PROTO layer error enumerator
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
enum LinProtoIf_eErrorCodes                                                            
{
  LinProtoIf_ERR_NO_ERROR                    = Lin_NO_ERROR,             /**< No error at all. */

  LinProtoIf_ERR_INIT                        = Lin_ERROR_AREA_PROTO + 0u, /**< Initialization error */
  
  LinProtoIf_ERR_FRM_DEF                     = Lin_ERROR_AREA_PROTO + 1u, /**< Error in frame definition table */
  LinProtoIf_ERR_SGN_DEF                     = Lin_ERROR_AREA_PROTO + 2u, /**< Error in signal definition table */
  LinProtoIf_ERR_TSK_TO_SLOW                 = Lin_ERROR_AREA_PROTO + 3u, /**< Task is called to slow for callback processing */
  LinProtoIf_ERR_SGN_COPY                    = Lin_ERROR_AREA_PROTO + 4u, /**< Error while coping signal data. */

  LinProtoIf_ERR_IMPL_ERROR_AREA             = Lin_ERROR_AREA_PROTO + (LIN_ERROR_AREA_SIZE/2u) /**< Any additional implementation specific error codes start here. */
    
};
// PRQA S 4521 --

typedef enum LinProtoIf_eErrorCodes LinProtoIf_eErrorCodes_t; /**< Typedef of LinProtoIf_eErrorCodes. */

/***************************************************************************//**
 * @brief An enumeration indicating how to proceed with a measured baudrate value
 ******************************************************************************/
enum  LinProtoIf_eMeasDoneRetVal
{
  LinProtoIf_MeasDoneRetVal_DISMISS           = 0,    /**< The measured baudrate should be ignored. Continue with the previous value. */
  LinProtoIf_MeasDoneRetVal_RESET             = 1,    /**< The measured baudrate should be ignored. Continue with the expected (nominal) baudrate value. */
  LinProtoIf_MeasDoneRetVal_UPDATE            = 2,    /**< The measured baudrate should be used as the new default baud rate value. */
  LinProtoIf_MeasDoneRetVal_UPDATE_NOMINAL    = 3,    /**< The measured baudrate should be used as the new nominal baudratre (recals timings, higher comp. efford). */
  /* not used in this project 
   *LinProtoIf_MeasDoneRetVal_ABORT    = 4, */ /**< The measured baudrate is too much off, stop further processing, and wait for next break. */
  LinProtoIf_MeasDoneRetVal_DOESNT_CARE       = 5     /**< The application or higher layer doesn't care about baudrate measurement */
};

typedef enum LinProtoIf_eMeasDoneRetVal LinProtoIf_eMeasDoneRetVal_t; /**< Typedef for LinProtoIf_eMeasDoneRetVal. */

typedef LinBusIf_eMsgAction_t LinProtoIf_eMsgAction_t;    /**< Redefinition of LinBusIf_eMsgAction_t. @ref LinBusIf_eMsgAction */
typedef LinBusIf_eIdleAction_t LinProtoIf_eIdleAction_t;  /**< Redefinition of LinBusIf_eIdleAction_t. @ref LinBusIf_eIdleAction  */

struct         LinProtoIf_sInterfaceFunctions;                                      /**< Forward declaration of LIN PROTO layer interface functions. */
typedef struct LinProtoIf_sInterfaceFunctions    LinProtoIf_sInterfaceFunctions_t;  /**< Typedef for LinProtoIf_sInterfaceFunctions. */
typedef        LinProtoIf_sInterfaceFunctions_t* LinProtoIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinProtoIf_sInterfaceFunctions. */
typedef const  LinProtoIf_sInterfaceFunctions_t* LinProtoIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinProtoIf_sInterfaceFunctions. */

struct         LinProtoIf_sCallbackFunctions;                                       /**< Forward declaration of LIN PROTO layer callback functions. */
typedef struct LinProtoIf_sCallbackFunctions     LinProtoIf_sCallbackFunctions_t;   /**< Typedef for LinProtoIf_sCallbackFunctions. */
typedef        LinProtoIf_sCallbackFunctions_t*  LinProtoIf_pCallbackFunctions_t;   /**< Typedef of pointer to LinProtoIf_sCallbackFunctions. */
typedef const  LinProtoIf_sCallbackFunctions_t*  LinProtoIf_cpCallbackFunctions_t;  /**< Typedef of constant pointer to LinProtoIf_sCallbackFunctions. */

struct         LinProtoIf_sThis;                       /**< Forward declaration of LIN PROTO layer This-Pointer. */
typedef struct LinProtoIf_sThis    LinProtoIf_sThis_t; /**< Typedef for LinProtoIf_sThis. */
typedef        LinProtoIf_sThis_t* LinProtoIf_pThis_t; /**< Typedef of pointer to LinProtoIf_sThis. */

struct         LinProtoIf_sInitParam;                            /**< Forward declaration of LIN PROTO layer initialization parameter struct. */
typedef struct LinProtoIf_sInitParam    LinProtoIf_sInitParam_t; /**< Typedef for LinProtoIf_sInitParam */
typedef        LinProtoIf_sInitParam_t* LinProtoIf_pInitParam_t; /**< Typedef of pointer to LinProtoIf_sInitParam */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINPROTO_TYPES_H_ */

/*! @} */ /* PROTO */
/*! @} */ /* ELMOS_LIN_DRIVER */
