/***************************************************************************//**
 * @file    LinDrvImp_Callbacks.c
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
 * $Id: LinDrvImp_Callbacks.c 9596 2022-01-19 14:17:54Z poe $
 *
 * $Revision: 9596 $
 *
 ******************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include <el_helper.h>
#include "LinDrvImp_Callbacks.h"
#include "LinDrvImp_Config.h"
#include "LinDrvImp_FrameConfig.h"
#include <string.h>
#include "eeprom_access_interface.h"
#include "lin_user.h"
#include "LinDrvImp_Callbacks.h"
#include "LinDrvImp_Config.h"
/* ****************************************************************************/
/* ***************************** USER INCLUDES ********************************/
/* ****************************************************************************/

#include "io_e52139b_IAR.h"
//#include "mod_pwm.h"
//#include "mod_sys.h"
#include "SystemMain.h"
#include "SystemStatus.h"
#include "SystemLinDiag.h"  //LJM add
#include "genLinConfig.h"
/* ****************************************************************************/
/* ***************************** USER INCLUDES ********************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINDIAGIF_DATADUMP_REQLEN             5u  /**< Length of a 'Assign NAD' request. */
/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/
uint8_t loc_ErrorEnableCounter = 0u;
uint8_t loc_RbiCounter = 0u;
/* ****************************************************************************/
/* ******************** FORWARD DECLARATIONS / PROTOTYPES *********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FUNCTION DEFINITIONS ****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/*                               BUS CALLBACKS                                */
/* ****************************************************************************/

/* ****************************************************************************/
/*                              PROTO CALLBACKS                               */
/* ****************************************************************************/
uint8_t GucSleepEn = 0;
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
 *
 ******************************************************************************/
void LinDrvImp_ProtoErrorCbFun(LinProtoIf_pGenericEnvData_t    genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                               LinProtoIf_Error_t              error,               LinBusIf_FrameID_t                frameID,
                               LinProtoIf_cpFrameDescription_t frameDesc,           LinProtoIf_pGenericCbCtxData_t    genericProtoCbCtxData)
{
  (void)genericProtoEnvData;
  (void)protoIfFuns;
  (void)frameID;
  (void)frameDesc;
  (void)genericProtoCbCtxData;
  
  /* Check if error code belongs to the BUS Layer. */
  if ( ( error >= (LinDiagIf_Error_t) Lin_ERROR_AREA_BUS ) &&
       ( error < ( (LinDiagIf_Error_t) Lin_ERROR_AREA_BUS + LIN_ERROR_AREA_SIZE ) ) )
  {
    /* Filter error codes which should not be reported. */
    if ( ( error != (LinDiagIf_Error_t) LinBusIf_ERR_HEADER_TIMEOUT ) &&
         ( error != (LinDiagIf_Error_t) LinBusIf_ERR_MESSAGE_TIMEOUT_NO_DATA ) &&
         ( error != (LinDiagIf_Error_t) LinBusIf_ERR_PID_PARITY ) &&
         ( error != (LinDiagIf_Error_t) LinBusIf_ERR_SYNC_FAIL ) &&
         ( error != (LinDiagIf_Error_t) LinBusIf_ERR_RX_OVERRUN ) )
    {
      LinDrvImp_ResponseErrorSignal= 1u;
    }
    
    linError = 1;
  }
  else { /* Continue. */ }
  
  /* Do other error handling here. */
  
  return;
}


/* **************************************************************************//**
 * @implementation
 * Bus idle event callback from LIN PROTO layer.
 *
 * @param[in]   genericProtoEnvData   LIN PROTO environment data
 * @param[in]   protoIfFuns           LIN PROTO IF pointer            
 * @param[in]   genericProtoCbCtxData LIN PROTO context data
 *
 * @return      none
 *
 ******************************************************************************/
#include "ModulePower.h"
LinProtoIf_eIdleAction_t LinDrvImp_ProtoIdleCbFun(LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                                                  LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData)
{
  //  LinDrvImp_BusIfFuns->GoToSleep(LinDrvImp_BusEnvData);
    if (sysStatus.sleepFlag == ESystemSleepAwake)
    {
        sysStatus.sleepFlag = ESystemSleepPrepare;
    }
	
    GucSleepEn = 1;
    return LinBusIf_IDLEACT_SLEEP;
}

/***************************************************************************//**
 * @implementation
 * Bus wakeup event callback from LIN PROTO layer.
 *
 * @param[in]   genericProtoEnvData   LIN PROTO environment data
 * @param[in]   protoIfFuns           LIN PROTO IF pointer
 * @param[in]   frameID               current LIN frame id            
 * @param[in]   frame_Desc            frame description of current LIN frame
 * @param[in]   genericProtoCbCtxData LIN PROTO context data
 *
 * @return      none
 *
 ******************************************************************************/
void LinDrvImp_ProtoWakupCbFun(LinProtoIf_pGenericEnvData_t   genericProtoEnvData, LinProtoIf_cpInterfaceFunctions_t protoIfFuns,
                               LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData)
{
  if (sysStatus.sleepFlag == ESystemSleepSleep)
    {
        systemInit();
    }
}



/* ****************************************************************************/
/*                              DIAG CALLBACKS                                */
/* ****************************************************************************/

/***************************************************************************//**
 * @implementation
 * For this example, we switch the LED off (if compiled with @ref SHOW_SLEEP_STATE_WITH_LED)
 * and invoke the bus interface function GoToSleep.
 ******************************************************************************/
#include "ModulePower.h"
void LinDrvImp_DiagGoToSleepCbFun(LinDiagIf_pGenericEnvData_t   genericDiagEnvData, LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                                  LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
    sysStatus.sleepFlag = ESystemSleepPrepare;
    GucSleepEn = 1;
}

/***************************************************************************//**
 * @implementation
 * Example on how to check the error and respond.
 ******************************************************************************/
void LinDrvImp_DiagErrorCbFun(LinDiagIf_pGenericEnvData_t   genericDiagEnvData, LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                              LinDiagIf_Error_t             error,              LinDiagIf_SID_t                  SID,
                              LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
  if((error != LinBusIf_ERR_HEADER_TIMEOUT) &&
     (error != LinBusIf_ERR_MESSAGE_TIMEOUT_NO_DATA) &&
	 (error != LinBusIf_ERR_SYNC_FAIL) &&
	 (error != LinBusIf_ERR_PID_PARITY))
  {
      LinDrvImp_ResponseErrorSignal = 1;
    } 
}


#if LINDIAG_SUP_SNPD == 1
/***************************************************************************//**
 * @implementation
 * For this example we check the SNPD method id against valid ELMOS ID's.
 ******************************************************************************/
LinDiagIf_MethodIdCheckResult_t LinDrvImp_DiagCheckSNPDMethodIdCbFun( LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                                                      LinDiagIf_cpInterfaceFunctions_t busIfFuns,
                                                                      LinDiagIf_MethodId_t methodId,           
                                                                      LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
  LinDiagIf_MethodIdCheckResult_t result = LinDiagIf_MethodId_Ignore;
  
  if (methodId == 0xf1 || methodId == 0x02)
  {
    result = LinDiagIf_MethodId_Accept;
  }

  return result;
}
#endif /* LINDIAG_SUP_SNPD == 1*/


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
                                 LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{

  LinDrvImp_Diag_triggerNADStorage = LIN_TRUE;

  return;
}

#if LINDIAG_SUP_SNPD == 1
/* ****************************************************************************
 * @implementation
 * For this example we just use the predefined threshold values.
 * ****************************************************************************/
void LinDrvImp_GetSNPDPlusParametersCbFun(LinBusIf_pGenericEnvData_t busEnvData, LinBusIf_cpInterfaceFunctions_t busIfFuns, LinBusImp_pSNPDCallbackConfig_t snpdConfigData, LinBusIf_pGenericCbCtxData_t genericBusCbCtxData)
{
  snpdConfigData->pre_thres    = SNPD_PRESEL_THRES;
  snpdConfigData->sel_thres    = SNPD_SEL_THRES;
}
#endif






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
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData )
{
  uint8_t loc_DataDumpBuffer[ LINDIAGIF_DATADUMP_REQLEN ] = {0};
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;
  
  if ( serviceData->ErrorCode == Lin_NO_ERROR )
  {
    if ( serviceData->DataLen == LINDIAGIF_DATADUMP_REQLEN ) 
    {
      for (uint8_t i=0; i < LINDIAGIF_DATADUMP_REQLEN; ++i )
      {
        /* Simple example data operation to generate a response based on request data. */
        loc_DataDumpBuffer[i] = ~serviceData->DataBuffer[i];
      }
      
      /* Set return value to processed. */
      returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
      
      // send positive response + data
      /* Initiate response.*/
      if ( thisRespIf->IfFunsTbl->InitResponse( thisRespIf->envData, serviceData->NAD,  serviceData->SID + LINDIAGIF_RSID_OFFSET, LINDIAGIF_DATADUMP_REQLEN ) == LIN_TRUE )
      {
        /* Append response data. */
        if ( thisRespIf->IfFunsTbl->AppendDataToMsgBuffer( thisRespIf->envData, loc_DataDumpBuffer, sizeof( loc_DataDumpBuffer ) ) == LIN_TRUE )
        {
          /* Set return value to send reponse. */
          returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
        }
        else { /* Error: Data could not be appended to response buffer! */ }
      }
      else { /* Error: Response could not be initiated! */ }
    }
    else { /* Error: invalid req length -> ignore! */ }
  }
  else { /* Error: error code is not as expected! */ }

  return( returnValue );
}

                               



                                                  


/***************************************************************************//**
zky test
 ******************************************************************************/
LinTransIf_eSIDReaction_t LinDrvImp_0x10( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData )
{
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;
  
  if ( serviceData->ErrorCode == Lin_NO_ERROR )
  {
    if ( serviceData->DataLen == (2-1) ) 
    {

      uint8_t buf[8] = {1,2,3,4,5,6,7,8};
      
      /* Set return value to processed. */
      returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
      
      // send positive response + data
      /* Initiate response.*/
      if ( thisRespIf->IfFunsTbl->InitResponse( thisRespIf->envData, serviceData->NAD,  serviceData->SID + LINDIAGIF_RSID_OFFSET, LINDIAGIF_DATADUMP_REQLEN ) == LIN_TRUE )
      {
        /* Append response data. */
        if ( thisRespIf->IfFunsTbl->AppendDataToMsgBuffer( thisRespIf->envData, buf, 5 ) == LIN_TRUE )
        {
          /* Set return value to send reponse. */
          returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
        }
        else { /* Error: Data could not be appended to response buffer! */ }
      }
      else { /* Error: Response could not be initiated! */ }
    }
    else { /* Error: invalid req length -> ignore! */ }
  }
  else { /* Error: error code is not as expected! */ }

  return( returnValue );
}

/* LJM Added */
LinTransIf_eSIDReaction_t LinDrvImp_0x22( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData )
{
    LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;
    LinDiagIf_Data_t buffer[32];
    Lin_uint16_t len;

    uint16_t SSID;

    if (serviceData->NAD == 0xA0)
    {
        SSID = (((uint16_t)(serviceData->DataBuffer[0]) << 8) | (uint16_t)(serviceData->DataBuffer[1]));
        if(systemLinDiagHandleSend((uint8_t*)&buffer, SSID, serviceData->DataLen , (uint16_t*)&len) == 0)
        {
          /* Set return value to processed. */
          returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
          len += 2;
          // send positive response + data
          /* Initiate response.*/
          if ( thisRespIf->IfFunsTbl->InitResponse( thisRespIf->envData, serviceData->NAD,  serviceData->SID + LINDIAGIF_RSID_OFFSET, len ) == LIN_TRUE )
          {
            /* Append response data. */
            if ( thisRespIf->IfFunsTbl->AppendDataToMsgBuffer( thisRespIf->envData, buffer, len ) == LIN_TRUE )
            {
              /* Set return value to send reponse. */
              returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
            }
            else 
            { /* Error: Data could not be appended to response buffer! */ }
          }
          else 
          { /* Error: Response could not be initiated! */ }
        }
    }                                      
    return returnValue;
}


/* LJM Added */
LinTransIf_eSIDReaction_t LinDrvImp_0x2E( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData )
{
    LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;
    LinDiagIf_Data_t buffer[32];
    uint16_t SSID;

    if (serviceData->NAD == 0xA0)
    {
        SSID = (((uint16_t)(serviceData->DataBuffer[0]) << 8) | (uint16_t)(serviceData->DataBuffer[1]));
        memcpy(buffer, serviceData->DataBuffer, serviceData->DataLen);
        if(systemLinDiagHandleRecv(buffer, SSID, serviceData->DataLen) == 0)
        {
            thisRespIf->IfFunsTbl->InitResponse(thisRespIf->envData, serviceData->NAD, serviceData->SID + LINDIAGIF_RSID_OFFSET,  0); 
            returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
        }  
    }                                      
    return returnValue;
}


/* ygh Added */
LinTransIf_eSIDReaction_t LinDrvImp_0x27( LinTransIf_cpThis_t                     thisTransIf,
                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData )
{
    LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;
    LinDiagIf_Data_t buffer_recv[32];
    LinDiagIf_Data_t buffer_send[32];
    Lin_uint16_t len;
    uint8_t SSID;

    if (serviceData->NAD == 0xA0)
    {
        memcpy(buffer_recv, serviceData->DataBuffer, sizeof(serviceData));
        SSID = buffer_recv[0];
        if(systemLinDiag0x27Handle(serviceData->DataBuffer, SSID, buffer_send, (uint16_t*)&len) == 0)
        {
            if ( thisRespIf->IfFunsTbl->InitResponse( thisRespIf->envData, serviceData->NAD,  serviceData->SID + LINDIAGIF_RSID_OFFSET, len ) == LIN_TRUE )
            {
                returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
                if ( thisRespIf->IfFunsTbl->AppendDataToMsgBuffer( thisRespIf->envData, buffer_send, len ) == LIN_TRUE )
                {
                    /* Set return value to send reponse. */
                    returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
                }
                else 
                { /* Error: Data could not be appended to response buffer! */ }
            }
        }  
    }                                      
    return returnValue;

}
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
                                          LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData )
{
  Lin_Bool_t retVal = LIN_FALSE;
  LinDiagIf_Data_t buffer[4];

   
  /* Buffer build for example response data. */
  buffer[0] = rbIdentifier;
  buffer[1] = loc_RbiCounter++;
  buffer[2] = ~rbIdentifier;
  buffer[3] = 0xAA;

  retVal = appendDataToMsgBufFun(genericDiagEnvData, (LinDiagIf_pData_t) buffer, sizeof(buffer));

  return(retVal);
}

/*! @} */ /* SAMPLE_CALLBACKS */
/*! @} */ /* SAMPLE_PROJECT */

