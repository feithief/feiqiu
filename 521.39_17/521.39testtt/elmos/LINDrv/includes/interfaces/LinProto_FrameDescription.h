/***************************************************************************//**
 * @file		LinProto_FrameDescription.h
 *
 * @creator		sbai
 * @created		27.01.2015
 *
 * @brief  		Definition of the 'Frame Description' data types for the
 *            PROTO Layer.
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

#ifndef LINPROTO_FRAMEDESCRIPTION_H_
#define LINPROTO_FRAMEDESCRIPTION_H_

/* PRQA S 1536 EOF #justification: This header file is a common file for many projects.
   So it is ok that some types tags and macros are defined that are not used in this project scope. */
/* Rule: 1536 MISRA a Rule-2.4
   Circumstances: The tag '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */
/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinProto_Types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/*                                Enumerator                                  */
/* ****************************************************************************/

/***************************************************************************//**
 * @brief LIN PROTO layer frame types of an entry in a 'Frame Description'.
 ******************************************************************************/
 enum LinProtoIf_eFrameKind
 {
   LinProtoIf_FrameKind_INVALID                 = 0,  /**< Invalid frame kind    */
   #if LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1
   LinProtoIf_FrameKind_PUBLISH_WITH_SGNL_LST   = 1,  /**< Published LIN frame defined with a 'Signal List'. @see LinProtoIf_sSignal  */
   LinProtoIf_FrameKind_SUBSCRIBE_WITH_SGNL_LST = 2,  /**< Subscribed LIN frame defined with a 'Signal List'. @see LinProtoIf_sSignal */
   #endif /* LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1 */
   LinProtoIf_FrameKind_PUBLISH_WITH_FUN        = 3,  /**< Published LIN frame build with a function.  */
   LinProtoIf_FrameKind_SUBSCRIBE_WITH_FUN      = 4,  /**< Subscribed LIN frame build with a function, */
   LinProtoIf_FrameKind_NON_BUFFERED            = 5   /**< Published/subscribed LIN frame build with a function which resembles bus layer interface with variable buffer pointer.  */
 };

 /***************************************************************************//**
  * @brief LIN PROTO 'Frame ID processed' callback handling type.
  ******************************************************************************/
enum LinProtoIf_eFrameIdProcCbHandling
{
  LinProtoIf_TxRxCbProc_DIRECT    = 0,  /**< The 'Frame ID processed' callback will be called directly
                                             after the 'Frame ID' has been received.*/
  LinProtoIf_TxRxCbProc_SCHEDULED = 1   /**< The 'Frame ID processed' callback will be called out of
                                             the PROTO Task. */
};

/***************************************************************************//**
 * @brief Defines the callback context data which shall be passed to the
 *        'Frame ID processed' callback.
 ******************************************************************************/
enum LinProtoIf_eFrameIdProcCbCtxDataType
{
  LinProtoIf_FrmIdPrcCbCtxDataType_NONE         = 0, /**< None (LIN_NULL), will be passed to the callback function. */
  LinProtoIf_FrmIdPrcCbCtxDataType_PER_CALLBACK = 1, /**< The callback context attached to the 'Frame Description'
                                                          will be passed to the callback function. @see LinProtoIf_sFrameDescription */
  LinProtoIf_FrmIdPrcCbCtxDataType_PER_DESCLST  = 2  /**< The callback context attached to the 'Frame Description List'
                                                          will be passed to the callback function. @see LinProtoIf_AddFrameDescLstIfFun_t */
};

/* ****************************************************************************/
/*                           Types for enumerator                             */
/* ****************************************************************************/
typedef enum LinProtoIf_eFrameKind                LinProtoIf_eFrameKind_t;                /**< Typedef for LinProtoIf_eFrameKind. */
typedef enum LinProtoIf_eFrameIdProcCbHandling    LinProtoIf_eFrameIdProcCbHandling_t;    /**< Typedef for LinProtoIf_eFrameIdProcCbHandling. */
typedef enum LinProtoIf_eFrameIdProcCbCtxDataType LinProtoIf_eFrameIdProcCbCtxDataType_t; /**< Typedef for LinProtoIf_eFrameIdProcCbCtxDataType. */

/* ****************************************************************************/
/*               Types for structs and pointer to structs                     */
/* ****************************************************************************/
typedef struct LinProtoIf_sFrameDescription     LinProtoIf_sFrameDescription_t;   /**< Typedef for LinProtoIf_sFrameDescription. */
typedef        LinProtoIf_sFrameDescription_t*  LinProtoIf_pFrameDescription_t;   /**< Typedef for pointer to LinProtoIf_sFrameDescription. */
typedef        LinProtoIf_sFrameDescription_t** LinProtoIf_ppFrameDescription_t;  /**< Typedef for pointer-pointer to LinProtoIf_sFrameDescription. */
typedef const  LinProtoIf_sFrameDescription_t*  LinProtoIf_cpFrameDescription_t;  /**< Typedef for constant pointer to LinProtoIf_sFrameDescription. */
typedef const  LinProtoIf_sFrameDescription_t** LinProtoIf_cppFrameDescription_t; /**< Typedef for constant pointer-pointer to LinProtoIf_sFrameDescription. */

/* ****************************************************************************/
/*               Callback function pointer definitions                        */
/* ****************************************************************************/

/***************************************************************************//**
 * @brief Callback function to build a complete LIN Frame by the user.
 *
 * @param genericProtoEnvData[in]   Pointer to reserved PROTO Layer environment data.
 * @param frameID[in]               'Frame ID' of the LIN frame to build.
 * @param frameDesc[in]             Pointer to 'Frame Description' associated to
 *                                  the received Frame ID (@p frameID), if available.
 * @param msgBuf[in,out]            Pointer to the buffer to write data to
 *                                  (published) or read from (subscribed).
 * @param msgBufLen[in]             Length of the buffer.
 * @param genericProtoCbCtxData[in] Pointer to PROTO Layer callback context data.
 *
 * @return How the PROTO Layer should proceed after the reception/transmission
 *         of the frame. (@ref LinBusIf_eFrameIdAction)
 *
 * This function type is called if the LIN frame in the
 * 'Frame Description List' is configured to be completely build by the
 * user application or an higher layer.
 *
 * @par "Build Frame Function:"
 * @mscfile msc_proto_buildframecbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinBusIf_eFrameIdAction_t (*LinProtoIf_pBuildFrameFun_t) (LinProtoIf_pGenericEnvData_t    genericProtoEnvData, LinBusIf_FrameID_t             frameID,
                                                                  LinProtoIf_cpFrameDescription_t frameDesc,           LinProtoIf_pData_t             msgBuf,
                                                                  LinProtoIf_BufLength_t          msgBufLen,           LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Callback function to pass raw buffer to the bus layer (frame kind NON_BUFFERED).
 *
 * @param genericProtoEnvData[in]   Pointer to reserved PROTO Layer environment data.
 * @param frameID[in]               'Frame ID' of the LIN frame to build.
 * @param frameDesc[in]             Pointer to 'Frame Description' associated to
 *                                  the received Frame ID (@p frameID), if available.
 * @param msgBufPtr[in,out]         Pointer to location where to store the buffer
 *                                  pointer to write data to (published) or read
 *                                  from (subscribed).
 * @param msgBufLenPtr[in,out]      Pointer to the length field of the buffer.
 * @param genericProtoCbCtxData[in] Pointer to PROTO Layer callback context data.
 *
 * @return How the PROTO Layer should proceed after the reception/transmission
 *         of the frame. (LinBusIf_eFrameIdAction)
 *
 * This function type is called if the LIN frame in the
 * 'Frame Description List' is configured to be completely build in a
 * non-buffered way by the user application or an higher layer.
 *
 * @see LinProtoIf_eFrameKind
 *
 ******************************************************************************/
typedef LinBusIf_eFrameIdAction_t (*LinProtoIf_pBuildNonBufferedFrameFun_t) (LinProtoIf_pGenericEnvData_t    genericProtoEnvData, LinBusIf_FrameID_t             frameID,
                                                                             LinProtoIf_cpFrameDescription_t frameDesc,           LinProtoIf_ppBusData_t         msgBufPtr,
                                                                             LinProtoIf_pBusBufLength_t      msgBufLenPtr,        LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Typedef of PROTO layer 'Frame ID Processed' callback function.
 *
 * @param genericProtoEnvData[in]   Pointer to reserved PROTO Layer environment data.
 * @param protoIfFuns[in]           Pointer to the PROTO Layer interface function struct.
 * @param frameID[in]               Processed frame ID.
 * @param frameDesc[in]             Pointer to 'Frame Description' associated to
 *                                  the received Frame ID (@p frameID), if available.
 * @param error[in]                 'LinProtoIf_ERR_NO_ERROR' if the frame has
 *                                  been processed successfully, otherwise the
 *                                  occurred error.
 * @param genericProtoCbCtxData[in] Pointer to PROTO Layer callback context data.
 *
 * @return The behavior of the PROTO Layer after the processing of the frame. Set
 *         the PROTO Layer to Sleep-Mode or continue normal operation.
 *         (@ref LinProtoIf_eMsgAction)
 *
 * This callback is called after 'Frame ID' has been processed. This means, the
 * LIN frame has been received, transmitted or ignored.
 *
 ******************************************************************************/
typedef LinProtoIf_eMsgAction_t (*LinProtoIf_FrameIdProcCbFun_t) (LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                                                  LinBusIf_FrameID_t             frameID,             LinProtoIf_cpFrameDescription_t   frameDesc,
                                                                  LinProtoIf_Error_t             error,               LinProtoIf_pGenericCbCtxData_t    genericProtoCbCtxData);


/***************************************************************************//**
 * @brief LIN PROTO layer 'Frame Description' struct.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This struct is part of an interface and can be used outside of this project scope. It should not be hidden. */
struct LinProtoIf_sFrameDescription
{
    LinBusIf_FrameID_t                     DefaultFrameID;               /**< Default 'Frame ID' */
    LinProtoIf_eCrcType_t                  CrcType;                      /**< LIN CRC calculation type. (@ref LinProtoIf_eCrcType) */
    LinProtoIf_eFrameKind_t                Kind;                         /**< Published or subscribed frame. (@ref LinProtoIf_eFrameKind) */
    LinProtoIf_MsgID_t                     MsgID;                        /**< LIN Message ID, if LIN Spec <1.3 is used. (@ref LinProtoIf_MsgID_t) */
    LinProtoIf_FrameLenght_t               Lenght;                       /**< Length of the LIN frame. */
    #if LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1
    LinProtoIf_pGenericSignalList_t        PtrToSgnlLst;                 /**< Pointer to 'Signal List' which describes the frame structure.*/
    #else
    LinProtoIf_pGenericFun_t               PtrToFun;                     /**< Pointer to function to build or receive a LIN frame. */
    #endif
    LinProtoIf_eFrameIdProcCbHandling_t    FrameIdProcCbHandling;        /**< Should the 'Processed Callback' be called from the PROTO Layer task or directly out of the IRQ. (@ref LinProtoIf_eFrameIdProcCbHandling)*/
    LinProtoIf_FrameIdProcCbFun_t          FrameIdProcessedCallbackFnc;  /**< Pointer to the 'Frame ID Processed' callback function. */
    LinProtoIf_eFrameIdProcCbCtxDataType_t FrameIdProcessedCbCtxType;    /**< Type of the 'Callback Context Data'. (@ref LinProtoIf_eFrameIdProcCbCtxDataType) */
    LinProtoIf_pGenericCbCtxData_t         FrameIdProcessedCallbackCtx;  /**< Pointer to the callback context data. */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINPROTO_FRAMEDESCRIPTION_H_ */

/*! @} */ /* PROTO */
/*! @} */ /* ELMOS_LIN_DRIVER */
