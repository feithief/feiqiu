#include "ModuleFlash.h"
#include <eeprom_wr.h>
#include <eeprom_drv.h>
#include <crc.h>
#include "ModuleWatchdog.h"
#include "../Modules/ModuleComm.h"
#include <atomic.h>

#define D_MAGIC_NUMBER  0xB0DA

#define EE_WRITE_KEY 0x07u

#define EE_DYNAMIC_AREA_START_KEBODA 0x0850u

#define VALIDEEPATTERN 0xA55Au
#define REVISION 0x0001u

#define EE_DYNAMIC_AREA_START 0x0840u
#define EE_DYNAMIC_VALID_PATTERN (EE_DYNAMIC_AREA_START)
#define EE_DYNAMIC_REV  (EE_DYNAMIC_AREA_START + 0x0002u)
#define EE_DYNAMIC_NAD  (EE_DYNAMIC_AREA_START + 0x0004u)
#define EE_DYNAMIC_CRC  (EE_DYNAMIC_AREA_START + 0x0006u)

#define EE_DYNAMIC_LENGTH_WO_CRC (EE_DYNAMIC_CRC-EE_DYNAMIC_AREA_START)

extern volatile uint16_t EE_DYN_VALID   __attribute((nodp, addr(EE_DYNAMIC_VALID_PATTERN)));
extern volatile uint16_t EE_DYN_REV   __attribute((nodp, addr(EE_DYNAMIC_REV)));
extern volatile uint16_t EE_DYN_NAD   __attribute((nodp, addr(EE_DYNAMIC_NAD)));
extern volatile uint16_t EE_DYN_CRC   __attribute((nodp, addr(EE_DYNAMIC_CRC)));

struct FlashContent savedConfig;

static uint8_t lastAddr = 0;

volatile EFlashUpdate flashFlag;

/* ----------------------------------------------------------------------------
 * calculate checksum according CRC CCITT
 */
static uint16_t calcEEChecksum(uint16_t startaddress, uint16_t length){
    uint16_t crc = 0;
    uint16_t i = 0;
    uint8_t* ptrData=(uint8_t*)startaddress;

    /* calculate checksum over provided data */
    for(i = 0; i < length; i++) {
        crc = crc_ccitt (*ptrData++, crc);
    }

    return crc;
}

/**
*@details   Our own string copy function.
*
*@retval    string address.
*/
//static char* __strcpy(char* dst, const char* src){
//  while(*src != 0){
//    *dst++ = *src++;
//  }
//  *dst = 0;
//  return dst;
//}

/**
*@details   Our own string copy function.
*
*@retval    string address.
*/
static bool_t __memcmp(uint8_t* src1, uint8_t* src2, uint16_t len){
  while((*src1++ == *src2++) && (len != 0)){
    len--;
  }

  if (len == 0)
    return btrue;
  else
    return bfalse;
}

void moduleFlashLoad(void)
{
  uint16_t index = 0;

  uint8_t* dataPtr = (uint8_t*)EE_DYNAMIC_AREA_START_KEBODA;
  uint8_t* dataDest = (uint8_t*)&savedConfig;

  for (index = 0; index < sizeof(struct FlashContent); index++)
    *(dataDest + index) = *(dataPtr + index);
}

/* ----------------------------------------------------------------------------
 * write node address into EEPROM including VALIDPATTERN, ID and CRC -> 8 data bytes
 */
static INLINE void writeNADintoEEPROM(uint8_t slaveAddress){
    uint16_t eepromArray[4] = {VALIDEEPATTERN,REVISION,0,0};

    /* write node address into RAM buffer */
    eepromArray[2] = (uint16_t)(slaveAddress&0x003Fu);
    /* calculate CRC from RAM buffer */
    eepromArray[3] = calcEEChecksum((uint16_t)&eepromArray[0],EE_DYNAMIC_LENGTH_WO_CRC);

    /* acknowledge absolute watchdog if needed */
    moduleWDTFeed();

    /* write RAM buffer into EEPROM page */
    EEPROM_WriteWord64_blocking(EE_DYNAMIC_AREA_START, eepromArray, EE_WRITE_KEY);

    /* acknowledge absolute watchdog if needed */
    moduleWDTFeed();
}

#define CURRENTSWVERSION 196
#define CURRENTHWVERSION   8

void moduleFlashInit(void)
{
  uint16_t index = 0;

  moduleFlashLoad();

  if (savedConfig.magicNO != D_MAGIC_NUMBER)
  {
    savedConfig.magicNO = D_MAGIC_NUMBER;
    savedConfig.nad = 63;
    savedConfig.LedNumber = 1;
    savedConfig.SWVersion = CURRENTSWVERSION;//v196
    savedConfig.HWVersion = CURRENTHWVERSION;//H08

    for (index = 0; index < RGBLEDMAX; index++)
    {
      savedConfig.redx[index] = 6071;
      savedConfig.redy[index] = 2499;
      savedConfig.redY[index] = 19680;
      savedConfig.greenx[index] = 1652;
      savedConfig.greeny[index] = 6936;
      savedConfig.greenY[index] = 29106;
      savedConfig.bluex[index] = 1545;
      savedConfig.bluey[index] = 246;
      savedConfig.blueY[index] = 1968;

      savedConfig.RCapCompensate[index] = 2;
      savedConfig.GCapCompensate[index] = 1;
      savedConfig.BCapCompensate[index] = 4;

      savedConfig.LedVoltage_25Celsius.LedVoltage[index].ledChannelData[1] = 1745;
      savedConfig.LedVoltage_25Celsius.LedVoltage[index].ledChannelData[0] = 2758;
      savedConfig.LedVoltage_25Celsius.LedVoltage[index].ledChannelData[2] = 2599;
    }
  }
  else if((savedConfig.SWVersion != CURRENTSWVERSION)||(savedConfig.HWVersion != CURRENTHWVERSION))
  {
    if(savedConfig.SWVersion != CURRENTSWVERSION)
    {
	  savedConfig.SWVersion = CURRENTSWVERSION;
    }
    if(savedConfig.HWVersion != CURRENTHWVERSION)
    {
	  savedConfig.HWVersion = CURRENTHWVERSION;
    }
  }
  else
  {}

  /* check EEPROM for valid dynamic content like node address */
  if((EE_DYN_VALID == VALIDEEPATTERN) && (EE_DYN_REV == REVISION) &&          \
     (EE_DYN_CRC == calcEEChecksum(EE_DYNAMIC_AREA_START,EE_DYNAMIC_LENGTH_WO_CRC)))
  {
    lastAddr = EE_DYN_NAD;
  }
  else
  {
      /* no valid EEPROM data */
    /*force to re-write nad address*/
    lastAddr = savedConfig.nad + 1;
  }

  moduleFlashSave();
}

void moduleFlashSave(void)
{
  uint16_t index = 0;

  uint8_t* dataPtr = (uint8_t*)EE_DYNAMIC_AREA_START_KEBODA;
  uint8_t* dataDest = (uint8_t*)&savedConfig;

  for (index = 0; index < (sizeof(struct FlashContent)/8); index++)
  {
    if (__memcmp(dataDest + index*8, dataPtr + index*8, 8) != btrue)
    {
      moduleWDTFeed();
      EEPROM_WriteWord64_blocking((uint16_t)(dataPtr + index*8), (uint16_t*)((uint16_t)dataDest + index*8), EE_WRITE_KEY);
      moduleWDTFeed();
    }
    else
    {}
  }

  if (lastAddr != savedConfig.nad)
  {
    lastAddr = savedConfig.nad;
    writeNADintoEEPROM(lastAddr);
  }
}

void Set_Last_Address(uint8_t slaveAddress)
{
	lastAddr = slaveAddress;
}

void Auto_Addressing_Write_NAD_TO_EEPROM(uint8_t slaveAddress)
{
	/* write node address into eeprom */
	writeNADintoEEPROM(slaveAddress);
}
