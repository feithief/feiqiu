/** @addtogroup flash_interface
 * @{ */
/***************************************************************************//**
 * @file			flash_interface.h
 *
 * @creator		poe
 * @created		15.05.2019
 *
 * @brief  		Interface and definitions for the bootloader ROM flash access functions. 
 *
 *
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
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
 * @purpose
 * This interface offers diagnostics functions.
 *
 * $Id: flash_access_interface.h 9264 2021-09-29 13:43:22Z poe $
 *
 * $Revision: 9264 $
 * -----------------------------------------------------------------------------
 * Date      Version    Who   Comment
 * YY/MM/DD  Ma.Mi.Bd
 * -----------------------------------------------------------------------------
 * 21/05/06  1.0.2      POE   reduced API to essential functions.
 * 20/04/15  1.0.1      POE   updated srcPtr and dstPtr to be const types.
 * 19/05/15  1.0.0      POE   Initial creation.
 * -----------------------------------------------------------------------------
 ******************************************************************************/

#ifndef _FLASH_INTERFACE_H_
#define _FLASH_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"
#include "flash_access_types.h"
/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define FLASH_INTERFACE_MODULE_API_VERSION     (0x0102u)     /**< @brief flash interface version */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/


/* *****************************************************************************
 * Interface callback function types
 ******************************************************************************/
  

/***************************************************************************//**
 * @brief Typedef of flash access 'Initialization' interface function.
 *
 * @param[in] genericFlashEnvData     Pointer to reserved flash access environment data.
 * @param[in] flashEnvDataSze         Size of the reserved RAM for flash access environment
 *                                    data.
 * @param[in] flashCbFuns             Pointer to flash access callback definitions.
 * @param[in] genericFlashCbCtxData   Pointer to flash access callback context data.
 * @param[in] genericFlashImpCfgData  Pointer to implementation dependent
 *                                    configuration data for the flash access module.
 *                                    \(flashImp_sCfgData)
 *
 * @return      flash_eErrorCode_NoError                    - Initialization was successful,
 *              flash_eErrorCode_ParameterError             - Parameter set was invalid
 *
 ******************************************************************************/
typedef flash_ErrorCode_t (*flashIf_MemInitialization_t)( flashIf_pGenericEnvData_t genericFlashEnvData, 
                                                          flashIf_EnvDataSze_t flashEnvDataSze, 
                                                          flashIf_cpCallbackFunctions_t flashCbFuns, 
                                                          flashIf_pGenericCbCtxData_t  genericFlashCbCtxData, 
                                                          flashIf_cpGenericImpCfgData_t genericFlashImpCfgData);


/***************************************************************************//**
 * Erases pages containing given number of bytes starting from dptr as start address.
 *
 * @param       genericFlashEnvData - Flash access environment data.
 * @param       passphrases         - Passphrase for current action.
 * @param       dstPtr              - Destination pointer for current action (must be page aligned).
 * @param       byteNum             - Number of bytes length of target area (must be page aligned).
 * @param       pPageEraseCount     - Pointer to a result counter - will contain 
 *                                    number of erased pages after success.
 *
 * @return      flash_eErrorCode_NoError  - Erase and verify operation was executed successfully
 *              flash_eErrorCode_IpRecoveryFailure - Due to a WrBusy- or RstBusy-Timeout a IP Recovery Reset was initiated which failed
 *              flash_eErrorCode_ParameterError - Alignment error or address check failed
 *              flash_eErrorCode_GeneralProgrammingFailure - No valid pointer to environment data
 *              flash_eErrorCode_IpBusyFailure - Ip was not ready for command
 *              flash_eErrorCode_WrongModeActive - Command was executed in wrong operating mode
 *              flash_eErrorCode_AccessDenied - Memory protected
 *              flash_eErrorCode_SupplyFailure - Supply voltage had a fail state
 *              flash_eErrorCode_MinTimeout - Min. threshold of busy measurement length not reached
 *
 ******************************************************************************/
typedef flash_ErrorCode_t (*flashIf_MemErasePage_t)( flashIf_cpGenericEnvData_t genericFlashEnvData, 
                                                     flash_cpPassphrases_t passphrases, 
                                                     flash_cpAddress_t dstPtr, 
                                                     const flash_Length_t byteNum, 
                                                     flash_PageCount_t* pPageEraseCount);


/***************************************************************************//**
 * Writes bytes from sptr to dptr.
 *
 * @param       genericFlashEnvData - Flash access environment data.
 * @param       passphrases         - Passphrase for current action.
 * @param       srcPtr              - Pointer to source data (must not be in destination area)
 * @param       dstPtr              - Destination address (must be 32bit size aligned)
 * @param       byteNum             - Number of bytes to write (must be 32bit size aligned)
 * @param       areaSelect          - Write in this area (MAIN or FUSE)
 *
 * @return      flash_eErrorCode_NoError - Write operation was successful
 *              flash_eErrorCode_IpRecoveryFailure - Due to a WrBusy- or RstBusy-Timeout a IP Recovery Reset was initiated which failed
 *              flash_eErrorCode_ParameterError - Alignment error or address check failed
 *              flash_eErrorCode_GeneralProgrammingFailure - No valid pointer to environment data
 *              flash_eErrorCode_IpBusyFailure - Ip was not ready for command
 *              flash_eErrorCode_WrongModeActive - Command was executed in wrong operating mode
 *              flash_eErrorCode_AccessDenied - Memory protected
 *              flash_eErrorCode_SupplyFailure - Supply voltage had a fail state
 *              flash_eErrorCode_MinTimeout - Min. threshold of busy measurement length not reached
 *
 ******************************************************************************/
typedef flash_ErrorCode_t (*flashIf_MemWrite_t)( flashIf_cpGenericEnvData_t genericFlashEnvData, 
                                                 flash_cpPassphrases_t passphrases, 
                                                 flash_cpByteData_t srcPtr, 
                                                 flash_cpAddress_t dstPtr, 
                                                 const flash_Length_t byteNum);

/***************************************************************************//**
 * Configure a protection access for target protection level.
 *
 * @param  genericFlashEnvData  -  Flash access environment data.
 * @param  passphrases          -  Configured passphrases for current action.
 * @param  protAccessLevel      -  Target access protection level.
 * @param  areaSelect           -  Area selection (MAIN or FUSE)
 * @param  lockProt             -  Normal access protection mask.
 * @param  lockProtFine         -  Fine access protection mask (if appliable).
 *
 * @return      flash_eErrorCode_NoError                    - Protection update was successful,
 *              flash_eErrorCode_GeneralProgrammingFailure  - Environment data was invalid
 *
 ******************************************************************************/
typedef flash_ErrorCode_t (*flashIf_MemSetProtectionAccess_t)( flashIf_cpGenericEnvData_t genericFlashEnvData, 
                                                               flash_cpPassphrase_t passphrase, 
                                                               flash_ProtectionLevel_t protAccessLevel, 
                                                               flash_AccessMode_t areaSelect, 
                                                               flash_LockBits_t lockProt, 
                                                               flash_LockBits_t lockProtFine);

 
/***************************************************************************//**
 * @brief md nvm access interface functions
 ******************************************************************************/
struct flashIf_sInterfaceFunctions
{
    flashIf_Version_t                     InterfaceVersion;                   /**< @copydoc flashIf_Version_t */
  
    flashIf_MemInitialization_t           Initialization;                     /**< @copydoc flashIf_MemInitialization_t */
    flashIf_MemWrite_t                    MemoryWrite;                        /**< @copydoc flashIf_MemWrite_t */
    flashIf_MemErasePage_t                MemoryErasePage;                    /**< @copydoc flashIf_MemErasePage_t */ 
    flashIf_MemSetProtectionAccess_t      MemoryProtect;                      /**< @copydoc flashIf_MemSetProtectionAccess_t */
};



/* *****************************************************************************
 * FLASH access callback function types
 ******************************************************************************/
/** @addtogroup flashIfCbFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of flash access 'ExitStrategy' callback function.
 *
 * @param[in] genericFlashEnvData   Pointer to reserved flash access environment data.
 * @param[in] flashIfFuns           Pointer to the flash access interface function struct.
 * @param[in] error                 Occurred error.
 * @param[in] genericFlashCbCtxData Pointer to flash access callback context data.
 *
 * Indicates a fatal error in the flash access.
 *
 ******************************************************************************/
typedef void (*flashIf_ExitStrategyCbFun_t) (flashIf_cpGenericEnvData_t   genericFlashEnvData, flashIf_cpInterfaceFunctions_t flashIfFuns,
                                             flashIf_Error_t             error,               flashIf_pGenericCbCtxData_t    genericFlashCbCtxData);



/***************************************************************************//**
 * @brief flash access callback functions struct.
 *
 * The set of callbacks which should be invoked in the higher layers.
 ******************************************************************************/
/* PRQA S 3630 ++ #This is an interface for callback funtions. They can be used by
                   application outside this rom project. It should not
                   be hidden! */
struct flashIf_sCallbackFunctions
{
  flashIf_Version_t              CallbackVersion;  /**< @brief Callback Version */

  flashIf_ExitStrategyCbFun_t    ExitStrategy;          /**< @brief Pointer to flash access 'ExitStrategy' callback function. (@ref flashIf_ExitStrategyCbFun_t) @copydetails flashIf_ExitStrategyCbFun_t */
};
//PRQA S 3630 --
/** @} flashIfCbFunDefs */


/***************************************************************************//**
 * @brief  Object-like This-pointer to connect the flash access mdoule to application.
 ******************************************************************************/
/* PRQA S 3630, 1755 2 #justification: this struct is not referenced by in this project, but can be accessed
 *                               externally over an API by other projects. */   
struct flashIf_sThis
{
    flashIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the flash access interface function struct. (flashIf_sInterfaceFunctions) */
    flashIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved flash access environment data. */
};

/***************************************************************************//**
 * @brief LOOKUP Layer interface configuration parameter.
 *
 * Data needed for initialization of the LOOKUP Layer.
 ******************************************************************************/
/* PRQA S 3630 ++ #justification: this struct is not referenced by in this project, but can be accessed
 *                               externally over an API by other projects. */   
struct flashIf_sInitParam
{
    flashIf_cpInterfaceFunctions_t IfFunsTbl;   /**< @brief Pointer to the constant flash access interface function struct. */
    flashIf_pGenericEnvData_t      envData;     /**< @brief Pointer to reserved flash access environment data. */
    flashIf_EnvDataSze_t           EnvDataLen;  /**< @brief Size of the reserved RAM for flash access environment data. */
    flashIf_pGenericImpCfgData_t   ImpCfgData;  /**< @brief Pointer to implementation dependent configuration data for the flash access. (flashImp_sCfgData) */
};
/* PRQA S 3630 -- */
/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/



#ifdef BUILD_ROM
extern const flashIf_InterfaceFunctions_t flashIf_InterfaceFunctions;
#endif

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* _FLASH_INTERFACE_H_ */
/** @} flash_interface */
