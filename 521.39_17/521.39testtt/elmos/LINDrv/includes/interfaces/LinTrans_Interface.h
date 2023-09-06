/***************************************************************************//**
 * @file			LinTrans_Interface.h
 *
 * @creator		sbai
 * @created		13.02.2015
 *
 * @brief  		Definition of the interface for the 'TRANS Layer'.
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

#ifndef LINTRANS_INTERFACE_H_
#define LINTRANS_INTERFACE_H_

/* PRQA S 1535 EOF #justification: This header file is a common file for many projects.
   So it is ok that some types are defined that are not used in this project scope. */
/* Rule: 1535 MISRA a Rule-2.3
   Circumstances: The typedef '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */
/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinProto_FrameDescription.h"
#if LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1
#include "LinProto_Signal.h"
#endif
#include "LinTrans_Types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINTRANS_INTERFACE_MODULE_API_VERSION         0x0106u /**< LIN TRANS interface version */

/***************************************************************************//**
 * @brief Macro for adding a service to 'SID List'.
 ******************************************************************************/
#define LINTRANS_INTERFACE_ADD_SERVICE(SERVICEID,         \
                                       CB_FUN,      \
                                       CB_CTX_TYPE, \
                                       CB_CTX)      \
                                       \
                                       {.StartSID        = (SERVICEID),   \
                                        .EndSID          = (SERVICEID),   \
                                        .CallbackFnc     = (CB_FUN),      \
                                        .CallbackCtxType = (CB_CTX_TYPE), \
                                        .CallbackCtx     = (CB_CTX)}      \

/***************************************************************************//**
 * @brief Macro for adding a service to 'SID List'.
 ******************************************************************************/
#define LINTRANS_INTERFACE_ADD_SERVICE_RANGE(SERVICEID_START,         \
                                             SERVICEID_END,         \
                                             CB_FUN,      \
                                             CB_CTX_TYPE, \
                                             CB_CTX)      \
                                             \
                                             {.StartSID        = (SERVICEID_START),   \
                                              .EndSID          = (SERVICEID_END),   \
                                              .CallbackFnc     = (CB_FUN),      \
                                              .CallbackCtxType = (CB_CTX_TYPE), \
                                              .CallbackCtx     = (CB_CTX)}      \



/***************************************************************************//**
 * @brief Macro to mark the end of a 'SID List'.
 ******************************************************************************/
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINTRANS_INTERFACE_ADD_SERVICE_LIST_END  {.CallbackFnc =  LIN_NULL}

#define LINTRANS_MAX_MSG_DATA_SIZE 4094u  /**< The absolute maximum number of data byte in a transport layer request (excl. SID). Actual implementation may limit the available message zize further due to limited buffers size */

#define LINTRANS_DEFAULT_PROTO_FRAME_SIZE 8u                                                        /**< The default LIN frame size used for standard conform PDU based communication */ 
#define LINTRANS_DEFAULT_INTERMEDIATE_RESPONSE_DATA_SIZE (LINTRANS_DEFAULT_PROTO_FRAME_SIZE - 3u)   /**< The default intermediate response data size (typically number of data bytes fitting into a SF PDU */ 


/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief Defines the callback context data which shall be passed to the
 *        'SID callback' callback.
 ******************************************************************************/
enum LinTransIf_eSIDDescLstCbCtxType
{
  LinTransIf_SIDDescLstCbCtxType_PER_SID  = 0, /**< The callback context of the SID description in the 'SID List'
                                                    will be passed to the callback function. @see LinTransIf_sSIDDescription */
  LinTransIf_SIDDescLstCbCtxType_PER_LST  = 1  /**< The callback context attached to the 'SID List'
                                                    will be passed to the callback function.*/
};

typedef enum LinTransIf_eSIDDescLstCbCtxType LinTransIf_eSIDDescLstCbCtxType_t; /**< Typedef of LinTransIf_eSIDDescLstCbCtxType */

/***************************************************************************//**
 * @brief Configuration data for the TRANS Layer to setup a
 *        'Frame ID Descriptions' for an 'Request-/Response-Channel' in
 *        PROTO layer.
 ******************************************************************************/
struct LinTransIf_sProtoFrameInfo
{
  LinBusIf_FrameID_t       MsgID;   /**< 'Message ID' of the TRANS Layer channel. */
  LinProtoIf_FrameLenght_t Length;  /**< Length of the LIN Frame used by the TRANS module layer. */
  LinProtoIf_eCrcType_t    CrcType; /**< CRC calculation type of the TRANS Layer channel. */
};

typedef struct LinTransIf_sProtoFrameInfo     LinTransIf_sProtoFrameInfo_t;   /**< Typedef of LinTransIf_sProtoFrameInfo */
typedef        LinTransIf_sProtoFrameInfo_t * LinTransIf_pProtoFrameInfo_t;   /**< Typedef of pointer to LinTransIf_sProtoFrameInfo. */
typedef const  LinTransIf_sProtoFrameInfo_t * LinTransIf_cpProtoFrameInfo_t;  /**< Typedef of constant pointer to LinTransIf_sProtoFrameInfo. */

/***************************************************************************//**
 * @brief Complete configuration of the TRANS Layer to setup
 *        the PROTO layer module.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This struct is part of an interface that can be used outside of this project scope. It should not be hidden. */
struct LinTransIf_sProtoFrameDescriptionInfo
{
  LinTransIf_sProtoFrameInfo_t            ReqFrameInfo;             /**< Request-Channel configuration. */
  LinTransIf_sProtoFrameInfo_t            RespFrameInfo;            /**< Response-Channel configuration. */
  
  LinProtoIf_pGenericFrmDescLstEnvData_t  FrameDescListEnvData;     /**< Environment data per 'Frame Description List'. */
  LinProtoIf_EnvDataSze_t                 FrameDescListEnvDataSze;  /**< Size of reserved environment data per 'Frame Description List'.  */

  Lin_Bool_t                              LdfRelevance;             /**< LIN_TRUE if the Response-/Request-Channel can be assigned/unassigned. */
};

typedef struct LinTransIf_sProtoFrameDescriptionInfo     LinTransIf_sProtoFrameDescriptionInfo_t;   /**< Typedef of LinTransIf_sProtoFrameDescriptionInfo */
typedef        LinTransIf_sProtoFrameDescriptionInfo_t * LinTransIf_pProtoFrameDescriptionInfo_t;   /**< Typedef of pointer to LinTransIf_sProtoFrameDescriptionInfo. */
typedef const  LinTransIf_sProtoFrameDescriptionInfo_t * LinTransIf_cpProtoFrameDescriptionInfo_t;  /**< Typedef of constant pointer to LinTransIf_sProtoFrameDescriptionInfo. */


/***************************************************************************//**
 * @brief SID processing parameter collection.
 ******************************************************************************/
struct LinTransIf_sSIDCbServiceData
{
  LinTransIf_Error_t             ErrorCode;    /**< Error code, if any*/

  LinTransIf_SIDProcessingHint_t hint;         /**< Processing hint*/
  
  LinTransIf_NAD_t               NAD;          /**< NAD address this request/response has been adressed to */
  
  LinTransIf_eSIDType_t          SIDType;      /**< The service type this processing is intended for (request/response)*/
  LinTransIf_SID_t               SID;          /**< Service ID*/
  
  LinTransIf_pData_t             DataBuffer;   /**< Data beeing passed allong the request (may be NULL i.e. for responses and manually triggered requests)*/
  LinTransIf_BufLength_t         DataLen;      /**< Number of bytes in the data buffer*/
  
};

typedef struct LinTransIf_sSIDCbServiceData     LinTransIf_sSIDCbServiceData_t;   /**< Typedef of LinTransIf_sSIDCbServiceData */
typedef        LinTransIf_sSIDCbServiceData_t * LinTransIf_pSIDCbServiceData_t;   /**< Typedef of pointer to LinTransIf_sSIDCbServiceData. */
typedef const  LinTransIf_sSIDCbServiceData_t * LinTransIf_cpSIDCbServiceData_t;  /**< Typedef of constant pointer to LinTransIf_sSIDCbServiceData. */

/***************************************************************************//**
 * @brief Configuration struct for setting up behavior and functionalities of Transport Layer
 ******************************************************************************/
typedef struct
{
  Lin_uint16_t AbortProc         :1;                   /**< If set to (1) the frame processing is abortet when new diag-frame is received 
                                                         (see lin2.2-conformance-test 13.2.1) - otherwise (0) answer will be transmitted */  
} LinTransIf_sCfgDataFlags_t;

/* *****************************************************************************
 * TRANS Layer callback function types.
 ******************************************************************************/

/** @addtogroup LinTransIfCbFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'SID' callback function.
 *
 * @param[in] thisTransIf           Pointer to TRANS Layer interface and environment data.
 * @param[in] thisRespIf            Pointer to response construction sub interface.
 * @param[in] serviceData           Structure containing all processing relevant data (NAD, SID, Error codes ...)
 * @param[in] genericTransCbCtxData Pointer to TRANS Layer callback context data.
 *
 * @return How the TRANS Layer should react to this service request.
 *         (@ref LinTransIf_eSIDReaction)
 *
 * This callback is called on reception of a SID.
 *
 ******************************************************************************/
typedef LinTransIf_eSIDReaction_t (*LinTransIf_SIDCbFun_t) (LinTransIf_cpThis_t                     thisTransIf,
                                                            LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                            LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                            LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Error' callback function.
 *
 * @param[in] thisTransIf           Pointer to TRANS Layer interface and environment data.
 * @param[in] error                 Occurred error.
 * @param[in] nad                   Related NAD address, if any
 * @param[in] SIDType               Related service type, if any 
 * @param[in] sid                   Related Service ID, if any
 * @param[in] genericTransCbCtxData Pointer to TRANS Layer callback context
 *                                  data.
 *
 * Indicates an error in the TRANS Layer and underlying layers of the LIN
 * Driver.
 *
 * @par "Call Description:"
 * @mscfile msc_trans_errorcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinTransIf_ErrorCbFun_t) (LinTransIf_cpThis_t            thisTransIf,
                                         LinTransIf_Error_t             error,          
                                         LinTransIf_NAD_t               nad,
                                         LinTransIf_eSIDType_t          SIDType, LinTransIf_SID_t                  sid,
                                         LinTransIf_pGenericCbCtxData_t genericTransCbCtxData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'SID Processed' callback function.
 *
 * @param[in] thisTransIf           Pointer to TRANS Layer interface and environment data.
 * @param[in] nad                   LIN Transport Layer Node Address (See also:
 *                                  LIN 2.2a Specification - Chapter 4.2.3.2 NAD)
 * @param[in] SIDType               Type of processing this callback ist called for (req/resp)
 * @param[in] sid                   Related Service ID.
 * @param[in] reaction              Return of previously called dedicated processing callback, if any 
 * @param[in] errorCode             Error code related to the SID processing 
 * @param[in] genericTransCbCtxData Pointer to TRANS Layer callback context
 *                                  data.
 *
 * This callback is called after a 'Service ID' has been processed.
 *
 * @par "Call Description:"
 * @mscfile msc_trans_sidprocessedcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinTransIf_SIDProcessedCbFun_t) (LinTransIf_cpThis_t            thisTransIf,
                                                LinTransIf_NAD_t               nad,
                                                LinTransIf_eSIDType_t          SIDType,
                                                LinTransIf_SID_t               sid, 
                                                LinTransIf_eSIDReaction_t      reaction, 
                                                LinTransIf_Error_t             errorCode,
                                                LinTransIf_pGenericCbCtxData_t genericTransCbCtxData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Check NAD' callback function.
 *
 * @param[in] thisTransIf           Pointer to TRANS Layer interface and environment data.
 * @param[in] nad                   Node address to check.
 * @param[in] SIDType               Related Service type.
 * @param[in] sid                   Related Service ID.
 * @param[in] sidDesc               Related Service Description this check is performed for
 * @param[in] genericTransCbCtxData Pointer to TRANS Layer callback context
 *                                  data if called via global callback. 
 *                                  Or pointer to SID description list callback context if called 
 *                                  as a per-SIDList NAD check.
 *
 * @return If the NAD should be accepted or declined.
 *
 * Callback function to let the user/higher layer decided, if the NAD
 * should be accepted or not.
 * This callback may be issued multiple times for subsequent matching service descriptions, if previous 
 * processing returned "LinTransIf_SIDRA_IGNORED"
 * Per-SID-List check callbacks have precedence over global checks. 
 *
 * @par "Call Description:"
 * @mscfile msc_trans_checknadcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinTransIf_eCheckNADResult_t (*LinTransIf_CheckNADCbFun_t) (LinTransIf_cpThis_t           thisTransIf,
                                                                    LinTransIf_NAD_t              nad,                 
                                                                    LinTransIf_eSIDType_t         SIDType,
                                                                    LinTransIf_SID_t              sid,
                                                                    LinTransIf_cpSIDDescription_t sidDesc,
																	LinTransIf_pGenericCbCtxData_t genericTransCbCtxData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Go To Sleep' callback function.
 *
 * @param[in] thisTransIf           Pointer to TRANS Layer interface and environment data.
 * @param[in] genericTransCbCtxData Pointer to TRANS Layer callback context
 *                                  data.
 *
 * This callback is called if the TRANS Layer receives a 'Go to sleep'
 * command PDU.
 *
 * @par "Call Description:"
 * @mscfile msc_trans_gotosleepcbcalled.dox
 * @n
 *
 *
 ******************************************************************************/
typedef void (*LinTransIf_GoToSleepCbFun_t) (LinTransIf_cpThis_t                     thisTransIf,
                                             LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);


/***************************************************************************//**
 * @brief TRANS Layer callback functions struct.
 *
 * The set of callbacks which should be invoked in the higher layers.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This struct is part of an interface that can be used outside of this project scope. It should not be hidden. */
struct LinTransIf_sCallbackFunctions
{
    Lin_Version_t                   CallbackVersion;  /**< @brief TRANS Layer Callback Version.*/
  
    LinTransIf_ErrorCbFun_t         Error;            /**< @brief Pointer to TRANS Layer 'Error' callback function. (@ref LinTransIf_ErrorCbFun_t) @copydetails LinTransIf_ErrorCbFun_t */
    LinTransIf_SIDProcessedCbFun_t  SIDProcessed;     /**< @brief Pointer to TRANS Layer 'SID Processed' callback function. (@ref LinTransIf_SIDProcessedCbFun_t) @copydetails LinTransIf_SIDProcessedCbFun_t */
    LinTransIf_CheckNADCbFun_t      CheckNAD;         /**< @brief Pointer to TRANS Layer global 'Check NAD' callback function. (@ref LinTransIf_CheckNADCbFun_t) @copydetails LinTransIf_CheckNADCbFun_t */
    LinTransIf_GoToSleepCbFun_t     GoToSleep;        /**< @brief Pointer to TRANS Layer 'Go To Sleep' callback function. (@ref LinTransIf_GoToSleepCbFun_t) @copydetails LinTransIf_GoToSleepCbFun_t */
};
/** @} LinTransIfCbFunDefs */


/* ******************************************************************************
 * TRANS Layer interface function types
 *******************************************************************************/

/** @addtogroup LinTransIfIfFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of TRANS layer 'Initialization' interface function.
 *
 * @param[in] genericTransEnvData     Pointer to reserved TRANS Layer environment data.
 * @param[in] transEnvDataSze         Size of the reserved RAM for TRANS environment
 *                                    data.
 * @param[in] protoFrmDescInfo        Pointer to the 'Frame Description List' used
 *                                    by the TRANS Layer for its Request and Response
 *                                    Channel.
 * @param[in] nasTimeout              N_As timeout.
 *                                    (See also: LIN 2.2a Specification -
 *                                    Chapter 3.2.5 - TIMING CONSTRAINTS)
 * @param[in] ncrTimeout              N_Cr timeout.
 *                                    (See also: LIN 2.2a Specification -
 *                                    Chapter 3.2.5 - TIMING CONSTRAINTS)
 * @param[in] genericTransImpCfgData  Pointer to implementation dependent
 *                                    configuration data for the TRANS Layer
 *                                    (LinTransImp_sCfgData).
 *
 * @return LIN_TRUE if the initialization was successfully LIN_FALSE if not.
 *
 * Initializes the TRANS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_InitializationIfFun_t) (LinTransIf_pGenericEnvData_t             genericTransEnvData, LinTransIf_EnvDataSze_t           transEnvDataSze,
                                                        LinTransIf_cpProtoFrameDescriptionInfo_t protoFrmDescInfo,    
                                                        LinTransIf_cpCallbackFunctions_t         transCbFuns,         LinTransIf_pGenericCbCtxData_t    tenericTransCbCtxData,
                                                        LinTransIf_NasTimeout_t                  nasTimeout,          LinTransIf_NcrTimeout_t           ncrTimeout,          
                                                        LinTransIf_sCfgDataFlags_t               configFlags,         LinTransIf_pGenericImpCfgData_t   genericTransImpCfgData
                                                        );

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Task' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * The 'Task' function has to be called periodically to process scheduled task of
 * the TRANS Layer.
 *
 ******************************************************************************/
typedef void (*LinTransIf_TaskIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Get Sub-Interface' interface function.
 *
 * @param[in]  genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in]  interfaceId         Sub-Interface ID.
 * @param[out] ifThisPtr           Address (pointer) of an instance of the
 *                                 LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          @p ifThisPtr could be set.
 *
 * Acquires the desired sub-interface (@p interfaceId) and writes the address
 * of its This-Pointer to @p ifThisPtr.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_GetSubInterfaceIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Init Response' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] responseNAD         NAD which shall be used in the 'Response'.
 * @param[in] rsid                Response SID, normally SID + 0x40.
 *                                (See also: LIN 2.2a Specification -
 *                                Chapter 4.2.3.5 RSID)
 * @param[in] dataLen             Length of data to response with.
 *
 * @return LIN_TRUE if all parameter are correct.
 *
 * InitResponse may be only used in a SID Processing Callback
 * If the user application or a higher layer module receives a 'LIN Transport
 * Layer Request'. This function initializes the 'Response' to send.
 * Attn: If processing of a request SID with a valid data buffer is ongoing, this buffer will be invalidated after 
 * InitResponse has been called. 
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_InitResponseIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_NAD_t       responseNAD,
                                                      LinTransIf_SID_t             rsid,                LinTransIf_PDUMsgLen_t dataLen);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Append Data to Message Buffer' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] data                Pointer to data to append to message buffer.
 * @param[in] dataLen             Length of data to append.
 *
 * @return LIN_TRUE if the data has been appended successfully to the
 *         message buffer.
 *
 * AppendDataToMsgBuffer may be only used in a SID Processing Callback
 * This function is necessary to deserialize the collection of data and
 * the actual process of sending them back onto the bus through a
 * 'LIN Transport Layer Response'. Not every device has the RAM to buffer
 * the full amount of data, which is possible to send with the 'LIN Transport
 * Layer'.
 *
 * @see LIN 2.2a Specification - Chapter 3.2.1.4 LEN
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_AppendDataToMsgBufferIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_cpData_t data,
                                                               LinTransIf_BufLength_t       dataLen);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Init pending Response' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] responseNAD         NAD which shall be used in the 'Response'.
 * @param[in] rsid                Response SID, normally SID + 0x40.
 *                                (See also: LIN 2.2a Specification -
 *                                Chapter 4.2.3.5 RSID)
 * @param[in] data                Pointer to data part of this intermediate response.
 * @param[in] dataLen             Length of data to respond with (max. size quite limited! has to fit into a SF).
 *
 * @return LIN_TRUE if all parameter are correct.
 *
 * If the user application or a higher layer module receives a 'LIN Transport
 * Layer Request', this function initializes a intermediate response which will be send out until a final response gets available.
 * Typical application case: Long lasting UDS Services which replay with a "Response Pending" NRC until finished.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_InitIntermediateResponseIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_NAD_t       responseNAD,
                                                                  LinTransIf_SID_t             rsid,                LinTransIf_cpData_t    data, 
                                                                  LinTransIf_BufLength_t       dataLen);


/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Add SID Description List' interface function.
 *
 * @param[in] genericTransEnvData             Pointer to reserved TRANS Layer
 *                                            environment data.
 * @param[in] sidDescLst                      Pointer to the 'SID Description List
 *                                            to add to the TRANS Layer.
 * @param[in] individualCheckNAD              Pointer to the individual checkNAD callback function for this 'SID Description List'.
 *                                            May be NULL if this list is only reladed to the global checkNAD callback.
 *                                            The global checkNAD callback has always precedence over the individual method. Only if the 
 *                                            global callback returns 'LinTransIf_ChkNADRes_Unknown' the SID lists are searched for a 
 *                                            matching SIDs and a decision in corresponding check method.
 *                                            IndividualCheckNAD callbacks will be called with 'genericTransPerSidLstCbCtxData' as context data
 * @param[in] genericTransPerSidLstCbCtxData  Callback context data connected to this
 *                                            'SID List'.
 *                                            (See also: @ref LinTransIf_eSIDDescLstCbCtxType)
 *
 * @return LIN_TRUE, if the list has been successfully added.
 *
 * Adds a 'SID List' to the TRANS Layer on which SIDs it should react on. 
 * The new SID list will be added in front of all already inserted lists and therefore 
 * in case of multiple time defined SIDs gains priority over these lists. 
 * Evaluation order within a list: First match is taken. 
 * Remark: It is possible to pass SID processing to previously registered lists or to 
 *         the next maching entry within the same list by returning "LinTransIf_SIDRA_IGNORED" in the callback. 
 *         Transport-Layer will continue to lookup further matching entries.  
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_AddSidDescLstIfFun_t) (LinTransIf_pGenericEnvData_t   genericTransEnvData, LinTransIf_cpSIDDescription_t sidDescLst,
                                                       LinTransIf_CheckNADCbFun_t     individualCheckNAD,  LinTransIf_pGenericCbCtxData_t genericTransPerSidLstCbCtxData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Remove SID Description List' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] sidDescLst          Pointer to the 'SID Description List
 *                                to remove from the TRANS Layer.
 *
 * @return LIN_TRUE, if the list has been successfully removed.
 *
 * Removes a list of 'SIDs' from the TRANS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_RemoveSidDescLstIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_cpSIDDescription_t sidDescLst);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Append Callbacks' interface function.
 *
 * @param[in] genericTransEnvData   Pointer to reserved TRANS Layer environment data.
 * @param[in] transCbFuns           Pointer to TRANS Layer callback function.
 *                                  struct, which shall be added.
 * @param[in] genericTransCbCtxData Pointer to TRANS Layer callback context
 *                                  data.
 *
 * @return LIN_TRUE, if the callbacks have been successfully added to the
 *         TRANS Layer.
 *
 * The TRANS Layer supports more then one callback struct, to allow more
 * then one layer to use it. This function adds a additional callback
 * function struct (LinTransIf_cpCallbackFunctions_t) to the call queue.
 * The new callback list is added to the end of the calling queue. 
 * 
 * @see LinTransIf_RemoveCallbacksIfFun_t
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_AppendCallbacksIfFun_t) (LinTransIf_pGenericEnvData_t   genericTransEnvData, LinTransIf_cpCallbackFunctions_t transCbFuns,
                                                         LinTransIf_pGenericCbCtxData_t genericTransCbCtxData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Remove Callbacks' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] transCbFuns         Pointer to TRANS Layer callback function.
 *                                struct, which shall be removed.
 *
 * @return LIN_TRUE, if the callbacks have been successfully removed from the
 *         TRANS Layer.
 *
 * Remove callbacks added via LinTransIf_AppendCallbacksIfFun_t.
 *
 * @see LinTransIf_AppendCallbacksIfFun_t
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_RemoveCallbacksIfFun_t) (LinTransIf_pGenericEnvData_t   genericTransEnvData, LinTransIf_cpCallbackFunctions_t transCbFuns);


/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Get Status' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * @return The current status of the TRANS Layer.
 *
 * Acquire the current status of the TRANS Layer.
 *
 ******************************************************************************/
typedef LinTransIf_eComStatus_t (*LinTransIf_GetStatusIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Get max. message size' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * @return Maximum message size. 
 *
 * Returns the maximum number of bytes in a request/response message. (plain data, the SID is counted separately!)
 * Since the actuall message size is calculated in the initialization phase according to the available env size,
 * this function might be helpfull for cross-check whether enought buffes have been allocated. 
 *
 ******************************************************************************/
typedef LinTransIf_PDUMsgLen_t (*LinTransIf_GetMaxMessageSizeIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Get max. pending resp. size' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * @return Maximum intermediate response. 
 *
 * Returns the maximum number of bytes in an intermediate response message as initiated by @ref LinTransIf_InitPendingResponseIfFun_t. (plain data, the SID is counted separately!)
 * This buffer is quite small, since intermediate responses have to fit into a single LIN frame (of type SF) 
 * Will return 0 if not supported by a particular implementation. 
 *
 ******************************************************************************/
typedef LinTransIf_PDUMsgLen_t (*LinTransIf_GetMaxIntermediateResponseSizeIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Set Timeout' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] timeoutType         Timeout type to set. (@ref LinTransIf_eTimeoutType_t)
 * @param[in] timeout             Timeout value.
 *
 * Set the N_As or N_Cr timeout.
 *
 ******************************************************************************/
typedef void (*LinTransIf_SetTimeoutIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_eTimeoutType_t timeoutType, LinTransIf_Timeout_t timeout);

/***************************************************************************//**
 * @brief Typedef of TRANS Layer 'Trigger SID Processing' interface function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] hint                  Processing hint passed to the SID callback once invoked.
 * @param[in] nad                   LIN Transport Layer Node Address (See also:
 *                                  LIN 2.2a Specification - Chapter 4.2.3.2 NAD)
 * @param[in] SIDType               Type of processing this callback ist called for (req/resp)
 * @param[in] sid                   Related Service ID.
 *
 * Triggers SID processing as it would have been cause by a bus request or respoonse transmission. 
 * This function is usefull to re-trigger some request processing after a prolonged action has completed.
 * Manually triggered processing has no request buffer associated to it. This can be identified in the callback by checking 
 * 'data' pointer for NULL
 * It is not guaranteed that a particular processing will take place. Since concurent bus actions (i.e. incomming request) 
 * may abort this processing request. 
 * Multiple calls to this function before processing could have been performed (i.e. transport task has been executed) 
 * will be ignored. Only the first request will take place. 
 * It is not allowed to pass LINTRANSIF_SID_PROC_HINT_TRANSPORT as hint. This will be squashed to LINTRANSIF_SID_PROC_HINT_GENERIC_MANUAL_TRIGGER!
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinTransIf_TriggerSIDProcessingIfFun_t) (LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_SIDProcessingHint_t hint, 
                                                        LinTransIf_NAD_t             nad,                 LinTransIf_eSIDType_t          SIDType,  
                                                        LinTransIf_SID_t             sid);

/***************************************************************************//**
 * @brief TRANS Layer interface functions struct.
 *
 * Collective struct for the TRANS Layer interface functions.
 ******************************************************************************/
struct LinTransIf_sInterfaceFunctions
{
    Lin_Version_t                                     InterfaceVersion;               /**< @brief TRANS Layer interface version.*/

    LinTransIf_InitializationIfFun_t                  Initialization;                 /**< @brief Pointer to TRANS Layer 'Initialization' interface function. (@ref LinTransIf_InitializationIfFun_t) @copydetails LinTransIf_InitializationIfFun_t */
    LinTransIf_TaskIfFun_t                            Task;                           /**< @brief Pointer to TRANS Layer 'Task' interface function. (@ref LinTransIf_TaskIfFun_t) @copydetails LinTransIf_TaskIfFun_t */
    LinTransIf_GetSubInterfaceIfFun_t                 GetSubInterface;                /**< @brief Pointer to TRANS Layer 'Get Sub Interface' interface function. (@ref LinTransIf_GetSubInterfaceIfFun_t) @copydetails LinTransIf_GetSubInterfaceIfFun_t */
    LinTransIf_AppendCallbacksIfFun_t                 AppendCallbacks;                /**< @brief Pointer to TRANS Layer 'Append Callbacks List' interface function. (@ref LinTransIf_AppendCallbacksIfFun_t) @copydetails LinTransIf_AppendCallbacksIfFun_t */
    LinTransIf_RemoveCallbacksIfFun_t                 RemoveCallbacks;                /**< @brief Pointer to TRANS Layer 'Remove Callbacks List' interface function. (@ref LinTransIf_RemoveCallbacksIfFun_t) @copydetails LinTransIf_RemoveCallbacksIfFun_t */
    LinTransIf_AddSidDescLstIfFun_t                   AddSIDDescriptionList;          /**< @brief Pointer to TRANS Layer 'Add SID Description List' interface function. (@ref LinTransIf_AddSidDescLstIfFun_t) @copydetails LinTransIf_AddSidDescLstIfFun_t */
    LinTransIf_RemoveSidDescLstIfFun_t                RemoveSIDDescriptionList;       /**< @brief Pointer to TRANS Layer 'Remove SID Description List' interface function. (@ref LinTransIf_RemoveSidDescLstIfFun_t) @copydetails LinTransIf_RemoveSidDescLstIfFun_t */
    LinTransIf_GetStatusIfFun_t                       GetStatus;                      /**< @brief Pointer to TRANS Layer 'Get Status' interface function. (@ref LinTransIf_GetStatusIfFun_t) @copydetails LinTransIf_GetStatusIfFun_t */
    LinTransIf_GetMaxMessageSizeIfFun_t               GetMaxMessageSize;              /**< @brief Pointer to TRANS Layer 'Get max. message size' interface function. (@ref LinTransIf_GetMaxMessageSizeIfFun_t) @copydetails LinTransIf_GetMaxMessageSizeIfFun_t */
    LinTransIf_GetMaxIntermediateResponseSizeIfFun_t  GetMaxIntermediateResponseSize; /**< @brief Pointer to TRANS Layer 'Get max. pending resp. size' interface function. (@ref LinTransIf_GetMaxPendingResponseSizeIfFun_t) @copydetails LinTransIf_GetMaxPendingResponseSizeIfFun_t */
    LinTransIf_SetTimeoutIfFun_t                      SetTimeout;                     /**< @brief Pointer to TRANS Layer 'Set Timeout' interface function. (@ref LinTransIf_SetTimeoutIfFun_t) @copydetails LinTransIf_SetTimeoutIfFun_t */
    LinTransIf_TriggerSIDProcessingIfFun_t            TriggerSIDProcessing;           /**< @brief Pointer to TRANS Layer 'CancelPendingResponse' interface function. (@ref LinTransIf_SetTimeoutIfFun_t) @copydetails LinTransIf_SetTimeoutIfFun_t */
 };

/***************************************************************************//**
 * @brief TRANS Layer response buffer modification sub-interface.
 *
 * Collective struct for the TRANS Layer interface functions.
 ******************************************************************************/
struct LinTransIf_sResponseSubInterfaceFunctions
{
    Lin_Version_t                                     InterfaceVersion;               /**< @brief TRANS Layer interface version.*/

    LinTransIf_InitResponseIfFun_t                    InitResponse;                   /**< @brief Pointer to TRANS Layer 'Init Response' interface function. (@ref LinTransIf_InitResponseIfFun_t) @copydetails LinTransIf_InitResponseIfFun_t */
    LinTransIf_AppendDataToMsgBufferIfFun_t           AppendDataToMsgBuffer;          /**< @brief Pointer to TRANS Layer 'Append Data to Message Buffer' interface function. (@ref LinTransIf_AppendDataToMsgBufferIfFun_t) @copydetails LinTransIf_AppendDataToMsgBufferIfFun_t */
    LinTransIf_InitIntermediateResponseIfFun_t        InitIntermediateResponse;       /**< @brief Pointer to TRANS Layer 'InitIntermediateResponse' interface function. (@ref LinTransIf_InitIntermediateResponseIfFun_t) @copydetails LinTransIf_InitIntermediateResponseIfFun_t */
 };


/** @} LinTransIfIfFunDefs */


/***************************************************************************//**
 * @brief Object-like This-pointer for callbacks and 
 *        to connect the TRANS Layer to higher layers or application.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This struct is part of an interface that can be used outside of this project scope. It should not be hidden. */
struct LinTransIf_sThis
{
    LinTransIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the TRANS Layer interface function struct. (LinTransIf_sInterfaceFunctions) */
    LinTransIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved TRANS Layer environment data. */
};
/***************************************************************************//**
 * @brief Object-like This-pointer for callbacks and 
 *        to connect the TRANS Layer to higher layers or application.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This struct is part of an interface that can be used outside of this project scope. It should not be hidden. */
struct LinTransIf_sResponseSubInterfaceThis
{
    LinTransIf_cpResponseSubInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the TRANS Layer response construction interface function struct. (LinTransIf_sResponseSubInterfaceFunctions) */
    LinTransIf_pGenericEnvData_t                 envData;   /**< @brief Pointer to corresponding TRANS Layer environment data. */
};

/***************************************************************************//**
 * @brief TRANS Layer interface configuration parameter.
 *
 * Data needed for initialization of the TRANS Layer.
 ******************************************************************************/
struct LinTransIf_sInitParam
{
    LinTransIf_cpInterfaceFunctions_t        IfFunsTbl;         /**< @brief Pointer to the constant TRANS Layer interface function struct. */
    LinTransIf_pGenericEnvData_t             envData;           /**< @brief Pointer to reserved TRANS Layer environment data. */
    LinTransIf_EnvDataSze_t                  EnvDataLen;        /**< @brief Size of the reserved RAM for TRANS Layer environment data. */
    LinTransIf_cpProtoFrameDescriptionInfo_t protoFrmDescInfo;  /**< @brief Pointer to the 'Frame Description List' used by the TRANS Layer for its Request and Response Channel. */
    LinTransIf_NasTimeout_t                  nasTimeout;        /**< @brief N_As timeout (See also: LIN 2.2a Specification - Chapter 3.2.5 - TIMING CONSTRAINTS) */
    LinTransIf_NcrTimeout_t                  NcrTimeout;        /**< @brief N_Cr timeout (See also: LIN 2.2a Specification - Chapter 3.2.5 - TIMING CONSTRAINTS) */
    LinTransIf_pGenericImpCfgData_t          ImpCfgData;        /**< @brief Pointer to implementation dependent configuration data for the TRANS Layer. (LinTransImp_sCfgData) */
    LinTransIf_sCfgDataFlags_t               InitCfgFlags;      /**< @brief Initial configuration flags for setting up behaviour of transport layer */
};

/***************************************************************************//**
 * @brief TRANS Layer 'SID Description' struct.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This struct is part of an interface that can be used outside of this project scope. It should not be hidden. */
struct LinTransIf_sSIDDescription
{
    LinTransIf_SID_t                  StartSID;        /**< @brief The first Service ID (SID) this entry is valid for. */
    LinTransIf_SID_t                  EndSID;          /**< @brief The last Service ID (SID) this entry is valid for. */
    LinTransIf_SIDCbFun_t             CallbackFnc;     /**< @brief Pointer to SID callback function. */
    LinTransIf_eSIDDescLstCbCtxType_t CallbackCtxType; /**< @brief Type of the context data pointer for the SID callback. (See also: @ref LinTransIf_eSIDDescLstCbCtxType) */
    LinTransIf_SIDCbCtx_t             CallbackCtx;     /**< @brief Pointer to callback context data. (See also: @ref LinTransIf_SIDDescLstCbCtxType_PER_SID) */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINTRANS_INTERFACE_H_ */

/*! @} */ /* TRANS */
/*! @} */ /* ELMOS_LIN_DRIVER */
