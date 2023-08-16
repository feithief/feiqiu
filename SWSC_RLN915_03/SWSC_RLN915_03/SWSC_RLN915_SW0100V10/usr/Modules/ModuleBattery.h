
#ifndef _MODULEBATTERY_H_
#define _MODULEBATTERY_H_

#include "SystemType.h"

#define D_UNDERVOLTAGE_OFF  (6200u) /*ADC value 6V*/
/*cold start, 7V--8.5V*/
#define D_UNDERVOLTAGE_ON   (7200u) /*ADC value 7V*/

/*HVIO max capacity of voltage is 45V, we do 40.*/
#define D_OVERVOLTAGE_ON    (17200u)/*ADC value 19V*/
#define D_OVERVOLTAGE_OFF   (18200u)/*ADC value 20V*/

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
//extern uint8_t VSenableFlag;
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
uint16_t moduleBatGetVoltage(void);
//uint32_t moduleVSGetVoltage(void);
/************VSenable************/
uint16_t moduleVSGetVoltage(void);
/************IO5enable************/
//uint32_t moduleIO5GetVoltage(void);
/***********************************/
uint8_t DeltaVSGetVoltage(void);

/** @} */

#endif
