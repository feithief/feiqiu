/***************************************************************************//**
 * @file      flash_access.h
 *
 * @creator   POE / SKU
 * @created   02.04.2019
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief       Provides access to the Flash memory of the 524.36 IC
 *
 * @purpose     This module accesses the memory registers of the 524.36 flash memory peripheral,
 *              to support following flash memory functions:
 *              - Writing 32 bit aligned data from SRAM or FLASH memory to the FLASH memory
 *                with subsequent data verification
 *              - Page erase, section erase or mass erase with subsequent data verification
 *              - Only verification of source data and destination data in flash memory
 *              - Setting of protection access of the FLASH Main and FLASH Fuse area
 *
 * $Id: flash_access.h 8988 2021-06-18 14:39:43Z poe $
 *
 * $Revision: 8988 $
 * -----------------------------------------------------------------------------
 * Date      Version    Who   Comment
 * YY/MM/DD  Ma.Mi.Bd
 * -----------------------------------------------------------------------------
 * 21/05/06  1.0.1      POE   synched with API functions.
 * 19/05/15  1.0.0      POE   Initial creation.
 * -----------------------------------------------------------------------------
 ******************************************************************************/

#ifndef FLASH_ACCESS_H_
#define FLASH_ACCESS_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"
#include "el_helper.h"
#include "flash_access_interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define FLASHIMP_CONFIG_DATA_VERSION    (0x0100u)

#define FLASH_RETRY_COUNT               (0x03u)
#define FLASH_ADDRESS_ITEM_SIZE         (sizeof(uint16_t))

#define FLASH_WORD_ALIGNMENT_MASK       (0x01u)
#define FLASH_DWORD_ALIGNMENT_MASK      (0x03u)

#define FLASH_WORD_LOW_MASK             (0x00FFu)
#define FLASH_WORD_HIGH_MASK            (0xFF00u)

#define BUS_BANDWITH                     (16u)

#define FLASHIMP_ENVIRONMENT_DATA_SZE    (6u)
/* ****************************************************************************/
/*  SFLASH                                                                    */
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************************* FUNCTIONS ********************************/
/* ****************************************************************************/



/***************************************************************************//**
 * Initializes the flash access software module.
 *
 * @param  genericFlashEnvData    - Flash access environment data.
 * @param  flashEnvDataSze        - Environment data size.
 * @param  flashCbFuns            - Flash callback functions.
 * @param  genericFlashCbCtxData  - Flash callback context data pointer.
 * @param  genericFlashImpCfgData - Flash access configuration data.
 *
 * @return      flash_eErrorCode_NoError                    - Initialization was successful,
 *              flash_eErrorCode_ParameterError             - Parameter set was invalid
 *
 ******************************************************************************/
flash_ErrorCode_t flash_Initialization(flashIf_pGenericEnvData_t genericFlashEnvData, flashIf_EnvDataSze_t flashEnvDataSze, flashIf_cpCallbackFunctions_t flashCbFuns, flashIf_pGenericCbCtxData_t  genericFlashCbCtxData, flashIf_cpGenericImpCfgData_t genericFlashImpCfgData);


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
flash_ErrorCode_t flash_ErasePage(flashIf_cpGenericEnvData_t genericFlashEnvData, flash_cpPassphrases_t passphrases, flash_cpAddress_t dstPtr, const flash_Length_t byteNum, flash_PageCount_t* pPageEraseCount);

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
flash_ErrorCode_t flash_Write(flashIf_cpGenericEnvData_t genericFlashEnvData, flash_cpPassphrases_t passphrases, flash_cpByteData_t srcPtr, flash_cpAddress_t dstPtr, const flash_Length_t byteNum);

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
flash_ErrorCode_t flash_SetProtectionAccess(flashIf_cpGenericEnvData_t genericFlashEnvData, flash_cpPassphrase_t passphrase, flash_ProtectionLevel_t protAccessLevel, flash_AccessMode_t areaSelect, flash_LockBits_t lockProt, flash_LockBits_t lockProtFine);



/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/


/***************************************************************************//**
 * @brief Struct for flash access implementation specific configuration data.
 ******************************************************************************/
struct flashImp_sCfgData
{
  flashIf_Version_t                           Version;             /**< @brief Config data struct version */ 
  
  flash_ExitStrategyOnFatalIPError_t          ExitStrategySelect;  /**< @brief Selected exit strategy in case of fatal ip error. */
};

typedef struct flashImp_sCfgData    flashImp_CfgData_t;   /* Typedef of flashImp_sCfgData. */
typedef        flashImp_CfgData_t*  flashImp_pCfgData_t;  /* Typedef of pointer to flashImp_sCfgData. */
typedef const  flashImp_CfgData_t*  flashImp_cpCfgData_t; /* Typedef of pointer to flashImp_sCfgData. */
#endif /* FLASH_ACCESS_H_ */
