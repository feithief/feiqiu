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
#include "Algorithm.h"

#include "ModuleWatchdog.h"
#include "ModuleTemperature.h"
#include "ModuleBattery.h"
#include "SystemLinDiag.h"
//#include "ModuleClock.h"
#include "pwm_device.h"

#define DMAX_RED_VOLTAGE_LIMIT   4500

/*if RED LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_RED_VOLTAGE_LIMIT   700

/*if GREEN LED voltage exceed this value, we believe this led is open circuit.*/
#define DMAX_GREEN_VOLTAGE_LIMIT   4500

/*if GREEN LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_GREEN_VOLTAGE_LIMIT   700

/*if BLUE LED voltage exceed this value, we believe this led is open circuit.*/
#define DMAX_BLUE_VOLTAGE_LIMIT   4500

/*if BLUE LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_BLUE_VOLTAGE_LIMIT   700

/*The const is aimed to provent status flag from fluctuating*/
#define DLED_DETECTION_STATUS_UPDATE_COUNTER  15


#define PWM_PRESCALER             2       // PWM clock 24 MHz / (2+1) -> counter running at 8MHz

/*BIT set in every channel*/

#define DHVDIFF_RED_DONE    0x01u
#define DHVDIFF_GREEN_DONE  0x04u
#define DHVDIFF_BLUE_DONE   0x02u
/* detection flag indicates which channel has detected */
static volatile uint8_t ADC_Detection_Done = 0;
static volatile uint8_t Detection_EN = 0;
//uint8_t SLEEPStatus = 0;
//static tick_t updateTick = 0;


/* ============================================================================ */
/* ------------------------ MODULE GLOBALE VARIABLES -------------------------- */
/* ============================================================================ */

/*Those variant shows current red, green. blue led status.*/
static ELedStatus redStatus = ELedStatusInit, greenStatus = ELedStatusInit, blueStatus = ELedStatusInit;

/*Those variant shows filtered current red, green. blue voltage(not in mV or any other unit).*/
static uint16_t FilteredRedVoltage = 2500;
static uint16_t FilteredGreenVoltage = 2500;
static uint16_t FilteredBlueVoltage = 3000;

/**@brief This variant shows current PWM ratio.*/
static SPWMParams currentPWM = {0,0,0};

/**@brief This variant shows current LED over write status.
 *It's dangerous to make this external, only for smaller code size.*/
volatile ELedControl ledControlFlag;
/**
*@details   Initialize LED functions.
*
*@note      This function must be invoked before any other LED functions.
*
*@retval    None.
*/
void moduleLedInit(void)
{
    /* Init LED current and PWM settings */
    (void)PWM_SetLedCurrent();
    PWM_Init(PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX);
    /* PWM 搬到2ms中间，镜像对称。 在PWM周期起始时开启定时器(仅一次)，2ms处定时器中断内，触发采样adc */
    PWM_RegisterPWMReloadIRQ(Adc_LED_trigger_delay);
    
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
static int16_t statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;//int8_t
static int16_t statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
static int16_t statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
//uint8_t trigger_counter = 0;
static void __moduleLedSetPWM(SPWMParams *pwm)
{  
  PWM_SetRGBValue(0,pwm->PWM_R,pwm->PWM_G,pwm->PWM_B);


/*********************************************************/         
/***********(moduleBatGetVoltage()<11500)***VS<10.5******/     //2021.02.05
  if((moduleBatGetVoltage()<11500) || ((int16_t)moduleTempMcuGet()<(int16_t)(-400)) || (DeltaVSGetVoltage() > 100)) //-150 represent -15 degree, measured by MCU, actual temp is -15 degree
  {
    Detection_EN &= ~DHVDIFF_RED_DONE;
    Detection_EN &= ~DHVDIFF_GREEN_DONE;
    Detection_EN &= ~DHVDIFF_BLUE_DONE;
    statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;    // rain 2021.01.25
    statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
    statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
    return;
  }
  else
  {
    if (pwm->PWM_R > 600)//111)     //420        //raising up time is 45us// rain 2021.02.04 for hardware attribute and ADC Timing
      Detection_EN |= DHVDIFF_RED_DONE;
    else
      Detection_EN &= ~DHVDIFF_RED_DONE;
    
    if (pwm->PWM_G > 600)//315)                  // rain 2021.02.04 for hardware attribute and ADC Timing
      Detection_EN |= DHVDIFF_GREEN_DONE;
    else
      Detection_EN &= ~DHVDIFF_GREEN_DONE;
    
    if (pwm->PWM_B > 600)//198)                  // rain 2021.02.04 for hardware attribute and ADC Timing
      Detection_EN |= DHVDIFF_BLUE_DONE;
    else
      Detection_EN &= ~DHVDIFF_BLUE_DONE;
  }

}

void moduleLedDetectionDone(ELedChannel channel)
{
  if (channel == ELedChannelRed)
    ADC_Detection_Done |= DHVDIFF_RED_DONE;//1
  
  if (channel == ELedChannelGreen)
    ADC_Detection_Done |= DHVDIFF_GREEN_DONE;//4
  
  if (channel == ELedChannelBlue)
    ADC_Detection_Done |= DHVDIFF_BLUE_DONE;//2
}

/**
*@details   Update R/G/B LED voltage and status.
*
*@retval    None.
*/
static int16_t moduleTempGetMCU()
{
  int32_t rawVal;
  rawVal=(uint32_t)moduleADCValue(ADC_CH_VTEMP);

  if(rawVal>=4090) rawVal=4090;
  if(rawVal<=10) rawVal=10;
  
  rawVal = rawVal * 2500 >>12; //4096;
  if(rawVal >= 1550)  //rain 1550
  {
    rawVal = (rawVal - 1550)*10/33;
    rawVal = 25 - rawVal;
  }
  else
  {
    rawVal = (1550 - rawVal)*10/33;
    rawVal = 25 + rawVal;
  }
  return (int16_t)rawVal;
}

/**
*@details   Get current filtered input RED value.
*
*@retval    Filtered ADC value.
*/
static uint16_t __moduleRedGetVoltage(void)
{
  uint32_t rawVal = 0;
  rawVal = moduleADCValue(ADC_CH_Red);
  rawVal = rawVal * 2;
  rawVal = adc_coeff[ADC_CH_Red].coefficient * rawVal + adc_coeff[ADC_CH_Red].offset;
  
  rawVal = rawVal >> MEASURE_GAIN_POS;
  return (uint16_t)rawVal;
}
/**
*@details   Get current filtered input GREEN value.
*
*@retval    Filtered ADC value.
*/

static uint16_t __moduleGreenGetVoltage(void)
{
  uint32_t rawVal = 0;
  rawVal = moduleADCValue(ADC_CH_Green);
  rawVal = rawVal * 2;
  rawVal = adc_coeff[ADC_CH_Green].coefficient * rawVal + adc_coeff[ADC_CH_Green].offset;
  
  rawVal = rawVal >> MEASURE_GAIN_POS;
  return (uint16_t)rawVal;
}

/**
*@details   Get current filtered input BLUE value.
*
*@retval    Filtered ADC value.
*/
static uint16_t __moduleBlueGetVoltage(void)
{
  uint32_t rawVal = 0;
  rawVal = moduleADCValue(ADC_CH_Blue);
  rawVal = rawVal * 2;
  rawVal = adc_coeff[ADC_CH_Blue].coefficient * rawVal + adc_coeff[ADC_CH_Blue].offset;
  
  rawVal = rawVal >> MEASURE_GAIN_POS;
  return (uint16_t)rawVal;
}


void moduleLedUpdateStatus(void)
{
  static uint16_t redVoltage = 2440, greenVoltage = 3500, blueVoltage = 3500;

  uint8_t ADC_Detection_Done_Flag = 0;
  ADC_Detection_Done_Flag = ADC_Detection_Done;

  if (Detection_EN & ADC_Detection_Done_Flag & DHVDIFF_RED_DONE) 
  {
    redVoltage = __moduleRedGetVoltage();
    FilteredRedVoltage = (FilteredRedVoltage*3 + redVoltage)>>2;
  }
  if (Detection_EN & ADC_Detection_Done_Flag & DHVDIFF_GREEN_DONE)
  {
    greenVoltage = __moduleGreenGetVoltage();
    FilteredGreenVoltage = (FilteredGreenVoltage*3 + greenVoltage)>>2;
  }
  if (Detection_EN & ADC_Detection_Done_Flag & DHVDIFF_BLUE_DONE)
  {
    blueVoltage = __moduleBlueGetVoltage();
    FilteredBlueVoltage = (FilteredBlueVoltage*3 + blueVoltage)>>2;
  }
  /*we only measure current R/G/B LED that has been turned on.*/
  /*LED may have malfunction at the same time, so we measure all first, then we update status.*/
  if (((ADC_Detection_Done_Flag & Detection_EN)== Detection_EN) && (ADC_Detection_Done_Flag != 0))
  {
    ELedStatus tmpRStatus=redStatus,tmpGStatus=greenStatus,tmpBStatus = blueStatus;
    ADC_Detection_Done = 0;

    //red detect       
    if (Detection_EN & ADC_Detection_Done_Flag & DHVDIFF_RED_DONE)
    {
          if(FilteredRedVoltage > DMAX_RED_VOLTAGE_LIMIT)
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
          }
    }

    //green detect
    if (Detection_EN & ADC_Detection_Done_Flag & DHVDIFF_GREEN_DONE)
    {     
          if(FilteredGreenVoltage  > DMAX_GREEN_VOLTAGE_LIMIT )
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
          }
      
    }

    //blue detect
    if (Detection_EN & ADC_Detection_Done_Flag & DHVDIFF_BLUE_DONE)
    {
          if (FilteredBlueVoltage  > DMAX_BLUE_VOLTAGE_LIMIT)
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
          }
    }
    
  #if 1
/******************************/
    if ((tmpRStatus & (ELedStatusOpen|ELedStatusShort)) && (statusRCounter > 0))
    {
      statusRCounter--;
    }
    else
    {
      redStatus = tmpRStatus;
      statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
    }
/******************************/
    if ((tmpGStatus & (ELedStatusOpen|ELedStatusShort)) && (statusGCounter > 0))
    {
      statusGCounter--;
    }
    else
    {
      greenStatus = tmpGStatus;
      statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
    }
/******************************/
    if ((tmpBStatus & (ELedStatusOpen|ELedStatusShort)) && (statusBCounter > 0))
    {
      statusBCounter--;
    }
    else
    {
      blueStatus = tmpBStatus;
      statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
    }
/******************************/
    //System Indictor update
    if ( ((redStatus|greenStatus|blueStatus) & (ELedStatusShort | ELedStatusOpen)) >0)
    {
      sysStatus.errFlag |= ESystemErrorLedError;
      //sysStatus.errFlag &= ~ESystemErrorLedError;

    }
    else
    {
      sysStatus.errFlag &= ~ESystemErrorLedError;
    }
    
    
#endif
  }  //if (((ADC_Detection_Done_Flag & Detection_EN)== Detection_EN) && (ADC_Detection_Done_Flag != 0))
 
}



/**
*@details   Reset module's fault detection flags.
*
*@retval    none.
*/

void moduleLedResetDetection(void)
{
  sysStatus.errFlag &= ~ESystemErrorLedError;
  
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
//  if (r > 0)
//    r += savedConfig.RCapCompensate;
//
//  if (g > 0)
//    g += savedConfig.GCapCompensate;
//
//  if (b > 0)
//    b += savedConfig.BCapCompensate;

  /**********************电源电压降额****************************/
//    uint32_t batteryVoltage = 12000;
//    static uint16_t PowerVoltage_Derate = 40000;
//    static uint16_t VS_buffer[4] = {12000,12000,12000,12000};
//    static uint8_t count = 0;
//    VS_buffer[count] = moduleBatGetVoltage();
//    if(count < 4)
//    {
//      count++;
//    }
//    else
//    {
//      count = 0;
//    }
//    batteryVoltage = (uint32_t)VS_buffer[0];
//    batteryVoltage += (uint32_t)VS_buffer[1];
//    batteryVoltage += (uint32_t)VS_buffer[2];
//    batteryVoltage += (uint32_t)VS_buffer[3];
//    batteryVoltage >>= 2;
//    if(batteryVoltage >= 16000u)
//    {
//    PowerVoltage_Derate = (4000u - (batteryVoltage - 16000u)) * 10u;
//    }
//    else
//    {
//    PowerVoltage_Derate = 40000;
//    }
//    r = (uint32_t)r * (uint32_t)PowerVoltage_Derate / (uint32_t)40000;
//    g = (uint32_t)g * (uint32_t)PowerVoltage_Derate / (uint32_t)40000;
//    b = (uint32_t)b * (uint32_t)PowerVoltage_Derate / (uint32_t)40000;//End
/**********************电源电压降额****************************/
    if ((currentPWM.PWM_R != r) ||
        (currentPWM.PWM_G != g) ||
        (currentPWM.PWM_B != b))
    {    
      if(r>=PWM_VALUE_MAX) r=PWM_VALUE_MAX-1;
      if(g>=PWM_VALUE_MAX) g=PWM_VALUE_MAX-1;
      if(b>=PWM_VALUE_MAX) b=PWM_VALUE_MAX-1;
  
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
  //SLEEPStatus = TRUE;

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

