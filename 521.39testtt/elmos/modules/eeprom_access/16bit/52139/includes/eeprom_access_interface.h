/** @addtogroup eeprom_interface
 * @{ */
/***************************************************************************//**
 * @file		eeprom_interface.h
 *
 * @creator		poe
 * @created		15.05.2019
 *
 * @brief  		Interface and definitions for the bootloader ROM eeprom access functions. 
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
 * $Id: eeprom_access_interface.h 9267 2021-09-30 08:22:22Z poe $
 *
 * $Revision: 9267 $
 *
 ******************************************************************************/

#ifndef _EEPROM_INTERFACE_H_
#define _EEPROM_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"
#include "eeprom_access.h"
/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define EEPROM_INTERFACE_MODULE_API_VERSION     (0x0100u)     /**< @brief eeprom interface version */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/
typedef         uint16_t                         eepromIf_Version_t;            /**< @brief eeprom interface version */

struct          eepromIf_sInterfaceFunctions;                                     /**< Forward declaration of eeprom access interface functions. */
typedef struct  eepromIf_sInterfaceFunctions     eepromIf_InterfaceFunctions_t;   /**< Typedef for eepromIf_sInterfaceFunctions. */
typedef         eepromIf_InterfaceFunctions_t*   eepromIf_pInterfaceFunctions_t;  /**< Typedef of pointer to eepromIf_sInterfaceFunctions. */
typedef const   eepromIf_InterfaceFunctions_t*   eepromIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to eepromIf_sInterfaceFunctions. */
/* *****************************************************************************
 * Interface callback function types
 ******************************************************************************/
  
/***************************************************************************//**
 * Erases data area containing given number of bytes starting from dptr as start address.
 *
 * @param  pass            - Configured passphrases for current action.
 * @param  dstPtr          - Destination pointer for current action (must be page aligned).
 * @param  byteNum         - Number of bytes length of target area (must be page aligned).
 * @param  lock_l          - Lower lockbit mask.
 * @param  lock_u          - Upper lockbit mask.                   
 *
 * @return TRUE            - Erase operation was successful,
 *         FALSE           - else.
 *
 ******************************************************************************/
typedef eeprom_Bool_t (*eepromIf_MemErase_t)( eeprom_Password_t pass,
                                              eeprom_pAddress_t dstPtr, 
                                              eeprom_Length_t byteNum, 
                                              eeprom_LockBits_t lock_l, 
                                              eeprom_LockBits_t lock_u);

/***************************************************************************//**
 * Writes bytes from srcPtrByte to dstPtr.
 *
 * @param  pass            - Configured passphrases for current action.
 * @param  srcPtrByte      - Pointer to source data (must not be in destination area)
 * @param  dstPtr          - Destination pointer for current action (must be page aligned).
 * @param  byteNum         - Number of bytes length of target area (must be page aligned).
 * @param  lock_l          - Lower lockbit mask.
 * @param  lock_u          - Upper lockbit mask.      
 *
 * @return TRUE            - Write operation was successful,
 *         FALSE           - else.
 *         
 ******************************************************************************/
typedef eeprom_Bool_t (*eepromIf_MemWrite_t)( eeprom_Password_t pass, 
                                              eeprom_cpData_t srcPtrByte, 
                                              eeprom_pAddress_t dstPtr, 
                                              eeprom_Length_t byteNum, 
                                              eeprom_LockBits_t lock_l, 
                                              eeprom_LockBits_t lock_u);




/***************************************************************************//**
 * Configure a protection access for target protection area.
 *
 * @param  pass                 -  EE password.
 * @param  lock_l               -  Lower lockbit mask.
 * @param  lock_u               -  Upper lockbit mask.
 * @param  lock_l_freeze        -  Lower freeze lockbit mask.
 * @param  lock_u_freeze        -  Upper freeze lockbit mask.
 *
 * @return none                                                  
                                                   
 * @image html eeprom_SetProtectionAccess.png "Activity chart: EEPROM Set Protection Access function"
 * @image latex eeprom_SetProtectionAccess.png "Activity chart: EEPROM Set Protection access function"
 ******************************************************************************/
typedef void (*eepromIf_MemSetProtectionAccess_t)( eeprom_Password_t pass, 
                                                   eeprom_LockBits_t lock_l, 
                                                   eeprom_LockBits_t lock_u,
                                                   eeprom_LockBits_t lock_l_freeze,
                                                   eeprom_LockBits_t lock_u_freeze);

 
/***************************************************************************//**
 * @brief md nvm access interface functions
 ******************************************************************************/
struct eepromIf_sInterfaceFunctions
{
    eepromIf_Version_t                     InterfaceVersion;                   /**< @copydoc eepromIf_Version_t */
  
    eepromIf_MemWrite_t                    MemoryWrite;                        /**< @copydoc eepromIf_MemWrite_t */                           
    eepromIf_MemErase_t                    MemoryErase;                        /**< @copydoc eepromIf_MemErase_t */ 
    eepromIf_MemSetProtectionAccess_t      MemoryProtect;                      /**< @copydoc eepromIf_MemSetProtectionAccess_t */
};



/* *****************************************************************************
 * eeprom access callback function types
 ******************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/



#ifdef BUILD_ROM
extern const eepromIf_InterfaceFunctions_t eepromIf_InterfaceFunctions;
#endif

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* _EEPROM_INTERFACE_H_ */
/** @} eeprom_interface */
