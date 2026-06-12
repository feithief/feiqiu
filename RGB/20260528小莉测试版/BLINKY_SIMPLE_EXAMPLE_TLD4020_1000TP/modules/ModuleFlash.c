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
#include "device.h"
#include "ModuleFlash.h"
#include <string.h>
#include "flash.h"
/*MAGIC number as well as version NO.*/
#define DMagicNumber  553

#define CODE_FLASH_SIZE      0x7600
#define DATA_FLASH_START     (UC_FLASH_START + CODE_FLASH_SIZE)
#define DATA_FLASH_SIZE      (UC_FLASH_SIZE - CODE_FLASH_SIZE)
#define DATA_FLASH_PAGE_COUNT      (DATA_FLASH_SIZE / UC_FLASH_PAGE_SIZE)
#define FLASH_CONFIG_PAGE_COUNT   ((sizeof(SFlashContent) + UC_FLASH_PAGE_SIZE - 1u) / UC_FLASH_PAGE_SIZE)
#define FLASH_1000TP_PAGE_COUNT   (UC_1000TP_SIZE / UC_1000TP_PAGE_SIZE)
#define FLASH_1000TP_STAGE_MAGIC       (0x3154u)
#define FLASH_1000TP_STATE_IDLE        (0u)
#define FLASH_1000TP_STATE_ERASE       (1u)
#define FLASH_1000TP_STATE_WRITE       (2u)
#define FLASH_1000TP_LASTREQ_WRITE     (0x01u)
#define FLASH_1000TP_LASTREQ_ERASE     (0x02u)

/*
*@brief This variant shows current flash's content.
*@note It's dangerous to make this external, only for smaller code size.
*/
SFlashContent savedConfig;
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

static status_t moduleFlashErasePages(uint32_t startAddr, uint8_t pageCount);
static status_t moduleFlashWritePages(uint32_t startAddr, const uint8_t *data, uint16_t len);

static void moduleFlashSetDefaultWhiteCIE(void)
{
    savedConfig.whitex = 3291u;
    savedConfig.whitey = 3396u;
}

static bool_t moduleFlashWhiteCIEInvalid(void)
{
    if ((savedConfig.whitex > 10000u) ||
        (savedConfig.whitey > 10000u) ||
        (savedConfig.whitex == 0u) ||
        (savedConfig.whitey == 0u))
    {
        return btrue;
    }

    return bfalse;
}

static uint8_t GbTestEn = 0;
static uint8_t GucFlashEnvData[96];
static bool_t GFlashNeeds1000TpMigration = bfalse;
static bool_t GFlashPending1000TpSave = bfalse;

typedef struct Flash1000TpStage
{
    uint16_t magic;
    uint16_t state;
    uint16_t nextPage;
    uint16_t payloadLength;
    uint8_t  payload[sizeof(SFlashContent)];
} SFlash1000TpStage;

static void moduleFlashLoadStage(SFlash1000TpStage *stage)
{
    uint16_t addr;
    uint8_t *dst = (uint8_t *)stage;

    for (addr = 0u; addr < (uint16_t)sizeof(SFlash1000TpStage); addr++)
    {
        dst[addr] = flashByteRead(addr);
    }
}

static bool_t moduleFlashStageIsValid(const SFlash1000TpStage *stage)
{
    if ((stage->magic != FLASH_1000TP_STAGE_MAGIC) ||
        (stage->payloadLength != sizeof(SFlashContent)) ||
        (stage->nextPage >= FLASH_CONFIG_PAGE_COUNT) ||
        ((stage->state != FLASH_1000TP_STATE_ERASE) && (stage->state != FLASH_1000TP_STATE_WRITE)))
    {
        return bfalse;
    }

    return btrue;
}

static bool_t moduleFlashStagePayloadIsValid(const SFlash1000TpStage *stage)
{
    const SFlashContent *stagedConfig;

    if (moduleFlashStageIsValid(stage) == bfalse)
    {
        return bfalse;
    }

    stagedConfig = (const SFlashContent *)stage->payload;
    if (stagedConfig->magicNO != DMagicNumber)
    {
        return bfalse;
    }

    return btrue;
}

static void moduleFlashClearStage(void)
{
    (void)moduleFlashErasePages(DATA_FLASH_START, FLASH_CONFIG_PAGE_COUNT);
}

static void moduleFlashPersistStage(const SFlash1000TpStage *stage)
{
    if (moduleFlashErasePages(DATA_FLASH_START, FLASH_CONFIG_PAGE_COUNT) != GENERIC_E_OK)
    {
        return;
    }

    (void)moduleFlashWritePages(DATA_FLASH_START, (const uint8_t *)stage, sizeof(SFlash1000TpStage));
}

static void moduleFlashClearSrvReqState(void)
{
    MEM->SRVREQ.reg = 0u;
    MEM->SRVREQPTR.reg = 0u;
}

static void moduleFlashContinuePending1000TpSave(void)
{
    SFlash1000TpStage stage;
    uint8_t lastReq;
    uint16_t pageOffset;
    uint16_t remaining;
    uint8_t pageData[UC_1000TP_PAGE_SIZE];

    moduleFlashLoadStage(&stage);
    if (moduleFlashStageIsValid(&stage) == bfalse)
    {
        GFlashPending1000TpSave = bfalse;
        moduleFlashClearSrvReqState();
        return;
    }

    GFlashPending1000TpSave = btrue;
    lastReq = (uint8_t)MEM->SRVREQ.bit.LASTREQ;

    if ((lastReq != 0u) && (MEM->SRVREQ.bit.RESULTSTS != 0u))
    {
        moduleFlashClearSrvReqState();
        return;
    }

    if ((stage.state == FLASH_1000TP_STATE_ERASE) && ((lastReq & FLASH_1000TP_LASTREQ_ERASE) != 0u))
    {
        stage.state = FLASH_1000TP_STATE_WRITE;
        moduleFlashPersistStage(&stage);
        lastReq = 0u;
    }
    else if ((stage.state == FLASH_1000TP_STATE_WRITE) && ((lastReq & FLASH_1000TP_LASTREQ_WRITE) != 0u))
    {
        stage.nextPage++;
        if (stage.nextPage >= FLASH_CONFIG_PAGE_COUNT)
        {
            GFlashPending1000TpSave = bfalse;
            moduleFlashClearStage();
            moduleFlashClearSrvReqState();
            return;
        }

        stage.state = FLASH_1000TP_STATE_ERASE;
        moduleFlashPersistStage(&stage);
        lastReq = 0u;
    }

    moduleFlashClearSrvReqState();

    if (stage.state == FLASH_1000TP_STATE_ERASE)
    {
        (void)moduleFlash1000TpRequestErasePage(stage.nextPage);
    }

    pageOffset = (uint16_t)(stage.nextPage * UC_1000TP_PAGE_SIZE);
    memset(pageData, 0xFF, sizeof(pageData));
    if (pageOffset < stage.payloadLength)
    {
        remaining = (uint16_t)(stage.payloadLength - pageOffset);
        if (remaining > UC_1000TP_PAGE_SIZE)
        {
            remaining = UC_1000TP_PAGE_SIZE;
        }
        memcpy(pageData, &stage.payload[pageOffset], remaining);
    }

    (void)moduleFlash1000TpRequestWritePage(stage.nextPage, pageData);
}

static bool_t moduleFlashConfigIsValid(const SFlashContent *data)
{
    if ((data == (void *)0) || (data->magicNO != DMagicNumber))
    {
        return bfalse;
    }

    if ((data->platform > EPlatformTypeMLBevoVW) ||
        (data->redx > 10000u) ||
        (data->redy > 10000u) ||
        (data->greenx > 10000u) ||
        (data->greeny > 10000u) ||
        (data->bluex > 10000u) ||
        (data->bluey > 10000u) ||
        (data->whitex > 10000u) ||
        (data->whitey > 10000u) ||
        (data->brightness_factor > 100u))
    {
        return bfalse;
    }

    return btrue;
}

static void moduleFlashLoadLegacyDataFlash(SFlashContent *data)
{
    uint16_t addr;

    for (addr = 0u; addr < (uint16_t)sizeof(SFlashContent); addr++)
    {
        *((uint8_t*)data + addr) = flashByteRead(addr);
    }
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
    SFlashContent legacyConfig;
    SFlash1000TpStage stage;

    GFlashNeeds1000TpMigration = bfalse;

    if (data == (void *)0)
    {
        return;
    }

    moduleFlashLoadStage(&stage);
    if (moduleFlashStagePayloadIsValid(&stage) == btrue)
    {
        memcpy(data, stage.payload, sizeof(SFlashContent));
        GFlashNeeds1000TpMigration = btrue;
    }
    else if ((moduleFlash1000TpRead(0u, (uint8_t *)data, sizeof(SFlashContent)) == btrue) &&
             (moduleFlashConfigIsValid(data) == btrue))
    {
        /* 1000TP already holds the active config. */
    }
    else
    {
        memset(&legacyConfig, 0, sizeof(legacyConfig));
        moduleFlashLoadLegacyDataFlash(&legacyConfig);

        if (moduleFlashConfigIsValid(&legacyConfig) == btrue)
        {
            memcpy(data, &legacyConfig, sizeof(SFlashContent));
            GFlashNeeds1000TpMigration = btrue;
        }
        else
        {
            memset(data, 0, sizeof(SFlashContent));
        }
    }

    /*ledSupplier & 0x00f0u == 0x0080 means flash content has been locked, */
    /*can not be changed in any circumstances*/
    if ((data->ledSupplier & 0x00f0u) ==  0x0080u)
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
    SFlashContent temp_content;
    SFlash1000TpStage stage;

    moduleFlashLoad(&temp_content);
    if ((GFlashNeeds1000TpMigration == bfalse) &&
        (__memcmp((uint8_t *)&temp_content, (uint8_t *)&savedConfig, (uint16_t)sizeof(SFlashContent)) == btrue))
    {
        return;
    }

    memset(&stage, 0xFF, sizeof(stage));
    stage.magic = FLASH_1000TP_STAGE_MAGIC;
    stage.state = FLASH_1000TP_STATE_ERASE;
    stage.nextPage = 0u;
    stage.payloadLength = sizeof(SFlashContent);
    memcpy(stage.payload, (const uint8_t *)&savedConfig, sizeof(SFlashContent));

    moduleFlashPersistStage(&stage);
    GFlashPending1000TpSave = btrue;

    /* If 1000TP request failed to trigger reset, keep lock flag semantics stable. */
    if ((savedConfig.ledSupplier & 0x00f0u) ==  0x0080u)
    {
        flashFlag |= EFlashUpdateLocked;
    }
    else
    {
        flashFlag &= ~EFlashUpdateLocked;
    }
}

void moduleFlashProcess(void)
{
    if (GFlashPending1000TpSave == bfalse)
    {
        return;
    }

    moduleFlashContinuePending1000TpSave();
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
    bool_t resumePending1000Tp = bfalse;
    bool_t stageValid = bfalse;
    SFlash1000TpStage stage;

    moduleFlashLoadStage(&stage);
    stageValid = moduleFlashStageIsValid(&stage);
    GFlashPending1000TpSave = bfalse;

    if (PMU->RESETSTS.bit.SOFT != 0u)
    {
        resumePending1000Tp = btrue;
        PMU->RESETSC.bit.SOFT = 1u;
    }

    if ((resumePending1000Tp != bfalse) && (stageValid != bfalse))
    {
        GFlashPending1000TpSave = btrue;
        moduleFlashContinuePending1000TpSave();
    }

    moduleFlashLoad(&savedConfig);

  if (savedConfig.magicNO != DMagicNumber) //+ whether crc is correct
  {
    savedConfig.magicNO = DMagicNumber;
		savedConfig.platform = EPlatformTypeMQB;

    savedConfig.AutoAddr = 0x10u;
    savedConfig.eolAddr  = 1u;//90
    // savedConfig.eolAddr  = 15790;//70
    savedConfig.writeAddr =0xff;

    savedConfig.ledSupplier = 0x02;
    savedConfig.singleAddr = savedConfig.AutoAddr;
    // savedConfig.CurrentNAD = 0xA0u;
    savedConfig.CurrentNAD = savedConfig.singleAddr;
    savedConfig.resetNO = Edefaultreset;
    savedConfig.factor = 0x8000;
    //90
    //1
     savedConfig.redx = 7025;
    savedConfig.redy = 2967;
    savedConfig.redY = 29000;

    savedConfig.greenx = 1370;
    savedConfig.greeny = 7069;
    savedConfig.greenY = 47060;

    savedConfig.bluex = 1521;
    savedConfig.bluey = 284;
    savedConfig.blueY = 8733;

    savedConfig.whitex = 3333;
    savedConfig.whitey = 3491;
    savedConfig.whiteY = 55580;
        // 1
    //  savedConfig.redx = 7030;
    // savedConfig.redy = 2965;
    // savedConfig.redY = 28030;

    // savedConfig.greenx = 1472;
    // savedConfig.greeny = 7211;
    // savedConfig.greenY = 51110;

    // savedConfig.bluex = 1519;
    // savedConfig.bluey = 297;
    // savedConfig.blueY = 8438;

    // savedConfig.whitex = 3360;
    // savedConfig.whitey = 3473;
    // savedConfig.whiteY = 53070;
    //70
//     savedConfig.redx = 7024;
//     savedConfig.redy = 2961;
//     savedConfig.redY = 8248;

//    savedConfig.greenx = 1673;
//    savedConfig.greeny = 7328;
//    savedConfig.greenY = 18700;

//    savedConfig.bluex = 1503;
//    savedConfig.bluey = 325;
//    savedConfig.blueY = 2300;
    memcpy((uint8_t*)savedConfig.partNO , "123456789012", DMAX_PART_NUMBER_LENGTH);
		memcpy((uint8_t*)savedConfig.hardwareVersion , "H04", DHARDWARE_VERSION_LENGTH);
		memcpy((uint8_t*)savedConfig.serialNO , "12345678900987654321", DMAX_SERIAL_NUMBER_LENGTH);

    savedConfig.ProductID[0] = 0x84;
    savedConfig.ProductID[1] = 0x00;
    savedConfig.ProductID[2] = 0x03;
    savedConfig.ProductID[3] = 0x00;


    savedConfig.brightness_factor= 100;

     savedConfig.x1 		= 		(uint8_t)10u;
     savedConfig.y1l 		= 		(uint8_t)(((uint16_t)3u) & 0x00ffu);
     savedConfig.y1h 		= 		(uint8_t)(((uint16_t)3u) >> 8u);

     savedConfig.x2 		= 		(uint8_t)20u;
     savedConfig.y2l 		= 		(uint8_t)((uint16_t)17u & 0x00ffu);
     savedConfig.y2h 		= 		(uint8_t)(((uint16_t)17u) >> 8u);

     savedConfig.x3 		= 		(uint8_t)30u;
     savedConfig.y3l 		= 		(uint8_t)(((uint16_t)5u * (uint16_t)10u) & 0x00ffu);
     savedConfig.y3h 		= 		(uint8_t)(((uint16_t)5u * (uint16_t)10u) >> 8u);

     savedConfig.x4 		=  		(uint8_t)40u;
     savedConfig.y4l 		= 		(uint8_t)(((uint16_t)10u * (uint16_t)10u) & 0x00ffu);
     savedConfig.y4h 		= 		(uint8_t)(((uint16_t)10u * (uint16_t)10u) >> 8u);

     savedConfig.x5 		= 		(uint8_t)50u;
     savedConfig.y5l 		= 		(uint8_t)(((uint16_t)18u * (uint16_t)10u) & 0x00ffu);
     savedConfig.y5h 		= 		(uint8_t)(((uint16_t)18u * (uint16_t)10u) >> 8u);

     savedConfig.x6 		= 		(uint8_t)60u;
     savedConfig.y6l 		= 		(uint8_t)(((uint16_t)28u * (uint16_t)10u) & 0x00ffu);
     savedConfig.y6h 		= 		(uint8_t)(((uint16_t)28u * (uint16_t)10u) >> 8u);

     savedConfig.x7 		= 		(uint8_t)70u;
     savedConfig.y7l 		= 		(uint8_t)(((uint16_t)41u * (uint16_t)10u) & 0x00ffu);
     savedConfig.y7h 		= 		(uint8_t)(((uint16_t)41u * (uint16_t)10u) >> 8u);

     savedConfig.x8 		= 		(uint8_t)80u;
     savedConfig.y8l 		= 		(uint8_t)(((uint16_t)57u * (uint16_t)10u) & 0x00ffu);
     savedConfig.y8h 		= 		(uint8_t)(((uint16_t)57u * (uint16_t)10u) >> 8u);

     savedConfig.x9 		= 		(uint8_t)90u;
     savedConfig.y9l 		= 		(uint8_t)(((uint16_t)77u * (uint16_t)10u) & 0x00ffu);
     savedConfig.y9h 		= 		(uint8_t)(((uint16_t)77u * (uint16_t)10u) >> 8u);
		savedConfig.u = 0;
		savedConfig.v = 0;
		memset(savedConfig.U, 0, 10);
		memset(savedConfig.V, 0, 10);
    savedConfig.Utemp = 80;
    savedConfig.Vtemp = 188;
    savedConfig.pART = Slave_Address_auto;
    savedConfig.pDevelor = 1;
    savedConfig.shortRed = 0;
    savedConfig.shortGreen = 0;
    savedConfig.shortBule = 0;
    savedConfig.openRed = 0;
    savedConfig.openGreen = 0;
    savedConfig.openBule = 0;
    savedConfig.error_ram = 0;
    savedConfig.error_rom = 0;
    savedConfig.error_NVM = 0;
		savedConfig.RthermolRatio = 92;
		savedConfig.GthermolRatio = 60;
		savedConfig.BthermolRatio = 55;
		savedConfig.MCUthermolRatio = 17;
//		savedConfig.AutoSaveCnt = 0;
    moduleFlashSave();
  }
/*The use of the 3C diagnostic DID 0x5000 can reset the address to the change state*/
//  savedConfig.Utemp = 40;
//  savedConfig.Vtemp = 94;
  savedConfig.ProductID[4] = 0x00;
  if ((savedConfig.CurrentNAD == 0u) ||
      (savedConfig.CurrentNAD == 0xFFu) ||
      (savedConfig.CurrentNAD == (uint8_t)savedConfig.singleAddr))
  {
    savedConfig.CurrentNAD = 0xA0u;
  }
  lastSavedSingalAdress = savedConfig.singleAddr;

  if (GFlashNeeds1000TpMigration == btrue)
  {
    moduleFlashSave();
  }
	
	/**/
//	     savedConfig.x1 		= 		(uint8_t)10u;
//     savedConfig.y1l 		= 		(uint8_t)(((uint16_t)3u) & 0x00ffu);
//     savedConfig.y1h 		= 		(uint8_t)(((uint16_t)3u) >> 8u);

//     savedConfig.x2 		= 		(uint8_t)20u;
//     savedConfig.y2l 		= 		(uint8_t)((uint16_t)17u & 0x00ffu);
//     savedConfig.y2h 		= 		(uint8_t)(((uint16_t)17u) >> 8u);

//     savedConfig.x3 		= 		(uint8_t)30u;
//     savedConfig.y3l 		= 		(uint8_t)(((uint16_t)5u * (uint16_t)10u) & 0x00ffu);
//     savedConfig.y3h 		= 		(uint8_t)(((uint16_t)5u * (uint16_t)10u) >> 8u);

//     savedConfig.x4 		=  		(uint8_t)40u;
//     savedConfig.y4l 		= 		(uint8_t)(((uint16_t)10u * (uint16_t)10u) & 0x00ffu);
//     savedConfig.y4h 		= 		(uint8_t)(((uint16_t)10u * (uint16_t)10u) >> 8u);

//     savedConfig.x5 		= 		(uint8_t)50u;
//     savedConfig.y5l 		= 		(uint8_t)(((uint16_t)18u * (uint16_t)10u) & 0x00ffu);
//     savedConfig.y5h 		= 		(uint8_t)(((uint16_t)18u * (uint16_t)10u) >> 8u);

//     savedConfig.x6 		= 		(uint8_t)60u;
//     savedConfig.y6l 		= 		(uint8_t)(((uint16_t)28u * (uint16_t)10u) & 0x00ffu);
//     savedConfig.y6h 		= 		(uint8_t)(((uint16_t)28u * (uint16_t)10u) >> 8u);

//     savedConfig.x7 		= 		(uint8_t)70u;
//     savedConfig.y7l 		= 		(uint8_t)(((uint16_t)41u * (uint16_t)10u) & 0x00ffu);
//     savedConfig.y7h 		= 		(uint8_t)(((uint16_t)41u * (uint16_t)10u) >> 8u);

//     savedConfig.x8 		= 		(uint8_t)80u;
//     savedConfig.y8l 		= 		(uint8_t)(((uint16_t)57u * (uint16_t)10u) & 0x00ffu);
//     savedConfig.y8h 		= 		(uint8_t)(((uint16_t)57u * (uint16_t)10u) >> 8u);

//     savedConfig.x9 		= 		(uint8_t)90u;
//     savedConfig.y9l 		= 		(uint8_t)(((uint16_t)77u * (uint16_t)10u) & 0x00ffu);
//     savedConfig.y9h 		= 		(uint8_t)(((uint16_t)77u * (uint16_t)10u) >> 8u);
	/**/}




uint8_t flashByteRead(uint16_t AddrOffset)
{
    return *(uint8_t *)(DATA_FLASH_START + AddrOffset);
}


/*
*@details   Our own string copy function.
*
*@retval    string address.
*/
bool_t __memcmp(uint8_t* src1, uint8_t* src2, uint16_t len)
{
    while (len != 0u)
    {
        if (*src1++ != *src2++)
        {
            return bfalse;
        }

        len--;
    }

    return btrue;
}

static status_t moduleFlashErasePages(uint32_t startAddr, uint8_t pageCount)
{
    status_t status = GENERIC_E_OK;
    uint8_t pageIndex;

    for (pageIndex = 0u; pageIndex < pageCount; pageIndex++)
    {
        status = user_nvm_erase(startAddr + ((uint32_t)pageIndex * UC_FLASH_PAGE_SIZE), NVM_ERASE_PAGE);
        if (status != GENERIC_E_OK)
        {
            break;
        }
    }

    return status;
}

static status_t moduleFlashWritePages(uint32_t startAddr, const uint8_t *data, uint16_t len)
{
    status_t status = GENERIC_E_OK;
    uint16_t offset = 0u;

    while (offset < len)
    {
        uint16_t bytesThisPage = (uint16_t)(len - offset);
        if (bytesThisPage > UC_FLASH_PAGE_SIZE)
        {
            bytesThisPage = UC_FLASH_PAGE_SIZE;
        }

        status = user_nvm_write(startAddr + offset, data + offset, bytesThisPage, NVM_OPTIONS_RETRY_MASK);
        if (status != GENERIC_E_OK)
        {
            break;
        }

        offset = (uint16_t)(offset + bytesThisPage);
    }

    return status;
}

bool_t moduleFlash1000TpRead(uint32_t offset, uint8_t *dst, uint32_t len)
{
    const uint8_t *src;

    if ((dst == (void *)0) || ((offset + len) > UC_1000TP_SIZE))
    {
        return bfalse;
    }

    src = (const uint8_t *)(UC_1000TP_START + offset);
    memcpy(dst, src, len);
    return btrue;
}

status_t moduleFlash1000TpRequestWritePage(uint32_t pageIndex, const uint8_t *src)
{
    srv_1000tp_write_t srv_1000tp_write;

    if ((src == (void *)0) || (pageIndex >= FLASH_1000TP_PAGE_COUNT))
    {
        return GENERIC_E_PARAM_INVALID;
    }

    memset(&srv_1000tp_write, 0xFF, sizeof(srv_1000tp_write));
    srv_1000tp_write.address = UC_1000TP_START + (pageIndex * UC_1000TP_PAGE_SIZE);
    srv_1000tp_write.length = UC_1000TP_PAGE_SIZE;
    memcpy(srv_1000tp_write.data, src, UC_1000TP_PAGE_SIZE);

    MEM->SRVREQ.reg = 0u;
    MEM->SRVREQPTR.reg = (uint32_t)&srv_1000tp_write;
    MEM->SRVREQ.bit.WR1000TP = 1u;

    moduleWatchdogFeed();
    __NVIC_SystemReset();

    for (;;)
    {
    }
}

status_t moduleFlash1000TpRequestErasePage(uint32_t pageIndex)
{
    srv_1000tp_erase_t srv_1000tp_erase;

    if (pageIndex >= FLASH_1000TP_PAGE_COUNT)
    {
        return GENERIC_E_PARAM_INVALID;
    }

    srv_1000tp_erase.address = UC_1000TP_START + (pageIndex * UC_1000TP_PAGE_SIZE);

    MEM->SRVREQ.reg = 0u;
    MEM->SRVREQPTR.reg = (uint32_t)&srv_1000tp_erase;
    MEM->SRVREQ.bit.ER1000TP = 1u;

    moduleWatchdogFeed();
    __NVIC_SystemReset();

    for (;;)
    {
    }
}

//bool_t GetEEPROMBusyState(void)
//{
//    if( EEPROM_CTRL_STATUS == (uint16_t)1)
//        return btrue;
//    return bfalse;
//}

void MemoryEraseAndWrite(uint8_t* src1, uint8_t* src2, uint16_t len) 
{
    status_t status;
    uint8_t pageCount;
    const uint8_t *writeData;

    (void)src1;

    if ((src2 == (void *)0) || (len == 0u))
    {
        return;
    }

    writeData = (const uint8_t *)src2;

    if (flashFlag == EFlashAutoAddressSaved)
    {
        (void)moduleFlashErasePages(DATA_FLASH_START, 1u);
        (void)moduleFlashWritePages(DATA_FLASH_START, writeData, UC_FLASH_PAGE_SIZE);
        return;
    }

    pageCount = (uint8_t)((len + UC_FLASH_PAGE_SIZE - 1u) / UC_FLASH_PAGE_SIZE);
    if ((pageCount == 0u) || (pageCount > DATA_FLASH_PAGE_COUNT))
    {
        return;
    }

    status = moduleFlashErasePages(DATA_FLASH_START, pageCount);
    if (status != GENERIC_E_OK)
    {
        return;
    }

    (void)moduleFlashWritePages(DATA_FLASH_START, writeData, len);
}