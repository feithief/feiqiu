
#ifndef _SYSTEMSTATUS_H_
#define _SYSTEMSTATUS_H_

#include "Algorithm.h"
#include "SystemType.h"

/** @file
*
* @defgroup SYSTEM_STATUS system status management.
* @{
*
* @ingroup RGB_SYSTEM
*
* @brief This module manage the real-time LIN signal, system error and power status.
*
* @details This module manage the real-time LIN signal, system error and power status.
*/

/**@brief This define will make RGB module wakes up and light white constantly.*/
//#define DLIN_COMM_NO_SLEEP_WHITE_LIGHT


/**@brief This ENUM indicates which system error occurred.*/
typedef enum SystemError
{
  ESystemErrorNone      = 0x0,  /**<System has no errors.*/
  ESystemErrorBatError  = 0x1,  /**<System input voltage is abnormal.*/
  ESystemErrorLedError  = 0x2,  /**<LED works abnormal.*/
}ESystemError;

typedef enum SystemFirstdiming
{
  FritstDimingStop      = 0x0,  /**<System has no errors.*/
  FristDimingstart  = 0x1,  /**<System input voltage is abnormal.*/
  FristDiminging  = 0x2,  /**<LED works abnormal.*/
}ESystemFirstdiming;

/**@brief This ENUM indicates power status.*/
typedef enum SystemSleep
{

  ESystemSleepAwake      = 0x1, /**<System is awake.*/
  ESystemSleepPrepare    = 0x2, /**<System is preparing to enter sleep mode.*/
  ESystemSleepProcessing = 0x3, /**<System sleep is processing, we do dimming.*/
  ESystemSleepSleep      = 0x4, /**<Sleep dimming finished, system officially enters sleep mode..*/
} ESystemSleep;

/**@brief This structure indicates current system status.*/
typedef struct SystemStatus
{
  ESystemSleep sleepFlag;     /**<System sleep status.*/
  ESystemError errFlag;       /**<System error status.*/
	ESystemError lasterrFlag;
  bool_t       newLinSignal;  /**<Whether system gets new LIN signals or not.*/
  uint8_t TimeBaseUpdate : 1;	/* rampe??basis??speed?????? */
  uint8_t IntensityUpdate : 1;	/* ????��???? */
  uint8_t ColorUpdate : 1;
  uint8_t reserved2 : 4;
  uint8_t lastSleepFlag;
} SSystemStatus;

typedef struct {
	uint8_t SWrite_Dummy                   : 5;
  uint8_t SWrite_Len                     : 3;
  uint8_t SWrite_MemAddress;
  uint8_t SWrite_Data_B1;
  uint8_t SWrite_Data_B2;
  uint8_t SWrite_Data_B3;
  uint8_t SWrite_Data_B4;
  uint8_t SWrite_Data_B5;
  uint8_t SWrite_CRC;
}SWrite;	

typedef struct {
	uint8_t  BCM_RGB_Color_u         		: 7 ;   /**<u Signal.*/
  uint8_t  BCM_RGB_Color_v      		: 7 ;   /**<v Signal.*/
  uint8_t  BCM_RGB_Dimmrampe            : 6 ;   /**<Dimming ramp.*/
  uint8_t  BCM_RGB_Sonderfunktion       : 4	;   /**<Mode flag, normal mode or special functions.*/
  uint8_t  BCM_RGB_Intensitaet          : 7	;   /**<Intensity.*/
  uint8_t  BCM_RGB_GlobalSync_little         : 1 ;
}ST_RGB;	

/**@brief This structure indicates LIN signals.*/
typedef struct {
  uint16_t BCM_RGB_Slave_Adresse         : 15;  /**<group address and single address.*/
  uint8_t  unused58_0      : 1;   /**<Whether group address is using.*/


  ST_RGB RGB;

  uint8_t  BCM_RGB_GlobalSync         : 1 ;
  uint8_t  unused59_0                   : 7 ;   /**<Reserved.*/
	
  SWrite Swrite;

  uint8_t  FristLin                      ;
  uint8_t  IsColorAudi                 	 ;
  uint8_t  BCM_RGB_Rot                   ;   /**<R Signal.*/
  uint8_t  BCM_RGB_Gruen                 ;   /**<G Signal.*/
  uint8_t  BCM_RGB_Blau                  ;   /**<B Signal.*/
} SLinSignals;

typedef struct LINREQESTNVM{
	uint8_t Requst_NAD;
	uint8_t Requst_Adress;
	uint8_t Requst_Length;
	uint8_t Requst_data01;
	uint8_t Requst_data02;
	uint8_t Requst_data03;
	uint8_t Requst_data04;
	uint8_t Requst_data05;
} SLINREQESTNVM;

/**@brief This structure indicates LIN Request 39frame signals.*/
typedef struct LINANSNVM{
	uint8_t Anwser_NAD;
	uint8_t Anwser_Adress;
	uint8_t Anwser_Length;
	uint8_t Anwser_data01;
	uint8_t Anwser_data02;
	uint8_t Anwser_data03;
	uint8_t Anwser_data04;
	uint8_t Anwser_data05;
} SLINANSNVM;


/**@brief This variant shows system status.
 * It's dangerous to make this external, only for smaller code size.*/
extern SSystemStatus sysStatus;
/**@brief This variant shows current LIN control signals.
 * It's dangerous to make this external, only for smaller code size.*/
extern SLinSignals sysLin;

extern SLinSignals sysLin_Stack;

/**
*@details   Initialize status management.
*
*@retval    None.
*/
void systemStatusInit(void);

/**
*@details   Update system status(Battery, temperature, LIN, system status...).
*
*@retval    None.
*/
void systemStatusUpdate(void);

/** @} */

#endif
