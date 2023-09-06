
#ifndef _SYSTEMSTATUS_H_
#define _SYSTEMSTATUS_H_

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "Algorithm.h"
#include "SystemType.h"

/**@brief This define will make RGB module wakes up and light white constantly.*/
//#define DLIN_COMM_NO_SLEEP_WHITE_LIGHT

/* ============================================================================ */
/* ----------------------- STRUCTS, ENUMS AND TYPEDEFS ------------------------ */
/* ============================================================================ */
/**This ENUM indicates which system error occurred.*/
typedef enum SystemError
{
  ESystemErrorNone      = 0x0,  /**<System has no errors.*/
  ESystemErrorBatError  = 0x1,  /**<System input voltage is abnormal.*/
  ESystemErrorLedError  = 0x2,  /**<LED works abnormal.*/
}ESystemError;

/**This ENUM indicates power status.*/
typedef enum SystemSleep
{
  ESystemSleepAwake      = 0x1, /**<System is awake.*/
  ESystemSleepPrepare    = 0x2, /**<System is preparing to enter sleep mode.*/
  ESystemSleepProcessing = 0x3, /**<System sleep is processing, we do dimming.*/
  ESystemSleepSleep      = 0x4, /**<Sleep dimming finished, system officially enters sleep mode..*/
} ESystemSleep;

/**This structure indicates current system status.*/
typedef struct SystemStatus
{
  ESystemSleep sleepFlag;     /**<System sleep status.*/
  ESystemError errFlag;       /**<System error status.*/
  bool_t       newLinSignal;  /**<Whether system gets new LIN signals or not.*/
} SSystemStatus;

/*This structure indicates LIN signals.*/


//36
typedef struct LinSignals{
  uint16_t BCM_RGB_Slave_Adresse         : 15;  /**<group address and single address.*/
  uint8_t  BCM_RGB_Adressierungsart      : 1;   /**<Whether group address is using.*/

  uint8_t  BCM_RGB_Rot                   : 8;   /**<R Signal.*/
  uint8_t  BCM_RGB_Gruen                 : 8;   /**<G Signal.*/
  uint8_t  BCM_RGB_Blau                  : 8;   /**<B Signal.*/

  uint8_t  BCM_RGB_Intensitaet           : 7;   /**<Intensity.*/
  uint8_t  BCM_RGB_Normierung_Intensitaet: 1;   /**<Reserved.*/

  uint8_t  BCM_RGB_Dimmrampe             : 4;   /**<Dimming ramp.*/
  uint8_t  unused52_0                    : 1;   /**<Reserved.*/
  uint8_t  BCM_RGB_Sonderfunktion        : 3;   /**<Mode flag, normal mode or special functions.*/

  uint8_t  BCM_RGB_Bewertung_Dimmzeit    : 1;   /**<Dimming attributes, relative or absolute.*/
  uint8_t  BCM_RGB_Gueltigkeit           : 1;   /**<Flag that indicates if LIN signal is valid.*/
  uint8_t  BCM_RGB_Dimmzeit_Zeitbasis    : 1;   /**<Flag that indicates the base dimming period.*/
  uint8_t  unused59_0                    : 5;   /**<Reserved.*/
} SLinSignals  ;

/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/**@brief This variant shows system status.
 * It's dangerous to make this external, only for smaller code size.*/
extern SSystemStatus sysStatus;
/**@brief This variant shows current LIN control signals.
 * It's dangerous to make this external, only for smaller code size.*/
extern SLinSignals sysLin;


/* ============================================================================ */
/* ------------------------ Application Definition ---------------------------- */
/* ============================================================================ */
/********************************************************************************
*@details   Initialize status management.
*
*@retval    None.
*******************************************************************************/
void systemStatusInit(void);

/********************************************************************************
*@details   Update system status(Battery, temperature, LIN, system status...).
*
*@retval    None.
*******************************************************************************/
void systemStatusUpdate(void);


#endif
