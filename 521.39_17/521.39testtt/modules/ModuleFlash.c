/***************************************************************************//**
* @file		mod_flash.c
*
* @creator		wca
* @created		2022.06.23
*
* @brief  		Top application layer with main() entry point.
*
* @purpose             Hardware and LIN driver intialization, 
*                      top application layer.
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
* $Id: $
*
* $Revision:  $
*
******************************************************************************/
#include "ModuleWatchdog.h"
#include "ModuleFlash.h"
#include "LinDrvImp_Config.h"
#include <string.h>

/*MAGIC number as well as version NO.*/
#define DMagicNumber  611



/*
*@brief This variant shows current flash's content.
*@note It's dangerous to make this external, only for smaller code size.
*/
struct FlashContent savedConfig;
SLineContent savedProduct;
volatile EHardwareType moduleType;
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
uint16_t lastSavedSingalAdress;



static uint8_t GbTestEn = 0;
static uint8_t GucFlashEnvData[96];
const uint16_t GusFlashArea[256/2] @0x8200 = {
    0x0000, 0xFF00,
};


flash_Passphrases_t GsFlashPass = {
    .ExternalPassphrase = { 0x24fa, 0x0dfd, 0x0599 },
    .Verify             = { 0xccb4, 0xa0d9, 0xb6ea },
    .Stop               = { 0x5384, 0xf457, 0x5dcd },
};

/*
*@details   Our own string CRC calculation function.
*
*@retval    CRC value.
*/
#if 0
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
#endif

uint8_t flashTest(void)
{
    flash_ErrorCode_t eErrCode;
    uint8_t ucErasedPage = 0;
    uint8_t ucWriteData[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    
    //    if ( 0 == GbTestEn ) {
    //        return 0xFF;
    //    }
    GbTestEn = 0;
    
    eErrCode = FlashImp_IfFuncs->MemoryErasePage( &GucFlashEnvData[0], &GsFlashPass, &GusFlashArea[0], sizeof(GusFlashArea), &ucErasedPage );
    if ( flash_eErrorCode_NoError != eErrCode ) {
        return eErrCode;
    }
    
    eErrCode = FlashImp_IfFuncs->MemoryWrite( &GucFlashEnvData[0], &GsFlashPass, &ucWriteData[0], &GusFlashArea[0], sizeof(ucWriteData) );
    if ( flash_eErrorCode_NoError != eErrCode ) {
        return eErrCode;
    }
    
    return flash_eErrorCode_NoError;
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
    uint16_t Datasize = sizeof(SFlashContent);
    uint16_t addr;
    for(addr=0; addr < Datasize; addr++)
    {
        *((uint8_t*)data + addr) = flashByteRead(addr);
    }
    
    /*ledSupplier & 0x00f0u == 0x0080 means flash content has been locked, */
    /*can not be changed in any circumstances*/
    if ((savedConfig.ledSupplier & 0x00f0u) ==  0x0080u)
    {
        flashFlag |= EFlashUpdateLocked;
    }
    else
    {
        flashFlag &= ~EFlashUpdateLocked;
    }
}



/**
*@details   Save current ram content to flash.
*
*@warning   This function can not be invoked in interrupt.
*
*@retval    None.
*/
void moduleFlashSave(void)
{
    flash_ErrorCode_t eErrCode;
    
    uint16_t DataLength;
    uint8_t progIndex = 0;
    SFlashContent *savedconfigptr=&savedConfig;
    uint32_t *ptrFlashConfig = (uint32_t *)savedconfigptr;
    SFlashContent  temp_content;
    DataLength = sizeof(SFlashContent);
    if(DataLength % 4 != 0)
    {
        DataLength /= 4;
        DataLength += 1;
        DataLength = DataLength * 4;
    }
    //uint8_t ucErasedPage = 0;
    
    //put all content in flash into struct temp_content, including the stored crc. compare the struct temp_content and saveConfig(which gets values in FlashInit(), or changes its value in other modules)
    moduleFlashLoad(&temp_content);
    
    /*if content do not been changed, we do not update flash.*/
    
    
    
    moduleWatchdogFeed();
    //modify
    MemoryEraseAndWrite((uint32_t*)&temp_content,
                 ptrFlashConfig,
                 DataLength);
    
    /*ledSupplier & 0x00f0u == 0x0080 means flash content has been locked, */
    /*can not be changed in any circumstances*/
    if ((savedConfig.ledSupplier & 0x00f0u) ==  0x0080u)
    {
        flashFlag |= EFlashUpdateLocked;
    }
    else
    {
        flashFlag &= ~EFlashUpdateLocked;
    }
}

/**
*@details   Initialize flash content.
*
*@warning   This function can not be invoked in interrupt.
*
*@retval    None.
*/

void moduleFlashInit(void)
{
    moduleFlashLoad(&savedConfig);
    if(savedConfig.platform == 9) 
    {
        savedConfig.magicNO = 7; //DebugLJM
    }

    moduleType = EHardwareTypeNoCap;
    if (savedConfig.magicNO != DMagicNumber) 
    {
        savedConfig.magicNO = DMagicNumber;
        savedConfig.platform = EPlatformTypeMLBevoVW;
        if ((savedConfig.ledSupplier & 0x00f0u) ==  0x0080u)
        {
            flashFlag |= EFlashUpdateLocked;
            
        }
        else
        {
            flashFlag &= ~EFlashUpdateLocked;
            savedConfig.ledSupplier = 0x2;
        }
            
        
            
        savedConfig.singleAddr = 0x8;
        savedConfig.groupAddr = 0x1;
        savedConfig.factor = 0x8000;
        
		savedConfig.redx = 7033;
		savedConfig.redy = 2959;
		savedConfig.redY = 17046;
		savedConfig.greenx = 1540;
		savedConfig.greeny = 7288;
		savedConfig.greenY = 37603;
		savedConfig.bluex = 1508;
		savedConfig.bluey = 325;
		savedConfig.blueY = 6327;
        
		memcpy((uint8_t*)savedConfig.partNO , "123456789012", DMAX_PART_NUMBER_LENGTH);
		memcpy((uint8_t*)savedConfig.hardwareVersion , "H04", DHARDWARE_VERSION_LENGTH);
		memcpy((uint8_t*)savedConfig.serialNO , "12345678900987654321", DMAX_SERIAL_NUMBER_LENGTH);
        
        savedConfig.RthermolRatio = 48;
        savedConfig.GthermolRatio = 76;
        savedConfig.BthermolRatio = 34;
        
        savedConfig.ProductID[0] = 0x84;
        savedConfig.ProductID[1] = 0x00;
        savedConfig.ProductID[2] = 0x03;
        savedConfig.ProductID[3] = 0x00;
        savedConfig.ProductID[4] = 0x01;
        
        savedConfig.hardwareType = EHardwareTypeNoCap;
        savedConfig.MCUthermolRatio = 0x4C;
        savedConfig.RatingLowerVoltage = 8;
        savedConfig.RatingHigherVoltage = 1;
        //savedConfig.RCapCompensate = 1;
        //savedConfig.GCapCompensate = 1;
        //savedConfig.BCapCompensate = 1;
        
		/* 写入所有内容 调用硬件EEPROM写入接口 */
        if (!(flashFlag & EFlashUpdateLocked))
            moduleFlashSave();
	}
//    savedConfig.ledSupplier
    savedConfig.ProductID[4] = 0x06;
    lastSavedSingalAdress = savedConfig.singleAddr;
}




uint8_t flashByteRead(uint16_t AddrOffset)
{
    return *(uint8_t *)(0x0400 + AddrOffset);
}


/*
*@details   Our own string copy function.
*
*@retval    string address.
*/
bool_t __memcmp(uint8_t* src1, uint8_t* src2, uint16_t len)
{
    while((*src1++ == *src2++) && (len != 0)){
        len--;
    }
    
    if (len == 0)
        return btrue;
    else
        return bfalse;
}

bool_t GetEEPROMBusyState(void)
{
    if( EEPROM_CTRL_STATUS == (uint16_t)1)
        return btrue;
    return bfalse;
}

void MemoryEraseAndWrite(uint32_t* src1, uint32_t* src2, uint16_t len) 
{
   uint8_t i;
   
    for (i = 0; len != 0; i++, len-=4 ,src1++, src2++)
    {
    
        if (*src1 != *src2) 
        {
            // 擦除前进行必要的擦除操作
            EEImp_IfFuns->MemoryErase( EE_PASSWORD, (eeprom_pAddress_t)(EE_START + 4*i), 4, 0x00, 0x00 );  
            // 写入新数据
            
    

            EEImp_IfFuns->MemoryWrite( EE_PASSWORD, (uint8_t *)&savedConfig + 4*i, (eeprom_pAddress_t)(EE_START + 4*i), 4, 0x00, 0x00 );
    
             

            

            continue;  // 继续下一个循环
        }
    }
   
}

