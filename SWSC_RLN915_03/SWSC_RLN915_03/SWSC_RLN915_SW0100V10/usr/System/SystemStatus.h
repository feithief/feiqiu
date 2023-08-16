
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
typedef struct LinSignals{
  uint16_t CtrlList_LIN10         : 16;  /**<group address and single address.*/
  uint8_t  Brightness_LIN10       : 8;   /**<Intensity.*/
  uint8_t  RGB_R_LIN10            : 8;   /**<R Signal.*/
  uint8_t  RGB_G_LIN10            : 8;   /**<G Signal.*/
  uint8_t  RGB_B_LIN10            : 8;   /**<B Signal.*/
  uint8_t  RampTime_LIN10         : 8;   /**<Dimming ramp.*/
  uint8_t  VehicleTypeInfo        : 8;   /**<Reserved.*/
} SLinSignals  ;


/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/**@brief This variant shows system status.
 * It's dangerous to make this external, only for smaller code size.*/
extern volatile SSystemStatus sysStatus;
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
