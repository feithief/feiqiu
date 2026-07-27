
#ifndef _MODULEBATTERY_H_
#define _MODULEBATTERY_H_
#include "el_types.h"
//#include "SystemType.h"
//typedef unsigned long  uint32_t;
/** @file
*
* @defgroup BASIC_ADC_MODULE basic ADC converting.
* @{
*
* @ingroup BASIC_MODULES
*
* @brief This module converts ADC value of one channel.
*
* @details This module converts ADC value of one channel. LED adc converting is processing
*          in interrupt.
*/

/**@brief This ENUM indicates input voltage status.*/
typedef enum BatStatus
{
  EBatStatusInitial      = 0x0, /**<input voltage initial status.*/
  EBatStatusNormal       = 0x1, /**<input voltage normal.*/
  EBatStatusUnderVoltage = 0x2,  /**<input voltage too low.*/
  EBatStatusOverVoltage  = 0x4,  /**<input voltage too High.*/
} EBatStatus;

/**
*@details   Initialize input voltage detection.
*
*@retval    None.
*/
void moduleBatInit(void);

/**
*@details   Update current input voltage and status.
*
*@retval    current input voltage status.
*/
EBatStatus moduleBatUpdateStatus(void);

/**
*@details   get current input power voltage.
*
*@retval    input voltage.
*/
uint32_t moduleBatGetVoltage(void);

/** @} */

#endif
