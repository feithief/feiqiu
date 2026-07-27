/***************************************************************************//**
 * @file			LinTrans_Implementation.h
 *
 * @creator		sbai
 * @created		13.02.2015
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief       Implements the LIN Transport Layer
 *
 * @purpose     -Processes the incoming Protocol Control Information (PCI) and
 *               the NAD of the Transport Layer PDU
 *              -Implements the  Transport Layer State machine for requests and responses
 *              -Handles Single Frame, First Frame and Consecutive Frame
 *              -Implements processed Frames callbacks
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

#ifndef LINTRANS_IMPLEMENTATION_H_
#define LINTRANS_IMPLEMENTATION_H_

/* PRQA S 1534 EOF #justification: This header file is a common file for many projects.
   So it is ok that some macros are defined that are not used in this project scope. */
/* Rule: 1534 MISRA a Rule-2.5
   Circumstances: The macro '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */
/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinProto_Interface.h"
#include "LinTrans_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINTRANSIMP_VERSION                       0x0111u  /**< LIN TRANS implementation version. */

#define LINTRANSIMP_CONFIG_DATA_VERSION           0x0100u  /**< Expected config data version. */

#define LINTRANSIMP_NUMBER_OF_FRAME_DESCRIPTIONS  2u  /**< Count of used 'Frame Description Lists' of PROTO LAYER in LIN TRANS Layer. */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * Struct for TRANS layer implementation specific configuration data.
 ******************************************************************************/
struct LinTransImp_sCfgData
{
  Lin_Version_t       Version;                /**< Config data struct version. */

  LinProtoIf_sThis_t  LinProtoInsThisPointer; /**< @copydoc LinProtoIf_sThis_t */
};

/***************************************************************************//**
 * Typedef of LinTransImp_sCfgData
 ******************************************************************************/
typedef struct LinTransImp_sCfgData    LinTransImp_sCfgData_t; /**< Typedef of LinTransImp_sCfgData. */
/***************************************************************************//**
 * Typedef of pointer to LinTransImp_sCfgData
 ******************************************************************************/
typedef        LinTransImp_sCfgData_t* LinTransImp_pCfgData_t; /**< Typedef of pointer to LinTransImp_sCfgData. */
typedef  const LinTransImp_sCfgData_t* LinTransImp_cpCfgData_t; /**< Typedef of pointer to LinTransImp_sCfgData. */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#if LINTRANSIMP_EXT_IFFUN_STRCT_ACCESS == 1
extern const LinTransIf_sInterfaceFunctions_t LinTransImp_InterfaceFunctions;
extern const LinTransIf_sResponseSubInterfaceFunctions_t LinTransImp_ResponseSubInterfaceFunctions;
#endif /* LINTRANSIMP_EXT_IFFUN_STRCT_ACCESS == 1 */

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/***************************************************************************//**
 * LIN TRANS layer 'Initialization' function.
 *
 * @param[in] genericTransEnvData     Pointer to reserved TRANS Layer environment data.
 * @param[in] transEnvDataSze         Size of the reserved RAM for TRANS environment
 *                                    data.
 * @param[in] transCbFuns             Pointer to TRANS Layer callback function.
 *                                    struct. Implemented in the higher layer or
 *                                    user application.
 *                                    (LinTransIf_sCallbackFunctions).
 * @param[in] genericTransCbCtxData   Pointer to TRANS Layer callback context
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
 * @param[in] configFlags             Configuration flags for behavior and functionalities
 *                                    of Transport Layer
 * @param[in] genericTransImpCfgData  Pointer to implementation dependent
 *                                    configuration data for the TRANS Layer
 *                                    LinTransImp_sCfgData.
 *
 * @return LIN_TRUE if the initialization of interfaces and data
 *         was successfully, LIN_FALSE if not.
 *
 * @details Initializes the TRANS Layer.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_Initialization(LinTransIf_pGenericEnvData_t             genericTransEnvData, LinTransIf_EnvDataSze_t           transEnvDataSze,
                                      LinTransIf_cpProtoFrameDescriptionInfo_t protoFrmDescInfo,    
                                      LinTransIf_cpCallbackFunctions_t         transCbFuns,         LinTransIf_pGenericCbCtxData_t    genericTransCbCtxData,
                                      LinTransIf_NasTimeout_t                  nasTimeout,          LinTransIf_NcrTimeout_t           ncrTimeout,          
                                      LinTransIf_sCfgDataFlags_t               configFlags,         LinTransIf_pGenericImpCfgData_t   genericTransImpCfgData
                                      );

/***************************************************************************//**
 * LIN TRANS layer 'Task' function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * @details The 'Task' function has to be called periodically to process scheduled task of
 *          the TRANS Layer.
 *
 ******************************************************************************/
void LinTransImp_Task(LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * LIN TRANS layer 'Get Sub Interface' function.
 *
 * @param[in]  genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in]  interfaceId         Sub-Interface ID.
 * @param[out] ifThisPtr           Address (pointer) of an instance of the
 *                                 LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          ifThisPtr could be set.
 *
 * @details Acquires the desired sub-interface (interfaceId) and writes the address
 *          of its This-Pointer to ifThisPtr.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_GetSubInterface(LinTransIf_pGenericEnvData_t genericTransEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * LIN TRANS layer 'Init Response' function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] responseNAD         NAD which shall be used in the 'Response'.
 * @param[in] rsid                Response SID, normally SID + 0x40.
 *                                (See also: LIN 2.2a Specification -
 *                                Chapter 4.2.3.5 RSID)
 * @param[in] dataLen             Length of data to response with.
 *
 * @return LIN_TRUE if response data are initialized and pointer to
 *         Transport Layer Environment is not a NULL pointer.
 *
 * @details If the user application or a higher layer module receives a 'LIN Transport
 *          Layer Request'. This function initializes the 'Response' to send.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_InitResponse(LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_NAD_t       responseNAD,
																		LinTransIf_SID_t             rsid,                LinTransIf_PDUMsgLen_t dataLen);

/***************************************************************************//**
 * LIN TRANS layer 'Append Data to Message Buffer' function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] data                Pointer to data to append to message buffer.
 * @param[in] dataLen             Length of data to append.
 *
 * @return LIN_TRUE if the data has been appended successfully to the
 *         message buffer.
 *
 * @details This function is necessary to deserialize the collection of data and
 *          the actual process of sending them back onto the bus through a
 *          'LIN Transport Layer Response'. Not every device has the RAM to buffer
 *          the full amount of data, which is possible to send with the 'LIN Transport
 *          Layer'.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_AppendDataToMsgBuffer(LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_cpData_t data,
                                             LinTransIf_BufLength_t       dataLen);


/***************************************************************************//**
 * Initialize intermediate response.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] responseNAD         NAD which shall be used in the 'Response'.
 * @param[in] rsid                Response SID, normally SID + 0x40.
 *                                (See also: LIN 2.2a Specification -
 *                                Chapter 4.2.3.5 RSID)
 * @param[in] data                pointer to response data
 * @param[in] dataLen             Length of data to response with.
 *
 * @return LIN_TRUE if response data are initialized and pointer to
 *         Transport Layer Environment is not a NULL pointer.
 *
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_InitIntermediateResponse(LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_NAD_t       responseNAD,
                                                LinTransIf_SID_t             rsid,                LinTransIf_cpData_t    data, 
                                                LinTransIf_BufLength_t       dataLen);

/***************************************************************************//**
 * LIN TRANS layer 'Add SID Description List' function.
 *
 * @param[in] genericTransEnvData             Pointer to reserved TRANS Layer
 *                                            environment data.
 * @param[in] sidDescLst                      Pointer to the 'SID Description List
 *                                            to add to the TRANS Layer.
 * @param[in] genericTransPerSidLstCbCtxData  Callback context data connected to this
 *                                            'SID List'.
 *                                            (See also: @ref LinTransIf_eSIDDescLstCbCtxType)
 *
 * @return LIN_TRUE, if the list has been successfully added,
 *         LIN_FALSE if not.
 *
 * @details Adds a 'SID List' to the TRANS Layer on which SIDs it should react on.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_AddSIDDescriptionList(LinTransIf_pGenericEnvData_t   genericTransEnvData, LinTransIf_cpSIDDescription_t sidDescLst,
                                             LinTransIf_CheckNADCbFun_t     individualCheckNAD,  LinTransIf_pGenericCbCtxData_t genericTransPerSidLstCbCtxData);

/***************************************************************************//**
 * LIN TRANS layer 'Remove SID Description List' function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] sidDescLst          Pointer to the 'SID Description List
 *                                to remove from the TRANS Layer.
 *
 * @return LIN_TRUE, if the list has been successfully removed.
 *
 * @details Removes a list of 'SIDs' from the TRANS Layer.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_RemoveSIDDescriptionList(LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_cpSIDDescription_t sidDescLst);

/***************************************************************************//**
 * Append a given callback function to the list of callback functions.
 *
 * @param[in] genericTransEnvData   Pointer to reserved TRANS Layer environment data.
 * @param[in] transCbFuns           callbackfunction to add to the list
 * @param[in] genericTransCbCtxData enviorment for the callback
 *
 * @return LIN_TRUE for success, otherwise LIN_FALSE.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_AppendCallbacks(LinTransIf_pGenericEnvData_t   genericTransEnvData, LinTransIf_cpCallbackFunctions_t transCbFuns,
                                                         LinTransIf_pGenericCbCtxData_t genericTransCbCtxData);

/***************************************************************************//**
 * Removes a given callback function from the list of callback functions.
 *
 * @param[in] genericTransEnvData   Pointer to reserved TRANS Layer environment data.
 * @param[in] transCbFuns           callbackfunction to add to the list
 *
 * @return LIN_TRUE for success, otherwise LIN_FALSE.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_RemoveCallbacks(LinTransIf_pGenericEnvData_t   genericTransEnvData, LinTransIf_cpCallbackFunctions_t transCbFuns);


/***************************************************************************//**
 * LIN TRANS layer 'Get Status' function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * @return The current status of the TRANS Layer.
 *
 * @details Acquire the current status of the TRANS Layer.
 *
 ******************************************************************************/
LinTransIf_eComStatus_t LinTransImp_GetStatus(const LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * Gets the maximum size of a message.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * @return maximum message size.
 ******************************************************************************/
LinTransIf_PDUMsgLen_t LinTransImp_GetMaxMessageSize(LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * Gets the maximum size of an intermediate response.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 *
 * @return maximum intermediate response size.
 ******************************************************************************/
LinTransIf_PDUMsgLen_t LinTransImp_GetMaxIntermediateResponseSize(LinTransIf_pGenericEnvData_t genericTransEnvData);

/***************************************************************************//**
 * LIN TRANS layer 'Set Timeout' function.
 *
 * @param[in] genericTransEnvData Pointer to reserved TRANS Layer environment data.
 * @param[in] timeoutType         Timeout type to set. (@ref LinTransIf_eTimeoutType_t)
 * @param[in] timeout             Timeout value.
 *
 * @details Sets the N_As or N_Cr timeout.
 *
 ******************************************************************************/
void LinTransImp_SetTimeout(LinTransIf_pGenericEnvData_t genericTransEnvData, LinTransIf_eTimeoutType_t timeoutType, LinTransIf_Timeout_t timeout);

/***************************************************************************//**
 * Trigger task for SID processing.
 *
 * @param[in] genericTransEnvData   Pointer to reserved TRANS Layer environment data.
 * @param[in] hint                  A user defined value which will be passed to the
 *                                  SID-Callback to indicate various calling condition
 *                                  (primarily used for manual triggered processing)
 * @param[in] nad                   node address
 * @param[in] SIDType               Enumerator about who to proceed with a received NAD
 * @param[in] sid                   Service Identifier
 *
 * @return TRUE for success, otherwise FALSE.
 *
 ******************************************************************************/
Lin_Bool_t LinTransImp_TriggerSIDProcessing(LinTransIf_pGenericEnvData_t    genericTransEnvData, 
                                            LinTransIf_SIDProcessingHint_t  hint,                LinTransIf_NAD_t             nad,
                                            LinTransIf_eSIDType_t           SIDType,             LinTransIf_SID_t             sid);

#endif /* LINTRANS_IMPLEMENTATION_H_ */


/*! @} */ /* TRANS */
/*! @} */ /* ELMOS_LIN_DRIVER */
