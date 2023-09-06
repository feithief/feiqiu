/***************************************************************************//**
 * @file		LinDiag_Interface.h
 *
 * @creator		sbai
 * @created		25.03.2015
 *
 * @brief  		Definition of the interface for the 'LIN DIAG Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DIAG DIAG Layer
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
 * ----------------------------------------------------------------------------- 
 * Date      Version    Who   Comment
 * YY/MM/DD  Ma.Mi.Bd
 * ------------------------------------------------------------------------------
 * 21/11/23  1.0.5   POE  Added DataDump cb to callback table.
 * ------------------------------------------------------------------------------
 ******************************************************************************/

#ifndef LINDIAG_INTERFACE_H_
#define LINDIAG_INTERFACE_H_


/* PRQA S 1534 EOF #justification: This header file is a common file for many projects.
   So it is ok that some types and macros are defined that are not used in this project scope. */
/* Rule: 1534 MISRA a Rule-2.5
   Circumstances: The macro '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */


/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinDiag_Types.h"
#include "LinTrans_Interface.h"
#include "LinSNPD_Interface.h"
#include "LinLookup_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

// 0x0105: added SaveConfiguration Callback
// 0x0104: added TargetedReset
// 0x0103: added CheckSNPDMethod
#define LINDIAG_INTERFACE_MODULE_API_VERSION      0x0106u

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* *****************************************************************************
 * Interface callback function types
 ******************************************************************************/
 
/** @addtogroup LinDiagIfIfFunDefs */
/** @{*/


/***************************************************************************//**
 * @brief Typedef of DIAG layer 'Initialization' interface function.
 *
 * @param[in] genericDiagEnvData      Pointer to reserved DIAG Layer environment data.
 * @param[in] diagEnvDataSze          Size of the reserved RAM space for DIAG environment
 *                                    data.
 * @param[in] diagCbFuns              Pointer to implementations of DIAG callbacks.
 * @param[in] genericDiagCbCtxData    Pointer to DIAG callback context data.
 * @param[in] invalidReadByIDAnswered Answer to an invalid "ReadById" request or not.
 * @param[in] genericDiagImpCfgData   DIAG layer configuration data depending of the
 *                                    implementation.
 *
 * @return                            LIN_TRUE if the initialization was successfully
 *                                    LIN_FALSE if not.
 *
 * Initializes the LIN DIAG layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_InitializationIfFun_t) (LinDiagIf_pGenericEnvData_t     genericDiagEnvData,      LinDiagIf_EnvDataSze_t         diagEnvDataSze,
                                                       LinDiagIf_cpCallbackFunctions_t diagCbFuns,              LinDiagIf_pGenericCbCtxData_t  genericDiagCbCtxData,
                                                       Lin_Bool_t                      invalidReadByIDAnswered, LinDiagIf_pGenericImpCfgData_t genericDiagImpCfgData);

/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'Task' interface function.
 *
 * @param[in] genericDiagEnvData   Pointer to reserved DIAG Layer environment data.
 *
 * The Task function has to be called periodically to process scheduled task of
 * the DIAG Layer.
 *
 ******************************************************************************/
typedef void (*LinDiagIf_TaskIfFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData);

/***************************************************************************//**
 * @brief Typedef of LIN DIAG layer 'Get Sub-Interface' interface function.
 *
 * @param[in]  genericDiagEnvData Pointer to reserved DIAG Layer environment data.
 * @param[in]  interfaceId        Sub-Interface ID.
 * @param[out] ifThisPtr          Address (pointer) of an instance of the
 *                                LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_GetSubInterfaceIfFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of DIAG layer 'Get NAD' interface function.
 *
 * @param[in] genericDiagEnvData Pointer to reserved DIAG Layer environment data.
 * @param[in] nadtype            Type of NAD to acquire.
 *
 * @return Desired NAD.
 *
 * Determine the actual NAD configured in the DIAG layer.
 *
 ******************************************************************************/
typedef LinDiagIf_NAD_t (*LinDiagIf_GetNADIfFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_eNADType_t nadtype);

/***************************************************************************//**
 * @brief Typedef of DIAG layer 'Set NAD' interface function.
 *
 * @param[in] genericDiagEnvData Pointer to reserved DIAG Layer environment data.
 * @param[in] nad                NAD to set.
 *
 * Configure the actually used NAD in the DIAG layer.
 *
 ******************************************************************************/
typedef void (*LinDiagIf_SetNADIfFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_NAD_t nad);

/***************************************************************************//**
 * @brief Typedef of DIAG layer 'Add RBI Table' interface function.
 *
 * @param[in] genericDiagEnvData Pointer to reserved DIAG Layer environment data.
 * @param[in] rbiTbl             Pointer to 'RBI Lookup Table'.
 *
 * @return LIN_TRUE if the 'RBI Lookup Table' is correctly defined and
 *         has been successfully added to the DIAG Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_AddRbiTableIfFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_pRbiLookupEntry_t rbiTbl);

/***************************************************************************//**
 * @brief Typedef of DIAG layer 'Remove RBI Table' interface function.
 *
 * @param[in] genericDiagEnvData Pointer to reserved DIAG Layer environment data.
 * @param[in] rbiTbl             Pointer to 'RBI Lookup Table'.
 *
 * @return LIN_TRUE if the 'RBI Lookup Table' has been successfully removed
 *         from the DIAG Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_RmvRbiTableIfFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_pRbiLookupEntry_t rbiTbl);

/***************************************************************************//**
 * @brief LIN DIAG layer interface functions struct.
 *
 * Collective struct for the DIAG layer interface functions.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This interface functions can be dereference outside of this project scope. It should not be hidden. */
struct LinDiagIf_sInterfaceFunctions
{
  Lin_Version_t                      InterfaceVersion;  /**< @brief DIAG Layer interface version.*/

  LinDiagIf_InitializationIfFun_t    Initialization;    /**< @brief Pointer to DIAG Layer 'Initialization' interface function. (@ref LinDiagIf_InitializationIfFun_t) @copydetails LinDiagIf_InitializationIfFun_t */
  LinDiagIf_TaskIfFun_t              Task;              /**< @brief Pointer to DIAG Layer 'Task' interface function. (@ref LinDiagIf_TaskIfFun_t) @copydetails LinDiagIf_TaskIfFun_t */
  LinDiagIf_GetSubInterfaceIfFun_t   GetSubInterface;   /**< @brief Pointer to DIAG Layer 'Get Sub-Interface' interface function. (@ref LinDiagIf_GetSubInterfaceIfFun_t) @copydetails LinDiagIf_GetSubInterfaceIfFun_t */
  LinDiagIf_GetNADIfFun_t            GetNAD;            /**< @brief Pointer to DIAG Layer 'Get NAD' interface function. (@ref LinDiagIf_GetNADIfFun_t) @copydetails LinDiagIf_GetNADIfFun_t */
  LinDiagIf_SetNADIfFun_t            SetNAD;            /**< @brief Pointer to DIAG Layer 'Set NAD' interface function. (@ref LinDiagIf_SetNADIfFun_t) @copydetails LinDiagIf_SetNADIfFun_t */
  LinDiagIf_AddRbiTableIfFun_t       AddRbiTable;       /**< @brief Pointer to DIAG Layer 'Add RBI Table' interface function. (@ref LinDiagIf_AddRbiTableIfFun_t) @copydetails LinDiagIf_AddRbiTableIfFun_t */
  LinDiagIf_RmvRbiTableIfFun_t       RmvRbiTable;       /**< @brief Pointer to DIAG Layer 'Remove RBI Table' interface function. (@ref LinDiagIf_RmvRbiTableIfFun_t) @copydetails LinDiagIf_RmvRbiTableIfFun_t */
};

/** @}*/ /* LinDiagIfIfFunDefs */

/* *****************************************************************************
 * LIN DIAG Layer Module callback function types
 ******************************************************************************/

/** @addtogroup LinDiagIfCbFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'Error' callback function.
 *
 * @param[in] genericUdsEnvData     Pointer to reserved DIAG Layer environment data.
 * @param[in] diagIfFuns            Pointer to the DIAG Layer interface function struct.
 * @param[in] error                 Occurred error.
 * @param[in] sid             	    Related SID if known.
 * @param[in] genericDiagCbCtxData  Pointer to DIAG Layer callback context data.
 *
 * Indicates an error in the DIAG Layer and underlying layers of the LIN
 * Driver.
 *
 * @par "Call Description:"
 * @mscfile msc_diag_errorcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinDiagIf_ErrorCbFun_t) ( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                         LinDiagIf_cpInterfaceFunctions_t diagIfFuns,
                                         LinDiagIf_Error_t error,
                                         LinDiagIf_SID_t sid,
                                         LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData );

/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'Go To Sleep' callback function.
 *
 * @param[in] genericUdsEnvData     Pointer to reserved DIAG Layer environment data.
 * @param[in] diagIfFuns            Pointer to the DIAG Layer interface function struct.
 * @param[in] genericDiagCbCtxData  Pointer to DIAG Layer callback context data.
 *
 * This callback is called if the DIAG Layer received a Go-To-Sleep frame.
 *
 * @par "Call Description:"
 * @mscfile msc_diag_gotosleepcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinDiagIf_GoToSleepCbFun_t) ( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                             LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                                             LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData) ;

/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'Timeout' callback function.
 *
 * @param[in] genericDiagEnvData  Pointer to reserved DIAG Layer environment data.
 * @param[in] diagIfFuns          Pointer to the DIAG Layer interface function struct.
 * @param[in] timerNum            Timer index.
 * @param[in] genericUdsCbCtxData Pointer to DIAG Layer callback context data.
 *
 * This callback is called after the configured timeout has timed out.
 *
 * @par "Call Description:"
 * @mscfile msc_diag_timeoutcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinDiagIf_TimeoutCbFun_t) ( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                           LinDiagIf_cpInterfaceFunctions_t diagIfFuns,
                                           Lin_uint8_t timerNum,           
                                           LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData );

/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'Store NAD' callback function.
 *
 * @param[in] genericDiagEnvData   Pointer to reserved DIAG Layer environment data.
 * @param[in] diagIfFuns           Pointer to the DIAG Layer interface function struct.
 * @param[in] nad                  NAD to store.
 * @param[in] genericDiagCbCtxData Pointer to DIAG Layer callback context data.
 *
 * This callback is called if the 'Store NAD' command has been send during
 * auto adressing.
 *
 * @par "Call Description:"
 * @mscfile msc_diag_storenadcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinDiagIf_StoreNADCbFun_t) ( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                            LinDiagIf_cpInterfaceFunctions_t diagIfFuns,
                                            LinDiagIf_NAD_t nad,                
                                            LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData );


/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'Reset' callback function.
 *
 * @param genericDiagEnvData[in]   Pointer to reserved DIAG Layer environment data.
 * @param diagIfFuns[in]           Pointer to the DIAG Layer interface function struct.
 * @param genericDiagCbCtxData[in] Pointer to DIAG Layer callback context data.
 * @return Should be LIN_TRUE, if the reset was successfully executed. Otherwise LIN_FALSE
 *
 * This callback is called if the 'Reset' command has been send eg. during "Targeted Reset" of LIN SAE-J2602
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_TargetedResetFun_t) ( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                                     LinDiagIf_cpInterfaceFunctions_t diagIfFuns,
                                                     LinTransIf_Error_t errorCode, 
                                                     LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);

/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'SaveConfiguration' callback function.
 *
 * @param genericDiagEnvData[in]   Pointer to reserved DIAG Layer environment data.
 * @param diagIfFuns[in]           Pointer to the DIAG Layer interface function struct.
 * @param genericDiagCbCtxData[in] Pointer to DIAG Layer callback context data.

 * @return Should be LIN_TRUE, if the configuration save has been triggered properly and a positive response should be send. 
 *         if LIN_FALSE is returned no response will be send. 
 *
 * This callback is called if the 'SaveConfiguration' command has been send.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_SaveConfigurationCbFun_t) ( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                                           LinDiagIf_cpInterfaceFunctions_t diagIfFuns,
                                                           LinTransIf_Error_t errorCode, 
                                                           LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);

/***************************************************************************//**
 * @brief Typedef of DIAG Layer 'Data Dump' callback function.
 *
 * @param genericDiagEnvData[in]   Pointer to reserved DIAG Layer environment data.
 * @param diagIfFuns[in]           Pointer to the DIAG Layer interface function struct.
 * @param genericDiagCbCtxData[in] Pointer to DIAG Layer callback context data.

 * @return Should be LIN_TRUE, if the data dump has been triggered properly and a positive response should be send. 
 *         if LIN_FALSE is returned no response will be send. 
 *
 * This callback is called if the 'Data Dump' command has been send.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_DataDumpCbFun_t) ( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                                  LinDiagIf_cpInterfaceFunctions_t diagIfFuns,
                                                  LinTransIf_Error_t errorCode, 
                                                  LinDiagIf_pDataDumpData_t dataDumpReqBuffer, 
                                                  LinDiagIf_pDataDumpData_t dataDumpRespBuffer,
                                                  LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);


/***************************************************************************//**
 * @brief LIN DIAG Layer callback functions struct.
 *
 * The set of callbacks which should be invoked in the user application.
 ******************************************************************************/
struct LinDiagIf_sCallbackFunctions
{
  Lin_Version_t                      CallbackVersion;   /**< @brief Callback Version */

  LinDiagIf_GoToSleepCbFun_t         GoToSleep;         /**< @brief Pointer to DIAG Layer 'Go To Sleep' callback function. (@ref LinDiagIf_GoToSleepCbFun_t) @copydetails LinDiagIf_GoToSleepCbFun_t */
  LinDiagIf_ErrorCbFun_t             Error;             /**< @brief Pointer to DIAG Layer 'Error' callback function. (@ref LinDiagIf_ErrorCbFun_t) @copydetails LinDiagIf_ErrorCbFun_t */
  LinDiagIf_TimeoutCbFun_t           Timeout;           /**< @brief Pointer to DIAG Layer 'Timeout' callback function. (@ref LinDiagIf_TimeoutCbFun_t) @copydetails LinDiagIf_TimeoutCbFun_t */
  LinDiagIf_StoreNADCbFun_t          StoreNAD;          /**< @brief Pointer to DIAG Layer 'Store NAD' callback function. (@ref LinDiagIf_StoreNADCbFun_t) @copydetails LinDiagIf_StoreNADCbFun_t */
  LinDiagIf_CheckSNPDMethodIdCbFun_t CheckSNPDMethod;   /**< @brief Pointer to DIAG Layer 'CheckSNPDMethod' callback function. (@ref LinDiagIf_CheckSNPDMethodIdCbFun_t) @copydetails LinDiagIf_CheckSNPDMethodIdCbFun_t */
  LinDiagIf_TargetedResetFun_t       TargetedReset;     /**< @brief Pointer to DIAG Layer 'TargetedReset' callback function. (@ref LinDiagIf_TargetedResetFun_t) @copydetails LinDiagIf_TargetedResetFun_t */
  LinDiagIf_SaveConfigurationCbFun_t SaveConfiguration; /**< @brief Pointer to DIAG Layer 'SaveConfiguration' callback function. (@ref LinDiagIf_SaveConfigurationCbFun_t) @copydetails LinDiagIf_SaveConfigurationCbFun_t */
  LinDiagIf_DataDumpCbFun_t          DataDump;          /**< @brief Pointer to DIAG Layer 'Data Dump' callback function. (@ref LinDiagIf_DataDumpCbFun_t) @copydetails LinDiagIf_DataDumpCbFun_t */
};

/** @}*/ /* LinDiagIfCbFunDefs */

/***************************************************************************//**
 * @brief Object-like This-pointer to connect the DIAG Layer to other layers.
 ******************************************************************************/
struct LinDiagIf_sThis
{
    LinDiagIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the DIAG Layer interface function struct. (@ref LinDiagIf_sInterfaceFunctions) */
    LinDiagIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved DIAG Layer environment data. */
};

/***************************************************************************//**
 * @brief LIN DIAG Layer interface configuration parameter.
 *
 * Data needed for initialization of the DIAG Layer.
 ******************************************************************************/
struct LinDiagIf_sInitParam
{
    LinDiagIf_cpInterfaceFunctions_t       IfFunsTbl;            /**< @brief Pointer to the constant DIAG Layer interface function struct. */
    LinDiagIf_pGenericEnvData_t            envData;              /**< @brief Pointer to reserved DIAG Layer environment data. */
    LinDiagIf_Length_t                     EnvDataLen;           /**< @brief Size of the reserved RAM for DIAG Layer environment data. */
    LinProtoIf_pGenericFrmDescLstEnvData_t FrmDescLstEnvData;    /**< Pointer to the PROTO layer environment data 'Frame Description List'. */
    LinProtoIf_EnvDataSze_t                FrmDescLstEnvDataSze; /**< PROTO layer 'Frame Description List' environment data length. */
    LinDiagIf_pGenericImpCfgData_t         ImpCfgData;           /**< @brief Pointer to implementation dependent configuration data for the DIAG Layer. (LinDiagImp_sCfgData) */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINDIAG_INTERFACE_H_ */

/*! @} */ /* DIAG */
/*! @} */ /* ELMOS_LIN_DRIVER */
