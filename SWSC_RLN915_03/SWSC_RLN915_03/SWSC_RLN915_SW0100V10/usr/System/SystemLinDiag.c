/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    SystemLinDiag.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file maintains system main initialization and loop.
  *Others:      None.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/
#include "SystemStatus.h"
#include "ModuleFlash.h"
#include "SystemLinDiag.h"
#include "ModuleTemperature.h"
#include "ModuleLed.h"
#include "ModuleBattery.h"

#include "ModuleClock.h"


#include "SystemLinDiag.h"
#include "ModuleFlash.h"
#include "ModuleLed.h"

/**@brief software VERSION section length.*/


#define DMAX_SERVICE_AMOUNT 15//15
    

const uint8_t softwareVersion[DSOFTWARE_VERSION_LENGTH+1] = "SW0100";    /* 7 bytes */

uint32_t SID27_Seed = 0, SID27_Key = 0;
         
         

static const SLinDiagSevice serveceList[DMAX_SERVICE_AMOUNT] =
{
  {0x0002,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.CtrlFlag)},
  {0x0003,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.singleAddr)},
  {0x0004,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.groupAddr)},
  /*The old version this section has two bytes, we add one reserved byte to prevent overflow.*/
  //{0x0005,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.platform)},
  {0x0006,  DS_READWRITE, NAD_CAL           , (2 * VehicleNum) , (uint8_t*)&(savedConfig.Lfactor)},
  {0x0007,  DS_READWRITE, NAD_CAL           , 8u , (uint8_t*)&(savedConfig.redx)},
  {0x0008,  DS_READWRITE, NAD_CAL           , 8u , (uint8_t*)&(savedConfig.greenx)},
  {0x0009,  DS_READWRITE, NAD_CAL           , 8u , (uint8_t*)&(savedConfig.bluex)},
  {0x0010,  DS_READWRITE     , NAD_CAL      , VehicleNum , (uint8_t*)&(savedConfig.MinUsrL)},  // ContentSize + ParasNo
  {0x0011,  DS_READ     , NAD_CAL           , 2u , (uint8_t*)&(savedConfig.ContentSize)},  // ContentSize + ParasNo
  {0x0012,  DS_READWRITE     , NAD_CAL      , 10u , (uint8_t*)&(savedConfig.RatingLowerVoltage)},
  {0x000A,  DS_WRITE    , NAD_CAL           , 1u , (uint8_t*)&(ledControlFlag)},
  {0x6269,  DS_READWRITE, NAD_CAL, DMAX_PART_NUMBER_LENGTH  , (uint8_t*)&(savedConfig.partNO[0])},
  {0x6469,  DS_READ     , NAD_CAL, DSOFTWARE_VERSION_LENGTH , (uint8_t*)&(softwareVersion[0])},
  {0x6869,  DS_READWRITE, NAD_CAL, DHARDWARE_VERSION_LENGTH , (uint8_t*)&(savedConfig.hardwareVersion[0])},
  {0x6A69,  DS_READWRITE, NAD_CAL, DMAX_SERIAL_NUMBER_LENGTH, (uint8_t*)&(savedConfig.serialNO[0])},
 // {0x6C69,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(sysLin.VehicleTypeInfo)},
 // {0x6C69,  DS_READ           , NAD_NORL          , DCONTENT_6C69_LEGNTH     , (uint8_t*)&CONTENT_0x6c69[0]},
 // {}
};



/**@brief task control structure.*/
struct RequsetTCB
{
  uint16_t        RequstDID;      /**<KEBODA DID*/
  uint16_t        totalLength;    /**<total access length, may not been used in reading*/
  EDiagServiceRW  operationType;  /**<Access operation type*/
  EDiagServiceNad nadType;        /**<Access NAD type*/
  uint8_t         pos;            /**<operation's start position of array.*/
  uint8_t         amount;         /**<In reading it dictates the read out length. In writing it represents the length of one writing operation.*/
  uint8_t*        data;           /**<pointer to data. This can be both in and out.*/
};
/*task control variant.*/
static struct RequsetTCB currentRequestTCB;

/**
*@details   Do actual read/write service job.
*
*@param[in%out] tcb  read/write task control block.
*
*@param[in%out] length read/write length
*
*@retval    if this access succeed.1 = failed, 0 = success
*/
static uint16_t __doRequest(struct RequsetTCB* tcb, uint16_t *length)
{
  uint16_t ret = 1;
  uint16_t index;
  //uint16_t z = 0;
  //when a diagnostic frame is detected, map all diagnostic service stored in eeprom
  for (index = 0; index < DMAX_SERVICE_AMOUNT; index++)
  {
    /*CHECH attributes*/
    if ((serveceList[index].serviceID == tcb->RequstDID) &&
        (serveceList[index].rwAttr & tcb->operationType) )
    {
      ret = 0;
      if (tcb->operationType == DS_READ)
      {      
        memcpy(tcb->data, serveceList[index].addr, serveceList[index].length);
        *length = serveceList[index].length;
      }

      if ((tcb->operationType == DS_WRITE) &&
         (serveceList[index].serviceID == 0x000A))
      {           
        memcpy(serveceList[index].addr, tcb->data, serveceList[index].length);
        return ret;
      }

      if ((tcb->operationType == DS_WRITE) &&
         !(flashFlag & EFlashUpdateLocked))
      {
        memcpy(serveceList[index].addr, tcb->data, serveceList[index].length);
        if (flashFlag == EFlashUpdateFree)
        {
          flashFlag |= EFlashUpdateProcessing;
        }
      }
      break;
    }
  }
  return ret;
}

/**
*@details   LIN diagnostic receiving call back function.
*
*@param[in] recvBuffer  Diagnostic received buffer..
*
*@retval    MELEXIS' response code.
*/
//uint16_t systemLinDiagHandleRecv(uint8_t* recvBuffer, uint16_t DID, uint16_t requestLength)
uint16_t systemLinDiag0x22Handle(const DiagReqInfo_t *const DiagReq, DiagRspInfo_t* diagRsp)
{
  uint16_t DID;
  //uint8_t   debug[8];
  uint8_t ret=0;

  DID = (uint16_t)DiagReq->payload[1]|((uint16_t)DiagReq->payload[0]<<8);

  currentRequestTCB.operationType = DS_READ;
  currentRequestTCB.totalLength = 0;
  currentRequestTCB.data = &(diagRsp->payload[2]);
  
  currentRequestTCB.RequstDID = DID;
  currentRequestTCB.amount = DiagReq->packLength;
  //currentRequestTCB.amount = requestLength - 3;
  currentRequestTCB.pos = 0;
  ret = __doRequest(&currentRequestTCB, &(diagRsp->packLength));
    
  if(ret == 0)
  {
    diagRsp->sid = DiagReq->sid;
    diagRsp->payload[0] = DiagReq->payload[0];
    diagRsp->payload[1] = DiagReq->payload[1];
    diagRsp->packLength += 2;   // above
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
    //diagRsp->payload  // above
  }
  
  //memcpy(debug, diagRsp->payload, 8);
  return ret;
}

/**
*@details   LIN diagnostic sending call back function.
*
*@param[in] recvBuffer  Diagnostic received buffer..
*
*@retval    MELEXIS' response code.
*/
//uint16_t systemLinDiagHandleSend(uint8_t* sendBuffer, uint16_t DID, uint16_t requestLength, uint16_t* respLength)
uint16_t systemLinDiag0x2EHandle(const DiagReqInfo_t *const DiagReq, uint8_t * ReqData_buffer, DiagRspInfo_t* diagRsp)
{
  uint16_t DID;
  //uint8_t   debug[8];
  uint8_t ret=0;
  
  
  DID = (uint16_t)ReqData_buffer[1]|((uint16_t)ReqData_buffer[0]<<8);
  currentRequestTCB.operationType = DS_WRITE;
  currentRequestTCB.totalLength = 0;
  currentRequestTCB.data = &(ReqData_buffer[2]);
  currentRequestTCB.RequstDID = DID;
//   if (ReqData_buffer[1] == 0x02 && (ReqData_buffer[2] & 0xF0) >> 4 == 8)
   if (ReqData_buffer[1] == 0x02 && ReqData_buffer[2] == 0x82)
    {
        savedConfig.CtrlFlag.bit.IsLocked  =  1;
        flashFlag |= EFlashUpdateLocked;
        moduleFlashSave();
    }
  ret = __doRequest(&currentRequestTCB, NULL);

  if(ret == 0)
  {
    diagRsp->sid = DiagReq->sid;
    diagRsp->payload[0] = DiagReq->payload[0];
    diagRsp->payload[1] = DiagReq->payload[1];
    diagRsp->packLength = 2;   // above
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
    //diagRsp->payload  // above
  }
  
  //memcpy(debug, diagRsp->payload, 8);
  return ret;
}

#include "linsNodeCfgIdentify.h"
uint16_t systemLinDiag0x27Handle(const DiagReqInfo_t *const DiagReq, uint8_t * ReqData_buffer, DiagRspInfo_t* diagRsp)
{
  uint8_t SSID = 0, ret_27 = 0;
  SSID = DiagReq->payload[0];
  ReadByIdNakRsp_t *rsp = (ReadByIdNakRsp_t *)((void *)diagRsp->payload);
  
  switch(SSID)
  {
  case 0x03:
    SID27_Seed = (uint32_t)moduleClockGet();
    diagRsp->sid = DiagReq->sid;
    diagRsp->packLength = 1 + 4; 
    diagRsp->payload[0] = SSID;
    diagRsp->payload[1] = (uint8_t)(SID27_Seed & 0x000000FF);
    diagRsp->payload[2] = (uint8_t)((SID27_Seed & 0x0000FF00) >> 8);
    diagRsp->payload[3] = (uint8_t)((SID27_Seed & 0x00FF0000) >> 16);
    diagRsp->payload[4] = (uint8_t)((SID27_Seed & 0xFF000000) >> 24);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
    ret_27 = 0;
    break;
  case 0x04:       
    SID27_Key = (uint32_t)(DiagReq->payload[1]) | (uint32_t)(DiagReq->payload[2]<<8) | (uint32_t)(DiagReq->payload[3]<<16) | (uint32_t)(DiagReq->payload[4]<<24);
    if (SID27_Key == (SID27_Seed + 0x0926))
    {
      diagRsp->sid = DiagReq->sid;
      diagRsp->payload[0] = SSID;
      diagRsp->packLength = 1; 
      ret_27 = 0;
    }
    else
    {             
      diagRsp->sid = 0x7FU;
      rsp->sid = 0x27;
      rsp->errorCode = 0x12U;
      diagRsp->packLength = (uint16_t)sizeof(ReadByIdNakRsp_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      ret_27 = 0;
    }
    SID27_Seed = 0;
    SID27_Key = 0;
    savedConfig.CtrlFlag.bit.IsLocked = bfalse;
    flashFlag = EFlashUpdateFree;
    
    break;
  default:
    diagRsp->sid = 0x7FU;
    rsp->sid = LIN_SID_READ_BY_ID;
    rsp->errorCode = 0x31U;
    diagRsp->packLength = (uint16_t)sizeof(ReadByIdNakRsp_t);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
    ret_27 = 0;
    SID27_Seed = 0;
    SID27_Key = 0;
    break;
  }
     
  return ret_27;
}
