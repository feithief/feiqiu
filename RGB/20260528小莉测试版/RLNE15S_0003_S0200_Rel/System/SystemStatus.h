/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemStatus.h
 * Project:		RLNE15S
 * Description:	This header file defines the system status management module.
 *              It declares data structures, macros, and functions to monitor
 *              and manage real-time LIN signals, system errors, power status,
 *              and sleep transitions. It also includes functionality to track 
 *              the system's operational state and LIN signal updates.
 * Note(s):	    - The sleep management states are crucial for power optimization.
 *              - This module provides a unified view of system and LIN status.
 * Update Record:	
 * Version     	Date     	 Modified by       Changes Made
 * 00.01      2024.11.28      Genhua Yang      Initial version
 ************************************************************************
 */

#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

#include "Algorithm.h"
#include "SystemType.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/



#define  SYSTEM_ERROR_NONE        0x0u /*!< System has no errors >*/
#define  SYSTEM_ERROR_BAT_ERROR   0x1u  /*!< System input voltage is abnormal >*/
#define  SYSTEM_ERROR_LED_ERROR   0x2u  /*!< LED works abnormal >*/


/*!< This ENUM indicates power status >*/
typedef enum
{
	E_SYSTEM_SLEEP_AWAKE      = 0x1, /*!< System is awake >*/
	E_SYSTEM_SLEEP_PREPARE    = 0x2, /*!< System is preparing to enter sleep mode >*/
	E_SYSTEM_SLEEP_PROCESSING = 0x3, /*!< System sleep is processing, we do dimming >*/
	E_SYSTEM_SLEEP_SLEEP      = 0x4, /*!< Sleep dimming finished, system officially enters sleep mode >*/
} E_SYSTEM_SLEEP;


/*!< This ENUM indicates sleep type >*/
typedef enum
{
	E_SLEEP_TYPE_IDLE       = 0x1, /*!< LIN bus is IDLE >*/
	E_SLEEP_TYPE_COMMAND    = 0x2, /*!< System is preparing to enter sleep mode >*/
} E_SLEEP_TYPE;


/*!< This structure indicates current system status >*/
typedef struct
{
	E_SYSTEM_SLEEP e_SleepFlag;     /*!< System sleep status >*/
	uint8_t				 u8_ErrFlag;       /*!< System error status >*/
	uint8_t       u8_NewLinSignal;  /*!< Whether system gets new LIN signals or not >*/
} ST_SystemStatus;


/*!< This structure indicates LIN signals >*/
typedef struct {

    union {
        struct {
            uint16_t BcmRgbSlaveAdresse         : 15;  /*!< Group address and single address >*/
            uint16_t reserved      : 1;              /*!< Whether group address is used (reserved bit) >*/
        } Group_Adresse;
        uint16_t u16_id;                             /*!< 16-bit identifier for the signal >*/
        uint8_t u8_id[2];                            /*!< 8-bit identifiers for the signal (two 8-bit bytes) >*/
    } ST_LED_ID;

    uint8_t u8_CdcuSigEnableColorProtocol  : 1;  /*!< Enables the color protocol signal (1 bit) >*/
    uint8_t u8_CdcuSigEnableLed            : 1;  /*!< Enables the LED signal (1 bit) >*/
    uint8_t u8_CdcuSigEnableFading         : 1;  /*!< Enables fading effect signal (1 bit) >*/
    uint8_t u8_unused59                     : 5;  /*!< Unused bits, reserved for future use >*/

    uint8_t u8_BcmRgbDimmrampe             : 8;  /*!< Dimming ramp value (8 bits), controls the dimming speed >*/
    uint8_t u8_BcmRgbIntensitaet           : 8;  /*!< Intensity of the RGB LED (8 bits, 0-255 range) >*/

    union {
        uint8_t u8_BcmRgbRot;                  /*!< Red color signal (8 bits) >*/
        uint8_t u8_BcmRgbPreColorNumber;       /*!< Pre-color number used for status flags (8 bits) >*/
    };

    uint8_t u8_BcmRgbGruen                  : 8;  /*!< Green color signal (8 bits) >*/
    uint8_t u8_BcmRgbBlau                   : 8;  /*!< Blue color signal (8 bits) >*/

} ST_SLinSignals;



/*!< This variable shows system status. It's dangerous to make this external, only for smaller code size. */
extern ST_SystemStatus g_st_SysStatus;

/*!< This variable shows current LIN control signals. It's dangerous to make this external, only for smaller code size. */
extern ST_SLinSignals g_st_sysLin;


/**
* @brief 		Initialize the system status and set default values for LIN communication and error flags.
* @note 		This function resets LIN-related variables, clears error flags, and sets the system to awake state.
*               It ensures the system starts with a clean state and prepares for LIN communication.
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void SystemStatus_Init(void);

/**
* @brief 		Update the current system status by checking battery, LED, and temperature conditions.
* @note 		This function processes the battery status, performs LED open/short detection, and updates temperature readings.
*               It sets or clears the corresponding error flags based on the status of each component.
*               If a new LIN signal is detected while an LED error exists, the LED detection process is reset.
* @return		None.
* @retval		None.
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.12.12       Genhua Yang
**/
void SystemStatus_Update(void);



#endif
