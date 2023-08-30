/***************************************************************************//**
 * @file		eeprom_access.h
 *
 * @creator	    POE
 * @created	    27.09.2019
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief  		This module provides functions to gives the user write and controlling access to EEPROM.
 *
 * @purpose
 *
 * @details
 * The module provides functions to erase, write and test EEPROM cells. 
 * Furthermore it gives the user the possibility to reset the IP, lock memroy areas and
 * read which memory areas are locked.
 *
 * $Id: eeprom_access.h 9267 2021-09-30 08:22:22Z poe $
 *
 * $Revision: 9267 $
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
#define EE_ERASED_VALUE      (0x0000u)

#define EE_PASSWORD          (0xA509u)    // password used to access EEPROM_CTRL registers and statemachine count.

#define EE_ALIGNMENT         ( 4u )         // erase works on double words only, therefore programming as well. 
#define EE_ALIGNMENT_MASK    ( 3u )        // 

#define EE_SIZE              (0x0080u)

#define EE_START             (0x0400u)
#define EE_END               (0x047Fu) //ee starts @0x400 and ends @0x47f

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/
typedef bool_t               eeprom_Bool_t;

typedef uint16_t *           eeprom_pAddress_t;
typedef uint16_t             eeprom_Length_t;

typedef uint8_t *            eeprom_pData_t;
typedef const uint8_t *      eeprom_cpData_t;

typedef uint8_t              eeprom_LockBits_t;

typedef uint16_t             eeprom_Password_t;

/* ****************************************************************************/
/* ********************************* FUNCTIONS ********************************/
/* ****************************************************************************/

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
eeprom_Bool_t eeprom_Write(eeprom_Password_t pass, eeprom_cpData_t srcPtrByte, eeprom_pAddress_t dstPtr, eeprom_Length_t byteNum, eeprom_LockBits_t lock_l, eeprom_LockBits_t lock_u);


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
eeprom_Bool_t eeprom_Erase(eeprom_Password_t pass, eeprom_pAddress_t dstPtr, eeprom_Length_t byteNum, eeprom_LockBits_t lock_l, eeprom_LockBits_t lock_u);


/***************************************************************************//**
 * Configure a protection access for target area.
 *
 * @param  pass                 -  EE password.
 * @param  lock_l               -  Lower lockbit mask.
 * @param  lock_u               -  Upper lockbit mask.
 * @param  lock_l_freeze        -  Lower freeze lockbit mask.
 * @param  lock_u_freeze        -  Upper freeze lockbit mask.
 *
 * @return TRUE                 -  Operation was successful,
 *         FALSE                -  else                                                    
                                                   
 ******************************************************************************/
void eeprom_SetLock(eeprom_Password_t pass, eeprom_LockBits_t lock_l, eeprom_LockBits_t lock_u,eeprom_LockBits_t lock_l_freeze,eeprom_LockBits_t lock_u_freeze);

/***************************************************************************//**
 * @brief Get current values of Eeproms LOCK_L and and LOCK_U registers.
 *
 * @param[out] lock_l     Pointer to lower lock bits variable
 * @param[out] lock_u     Pointer to upper lock bits variable
 *
 * @details Copies current values of Eeproms LOCK_L and LOCK_U register
 * to destinations lock_l and lock_u.
 *
 ******************************************************************************/
void eeprom_GetLock(eeprom_LockBits_t *lock_l, eeprom_LockBits_t *lock_u);


/***************************************************************************//**
 * @brief Reset EEPROM IP
 *
 * @return Returns always TRUE after execution.
 *
 * @details Resets EEPROM IP when Status register BUSY flag is set.
 * Also disables all Eeprom IRQs and clears IRQ Status flags.
 *
 ******************************************************************************/
eeprom_Bool_t eeprom_ResetIP(void);


/***************************************************************************//**
 * @brief Check if Eeprom IP is busy.
 *
 * @return TRUE if Eeprom IP is NOT in busy state. FALSE when not in busy state.
 *
 * @details Checks Eeprom status registers Busy-flag, if Eeprom is
 * in busy state. If Eeprom is in busy state, reset IP and return FALSE.
 * Otherwise return TRUE.
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
