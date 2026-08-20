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
#include "Swc_PersistentData.h"
#include "Swc_Diagnostic.h"
#include "SystemLinDiag.h"
#include "IoHwAb_Temperature.h"
#include "IoHwAb_Led.h"
#include "IoHwAb_Battery.h"
#include "IoHwAb_Time.h"
#include "Rte_Bsw.h"

/**@brief software VERSION section length.*/
#define DSOFTWARE_VERSION_LENGTH  5U

/**@brief module name section length.*/
#define DCONTENT_6C69_LEGNTH      13U
#if 0
#include <stdint.h>

#define STACK_SIZE 0x400 // ?μ???
extern uint32_t __initial_sp; // ??￥3??μ?·

// 3???ˉ??????¨?
void initStackSpace(void) {
    volatile uint32_t *stackPtr;
    // ???μ???·
    volatile uint32_t *stackBottom = (volatile uint32_t *)((uint32_t)&__initial_sp - STACK_SIZE);
    // ???¨μ?∥?3???ˉ????    for (stackPtr = stackBottom; stackPtr < &__initial_sp; ++stackPtr) {
        *stackPtr = 0xCCCCCCCC; // ??0xCC??????3???ˉ?
    }
}

// ?1à?μ?1????
uint32_t measureStackUsage(void) {
    volatile uint32_t *stackPtr;
    uint32_t used = 0;

    // ???μ???·
    volatile uint32_t *stackBottom = (volatile uint32_t *)(((uint32_t)&__initial_sp) - STACK_SIZE);

    // ′??μ?a????è￡?2镒μ????2?μ???¨3???ˉ?μ??·￡?±??μ?1??a?
    for (stackPtr = stackBottom; stackPtr < &__initial_sp; ++stackPtr) {
        if (*stackPtr != 0xCCCCCCCC) { // 2镒μ????2??3???ˉ?μ??·
            used = (uint32_t)(&__initial_sp - stackPtr);
            break; // ?μ????μ?????3???·
        }
    }

    return used * sizeof(uint32_t); // ·μ??ъ1?μ?????С￡?μ￥λ????
}


#endif

const uint8_t softwareVersion[DSOFTWARE_VERSION_LENGTH + 1] = "S0002";    /* 5 bytes */

const uint8_t CONTENT_0x6c69[DCONTENT_6C69_LEGNTH] = "RGBe_Slave   "; /* 13 bytes */
uint16_t DebugLJM[15];
static uint8_t s_ledControlFlag;
static uint16_t s_quiltCheck[3];
/**@brief Service list.*/
#define DMAX_SERVICE_AMOUNT 21
//extern int16_t debugTemp, debugJunction;

static const SLinDiagSevice serveceList[DMAX_SERVICE_AMOUNT] =
{
  {0x0002,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.ledSupplier)},
  {0x0003,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.singleAddr)},
  {0x0004,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.groupAddr)},
  /*The old version this section has two bytes, we add one reserved byte to prevent overflow.*/
  {0x0005,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.platform)},
  {0x0006,  DS_READWRITE, NAD_CAL           , 2u , (uint8_t*)&(savedConfig.factor)},
  {0x0007,  DS_READWRITE, NAD_CAL           , 8u , (uint8_t*)&(savedConfig.redx)},
  {0x0008,  DS_READWRITE, NAD_CAL           , 8u , (uint8_t*)&(savedConfig.greenx)},
  {0x0009,  DS_READWRITE, NAD_CAL           , 8u , (uint8_t*)&(savedConfig.bluex)},
  {0x000A,  DS_WRITE          , NAD_CAL     , 1u , &s_ledControlFlag},
  {0x000B,  DS_READWRITE, NAD_CAL           , 17u , (uint8_t*)&(savedProduct)}, //SA + GA + InCoef +PN
  {0x000C,  DS_READWRITE, NAD_CAL           , 24u , (uint8_t*)&(savedConfig.redx)},
	{0x0100,  DS_READ     , NAD_CAL           , 20u , (uint8_t*)(DebugLJM)},
  {0x6269,  DS_READWRITE, NAD_NORLCAL, DMAX_PART_NUMBER_LENGTH  , (uint8_t*)&(savedConfig.partNO[0])},
  {0x6469,  DS_READ           , NAD_NORLCAL          , DSOFTWARE_VERSION_LENGTH , (uint8_t*)&(softwareVersion[0])},
  {0x6669,  DS_READWRITE, NAD_NORLCAL, DMAX_PART_NUMBER_LENGTH  , (uint8_t*)&(savedConfig.partNO[0])},
  {0x6869,  DS_READWRITE           , NAD_NORLCAL          , DHARDWARE_VERSION_LENGTH , (uint8_t*)&(savedConfig.hardwareVersion[0])},
  {0x6A69,  DS_READWRITE, NAD_NORLCAL, DMAX_SERIAL_NUMBER_LENGTH, (uint8_t*)&(savedConfig.serialNO[0])},
	{0x6B69,  DS_READ,         NAD_CAL|NAD_NORL,      6u, 						(uint8_t*)&s_quiltCheck[0]},
  {0x6C69,  DS_READ           , NAD_NORL          , DCONTENT_6C69_LEGNTH     , (uint8_t*)&CONTENT_0x6c69[0]},
//	{0x6D69,  DS_READ,      NAD_CAL,          2u                        , (uint8_t*)&savedConfig.Volt_Califlag},
	{0x6E69,  DS_READ | DS_WRITE, NAD_CAL,           24u, 						(uint8_t*)&savedConfig.redx},
  {0x6F69,  DS_READ | DS_WRITE, NAD_CAL,           22u, 						(uint8_t*)&savedConfig.factor},//22
//  {0x7030,  DS_READ    , NAD_CAL  ,     20u     , ((uint8_t*)&savedConfig) + 0}, //crc and magic number can not be included
//  {0x7031,  DS_READ    , NAD_CAL   ,    20u     , ((uint8_t*)&savedConfig) + 20},
//  {0x7032,  DS_READ    , NAD_CAL   ,    20u     , ((uint8_t*)&savedConfig) + 40}, 
//  {0x7033,  DS_READ    , NAD_CAL   ,    20u     , ((uint8_t*)&savedConfig) + 60},
//  {0x7034,  DS_READ    , NAD_CAL   ,    20u     , ((uint8_t*)&savedConfig) + 80},
//  {0x7035,  DS_READ    , NAD_CAL   ,    20u     , ((uint8_t*)&savedConfig) + 100},
  
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
#if 0
/**
*@details   reset task control block
*
*@retval    None.
*/
static void resetTaskControlBlock(void)
{
  currentRequestTCB.RequstDID = 0;
  currentRequestTCB.amount = 0;
  currentRequestTCB.data = 0;
  currentRequestTCB.nadType = 0;
  currentRequestTCB.operationType = NAD_NORL;
  currentRequestTCB.pos = 0;
  currentRequestTCB.totalLength = 0;
}
#endif
/**
*@details   Do actual read/write service job.
*
*@param[in%out] tcb  read/write task control block.
*
*@param[in%out] length read/write length
*
*@retval    if this access succeed.1 = failed, 0 = success
*/
uint8_t systemDeratingGetTempstate(void);
static uint16_t __doRequest(struct RequsetTCB* tcb, uint16_t *length)
{
  uint16_t ret = 1,cnt;
  uint16_t index;
  uint16_t z = 0;
  //when a diagnostic frame is detected, map all diagnostic service stored in eeprom
  for (index = 0; index < DMAX_SERVICE_AMOUNT; index++)
  {
    /*CHECH attributes*/
    if ((serveceList[index].serviceID == tcb->RequstDID) &&
        (serveceList[index].rwAttr & tcb->operationType) &&
        (serveceList[index].nadAttr & tcb->nadType))
    {
      if ((tcb->operationType == DS_WRITE) &&
          (tcb->amount != serveceList[index].length))
      {
        return 2U;
      }
      ret = 0;
			if(tcb->RequstDID == 0x0100)
      {
//				DebugLJM[0] = (uint16_t)measureStackUsage();
				 DebugLJM[0] = (uint16_t)IoHwAb_Temperature_GetMcu();
				 DebugLJM[1] = (uint16_t)IoHwAb_Temperature_GetLed();
				 
				 DebugLJM[2] = (uint16_t)IoHwAb_Led_GetVoltage(ELedChannelRed);
				 DebugLJM[3] = (uint16_t)IoHwAb_Led_GetVoltage(ELedChannelGreen);
				 DebugLJM[4] = (uint16_t)IoHwAb_Led_GetVoltage(ELedChannelBlue);
////				 DebugLJM[3] = (uint16_t)ReturnADCBuffer(ADC_CH_VTEMP);
////				 DebugLJM[4] = (uint16_t)((measureStackUsage()>>16)&0x0000ffff);
//				
//				 //DebugLJM[4] = (uint16_t)(b&0x0000ffff);
////				 DebugLJM[4] = (uint16_t)ReturnADCBuffer(ADC_CH_VTEMP);
				 DebugLJM[5] = (uint16_t)IoHwAb_Led_GetPWM(ELedChannelRed);
				 DebugLJM[6] = (uint16_t)IoHwAb_Led_GetPWM(ELedChannelGreen);
				 DebugLJM[7] = (uint16_t)IoHwAb_Led_GetPWM(ELedChannelBlue);
//				 //DebugLJM[8] = (uint16_t)((b>>16)&0x0000ffff);//moduleBatGetVoltage();
				 DebugLJM[8] = (uint16_t)IoHwAb_Battery_GetVoltage();
//						DebugLJM[0] = (uint16_t)moduleBatGetVoltage();
				DebugLJM[9] = (uint16_t)systemDeratingGetTempstate();
      }
			if(tcb->RequstDID == 0x6B69)
			{
				s_quiltCheck[0] =
				    (uint16_t)IoHwAb_Led_GetStatus(ELedChannelRed);
				s_quiltCheck[1] =
				    (uint16_t)IoHwAb_Led_GetStatus(ELedChannelGreen);
				s_quiltCheck[2] =
				    (uint16_t)IoHwAb_Led_GetStatus(ELedChannelBlue);
			}
			
      if (tcb->operationType == DS_READ)
      {
        if (tcb->data != 0)
        {
          for(uint16_t j = 0; j < tcb->amount; j++)
          {
            if ((tcb->pos + j) < serveceList[index].length)
              tcb->data[j] = serveceList[index].addr[tcb->pos + j];
          }
        }else
          *length = serveceList[index].length;
      }

      if ((tcb->operationType == DS_WRITE) &&(serveceList[index].serviceID == 0x000A))
      {
        if (tcb->data != 0)
        {
          for(z = 0; z < tcb->amount; z++)
          {
            if ((tcb->pos + z) < serveceList[index].length)
              serveceList[index].addr[tcb->pos + z] = *(tcb->data + z);
          }
        }
        (void)IoHwAb_Led_SetCalibrationControl(s_ledControlFlag);
        return ret;
      }

      if ((tcb->operationType == DS_WRITE) &&!(flashFlag & EFlashUpdateLocked))
      {
        if (tcb->data != 0)
        {
          for(z = 0; z < tcb->amount; z++)
          {
            if ((tcb->pos + z) < serveceList[index].length)
              serveceList[index].addr[tcb->pos + z] = *(tcb->data + z);
          }
          /*if flash is idle, then set write flag*/          
          if(serveceList[index].serviceID == 0x000B )
          {           
            for(cnt=0;cnt<6;cnt++)
            {
             *(((uint8_t*)&(savedConfig.singleAddr)+cnt)) = *((uint8_t*)(&savedProduct) + cnt);//SA(2), GA(2), Factor(2)
            }
            for(cnt=0;cnt<11;cnt++)
            {
             *(((uint8_t*)&savedConfig.partNO)+ cnt) = *((uint8_t*)(&savedProduct)+ cnt+6) ;//SA(2), GA(2), Factor(2)
            }
          }

          if (flashFlag == EFlashUpdateFree)
             flashFlag |= EFlashUpdateProcessing;
                 
        }
      }
      break;
    } //if found a defined request 
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
uint16_t systemLinDiagHandleRecv(uint8_t* recvBuffer, uint16_t DID, uint16_t requestLength)
{
  currentRequestTCB.operationType = DS_WRITE;
  currentRequestTCB.RequstDID = DID;
  currentRequestTCB.totalLength = 0;
  currentRequestTCB.amount = requestLength - 3;
  currentRequestTCB.pos = 0;
  currentRequestTCB.data = &recvBuffer[3];

  if (Rte_Call_Diagnostic_GetNad() == 0xA0U)
    currentRequestTCB.nadType = NAD_CAL;
  else
    currentRequestTCB.nadType = NAD_NORL;
  
  return __doRequest(&currentRequestTCB, 0);
}

/**
*@details   LIN diagnostic sending call back function.
*
*@param[in] recvBuffer  Diagnostic received buffer..
*
*@retval    MELEXIS' response code.
*/
uint16_t systemLinDiagHandleSend(uint8_t* sendBuffer, uint16_t DID, uint16_t requestLength, uint16_t* respLength)
{
  sendBuffer[0] = 0x62;
  sendBuffer[1] = ((DID&0xff00)>>8);
  sendBuffer[2] = DID&0xff;
  
  currentRequestTCB.RequstDID = DID;
  currentRequestTCB.operationType = DS_READ;
  currentRequestTCB.pos = 0;
  currentRequestTCB.totalLength = 0;
  
  if (Rte_Call_Diagnostic_GetNad() == 0xA0U)
    currentRequestTCB.nadType = NAD_CAL;
  else
    currentRequestTCB.nadType = NAD_NORL;
  
  currentRequestTCB.data = 0;
  __doRequest(&currentRequestTCB, respLength);
  
  currentRequestTCB.data = &sendBuffer[3];
  currentRequestTCB.amount = *respLength;
  
  return __doRequest(&currentRequestTCB, 0);
}

uint32_t SID27_Seed = 0, SID27_Key = 0;
uint16_t systemLinDiag0x27Handle(uint8_t* recvBuffer, uint8_t DID, uint8_t* sendBuffer, uint16_t* respLength)
{
  uint8_t SSID = 0, ret_27 = 0;
  SSID = DID;

  uint32_t a;
  switch(SSID)
  {
  case 0x03:
    SID27_Seed = (uint32_t)IoHwAb_Time_Get();

    *respLength = 2 + 4;
    sendBuffer[0] = 0x67u;
    sendBuffer[1] = SSID;
    sendBuffer[2] = (uint8_t)(SID27_Seed & 0x000000FF);
    sendBuffer[3] = (uint8_t)((SID27_Seed & 0x0000FF00) >> 8);
    sendBuffer[4] = (uint8_t)((SID27_Seed & 0x00FF0000) >> 16);
    sendBuffer[5] = (uint8_t)((SID27_Seed & 0xFF000000) >> 24);

    ret_27 = 0;
    break;
  case 0x04:
    SID27_Key = (uint32_t)(((uint32_t)(recvBuffer[3]) | (uint32_t)(recvBuffer[4]<<8)));
    a = (uint32_t)((uint32_t)(recvBuffer[0]) | (uint32_t)(recvBuffer[2]<<8));
    SID27_Key += a;
    if (SID27_Key == (SID27_Seed + 0x0C04))
    {
      sendBuffer[0] = 0x67u;
      sendBuffer[1] = SSID;
      *respLength = 2;
      ret_27 = 0;
      savedConfig.ledSupplier &= 0xff7f;
      flashFlag = EFlashUpdateProcessing;
    }
    else
    {             
      sendBuffer[0] = 0x7Fu;
      sendBuffer[1] = 0x27u;
      sendBuffer[2] = 0x35u;
      *respLength = 3;
      ret_27 = 0;
    }
    SID27_Seed = 0;
    SID27_Key = 0;
   
    
    break;
  default:
    sendBuffer[0] = 0x7Fu;
    sendBuffer[1] = 0x27u;
    sendBuffer[2] = 0x12u;
    *respLength = 3;
    ret_27 = 0;
    SID27_Seed = 0;
    SID27_Key = 0;
    break;
  }
     
  return ret_27;
}

static EDiagServiceNad Swc_Diagnostic_GetNadType(uint8_t Nad)
{
  return (Nad == 0xA0U) ? NAD_CAL : NAD_NORL;
}

Std_ReturnType Swc_Diagnostic_ReadData(
    uint16_t Did, uint8_t Nad, uint8_t *DataPtr,
    uint16_t DataCapacity, uint16_t *DataLengthPtr)
{
  uint16_t length = 0U;

  if ((DataPtr == 0) || (DataLengthPtr == 0))
  {
    return E_NOT_OK;
  }
  currentRequestTCB.RequstDID = Did;
  currentRequestTCB.operationType = DS_READ;
  currentRequestTCB.nadType = Swc_Diagnostic_GetNadType(Nad);
  currentRequestTCB.pos = 0U;
  currentRequestTCB.totalLength = 0U;
  currentRequestTCB.amount = 0U;
  currentRequestTCB.data = 0;
  if ((__doRequest(&currentRequestTCB, &length) != 0U) ||
      (length > DataCapacity))
  {
    return E_NOT_OK;
  }
  currentRequestTCB.amount = (uint8_t)length;
  currentRequestTCB.data = DataPtr;
  if (__doRequest(&currentRequestTCB, 0) != 0U)
  {
    return E_NOT_OK;
  }
  *DataLengthPtr = length;
  return E_OK;
}

Std_ReturnType Swc_Diagnostic_WriteData(
    uint16_t Did, uint8_t Nad, const uint8_t *DataPtr,
    uint16_t DataLength)
{
  if ((DataPtr == 0) || (DataLength > 255U))
  {
    return E_NOT_OK;
  }
  currentRequestTCB.RequstDID = Did;
  currentRequestTCB.operationType = DS_WRITE;
  currentRequestTCB.nadType = Swc_Diagnostic_GetNadType(Nad);
  currentRequestTCB.pos = 0U;
  currentRequestTCB.totalLength = DataLength;
  currentRequestTCB.amount = (uint8_t)DataLength;
  currentRequestTCB.data = (uint8_t *)DataPtr;
  return (__doRequest(&currentRequestTCB, 0) == 0U) ? E_OK : E_NOT_OK;
}

Std_ReturnType Swc_Diagnostic_SecurityAccess(
    const uint8_t *RequestPtr, uint16_t RequestLength,
    uint8_t *ResponsePtr, uint16_t ResponseCapacity,
    uint16_t *ResponseLengthPtr)
{
  uint8_t request[5];
  uint16_t index;

  if ((RequestPtr == 0) || (RequestLength == 0U) ||
      (RequestLength > sizeof(request)) || (ResponsePtr == 0) ||
      (ResponseLengthPtr == 0) || (ResponseCapacity < 6U))
  {
    return E_NOT_OK;
  }
  for (index = 0U; index < RequestLength; index++)
  {
    request[index] = RequestPtr[index];
  }
  (void)systemLinDiag0x27Handle(request, request[0], ResponsePtr,
                                ResponseLengthPtr);
  return (*ResponseLengthPtr <= ResponseCapacity) ? E_OK : E_NOT_OK;
}

void Swc_Diagnostic_Init(void)
{
  s_ledControlFlag = 0U;
  s_quiltCheck[0] = 0U;
  s_quiltCheck[1] = 0U;
  s_quiltCheck[2] = 0U;
}

void Swc_Diagnostic_MainFunction(void)
{
  /* DCM and NvM main functions are scheduled centrally by BswM. */
}

Std_ReturnType Swc_Diagnostic_StoreNad(uint8_t Nad)
{
  if ((Nad < 1U) || (Nad > 16U))
  {
    return E_NOT_OK;
  }
  Swc_PersistentData_StoreDiagnosticNad(Nad);
  return E_OK;
}
