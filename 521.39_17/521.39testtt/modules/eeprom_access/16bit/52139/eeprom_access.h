/***************************************************************************//**
 * @file        eeprom_access.c
 *
 * @creator     POE
 * @created     27.09.2019
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief       This module offers functions to write/erase/verify data in the 
 *              EE.
 *
 ***************************************************************************
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
 * @purpose
 *
 *
 * $Id: eeprom_access.h 9632 2022-01-28 12:19:41Z poe $
 *
 * $Revision: 9632 $
 *
 ******************************************************************************/

#ifndef EEPROM_ACCESS_H_
#define EEPROM_ACCESS_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
typedef bool_t               eeprom_Bool_t;     /**< Typedef for ee bool status. */

typedef uint16_t *           eeprom_pAddress_t; /**< Typedef for address parameter. */
typedef uint16_t             eeprom_Length_t;   /**< Typedef for length parameter. */

typedef uint8_t *            eeprom_pData_t;    /**< Typedef for pointer to byte data. */
typedef const uint8_t *      eeprom_cpData_t;   /**< Typedef for const pointer to byte data. */    

typedef uint8_t              eeprom_LockBits_t; /**< Typedef for lock bit mask. */

typedef uint16_t             eeprom_Password_t; /**< Typedef for eeprom password. */

#define EE_ERASED_VALUE      (0x0000u)

#define EE_PASSWORD          (0xA509u)          /**< @brief Password used to access EEPROM_CTRL registers and statemachine count. */

#define EE_ALIGNMENT         ( 4u )             /**< @brief Erase works on double words only, therefore programming as well. */
#define EE_ALIGNMENT_MASK    ( 3u )             /**< @brief 32bit alignment mask. */

#define EE_SIZE              (0x0080u)          /**< @brief Byte-size of EE NVM. */

#define EE_START             (0x0400u)          /**< @brief EE start address. */
#define EE_END               (0x047Fu)          /**< @brief EE end address. */ //ee starts @0x400 and ends @0x47f

/* ****************************************************************************/
/* ********************************* FUNCTIONS ********************************/
/* ****************************************************************************/

/***************************************************************************//**
 * Writes bytes from srcPtrByte to dstPtr.
 *
 * @param[in] pass       - Configured passphrases for current action.
 * @param[in] srcPtrByte - Pointer to source data (must not be in destination area)
 * @param[in] dstPtr     - Destination pointer for current action (must be page aligned).
 * @param[in] byteNum    - Number of bytes length of target area (must be page aligned).
 * @param[in] lock_l     - Lower lockbit mask.
 * @param[in] lock_u     - Upper lockbit mask.      
 *
 * @return TRUE          - Write operation was successful,
 *         FALSE         - Else.
 *         
 ******************************************************************************/
eeprom_Bool_t eeprom_Write(eeprom_Password_t pass, eeprom_cpData_t srcPtrByte, eeprom_pAddress_t dstPtr, eeprom_Length_t byteNum, eeprom_LockBits_t lock_l, eeprom_LockBits_t lock_u);


/***************************************************************************//**
 * Overwrites data area containing given number of bytes starting from dptr as start address
 * with zeroes.
 *
 * @param[in] pass       - Configured passphrases for current action.
 * @param[in] dstPtr     - Destination pointer for current action (must be page aligned).
 * @param[in] byteNum    - Number of bytes length of target area (must be page aligned).
 * @param[in] lock_l     - Lower lockbit mask.
 * @param[in] lock_u     - Upper lockbit mask.                   
 *
 * @return TRUE          - Erase operation was successful,
 *         FALSE         - Else.
 *
 ******************************************************************************/
eeprom_Bool_t eeprom_Erase(eeprom_Password_t pass, eeprom_pAddress_t dstPtr, eeprom_Length_t byteNum, eeprom_LockBits_t lock_l, eeprom_LockBits_t lock_u);


/***************************************************************************//**
 * Configure a protection access for target area.
 *
 * @param[in] pass          -  EE password.
 * @param[in] lock_l        -  Lower lockbit mask.
 * @param[in] lock_u        -  Upper lockbit mask.
 * @param[in] lock_l_freeze -  Lower freeze lockbit mask.
 * @param[in] lock_u_freeze -  Upper freeze lockbit mask.
 *
 * @return TRUE             -  Operation was successful,
 *         FALSE            -  Else.                                                    
 *                                                  
 ******************************************************************************/
void eeprom_SetLock(eeprom_Password_t pass, eeprom_LockBits_t lock_l, eeprom_LockBits_t lock_u,eeprom_LockBits_t lock_l_freeze,eeprom_LockBits_t lock_u_freeze);

/***************************************************************************//**
 * Returns the EE protection access status.
 *
 * @param[out] lock_l    - Pointer to lower lock mask result value.
 * @param[out] lock_u    - Pointer to upper lock mask result value.
 *
 * @return none.
 *
 ******************************************************************************/
void eeprom_GetLock(eeprom_LockBits_t *lock_l, eeprom_LockBits_t *lock_u);


/***************************************************************************//**
 * Resets the EE IP.
 *
 * @param  none.
 *
 * @return TRUE  - IP reset successfully,
 *         FALSE - Else.
 *
 ******************************************************************************/
eeprom_Bool_t eeprom_ResetIP(void);


/***************************************************************************//**
 * Checks the EE IP operational state.
 *
 * @param  none.
 *
 * @return TRUE  - IP is in a usable state,
 *         FALSE - Else.
 *
 ******************************************************************************/
eeprom_Bool_t eeprom_CheckIPState(void);

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* EEPROM_ACCESS_H_ */
