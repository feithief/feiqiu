/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
* \file     bootrom.h
*
* \brief    BootROM low level access library
*
* \version  V1.0.2
* \date     07. Jul 2025
*
*/

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** VO           Vanessa Ongaro                                                **
** SL           Stefano Lissandron                                            **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2023-09-27, VO:   [ATVCOMETLL-125] Initial version                 **
** V1.0.1: 2024-03-25, SL:   [ATVCOMETLL-156] Added definitions of startup    **
**                           service routine usage                            **
** V1.0.2: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

#if !defined(_BOOTROM_H_)
#define _BOOTROM_H_


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/

#include "types.h"
#include "variants.h"


/*******************************************************************************
**                       Entry points into the BootROM                        **
*******************************************************************************/

#define addr_user_nvm_write                ( 0x1fb1 )
#define addr_user_nvm_erase                ( 0x1fb5 )
#define addr_user_nvm_verify               ( 0x1fb9 )
#define addr_user_nvm_ecc_check            ( 0x1fbd )
#define addr_user_nvm_ecc_addr_get         ( 0x1fc1 )


/*******************************************************************************
**                        Global Constant Declarations                        **
*******************************************************************************/

#define NVM_OPTIONS_NONE                   (0x00u)
#define NVM_OPTIONS_RETRY_MASK             (0x01u)


/*******************************************************************************
**                          Global Type Declarations                          **
*******************************************************************************/

/**
 * @brief Data structure for the error code of device operations.
 */
typedef enum
{
  /* Generic error code */
  GENERIC_E_OK                = 0x00u, /**< SUCCESS */
  GENERIC_E_NOT_OK            = 0x01u, /**< the requested operation failed */
  GENERIC_E_BUSY              = 0x02u, /**< the requested operation failed, because device is busy with other operation */
  GENERIC_E_CONFIG_FAILED     = 0x03u, /**< internal error code of device initialization */
  GENERIC_E_PROTECTED         = 0x04u, /**< the requested operation failed, due to protection */
  GENERIC_E_PARAM_INVALID     = 0x05u, /**< parameter validation failed */

  /* NVM error code */
  NVM_E_INTEGRITY_FAILED      = 0x06u, /**< NVM integrity failed */
  NVM_E_INTEGRITY_FAILED_ECC2 = 0x07u, /**< NVM integrity failed, due to ECC1 error */
  NVM_E_INTEGRITY_FAILED_ECC1 = 0x08u, /**< NVM integrity failed, due to ECC2 error */
  NVM_E_HARDWARE              = 0x09u, /**< NVM hardware failure */
  NVM_E_VIRGIN_PRODUCT        = 0x0Au, /**< internal error code of NVM initialization, product is virgin */
  NVM_E_VIRGIN_DEVICE         = 0x0Bu, /**< internal error code of NVM initialization, device is virgin */

  /* BSL error code*/
  BSL_E_UNSUPPORTED           = 0x0Cu, /**< BSL operation not supported */
  BSL_E_INTEGRITY_FAILED      = 0x0Du, /**< BSL frame integrity failed */

  /* Startup error code */
  SRV_E_EXEC_FAILED           = 0x0Eu, /**< service request execution failed */

  /* Internal */
  TRIM_E_MAGIC                = 0x0Fu, /**< internal error code of analog trimming */
  TRIM_E_ADDR                 = 0x10u  /**< internal error code of analog trimming */
} status_t;

/**
 * @brief Data structure for the erase operation scopes.
 */
typedef enum
{
  NVM_ERASE_PAGE     = 0x00u,     /**< page Erase */
  NVM_ERASE_SECTOR   = 0x01u,     /**< sector Erase */
  NVM_ERASE_MODULE   = 0x02u      /**< module Erase */
} erase_type_t;

/**
 * @brief The operations of interface management, either activate or deactivate the selected interface.
 */
typedef enum
{
  IF_DEACTIVATE     = 0x55u,    /**< deactivate SWD access */
  IF_ACTIVATE       = 0xFFu     /**< activate SWD access */
} interface_op_t;

/**
 * @brief interface selection, used by interface management operation.
 */
typedef enum
{
  SWD     = 0x01u,    /**< select target interface SWD */
  BSL     = 0x10u     /**< select target interface BSL */
} interface_t;


/**
 * @brief data structure of service request operation: 1000tp write.
 */
typedef struct
{
  uint32_t address;
  uint32_t length;
  uint8_t data[UC_1000TP_PAGE_SIZE];
} srv_1000tp_write_t;

/**
 * @brief data structure of service request operation: 1000tp erase.
 */
typedef struct
{
  uint32_t address;
} srv_1000tp_erase_t;

/**
 * @brief data structure of service request operation: interface management.
 */
typedef struct
{
  interface_t interface;
  interface_op_t interface_op;
  uint16_t reserved; /* padding */
} srv_ifmgnt_t;

/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/

/**
 * @defgroup USER_API_GROUP BootROM API
 * BootROM APIs support flash access and other low-level operations.
 *
 * @brief    These routines are exposed by the BootROM to the customer user mode software.
 * @{
 */

/**
 * This function erases flash in page-wise or in sector-wise.
 *
 * @param address      Address of the flash memory.
 * @param erase_type   Scope of erase operation. Refer to @ref erase_type_t.
 * - #NVM_ERASE_PAGE   Erase the selected page.
 * - #NVM_ERASE_SECTOR Erase the selected sector.
 * - #NVM_ERASE_MODULE Not supported in API.
 * @remark In an interrupt or multi-threaded environment, this function cannot be called in a re-entrant context.
 *
 * @return Function execution status. Refer to @ref status_t.
 */
#define user_nvm_erase                  ( (status_t(*)(uint32_t, erase_type_t))                                addr_user_nvm_erase        )

/**
 * This function writes data from the source to the specified flash address.
 *
 * @param address    Address of the flash memory.
 * @param data       Pointer to source data.
 * @param length     Number of bytes to write.
 * @param prog_flag  Optional program flag, set #NVM_OPTIONS_RETRY_MASK to enable retry operation, otherwise, set to #NVM_OPTIONS_NONE.
 *
 * Supported \a prog_flag:
 * - #NVM_OPTIONS_NONE       The default setting: In case of write failure, no retry.
 * - #NVM_OPTIONS_RETRY_MASK Enables retry write operation if the first write operation verification failed.
 *
 * @remark In an interrupt or multi-threaded environment, this function cannot be called in a re-entrant context.
 *
 * @return Function execution status. Refer to @ref status_t.
 */
#define user_nvm_write                  ( (status_t(*)(uint32_t, const uint8_t*, uint32_t, uint32_t))          addr_user_nvm_write        )

/**
 * This function verifies data in flash memory by comparing calculated checksum against the given checksum.
 *
 * @param address    Address of the flash memory.
 * @param length     Number of bytes to be verified.
 * @param checksum   Classic XOR 32-bit checksum.
 *
 * @remark In an interrupt or multi-threaded environment, this function cannot be called in a re-entrant context.
 *
 * @return Function execution status. Refer to @ref status_t.
 */
#define user_nvm_verify                 ( (status_t(*)(uint32_t, uint32_t, uint32_t))                          addr_user_nvm_verify       )

/**
 * This function reads flash memory and reports ECC error if there is.
 *
 * @param address    Address of the flash memory.
 * @param length     Number of bytes to read.
 *
 * @remark In an interrupt or multi-threaded environment, this function cannot be called in a re-entrant context.
 *
 * @return Function execution status. Refer to @ref status_t.
 */
#define user_nvm_ecc_check              ( (status_t(*)(uint32_t, uint32_t))                                    addr_user_nvm_ecc_check    )

/**
 * This function gets address of last ECC event.
 *
 * @param nvm_ecc_addr_ptr    Pointer to retrieved ECC address.
 *
 * @return Function execution status. Refer to @ref status_t.
 */
#define user_nvm_ecc_addr_get           ( (status_t(*)(uint32_t*))                                             addr_user_nvm_ecc_addr_get )

/** @} */

/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/

#ifdef __cplusplus
}
#endif


#endif /*!defined(_BOOTROM_H_)*/
