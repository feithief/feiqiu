/***************************************************************************//**
 * @file			LinProto_Interface.h
 *
 * @creator		sbai
 * @created		15.01.2015
 *
 * @brief The LIN PROTO Layer module implements the LIN protocol layer described
 *        in the LIN specification. It is an sophisticated and LIN message based
 *        module with an respective interface. In simple terms it resembles the
 *        LDS-based view of a node. The user configures a special table which
 *        represents his frames defined in his LDF-file and the PROTO module
 *        'automatically' send or receives data from the LIN bus.
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

#ifndef LINPROTO_INTERFACE_H_
#define LINPROTO_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinProto_FrameDescription.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define LINPROTO_INTERFACE_MODULE_API_VERSION      0x0102u /**< LIN PROTO interface version */

#define LINPROTO_INTERFACE_MAX_CALLBACK_TABLES     3u      /**< Maximal allowed count of callback
                                                                tables which can be added to the PROTO Layer. */

//#define LIN_SPEC_1_3.

#if LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1
/***************************************************************************//**
 * @brief Macro for adding signals to a 'Signal List'.
 * 
 * A signal list holds all signals for one frame.
 * Signals can be defined like in the LDF or they can be used for automatic data
 * handling for "normal" frames without signals in the LDF.
 *
 * @param TYPE[in]		The type of the signal. The type determines the data handling of the signal. @see LinProtoIf_eSignalType_t
 * @param LEN[in]		The length of the signal in Bits or Bytes (determines on the type).@see LinProtoIf_SignalLenght_t 	
 * @param OFFSET[in] 	The offset in the frame in Bits. @see LinProtoIf_SignalOffset_t
 * @param PTRTODATA[in] The content of this data will automatically be copied into/from signal
 * @param CTXDATA[in] 	Pointer to the context data (if a custom - not global - data is needed in the callback function). Only used if type is a function 
 ******************************************************************************/
#define LINPROTO_INTERFACE_ADD_SIGNAL(TYPE,                                               \
                                      LENGTH,                                             \
                                      OFFSET,                                             \
                                      PTRTODATA,                                          \
                                      CTXDATA)                                            \
                                                                                          \
                                      {.Type      = (TYPE),                                 \
                                       .Length    = (LENGTH),                               \
                                       .Offset    = (OFFSET),                               \
                                       .PtrToData = (LinProtoIf_pSignalData_t) (PTRTODATA), \
                                       .CtxData   = (LinProtoIf_pCtxData_t) (CTXDATA)}

/***************************************************************************//**
 * @brief Macro to mark the end of a 'Signal List'.
 ******************************************************************************/
#define LINPROTO_INTERFACE_SIGNAL_END {.Type = LinProtoIf_SignalType_Invalid}
#endif /* LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1 */

#if LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1
  /***************************************************************************//**
 * Macro to add a 'Frame Description' to a 'Frame Description List'.
 * For LIN2.0/SAE-J2602 with support of signal lists and handler functions.
   ******************************************************************************/
  #define LINPROTO_INTERFACE_ADD_FRAME_DESCRIPTION_LIN20(DEFAULT_FRAME_ID,          \
                                                   CRC_TYPE,                  \
                                                   FRAME_KIND,                \
                                                   MSG_ID,                    \
                                                   FRAME_LENGHT,              \
                                                   SIGNAL_LST_ADR,            \
                                                   FRAME_ID_PROC_CB_HANDLING, \
                                                   FRAME_ID_PROC_CB_FUNC,     \
                                                   FRAME_ID_PROC_CB_CTX_TYPE, \
                                                   FRAME_ID_PROC_CB_CTX)      \
                                                   \
                                                   {.DefaultFrameID              = (DEFAULT_FRAME_ID),                      \
                                                    .CrcType                     = (CRC_TYPE),                              \
                                                    .Kind                        = (FRAME_KIND),                            \
                                                    .MsgID                       = (MSG_ID),                                \
                                                    .Lenght                      = (FRAME_LENGHT),                          \
                                                    .PtrToSgnlLst                = (LinProtoIf_pGenericSignalList_t) (SIGNAL_LST_ADR), \
                                                    .FrameIdProcCbHandling       = (FRAME_ID_PROC_CB_HANDLING),             \
                                                    .FrameIdProcessedCallbackFnc = (FRAME_ID_PROC_CB_FUNC),                 \
                                                    .FrameIdProcessedCbCtxType   = (FRAME_ID_PROC_CB_CTX_TYPE),             \
                                                    .FrameIdProcessedCallbackCtx = (LinProtoIf_pGenericCbCtxData_t) (FRAME_ID_PROC_CB_CTX)}
#else
  /***************************************************************************//**
   * Macro to add a 'Frame Description' to a 'Frame Description List'.
   * For LIN2.0/SAE-J2602 w/o support of signal lists.
   ******************************************************************************/
  #define LINPROTO_INTERFACE_ADD_FRAME_DESCRIPTION_LIN20(DEFAULT_FRAME_ID,          \
                                                   CRC_TYPE,                  \
                                                   FRAME_KIND,                \
                                                   MSG_ID,                    \
                                                   FRAME_LENGHT,              \
                                                   FUNCTION_ADR,              \
                                                   FRAME_ID_PROC_CB_HANDLING, \
                                                   FRAME_ID_PROC_CB_FUNC,     \
                                                   FRAME_ID_PROC_CB_CTX_TYPE, \
                                                   FRAME_ID_PROC_CB_CTX)      \
                                                   \
                                                   {.DefaultFrameID              = (DEFAULT_FRAME_ID),                      \
                                                    .CrcType                     = (CRC_TYPE),                              \
                                                    .Kind                        = (FRAME_KIND),                            \
                                                    .MsgID                       = (MSG_ID),                                \
                                                    .Lenght                      = (FRAME_LENGHT),                          \
                                                    .PtrToFun                    = (LinProtoIf_pGenericFun_t) (FUNCTION_ADR), \
                                                    .FrameIdProcCbHandling       = (FRAME_ID_PROC_CB_HANDLING),             \
                                                    .FrameIdProcessedCallbackFnc = (FRAME_ID_PROC_CB_FUNC),                 \
                                                    .FrameIdProcessedCbCtxType   = (FRAME_ID_PROC_CB_CTX_TYPE),             \
                                                    .FrameIdProcessedCallbackCtx = (LinProtoIf_pGenericCbCtxData_t) (FRAME_ID_PROC_CB_CTX)}
#endif

#if LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1
  /***************************************************************************//**
   * Macro to add a 'Frame Description' to a 'Frame Description List'.
   * For LIN2.2 with support of signal lists and handler functions.
   ******************************************************************************/
//PRQA S 0791 ++ #In C99 there are 63 significant characters supported and not 31 like required in this c90 rules
  #define LINPROTO_INTERFACE_ADD_FRAME_DESCRIPTION(DEFAULT_FRAME_ID,          \
                                                   CRC_TYPE,                  \
                                                   FRAME_KIND,                \
                                                   FRAME_LENGHT,              \
                                                   SIGNAL_LST_ADR,            \
                                                   FRAME_ID_PROC_CB_HANDLING, \
                                                   FRAME_ID_PROC_CB_FUNC,     \
                                                   FRAME_ID_PROC_CB_CTX_TYPE, \
                                                   FRAME_ID_PROC_CB_CTX)      \
                                                   \
                                                   {.DefaultFrameID              = (DEFAULT_FRAME_ID),                      \
                                                    .CrcType                     = (CRC_TYPE),                              \
                                                    .Kind                        = (FRAME_KIND),                            \
                                                    .MsgID                       = 0,                                     \
                                                    .Lenght                      = (FRAME_LENGHT),                          \
                                                    .PtrToSgnlLst                = (LinProtoIf_pGenericSignalList_t) (SIGNAL_LST_ADR), \
                                                    .FrameIdProcCbHandling       = (FRAME_ID_PROC_CB_HANDLING),             \
                                                    .FrameIdProcessedCallbackFnc = (FRAME_ID_PROC_CB_FUNC),                 \
                                                    .FrameIdProcessedCbCtxType   = (FRAME_ID_PROC_CB_CTX_TYPE),             \
                                                    .FrameIdProcessedCallbackCtx = (LinProtoIf_pGenericCbCtxData_t) (FRAME_ID_PROC_CB_CTX)}
//PRQA S 0791 --
#else
  /***************************************************************************//**
   * Macro to add a 'Frame Description' to a 'Frame Description List'.
   * For LIN2.2 w/o support of signal lists.
   ******************************************************************************/
  #define LINPROTO_INTERFACE_ADD_FRAME_DESCRIPTION(DEFAULT_FRAME_ID,          \
                                                   CRC_TYPE,                  \
                                                   FRAME_KIND,                \
                                                   FRAME_LENGHT,              \
                                                   FUNCTION_ADR,              \
                                                   FRAME_ID_PROC_CB_HANDLING, \
                                                   FRAME_ID_PROC_CB_FUNC,     \
                                                   FRAME_ID_PROC_CB_CTX_TYPE, \
                                                   FRAME_ID_PROC_CB_CTX)      \
                                                   \
                                                   {.DefaultFrameID              = (DEFAULT_FRAME_ID),                      \
                                                    .CrcType                     = (CRC_TYPE),                              \
                                                    .Kind                        = (FRAME_KIND),                            \
                                                    .MsgID                       = 0,                                     \
                                                    .Lenght                      = (FRAME_LENGHT),                          \
                                                    .PtrToFun                    = (LinProtoIf_pGenericFun_t) (FUNCTION_ADR), \
                                                    .FrameIdProcCbHandling       = (FRAME_ID_PROC_CB_HANDLING),             \
                                                    .FrameIdProcessedCallbackFnc = (FRAME_ID_PROC_CB_FUNC),                 \
                                                    .FrameIdProcessedCbCtxType   = (FRAME_ID_PROC_CB_CTX_TYPE),             \
                                                    .FrameIdProcessedCallbackCtx = (LinProtoIf_pGenericCbCtxData_t) (FRAME_ID_PROC_CB_CTX)}
#endif /* LINPROTOIMP_SUPPORT_SIGNAL_LISTS == 1 */


/***************************************************************************//**
 * Macro for marking the end of a 'Frame Description List'
 ******************************************************************************/
#define LINPROTO_INTERFACE_FRAME_DESCRIPTION_END  {.Kind = LinProtoIf_FrameKind_INVALID}

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ******************************************************************************
 * LIN Protocol Layer Module interface function types
 *******************************************************************************/

/** @addtogroup LinProtoIfIfFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Initialization' interface function.
 *
 * @param[in] genericProtoEnvData     Pointer to reserved PROTO Layer environment data.
 * @param[in] protoEnvDataSze         Size of the reserved RAM for PROTO environment
 *                                    data.
 * @param[in] protoCbFuns             Pointer to PROTO Layer callback function.
 *                                    struct. Implemented in the higher layer or
 *                                    user application.
 * @param[in] genericProtoCbCtxData   Pointer to PROTO Layer callback context
 *                                    data.
 * @param[in] genericProtoImpCfgData  Pointer to implementation dependent
 *                                    configuration data for the PROTO Layer
 *                                    (LinProtoImp_sCfgData).
 *
 * @return LIN_TRUE if the initialization was successfully LIN_FALSE if not.
 *
 * Initializes the LIN PROTO Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_InitializationIfFun_t) (LinProtoIf_pGenericEnvData_t     genericProtoEnvData, LinProtoIf_EnvDataSze_t        protoEnvDataSze,
                                                        LinProtoIf_cpCallbackFunctions_t protoCbFuns,         LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData,
                                                        LinProtoIf_pGenericImpCfgData_t  genericProtoImpCfgData);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Get Sub Interface' interface function.
 *
 * @param[in]  genericProtoEnvData  Pointer to reserved PROTO Layer environment data.
 * @param[in]  interfaceId          Sub-Interface ID.
 * @param[out] ifThisPtr            Address (pointer) of an instance of the
 *                                  LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          @p ifThisPtr could be set.
 *
 * Acquires the desired sub-interface (@p interfaceId) and writes the address
 * of its This-Pointer to @p ifThisPtr.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_GetSubInterfaceIfFun_t) (LinProtoIf_pGenericEnvData_t genericProtoEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Task' interface function.
 *
 * @param[in] genericProtoEnvData Pointer to reserved PROTO Layer environment data.
 *
 * The 'Task' function has to be called periodically to process scheduled task of
 * the PROTO Layer.
 *
 ******************************************************************************/
typedef void (*LinProtoIf_TaskIfFun_t) (LinProtoIf_pGenericEnvData_t genericProtoEnvData);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Append Callbacks' interface function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoCbFuns           Pointer to PROTO Layer callback function.
 *                                  struct, which shall be added.
 * @param[in] genericProtoCbCtxData Pointer to PROTO Layer callback context
 *                                  data.
 *
 * @return LIN_TRUE, if the callbacks have been successfully added to the
 *         PROTO Layer.
 *
 * The PROTO Layer supports more then one callback struct, to allow more
 * then one layer to use it. This function adds a additional callback
 * function struct (LinProtoIf_sCallbackFunctions) to the call queue.
 *
 * @see LinProtoIf_RemoveCallbacksIfFun_t
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_AppendCallbacksIfFun_t) ( LinProtoIf_pGenericEnvData_t   genericProtoEnvData, 
                                                          LinProtoIf_cpCallbackFunctions_t protoCbFuns,
                                                          LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Remove Callbacks' interface function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoCbFuns           Pointer to PROTO Layer callback function.
 *                                  struct, which shall be removed.
 *
 * @return LIN_TRUE, if the callbacks have been successfully removed from the
 *         PROTO Layer.
 *
 * Remove callbacks added via LinProtoIf_AppendCallbacksIfFun_t.
 *
 * @see LinProtoIf_AppendCallbacksIfFun_t
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_RemoveCallbacksIfFun_t) ( LinProtoIf_pGenericEnvData_t genericProtoEnvData, 
                                                          LinProtoIf_cpCallbackFunctions_t protoCbFuns);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Add Frame Description List' interface function.
 *
 * @param[in] genericProtoEnvData             Pointer to reserved PROTO Layer
 *                                            environment data.
 * @param[in] genericFrmDscLst                Pointer to 'Frame Description List'
 *                                            to add.
 * @param[in] ldfRelevance                    TRUE if the 'Frame IDs' in this list
 *                                            can be assigned/unassigned.
 * @param[in] genericFrmDescLstEnvData        Environment data per 'Frame
 *                                            Description List'.
 * @param[in] frmDescLstEnvDataSze            Size of reserved environment data
 *                                            per 'Frame Description List'.
 * @param[in] genericProtoPerDescLstCbCtxData Callback context data connected to
 *                                            this list
 *                                            (See also: LinProtoIf_eFrameIdProcCbCtxDataType).
 *
 * @return TRUE, if the list has been successfully added.
 *
 * Adds a 'Frame Description List' with 'Frame Descriptions' to which the
 * PROTO Layer reacts on.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_AddFrameDescLstIfFun_t) (LinProtoIf_pGenericEnvData_t genericProtoEnvData,  LinProtoIf_cpGenericFrameDescriptionList_t genericFrmDscLst,
                                                         Lin_Bool_t                   ldfRelevance,         LinProtoIf_pGenericFrmDescLstEnvData_t     genericFrmDescLstEnvData,
                                                         LinProtoIf_EnvDataSze_t      frmDescLstEnvDataSze, LinProtoIf_pGenericCbCtxData_t             genericProtoPerDescLstCbCtxData);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Remove Frame Description List' interface function.
 *
 * @param[in] genericProtoEnvData Pointer to reserved PROTO Layer environment data.
 * @param[in] genericFrmDscLst    Pointer to 'Frame Description List' to remove.
 *
 * @return TRUE, if the list has been successfully removed.
 *
 * Removes a list of 'Frame Descriptions' from the PROTO Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinProtoIf_RmvFrameDescLstIfFun_t) (LinProtoIf_pGenericEnvData_t genericProtoEnvData, LinProtoIf_cpGenericFrameDescriptionList_t genericFrmDscLst);

/***************************************************************************//**
 * @brief PROTO Layer interface functions struct.
 *
 * Collective struct for the PROTO Layer interface functions.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This interface function can be dereference outside of this project scope. It should not be hidden. */
struct LinProtoIf_sInterfaceFunctions
{
    Lin_Version_t                     InterfaceVersion;  /**< Interface version */

    LinProtoIf_InitializationIfFun_t  Initialization;    /**< @brief Pointer to PROTO Layer 'Initialization' interface function. (@ref LinProtoIf_InitializationIfFun_t) @copydetails LinProtoIf_InitializationIfFun_t */
    LinProtoIf_GetSubInterfaceIfFun_t GetSubInterface;   /**< @brief Pointer to PROTO Layer 'Get Sub-Interface' interface function. (@ref LinProtoIf_GetSubInterfaceIfFun_t) @copydetails LinProtoIf_GetSubInterfaceIfFun_t */
    LinProtoIf_TaskIfFun_t            Task;              /**< @brief Pointer to PROTO Layer 'Task' interface function. (@ref LinProtoIf_TaskIfFun_t) @copydetails LinProtoIf_TaskIfFun_t */
    LinProtoIf_AppendCallbacksIfFun_t AppendCallbacks;   /**< @brief Pointer to PROTO Layer 'AppendCallbacks' interface function. (@ref LinProtoIf_AppendCallbacksIfFun_t) @copydetails LinProtoIf_AppendCallbacksIfFun_t */
    LinProtoIf_RemoveCallbacksIfFun_t RemoveCallbacks;   /**< @brief Pointer to PROTO Layer 'RemoveCallbacks' interface function. (@ref LinProtoIf_RemoveCallbacksIfFun_t) @copydetails LinProtoIf_RemoveCallbacksIfFun_t */
    LinProtoIf_AddFrameDescLstIfFun_t AddFrameDescLst;   /**< @brief Pointer to PROTO Layer 'AddFrameDescLst' interface function. (@ref LinProtoIf_AddFrameDescLstIfFun_t) @copydetails LinProtoIf_AddFrameDescLstIfFun_t */
    LinProtoIf_RmvFrameDescLstIfFun_t RmvFrameDescLst;   /**< @brief Pointer to PROTO Layer 'RmvFrameDescLst' interface function. (@ref LinProtoIf_RmvFrameDescLstIfFun_t) @copydetails LinProtoIf_RmvFrameDescLstIfFun_t */
};

/** @} LinProtoIfIfFunDefs */

/* *****************************************************************************
 * PROTO Layer callback function types
 ******************************************************************************/

/** @addtogroup LinProtoIfCbFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Error' callback function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoIfFuns           Pointer to the PROTO Layer interface function struct.
 * @param[in] error                 Occurred error.
 * @param[in] frameID               Related Frame ID if known, otherwise
 *                                  "LINDRV_INVALID_FRAMEID".
 * @param[in] frameDesc             Pointer to 'Frame Description' associated to
 *                                  the received Frame ID (@p frameID), if available.
 * @param[in] genericProtoCbCtxData Pointer to PROTO Layer callback context data.
 *
 * Indicates an error in the PROTO Layer and underlying layers of the LIN
 * Driver.
 *
 * @par "Call Description:"
 * @mscfile msc_proto_errorcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinProtoIf_ErrorCbFun_t) (LinProtoIf_pGenericEnvData_t    genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                         LinProtoIf_Error_t              error,               LinBusIf_FrameID_t                frameID,
                                         LinProtoIf_cpFrameDescription_t frameDesc,           LinProtoIf_pGenericCbCtxData_t    genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Restart' callback function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoIfFuns           Pointer to the PROTO Layer interface function struct.
 * @param[in] genericProtoCbCtxData Pointer to PROTO Layer callback context data.
 *
 * This callback is invoked once the LIN driver performs a restart
 * (i.e. typically after an error, or wakeup )
 *
 * @par "Call Description 1:"
 * @mscfile msc_proto_restartcbcalled_1.dox
 * @n
 * @par "Call Description 2:"
 * @mscfile msc_proto_restartcbcalled_2.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinProtoIf_RestartCbFun_t) (LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                           LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Baud Rate Measurement Done' callback function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoIfFuns           Pointer to the PROTO Layer interface function struct.
 * @param[in] exp_baudrate          The nominal (configured) baud rate.
 * @param[in] prev_baudrate         The baud rate used for previous message, and as the
 *                                  initial baud rate for sync measurement.
 * @param[in] meas_baudrate         The actual measured baud rate.
 * @param[in] genericProtoCbCtxData Pointer to PROTO Layer callback context data.
 *
 * @return How to proceed with the measured baudrate value.
 *        (@ref LinProtoIf_eMeasDoneRetVal)
 *
 * This callback event indicates successful measurement of a sync character.
 * The measured baud rate (in terms of divider value) is provided as argument.
 *
 * @par "Call Description:"
 * @mscfile msc_proto_measdonecbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinProtoIf_eMeasDoneRetVal_t (*LinProtoIf_MeasDoneCbFun_t) (LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                                                    LinProtoIf_Baudrate_t          exp_baudrate,        LinProtoIf_Baudrate_t             prev_baudrate,
                                                                    LinProtoIf_Baudrate_t          meas_baudrate,       LinProtoIf_BreakLen_t             meas_break_len,
                                                                    LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);


/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Idle' callback function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoIfFuns           Pointer to the PROTO Layer interface function struct.
 * @param[in] genericProtoCbCtxData Pointer to PROTO Layer callback context data.
 *
 * @return Action how to proceed after the reception/transmission.
 *
 * This event indicates that there was no bus action for specified idle duration.
 *
 * @par "Call Description:"
 * @mscfile msc_proto_idlecbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinProtoIf_eIdleAction_t (*LinProtoIf_IdleCbFun_t) (LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                                            LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief Typedef of PROTO Layer 'Wakeup' callback function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoIfFuns           Pointer to the PROTO Layer interface function struct.
 * @param[in] genericProtoCbCtxData Pointer to PROTO Layer callback context data.
 *
 * A wakeup event (basically a break start while the bus drive is in sleep mode)
 * occurred.
 *
 * @par "Call Description:"
 * @mscfile msc_proto_wakupcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinProtoIf_WakeupCbFun_t) (LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                          LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @brief LIN PROTO Layer callback functions struct.
 *
 * The set of callbacks which should be invoked in the higher layers.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This struct of callback functions can be dereference outside of this project scope. It should not be hidden. */
struct LinProtoIf_sCallbackFunctions
{
    Lin_Version_t              CallbackVersion; /**< Callback Version */

    LinProtoIf_ErrorCbFun_t    Error;           /**< @brief Pointer to PROTO Layer 'Error' callback function. (@ref LinProtoIf_ErrorCbFun_t) @copydetails LinProtoIf_ErrorCbFun_t */
    LinProtoIf_RestartCbFun_t  Restart;         /**< @brief Pointer to PROTO Layer 'Restart' callback function. (@ref LinProtoIf_RestartCbFun_t) @copydetails LinProtoIf_RestartCbFun_t */
    LinProtoIf_MeasDoneCbFun_t MeasDone;        /**< @brief Pointer to PROTO Layer 'MeasDone' callback function. (@ref LinProtoIf_MeasDoneCbFun_t) @copydetails LinProtoIf_MeasDoneCbFun_t */
    LinProtoIf_IdleCbFun_t     Idle;            /**< @brief Pointer to PROTO Layer 'Idle' callback function. (@ref LinProtoIf_IdleCbFun_t) @copydetails LinProtoIf_IdleCbFun_t */
    LinProtoIf_WakeupCbFun_t   WakeUp;          /**< @brief Pointer to PROTO Layer 'Wakeup' callback function. (@ref LinProtoIf_WakeupCbFun_t) @copydetails LinProtoIf_WakeupCbFun_t */
};
/** @} LinProtoIfCbFunDefs */

/***************************************************************************//**
 * @brief  Object-like This-pointer to connect the PROTO Layer to higher layers
 ******************************************************************************/
struct LinProtoIf_sThis
{
    LinProtoIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the PROTO Layer interface function struct. (@ref LinProtoIf_sInterfaceFunctions) */
    LinProtoIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved PROTO Layer environment data. */
};

/***************************************************************************//**
 * @brief LIN PROTO Layer interface configuration parameter.
 *
 * Data needed for initialization of the PROTO Layer.
 ******************************************************************************/
struct LinProtoIf_sInitParam
{
    LinProtoIf_cpInterfaceFunctions_t IfFunsTbl;  /**< @brief Pointer to the constant PROTO Layer interface function struct. */
    LinProtoIf_pGenericEnvData_t      envData;    /**< @brief Pointer to reserved PROTO Layer environment data. */
    LinProtoIf_EnvDataSze_t           EnvDataLen; /**< @brief Size of the reserved RAM for PROTO Layer environment data. */
    LinProtoIf_pGenericImpCfgData_t   ImpCfgData; /**< @brief Pointer to implementation dependent configuration data for the Proto Layer. (LinProtoImp_sCfgData) */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINPROTO_INTERFACE_H_ */

/*! @} */ /* PROTO */
/*! @} */ /* ELMOS_LIN_DRIVER */
