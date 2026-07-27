/***************************************************************************//**
 * @file      LinDrvImp_FrameConfig.c
 *
 * @creator   poe
 * @created   2021.02.23 
 *
 * @brief     LIN driver custom frame id and handler definition. 
 *
 * @purpose   Custom frame id description and handling is done here.
 *            
 * 
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
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
 *
 * $Id: LinDrvImp_FrameConfig.c 9596 2022-01-19 14:17:54Z poe $
 *
 * $Revision: 9596 $
 *
 ******************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include <el_helper.h>
#include "LinDrvImp_FrameConfig.h"
#include "LinDrvImp_Config.h"
#include "LinDrvImp_Callbacks.h"
#include "lin_user.h"
#include "stdio.h"
#include "string.h"

#include "genLinConfig.h"
/* ****************************************************************************/
/* ****************************** USER INCLUDES *******************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/

uint8_t masterSendBuffer[8] = {0};
uint8_t masterReadBuffer[8] = {0};
Lin_Bool_t LinDrvImp_ResponseErrorSignal = LIN_FALSE;

LinBusIf_eFrameIdAction_t Lin_Master_Write_Process(LinProtoIf_pGenericEnvData_t    genericProtoEnvData, LinBusIf_FrameID_t             frameID,
                                          LinProtoIf_cpFrameDescription_t frameDesc,           LinProtoIf_pData_t             msgBuf,
                                          LinProtoIf_BufLength_t          msgBufLen,           LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData)
{

    LinBusIf_eFrameIdAction_t retVal = LinBusIf_PIDACT_IGNORE;
  
  if(msgBufLen == 8 )
  { 
    memcpy(masterSendBuffer,msgBuf,8);
    retVal = LinBusIf_PIDACT_RECEIVE;
  }

  return(retVal);
}

/* Response LIN (Response from slave) */
LinBusIf_eFrameIdAction_t Lin_Master_Read_Process(LinProtoIf_pGenericEnvData_t    genericProtoEnvData, LinBusIf_FrameID_t frameID,
                                           LinProtoIf_cpFrameDescription_t frameDesc,           LinProtoIf_pData_t msgBuf,
                                           LinProtoIf_BufLength_t          msgBufLen,
                                           LinProtoIf_pGenericCbCtxData_t  genericProtoCbCtxData)
{

    LinBusIf_eFrameIdAction_t retVal = LinBusIf_PIDACT_IGNORE;
    
   l_bool_wr_RGB_Slave_01_ResponseError(LinDrvImp_ResponseErrorSignal);

    memcpy(msgBuf,masterReadBuffer,8);
    
    retVal = LinBusIf_PIDACT_TRANSMIT;
    LinDrvImp_ResponseErrorSignal = 0;
  return(retVal);
}


/* ****************************************************************************/
/* ************************ FRAME DESCRIPTION LIST ************************** */
/* ****************************************************************************/


LinProtoIf_sFrameDescription_t LinDrvImp_FrameDescriptionList[LINPROTOIMP_FRAMEDESCLST_SZE] =
{
  /*                                      | FRAME ID |         CRC TYPE        |               FRAME KIND                 | LEN |     POINTER TO SIGNAL LIST  |       PROCESSING                |    FRAME ID PROCESSED CALLBACK     |      CALLBACK CTX DATA TYPE             |    CTX DATA POINTER       | */
  LINPROTO_INTERFACE_ADD_FRAME_DESCRIPTION(      0x15,  LinProtoIf_CRC_Extended,   LinProtoIf_FrameKind_SUBSCRIBE_WITH_FUN,    8,      Lin_Master_Write_Process,     LinProtoIf_TxRxCbProc_DIRECT,                            LIN_NULL,    LinProtoIf_FrmIdPrcCbCtxDataType_NONE,                  LIN_NULL),
  LINPROTO_INTERFACE_ADD_FRAME_DESCRIPTION(      0x01,  LinProtoIf_CRC_Extended,   LinProtoIf_FrameKind_PUBLISH_WITH_FUN  ,    8,      Lin_Master_Read_Process ,     LinProtoIf_TxRxCbProc_DIRECT,                            LIN_NULL,    LinProtoIf_FrmIdPrcCbCtxDataType_NONE,                  LIN_NULL),
  LINPROTO_INTERFACE_FRAME_DESCRIPTION_END
  
};


/* 
   Service Description Listing for frameID recognition and handling 
   SID                         Defines the target service ID to register to the system for recognition and handling
   CB FUNCTION                 Defines a function callback pointer for after frame has been processed @POINTER TO SIGNAL LIST
   CB CTX TYPE                 Type defining how to handle the callback processing
   CB CTX                      Defines a context data pointer for additional information context
*/
//const LinTransIf_sSIDDescription_t LinDrvImp_SIDDescrLst[3] = 
const LinTransIf_sSIDDescription_t LinDrvImp_SIDDescrLst[KBDSIDNum] =  // LJM added on 20230425
{
  /*                            | SID                 |             CB FUNCTION         |               CB CTX TYPE               |     CB CTX             | */
  LINTRANS_INTERFACE_ADD_SERVICE( 0xB4                , &LinDrvImp_DataDump             , LinTransIf_SIDDescLstCbCtxType_PER_SID  ,               LIN_NULL),
  LINTRANS_INTERFACE_ADD_SERVICE( 0x10                , &LinDrvImp_0x10                 , LinTransIf_SIDDescLstCbCtxType_PER_SID  ,               LIN_NULL),  
  LINTRANS_INTERFACE_ADD_SERVICE( 0x22                , &LinDrvImp_0x22                 , LinTransIf_SIDDescLstCbCtxType_PER_SID  ,               LIN_NULL),
  LINTRANS_INTERFACE_ADD_SERVICE( 0x2E                , &LinDrvImp_0x2E                 , LinTransIf_SIDDescLstCbCtxType_PER_SID  ,               LIN_NULL),
  LINTRANS_INTERFACE_ADD_SERVICE( 0x27                , &LinDrvImp_0x27                 , LinTransIf_SIDDescLstCbCtxType_PER_SID  ,               LIN_NULL), 
  LINTRANS_INTERFACE_ADD_SERVICE_LIST_END //List terminator entry.
};


/* 
   Lookup Listing for RbID recognition and handling 
   FirstRbIdentifier           Defines the starting rbi ID for the connected handler callback.
   LastRbIdentifier            Defines the ending rbi ID for the connected handler callback.
   LengthType                  Defines for the length parameter types of a 'Read By Identifier' table entry. 
   CB CTX                      Defines a context data pointer for additional information context
   Length                      Defines the message data length.
*/
const LinDiagIf_sRbiLookupEntry_t LinDrvImp_RbiTbl[2] =
{
  {.FirstRbIdentifier = 0x20, .LastRbIdentifier = 0x22, .Callback = &LinDrvImp_RbiApplicationIdent, .LengthType = LinDiagIf_RbiLenType_Value, .CbCtxData = LIN_NULL, .Length = (LinDiagIf_RbiLenght_t) 4u},
  {.FirstRbIdentifier = 0u, .LastRbIdentifier = 0u, .Callback = LIN_NULL, .CbCtxData = LIN_NULL, .Length = 0u} //List terminator entry.
};
