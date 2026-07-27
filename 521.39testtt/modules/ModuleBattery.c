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
//#include "SystemStatus.h"


/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */
/*cold start, 6V--5.8V*/
#define D_UNDERVOLTAGE_OFF  (6000) /*ADC value 5V*/
/*cold start, 7V--8.5V*/
#define D_UNDERVOLTAGE_ON   (6500) /*ADC value 5.5V*/

/*HVIO max capacity of voltage is 45V, we do 40.*/
#define D_OVERVOLTAGE_ON    (28000)/*ADC value 18.5V*/
#define D_OVERVOLTAGE_OFF   (28500)/*ADC value 18V*/

/*D_MAX_FILTER_COUNTER is used to prevent some flags from fluctuating*/
#define D_MAX_FILTER_COUNTER  5


/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/**@brief This variant shows current input voltage status.*/
static EBatStatus currentBatStatus = EBatStatusInitial;

/**@brief This variant shows current input voltage value.*/
static uint32_t batteryVoltage = 12000;

/*statusCounter is used to prevent some flags from fluctuating*/
static int8_t statusCounter;


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
static uint32_t __moduleBatGetVoltage(void)
{
  uint32_t rawVal = 0;
  rawVal=ReturnADCBuffer(ADC_CH_VS);
  rawVal=rawVal*1000;  
  rawVal=rawVal/112;
  return rawVal;

}

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
uint32_t moduleBatGetVoltage(void)
{
  return batteryVoltage;
}

