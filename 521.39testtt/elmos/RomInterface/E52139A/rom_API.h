/***************************************************************************//**
 * @file		rom_API.h
 *
 * @creator		RPY
 * @created		20.04.2015
 * @sdfv      Elmos Flow - Grade 2
 *
 * @brief       ROM API access
 *
 * @purpose     -Defines interface to ROM API
 *              -Provides function to get pointer to interface of
 *               specified interface Id.
 *              -Definition of ROM version.
 *
 * $Id: rom_API.h 7120 2020-02-13 17:31:43Z poe $
 *
 * $Revision: 7120 $
 *
 ******************************************************************************/
/* ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor AG provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer:
 * Elmos Semiconductor AG shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor AG. To the extend permitted by law 
 * Elmos Semiconductor AG hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 *******************************************************************************/
 
#ifndef ROM_API_H_
#define ROM_API_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"


/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define ROMIF_MAIN_INTERFACE_VERSION    (0x0100u)

//TODO: define
#define ROMIF_MAIN_INTERFACE_LOCATION   (0x2010u)

/***************************************************************************//**
 * Interface identifiers
 ******************************************************************************/

#define ROMIF_MAIN                  (  0u)

#define ROMIF_API_VIC               (  1u)
// ... more APIs to come...

#define ROMIF_BOOTLOADER            ( 30u)

#define ROMIF_LIN_BUS               ( 35u)
#define ROMIF_LIN_PROTO             ( 36u)
#define ROMIF_LIN_TRANS             ( 37u)

#define ROMIF_LIN_LOOKUP_FIXED      ( 39u)
#define ROMIF_LIN_LOOKUP_SEQUENTIAL ( 40u)

#define ROMIF_MTP_ACCESS            (100u)
#define ROMIF_FLASH_ACCESS          (102u)

#define ROMIF_INVALID               0xFFFFu

/***************************************************************************//**
 * Data structure encoding ROM version
 ******************************************************************************/
typedef struct 
{
  uint8_t  Major;
  uint8_t  Minor;
  uint16_t Build;
} rom_Version_t;

/***************************************************************************//**
 * Main ROM application related entry point
 ******************************************************************************/

/***************************************************************************//**
 * Data type for the ROM interface version
 ******************************************************************************/
typedef uint16_t romIf_Version_t;
/***************************************************************************//**
 * Data type for the ROM interface Id
 ******************************************************************************/
typedef uint16_t romIf_InterfaceId_t;
/***************************************************************************//**
 * Data type for the ROM interface length
 ******************************************************************************/
typedef uint16_t romIf_Length_t;
/***************************************************************************//**
 * Pointer type for the ROM Generic interface
 ******************************************************************************/
typedef        void*  romIf_pGenericInterface_t;
/***************************************************************************//**
 * Constant pointer type for the ROM generic interface
 ******************************************************************************/
typedef const  void*  romIf_cpGenericInterface_t;

/***************************************************************************//**
 * Get interface function wrapper.
 *
 * @param[in]  id interface id
 * @param[out] interfaceFunctions interface functions
 * @param[out] envSize            size of the enviormant for the iterface
 *
 * @return TRUE for success, otherwise FALSE
 ******************************************************************************/
typedef bool_t (*romIf_Interface_Get_t) (romIf_InterfaceId_t id, romIf_cpGenericInterface_t *interfaceFunctions, romIf_Length_t *envSize);


/***************************************************************************//**
 * Main ROM application related entry point type declaration
 ******************************************************************************/
struct romIf_MainInterfaceFunctions
{
    romIf_Version_t           InterfaceVersion;
    
    romIf_Interface_Get_t     Interface_Get;    
    
    // ... more calls ?
};

typedef struct romIf_MainInterfaceFunctions     romIf_MainInterfaceFunctions_t;
typedef        romIf_MainInterfaceFunctions_t*  romIf_pMainInterfaceFunctions_t;
typedef const  romIf_MainInterfaceFunctions_t*  romIf_cpMainInterfaceFunctions_t;

/***************************************************************************//**
 * Main ROM application related entry point
 ******************************************************************************/
#ifndef BUILD_ROM

/***************************************************************************//**
 * General pointer to the interface structure. 
 ******************************************************************************/
extern const romIf_cpMainInterfaceFunctions_t romIf_MainInterface;

/***************************************************************************//**
 * Get interface function wrapper.
 *
 * @param[in]  id interface id
 * @param[out] interfaceFunctions interface functions
 * @param[out] envSize            size of the enviormant for the iterface
 *
 * @return TRUE for success, otherwise FALSE
 ******************************************************************************/
bool_t romIf_Interface_Get(romIf_InterfaceId_t id, romIf_cpGenericInterface_t *interfaceFunctions, romIf_Length_t *envSize);

#endif

#endif /* ROM_API_H_ */




