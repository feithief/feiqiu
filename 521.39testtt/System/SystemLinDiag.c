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
#include "ModuleAdc.h"
//#include "lin_driver_api.h"
//#include "lin_app.h"
#include "LinDrvImp_Config.h"
#include "ModuleClock.h"
/**@brief software VERSION section length.*/
#define DSOFTWARE_VERSION_LENGTH  5U

/**@brief module name section length.*/

#define DDIAGNOSTIC_ADDR          0xA0U

const uint8_t softwareVersion[DSOFTWARE_VERSION_LENGTH] = "S0400";    /* 5 bytes */

//*****************debug*************************//
uint16_t Debug_0100[8] = {0};

//*****************debug*************************//

/**@brief Service list.*/
#define DMAX_SERVICE_AMOUNT 15//15
static const SLinDiagSevice serveceList[DMAX_SERVICE_AMOUNT] =
{
    {0x0002,  DS_READWRITE, NAD_CAL           , 2u                        , (uint8_t*)&(savedConfig.ledSupplier)          },
    {0x0003,  DS_READWRITE, NAD_CAL           , 2u                        , (uint8_t*)&(savedConfig.singleAddr)           },
    {0x0004,  DS_READWRITE, NAD_CAL           , 2u                        , (uint8_t*)&(savedConfig.groupAddr)            },
    /*The old version this section has two bytes, we add one reserved byte to prevent overflow.*/
    {0x0005,  DS_READWRITE, NAD_CAL           , 2u                        , (uint8_t*)&(savedConfig.platform)             },
    {0x0006,  DS_READWRITE, NAD_CAL           , 2u                        , (uint8_t*)&(savedConfig.factor)               },
    {0x0007,  DS_READWRITE, NAD_CAL           , 8u                        , (uint8_t*)&(savedConfig.redx)                 },
    {0x0008,  DS_READWRITE, NAD_CAL           , 8u                        , (uint8_t*)&(savedConfig.greenx)               },
    {0x0009,  DS_READWRITE, NAD_CAL           , 8u                        , (uint8_t*)&(savedConfig.bluex)                },
    {0x000A,  DS_WRITE    , NAD_CAL           , 1u                        , (uint8_t*)&(ledControlFlag)                   },
    {0x0100,  DS_READ     , NAD_NORLCAL       , 16u                       , (uint8_t*)&(Debug_0100)                  },
    //  {0x0103,  DS_READ    , NAD_NORLCAL       , 6u                       , (uint8_t*)&(Debug.VledR)                  },
    // {0x000C,  DS_READ   , NAD_NORLCAL       , 2u                        , (uint8_t*)&(savedConfig.Reserved2)   },
    {0x6269,  DS_READWRITE, NAD_CAL           , DMAX_PART_NUMBER_LENGTH   , (uint8_t*)&(savedConfig.partNO[0])            },
    {0x6469,  DS_READ     , NAD_CAL           , DSOFTWARE_VERSION_LENGTH  , (uint8_t*)&(softwareVersion[0])               },
    //{0x6669,  DS_READWRITE, NAD_CAL         , DMAX_PART_NUMBER_LENGTH   , (uint8_t*)&(savedConfig.partNO[0])            },
    {0x6869,  DS_READWRITE, NAD_CAL           , DHARDWARE_VERSION_LENGTH  , (uint8_t*)&(savedConfig.hardwareVersion[0])   },
    {0x6A69,  DS_READWRITE, NAD_CAL           , DMAX_SERIAL_NUMBER_LENGTH , (uint8_t*)&(savedConfig.serialNO[0])          },
    //{0x6C69,  DS_READ     , NAD_CAL         , DCONTENT_6C69_LEGNTH      , (uint8_t*)&CONTENT_0x6c69[0]                  },        // delete
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
    uint16_t z = 0;
    //when a diagnostic frame is detected, map all diagnostic service stored in eeprom
    for (index = 0; index < DMAX_SERVICE_AMOUNT; index++)
    {
        /*CHECH attributes*/
        if ((serveceList[index].serviceID == tcb->RequstDID) &&
            (serveceList[index].rwAttr & tcb->operationType)/* &&
                (serveceList[index].nadAttr & tcb->nadType)*/)
        {
            ret = 0;
            if(tcb->RequstDID == 0x0100)
            {
                Debug_0100[0] = savedConfig.Cali_Volt[0];
                Debug_0100[1] = savedConfig.Cali_Volt[1];
                Debug_0100[2] = savedConfig.Cali_Volt[2];
                Debug_0100[3] = moduleLedGetVoltage(ELedChannelRed);
                Debug_0100[4] = moduleLedGetVoltage(ELedChannelGreen);
                Debug_0100[5] = moduleLedGetVoltage(ELedChannelBlue);
                Debug_0100[6] = moduleTempMcuGet();
                Debug_0100[7] = savedConfig.Cali_Temp;     
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
            
            if ((tcb->operationType == DS_WRITE) &&
                (serveceList[index].serviceID == 0x000A))
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
            
            if ((tcb->operationType == DS_WRITE) && !(flashFlag & EFlashUpdateLocked))
            {
                if (tcb->data != 0)
                {
                    for(z = 0; z < tcb->amount; z++)
                    {
                        if ((tcb->pos + z) < serveceList[index].length)
                            serveceList[index].addr[tcb->pos + z] = *(tcb->data + z);
                    }
                    /*if flash is idle, then set write flag*/
                    if (flashFlag == EFlashUpdateFree)
                        flashFlag |= EFlashUpdateProcessing;
                }
            }
            //Add light weight calibration function
            
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
//  DS_READ   = 0x1,  /**<service has write property.*/
//  DS_WRITE  = 0x2,  /**<service has read property.*/
//  DS_READWRITE = DS_READ|DS_WRITE,
    currentRequestTCB.operationType = DS_WRITE;
    currentRequestTCB.RequstDID = DID;
    currentRequestTCB.totalLength = 0;
    currentRequestTCB.amount = requestLength - 2;
    currentRequestTCB.pos = 0;
    currentRequestTCB.data = &recvBuffer[2];
    //ygh Add
    if (recvBuffer[1] == 0x02 && recvBuffer[2] == 0x82)
    {
        savedConfig.ledSupplier =  0x0080u;
        flashFlag |= EFlashUpdateLocked;
        moduleFlashSave();
    }
        
    
    //    if (l_get_current_NAD() == 0xA0)
    //        currentRequestTCB.nadType = NAD_CAL;
    //    else
    //        currentRequestTCB.nadType = NAD_NORL;
    
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
    sendBuffer[0] = ((DID&0xff00)>>8);
    sendBuffer[1] = DID&0xff;
    
    currentRequestTCB.RequstDID = DID;
    currentRequestTCB.operationType = DS_READ;
    currentRequestTCB.pos = 0;
    currentRequestTCB.totalLength = 0;
    
    //  if (l_get_current_NAD() == 0xA0)
    //    currentRequestTCB.nadType = NAD_CAL;
    //  else
    //    currentRequestTCB.nadType = NAD_NORL;
    
    currentRequestTCB.data = 0;
    __doRequest(&currentRequestTCB, respLength);
    
    currentRequestTCB.data = &sendBuffer[2];
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
    SID27_Seed = (uint32_t)moduleClockGet();

    *respLength = 2 + 4;
    sendBuffer[0] = 0x27u;
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
      sendBuffer[0] = 0x27u;
      sendBuffer[1] = SSID;
      *respLength = 2;
      ret_27 = 0;
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
    savedConfig.ledSupplier &= 0xff7f;
    flashFlag = EFlashUpdateFree;
   
    
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