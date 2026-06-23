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
#include "ModuleClock.h"
#include "ModuleBattery.h"
#include "lin_driver_api.h"
#include "lin_app.h"
#include "SystemLinComm.h"
#include "SystemDerating.h"
#include "lin_autoaddressing.h"
#include "genLinConfig.h"
#include "lin_main.h"

/**@brief software VERSION section length.*/
#define DSOFTWARE_VERSION_LENGTH  5U

/**@brief module name section length.*/
const uint8_t softwareVersion[DSOFTWARE_VERSION_LENGTH + 1] = "S0700";    /* 5 bytes */

uint16_t Debug[12] = {0};
/**@brief Service list.*/
#define DMAX_SERVICE_AMOUNT 24//15
//extern int16_t debugTemp, debugJunction;
//#include <stdint.h>

//#define STACK_SIZE 0x400 // ջ�Ĵ�С
//extern uint32_t __initial_sp; // ջ����ʼ��ַ

//// ��ʼ��ջ�ռ�Ϊ�ض�ֵ
//void initStackSpace(void) {
//    volatile uint32_t *stackPtr;
//    // ����ջ�׵ĵ�ַ
//    volatile uint32_t *stackBottom = (volatile uint32_t *)((uint32_t)&__initial_sp - STACK_SIZE);
//    // ���ض��ķ���ֵ��ʼ��ջ�ռ�
//    for (stackPtr = stackBottom; stackPtr < &__initial_sp; ++stackPtr) {
//        *stackPtr = 0xCCCCCCCC; // ʹ��0xCC��Ϊջ�ռ�ĳ�ʼ���?
//    }
//}

//// ����ջ��ʹ�����?
//uint32_t measureStackUsage(void) {
//    volatile uint32_t *stackPtr;
//    uint32_t used = 0;

//    // ����ջ�׵ĵ�ַ
//    volatile uint32_t *stackBottom = (volatile uint32_t *)(((uint32_t)&__initial_sp) - STACK_SIZE);

//    // ��ջ�׿�ʼ����ɨ�裬���ҵ�һ���������ض���ʼ��ֵ�ĵ�ַ����ʾջ��ʹ�ÿ�ʼ
//    for (stackPtr = stackBottom; stackPtr < &__initial_sp; ++stackPtr) {
//        if (*stackPtr != 0xCCCCCCCC) { // ���ҵ�һ�����ǳ�ʼ��ֵ�ĵ�ַ
//            used = (uint32_t)(&__initial_sp - stackPtr);
//            break; // �ҵ�ջʹ�õ���ʼ�������ѭ��?
//        }
//    }

//    return used * sizeof(uint32_t); // ������ʹ�õ�ջ�ռ��С����λΪ�ֽ�?
//}








static const SLinDiagSevice serveceList[DMAX_SERVICE_AMOUNT] =
{
  {0x0002,  DS_READ | DS_WRITE, NAD_CAL,			2u,							(uint8_t*)&(savedConfig.ledSupplier)},
  {0x0003,  DS_READ | DS_WRITE | DS_Nolock, NAD_CAL,2u,							(uint8_t*)&(savedConfig.eolAddr)},
  {0x0005,  DS_READ | DS_WRITE, NAD_CAL,			8u, 						(uint8_t*)&(savedConfig.whitex)},
  {0x0006,  DS_READ | DS_WRITE, NAD_CAL           , 2u,							(uint8_t*)&(savedConfig.factor)},
  {0x0007,  DS_READ | DS_WRITE, NAD_CAL, 			8u, 						(uint8_t*)&(savedConfig.redx)},
  {0x0008,  DS_READ | DS_WRITE, NAD_CAL, 			8u, 						(uint8_t*)&(savedConfig.greenx)},
  {0x0009,  DS_READ | DS_WRITE, NAD_CAL,			8u, 						(uint8_t*)&(savedConfig.bluex)},
  {0x000A,  DS_WRITE          , NAD_CAL, 			2u, 						(uint8_t*)&(ledControlFlag)},
  {0x0100,  DS_READ          , NAD_CAL, 			16u, 						(uint8_t*)&(Debug)},
  {0x5000,	DS_READ | DS_WRITE, NAD_CAL,            2u,                         (uint8_t*)&(savedConfig.resetNO)},
  {0x1111,  DS_READ           , NAD_CAL, 		2, 						(uint8_t*)&(savedConfig.u)},
  {0x2222,	DS_READ, NAD_CAL,            10u,                         (uint8_t*)&(savedConfig.U)},
  {0x3333,	DS_READ, NAD_CAL,            10u,                         (uint8_t*)&(savedConfig.V)},
  {0x6269,  DS_READ | DS_WRITE, NAD_CAL,            DMAX_PART_NUMBER_LENGTH, 	(uint8_t*)&(savedConfig.partNO[0])},
  {0x6469,  DS_READ           , NAD_CAL,            DSOFTWARE_VERSION_LENGTH, 	(uint8_t*)&(softwareVersion[0])},
  {0x6869,  DS_READ | DS_WRITE, NAD_CAL,            DHARDWARE_VERSION_LENGTH, 	(uint8_t*)&(savedConfig.hardwareVersion[0])},
  {0x6A69,  DS_READ | DS_WRITE, NAD_CAL,            DMAX_SERIAL_NUMBER_LENGTH, 	(uint8_t*)&(savedConfig.serialNO[0])},
  {0x6B69,  DS_READ,         	NAD_CAL,      		6u, 						(uint8_t*)&(Quiltcheck)},
  {0x6E69,  DS_READ | DS_WRITE, NAD_CAL,            24u, 						(uint8_t*)&savedConfig.redx},
  {0x6F79,  DS_READ | DS_WRITE, NAD_CAL,            16u, 						(uint8_t*)&savedConfig.partNO[0]},
  {0x1234,  DS_READ, 			NAD_CAL,            18u, 						(uint8_t*)&savedConfig.pART},
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
extern tick_t currentTick;
extern l_u16 BusIdleTimeOutCounter;
static uint16_t __doRequest(struct RequsetTCB* tcb, uint16_t *length)
{
  uint16_t ret = 1;
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
      ret = 0;
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

      if (serveceList[index].serviceID == 0x000A)
      {
        if (tcb->data != 0)
        {
          for(z = 0; z < tcb->amount; z++)
          {
            if ((tcb->pos + z) < serveceList[index].length)
              serveceList[index].addr[tcb->pos + z] = *(tcb->data + z);
          }
        }
        return ret;
      }

      if ((tcb->operationType == DS_WRITE)&& !(flashFlag & EFlashUpdateLocked))
      {
        if (tcb->data != 0)
        {
          for(z = 0; z < tcb->amount; z++)
          {
            if ((tcb->pos + z) < serveceList[index].length){
              serveceList[index].addr[tcb->pos + z] = *(tcb->data + z);
//              if(tcb->RequstDID == 0x0003)
//            	  savedConfig.pART = Slave_Address_eol;
            }
          }
          /*if flash is idle, then set write flag*/
          if (flashFlag == EFlashUpdateFree)
            flashFlag = EFlashUpdateProcessing;
        }
      }
	  if (savedConfig.resetNO == EStartreset)
	  {
		  savedConfig.resetNO = Edefaultreset;
		  savedConfig.AutoAddr = 0x10u;
		  // savedConfig.eolAddr  = 1u;
		  savedConfig.pART = Slave_Address_auto;
            flashFlag = EFlashUpdateProcessing;
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
uint16_t systemLinDiagHandleRecv(uint8_t* recvBuffer, uint16_t DID, uint16_t requestLength)
{
  currentRequestTCB.operationType = DS_WRITE;
  currentRequestTCB.RequstDID = DID;
  currentRequestTCB.totalLength = 0;
  currentRequestTCB.amount = requestLength - 3;
  currentRequestTCB.pos = 0;
  currentRequestTCB.data = &recvBuffer[3];

  if (l_get_current_NAD() == 0xA0)
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
  
  if (l_get_current_NAD() == 0xA0)
    currentRequestTCB.nadType = NAD_CAL;
  else
    currentRequestTCB.nadType = NAD_NORL;
  
  currentRequestTCB.data = 0;
  __doRequest(&currentRequestTCB, respLength);
  
  currentRequestTCB.data = &sendBuffer[3];
  currentRequestTCB.amount = *respLength;
  
  return __doRequest(&currentRequestTCB, 0);
}
/**
*@details   update LIN slave response signal.
*
*@retval    None.
*/
#define BATTER_VOLT_NORMAL_CAP 		16000
#define BATTER_VOLT_NORMAL_BUTTOM 	9000

void systemLinCommRespUpdate(void)
{
	Rsp_Frame *frame_data = (Rsp_Frame *)l_LinData.frames.l_frm_RGBs_Slave_08a.frame_data;
    CMSIS_Irq_Dis();
    l_u8_wr_RGB_Slave_08a_TempState(systemDeratingGetTempstate());
    CMSIS_Irq_En();
    CMSIS_Irq_Dis();
    l_u8_wr_RGB_Slave_08a_HelligkeitsFaktor( ((uint8_t)savedConfig.brightness_factor & (uint8_t)0x7fu) );
    CMSIS_Irq_En();
    CMSIS_Irq_Dis();
    l_u8_wr_RGB_Slave_08a_HW_Version((uint8_t)(savedConfig.hardwareVersion[2] - (uint8_t)0x30));
    CMSIS_Irq_En();
    CMSIS_Irq_Dis();
    l_u8_wr_RGB_Slave_08a_SW_Version((uint8_t)softwareVersion[2] - (uint8_t)0x30);
    CMSIS_Irq_En();

	if(savedConfig.pART == Slave_Address_eol)
	{
    CMSIS_Irq_Dis();
		l_u8_wr_RGB_Slave_08a_StoredNAD(0xFFu);
    CMSIS_Irq_En();
	}
	else
	{
    CMSIS_Irq_Dis();
		l_u8_wr_RGB_Slave_08a_StoredNAD(savedConfig.AutoAddr);
    CMSIS_Irq_En();
	}

	if(moduleLedGetStatus(ELedChannelRed) == ELedStatusInit && moduleLedGetStatus(ELedChannelGreen) == ELedStatusInit && moduleLedGetStatus(ELedChannelBlue) == ELedStatusInit)
	{
		/*UN-INIT STATE, UPDATE NO MATTER VOLTAGE*/
		if((l_LinData.frames.l_frm_RGBs_Slave_08a.frame_data[0] & (252u)) == Fehler || 
			 (l_LinData.frames.l_frm_RGBs_Slave_08a.frame_data[0] & (252u)) == kein_Fehler)
		{
			/*recovering, don't update signal*/

		}
		else
		{
			/*wake-up*/
      CMSIS_Irq_Dis();
			l_u8_wr_RGB_Slave_08a_IntError(LED_init);
      CMSIS_Irq_En();
		}
	}
	else
	{
		if(( moduleBatGetVoltage() <= BATTER_VOLT_NORMAL_CAP && BATTER_VOLT_NORMAL_BUTTOM <= moduleBatGetVoltage() ))
		{
			if((moduleLedGetStatus(ELedChannelRed )& ELedStatusOpen)||(moduleLedGetStatus(ELedChannelGreen) & ELedStatusOpen)||(moduleLedGetStatus(ELedChannelBlue) & ELedStatusOpen) ||
					(moduleLedGetStatus(ELedChannelRed )& ELedStatusShort)||(moduleLedGetStatus(ELedChannelGreen) & ELedStatusShort)||(moduleLedGetStatus(ELedChannelBlue) & ELedStatusShort))
			{
					/*normal*/
          CMSIS_Irq_Dis();
					l_u8_wr_RGB_Slave_08a_IntError(Fehler);
          CMSIS_Irq_En();
			}
			else
			{
				/*no open short*/
        CMSIS_Irq_Dis();
				l_u8_wr_RGB_Slave_08a_IntError(kein_Fehler);
        CMSIS_Irq_En();
			}
		}
		else
		{
			/*only update under normal voltage or wakeup*/
			
		}
	}
}

uint32_t SID27_Seed = 0, SID27_Key = 0;
uint16_t systemLinDiag0x27Handle(uint8_t* recvBuffer, uint8_t DID, uint8_t* sendBuffer, uint16_t* respLength)
{
  uint8_t SSID = 0, ret_27 = 0;
  SSID = DID;

  switch(SSID)
  {
  case 0x03:
    SID27_Seed = (uint32_t)moduleClockGet();

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
    SID27_Key = (uint32_t)(((uint32_t)(recvBuffer[1]) | (uint32_t)(recvBuffer[2]<<8)));
    if ((SID27_Key & 0x0000FFFF) == ((SID27_Seed + 0x0C04) & 0x0000FFFF))
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
      *respLength = 2;
      ret_27 = 0;
    }
    SID27_Seed = 0;
    SID27_Key = 0;
    
   
    
    break;
  default:
    sendBuffer[0] = 0x7Fu;
    sendBuffer[1] = 0xB2u;
    *respLength = 2;
    ret_27 = 0;
    SID27_Seed = 0;
    SID27_Key = 0;
    break;
  }
     
  return ret_27;
}


