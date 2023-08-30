/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleLed.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains LED initialize, power on, set PWM and LED open/short detection.
  *Others:      None.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
                     V01 Modified PWM_PERIOD to 32000. This should enhance the color mixing capacity.
                     Modified by Leo, Song. 2018.1.12
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "ModuleLed.h"
#include "ModuleAdc.h"
#include "ModuleFlash.h"
#include "SystemStatus.h"
#include "mod_pwm.h"
//#include "def_adc.h"
//#include "def_gpio.h"
#include "el_types.h"
#include "ModuleWatchdog.h"
#include "ModuleTemperature.h"
//#define DMIN_WORK_VOLTAGE 7000 //mcu under this voltage will not be checked open short test.
#define DABNORMNAL_VOLTAGE_THREASHOLD 400//550
/*if RED LED voltage exceed this value, we believe this led is open circuit.*/
//although the output value reaches to 11V when led opens, the value is better to be set as 6.8v, otherwise open detect will fail when vcc<7V
#define DMAX_RED_VOLTAGE_LIMIT   6999

/*if RED LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_RED_VOLTAGE_LIMIT   400

/*if GREEN LED voltage exceed this value, we believe this led is open circuit.*/
#define DMAX_GREEN_VOLTAGE_LIMIT   6999

/*if GREEN LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_GREEN_VOLTAGE_LIMIT   400

/*if BLUE LED voltage exceed this value, we believe this led is open circuit.*/
#define DMAX_BLUE_VOLTAGE_LIMIT   6999

/*if BLUE LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_BLUE_VOLTAGE_LIMIT   400

/*The const is aimed to provent status flag from fluctuating*/
//#define DLED_DETECTION_FILTER_SIZE            10
#define DLED_DETECTION_STATUS_UPDATE_COUNTER  100  //jiaminlu

/*Register related*/
#define PWM_ChnlR                   PWM_CH0 // channel definition for particular glow plug
#define PWM_ChnlB                   PWM_CH1
#define PWM_ChnlG                   PWM_CH2
#define PWM_GP4                   PWM_CH3

/*BIT set in every channel*/
#define DHVDIFF_RED_DONE    0x0001u
#define DHVDIFF_GREEN_DONE  0x0004u
#define DHVDIFF_BLUE_DONE   0x0002u

/* detection flag indicates which channel has detected */
//static volatile uint16_t detection = 0;
static uint16_t detectionMask = 0;

/* ============================================================================ */
/* ------------------------ MODULE GLOBALE VARIABLES -------------------------- */
/* ============================================================================ */

/*Those variant shows current red, green. blue led status.*/
static ELedStatus redStatus = ELedStatusInit, greenStatus = ELedStatusInit, blueStatus = ELedStatusInit;

/*Those variant shows filtered current red, green. blue voltage(not in mV or any other unit).*/
static volatile int32_t FilteredRedVoltage = 2300, FilteredGreenVoltage = 2400, FilteredBlueVoltage = 2400;


/**@brief This variant shows current PWM ratio.*/
static SPWMParams  currentPWM = {0,0,0};

/**@brief This variant shows current LED over write status.
 *It's dangerous to make this external, only for smaller code size.*/
volatile ELedControl ledControlFlag;

extern volatile uint16_t adc_DMAArea[ADC_DMA_AREA_SIZE];

/**
*@details   Initialize LED functions.
*
*@note      This function must be invoked before any other LED functions.
*
*@retval    None.
*/
void moduleLedInit(void)
{
//  SYS_STATE_CONTROL_bit.pwm_clk_sel = 1;//48MHz
  PWM_PRESCALER0 = 2;
  PWM_PERIOD0    = PWM_PERIOD-1;
  PWM_START0     = 0u;
  PWM_STOP0      = 0;

  PWM_PRESCALER1 = 2;
  PWM_PERIOD1    = PWM_PERIOD-1;
  PWM_START1     = 0u;
  PWM_STOP1      = 0;

  PWM_PRESCALER2 = 2;
  PWM_PERIOD2    = PWM_PERIOD-1;
  PWM_START2     = 0u;
  PWM_STOP2      = 0;
  

  PWM_PRESCALER3 = 2;
  PWM_PERIOD3    = PWM_PERIOD-1;
  PWM_START3     = 0u;
  PWM_STOP3      = 0;

  /* Set pwm currents to 30mA. */
  PWM_DRIVER     = 0x5555;
  /* Enable all pwm ch and ch drivers. */
  PWM_CONTROL    = 0x00FFu; 
  
  //PWM_CH_DISABLE(3);
  //PWM_CH_DRIVER_DISABLE(3);
}


/**
*@details   Do the update PWM pulse value job.
*
*@warning   Fist we do as we could to split R/G/B on and off in different time.
*           But it will cause some strange problem, and the wave is very ugly.
*           At last we find out that if we turn off R/G/B at the same time, the wave becomes
*           beautiful, and that strange problem disappeared.
*
*@param[in] pwm  R/G/B channel PWM value.
*
*@retval    None.
*/

static void __moduleLedSetPWM(SPWMParams *pwm)
{  
  PWM_STOP_SET(PWM_ChnlR, pwm->PWM_R);
  PWM_STOP_SET(PWM_ChnlB, pwm->PWM_B); //the PWM output has already involved compensation value
  PWM_STOP_SET(PWM_ChnlG, pwm->PWM_G);
  
  if(pwm->PWM_R <=1) PWM_ADC_TRIGGERDELAY_SET(PWM_ChnlR, 0);
  else PWM_ADC_TRIGGERDELAY_SET(PWM_ChnlR, pwm->PWM_R / 2);
  
  if(pwm->PWM_B <=1) PWM_ADC_TRIGGERDELAY_SET(PWM_ChnlB, 0);
  else PWM_ADC_TRIGGERDELAY_SET(PWM_ChnlB, pwm->PWM_B / 2);
  
  if(pwm->PWM_G <=1) PWM_ADC_TRIGGERDELAY_SET(PWM_ChnlG, 0);
  else PWM_ADC_TRIGGERDELAY_SET(PWM_ChnlG, pwm->PWM_G / 2);

  if (pwm->PWM_R > 480) //led voltage wave is delayed(60us) due to capacity between VCC and ROUT.
    detectionMask |= DHVDIFF_RED_DONE;//so the ADC sampling should be delayed 60us(=480 PWM CLOCK CYCLEs)
  else
    detectionMask &= ~DHVDIFF_RED_DONE;
  
  if (pwm->PWM_G > 480)
   detectionMask |= DHVDIFF_GREEN_DONE;
  else
    detectionMask &= ~DHVDIFF_GREEN_DONE;
  
  if (pwm->PWM_B > 480)
    detectionMask |= DHVDIFF_BLUE_DONE;
  else
    detectionMask &= ~DHVDIFF_BLUE_DONE;
}

#if 0
void moduleLedDetectionDone(ELedChannel channel)
{
  if (channel == ELedChannelRed)
    detection |= DHVDIFF_RED_DONE;//1
  
  if (channel == ELedChannelGreen)
    detection |= DHVDIFF_GREEN_DONE;//4
  
  if (channel == ELedChannelBlue)
    detection |= DHVDIFF_BLUE_DONE;//2
}
#endif

/**
*@details   Update R/G/B LED voltage and status.
*
*@retval    None.
*/
static int16_t statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;//int8_t
static int16_t statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
static int16_t statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;


#define Coef_LED2Vol (560) //475_560_645
#define Coef_VS2Vol (112) //108_112_116    P14

void moduleLedUpdateStatus(void)
{

  int32_t Voltage_mV;
  ELedStatus tmpRStatus=redStatus,tmpGStatus=greenStatus,tmpBStatus = blueStatus;
  
  
    if (detectionMask == 0)
    {
        return;
    }
  
    if ((detectionMask & DHVDIFF_RED_DONE)>0)
    {
        /* Get LEDR Vol */
        Voltage_mV = (int32_t)ReturnADCBuffer(ADC_CH_LED0) * (int32_t)1000 / (int32_t)Coef_LED2Vol;
        FilteredRedVoltage = (uint16_t)Voltage_mV;
        if(FilteredRedVoltage >= DMAX_RED_VOLTAGE_LIMIT ) 
        {
            tmpRStatus = ELedStatusOpen;
        } 
        else if (FilteredRedVoltage < DMIN_RED_VOLTAGE_LIMIT) 
        {
            tmpRStatus = ELedStatusShort;
        }
        else
        {
            tmpRStatus = ELedStatusNormal;
            redStatus = ELedStatusNormal;
            statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
        }

        if ((tmpRStatus & (ELedStatusOpen|ELedStatusShort)) && (statusRCounter > 0))
        {
            statusRCounter--;
        }
        else
        {
            redStatus = tmpRStatus;
            statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
        }
    }
    
    if ((detectionMask & DHVDIFF_GREEN_DONE)>0)
    {
        /* Get LEDG Vol */
        Voltage_mV = (int32_t)ReturnADCBuffer(ADC_CH_LED2) * (int32_t)1000 / (int32_t)Coef_LED2Vol;
        FilteredGreenVoltage = (uint16_t)Voltage_mV;
        if (FilteredGreenVoltage >= DMAX_GREEN_VOLTAGE_LIMIT) 
        {
            tmpGStatus = ELedStatusOpen;
        } 
        else if (FilteredGreenVoltage < DMIN_GREEN_VOLTAGE_LIMIT) 
        {
            tmpGStatus = ELedStatusShort;
        }
        else
        {
            tmpGStatus = ELedStatusNormal;
            greenStatus = ELedStatusNormal;
            statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
        }

        if ((tmpGStatus & (ELedStatusOpen|ELedStatusShort)) && (statusGCounter > 0))
        {
            statusGCounter--;
        }
        else
        {
            greenStatus = tmpGStatus;
            statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
        }
        
    }
    if ((detectionMask & DHVDIFF_BLUE_DONE)>0)
    {
        /* Get LEDB Vol */
        Voltage_mV = (int32_t)ReturnADCBuffer(ADC_CH_LED1) * (int32_t)1000 / (int32_t)Coef_LED2Vol;
        FilteredBlueVoltage = (uint16_t)Voltage_mV;
        if(FilteredBlueVoltage >= DMAX_BLUE_VOLTAGE_LIMIT)
        {
            tmpBStatus = ELedStatusOpen;
        } 
        else if (FilteredBlueVoltage < DMIN_BLUE_VOLTAGE_LIMIT) 
        {
            tmpBStatus = ELedStatusShort;
        }
        else 
        {
            tmpBStatus = ELedStatusNormal;
            blueStatus = ELedStatusNormal;
            statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
        }

        if ((tmpBStatus & (ELedStatusOpen|ELedStatusShort)) && (statusBCounter > 0))
        {
            statusBCounter--;
        }
        else
        {
            blueStatus = tmpBStatus;
            statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
        }
    }
    
  
    //System Indictor update
    if ( (redStatus|greenStatus|blueStatus) & (ELedStatusShort | ELedStatusOpen) )
    {
      sysStatus.errFlag |= ESystemErrorLedError;
    }
    else
    {
      sysStatus.errFlag &= ~ESystemErrorLedError;
    }
}

/**
*@details   Reset module's fault detection flags.
*
*@retval    none.
*/

void moduleLedResetDetection(void)
{
  sysStatus.errFlag &= ~ESystemErrorLedError;

  statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
  statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
  statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;

  if (redStatus & (ELedStatusOpen|ELedStatusShort))
    redStatus = ELedStatusInit;

  if (greenStatus & (ELedStatusOpen|ELedStatusShort))
    greenStatus = ELedStatusInit;

  if (blueStatus & (ELedStatusOpen|ELedStatusShort))
    blueStatus = ELedStatusInit;
  
}

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
void moduleLedUpdatePWM(uint16_t r, uint16_t g, uint16_t b)
{
  /*if in calibration mode, turn on red constant and only*/
  if (ledControlFlag == ELedControlRedOn)
  {
    r = (uint16_t)PWM_PERIOD/10*4;
    g = 0;
    b = 0;
  }
  /*if in calibration mode, turn on green constant and only*/
  if (ledControlFlag == ELedControlGreenOn)
  {
    r = 0;
    g = (uint16_t)PWM_PERIOD/10*4;
    b = 0;
  }
  /*if in calibration mode, turn on blue constant and only*/
  if (ledControlFlag == ELedControlBlueOn)
  {
    r = 0;
    g = 0;
    b = (uint16_t)PWM_PERIOD/10*4;
  }

  if (r > 0)
    r += savedConfig.RCapCompensate;

  if (g > 0)
    g += savedConfig.GCapCompensate;

  if (b > 0)
    b += savedConfig.BCapCompensate;

  if (ledControlFlag == ELedControlSaveCali)
    {
        ledControlFlag = ELedControlAllNormal;
        savedConfig.Cali_Volt[0] = FilteredRedVoltage;
        savedConfig.Cali_Volt[1] = FilteredGreenVoltage;
        savedConfig.Cali_Volt[2] = FilteredBlueVoltage;
        savedConfig.Cali_Temp = moduleTempMcuGet();
        ledControlFlag = ELedControlAllNormal;
        if (flashFlag == EFlashUpdateFree)
            flashFlag |= EFlashUpdateProcessing;
    }
  if ((currentPWM.PWM_R != r) ||
      (currentPWM.PWM_G != g) ||
      (currentPWM.PWM_B != b))
  {    
    if(r>=32000) r=31999;
    if(g>=32000) g=31999;
    if(b>=32000) b=31999;

    currentPWM.PWM_R = r;
    currentPWM.PWM_G = g;
    currentPWM.PWM_B = b;
    
    __moduleLedSetPWM(&currentPWM);
  }
}

/**
*@details   Power off LED.
*
*@retval    None.
*/
void moduleLedPowerOff(void)
{
  moduleLedUpdatePWM(0, 0, 0);
}

/**
*@details   re-initialize LED status and get ready to sleep.
*
*@retval    None.
*/


void moduleLedSleep(void)
{
  redStatus = ELedStatusInit;
  greenStatus = ELedStatusInit;
  blueStatus = ELedStatusInit;
}

/**
*@details   Get one of three led's status.
*
*@retval    LED status.
*/
ELedStatus moduleLedGetStatus(ELedChannel channel)
{
  ELedStatus status = ELedStatusInit;
  
  
  switch (channel)
  {
    case ELedChannelRed:
      status = redStatus;
      break;
    case ELedChannelGreen:
      status = greenStatus;
      break;
    case ELedChannelBlue:
      status = blueStatus;
      break;
  }

  return status;
}

/**
*@details   Get one of three led's voltage.
*
*@retval    LED's voltage.
*/
int16_t moduleLedGetVoltage(ELedChannel channel)
{
  int16_t voltage = 2000;
  switch (channel)
  {
    case ELedChannelRed:
      voltage = FilteredRedVoltage;
      break;
    case ELedChannelGreen:
      voltage = FilteredGreenVoltage;
      break;
    case ELedChannelBlue:
      voltage = FilteredBlueVoltage;
      break;
  }

  return voltage;
}

/**
*@details   Get one of three led's PWM.
*
*@retval    LED's PWM.
*/
uint16_t moduleLedGetPWM(ELedChannel channel)
{
  uint16_t pwm = 0;
  switch (channel)
  {
    case ELedChannelRed:
      pwm = currentPWM.PWM_R;
      break;
    case ELedChannelGreen:
      pwm = currentPWM.PWM_G;
      break;
    case ELedChannelBlue:
      pwm = currentPWM.PWM_B;
      break;
  }

  return pwm;
}
