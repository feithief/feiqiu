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
#include "SystemControl.h"
#include "mod_pwm.h"
//#include "def_adc.h"
//#include "def_gpio.h"
#include "Algorithm.h"
#include "main.h"                
#include "ModuleWatchdog.h"
#include "SystemDerating.h"
#include "ModuleTemperature.h"
//#define DMIN_WORK_VOLTAGE 7000 //mcu under this voltage will not be checked open short test.
#define DABNORMNAL_VOLTAGE_THREASHOLD 400//550
/*if RED LED voltage exceed this value, we believe this led is open circuit.*/
//although the output value reaches to 11V when led opens, the value is better to be set as 6.8v, otherwise open detect will fail when vcc<7V
#define DMAX_RED_VOLTAGE_LIMIT   4500

/*if RED LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_RED_VOLTAGE_LIMIT   500

/*if GREEN LED voltage exceed this value, we believe this led is open circuit.*/
#define DMAX_GREEN_VOLTAGE_LIMIT   4500

/*if GREEN LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_GREEN_VOLTAGE_LIMIT   500    //ygh debug

/*if BLUE LED voltage exceed this value, we believe this led is open circuit.*/
#define DMAX_BLUE_VOLTAGE_LIMIT   4500

/*if BLUE LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_BLUE_VOLTAGE_LIMIT   500

/*if WHITE LED voltage exceed this value, we believe this led is open circuit.*/
#define DMAX_WHITE_VOLTAGE_LIMIT   4500

/*if WHITE LED voltage lower than this value, we believe this led is short circuit.*/
#define DMIN_WHITE_VOLTAGE_LIMIT   500

/*The const is aimed to provent status flag from fluctuating*/
//#define DLED_DETECTION_FILTER_SIZE            10
#define DLED_DETECTION_STATUS_UPDATE_COUNTER  500  //jiaminlu

/*Register related*/
#define PWM_ChnlW                   PWM_CH0
#define PWM_ChnlG                   PWM_CH1
#define PWM_ChnlB                   PWM_CH2
#define PWM_ChnlR                   PWM_CH3

/*BIT set in every channel*/
#define DHVDIFF_RED_DONE    0x0001u
#define DHVDIFF_GREEN_DONE  0x0004u
#define DHVDIFF_BLUE_DONE   0x0002u
#define DHVDIFF_WHITE_DONE  0x0008u

/* detection flag indicates which channel has detected */
//static volatile uint16_t detection = 0;
static uint16_t detectionMask = 0;

/* ============================================================================ */
/* ------------------------ MODULE GLOBALE VARIABLES -------------------------- */
/* ============================================================================ */

/*Those variant shows current red, green. blue led status.*/
static ELedStatus redStatus = ELedStatusInit, greenStatus = ELedStatusInit, blueStatus = ELedStatusInit, whiteStatus = ELedStatusInit;

/*Those variant shows filtered current red, green. blue voltage(not in mV or any other unit).*/
static volatile int32_t FilteredRedVoltage = 2300, FilteredGreenVoltage = 2400, FilteredBlueVoltage = 2400, FilteredWhiteVoltage = 2400;


/**@brief This variant shows current PWM ratio.*/
static SPWMParams  currentPWM = {0,0,0};
static uint16_t currentWhitePWM = 0;

/**@brief This variant shows current LED over write status.
 *It's dangerous to make this external, only for smaller code size.*/
volatile ELedControl ledControlFlag;


//#define DLED_DETECTION_STATUS_UPDATE_COUNTER  80//if 100 test time??1.1s so set it 90 ??1s
#define DLED_DETECTION_Quiltcheck_COUNTER     25
uint16_t Quiltcheck[4]= {0};
/**
*@details   Initialize LED functions.
*
*@note      This function must be invoked before any other LED functions.
*
*@retval    None.
*/
void moduleLedInit(void)
{
  /*{45CE83A7-839F-4ea4-8C52-91DBC6DEAB23}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_CODE_MODULE_DISABLED_IN_CW;

  //#if ((LDRV_MOD_CFG & LDRV_MOD_CFG_LD_ENABLE_Msk) == LDRV_MOD_CFG_LD_ENABLE_Msk)
    s8_returnCode = ERR_LOG_SUCCESS;

    LDRV->PWM_FREQ.reg = (uint32) LDRV_PWM_FREQ;

    LDRV->PWM_PHASE_CFG0.reg = (uint32) LDRV_PWM_PHASE_CFG0;
    LDRV->PWM_PHASE_CFG1.reg = (uint32) LDRV_PWM_PHASE_CFG1;
    LDRV->PWM_PHASE_CFG2.reg = (uint32) LDRV_PWM_PHASE_CFG2;
    LDRV->PWM_PHASE_CFG3.reg = (uint32) LDRV_PWM_PHASE_CFG3;

    LDRV->PWM_DC_CFG0.reg = (uint32) LDRV_PWM_DC_CFG0;
    LDRV->PWM_DC_CFG1.reg = (uint32) LDRV_PWM_DC_CFG1;
    LDRV->PWM_DC_CFG2.reg = (uint32) LDRV_PWM_DC_CFG2;
    LDRV->PWM_DC_CFG3.reg = (uint32) LDRV_PWM_DC_CFG3;
      
    LDRV->PWM_ISET_CFG0.reg = (uint32) LDRV_PWM_ISET_CFG0;
    LDRV->PWM_ISET_CFG1.reg = (uint32) LDRV_PWM_ISET_CFG1;
    LDRV->PWM_ISET_CFG2.reg = (uint32) LDRV_PWM_ISET_CFG2;
    LDRV->PWM_ISET_CFG3.reg = (uint32) LDRV_PWM_ISET_CFG3;

    LDRV->OSP_CFG.reg = (uint32) LDRV_OSP_CFG;

    LDRV->DIAG_CFG.reg = (uint32) LDRV_DIAG_CFG;
    LDRV->DIAG_PHASE_CFG0.reg = (uint32) LDRV_DIAG_PHASE_CFG0;
    LDRV->DIAG_PHASE_CFG1.reg = (uint32) LDRV_DIAG_PHASE_CFG1;
    LDRV->DIAG_PHASE_CFG2.reg = (uint32) LDRV_DIAG_PHASE_CFG2;
    LDRV->DIAG_PHASE_CFG3.reg = (uint32) LDRV_DIAG_PHASE_CFG3;

    LDRV->IEN0.reg = (uint32) LDRV_IEN0;
    LDRV->IEN1.reg = (uint32) LDRV_IEN1;

    LDRV->MOD_CFG.reg = (uint32) LDRV_MOD_CFG;

 // #endif


	
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
  LED_0 = currentWhitePWM;
  LED_1 = pwm->PWM_G;
  LED_2 = pwm->PWM_B;
  LED_3 = pwm->PWM_R;

  if (LED_0 > 480) //led voltage wave is delayed(60us) due to capacity between VCC and ROUT.
    detectionMask |= DHVDIFF_WHITE_DONE;//so the ADC sampling should be delayed 60us(=480 PWM CLOCK CYCLEs)
  else
    detectionMask &= ~DHVDIFF_WHITE_DONE;

  if (LED_1 > 480)
    detectionMask |= DHVDIFF_GREEN_DONE;
  else
    detectionMask &= ~DHVDIFF_GREEN_DONE;

  if (LED_2 > 480)
    detectionMask |= DHVDIFF_BLUE_DONE;
  else
    detectionMask &= ~DHVDIFF_BLUE_DONE;

  if (LED_3 > 480)
    detectionMask |= DHVDIFF_RED_DONE;
  else
    detectionMask &= ~DHVDIFF_RED_DONE;
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
static int16_t statusWCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
uint8 Led_Error_flag = 0;

#define Coef_LED2Vol (560) //475_560_645
#define Coef_VS2Vol (112) //108_112_116    P14

void moduleLedUpdateStatus(void)
{
  int16_t redVoltage = 2000, greenVoltage = 2500, blueVoltage = 2500, whiteVoltage = 2500;
  ELedStatus tmpRStatus = redStatus, tmpGStatus = greenStatus, tmpBStatus = blueStatus, tmpWStatus = whiteStatus;

  whiteVoltage = (int16_t)(ReturnADCBuffer(ADC_CH_LED0) * 8000 / 2047);
  if (whiteStatus != ELedStatusNormal)
    FilteredWhiteVoltage = whiteVoltage;
  else
    FilteredWhiteVoltage = (FilteredWhiteVoltage * 3 + whiteVoltage) / 4;

  greenVoltage = (int16_t)(ReturnADCBuffer(ADC_CH_LED1) * 8000 / 2047);
  if (greenStatus != ELedStatusNormal)
    FilteredGreenVoltage = greenVoltage;
  else
    FilteredGreenVoltage = (FilteredGreenVoltage * 3 + greenVoltage) / 4;

  blueVoltage = (int16_t)(ReturnADCBuffer(ADC_CH_LED2) * 8000 / 2047);
  if (blueStatus != ELedStatusNormal)
    FilteredBlueVoltage = blueVoltage;
  else
    FilteredBlueVoltage = (FilteredBlueVoltage * 3 + blueVoltage) / 4;

  redVoltage = (int16_t)(ReturnADCBuffer(ADC_CH_LED3) * 8000 / 2047);
  if (redStatus != ELedStatusNormal)
    FilteredRedVoltage = redVoltage;
  else
    FilteredRedVoltage = (FilteredRedVoltage * 3 + redVoltage) / 4;

  if (detectionMask & DHVDIFF_RED_DONE)
  {
    if (redVoltage >= DMAX_RED_VOLTAGE_LIMIT)
      tmpRStatus = ELedStatusOpen;
    else if (redVoltage <= DMIN_RED_VOLTAGE_LIMIT)
      tmpRStatus = ELedStatusShort;
    else
      tmpRStatus = ELedStatusNormal;
  }

  if (detectionMask & DHVDIFF_GREEN_DONE)
  {
    if (greenVoltage >= DMAX_GREEN_VOLTAGE_LIMIT)
      tmpGStatus = ELedStatusOpen;
    else if (greenVoltage <= DMIN_GREEN_VOLTAGE_LIMIT)
      tmpGStatus = ELedStatusShort;
    else
      tmpGStatus = ELedStatusNormal;
  }

  if (detectionMask & DHVDIFF_BLUE_DONE)
  {
    if (blueVoltage >= DMAX_BLUE_VOLTAGE_LIMIT)
      tmpBStatus = ELedStatusOpen;
    else if (blueVoltage <= DMIN_BLUE_VOLTAGE_LIMIT)
      tmpBStatus = ELedStatusShort;
    else
      tmpBStatus = ELedStatusNormal;
  }

  if (detectionMask & DHVDIFF_WHITE_DONE)
  {
    if (whiteVoltage >= DMAX_WHITE_VOLTAGE_LIMIT)
      tmpWStatus = ELedStatusOpen;
    else if (whiteVoltage <= DMIN_WHITE_VOLTAGE_LIMIT)
      tmpWStatus = ELedStatusShort;
    else
      tmpWStatus = ELedStatusNormal;
  }

  if ((tmpRStatus & (ELedStatusOpen | ELedStatusShort)) && (statusRCounter > 0))
  {
    statusRCounter--;
    if (statusRCounter == DLED_DETECTION_STATUS_UPDATE_COUNTER - DLED_DETECTION_Quiltcheck_COUNTER)
    {
      Quiltcheck[0] = tmpRStatus;
    }
  }
  else
  {
    redStatus = tmpRStatus;
    statusRCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
  }

  if ((tmpGStatus & (ELedStatusOpen | ELedStatusShort)) && (statusGCounter > 0))
  {
    statusGCounter--;
    if (statusGCounter == DLED_DETECTION_STATUS_UPDATE_COUNTER - DLED_DETECTION_Quiltcheck_COUNTER)
    {
      Quiltcheck[1] = tmpGStatus;
    }
  }
  else
  {
    greenStatus = tmpGStatus;
    statusGCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
  }

  if ((tmpBStatus & (ELedStatusOpen | ELedStatusShort)) && (statusBCounter > 0))
  {
    statusBCounter--;
    if (statusBCounter == DLED_DETECTION_STATUS_UPDATE_COUNTER - DLED_DETECTION_Quiltcheck_COUNTER)
    {
      Quiltcheck[2] = tmpBStatus;
    }
  }
  else
  {
    blueStatus = tmpBStatus;
    statusBCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
  }

  if ((tmpWStatus & (ELedStatusOpen | ELedStatusShort)) && (statusWCounter > 0))
  {
    statusWCounter--;
    if (statusWCounter == DLED_DETECTION_STATUS_UPDATE_COUNTER - DLED_DETECTION_Quiltcheck_COUNTER)
    {
      Quiltcheck[3] = tmpWStatus;
    }
  }
  else
  {
    whiteStatus = tmpWStatus;
    statusWCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
  }

  if ((redStatus | greenStatus | blueStatus | whiteStatus) & (ELedStatusShort | ELedStatusOpen))
  {
    sysStatus.errFlag |= ESystemErrorLedError;
  }
  else
  {
    sysStatus.errFlag &= ~ESystemErrorLedError;
  }

  if (Led_Error_flag == 0)
  {
    if (redStatus & ELedStatusOpen)
    {
      savedConfig.openRed = 1;
      flashFlag = EFlashAutoAddressSaved;
      Led_Error_flag = 1;
    }

    if (greenStatus & ELedStatusOpen)
    {
      savedConfig.openGreen = 1;
      flashFlag = EFlashAutoAddressSaved;
      Led_Error_flag = 1;
    }

    if (blueStatus & ELedStatusOpen)
    {
      savedConfig.openBule = 1;
      flashFlag = EFlashAutoAddressSaved;
      Led_Error_flag = 1;
    }

    if (redStatus & ELedStatusShort)
    {
      savedConfig.shortRed = 1;
      flashFlag = EFlashAutoAddressSaved;
      Led_Error_flag = 1;
    }

    if (greenStatus & ELedStatusShort)
    {
      savedConfig.shortGreen = 1;
      flashFlag = EFlashAutoAddressSaved;
      Led_Error_flag = 1;
    }

    if (blueStatus & ELedStatusShort)
    {
      savedConfig.shortBule = 1;
      flashFlag = EFlashAutoAddressSaved;
      Led_Error_flag = 1;
    }
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
  statusWCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;

  if (redStatus & (ELedStatusOpen|ELedStatusShort))
    redStatus = ELedStatusInit;

  if (greenStatus & (ELedStatusOpen|ELedStatusShort))
    greenStatus = ELedStatusInit;

  if (blueStatus & (ELedStatusOpen|ELedStatusShort))
    blueStatus = ELedStatusInit;

  if (whiteStatus & (ELedStatusOpen|ELedStatusShort))
    whiteStatus = ELedStatusInit;
  
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
*@param[in] w  white channel PWM value.
*
*@retval    None.
*/
void moduleLedUpdatePWM(uint16_t r, uint16_t g, uint16_t b, uint16_t w)
{

  /*if in calibration mode, turn on red constant and only*/
  
  if (ledControlFlag == ELedControlRedOn)
  {
    r = (uint16_t)PWM_PERIOD/10*4;
    g = 0;
    b = 0;
    w = 0;
  }
  /*if in calibration mode, turn on green constant and only*/
  if (ledControlFlag == ELedControlGreenOn)
  {
    r = 0;
    g = (uint16_t)PWM_PERIOD/10*4;
    b = 0;
    w = 0;
  }
  /*if in calibration mode, turn on blue constant and only*/
  if (ledControlFlag == ELedControlBlueOn)
  {
    r = 0;
    g = 0;
    b = (uint16_t)PWM_PERIOD/10*4;
    w = 0;
  }
  if (ledControlFlag == ELedControlWhiteOn)
  {
    r = 0;
    g = 0;
    b = 0;
    w = (uint16_t)PWM_PERIOD/10*4;
  }

//	if (savedConfig.factor == 0)
		
//	if (ledControlFlag == ELedControlAllNormal)
//  {
//    r = 5000;
//    g = 5000;
//    b = 5000;
//  }
  if (r > 0)
	{
    r += 3;
	}

	if (g > 0)
	{
    g += 3;
	}
	
	if (b > 0)
	{
    b += 3;
	}

	if (w > 0)
	{
    w += 3;
	}
  

  
  if ((currentPWM.PWM_R != r) ||
      (currentPWM.PWM_G != g) ||
      (currentPWM.PWM_B != b) ||
      (currentWhitePWM != w))
  {    
    if(r>=PWM_PERIOD) r=PWM_PERIOD - 1;
    if(g>=PWM_PERIOD) g=PWM_PERIOD - 1;
    if(b>=PWM_PERIOD) b=PWM_PERIOD - 1;
    if(w>=PWM_PERIOD) w=PWM_PERIOD - 1;

    currentWhitePWM = w;
    currentPWM.PWM_R = r;//(uint16_t)((uint32_t)r*65535/32000);
    currentPWM.PWM_G = g;//(uint16_t)((uint32_t)g*65535/32000);
    currentPWM.PWM_B = b;//(uint16_t)((uint32_t)b*65535/32000);
		

    
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
  moduleLedUpdatePWM(0, 0, 0, 0);
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
  whiteStatus = ELedStatusInit;
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
    case ELedChannelWhite:
      status = whiteStatus;
      break;
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
    case ELedChannelWhite:
      voltage = FilteredWhiteVoltage;
      break;
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
    case ELedChannelWhite:
      pwm = currentWhitePWM;
      break;
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
