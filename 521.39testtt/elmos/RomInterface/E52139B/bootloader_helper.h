/***************************************************************************//**
 * @file        bootloader_types.h
 *
 * @creator     POE
 * @created     2021/02/08
 * @sdfv        TODO: Automotive Spice or Elmos Flow or Demo Flow
 *
 * @brief  		TODO: Short description of this module
 *
 * @purpose
 *
 * @addtogroup Bootloader
 * @{
 *
 * $Id: $
 *
 * $Revision: $
 *
 ******************************************************************************/

#ifndef __BL_HELPER_H__
#define __BL_HELPER_H__

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/



/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
/***************************************************************************//**
 * Memory section defines
 ******************************************************************************/

/* 16Bit memory sections. */

#define BLD_MEMSECT_16BIT_OTP             (0x00u)   /**< @brief 16bit OTP memory section ID. */
#define BLD_MEMSECT_16BIT_EEPROM          (0x01u)   /**< @brief 16bit EE memory section ID. */
#define BLD_MEMSECT_16BIT_MDNVM           (0x02u)   /**< @brief 16bit MDNVM memory section ID. */
#define BLD_MEMSECT_16BIT_FLASH_MAIN      (0x03u)   /**< @brief 16bit FLASH MAIN memory section ID. */
#define BLD_MEMSECT_16BIT_FLASH_FUSE      (0x04u)   /**< @brief 16bit FLASH FUSE/INFO memory section ID. */

#define BLD_MEMSECT_16BIT_FLAT_BYTE       (0x80u)   /**< @brief 16bit FLAT 8bit memory section ID. */
#define BLD_MEMSECT_16BIT_FLAT_WORD       (0x81u)   /**< @brief 16bit FLAT 16bit memory section ID. */
#define BLD_MEMSECT_16BIT_FLAT_DWORD      (0x82u)   /**< @brief 16bit FLAT 32bit memory section ID. */

#define BLD_MEMSECT_16BIT_RAM             (0x90u)   /**< @brief 16bit RAM memory section ID. */

/* 32Bit memory sections. */

#define BLD_MEMSECT_32BIT_OTP             (0x40u)   /**< @brief 32bit OTP memory section ID. */
#define BLD_MEMSECT_32BIT_EEPROM          (0x41u)   /**< @brief 32bit EE memory section ID. */
#define BLD_MEMSECT_32BIT_MDNVM           (0x42u)   /**< @brief 32bit MDNVM memory section ID. */
#define BLD_MEMSECT_32BIT_FLASH_MAIN      (0x43u)   /**< @brief 32bit FLASH MAIN memory section ID. */
#define BLD_MEMSECT_32BIT_FLASH_FUSE      (0x44u)   /**< @brief 32bit FLASH FUSE/INFO memory section ID. */

#define BLD_MEMSECT_32BIT_FLAT_BYTE       (0xC0u)   /**< @brief 32bit FLAT 8bit memory section ID. */
#define BLD_MEMSECT_32BIT_FLAT_WORD       (0xC1u)   /**< @brief 32bit FLAT 16bit memory section ID. */
#define BLD_MEMSECT_32BIT_FLAT_DWORD      (0xC2u)   /**< @brief 32bit FLAT 32bit memory section ID. */
                                                 
#define BLD_MEMSECT_32BIT_RAM             (0xD0u)   /**< @brief 32bit RAM memory section ID. */


/* Alignment masks. */

#define ALIGN_CHECK_MASK_16BIT            (0x0001u)
#define ALIGN_CHECK_MASK_32BIT            (0x0003u)


#define FLAT_SIZE_16BIT                   (0xFFFFu)
#define FLAT_SIZE_32BIT                   (0xFFFFFFFFu)

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

enum bld_eResponseType
{
  Bld_Response_Fail                                       = 0x00,
  Bld_Response_Success                                    = 0x01,
  Bld_Response_Ongoing                                    = 0x02
};

typedef enum bld_eResponseType bld_ResponseType_t;


/**< @brief Bootloader response error codes. */
enum bld_eNegativeResponseType
{
  bld_eNegativeResponseType_NoError                           = 0x00u,
  bld_eNegativeResponseType_FunctioNotSupported               = 0x12u,
  bld_eNegativeResponseType_MessageFormatInvalid              = 0x13u,
  bld_eNegativeResponseType_ResponseTooLong                   = 0x14u,
  bld_eNegativeResponseType_PreconditionsIncorrect            = 0x22u,
  bld_eNegativeResponseType_ParameterOutOfRange               = 0x31u,
  bld_eNegativeResponseType_AccessDenied                      = 0x33u,
  bld_eNegativeResponseType_ParameterError                    = 0x70u,
  bld_eNegativeResponseType_General_Programming_Failure       = 0x72u,
  bld_eNegativeResponseType_TemperatureTooLow                 = 0x86u,
  bld_eNegativeResponseType_TemperatureTooHigh                = 0x87u,
  bld_eNegativeResponseType_VoltageTooLow                     = 0x92u,
  bld_eNegativeResponseType_VoltageTooHigh                    = 0x93u,
};

/**< @brief Type definition for bootloader negative response types */
typedef enum bld_eNegativeResponseType bld_NegativeResponseType_t;


typedef enum
{
  BLD_ET_KEEP_RUNNING = 0,  // Attn: keep in sync with Exit Command!
  BLD_ET_SOFT_RESET   = 1,
  BLD_ET_HARD_RESET   = 2,
  BLD_ET_TEST_MODE    = 3
    
  
} bld_ExitType_t;

typedef uint16_t bld_ExitDelay_t;  // in ms

/****************************************************************************
  Configuration types
***************************************************************************/
#if defined ( __ICC430__ )
  typedef uint16_t    bldConf_LockWord_t;
#else
  typedef uint32_t    bldConf_LockWord_t;
#endif
/***************************************************************************/
#if defined ( __ICC430__ )
  typedef uint16_t          bldConf_CRC_t;
#else
  typedef uint32_t        bldConf_CRC_t;
#endif
/***************************************************************************/

struct bldConf_sVersion
{
  uint8_t  Major;
  uint8_t  Minor;
  uint16_t Build;
};

typedef struct bldConf_sVersion bldConf_Version_t;

/***************************************************************************/
typedef enum
{
  bldConf_CONF_EE_UNLOCKED = 0, // current config data can be changed 
  bldConf_CONF_EE_LOCKED   = 1, // current config data has been locked (either by lock word, or EEaccess inhibit)
} bldConf_Lock_t;


/******************************************************************************
 *************** SETMEMPASSWORD CMD
 ******************************************************************************/

typedef uint16_t bld_Password_t;

struct bld_sPassphrase
{
  bld_Password_t pass1;
  bld_Password_t pass2;
  bld_Password_t pass3;
};

typedef struct  bld_sPassphrase   bld_Passphrase_t;
typedef         bld_Passphrase_t* bld_pPassphrase_t;
typedef const   bld_Passphrase_t* bld_cpPassphrase_t;



/* ****************************************************************************/
/* ********************************* FUNCTIONS ********************************/
/* ****************************************************************************/

#endif /* __BL_HELPER_H__ */
 /** @} Elmos_Bootloader */
