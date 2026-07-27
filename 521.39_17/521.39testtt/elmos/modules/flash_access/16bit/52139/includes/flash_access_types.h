/***************************************************************************//**
 * @file			flash_access_types.h
 *
 * @creator		POE
 * @created		02.04.2019
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief  		Data types for module flash access
 *
 * @purpose
 *
 * Definitions of Enum-, Structure- or Atomic data types.
 *
 * $Id: flash_access_types.h 8988 2021-06-18 14:39:43Z poe $
 *
 * $Revision: 8988 $
 *
 ******************************************************************************/

#ifndef FLASH_ACCESS_TYPES_H_
#define FLASH_ACCESS_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"
#include "el_helper.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define FLASH_RETRY_COUNT               (0x03u)
#define FLASH_ADDRESS_ITEM_SIZE         (sizeof(uint16_t))
  
/* NVM_READ_BACK_OFFSET  0x04000000u    NVM is mirrored into the memory space several time. use this higher lcations to read back, without exception table overlay*/

#define FLASH_WORD_ALIGNMENT_MASK       (0x01u)
#define FLASH_DWORD_ALIGNMENT_MASK      (0x03u)

#define FLASH_WORD_LOW_MASK             (0x00FFu)
#define FLASH_WORD_HIGH_MASK            (0xFF00u)

#if defined (BUS_BANDWIDTH_16)
  #define FLASH_ADDR_RANGE_TYPE (uint16_t)
#else
  #define FLASH_ADDR_RANGE_TYPE (uint32_t)
#endif


/* ****************************************************************************/
/* ********************STRUCTS, ENUMS AND TYPEDEFS ****************************/
/* ****************************************************************************/
typedef void    *                               flash_pvoid_t;                /**< Void-pointer data type. */
typedef const   void *                          flash_cpvoid_t;               /**< Constant void-pointer data type. */


typedef         intptr_t                        flash_intptr_t;               /**< Signed integer type capable of holding an object pointer. */
typedef         uintptr_t                       flash_uintptr_t;              /**< Unsigned integer type capable of holding an object pointer. */

typedef         flash_pvoid_t                   flashIf_pGenericEnvData_t;    /**< Generic pointer to environment data of the FLASH access module. */
typedef         flash_cpvoid_t                  flashIf_cpGenericEnvData_t;   /**< Generic pointer to constant environment data of the FLASH access module. */
typedef         flash_uintptr_t                 flash_EnvDataSze_t;           /**< Generic type for the environment data size. */
typedef         flash_EnvDataSze_t              flashIf_EnvDataSze_t;         /**< FLASH access data type for the environment data length. */
typedef         flash_pvoid_t                   flashIf_pGenericImpCfgData_t; /**< Generic pointer to configuration parameter of the specific flash access implementation. */
typedef         flash_cpvoid_t                  flashIf_cpGenericImpCfgData_t;/**< Generic pointer to constant configuration parameter of the specific flash access implementation. */
typedef         flash_pvoid_t                   flashIf_pGenericCbCtxData_t;  /**< Pointer to flash access callback context data. */

struct          flashIf_sInterfaceFunctions;                                     /**< Forward declaration of flash access interface functions. */
typedef struct  flashIf_sInterfaceFunctions     flashIf_InterfaceFunctions_t;   /**< Typedef for flashIf_sInterfaceFunctions. */
typedef         flashIf_InterfaceFunctions_t*   flashIf_pInterfaceFunctions_t;  /**< Typedef of pointer to flashIf_sInterfaceFunctions. */
typedef const   flashIf_InterfaceFunctions_t*   flashIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to flashIf_sInterfaceFunctions. */

struct          flashIf_sCallbackFunctions;                                     /**< Forward declaration of flash access callback functions. */
typedef struct  flashIf_sCallbackFunctions      flashIf_CallbackFunctions_t;   /**< Typedef for flashIf_sCallbackFunctions. */
typedef         flashIf_CallbackFunctions_t*    flashIf_pCallbackFunctions_t;   /**< Typedef of pointer to flashIf_sCallbackFunctions. */
typedef const   flashIf_CallbackFunctions_t*    flashIf_cpCallbackFunctions_t;  /**< Typedef of constant pointer to flashIf_sCallbackFunctions. */


typedef         uint16_t                        flashIf_Error_t;

typedef         uint16_t                        flashIf_Version_t;            /**< @brief NDVNM interface version */

typedef         uint8_t                         flash_PageCount_t;
typedef         uint16_t                        flash_ByteCount_t;
typedef         bool_t                          flash_Bool_t;
#if defined ( __ICC430__ )
typedef         uint16_t                        flash_Address_t;
#else
typedef         uint32_t                        flash_Address_t;
#endif
typedef         flash_Address_t*                flash_pAddress_t;
typedef const   flash_Address_t*                flash_cpAddress_t;
typedef         uint16_t                        flash_PageOffset_t;
typedef         uint16_t                        flash_Length_t;
typedef         uint16_t                        flash_Password_t;
typedef         uint16_t                        flash_Checksum_t;
typedef         flash_Checksum_t*               flash_pChecksum_t;

typedef         uint8_t                        flash_ByteData_t;
typedef         uint8_t *                      flash_pByteData_t;
typedef const   uint8_t *                      flash_cpByteData_t;

typedef         uint16_t                        flash_WordData_t;
typedef         uint16_t *                      flash_pWordData_t;
typedef const   uint16_t *                      flash_cpWordData_t;

#if defined ( __ICC430__ )
  typedef         uint16_t                        flash_Data_t;
  typedef         uint16_t *                      flash_pData_t;
  typedef const   uint16_t *                      flash_cpData_t;
#else
  typedef         uint32_t                        flash_Data_t;
  typedef         uint32_t *                      flash_pData_t;
  typedef const   uint32_t *                      flash_cpData_t;
#endif
typedef         uint16_t                        flash_LockBits_t;




enum flash_eErrorCode
{
  flash_eErrorCode_NoError                    = 0x00u,
  flash_eErrorCode_RstBusyTimeout             = 0x01u,
  flash_eErrorCode_MinTimeout                 = 0x02u,
  flash_eErrorCode_WrBusyTimeout              = 0x03u,
  flash_eErrorCode_WrongModeActive            = 0x04u,
  flash_eErrorCode_ParameterOutOfRange        = 0x31u,
  flash_eErrorCode_AccessDenied               = 0x33u,
  flash_eErrorCode_ParameterError             = 0x70u,
  flash_eErrorCode_GeneralProgrammingFailure  = 0x72u,
  flash_eErrorCode_VerifyFailure              = 0x73u, 
  flash_eErrorCode_IpBusyFailure              = 0x74u,
  flash_eErrorCode_IpRecoveryFailure          = 0x75u,
  flash_eErrorCode_SupplyFailure              = 0x92u,
  
};

typedef enum flash_eErrorCode flash_ErrorCode_t;


enum flash_eEraseFormatSelect
{
  flash_eEraseFormatSelect_Page   =0u,
  flash_eEraseFormatSelect_Sector =1u,
  flash_eEraseFormatSelect_Mass   =2u
};

typedef enum flash_eEraseFormatSelect flash_EraseFormatSelect_t;

enum flash_eCommandSelect
{
  flash_eCommandSelect_Read         = 0u,
  flash_eCommandSelect_Program      = 1u,
  flash_eCommandSelect_ErasePage    = 2u,
  flash_eCommandSelect_EraseSector  = 3u,
  flash_eCommandSelect_EraseMass    = 4u,
  flash_eCommandSelect_Verify       = 5u,
  flash_eCommandSelect_MarginRead   = 6u
};

typedef enum flash_eCommandSelect flash_CommandSelect_t;

enum flash_eVerifyOp
{
  flash_eVerifyOp_Write_RefData      = 0u,
  flash_eVerifyOp_Erase_ExpState     = 1u,
  flash_eVerifyOp_Write_NoRefData    = 2u
};

typedef enum flash_eVerifyOp flash_VerifyOp_t;


enum flash_eNegativeResponseType
{
  flash_eNegativeResponseType_NoError                           = 0x00u,
  flash_eNegativeResponseType_PreconditionsIncorrect            = 0x22u,
  flash_eNegativeResponseType_ParameterOutOfRange               = 0x31u,
  flash_eNegativeResponseType_AccessDenied                      = 0x33u,
  flash_eNegativeResponseType_ParameterError                    = 0x70u,
  flash_eNegativeResponseType_General_Programming_Failure       = 0x72u,
};

typedef enum flash_eNegativeResponseType flash_NegativeResponseType_t;


/**********************************************************************/

typedef enum
{
  FLA_ACCESS_MAIN = 0u,
  FLA_ACCESS_FUSE = 1u
} flash_AccessMode_t;

/**********************************************************************/

typedef enum
{
  flash_eProtectionLevel_0 = 0u,
  flash_eProtectionLevel_1 = 1u,
  flash_eProtectionLevel_2 = 2u,
  flash_eProtectionLevel_3 = 3u
} flash_ProtectionLevel_t;


/**********************************************************************/

enum flash_eExitStrategyOnFatalIPError
{
  flash_eExitStrategy_NONE                  = 0u,         /**< @brief On fatal IP error, return an error code. */
  flash_eExitStrategy_CALLBACK              = 1u,         /**< @brief On fatal IP error, execute an error callback. */
  flash_eExitStrategy_SAFESTATE             = 2u,         /**< @brief On fatal IP error, disable all irqs and stay in RAM based infinite loop. */
  flash_eExitStrategy_SWRESET               = 3u          /**< @brief On fatal IP error, trigger a SW reset. */
};

typedef enum flash_eExitStrategyOnFatalIPError flash_ExitStrategyOnFatalIPError_t;


/**********************************************************************/
enum flash_ePassphraseType
{
  flash_ePassphraseType_UserConfig        = 0u,
  flash_ePassphraseType_EraseProgram      = 1u,
  flash_ePassphraseType_Verify            = 2u,
  flash_ePassphraseType_Stop              = 3u,
  flash_ePassphraseType_InitialFuseRead   = 4u
};

typedef enum flash_ePassphraseType flash_PassphraseType_t;

enum flash_eCookieType
{
    flash_cpm_ps0 =  (0x71),
    flash_cpf_ps0 =  (0x00),
    flash_cpm_ps2 =  (0x08),
    flash_cpm_ps1 =  (0xD2),
    flash_cpm_ps3 =  (0xCF)
};

typedef enum flash_eCookieType flash_CookieType_t;

struct flash_sPassphrase
{
  flash_Password_t pass1;
  flash_Password_t pass2;
  flash_Password_t pass3;
};

typedef struct  flash_sPassphrase   flash_Passphrase_t;
typedef         flash_Passphrase_t* flash_pPassphrase_t;
typedef const   flash_Passphrase_t* flash_cpPassphrase_t;

/* PRQA S 3630 ++ #This is a struct definition that is API part of module
                   flash_acceess. It can be used by used from application 
                   outside this rom project. It should not
                   be hidden! */
struct flash_sPassphrases
{
  flash_Passphrase_t ExternalPassphrase;
  flash_Passphrase_t Verify;
  flash_Passphrase_t Stop;
};
// PRQA S 3630 --

typedef struct flash_sPassphrases    flash_Passphrases_t;
typedef        flash_Passphrases_t*  flash_pPassphrases_t;
typedef const  flash_Passphrases_t*  flash_cpPassphrases_t;



/* ****************************************************************************/
/*  SFLASH                                                                    */
/* ****************************************************************************/


/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* FLASH_ACCESS_TYPES_H_ */
