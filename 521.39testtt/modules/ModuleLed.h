#ifndef _MODULELED_H_
#define _MODULELED_H_

#include "SystemType.h"
//#include "Algorithm.h"
/** @file
*
* @defgroup RGB_LED_MODULE RGB led management
* @{
*
* @ingroup BASIC_MODULES
*
* @brief This module manage LED PWM pulse and LED open short detection.
*
* @details This file contains LED initialize, power on, set PWM and LED open/short detection.
*/

/**@brief This macro defines PWM period counter.*/
#define PWM_PERIOD  32000

/**@brief This structure indicates current led(R/G/B) status.*/
typedef enum LedStatus
{
  ELedStatusInit   = 0x1, /**<LED status initialized.*/
  ELedStatusNormal = 0x2, /**<LED status normal.*/
  ELedStatusOpen   = 0x4, /**<LED open circuit detected.*/
  ELedStatusShort  = 0x8, /**<LED short circuit detected.*/
}ELedStatus;

/**@brief This ENUM shows overwrite status.This will be used when calibration is processing*/
typedef enum LedControl
{
  ELedControlAllNormal = 0x0, /**<LED color do not over write.*/
  ELedControlRedOn     = 0x1, /**<If status set to this, LED red will be on constantly.*/
  ELedControlGreenOn   = 0x2, /**<If status set to this, LED green will be on constantly.*/
  ELedControlBlueOn    = 0x3, /**<If status set to this, LED blue will be on constantly.*/
  ELedControlSaveCali  = 0x4,
}ELedControl;

/**@brief This variant shows current LED over write status.
 *We do not make APIs for this function only for smaller code size.*/
extern volatile ELedControl ledControlFlag;

/**
*@details   Initialize LED functions.
*
*@note      This function must be invoked before any other LED functions.
*
*@retval    None.
*/
void moduleLedInit(void);

/**
*@details   Update PWM pulse value.
*
*@param[in] r  red channel PWM value.
*
*@param[in] g  green channel PWM value.
*
*@param[in] b  blue channel PWM value.
*
*@retval    None.
*/
void moduleLedUpdatePWM(uint16_t r, uint16_t g, uint16_t b);

/**
*@details   Power off LED.
*
*@retval    None.
*/
void moduleLedPowerOff(void);

/**
*@details   Update R/G/B LED voltage and status.
*
*@retval    None.
*/
void moduleLedUpdateStatus(void);

/**
*@details   re-initialize LED status and get ready to sleep.
*
*@retval    None.
*/
void moduleLedSleep(void);

/**@brief We use this ENUM to inquiry Led status and voltages.*/
typedef enum LedChannel
{
  ELedChannelRed   = 0x2,
  ELedChannelGreen = 0x3,
  ELedChannelBlue  = 0x4,
}ELedChannel;

/**
*@details   Get one of three led's status.
*
*@retval    LED status.
*/
ELedStatus moduleLedGetStatus(ELedChannel channel);

/**
*@details   Get one of three led's voltage.
*
*@retval    LED's voltage.
*/
int16_t moduleLedGetVoltage(ELedChannel channel);

/**
*@details   Get one of three led's PWM.
*
*@retval    LED's PWM.
*/
uint16_t moduleLedGetPWM(ELedChannel channel);

/**
*@details   Reset module's fault detection flags.
*
*@retval    none.
*/
void moduleLedResetDetection(void);

//void moduleLedDetectionDone(ELedChannel channel);

#endif
