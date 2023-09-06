/***************************************************************************//**
 * @file      bootloader_UserConfig.h
 *
 * @creator   sbai
 * @created   15.06.2015
 * @sdfv      Elmos Flow - Grade 2
 *
 * @brief     User config data type definitions
 *
 * @purpose   Data type and interface definitions for user modifiable
 *            configuration data.
 *
 *
 * $Id: $
 ******************************************************************************/
/* ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
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
 *******************************************************************************/

#ifndef BOOTLOADER_USERCONFIG_H_
#define BOOTLOADER_USERCONFIG_H_

#include "el_types.h"
/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "bootloader_helper.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/


typedef  uint8_t  bldConf_SNPDType3MaxSteps_t;
typedef uint8_t   bldConf_SNPDType3MaxIShunt_t;
typedef uint16_t  bldConf_SNPDType3StepSize_t;
/***************************************************************************/

typedef uint16_t  bldConf_SNPDThres_t;
/***************************************************************************/

typedef uint16_t  bldConf_CustomDataVersion_t;
/***************************************************************************/

typedef uint16_t  bldConf_EELockBits_t;
/***************************************************************************/

typedef uintptr_t _reserved_t;


/***************************************************************************//**
 * Type definition of bootloader NAD configuration
 ******************************************************************************/
typedef uint8_t   bldConf_NAD_t;

/***************************************************************************/
typedef uint16_t  bldConf_ApplicationLocation_t;


/***************************************************************************/
struct bldConf_sApplicationInfoData  
{   
  bldConf_ApplicationLocation_t  ResetVectorLocation;
  bldConf_ApplicationLocation_t  CRCStartAddress; // has to be a word aligned address!
  bldConf_ApplicationLocation_t  CRCWordCount;    // has to be a word aligned !
  bldConf_CRC_t                  ExpectedCRC; // ... over application [CRCStartAddress,CRCStartAddress + CRCWordCount] if &CRC within this range, that word is ommited.  
};


typedef struct bldConf_sApplicationInfoData       bldConf_ApplicationInfoData_t;
typedef        bldConf_ApplicationInfoData_t *    bldConf_pApplicationInfoData_t;
typedef const  bldConf_ApplicationInfoData_t *    bldConf_cpApplicationInfoData_t;

#ifdef BLD_SUPPORT_APPCRC_CHECKING

typedef enum
{
  AIA_CALC_CRC            = 2,
  
} bldConf_ApplicationInfoAction_t;

#endif


/***************************************************************************/

/***************************************************************************//**
 * Type definition for the 3 SNPD Modes
 ******************************************************************************/
typedef enum 
{
  bldConf_SNPD_SELECT_TYPE1    = 0u,      /**< @brief LINAA Type1 BSM with internal 1 Ohm shunt. */
  bldConf_SNPD_SELECT_TYPE2    = 1u,      /**< @brief LINAA Type2 BSM with external 0.2 Ohm shunt. */
  bldConf_SNPD_SELECT_TYPE3    = 2u       /**< @brief LINAA Type3 BSM with external 0.2 Ohm shunt. */
} bldConf_SNPDMode_t;

typedef uint16_t          bldConf_LINDynamicReadByIdentResponseTable_t;
/***************************************************************************/

typedef uint16_t          bldConf_CustomConfigDataLocation_t;

/***************************************************************************/
typedef uint16_t          bldConf_ResetVectorLocation_t;

/***************************************************************************/
typedef uint16_t          bldConf_ProtectedConfigDataLocation_t;

/***************************************************************************/
typedef uint32_t          bldConf_Baudrate_t;

/***************************************************************************/
typedef uint16_t          bldConf_SampleExtValue_t;

/***************************************************************************/
typedef uint8_t           bldConf_SNPDSelect_t;
/***************************************************************************/
typedef uint16_t          bldConf_SNPDStepSize_t;

/***************************************************************************/


/***************************************************************************/

struct bldConf_sBootloaderConfigFlags
{    
  uint8_t TimeWindow                 : 3;  // -> 0 = off, 1 .. 7  = 1=20ms ... 2=30 ... 3=40 ... 7=80ms  
  uint8_t EnableSoftResetEntry       : 1;  // -> 1 = bootloader reenter via bootloader interface API enabled   
  // .... 
};

/***************************************************************************//**
 * Type definition for the bootloader config flags
 ******************************************************************************/
typedef struct bldConf_sBootloaderConfigFlags bldConf_BootloaderConfigFlags_t;


/***************************************************************************//**
 * Structure definition for the Bootloader config serial
 ******************************************************************************/
struct bldConf_sSerial // keep in sync with bldIf_pSerial_t!
{
  uint8_t Data[4];
};

/***************************************************************************//**
 * Type definition for the Bootloader config serial
 ******************************************************************************/
typedef struct bldConf_sSerial  bldConf_Serial_t;
typedef bldConf_Serial_t *      bldConf_pSerial_t;

/***************************************************************************//**
 * Structure definition for product information
 ******************************************************************************/
struct bldConf_sProductInfo // keep in sync with bldIf_sProductInfo
{
  uint16_t SupplierID;
  uint16_t FunctionID;
  uint8_t  VariantID;
  uint8_t  Stepping;
};

/***************************************************************************//**
 * Type definition for product information
 ******************************************************************************/
typedef struct bldConf_sProductInfo bldConf_ProductInfo_t;

/***************************************************************************//**
 * Structure definition for the SNPD Type 3 config
 ******************************************************************************/
struct bldConf_sSNPDType3Config // configuration for type3 LIN auto addressing
{
  bldConf_SNPDThres_t       SNPDSelectThres;
  bldConf_SNPDThres_t       MaxIShuntSelect;
  bldConf_SNPDThres_t       ISel_StepSize;
  bldConf_SNPDThres_t       ISel_StepSizeMax;
};

/***************************************************************************//**
 * Type definition for the SNPD Type 3 config
 ******************************************************************************/
typedef struct bldConf_sSNPDType3Config bldConf_SNPDType3Config_t;

/***************************************************************************//**
 * Structure definition for the bootloader key
 ******************************************************************************/
struct bldConf_sKey
{
  uint8_t Data[8];
};

/***************************************************************************//**
 * Type definition for the bootloader key
 ******************************************************************************/
typedef struct bldConf_sKey  bldConf_Key_t;
typedef bldConf_Key_t *      bldConf_pKey_t;

/***************************************************************************//**
 * Structure definition for the bootloader access bits
 ******************************************************************************/
struct bldConf_sBootloaderAccess_bit
{
  uint8_t EnableFLASHAccess           : 1;
  uint8_t EnableEEPROMAccess          : 1;
  uint8_t EnableRAMAccess             : 1;
  uint8_t EnableFLATAccess            : 1;
  uint8_t EnableReadAccess            : 1;
  uint8_t EnableProcCall              : 1;
  uint8_t EnableExit                  : 1;  
};

/***************************************************************************//**
 * Union for the bootloader access bits
 ******************************************************************************/
union bldConf_uBootloaderAccess
{
  struct bldConf_sBootloaderAccess_bit Flags;
  uint8_t                              Value;
};

/***************************************************************************//**
 * Type definition for the bootloader access bits (Data, pointer and constant pointer)
 ******************************************************************************/
typedef union bldConf_uBootloaderAccess     bldConf_BootloaderAccess_t;
typedef       bldConf_BootloaderAccess_t *  bldConf_pBootloaderAccess_t;
typedef const bldConf_BootloaderAccess_t *  bldConf_cpBootloaderAccess_t;



/***************************************************************************/
/* PRQA S 3630 2 #justification: this struct is not referenced by in this project, but can be accessed
 *                               externally over an API by other projects. */                
struct bldConf_sDeviceSerial // keep in sync with bldIf_DeviceSerial_t!
{
  uint8_t Data[4];
};

typedef struct bldConf_sDeviceSerial  bldConf_DeviceSerial_t;
typedef bldConf_DeviceSerial_t *      bldConf_pDeviceSerial_t;





struct bldConf_sDevID
{
  uint8_t Data[8];
};

typedef struct bldConf_sDevID  bldConf_DevID_t;
typedef bldConf_DevID_t *      bldConf_pDevID_t;

/***************************************************************************/
struct bldConf_sBootloaderSNPDConfig
{
  uint16_t PullUpCurrent               : 5;
  uint16_t Step5WaitMode               : 1; 
  uint16_t EndOfBreakMode              : 1; 
  uint16_t ImmediateNADUpdate          : 1; 
  
};

typedef struct bldConf_sBootloaderSNPDConfig bldConf_BootloaderSNPDConfig_t;

/***************************************************************************/
struct bldConf_sBootloaderStaticConfigFlags
{    
  uint8_t DisableLINMsgTimeout        : 1; 
};

/***************************************************************************//**
 * Type definition for bootloader config SID
 ******************************************************************************/
typedef uint8_t  					bldConf_SID_t;
  // .... 

/***************************************************************************//**
 * Type definition for the device serial location (PUDIN)
 ******************************************************************************/
typedef uintptr_t         bldConf_DeviceSerialLocationLocation_t;

/***************************************************************************//**
 * Type definition for the device serial location
 ******************************************************************************/
typedef uintptr_t         bldConf_DeviceSerialLocation_t;



/***************************************************************************//**
 * Type definition for the SNPD method Id
 ******************************************************************************/
typedef uint8_t  					bldConf_SNPDMethodID_t; 

typedef struct bldConf_sBootloaderStaticConfigFlags bldConf_BootloaderStaticConfigFlags_t;

enum bldConf_eOutputCurrentDestination  
{
	bldConf_eCurrentDestinaion_LIN_S=0u,
	bldConf_eCurrentDestinaion_LIN_M=1u,
};

typedef enum bldConf_eOutputCurrentDestination bldConf_OutputCurrentDestination_t;

      
/***************************************************************************/
struct bldConf_sBootloaderCustomConfigFlags
{    
  uint16_t EnableEEPROMConfig          : 1; 
  uint16_t EnableSNPD                  : 1;   
  uint16_t EnableLINHighSpeed          : 1; 
  uint16_t DisableLINMsgTimeout        : 1;
  uint16_t DetectPostPIDCollisions     : 1;
  uint16_t PowerOff                    : 1; 
  uint16_t ExitOnIdle                  : 1; 
  uint16_t reserved0                   : 1;  
  uint16_t CheckAppCRC                 : 1;  
  uint16_t WakeUpFlags                 : 5;
  uint16_t reserved1                   : 2;
  /*WakeUpFlags:
  PORWakeUpFlag   -0
  LINWakeUpFlag   -1
  TimerWakeUpFlag -2
  TempWakeUpFlag  -3
  JTAGWakeUpFlag  -4
  */

  // .... 
};

typedef struct bldConf_sBootloaderCustomConfigFlags bldConf_BootloaderCustomConfigFlags_t;
/***************************************************************************//**
 * Structure definition for the SNPD config flags
 ******************************************************************************/
struct bldConf_sBootloaderSNPDConfigFlags
{
  uint8_t ImmediateNADUpdate                           : 1;
  uint8_t CheckMethodID                                : 1;
  uint8_t EnableType3ReverseCurrentDetection           : 1;
  uint8_t EnableType3MeasOnlyModeUsage                 : 1;
  uint8_t reserved                                     : 4;
};

typedef uint16_t bldConf_SNPDReverseCurrDetectThreshold_t;

typedef uint16_t bldConf_ADC_F12Config_t;
typedef uint16_t bldConf_ADC_SampleExtAAConfig_t;
/***************************************************************************//**
 * Type definition for the SNPD config flags
 ******************************************************************************/
typedef struct bldConf_sBootloaderSNPDConfigFlags bldConf_BootloaderSNPDConfigFlags_t;

/***************************************************************************//**
 * Structure definition for the SNPD physical configuration
 ******************************************************************************/
struct bldConf_sSNPDPhyCfg
{
  uint8_t MeasInMuxSelect           : 1;
  uint8_t GainSelect                : 2;
  uint8_t EnableFilter              : 1;
  uint8_t OffsetSelect              : 1;
  uint8_t AmpSelect                 : 1;
  uint8_t EnableCommonModeMeas      : 1;
  uint8_t OutputCurrentDestination  : 1;
};

/***************************************************************************//**
 * Type definition for the SNPD physical configuration
 ******************************************************************************/
typedef struct bldConf_sSNPDPhyCfg bldConf_SNPDPhyCfg_t;


/***************************************************************************//**
 * Structure definition for the SNPD FSM config
 ******************************************************************************/
struct bldConf_sSNPDFSMCfg
{
  uint8_t EndOfBreakMode            : 1; 
  uint8_t Step4WaitMode             : 1;
  uint8_t Step5WaitMode             : 1;
  uint8_t FSMDiffSelect             : 1; 
};

typedef struct bldConf_sSNPDFSMCfg bldConf_SNPDFSMCfg_t;

/***************************************************************************//**
 * Structure definition for the SNPD config mode
 ******************************************************************************/
struct bldConf_sSNPDModeConfig
{
  uint16_t  EnablePullUp             : 1;
  uint16_t  EnableCurrentSources     : 1;
  uint16_t  SNPDPullUpCurrent        : 7;
  uint16_t  EnableCurrentRamp        : 1;
  uint16_t  OversamplingRate         : 2;
  uint16_t  reserved                 : 4;
  
};
typedef struct bldConf_sSNPDModeConfig bldConf_SNPDModeConfig_t;

struct bldConf_sSNPDConfig
{
  bldConf_BootloaderSNPDConfigFlags_t            SNPDConfigFlags;
  bldConf_SNPDSelect_t                           SNPDModeSelect;
  bldConf_SNPDPhyCfg_t                           SNPDPhyConfig;
  bldConf_SNPDFSMCfg_t                           SNPDFsmConfig;
  bldConf_SNPDType3Config_t                      SNPDType3Config; 
  bldConf_SNPDModeConfig_t                       SNPDModePreselectConfig;
  bldConf_SNPDModeConfig_t                       SNPDModeSelectConfig;
};

typedef struct bldConf_sSNPDConfig bldConf_SNPDConfig_t;

/***************************************************************************//**
 * Structure definition for the debouncing configuration
 ******************************************************************************/
struct bldConf_sDebounceConfig_t
{    
  uint8_t DebounceValue               : 3; 
  uint8_t                             : 1;
  uint8_t ShortDetectDebounceValue    : 3;   
  uint8_t                             : 1;
  
  // .... 
};



typedef uint16_t bldConf_ReadByIdentTableLocation_t;
typedef uint16_t bldConf_ReadByIdentVersion_t;
typedef uint16_t bldConf_ReadByIdentCookie_t;

/***************************************************************************//**
 * Type definition for the debouncing configuration
 ******************************************************************************/
typedef struct bldConf_sDebounceConfig_t bldConf_DebounceConfig_t;


typedef uint16_t bldConf_flashLockBits_t;
typedef uint8_t  bldConf_flashExitStrategy_t;

struct bldConf_sflashConfig
{
   bldConf_flashLockBits_t      FlashFuseLockBits;
   bldConf_flashLockBits_t      FlashMainLockBits;
   bldConf_flashLockBits_t      FlashMainFineLv0LockBits;
   bldConf_flashLockBits_t      FlashMainFineLv1LockBits;
   bldConf_flashLockBits_t      FlashMainFineLv2LockBits;
   bldConf_flashLockBits_t      FlashMainFineLv3LockBits;
   bldConf_flashExitStrategy_t  FlashIPExitStrategySelect;
};

typedef struct bldConf_sflashConfig bldConf_flashConfig_t;

/***************************************************************************/

/* Has to be word aligned */
struct bldConf_sConfigData
{
  bldConf_NAD_t                         NodeAddress;
  bldConf_BootloaderAccess_t            AccessFlags;
  bldConf_BootloaderConfigFlags_t       Flags;             // actually in this setup time window only ...
  uint8_t                               _reserved;
  bldConf_SNPDThres_t                   SNPDPreselectThres; 
  bldConf_SNPDThres_t                   SNPDSelectThres; 
  bldConf_Serial_t                      Serial;
};



typedef struct bldConf_sConfigData     bldConf_ConfigData_t;
typedef        bldConf_ConfigData_t *  bldConf_pConfigData_t;
typedef const  bldConf_ConfigData_t *  bldConf_cpConfigData_t;
/***************************************************************************/

/* Has to be word aligned. */
struct bldConf_sCustomConfigData  // this is bootloader config data which is fixed, but customer changeable. 
{
   bldConf_ProductInfo_t                          ProductInfo;
   bldConf_SID_t                                  BootloaderServiceID;
   bldConf_SID_t                                  SNPDServiceID;
   bldConf_NAD_t                                  DefaultNAD;
   bldConf_Baudrate_t                             Baudrate;
   bldConf_DebounceConfig_t                       DebounceConfig;
   bldConf_BootloaderCustomConfigFlags_t          Flags;
   bldConf_SNPDConfig_t                           SNPDConfig;
   bldConf_SNPDMethodID_t                         SNPDMethodID;
   bldConf_Key_t                                  Key;

   _reserved_t                                    _reserved;
   bldConf_flashConfig_t                          FlashConfig;
   bldConf_ReadByIdentTableLocation_t             ReadByIdentExtensionTableLocation;
   bldConf_ApplicationInfoData_t                  ApplicationInfoData;     
};
   
typedef struct bldConf_sCustomConfigData     bldConf_CustomConfigData_t;
typedef        bldConf_CustomConfigData_t *  bldConf_pStaticCustomConfigData_t;
typedef const  bldConf_CustomConfigData_t *  bldConf_cpStaticCustomConfigData_t;
/***************************************************************************/

struct bldConf_sConstCustomData
{
  bldConf_CustomDataVersion_t        Version;    
  bldConf_CRC_t                      CRC; // ... over both config structs CustomConfig and DefaultRuntimeConfig
 
  bldConf_CustomConfigData_t         CustomConfig;              // make sure bldConf_CustomConfigData_t is properly aligned, so there is no gap (due to CRC calculation) between CustomConfig and DefaultRuntimeConfig
  bldConf_ConfigData_t               DefaultRuntimeConfig; 
  
};

typedef struct bldConf_sConstCustomData    bldConf_ConstCustomData_t;
typedef bldConf_ConstCustomData_t *        bldConf_pConstCustomData_t;
typedef const bldConf_ConstCustomData_t *  bldConf_cpConstCustomData_t;
/***************************************************************************//**
 * Structure definition for the Static config data
 ******************************************************************************/
struct bldConf_sStaticConfigData  // this is factory configurable data stored by elmos! (this struct has to have a word aligned size)
{
   bldConf_DevID_t                               DeviceID;    
   bldConf_DeviceSerialLocation_t                DeviceSerialLocation;                // 
   bldConf_CustomConfigDataLocation_t            CustomConfigDataLocation;      //  
   bldConf_ProtectedConfigDataLocation_t         RuntimeConfigDataLocation;           // ... within EEPROM ...
   bldConf_SNPDReverseCurrDetectThreshold_t      ReverseCurrentDetectionThreshold;    //SNPD T3 reverse current detection threshold (internal)
   bldConf_ADC_F12Config_t                       ADCF12Config;
   bldConf_ADC_SampleExtAAConfig_t               ADCSampleExtAAConfig;
   bldConf_EELockBits_t                          EELockBits;                          // used to protect runtime config. combined LOCK_L/LOCK_U value.    
   // ....
};

/***************************************************************************//**
 * Type definition for the Static config data (Data, Pointer and Constant pointer
 ******************************************************************************/
typedef struct bldConf_sStaticConfigData     bldConf_StaticConfigData_t;
typedef       bldConf_StaticConfigData_t *   bldConf_pStaticConfigData_t;
typedef const bldConf_StaticConfigData_t *   bldConf_cpStaticConfigData_t;


/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* BOOTLOADER_USERCONFIG_H_ */  
