/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleBattery.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains current input voltage and status detection.
  *Others:      None.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "ModuleBattery.h"
#include "ModuleAdc.h"
#include "SystemStatus.h"
#include "ModuleFlash.h"
#include "ModuleClock.h"
#include "SystemType.h"


/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */
/*cold start, 6V--5.8V*/


/*D_MAX_FILTER_COUNTER is used to prevent some flags from fluctuating*/
#define D_MAX_FILTER_COUNTER  5

#define DELTA_UPDATE_CYCLE_PERIOD  8

/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/**@brief This variant shows current input voltage status.*/
static EBatStatus currentBatStatus = EBatStatusInitial;

/**@brief This variant shows current input voltage value.*/
static uint16_t batteryVoltage = 12000;
static uint16_t VSbatteryVoltage = 11000;
/*statusCounter is used to prevent some flags from fluctuating*/
static int8_t statusCounter;
static uint8_t DeltaVSVoltage = 0;
static tick_t updateTick = 0;
static uint16_t VSbatteryVoltage_A = 11000;




/* ============================================================================ */
/* ------------------------ Application Definition ---------------------------- */
/* ============================================================================ */


/**
*@details   Initialize input voltage detection.
*
*@retval    None.
*/
void moduleBatInit(void)
{
  statusCounter = D_MAX_FILTER_COUNTER;
  moduleBatUpdateStatus();
}

/**
*@details   Get current filtered input voltage value.
*
*@retval    Filtered ADC value.
*/

static uint16_t __moduleBatGetVoltage(void)
{
  uint32_t rawVal = 0;
  rawVal = moduleADCValue(ADC_CH_VS);
  rawVal = rawVal * 2;
  rawVal = adc_coeff[ADC_CH_VS].coefficient * rawVal + adc_coeff[ADC_CH_VS].offset;
  
  rawVal = rawVal >> MEASURE_GAIN_POS;
  return (uint16_t)rawVal + 1000;
}

/**
*@details   Get current filtered input voltage value.
*
*@retval    Filtered ADCLEDA value.
*/

/**
*@details   Get current filtered input voltage value and change current input status or not.
*
*@retval    current input voltage status.
*/
EBatStatus moduleBatUpdateStatus(void)
{
  uint32_t filterLimitMax = D_UNDERVOLTAGE_ON, filterLimitMin = D_OVERVOLTAGE_ON;
 
  EBatStatus temp;

  switch(currentBatStatus)
  {
    case EBatStatusUnderVoltage:
      filterLimitMin = D_UNDERVOLTAGE_ON;
      filterLimitMax = D_OVERVOLTAGE_OFF;
      break;
    case EBatStatusOverVoltage:
      filterLimitMin = D_UNDERVOLTAGE_OFF;
      filterLimitMax = D_OVERVOLTAGE_ON;
      break;
    case EBatStatusInitial:
    case EBatStatusNormal:
      filterLimitMin = D_UNDERVOLTAGE_OFF;
      filterLimitMax = D_OVERVOLTAGE_OFF;
      break;
  }

  batteryVoltage = __moduleBatGetVoltage();
  VSbatteryVoltage = batteryVoltage-1000;
  if (batteryVoltage < filterLimitMin)
    temp = EBatStatusUnderVoltage;
  else if (batteryVoltage > filterLimitMax)
    temp = EBatStatusOverVoltage;
  else
    temp = EBatStatusNormal;

  if (temp != currentBatStatus)
    statusCounter--;
  else
    statusCounter = D_MAX_FILTER_COUNTER;

  if (currentBatStatus == EBatStatusInitial)
    statusCounter = 0;

  if (statusCounter <= 0)
  {
    currentBatStatus = temp;
    statusCounter = D_MAX_FILTER_COUNTER;
  }

  return currentBatStatus;

}

/**
*@details   get current input power voltage.
*
*@retval    input voltage.
*/
uint16_t moduleBatGetVoltage(void)
{
  return batteryVoltage;
}
/**
*@details   get current input VS voltage.
*
*@retval    input vs voltage.
*/
uint16_t moduleVSGetVoltage(void)
{
  return VSbatteryVoltage;
}
/**
*@details   get delta VS voltage from 10ms ago input VS voltage to current input VS voltage.
*
*@retval    delta vs voltage.
*/
// for testing lost frame 2021.06.16

uint8_t DeltaVSGetVoltage(void)
{
  if (DTickAfter(moduleClockGet(), updateTick + DELTA_UPDATE_CYCLE_PERIOD))
  {
    updateTick = moduleClockGet();
    VSbatteryVoltage_A = moduleVSGetVoltage();
  }
  if(VSbatteryVoltage < VSbatteryVoltage_A)
  {
    DeltaVSVoltage = VSbatteryVoltage_A - VSbatteryVoltage;
  }
  else
  {
    DeltaVSVoltage = VSbatteryVoltage - VSbatteryVoltage_A;
  } 
  return DeltaVSVoltage;
}


