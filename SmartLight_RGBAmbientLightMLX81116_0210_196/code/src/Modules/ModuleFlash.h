/*

 * flash.h
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#ifndef _MODULE_FLASH_H_
#define _MODULE_FLASH_H_

#include "../system/SystemConfig.h"
#include "ModuleAdc.h"

/**@brief This macro defines max length of serial NO..*/
#define DMAX_SERIAL_NUMBER_LENGTH 20u
/**@brief hardware section length.*/
#define DHARDWARE_VERSION_LENGTH  3U

typedef struct FlashContent
{
  uint8_t        nad;
  uint8_t        LedNumber;   /**<LED number of each channel.*/

  uint16_t       redx[RGBLEDMAX];        /**<Current led red CIE x coordinate.*/
  uint16_t       redy[RGBLEDMAX];        /**<Current led red CIE y coordinate.*/
  uint32_t       redY[RGBLEDMAX];        /**<Current led red CIE Y coordinate.*/

  uint16_t       greenx[RGBLEDMAX];      /**<Current led green CIE x coordinate.*/
  uint16_t       greeny[RGBLEDMAX];      /**<Current led green CIE y coordinate.*/
  uint32_t       greenY[RGBLEDMAX];      /**<Current led green CIE Y coordinate.*/

  uint16_t       bluex[RGBLEDMAX];       /**<Current led blue CIE x coordinate.*/
  uint16_t       bluey[RGBLEDMAX];       /**<Current led blue CIE y coordinate.*/
  uint32_t       blueY[RGBLEDMAX];       /**<Current led blue CIE Y coordinate.*/

  uint8_t        RCapCompensate[RGBLEDMAX];      /**<Compensation for PWM lost causing by capacitor.*/
  uint8_t        GCapCompensate[RGBLEDMAX];      /**<Compensation for PWM lost causing by capacitor.*/
  uint8_t        BCapCompensate[RGBLEDMAX];      /**<Compensation for PWM lost causing by capacitor.*/

  SLEDVoltages   LedVoltage_25Celsius;
  uint16_t       magicNO;     /**<magic number is used to determine whether there is configuration data in the EEPROM,
                                  if not, write the default configuration data to EEPROM.*/
  uint16_t       SWVersion;   /**<Software version.*/
  uint16_t       HWVersion;   /**<Hardware version.*/
  uint8_t        reserved[2]; /**<Write to EEPROM is based on 8 bytes, here is to make the size of the content a multiple of 8.*/

}SFlashContent;

/**@brief This ENUM indicates flash operation's status.*/
typedef enum FlashUpdate
{
  EFlashUpdateFree       = 0x0,    /**<Flash is free for updating.*/
  EFlashUpdateProcessing = 0x1,    /**<Flash's memory updating is on process.*/
  EFlashUpdateLocked     = 0x80,   /**<Flash's memory has been locked.*/
} EFlashUpdate;

/**@brief This variant shows flash's current status.
 *It's dangerous to make this external, only for smaller code size.*/
extern volatile EFlashUpdate flashFlag;

extern struct FlashContent savedConfig;

void moduleFlashInit(void);
void moduleFlashLoad(void);
void moduleFlashSave(void);
void Set_Last_Address(uint8_t slaveAddress);
void Auto_Addressing_Write_NAD_TO_EEPROM(uint8_t slaveAddress);

#endif /* CODE_SRC_DRIVERS_FLASH_H_ */
