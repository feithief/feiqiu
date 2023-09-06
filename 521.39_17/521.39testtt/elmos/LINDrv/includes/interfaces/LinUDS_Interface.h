/***************************************************************************//**
 * @file      LinUDS_Interface.h
 *
 * @creator   sbai
 * @created   25.03.2015
 *
 * @brief     Definition of the interface for the 'LIN UDS Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DIAG_UDS DIAG_UDS Layer
 * @{
 *
 * $Id: LinUDS_Interface.h 8305 2021-02-03 13:29:35Z poe $
 *
 * $Revision: 8305 $
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

#ifndef LINUDS_INTERFACE_H_
#define LINUDS_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinUDS_Types.h"
#include "LinTrans_Interface.h"
#include "LinSNPD_Interface.h"
#include "LinLookup_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

// 0x0103: modified timer setup 
// 0x0102: added Targeted Reset
// 0x0101: added SNPD
#define LINUDS_INTERFACE_MODULE_API_VERSION      0x0103u


/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ******************************************************************************
 * UDS Layer interface function types
 *******************************************************************************/
 
/** @addtogroup LinUdsIfIfFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Initialization' interface function.
 *
 * @param[in] genericUdsEnvData       Pointer to reserved UDS Layer environment data.
 * @param[in] udsEnvDataSze           Size of the reserved RAM space for UDS environment
 *                                    data.
 * @param[in] udsCbFuns               Pointer to implementations of UDS callbacks.
 * @param[in] genericUdsCbCtxData     Pointer to UDS callback context data.
 * @param[in] invalidReadByIDAnswered Answer to an invalid "ReadById" request or not.
 * @param[in] genericUdsImpCfgData    UDS layer configuration data depending of the
 *                                    implementation.
 *
 * @return                            LIN_TRUE if the initialization was successfully
 *                                    LIN_FALSE if not.
 *
 * Initializes the LIN UDS layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_InitializationIfFun_t) (LinUDSIf_pGenericEnvData_t     genericUdsEnvData,       LinUDSIf_EnvDataSze_t         udsEnvDataSze,
                                                      LinUDSIf_cpCallbackFunctions_t udsCbFuns,               LinUDSIf_pGenericCbCtxData_t  genericUdsCbCtxData,
                                                      Lin_Bool_t                     invalidReadByIDAnswered, LinUDSIf_pGenericImpCfgData_t genericUdsImpCfgData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Task' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 *
 * The Task function has to be called periodically to process scheduled task of
 * the UDS Layer.
 *
 ******************************************************************************/
typedef void (*LinUDSIf_TaskIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData);

/***************************************************************************//**
 * @brief Typedef of LIN UDS layer 'Get Sub-Interface' interface function.
 *
 * @param[in]  genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in]  interfaceId       Sub-Interface ID.
 * @param[out] ifThisPtr         Address (pointer) of an instance of the
 *                               LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_GetSubInterfaceIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Get NAD' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] nadtype           Type of NAD to acquire.
 *
 * @return Desired NAD.
 *
 * Determine the actual NAD configured in the UDS layer.
 *
 ******************************************************************************/
typedef LinUDSIf_NAD_t (*LinUDSIf_GetNADIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_eNADType_t nadtype);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Set NAD' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] nad               NAD to set.
 *
 * Configure the actually used NAD in the UDS layer.
 *
 ******************************************************************************/
typedef void (*LinUDSIf_SetNADIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_NAD_t nad);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Add RBI Table' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] rbiTbl            Pointer to 'RBI Lookup Table'.
 *
 * @return LIN_TRUE if the 'RBI Lookup Table' is correctly defined and
 *         has been successfully added to the UDS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_AddRbiTableIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_pRbiLookupEntry_t rbiTbl);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Remove RBI Table' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] rbiTbl            Pointer to 'RBI Lookup Table'.
 *
 * @return LIN_TRUE if the 'RBI Lookup Table' has been successfully removed
 *         from the UDS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RmvRbiTableIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_pRbiLookupEntry_t rbiTbl);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Add DID Table' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] didTbl            Pointer to 'DID Lookup Table'.
 *
 * @return LIN_TRUE if the 'DID Lookup Table' is correctly defined and
 *         has been successfully added to the UDS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_AddDidTableIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpDidLookupEntry_t didTbl);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Remove DID Table' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] didTbl            Pointer to 'DID Lookup Table'.
 *
 * @return LIN_TRUE if the 'DID Lookup Table' has been successfully removed
 *         from the UDS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RmvDidTableIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpDidLookupEntry_t didTbl);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Add Routine ID Table' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] routineIdTbl      Pointer to 'Routine ID Lookup Table'.
 *
 * @return LIN_TRUE if the 'Routine ID Lookup Table' is correctly defined and
 *         has been successfully added to the UDS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_AddRoutineIdTableIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpRoutineIdLookupEntry_t routineIdTbl);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Remove Routine ID Table' interface function.
 *
 * @param[in] genericUdsEnvData Pointer to reserved UDS Layer environment data.
 * @param[in] routineIdTbl      Pointer to 'Routine ID Lookup Table'.
 *
 * @return LIN_TRUE if the 'Routine ID Lookup Table' has been successfully removed
 *         from the UDS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RmvRoutineIdTableIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpRoutineIdLookupEntry_t routineIdTbl);

/***************************************************************************//**
 * @brief Typedef of UDS layer 'Set Up Timer' interface function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] timerNum            ID of the timer to set.
 * @param[in] timeout             Value for the timeout, if this time has been
 *                                passed the callback will be called.
 * @param[in] genericUdsCbCtxData Pointer to the callback context data, passed
 *                                to the callback function.
 *
 * @return LIN_TRUE if the timer has been configured successfully.
 *
 * Interface function to configure a timer. After the configured timeout has 
 * been passed, a callback will be called.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_SetUpTimerIfFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_TimerNum_t          timerNum,
                                                  LinUDSIf_eTimerType_t type,                   LinUDSIf_Timeout_t         timeout,           
                                                  LinUDSIf_pGenericCbCtxData_t cbCtxData);

/***************************************************************************//**
 * @brief LIN UDS layer interface functions struct.
 *
 * Collective struct for the UDS layer interface functions.
 ******************************************************************************/
struct LinUDSIf_sInterfaceFunctions
{
    Lin_Version_t                     InterfaceVersion;  /**< @brief UDS Layer interface version.*/

    LinUDSIf_InitializationIfFun_t    Initialization;    /**< @brief Pointer to UDS Layer 'Initialization' interface function. (@ref LinUDSIf_InitializationIfFun_t) @copydetails LinUDSIf_InitializationIfFun_t */
    LinUDSIf_TaskIfFun_t              Task;              /**< @brief Pointer to UDS Layer 'Task' interface function. (@ref LinUDSIf_TaskIfFun_t) @copydetails LinUDSIf_TaskIfFun_t */
    LinUDSIf_GetSubInterfaceIfFun_t   GetSubInterface;   /**< @brief Pointer to UDS Layer 'Get Sub-Interface' interface function. (@ref LinUDSIf_GetSubInterfaceIfFun_t) @copydetails LinUDSIf_GetSubInterfaceIfFun_t */
    LinUDSIf_GetNADIfFun_t            GetNAD;            /**< @brief Pointer to UDS Layer 'Get NAD' interface function. (@ref LinUDSIf_GetNADIfFun_t) @copydetails LinUDSIf_GetNADIfFun_t */
    LinUDSIf_SetNADIfFun_t            SetNAD;            /**< @brief Pointer to UDS Layer 'Set NAD' interface function. (@ref LinUDSIf_SetNADIfFun_t) @copydetails LinUDSIf_SetNADIfFun_t */
    LinUDSIf_AddRbiTableIfFun_t       AddRbiTable;       /**< @brief Pointer to UDS Layer 'Add RBI Table' interface function. (@ref LinUDSIf_AddRbiTableIfFun_t) @copydetails LinUDSIf_AddRbiTableIfFun_t */
    LinUDSIf_RmvRbiTableIfFun_t       RmvRbiTable;       /**< @brief Pointer to UDS Layer 'Remove RBI Table' interface function. (@ref LinUDSIf_RmvRbiTableIfFun_t) @copydetails LinUDSIf_RmvRbiTableIfFun_t */
    LinUDSIf_AddDidTableIfFun_t       AddDidTable;       /**< @brief Pointer to UDS Layer 'Add DID Table' interface function. (@ref LinUDSIf_AddDidTableIfFun_t) @copydetails LinUDSIf_AddDidTableIfFun_t */
    LinUDSIf_RmvDidTableIfFun_t       RmvDidTable;       /**< @brief Pointer to UDS Layer 'Remove DID Table' interface function. (@ref LinUDSIf_RmvDidTableIfFun_t) @copydetails LinUDSIf_RmvDidTableIfFun_t */
    LinUDSIf_AddRoutineIdTableIfFun_t AddRoutineIdTable; /**< @brief Pointer to UDS Layer 'Add Routine ID Table' interface function. (@ref LinUDSIf_AddRoutineIdTableIfFun_t) @copydetails LinUDSIf_AddRoutineIdTableIfFun_t */
    LinUDSIf_RmvRoutineIdTableIfFun_t RmvRoutineIdTable; /**< @brief Pointer to UDS Layer 'Remove Routine ID Table' interface function. (@ref LinUDSIf_RmvRoutineIdTableIfFun_t) @copydetails LinUDSIf_RmvRoutineIdTableIfFun_t */
    LinUDSIf_SetUpTimerIfFun_t        SetUpTimer;        /**< @brief Pointer to UDS Layer 'Set Up Timer' interface function. (@ref LinUDSIf_SetUpTimerIfFun_t) @copydetails LinUDSIf_SetUpTimerIfFun_t */
};

/** @}*/ /* LinUdsIfIfFunDefs*/

/* *****************************************************************************
 * UDS Layer callback function types.
 ******************************************************************************/

/** @addtogroup LinUdsIfCbFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Error' callback function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in] error               Occurred error.
 * @param[in] sid                 Related SID if known.
 * @param[in] genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * Indicates an error in the UDS Layer and underlying layers of the LIN
 * Driver.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_errorcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinUDSIf_ErrorCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                       LinUDSIf_Error_t             error,             LinUDSIf_SID_t                  sid,
                                       LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Go To Sleep' callback function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in] genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * This callback is called if the UDS Layer received a Go-To-Sleep frame.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_gotosleepcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinUDSIf_GoToSleepCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                           LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'DID Length' callback function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in] did                 Data Identifier.
 * @param[in] genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * This callback is called if the UDS Layer received a 'Read Data by Identifier', 
 * 'Write Data by Identifier' and 'IO Control' service request and the 'LengthType' 
 * in the DID Lookup Entry in the corresponding table is configured as
 * 'LinDiagIf_DidLenType_Callback'.
 *
 * @return The length of the data length of the response to the corresponding 'Read
 *         Data by Identifier', 'Write Data by Identifier' and 'IO Control'
 *         service requests.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_didlencbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinUDSIf_BufLength_t (*LinUDSIf_DidLenCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                        LinUDSIf_DID_t             did,               LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Diagnostic Session Control' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg   LIN_TRUE if the positive response should
 *                                 be suppressed.
 * @param[in]  diagSessionType     Requested UDS Session.
 * @param[out] pP2SrvMax           P2_Server_Max value for the response. The resolution
 *                                 is 1 ms. Timeout value between an the reception of an
 *                                 UDS request and the corresponding UDS request.
 * @param[out] pP2StarSrvMax       P2*_Server_Max value for the response. The resolution
 *                                 is 10 ms. Timeout value between an the reception of an
 *                                 UDS request, which has been answered with the NRC 0x78
 *                                 (RequestCorrectlyReceived-ResponsePending) and a renewed
 *                                 UDS response possibility.
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in] genericUdsCbCtxData  Pointer to UDS Layer callback context data.
 *
 * This callback is called if the UDS Layer received a 'Diagnostic Session Control'
 * request. A return value of LIN_TRUE means the request will be processed and the
 * UDS session will be initiated. 
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_diagsessionctrlcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_DiagSessionCtrlCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                       Lin_Bool_t                 suppressPosRspMsg, LinUDSIf_DiagnosticSessionType_t diagSessionType,
                                                       LinUDSIf_pP2SrvMax_t       pP2SrvMax,         LinUDSIf_pP2StarSrvMax_t         pP2StarSrvMax,
                                                       LinUDSIf_pNRC_t            pNrc,              LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'ECU Reset' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg   LIN_TRUE if the positive response should
 *                                 be suppressed.
 * @param[in]  resetType           Reset Type (i.e. LINUDSIF_RT_HARD_RESET)
 * @param[out] pPowerDownTime      'Power Down Time', the time need by the server
 *                                 to execute the reset.
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * This callback is called if the UDS Layer received a 'ECU Reset' request. A return 
 * value of LIN_TRUE means the request will be processed and a reset will be 
 * executed after the returned 'Power Down Time' (pPowerDownTime).
 * 
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_ecuresetcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_ECUResetCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                Lin_Bool_t                   suppressPosRspMsg, LinUDSIf_ResetType_t             resetType,
                                                LinUDSIf_pPowerDownTime_t    pPowerDownTime,    LinUDSIf_pNRC_t                  pNrc,
                                                LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Seed Length' callback function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in] secuAcsType         The 'AccessType' from the request. Uneven numbers
 *                                are handled as 'Request Seed Access Types'.
 * @param[out] pSeedLen           Length of the 'Seed' which will be returned in
 *                                he response.
 * @param[in] genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * This callback is called if the UDS Layer received a 'Security Access' request.
 * If the sub-function parameter is an uneven number, it will be handled as 
 * 'Request Seed Access Type' and this callback will be called, to determine the
 * length of the seed, that will be returned.
 *
 * @return LIN_TRUE if the seed length has been written to 'pSeedLen'.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_seedlencbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_SeedLengthCbFun_t) (LinUDSIf_pGenericEnvData_t    genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                  LinUDSIf_SecurityAccessType_t secuAcsType,       LinUDSIf_pBufLength_t           pSeedLen,
                                                  LinUDSIf_pGenericCbCtxData_t  genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Request Seed' callback function.
 *
 * @param[in]  genericUdsEnvData     Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns             Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg     LIN_TRUE if the positive response should
 *                                   be suppressed.
 * @param[in]  secuAcsType           The 'AccessType' from the request. Uneven numbers
 *                                   are handled as 'Request Seed Access Types'.
 * @param[in]  secAcsDataRec         The 'Security Access Data Record', if the 'Security
 *                                   Access Type' is 'requestSeed', it holds additional
 *                                   parameter and it's optional.
 * @param[in]  appendDataToMsgBufFun Function to append the 'Seed' to the message buffer.
 * @param[out] pNrc                  'Negative Response Code' to return, if an error
 *                                   occurred and/or the service can not be processed.
 *                                   This variable will only be evaluated, if LIN_FALSE
 *                                   is returned.
 * @param[in]  genericUdsCbCtxData   Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the seed has been written to the message buffer and can
 *         be send out.
 *
 * This callback is called if the UDS Layer received a 'Security Access' request.
 * If the sub-function parameter is an uneven number, it will be handled as 
 * 'Request Seed Access Type' and this callback will be called, to append the 'Seed'
 * to the response message.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_seedlencbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RequestSeedCbFun_t) (LinUDSIf_pGenericEnvData_t           genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                   Lin_Bool_t                           suppressPosRspMsg, LinUDSIf_SecurityAccessType_t    secuAcsType,
                                                   LinUDSIf_pSecurityAccessDataRecord_t secAcsDataRec,     LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun,
                                                   LinUDSIf_pNRC_t                      pNrc,              LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Send Key' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg   LIN_TRUE if the positive response should
 *                                 be suppressed.
 * @param[in]  secuAcsType         The 'AccessType' from the request. Uneven numbers
 *                                 are handled as 'Request Seed Access Types'.
 * @param[in]  pKey                Pointer to the received key.
 * @param[in]  keyLen               Length of the received key.
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the received key is correct.
 *
 * This callback is called during the 'Security Access' procedure. After the seed 
 * has been send to the client and it answered with its calculated key.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_sendkeycbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_SendKeyCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                               Lin_Bool_t                 suppressPosRspMsg, LinUDSIf_SecurityAccessType_t   secuAcsType,
                                               LinUDSIf_pKey_t            pKey,              LinUDSIf_BufLength_t            keyLen,
                                               LinUDSIf_pNRC_t            pNrc,              LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Communication Control' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg   LIN_TRUE if the positive response should
 *                                 be suppressed.
 * @param[in]  comCtrlType         UDS sub-function parameter. It defines in which
 *                                 way the server should modify his communication
 *                                 behavior/capability.
 *                                 (i.e LINDIAGIF_CTRLTYPE_ERXTX)
 * @param[in]  comType             A bit-encoded parameter. It defines which
 *                                 communication type should be affected by the
 *                                 'comCtrlType' parameter.
 * @param[in]  nin                 Node Identification Number, to address nodes
 *                                 from outside of the sub-net. This parameter is
 *                                 only present if  'comCtrlType' is
 *                                 LINDIAGIF_CTRLTYPE_ERXDTXWEAI (0x04) or
 *                                 LINDIAGIF_CTRLTYPE_ERXTXWEAI (0x05)
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Communication Control'
 * service request. The 'Communication Control' service controls the 
 * transmission capability of certain messages of a server.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_comctrlcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_CommunicationControlCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                            Lin_Bool_t                   suppressPosRspMsg,  LinUDSIf_ComCtrlType_t         comCtrlType,
                                                            LinUDSIf_CommunicationType_t comType,            LinUDSIf_NIN_t                 nin,
                                                            LinUDSIf_pNRC_t              pNrc,               LinUDSIf_pGenericCbCtxData_t   genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Tester Present' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg   LIN_TRUE if the positive response should
 *                                 be suppressed.
 * @param[in]  zeroSubFun          UDS sub-function parameter. It has no purpose
 *                                 in this service.
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Tester Present'
 * service request. The 'Tester Present' service indicates, that a client is
 * still connected to the server and activated services shall stay active.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_testerpresentcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_TesterPresentCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                     Lin_Bool_t                 suppressPosRspMsg, LinUDSIf_ZeroSubFunction_t      zeroSubFun,
                                                     LinUDSIf_pNRC_t            pNrc,              LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'ROE Response Length' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  eventType           UDS sub-function parameter. The 'Event Type'.
 * @param[out] roeRspnsLen         Length of the expected 'Event Type Record'.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the data for the 'Response On Event' service response
 *         has been written to 'roeRspnsLen'.
 *
 * This callback is called if the server received a 'Response On Event' service
 * request and the 'Event Type' is LINUDSIF_ET_RAE or unknown to the the server,
 * so the user can determine the length of the 'eventTypeRecord' parameter on 
 * demand.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_roeresponselencbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RoeResponseLengthCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                         LinUDSIf_EventType_t         eventType,         LinUDSIf_pBufLength_t           roeRspnsLen,
                                                         LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Response On Event' callback function.
 *
 * @param[in]  genericUdsEnvData     Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns             Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg     LIN_TRUE if the positive response should
 *                                   be suppressed.
 * @param[in]  eventType             UDS sub-function parameter. The 'Event Type'.
 * @param[in]  eventWindowTime       The time the event shall be active in the
 *                                   server. Should not be evaluated if the 'eventType'
 *                                   is LINUDSIF_ET_RAE.
 * @param[in]  pEventTypeRecord      Pointer to the 'eventTypeRecord'.
 * @param[in]  eventTypeRecordLen    Length of the 'eventTypeRecord'.
 * @param[in]  pSerToRspdToRcrd      Pointer to the 'serviceToRespondToRecord'.
 * @param[in]  serToRspdToRcrdLen    Length of the 'serviceToRespondToRecord'
 * @param[in]  appendDataToMsgBufFun Function to append data for the response.
 * @param[out] pNrc                  'Negative Response Code' to return, if an error
 *                                   occurred and/or the service can not be processed.
 *                                   This variable will only be evaluated, if LIN_FALSE
 *                                   is returned.
 * @param[in]  genericUdsCbCtxData   Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called if the server received a 'Response On Event' service
 * request. The availability and usability of the parameters depend on the 
 * 'eventType' parameter. Not all parameters hold useful information for every
 * 'eventType'.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_responseoneventcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_ResponseOnEventCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData,  LinUDSIf_cpInterfaceFunctions_t      udsIfFuns,
                                                       Lin_Bool_t                 suppressPosRspMsg,  LinUDSIf_EventType_t                 eventType,
                                                       LinUDSIf_EventWindowTime_t eventWindowTime,    LinUDSIf_pEventTypeRecord_t          pEventTypeRecord,
                                                       LinUDSIf_BufLength_t       eventTypeRecordLen, LinUDSIf_pServiceToRespondToRecord_t pSerToRspdToRcrd,
                                                       LinUDSIf_BufLength_t       serToRspdToRcrdLen, LinUDSIf_AppendDataToMsgBuffer_t     appendDataToMsgBufFun,
                                                       LinUDSIf_pNRC_t            pNrc,               LinUDSIf_pGenericCbCtxData_t         genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Control DTC Setting' callback function.
 *
 * @param[in]  genericUdsEnvData    Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns            Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg    LIN_TRUE if the positive response should
 *                                  be suppressed.
 * @param[in]  dtcSettingType       UDS sub-function parameter. Basically controls
 *                                  weather the diagnostic trouble code status bits
 *                                  update shall be stopped or started. Can also hold
 *                                  specific values not defined in the ISO
 *                                  specification.
 * @param[in]  dtcSetCtrlOptRcrd    Optional parameter record to transfer additional
 *                                  information.
 * @param[in]  dtcSetCtrlOptRcrdLen Length of the 'DTCSettingControlOptionRecord'
 *                                  parameter.
 * @param[out] pNrc                 'Negative Response Code' to return, if an error
 *                                  occurred and/or the service can not be processed.
 *                                  This variable will only be evaluated, if LIN_FALSE
 *                                  is returned.
 * @param[in]  genericUdsCbCtxData  Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Control DTC Setting' service
 * request. To enable or disable the updating of the diagnostic trouble code status
 * bits.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_testerpresentcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_ControlDTCSettingCbFun_t) (LinUDSIf_pGenericEnvData_t    genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                         Lin_Bool_t                    suppressPosRspMsg, LinUDSIf_DTCSettingType_t       dtcSettingType,
                                                         LinUDSIf_pDTCSetCtrlOptRcrd_t dtcSetCtrlOptRcrd, LinUDSIf_BufLength_t            dtcSetCtrlOptRcrdLen,
                                                         LinUDSIf_pNRC_t               pNrc,              LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Read DTC Info Response Length' callback function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in] reportType          UDS sub-function parameter. Type of the report to
 *                                respond with and to determine the length of the 
 *                                data in this response.
 *                                (i.e. LINUDSIF_REPT_RNODTCBSM) 
 * @param[in] genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return The length of the response to the corresponding 'Read DTC Information'
 *         service request.
 *
 * This callback is called after the reception of an 'Control DTC Setting' service
 * request. To enable or disable the updating of the diagnostic trouble code status
 * bits.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_testerpresentcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinUDSIf_BufLength_t (*LinUDSIf_ReadDTCInfoRespLenCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                                    LinUDSIf_DtcReportType_t   reportType,        LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Read DTC Information' callback function.
 *
 * @param[in]  genericUdsEnvData     Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns             Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg     LIN_TRUE if the positive response should
 *                                   be suppressed.
 * @param[in]  reportType            UDS sub-function parameter. Type of the report to
 *                                   respond with. (i.e. LINUDSIF_REPT_RNODTCBSM)
 * @param[in]  pDtcParam             Pointer to the 'Read DTC Information' parameter,
 *                                   which depend on the 'reportType'.
 * @param[in]  dtcParamLen           Length of the 'Read DTC Information' parameter.
 * @param[in]  appendDataToMsgBufFun Function to append data for the response.
 * @param[out] pNrc                  'Negative Response Code' to return, if an error
 *                                   occurred and/or the service can not be processed.
 *                                   This variable will only be evaluated, if LIN_FALSE
 *                                   is returned.
 * @param[in]  genericUdsCbCtxData   Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Read DTC Information' service
 * request. This service gathers diagnostic data and creates different reports about
 * it.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_readdtcinfocbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_ReadDTCInformationCbFun_t) (LinUDSIf_pGenericEnvData_t       genericUdsEnvData,     LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                          Lin_Bool_t                       suppressPosRspMsg,     LinUDSIf_DtcReportType_t        reportType,
                                                          LinUDSIf_pDtcParameter_t         pDtcParam,             LinUDSIf_DtcParamLen_t          dtcParamLen,
                                                          LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun, LinUDSIf_pNRC_t                 pNcr,
                                                          LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Clear Diagnostic Information' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  suppressPosRspMsg   LIN_TRUE if the positive response should
 *                                 be suppressed.
 * @param[in]  groupOfDTC          A 3-byte-value identifying a group of DTCs or
 *                                 one particular DTC.
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Clear Diagnostic Information' 
 * service request. It clears diagnostic trouble code bits.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_clrdiaginfocbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_ClearDiagnosticInformationCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                                  Lin_Bool_t                 suppressPosRspMsg, LinUDSIf_GroupOfDTC_t           groupOfDTC,
                                                                  LinUDSIf_pNRC_t            pNrc,              LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Request Download' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  reqDownloadRecParam Pointer to the struct containing the received
 *                                 download parameter.
 *                                 (See also @ref LinUDSIf_sReqDataTransRecParam)
 * @param[in]  reqDownloadRetParam Pointer to the struct containing where the
 *                                 parameter for the response have to be written
 *                                 to.
 *                                 (See also @ref LinUDSIf_sReqDataTransRetParam)
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Request Download' service 
 * request. This service transfers data from the client to the server. I.e.
 * flash software onto the ECU.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_uploaddownloadfunctionalunit.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RequestDownloadCbFun_t) (LinUDSIf_pGenericEnvData_t       genericUdsEnvData,   LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                       LinUDSIf_pReqDataTransRecParam_t reqDownloadRecParam, LinUDSIf_pReqDataTransRetParam_t reqDownloadRetParam,
                                                       LinUDSIf_pNRC_t                  pNrc,                LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Request Upload' callback function.
 *
 * @param[in]  genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in]  reqUploadRecParam   Pointer to the struct containing the received
 *                                 download parameter.
 *                                 (See also @ref LinUDSIf_sReqDataTransRecParam)
 * @param[in]  reqUploadRetParam   Pointer to the struct containing where the
 *                                 parameter for the response have to be written
 *                                 to.
 *                                 (See also @ref LinUDSIf_sReqDataTransRetParam)
 * @param[out] pNrc                'Negative Response Code' to return, if an error
 *                                 occurred and/or the service can not be processed.
 *                                 This variable will only be evaluated, if LIN_FALSE
 *                                 is returned.
 * @param[in]  genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Request Upload' service 
 * request. This service transfers data from the server to the client. I.e.
 * read out memory.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_requploadcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RequestUploadCbFun_t) (LinUDSIf_pGenericEnvData_t       genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                     LinUDSIf_pReqDataTransRecParam_t reqUploadRecParam, LinUDSIf_pReqDataTransRetParam_t reqUploadRetParam,
                                                     LinUDSIf_pNRC_t                  pNrc,              LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Transfer Response Parameter Record Length' 
 *        callback function.
 *
 * @param[in]  genericUdsEnvData     Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns             Pointer to the UDS Layer interface function struct.
 * @param[in]  transDirection        Direction of the data transfer. Upload or Download.
 * @param[out] transRespParamRecLen  Pointer to the the variable to write the response
 *                                   data length to.
 * @param[in]  genericUdsCbCtxData   Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if he response data length has been written to
           'transRespParamRecLen'
 *
 * This callback is called after the reception of an 'Transfer Data' and 'Request 
 * Transfer Exit' service request. It determines the data length in the response 
 * message.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_uploaddownloadfunctionalunit.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_TransRespParamRcrdLenCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                             LinUDSIf_eTransReqType_t     transDirection,    LinUDSIf_pTransParamRecLen_t    transRespParamRecLen,
                                                             LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Transfer Data' callback function.
 *
 * @param[in]  genericUdsEnvData     Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns             Pointer to the UDS Layer interface function struct.
 * @param[in]  blkSeqCnt             Block sequence counter of the actual data transfer.
 * @param[in]  transParamRec         Only valid for a data download. The struct contains
 *                                   the pointer to the received data and the length.
 * @param[in]  appendDataToMsgBufFun Function to append data for the response.
 * @param[out] pNrc                  'Negative Response Code' to return, if an error
 *                                   occurred and/or the service can not be processed.
 *                                   This variable will only be evaluated, if LIN_FALSE
 *                                   is returned.
 * @param[in]  genericUdsCbCtxData   Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Transfer Data' service 
 * request. The 'transParamRec' struct contains the pointer to the received data
 * and it's length. During an data download, the data pointer is NULL and the
 * length zero. In both cases the data for the response is appended via the
 * 'appendDataToMsgBufFun' parameter function. I.e. for an data download it
 * could be a checksum and for an upload requested data content.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_uploaddownloadfunctionalunit.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_TransferDataCbFun_t) (LinUDSIf_pGenericEnvData_t       genericUdsEnvData,     LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                    LinUDSIf_BlkSeqCnt_t             blkSeqCnt,             LinUDSIf_sTransParamRec_t       transParamRec,
                                                    LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun, LinUDSIf_pNRC_t                 pNrc,
                                                    LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Request Transfer Exit' callback function.
 *
 * @param[in]  genericUdsEnvData     Pointer to reserved UDS Layer environment data.
 * @param[in]  udsIfFuns             Pointer to the UDS Layer interface function struct.
 * @param[in]  transReqParamRec      Struct containing the pointer to the
 *                                   'transferRequestParameterRecord' data and its
 *                                   length.
 * @param[in]  appendDataToMsgBufFun Function to append data for the response.
 * @param[out] pNrc                  'Negative Response Code' to return, if an error
 *                                   occurred and/or the service can not be processed.
 *                                   This variable will only be evaluated, if LIN_FALSE
 *                                   is returned.
 * @param[in]  genericUdsCbCtxData   Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the service has been successfully processed.
 *
 * This callback is called after the reception of an 'Request Transfer Exit' service 
 * request. This service terminates a data transfer.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_uploaddownloadfunctionalunit.dox
 * @n
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RequestTransferExitCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                           LinUDSIf_sTransParamRec_t  transReqParamRec,  LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun,
                                                           LinUDSIf_pNRC_t            pNrc,              LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Timeout' callback function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in] timerNum            Timer index.
 * @param[in] genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * This callback is called after the configured timeout has timed out.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_timeoutcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinUDSIf_TimeoutCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                         LinUDSIf_TimerNum_t        timerNum,          LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Store NAD' callback function.
 *
 * @param[in] genericUdsEnvData   Pointer to reserved UDS Layer environment data.
 * @param[in] udsIfFuns           Pointer to the UDS Layer interface function struct.
 * @param[in] nad                 NAD to store.
 * @param[in] genericUdsCbCtxData Pointer to UDS Layer callback context data.
 *
 * This callback is called if the 'Store NAD' command has been send during
 * auto adressing.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_storenadcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinUDSIf_StoreNADCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                          LinUDSIf_NAD_t             nad,               LinUDSIf_pGenericCbCtxData_t    genericUdsCbCtxData);


/***************************************************************************//**
 * @brief Typedef of UDS Layer 'TargetedReset' callback function.
 *
 * @param genericUDSEnvData[in]   Pointer to reserved DIAG Layer environment data.
 * @param diagIfFuns[in]           Pointer to the DIAG Layer interface function struct.
 * @param genericDiagCbCtxData[in] Pointer to DIAG Layer callback context data.
 * @return Should be LIN_TRUE, if the reset was successfully executed. Otherwise LIN_FALSE
 *
 * This callback is called if the 'TargetedReset' SID has been send (LIN SAE-J2602)
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_TargetedResetFun_t) (LinUDSIf_pGenericEnvData_t genericDiagEnvData, LinUDSIf_cpInterfaceFunctions_t diagIfFuns,
                                                    LinTransIf_ErrorFlag_t errorFlag            , LinUDSIf_pGenericCbCtxData_t    genericDiagCbCtxData);


/***************************************************************************//**
 * @brief LIN UDS Layer callback functions struct.
 *
 * The set of callbacks which should be invoked in the user application.
 ******************************************************************************/
struct LinUDSIf_sCallbackFunctions
{
    Lin_Version_t                              CallbackVersion;            /**< @brief Callback Version */

    LinUDSIf_GoToSleepCbFun_t                  GoToSleep;                  /**< @brief Pointer to UDS Layer 'Go To Sleep' callback function. (@ref LinUDSIf_GoToSleepCbFun_t) @copydetails LinUDSIf_GoToSleepCbFun_t */
    LinUDSIf_ErrorCbFun_t                      Error;                      /**< @brief Pointer to UDS Layer 'Error' callback function. (@ref LinUDSIf_ErrorCbFun_t) @copydetails LinUDSIf_ErrorCbFun_t */
    LinUDSIf_DiagSessionCtrlCbFun_t            SessionControl;             /**< @brief Pointer to UDS Layer 'Diagnostic Session Control' callback function. (@ref LinUDSIf_DiagSessionCtrlCbFun_t) @copydetails LinUDSIf_DiagSessionCtrlCbFun_t */
    LinUDSIf_ECUResetCbFun_t                   ECUReset;                   /**< @brief Pointer to UDS Layer 'ECU Reset' callback function. (@ref LinUDSIf_ECUResetCbFun_t) @copydetails LinUDSIf_ECUResetCbFun_t */
    LinUDSIf_SeedLengthCbFun_t                 SeedLength;                 /**< @brief Pointer to UDS Layer 'Seed Length' callback function. (@ref LinUDSIf_SeedLengthCbFun_t) @copydetails LinUDSIf_SeedLengthCbFun_t */
    LinUDSIf_RequestSeedCbFun_t                RequestSeed;                /**< @brief Pointer to UDS Layer 'Request Seed' callback function. (@ref LinUDSIf_RequestSeedCbFun_t) @copydetails LinUDSIf_RequestSeedCbFun_t */
    LinUDSIf_SendKeyCbFun_t                    SendKey;                    /**< @brief Pointer to UDS Layer 'Send Key' callback function. (@ref LinUDSIf_SendKeyCbFun_t) @copydetails LinUDSIf_SendKeyCbFun_t */
    LinUDSIf_CommunicationControlCbFun_t       CommunicationControl;       /**< @brief Pointer to UDS Layer 'Communication Control' callback function. (@ref LinUDSIf_CommunicationControlCbFun_t) @copydetails LinUDSIf_CommunicationControlCbFun_t */
    LinUDSIf_TesterPresentCbFun_t              TesterPresent;              /**< @brief Pointer to UDS Layer 'Tester Present' callback function. (@ref LinUDSIf_TesterPresentCbFun_t) @copydetails LinUDSIf_TesterPresentCbFun_t */
    LinUDSIf_RoeResponseLengthCbFun_t          RoeResponseLength;          /**< @brief Pointer to UDS Layer 'ROE Response Length' callback function. (@ref LinUDSIf_RoeResponseLengthCbFun_t) @copydetails LinUDSIf_RoeResponseLengthCbFun_t */
    LinUDSIf_ResponseOnEventCbFun_t            ResponseOnEvent;            /**< @brief Pointer to UDS Layer 'Response On Event' callback function. (@ref LinUDSIf_ResponseOnEventCbFun_t) @copydetails LinUDSIf_ResponseOnEventCbFun_t */
    LinUDSIf_ControlDTCSettingCbFun_t          ControlDTCSetting;          /**< @brief Pointer to UDS Layer 'Control DTC Setting' callback function. (@ref LinUDSIf_ControlDTCSettingCbFun_t) @copydetails LinUDSIf_ControlDTCSettingCbFun_t */
    LinUDSIf_ReadDTCInfoRespLenCbFun_t         ReadDTCInfoRespLen;         /**< @brief Pointer to UDS Layer 'Read DTC Info Response Length' callback function. (@ref LinUDSIf_ReadDTCInfoRespLenCbFun_t) @copydetails LinUDSIf_ReadDTCInfoRespLenCbFun_t */
    LinUDSIf_ReadDTCInformationCbFun_t         ReadDTCInformation;         /**< @brief Pointer to UDS Layer 'Read DTC Information' callback function. (@ref LinUDSIf_ReadDTCInformationCbFun_t) @copydetails LinUDSIf_ReadDTCInformationCbFun_t */
    LinUDSIf_ClearDiagnosticInformationCbFun_t ClearDiagnosticInformation; /**< @brief Pointer to UDS Layer 'Clear Diagnostic Information' callback function. (@ref LinUDSIf_ClearDiagnosticInformationCbFun_t) @copydetails LinUDSIf_ClearDiagnosticInformationCbFun_t */
    LinUDSIf_RequestDownloadCbFun_t            RequestDownload;            /**< @brief Pointer to UDS Layer 'Request Download' callback function. (@ref LinUDSIf_RequestDownloadCbFun_t) @copydetails LinUDSIf_RequestDownloadCbFun_t */
    LinUDSIf_RequestUploadCbFun_t              RequestUpload;              /**< @brief Pointer to UDS Layer 'Request Upload' callback function. (@ref LinUDSIf_RequestUploadCbFun_t) @copydetails LinUDSIf_RequestUploadCbFun_t */
    LinUDSIf_TransRespParamRcrdLenCbFun_t      TransRespParamRcrdLen;      /**< @brief Pointer to UDS Layer 'Transfer Response Parameter Record Length' callback function. (@ref LinUDSIf_TransRespParamRcrdLenCbFun_t) @copydetails LinUDSIf_TransRespParamRcrdLenCbFun_t */
    LinUDSIf_TransferDataCbFun_t               TransferData;               /**< @brief Pointer to UDS Layer 'Transfer Data' callback function. (@ref LinUDSIf_TransferDataCbFun_t) @copydetails LinUDSIf_TransferDataCbFun_t */
    LinUDSIf_RequestTransferExitCbFun_t        RequestTransferExit;        /**< @brief Pointer to UDS Layer 'Request Transfer Exit' callback function. (@ref LinUDSIf_RequestTransferExitCbFun_t) @copydetails LinUDSIf_RequestTransferExitCbFun_t */
    LinUDSIf_TimeoutCbFun_t                    Timeout;                    /**< @brief Pointer to UDS Layer 'Timeout' callback function. (@ref LinUDSIf_TimeoutCbFun_t) @copydetails LinUDSIf_TimeoutCbFun_t */
    LinUDSIf_StoreNADCbFun_t                   StoreNAD;                   /**< @brief Pointer to UDS Layer 'Store NAD' callback function. (@ref LinUDSIf_StoreNADCbFun_t) @copydetails LinUDSIf_StoreNADCbFun_t */
    LinUDSIf_CheckSNPDMethodIdCbFun_t          CheckSNPDMethod;            /**< @brief Pointer to UDS Layer 'CheckSNPDMethod' callback function. (@ref LinUDSIf_CheckSNPDMethodIdCbFun_t) @copydetails LinUDSIf_CheckSNPDMethodIdCbFun_t */
    LinUDSIf_TargetedResetFun_t                TargetedReset;              /**< @brief Pointer to UDS Layer 'TargetedReset' callback function. (@ref LinDiagIf_ResetCbFun_t) @copydetails LinUDSIf_TargetedResetCbFun_t */

};

/** @}*/ /* LinUdsIfCbFunDefs*/

/***************************************************************************//**
 * @brief Object-like This-pointer to connect the UDS Layer to other layers.
 ******************************************************************************/
struct LinUDSIf_sThis
{
    LinUDSIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the UDS Layer interface function struct. (@ref LinUDSIf_sInterfaceFunctions) */
    LinUDSIf_pGenericEnvData_t      EnvData;   /**< @brief Pointer to reserved UDS Layer environment data. */
};

/***************************************************************************//**
 * @brief LIN UDS Layer interface configuration parameter.
 *
 * Data needed for initialization of the UDS Layer.
 ******************************************************************************/
struct LinUDSIf_sInitParam
{
  LinUDSIf_cpInterfaceFunctions_t        IfFunsTbl;            /**< Pointer to the constant UDS Layer interface function struct. */
  LinUDSIf_pGenericEnvData_t             EnvData;              /**< Pointer to reserved UDS Layer environment data. */
  LinUDSIf_EnvDataSze_t                  EnvDataLen;           /**< Size of the reserved RAM for UDS Layer environment data. */
  LinProtoIf_pGenericFrmDescLstEnvData_t FrmDescLstEnvData;    /**< Pointer to the PROTO layer environment data 'Frame Description List'. */
  LinProtoIf_EnvDataSze_t                FrmDescLstEnvDataSze; /**< PROTO layer 'Frame Description List' environment data length. */
  LinUDSIf_NIN_t                         NIN;                  /**< NIN (Node Identification Number) */
  LinUDSIf_pGenericImpCfgData_t          ImpCfgData;           /**< Pointer to implementation dependent configuration data for the UDS Layer. (LinUDSImp_sCfgData) */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINUDS_INTERFACE_H_ */

/*! @} */ /* DIAG_UDS */
/*! @} */ /* ELMOS_LIN_DRIVER */
