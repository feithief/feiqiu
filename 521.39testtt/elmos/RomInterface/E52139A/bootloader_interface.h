/***************************************************************************//**
 * @file     bootloader_interface.h
 *
 * @creator   RPY
 * @created   21.04.2015
 *
 * @brief     This module describes the bootloader interface's API functions.
 *
 * @purpose
 *
 * This module describes to bootloader's ROM API interface and the provided
 * functions to interact with the bootloader's configuration data.
 *
 * $Id: $
 *
 * $Revision: $
 *
 ******************************************************************************/

#ifndef BOOTLOADER_INTERFACE_H_
#define BOOTLOADER_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"
#include "bootloader_UserConfig.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define BOOTLOADER_INTERFACE_VERSION           (0x0102u)
#define BLDIF_WRITE_PASSWORD                   (0xA509u)
/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

typedef uint16_t     bldIf_Version_t;         /**< @brief Typedef for bootloader interface version number.*/
typedef uint16_t     bldIf_WritePassword_t;   /**< @brief Typedef for bootloader config ee password. */
typedef void*        bldIf_reserved_t;        /* Typedef for reserved data. */

typedef enum 
{
  bldIf_RESTART_APPLICATION       = 0u,    /**<< Restart device (software reset), and go to application. (option code 0) (this shoud be the default restart code) */
  
  bldIf_RESTART_APPLICATION_OPT_0 = bldIf_RESTART_APPLICATION, 
  bldIf_RESTART_APPLICATION_OPT_1 = 1u,    /**<< Restart application, and pass corresponding SW restart code */
  bldIf_RESTART_APPLICATION_OPT_2 = 2u,    /**<< Restart application, and pass corresponding SW restart code */
  bldIf_RESTART_APPLICATION_OPT_3 = 3u,    /**<< Restart application, and pass corresponding SW restart code */
  bldIf_RESTART_APPLICATION_OPT_4 = 4u,    /**<< Restart application, and pass corresponding SW restart code */
  bldIf_RESTART_APPLICATION_OPT_5 = 5u,    /**<< Restart application, and pass corresponding SW restart code */

  bldIf_RESTART_PERM_BOOTLOADER   = 6u,    /**<< Restart device (software reset), and force bootloader operation, stay in BL until idle timeout or until power off. */
  bldIf_RESTART_BOOTLOADER        = 7u,    /**<< Restart device (software reset), and start bootloader as it would occured due to POR. */

  bldIf_RESTART_CODE_COUNT        = 8u     /**<< number of available restart codes. */
  
} bldIf_RestartCode_t;


/* *****************************************************************************
 * Interface callback function types
 ******************************************************************************/



/***************************************************************************//**
 * Copys the variable configuration
 *
 * @param[in] pConf - Location where the configuration should be stored.
 *
 * @return    TRUE  - The bootloader config was successfully copied,
 *            FALSE - Else.
 *
 ******************************************************************************/
typedef bool_t (*bldIf_ReadConfig_t) (bldConf_pConfigData_t pConf);

/***************************************************************************//**
 * Writes a new configuration into memory.
 *
 * @param[in] pass    - EEPROM write password.
 * @param[in] cpConf  - Configuration that should be stored.
 * @param[in] lock    - New protection state of configuration data.
 *
 * @return TRUE       - the bootloader config was successfully copied,
 *         FALSE      - Else.
 *
 ******************************************************************************/
typedef bool_t (*bldIf_WriteConfig_t) (bldIf_WritePassword_t pass,bldConf_cpConfigData_t cpConf,bldConf_Lock_t lock);

/***************************************************************************//**
 * Reads the device serial number.
 *
 * @param[in] pSerial - Memory location where the serial number should be stored.
 *
 * @return TRUE       - At least a part of the serial number has been read,
 *         FALSE      - Else.
 *
 ******************************************************************************/
typedef bool_t (*bldIf_ReadDeviceSerial_t) (bldConf_pDeviceSerial_t pSerial);

/***************************************************************************//**
 * Reads the static configuration.
 *
 * @param[in] pConf - Memory location where the static configuration should be stored.
 *
 * @return    TRUE  - If the system has access to the stored configuration data,
 *            FALSE - Else.
 *
 ******************************************************************************/
typedef bool_t (*bldIf_ReadStaticConfig_t) (bldConf_pStaticConfigData_t pConf);

/***************************************************************************//**
 * Reads the Custom Static Configuration.
 *
 * @param[in] pConf - Memory location where the Custom Static Configuration should be stored
 *
 * @return    TRUE  - If the system has access to the stored configuration data.
 *            FALSE - Else.
 *
 ******************************************************************************/
typedef bool_t (*bldIf_ReadCustomStaticConfig_t) (bldConf_pStaticCustomConfigData_t pConf);

/***************************************************************************//**
 * Restarts the device.
 *
 * @param[in] code  - Restart code.
 *
 * @return    none.
 *
 * @details
 * If the restart code is out of range, it will be set to the default one (bldIf_RESTART_APPLICATION).
 * Before the software reset is triggered, all "software reset flags" are cleared and the new flags are set
 * based on the code variable.
 ******************************************************************************/
typedef void (*bldIf_RestartDevice_t) (bldIf_RestartCode_t code);


/***************************************************************************//**
 * Bootloader application layer interface function pointer
 ******************************************************************************/
struct bldIf_sBootloaderFunctions
{
    bldIf_Version_t                     InterfaceVersion;                     /**< @brief Interface version number. */
 
    bldIf_ReadConfig_t                  ReadConfig;                           /**< @brief Read bootloader runtime configuration. */       
    bldIf_WriteConfig_t                 WriteConfig;                          /**< @brief Write bootloader runtime configuration. */
    
    bldIf_reserved_t                    _reserved0;                           /**< @brief Reserved. */
    bldIf_reserved_t                    _reserved1;                           /**< @brief Reserved. */

    bldIf_ReadDeviceSerial_t            ReadDeviceSerial;                     /**< @brief Read device serial from bootloader runtime configuration. */

    bldIf_ReadStaticConfig_t            ReadStaticConfig;                     /**< @brief Read bootloader static config configuration data. */
    bldIf_ReadCustomStaticConfig_t      ReadCustomStaticConfig;               /**< @brief Read bootloader custom static config configuration data. */
    
    bldIf_RestartDevice_t               RestartDevice;                        /**< @brief Trigger a device software reset. */
};
  
typedef struct bldIf_sBootloaderFunctions    bldIf_BootloaderFunctions_t;     /**< Typedef for bldIf_sBootloaderFunctions. */
typedef        bldIf_BootloaderFunctions_t*  bldIf_pBootloaderFunctions_t;    /**< Typedef of pointer for bldIf_BootloaderFunctions_t. */
typedef const  bldIf_BootloaderFunctions_t*  bldIf_cpBootloaderFunctions_t;   /**< Typedef of const pointer for bldIf_BootloaderFunctions_t. */


/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

#define BLDIF_ENVIRONMENT_DATA_SIZE    (0u)  // currently no env data needed. 

#ifdef BUILD_ROM
extern const bldIf_BootloaderFunctions_t bldIf_InterfaceFunctions;
#endif

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* BOOTLOADER_INTERFACE_H_ */
