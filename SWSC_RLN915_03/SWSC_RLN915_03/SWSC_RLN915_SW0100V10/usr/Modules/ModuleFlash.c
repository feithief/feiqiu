/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleFlash.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains operations of flash.
  *Others:      None.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

#include "ModuleFlash.h"
//#include "api.h"
#include "ModuleWatchdog.h"
#include "SystemLinDiag.h"   //SW Version

#include "flash_device.h"

/*MAGIC number as well as version NO.*/
#define DMagicNumber  0xB2A1

/*
*@brief This variant shows current flash's content.
*@note It's dangerous to make this external, only for smaller code size.
*/
struct FlashContent savedConfig;
//struct DebugContent Debug;

/*
*@brief This variant shows flash's current status.
*@note It's dangerous to make this external, only for smaller code size.
*/
volatile EFlashUpdate flashFlag;

/*
*@brief This variant indicates if slave single address has been updated.
*@warning Do not delete this flag, sometime we change slave address to 0xA0 
  in order to know whether flash need to be updated.
*@note It's dangerous to make this external, only for smaller code size.
*/
volatile uint16_t lastSavedSingalAdress;

/*
*@brief This variant shows current module's type.
*@note It's dangerous to make this external, only for smaller code size.
*/
volatile EHardwareType moduleType;

static void moduleFlashLoad(SFlashContent  *data);


/**
*@details   Initialize flash content.
*
*@warning   This function can not be invoked in interrupt.
*
*@retval    None.
*/
//[0]是默认值， 用户vehicle type从1开始
const uint16_t cFactorList[VehicleNum] = 
{10082, 2646, 3150, 2394, 10082, 3528, 32768,
 32768, 32768, 32768, 32768};
uint16_t factor[6] = {10082, 2646, 3150, 2394, 10082, 3528};
const uint8_t cMimUserLList[VehicleNum] = 
{17, 19, 56, 63, 17, 57, 0,
 0, 0, 0, 0};


/*
const uint16_t cMimUserLList[VehicleNum] = 
{20, 20, 56, 64, 58, 0,
 0, 0, 0, 0, 0};
*/

void moduleFlashInit(void)
{
    uint16_t cnt;
  moduleFlashLoad(&savedConfig);
  
  //instantCRCcalculation = usMBCRC16((uint8_t*)&savedConfig +2, sizeof(SFlashContent ) - 2);
  if (savedConfig.magicNO != DMagicNumber) //+ whether crc is correct
//  if (0)
  {
    savedConfig.magicNO = DMagicNumber;
    savedConfig.ContentSize = (uint8_t)sizeof(SFlashContent);
    savedConfig.ParasNO = 1;
    //savedConfig.platform = EPlatformTypeMQB;
    //savedConfig.CtrlFlag = 0x0;   //  original led supplier
    savedConfig.CtrlFlag.num = 0;
    savedConfig.CtrlFlag.bit.IsLocked = 0;
    savedConfig.CtrlFlag.bit.IsNormalVW = 1;
    savedConfig.CtrlFlag.bit.IsTimeBasisValid = 0;
    
    
    savedConfig.singleAddr = 0x17;
    savedConfig.groupAddr = 0x80;
    
    memcpy(savedConfig.Lfactor, cFactorList, sizeof(cFactorList));
    memcpy(savedConfig.MinUsrL, cMimUserLList, sizeof(cMimUserLList));

    savedConfig.redx = 6939;
    savedConfig.redy = 3058;
    savedConfig.redY = 11810;

    savedConfig.greenx = 1628;
    savedConfig.greeny = 7248;
    savedConfig.greenY = 19810;

    savedConfig.bluex = 1542;
    savedConfig.bluey = 246;
    savedConfig.blueY = 2985;

    memcpy(savedConfig.serialNO , "12345678900987654321", DMAX_SERIAL_NUMBER_LENGTH);
    memcpy(savedConfig.partNO , "123456789012" , DMAX_PART_NUMBER_LENGTH);
    memcpy(savedConfig.hardwareVersion , "HW0100" , DHARDWARE_VERSION_LENGTH);

    savedConfig.RthermolRatio = 48;//48;
    savedConfig.GthermolRatio = 76;//76;
    savedConfig.BthermolRatio = 54;//34;


//configuration for RLN211
    savedConfig.HWResp = (uint8_t)(savedConfig.hardwareVersion[2] - 0x30);
    savedConfig.SWResp = (uint8_t)(softwareVersion[2]-0x30);

    savedConfig.MCUthermolRatio = 20;//68;
    savedConfig.RatingLowerVoltage = 8;
    savedConfig.RatingHigherVoltage = 1;

    
        moduleFlashSave();
    
  } 
  savedConfig.SubVersion = 0x10; //第三版移植到英迪芯
  lastSavedSingalAdress = savedConfig.singleAddr;
}

/**
*@details   Save current ram content to flash.
*
*@warning   This function can not be invoked in interrupt.
*
*@retval    None.
*/
//SFlashContent  temp_content;
void moduleFlashSave(void)
{
    SFlashContent saveconfig_0;
    moduleFlashLoad(&saveconfig_0);
//    int p;
//    for (p = 0; p<6; p++)
//    {
//        if(savedConfig.Lfactor[p] == 0 )
//        {
//            savedConfig.Lfactor[p] = saveconfig_0.Lfactor[p];
//        }
//    }
    
//        int b, c = 0;
//      for (b=0; b<6; b++)
//      {
//          if (factor[b] != savedConfig.Lfactor[b])
//              c = 1;
//        
//      }
      savedConfig.Lfactor[4] = savedConfig.Lfactor[0];
      
//      if (1 == c)
      {
//          if(savedConfig.Lfactor[1] == factor[1])
          {
                savedConfig.Lfactor[1] = (uint16_t)(((float)savedConfig.Lfactor[0]) * (0.21 / 0.8));
          }
//          if(savedConfig.Lfactor[2] == factor[2])
          {
                savedConfig.Lfactor[2] = (uint16_t)(((float)savedConfig.Lfactor[0]) * (0.25 / 0.8));
          }

//          if(savedConfig.Lfactor[3] == factor[3])
          {
                savedConfig.Lfactor[3] = (uint16_t)(((float)savedConfig.Lfactor[0]) * (0.19 / 0.8));
          }

//          if(savedConfig.Lfactor[5] == factor[5])
          {
                savedConfig.Lfactor[5] = (uint16_t)(((float)savedConfig.Lfactor[0]) * (0.28 / 0.8));
          }
      }
      
         
                
      

      uint16_t DataSize;
      SFlashContent *savedconfigptr = &savedConfig;
      
      DataSize = sizeof(SFlashContent); //write in data and crc

      //calcute crc
      //savedConfig.crc=usMBCRC16(((uint8_t*)&savedConfig) + 2 , sizeof(SFlashContent) - 2 );
      moduleWatchdogFeed();
     

            
      
     (void)Flash_devCopyToFlash(FLASH_START_ADDRESS_USER_DATA, (uint8_t *) savedconfigptr, DataSize);   // read verne.h
     
      moduleWatchdogFeed();
      /*ledSupplier & 0x00f0u == 0x0080 means flash content has been locked, */
      /*can not be changed in any circumstances*/
      if (savedConfig.CtrlFlag.bit.IsLocked  ==  1)
      {
        flashFlag |= EFlashUpdateLocked;
      }
      else
      {
        flashFlag &= ~EFlashUpdateLocked;
      }
//      int a;
//      for (a=0; a<6; a++)
//      {
//        factor[a] = savedConfig.Lfactor[a];
//      }
    
}

/**
*@details   Load current flash content to ram.
*
*@warning   This function can not be invoked in interrupt.
*
*@retval    None.
*/
static void moduleFlashLoad(SFlashContent  *data)
{
  uint16_t DataSize = sizeof(SFlashContent);

  memcpy(data, (void const*)FLASH_START_ADDRESS_USER_DATA, DataSize);  // read verne.h
  
  /*ledSupplier & 0x00f0u == 0x0080 means flash content has been locked, */
  /*can not be changed in any circumstances*/
  if (savedConfig.CtrlFlag.bit.IsLocked  ==  1u)
  {
    flashFlag |= EFlashUpdateLocked;
  }
  else
  {
    flashFlag &= ~EFlashUpdateLocked;
  }
}

void moduleFlashLoad1(SFlashContent  *data)
{
    moduleFlashLoad(data);
}

/*
*@details   Our own string CRC calculation function.
*
*@retval    CRC value.
*/
uint16_t usMBCRC16(uint8_t * pucFrame, uint8_t usLen)
{
  uint8_t ucCRCHi = 0xFF;
  uint8_t ucCRCLo = 0xFF;
  uint16_t iIndex;
  while (usLen--)
  {
    iIndex = ucCRCLo ^ *(pucFrame++);
    ucCRCLo = (uint8_t)(ucCRCHi ^ aucCRCHi[iIndex]);
    ucCRCHi = aucCRCLo[iIndex];
  }
  return (uint16_t)((ucCRCHi << 8) | ucCRCLo);
}



