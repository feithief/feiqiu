#ifndef _MODULEEEPROM_H_
#define _MODULEEEPROM_H_

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
////#include "api.h"
#include "SystemType.h"


/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */
/*This macro defines max length of serial Number*/
#define DMAX_SERIAL_NUMBER_LENGTH 20u
/**@brief This macro defines max length of part NO..*/
#define DMAX_PART_NUMBER_LENGTH   12u
/**@brief hardware section length.*/
#define DHARDWARE_VERSION_LENGTH  6U


/* ============================================================================ */
/* -------------------- FORWARD DECLARATIONS / PROTOTYPES --------------------- */
/* ============================================================================ */
/*This ENUM indicates which platform we are using*/
typedef enum PlatformType
{
  EPlatformTypeMQB        = 0x0000, /**<MQB platform.*/
  EPlatformTypeMLBevo     = 0x0001, /**<MLBevo platform.*/
  EPlatformTypeMLBevoVW   = 0x0002, /**<MLBevoVW platform.*/
  EPlatformTypeMax        = 0xFFFF, /**<To make this ENUM's size equals two.*/
} EPlatformType;

typedef enum HardwareType
{
  EHardwareTypeRLN211   = 0x11,
  EHardwareTypeRLN211A  = 0x1A,
}EHardwareType;

/*@brief This structure contains flash memory.*/


typedef union UCtrlFlag
{
  uint16_t num;
  struct SBit
  {
    uint16_t reserved0                   :1;   //bit 0
    uint16_t IsNormalVW                  :1;   //bit 1   original platform
    uint16_t reserved1                   :4;   //bit 2-5
    uint16_t IsTimeBasisValid            :1;   // bit 6  =0:always slow(250), =1: 250 or 50 depend on lin signal
    uint16_t IsLocked                    :1;   // bit 7
    uint16_t reserved2                   :8;
  }bit; 
}CtrlFlag_t;

#define VehicleNum  (11)

#if 1
typedef struct FlashContent
{
  uint16_t       crc;         /**<Flash content crc16 result.*/
  uint16_t       magicNO;     /**<magic no also using as version flag.*/
  
  /*The old version this section has two bytes, we add two reserved bytes to prevent overflow.*/   
  uint8_t       ContentSize;    /* 结构体大小 */
  uint8_t       ParasNO;        /* 参数表的号码 */
  CtrlFlag_t     CtrlFlag;   //uint16_t       CtrlFlag; 
  
  uint16_t       singleAddr;  /**<this member contains this RGB module's single address.*/
  uint16_t       groupAddr;   /**<this member contains this RGB module's group address.*/
  
  uint16_t       redx;        /**<Current led red CIE x coordinate.*/
  uint16_t       redy;        /**<Current led red CIE y coordinate.*/  
  uint32_t       redY;        /**<Current led red CIE Y coordinate.*/ 
  uint16_t       greenx;      /**<Current led green CIE x coordinate.*/
  uint16_t       greeny;      /**<Current led green CIE y coordinate.*/
  uint32_t       greenY;      /**<Current led green CIE Y coordinate.*/
  uint16_t       bluex;       /**<Current led blue CIE x coordinate.*/
  uint16_t       bluey;       /**<Current led blue CIE y coordinate.*/
  uint32_t       blueY;       /**<Current led blue CIE Y coordinate.*/
  
  uint8_t        serialNO[DMAX_SERIAL_NUMBER_LENGTH];          /**<serial number, add one byte for '\0'.*/
  uint8_t        partNO[DMAX_PART_NUMBER_LENGTH];              /**<part number, add one byte for '\0'.*/
  
  uint8_t        hardwareVersion[DHARDWARE_VERSION_LENGTH];    /**<hardware version, add one byte for '\0'.*/
  int8_t         RatingLowerVoltage;  /**<Compensation for low voltage.*/
  int8_t         RatingHigherVoltage; /**<Compensation for High voltage.*/
  
  int16_t        RthermolRatio;       /**<Thermal ratio of red led.*/
  int16_t        GthermolRatio;       /**<Thermal ratio of green led.*/
  int16_t        BthermolRatio;       /**<Thermal ratio of blue led.*/
  int16_t        MCUthermolRatio;     /**<Thermal ratio of MCU.*/
  
  
  uint8_t        HWResp;        
  uint8_t        SubVersion;
  uint8_t        SWResp;
  uint8_t       reserved1;
  
  uint16_t       Lfactor[VehicleNum];      /**<Customor uses this member to adjust RGB light intensity.*/
  uint8_t        MinUsrL[VehicleNum];
  
}SFlashContent;
#endif











#if 0
typedef struct DebugContent
{
  uint16_t       T_LED;
  uint16_t       T_MCU;
  uint16_t       V_VS;
  uint16_t       VledR;
  uint16_t       VledG;
  uint16_t       VledB;
}SDebugContent;
#endif

/*This ENUM indicates flash operation's status.*/
typedef enum FlashUpdate
{
  EFlashUpdateFree       = 0x0,    /**<Flash is free for updating.*/
  EFlashUpdateProcessing = 0x1,    /**<Flash's memory updating is on process.*/
  EFlashUpdateCali_R     = 0x2,    /**<Caliberation, record standard values>*/
  EFlashUpdateCali_G     = 0x4,    /**<Caliberation, record standard values>*/
  EFlashUpdateCali_B     = 0x8,    /**<Caliberation, record standard values>*/
  EFlashUpdateLocked     = 0x80,   /**<Flash's memory has been locked.*/
} EFlashUpdate;


/* ============================================================================ */
/* ----------------------------SYSTEM GLOBALE VARIABLES ----------------------- */
/* ============================================================================ */

/*******************************************************************************
*@brief This variant shows flash's current status.
*@Note It's dangerous to make this external, only for smaller code size.
********************************************************************************/
extern volatile EFlashUpdate flashFlag;

/*******************************************************************************
*@brief This variant shows current flash's content.
*@Note It's dangerous to make this external, only for smaller code size.
********************************************************************************/
extern SFlashContent savedConfig;
//extern SDebugContent Debug;
/*******************************************************************************
*@brief This variant indicates if slave single address has been updated.
*@warning Do not delete this flag, sometime we change slave address to 0xA0, 
    in order to know whether the flash need to be updated
*@Note It's dangerous to make this external, only for smaller code size.
********************************************************************************/
extern volatile uint16_t lastSavedSingalAdress;


/*******************************************************************************
*@brief This variant shows current module's type.
*@Note *It's dangerous to make this external, only for smaller code size.
*******************************************************************************/
extern volatile EHardwareType moduleType;

/*******************************************************************************
*@details   Initialize flash content, for RAM Mode.
*
*@warning   This function can not be invoked in interrupt.
*
*@retval    None.
*******************************************************************************/
void moduleFlashInit(void);

/**
*@details   Save current ram content to flash.
*
*@warning   This function can not be invoked in interrupt.
*
*@retval    None.
*/
void moduleFlashSave(void);
void moduleFlashLoad1(SFlashContent  *data);
/** @} */





static const unsigned char aucCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40
};
static const unsigned char aucCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
	0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
	0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
	0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
	0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
	0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
	0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
	0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
	0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
	0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
	0x41, 0x81, 0x80, 0x40
};

uint16_t usMBCRC16(uint8_t * pucFrame, uint8_t usLen);


uint8_t* __strcpy(uint8_t* dst, const uint8_t* src);

bool_t __memcmp(uint8_t* src1, uint8_t* src2, uint16_t len);


//int16_t flash_from_flash(flash_ctrl_num_t flash_ctrl_no, uintptr_t *sptr, uintptr_t *dptr, uint16_t words, bool info);











#endif                                                                  
/* _MODULEEEPROM_H_                */

/**********************************************************************************************************
  END FILE
**********************************************************************************************************/
