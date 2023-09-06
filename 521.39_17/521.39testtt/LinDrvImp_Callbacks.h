/***************************************************************************//**
 * @file    LinDrvImp_Callbacks.h
 *
 * @creator poe
 * @created 2021/03/05
 *
 * @brief Point of convergence for LIN driver layer callbacks.
 *
 * @purpose All relevant callback functions of the LIN driver layers are declared here.
 *
 *
 * @addtogroup SAMPLE_PROJECT Sample Project
 * @{
 * @addtogroup SAMPLE_CALLBACKS LIN Driver Callbacks
 * @{
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
 * $Id: LinDrvImp_Callbacks.h 9596 2022-01-19 14:17:54Z poe $
 *
 * $Revision: 9596 $
 *
 ******************************************************************************/

#ifndef LINDRVIMP_CALLBACKS_H_
#define LINDRVIMP_CALLBACKS_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_helper.h"
#include "LinProto_Interface.h"
#include "LinDiag_Interface.h"
#include "LinBus_Implementation.h"
/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************* BASIC LIN CONFIGURATION ************************ */
/* ****************************************************************************/
#define SNPD_PRESEL_THRES (100u)
#define SNPD_SEL_THRES    (75u)

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/* ****************************************************************************/
/*                               BUS CALLBACKS                                */
/* ****************************************************************************/

/* ****************************************************************************/
/*                              PROTO CALLBACKS                               */
/* ****************************************************************************/

/* **************************************************************************//**
 * @implementation
 * Error callback from LIN PROTO layer.
 *
 * @param[in] genericProtoEnvData Pointer to PROTO layer environment data.
 * @param[in] protoIfFuns         Pointer to PROTO layer interface functions
 * @param[in] error               The occurred error
 * @param[in] frameID             The frame-ID on which the error occurred
 * @param[in] frameDesc           The frame description for this frame
 * @param[in] genericBusCbCtxData Pointer to BUS layer callback context data.
 *
 * @return    none
 ******************************************************************************/
void LinDrvImp_ProtoErrorCbFun(LinProtoIf_pGenericEnvData_t    genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                               LinProtoIf_Error_t              error,               LinBusIf_FrameID_t                frameID,
                               LinProtoIf_cpFrameDescription_t frameDesc,           LinProtoIf_pGenericCbCtxData_t    genericProtoCbCtxData);

/* **************************************************************************//**
 * @implementation
 * Bus idle event callback from LIN PROTO layer.
 *
 * @param[in]   genericProtoEnvData   LIN PROTO environment data
 * @param[in]   protoIfFuns           LIN PROTO IF pointer            
 * @param[in]   genericProtoCbCtxData LIN PROTO context data
 * @return      none
 * *****************************************************************************/
LinProtoIf_eIdleAction_t LinDrvImp_ProtoIdleCbFun(LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                                  LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);


/* **************************************************************************//**
 * @implementation
 * Bus wakeup event callback from LIN PROTO layer.
 *
 * @param[in]   genericProtoEnvData   LIN PROTO environment data
 * @param [in]  protoIfFuns           LIN PROTO IF pointer            
 * @param[in]   genericProtoCbCtxData LIN PROTO context data
 * @return      none
 * *****************************************************************************/
void LinDrvImp_ProtoWakupCbFun(LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                               LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);





/* ****************************************************************************/
/*                              DIAG CALLBACKS                                */
/* ****************************************************************************/
/***************************************************************************//**
 * @implementation
 * For this example, we switch the LED off (if compiled with @ref SHOW_SLEEP_STATE_WITH_LED)
 * and invoke the bus interface function GoToSleep.
 ******************************************************************************/
void LinDrvImp_DiagGoToSleepCbFun(LinDiagIf_pGenericEnvData_t   genericDiagEnvData, LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                                  LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);

/***************************************************************************//**
 * @implementation
 * Example on how to check the error and respond.
 ******************************************************************************/
void LinDrvImp_DiagErrorCbFun(LinDiagIf_pGenericEnvData_t   genericDiagEnvData, LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                              LinDiagIf_Error_t             error,              LinDiagIf_SID_t                  SID,
                              LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);


#if LINDIAG_SUP_SNPD == 1
/***************************************************************************//**
 * @implementation
 * For this example we check the SNPD method id against valid ELMOS ID's.
 ******************************************************************************/
LinDiagIf_MethodIdCheckResult_t LinDrvImp_DiagCheckSNPDMethodIdCbFun (LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                                                      LinDiagIf_cpInterfaceFunctions_t busIfFuns,
                                                                      LinDiagIf_MethodId_t methodId,
                                                                      LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);
#endif /* LINDIAG_SUP_SNPD == 1 */


#if LINDIAG_SUP_SNPD == 1
/* ****************************************************************************
 * @implementation
 * Example implementation for a diagnostic store nad callback function.
 *
 * @param genericDiagEnvData    ptr to diagnostic layer envData.
 * @param diagIfFuns            ptr to diagnostic layer interface.
 * @param nad                   currently assigned NAD.
 * @param genericDiagCbCtxData  optional callback context data of diagnostic layer.
 *
 * @return none
 * ****************************************************************************/
void LinDrvImp_DiagStoreNadCbFun(LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                 LinDiagIf_cpInterfaceFunctions_t diagIfFuns,
                                 LinDiagIf_NAD_t nad,                
                                 LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);
#endif /* LINDIAG_SUP_SNPD == 1 */


#if LINDIAG_SUP_SNPD == 1
/* ****************************************************************************
 * @implementation
 * For this example we just use the predefined threshold values.
 * ****************************************************************************/
void LinDrvImp_GetSNPDPlusParametersCbFun(LinBusIf_pGenericEnvData_t busEnvData, LinBusIf_cpInterfaceFunctions_t busIfFuns, LinBusImp_pSNPDCallbackConfig_t snpdConfigData, LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);
#endif /* LINDIAG_SUP_SNPD == 1 */


//void sleepTick(void);
/* ****************************************************************************/
/*                              FRAME RESP CALLBACKS                          */
/* ****************************************************************************/

/***************************************************************************//**
 * @implementation
 * Example for the frame id processed of signal list ExchangeToBufferRequest.
 ******************************************************************************/
LinProtoIf_eMsgAction_t ExchangeToBufferRequest_FrameProcessed( LinProtoIf_pGenericEnvData_t genericProtoEnvData, 
                                                                LinProtoIf_cpInterfaceFunctions_t ifFunctions,
                                                                LinBusIf_FrameID_t frameID,             
                                                                LinProtoIf_cpFrameDescription_t frameDesc,
                                                                LinProtoIf_Error_t error,               
                                                                LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * @implementation
 * Example for the frame id processed of signal list BufferToExchange_Response.
 ******************************************************************************/
LinProtoIf_eMsgAction_t BufferToExchange_Response_FrameProcessed( LinProtoIf_pGenericEnvData_t genericProtoEnvData, 
                                                                  LinProtoIf_cpInterfaceFunctions_t ifFunctions,
                                                                  LinBusIf_FrameID_t frameID,
                                                                  LinProtoIf_cpFrameDescription_t frameDesc,
                                                                  LinProtoIf_Error_t error,
                                                                  LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData );



/* ****************************************************************************/
/*                              SERVICE HANDLER CALLBACKS                     */
/* ****************************************************************************/
/***************************************************************************//**
 * @implementation
 * Example for the 0xB4 DataDump diagnostic service implementation.
 * @param thisTransIf
 * @param thisRespIf 
 * @param serviceData
 * @param genericTransCbCtxData
 *
 * return LinTransIf_SIDRA_IGNORED            if message data did not match specification,
 *        LinTransIf_SIDRA_SUCCESSFULLY_PROC  if message was received and processed but no valid response
 *                                            could be generated,
 *        LinTransIf_SIDRA_SEND_RESPONSE      if the message was received and processed and a response is ready.
 ******************************************************************************/
LinTransIf_eSIDReaction_t LinDrvImp_DataDump( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData );



LinTransIf_eSIDReaction_t LinDrvImp_0x10( LinTransIf_cpThis_t                     thisTransIf,
                                          LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                          LinTransIf_cpSIDCbServiceData_t         serviceData,
                                          LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData );
 
/* LJM Added */
LinTransIf_eSIDReaction_t LinDrvImp_0x22( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData );
LinTransIf_eSIDReaction_t LinDrvImp_0x2E( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData );

LinTransIf_eSIDReaction_t LinDrvImp_0x27( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData );
/* ****************************************************************************/
/*                      DIAGNOSTIC RBI HANDLER CALLBACKS                      */
/* ****************************************************************************/


/***************************************************************************//**
 *@implementation
 * Example for customized Rbi request handling.
 * @param genericDiagEnvData
 * @param ifFunctions 
 * @param rbIdentifier
 * @param appendDataToMsgBufFun
 * @param genericDiagCbCtxData
 *
 * return LIN_TRUE  Handling was done successfully,
 *        LIN_FALSE else, does not respond.
 ******************************************************************************/
Lin_Bool_t LinDrvImp_RbiApplicationIdent( LinDiagIf_pGenericEnvData_t genericDiagEnvData,   
                                          LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                                          LinDiagIf_RbIdentifier_t rbIdentifier,         
                                          LinDiagIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun,
                                          LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData );


#endif /* LINDRVIMP_CALLBACKS_H_ */
/*! @} */ /* SAMPLE_CALLBACKS */
/*! @} */ /* SAMPLE_PROJECT */
