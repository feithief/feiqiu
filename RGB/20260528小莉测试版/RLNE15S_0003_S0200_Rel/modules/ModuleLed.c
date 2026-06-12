/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleLed.c
* Project:		RLNE15S
* Description:	Module for controlling LED functionality, including status updates, 
*               PWM adjustments, voltage readings, and detection reset. 
*               Handles initialization, sleep mode, power-off, and monitoring of 
*               LED channels (Red, Green, Blue).
* Note(s):		None.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
#include "ModuleLed.h"
#include "ModuleAdc.h"
#include "ModuleFlash.h"
#include "SystemStatus.h" 
#include "SystemControl.h"
#include "Algorithm.h"
#include "main.h"                
#include "SystemDerating.h"
#include "SystemMain.h"
#include "ModuleTemperature.h"
#include "ldrv.h"

/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/
/*!< if RED LED voltage exceed this value, we believe this led is open circuit >*/
#define DMAX_RED_VOLTAGE_LIMIT   		(4500)
/*!< if RED LED voltage lower than this value, we believe this led is short circuit >*/
#define DMIN_RED_VOLTAGE_LIMIT   		(500)
/*!< if GREEN LED voltage exceed this value, we believe this led is open circuit >*/
#define DMAX_GREEN_VOLTAGE_LIMIT   		(5000)
/*!< if GREEN LED voltage lower than this value, we believe this led is short circuit >*/
#define DMIN_GREEN_VOLTAGE_LIMIT   		(500)
/*!< if BLUE LED voltage exceed this value, we believe this led is open circuit >*/
#define DMAX_BLUE_VOLTAGE_LIMIT   		(4500)
/*!< if BLUE LED voltage lower than this value, we believe this led is short circuit >*/
#define DMIN_BLUE_VOLTAGE_LIMIT   		(500)
/*!< The const is aimed to provent status flag from fluctuating >*/
#define DLED_DETECTION_STATUS_UPDATE_COUNTER  (80)  
/*!< The filter counter of quick detection of open or short circuit >*/
#define DLED_DETECTION_Quiltcheck_COUNTER     (25U)
/*!< Flag for valid ADC sampling for the red channel >*/
#define DHVDIFF_RED_DONE    			(0x0001U)
/*!< Flag for valid ADC sampling for the green channel >*/
#define DHVDIFF_GREEN_DONE  			(0x0004U)
/*!< Flag for valid ADC sampling for the blue channel >*/
#define DHVDIFF_BLUE_DONE   			(0x0002U)
/*!< Threshold of valid ADC sampling >*/
#define MIN_SAMPLING_THRESHOLD			(480U)
/*!< The length of quick detiction of open or short circuit >*/
#define QUICK_CHECK_LENGTH     			(3U)

/*!< detection flag indicates which channel has detected >*/
static uint16_t g_u16_DetectionMask = 0;
/*!< Current status of red >*/
static uint8_t g_u8_RedStatus 	= (uint8_t)E_LED_STATUS_INIT;
/*!< Current status of green >*/
static uint8_t g_u8_GreenStatus = (uint8_t)E_LED_STATUS_INIT;
/*!< Current status of blue >*/
static uint8_t g_u8_BlueStatus 	= (uint8_t)E_LED_STATUS_INIT;
/*!< The current filtered voltage of red(mV) >*/
static volatile int32_t g_s32_FilteredRedVoltage 	= 2300;
/*!< The current filtered voltage of green(mV) >*/
static volatile int32_t g_s32_FilteredGreenVoltage 	= 2400;
/*!< The current filtered voltage of blue(mV) >*/
static volatile int32_t g_s32_FilteredBlueVoltage 	= 2400;
/*!< Current PWM ratio >*/
static ST_PWM_PARAMS  g_st_CurrentPwm = {0,0,0};
/*!< Current LED over write status >*/
volatile E_LED_CONTROL g_e_LedControlFlag;
/*!< The array of fast open and short circuit detection >*/
uint16_t g_au16_QuickCheck[QUICK_CHECK_LENGTH]= {0};
/*!< The filter counter of updating red status >*/
static int16_t g_s16_StatusRedCounter   = DLED_DETECTION_STATUS_UPDATE_COUNTER;
/*!< The filter counter of updating green status >*/
static int16_t g_s16_StatusGreenCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
/*!< The filter counter of updating blue status >*/
static int16_t g_s16_StatusBlueCounter  = DLED_DETECTION_STATUS_UPDATE_COUNTER;
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/

/**
* @brief      Sets the PWM values for controlling the LED brightness for red, green, and blue channels.
* @note       This function adjusts the PWM values for three LEDs (red, green, blue) based on the input parameters. 
* @param[in]  pst_Pwm Pointer to a structure (`ST_PWM_PARAMS`) containing the PWM values for the red, green, and blue LEDs.
* @return     None
* @retval     None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12      Sen Gao
**/
static void ModuleLed_SetPwm(const ST_PWM_PARAMS *pst_Pwm)
{  
	// Set the PWM values for the blue, green, and red LEDs
	LED_0 =  pst_Pwm->u16_PwmR;
	LED_1 =  pst_Pwm->u16_PwmB;
	LED_2 =  pst_Pwm->u16_PwmG;
	
	// Check if the PWM value of the red LED reaches the minimum sampling threshold
	if (LED_0 > MIN_SAMPLING_THRESHOLD)
	{
		g_u16_DetectionMask |= DHVDIFF_RED_DONE;
	}else{
		g_u16_DetectionMask &= ~DHVDIFF_RED_DONE;
	}
	
	// Check if the PWM value of the green LED reaches the minimum sampling threshold
	if (LED_2 > MIN_SAMPLING_THRESHOLD)
	{
		g_u16_DetectionMask |= DHVDIFF_GREEN_DONE;
	}else{
		g_u16_DetectionMask &= ~DHVDIFF_GREEN_DONE;
	}
	
	// Check if the PWM value of the blue LED reaches the minimum sampling threshold
	if (LED_1 > MIN_SAMPLING_THRESHOLD)
	{
		g_u16_DetectionMask |= DHVDIFF_BLUE_DONE;
	}else{
		g_u16_DetectionMask &= ~DHVDIFF_BLUE_DONE;
	}
}
/**
 * @brief 		Converts the ADC value of the specified channel to voltage and updates the filtered voltage value.
 * @note 		This function handles three channels: LED0, LED1, and LED2. It applies weighted averaging for 
 *       		filtered voltage if the LED status is normal.
 * @param[in]  	u32_Channel The channel number (ADC_CH_LED0, ADC_CH_LED1, or ADC_CH_LED2).
 * @param[in]  	ps16_VoltageVol Pointer to store the converted voltage value.
 * @return     	None.
 * @retval     	None.
 * @par       Edition History
 *             Version Number    Time            Author
 *             00.01             2017.10.24     Leo Song
 *             00.02             2024.10.12     Sen Gao
 */
static void ModuleLed_VoltageConvert(uint32_t u32_Channel, int16_t* ps16_VoltageVol)
{

	switch(u32_Channel)
	{
		// Get ADC value for LED0 channel and convert it to voltage (in mV).
		case ADC_CH_LED0:
			*ps16_VoltageVol = (int16_t)(ModuleAdc_RegValueGet(u32_Channel) * 8000U / 2047U);
			if (g_u8_RedStatus != (uint8_t)E_LED_STATUS_NORMAL)
			{
				g_s32_FilteredRedVoltage = *ps16_VoltageVol;
			}else{
				g_s32_FilteredRedVoltage = ((g_s32_FilteredRedVoltage*3) + (*ps16_VoltageVol))/4;
			}
			break;
		
		// Get ADC value for LED1 channel and convert it to voltage (in mV).
		case ADC_CH_LED1:
			*ps16_VoltageVol = (int16_t)(ModuleAdc_RegValueGet(u32_Channel) * 8000U / 2047U);
			if (g_u8_BlueStatus != (uint8_t)E_LED_STATUS_NORMAL)
			{
				g_s32_FilteredBlueVoltage = *ps16_VoltageVol;
			}else{
				g_s32_FilteredBlueVoltage = ((g_s32_FilteredBlueVoltage*3) + (*ps16_VoltageVol))/4;
			}
			break;
		
		// Get ADC value for LED2 channel and convert it to voltage (in mV).
		case ADC_CH_LED2:
			*ps16_VoltageVol = (int16_t)(ModuleAdc_RegValueGet(u32_Channel) * 8000U / 2047U);
			if (g_u8_GreenStatus != (uint8_t)E_LED_STATUS_NORMAL)
			{
				g_s32_FilteredGreenVoltage = *ps16_VoltageVol;
			}else{
				g_s32_FilteredGreenVoltage = ((g_s32_FilteredGreenVoltage*3) + (*ps16_VoltageVol))/4;
			}
			break;
			
		// Handle invalid channel number; no action taken.
		default:
				;
			break;
	}
	
}


/**
* @brief      Determines the status of the LED based on the given voltage thresholds.
* @note       
* @param      s16_Voltage       The measured voltage value.
* @param      s16_MinThreshold  The minimum threshold voltage.
* @param      s16_MaxThreshold  The maximum threshold voltage.
* @return     E_LED_STATUS      The determined LED status.
* @retval     E_LED_STATUS_OPEN  Voltage exceeds or equals max threshold.
* @retval     E_LED_STATUS_SHORT Voltage is below or equals min threshold.
* @retval     E_LED_STATUS_NORMAL Voltage is within normal range.
* @par        Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
static E_LED_STATUS ModuleLed_StatusDetermine(int16_t s16_Voltage, int16_t s16_MinThreshold, int16_t s16_MaxThreshold)
{
	E_LED_STATUS e_LedStatusVal = E_LED_STATUS_NORMAL;
	
	// If the voltage is greater than or equal to the maximum threshold, set the LED status to OPEN.
	if (s16_Voltage >= s16_MaxThreshold)
	{
		e_LedStatusVal = E_LED_STATUS_OPEN;
	}
	// If the voltage is less than or equal to the minimum threshold, set the LED status to SHORT.
	else if (s16_Voltage <= s16_MinThreshold)
	{
		e_LedStatusVal = E_LED_STATUS_SHORT;
	}
	// If the voltage is within the normal range, set the LED status to NORMAL.
	else
	{
	  e_LedStatusVal = E_LED_STATUS_NORMAL;
	}
	
	return e_LedStatusVal;
}


/**
* @brief      Update R/G/B LED voltage and status.
* @note       This function reads the ADC values for the red, green, and blue LEDs, filters the voltage readings, 
			    and updates the LED status based on predefined voltage limits. 
			  It also manages a counter for detecting LED faults and updates system error flags as necessary.
* @param      void
* @return     void
* @retval     None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleLed_StatusUpdate(void)
{
	int16_t s16_RedVoltage 	 = 2000;
	int16_t s16_GreenVoltage = 2500;
	int16_t s16_BlueVoltage  = 2500;
	
	uint8_t u8_TmpRStatus	=	g_u8_RedStatus;
	uint8_t u8_TmpGStatus	=	g_u8_GreenStatus;
	uint8_t u8_TmpBStatus 	= 	g_u8_BlueStatus;
 
  
	//Calculate and filter RGB voltage.
	ModuleLed_VoltageConvert(ADC_CH_LED0, &s16_RedVoltage);
	ModuleLed_VoltageConvert(ADC_CH_LED1, &s16_BlueVoltage);
	ModuleLed_VoltageConvert(ADC_CH_LED2, &s16_GreenVoltage);

	
	//Red open or short circuit detection.	
	if ((g_u16_DetectionMask & DHVDIFF_RED_DONE) != 0 )
	{
		u8_TmpRStatus = (uint8_t)ModuleLed_StatusDetermine(s16_RedVoltage, DMIN_RED_VOLTAGE_LIMIT, DMAX_RED_VOLTAGE_LIMIT);
	}
	//Green open or short circuit detection.
	if ((g_u16_DetectionMask & DHVDIFF_GREEN_DONE) != 0 )
	{
		u8_TmpGStatus = (uint8_t)ModuleLed_StatusDetermine(s16_GreenVoltage, DMIN_GREEN_VOLTAGE_LIMIT, DMAX_GREEN_VOLTAGE_LIMIT); 
	}
	//Blue open or short circuit detection.
	if ((g_u16_DetectionMask & DHVDIFF_BLUE_DONE) != 0 )
	{
		u8_TmpBStatus = (uint8_t)ModuleLed_StatusDetermine(s16_BlueVoltage, DMIN_BLUE_VOLTAGE_LIMIT, DMAX_BLUE_VOLTAGE_LIMIT);
	}

	// Handle red LED status update logic.
	if (((u8_TmpRStatus & (((uint8_t)E_LED_STATUS_OPEN)|((uint8_t)E_LED_STATUS_SHORT))) != 0) &&
	  (g_s16_StatusRedCounter > 0))
	{
		g_s16_StatusRedCounter--;
		if(g_s16_StatusRedCounter == (int16_t)(DLED_DETECTION_STATUS_UPDATE_COUNTER - DLED_DETECTION_Quiltcheck_COUNTER))
		{
			g_au16_QuickCheck[0] = (uint16_t)u8_TmpRStatus;
		}
	}else
	{
		g_u8_RedStatus = u8_TmpRStatus;
		g_s16_StatusRedCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
	}
	// Handle green LED status update logic.
	if (((u8_TmpGStatus & (((uint8_t)E_LED_STATUS_OPEN)|((uint8_t)E_LED_STATUS_SHORT))) != 0) &&
	  (g_s16_StatusGreenCounter > 0))
	{
		g_s16_StatusGreenCounter--;
		if(g_s16_StatusGreenCounter == (int16_t)(DLED_DETECTION_STATUS_UPDATE_COUNTER - DLED_DETECTION_Quiltcheck_COUNTER))
		{
			g_au16_QuickCheck[1] = (uint16_t)u8_TmpGStatus;
		}
	}
	else
	{
		g_u8_GreenStatus = u8_TmpGStatus;
		g_s16_StatusGreenCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
	}
	// Handle blue LED status update logic.
	if (((u8_TmpBStatus & (((uint8_t)E_LED_STATUS_OPEN)|((uint8_t)E_LED_STATUS_SHORT))) != 0) &&
	(g_s16_StatusBlueCounter > 0))
	{
		g_s16_StatusBlueCounter--;
		if(g_s16_StatusBlueCounter == (int16_t)(DLED_DETECTION_STATUS_UPDATE_COUNTER - DLED_DETECTION_Quiltcheck_COUNTER))
		{
			g_au16_QuickCheck[2] = (uint16_t)u8_TmpBStatus;
		}
	}
	else
	{
		g_u8_BlueStatus = u8_TmpBStatus;
		g_s16_StatusBlueCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
	}
    
	//System Indictor update
	if ( ((g_u8_RedStatus|g_u8_GreenStatus|g_u8_BlueStatus) & (((uint8_t)E_LED_STATUS_SHORT) | ((uint8_t)E_LED_STATUS_OPEN))) != 0)
	{
		g_st_SysStatus.u8_ErrFlag |= SYSTEM_ERROR_LED_ERROR;
	}
	else
	{
		g_st_SysStatus.u8_ErrFlag &= ~(SYSTEM_ERROR_LED_ERROR);
	}
}


/**
* @brief      Resets the LED detection status and error flags.
* @note       None
* @param      void
* @return     void
* @retval     None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleLed_DetectionReset(void)
{
	// Clear the LED error flag in the system status
	g_st_SysStatus.u8_ErrFlag &= ~((uint8_t)SYSTEM_ERROR_LED_ERROR);

	// Reset detection counters for all LEDs
	g_s16_StatusRedCounter   = DLED_DETECTION_STATUS_UPDATE_COUNTER;
	g_s16_StatusGreenCounter = DLED_DETECTION_STATUS_UPDATE_COUNTER;
	g_s16_StatusBlueCounter  = DLED_DETECTION_STATUS_UPDATE_COUNTER;

	// Reset the red LED status if it was previously open or short
	if ((g_u8_RedStatus & (((uint8_t)E_LED_STATUS_OPEN)|((uint8_t)E_LED_STATUS_SHORT))) != 0)
	{
		g_u8_RedStatus = (uint8_t)E_LED_STATUS_INIT;
	}
	// Reset the green LED status if it was previously open or short
	if ((g_u8_GreenStatus & (((uint8_t)E_LED_STATUS_OPEN)|((uint8_t)E_LED_STATUS_SHORT))) != 0)
	{
		g_u8_GreenStatus = (uint8_t)E_LED_STATUS_INIT;
	}
	// Reset the blue LED status if it was previously open or short
	if ((g_u8_BlueStatus & (((uint8_t)E_LED_STATUS_OPEN)|((uint8_t)E_LED_STATUS_SHORT))) != 0)
	{
		g_u8_BlueStatus = (uint8_t)E_LED_STATUS_INIT;
	}
}


/**
* @brief      Update PWM pulse value.
* @note       The function adjusts the PWM settings for red, green, and blue LEDs. 
* @param      u16_RedPwm: Desired PWM value for the red LED.
* @param      u16_GreenPwm: Desired PWM value for the green LED.
* @param      u16_BluePwm: Desired PWM value for the blue LED.
* @return     None
* @retval     None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleLed_PwmUpdate(uint16_t u16_RedPwm, uint16_t u16_GreenPwm, uint16_t u16_BluePwm)
{
	uint16_t *pau16_PwmVal[3] = {&u16_RedPwm, &u16_GreenPwm, &u16_BluePwm};
	
	// Calibration mode handling
    switch (g_e_LedControlFlag)
	{
        case E_LED_CONTROL_RED_ON: 
			// Set PWM value for the red LED to 40% of the PWM_PERIOD
			*pau16_PwmVal[0] = (uint16_t)PWM_PERIOD / 10 * 4; 
			*pau16_PwmVal[1] = 0; 
			*pau16_PwmVal[2] = 0; 
			break;
        case E_LED_CONTROL_GREEN_ON: 
			// Set PWM value for the green LED to 40% of the PWM_PERIOD
			*pau16_PwmVal[0] = 0; 
			*pau16_PwmVal[1] = (uint16_t)PWM_PERIOD / 10 * 4; 
			*pau16_PwmVal[2] = 0; 
			break;
        case E_LED_CONTROL_BLUE_ON: 
			// Set PWM value for the blue LED to 40% of the PWM_PERIOD
			*pau16_PwmVal[0] = 0; 
			*pau16_PwmVal[1] = 0; 
			*pau16_PwmVal[2] = (uint16_t)PWM_PERIOD / 10 * 4; 
			break;
		default:
			// If the LED control flag is unrecognized, do nothing
			break;
    }
	
	//Compensating for RGB pwm values
    for (uint32_t u32_Index = 0; u32_Index < 3; u32_Index++) {
        if (*pau16_PwmVal[u32_Index] > 0) {
            *pau16_PwmVal[u32_Index] += 3;
        }
    }
  

	// Check if the new PWM values differ from the current ones
	if ((g_st_CurrentPwm.u16_PwmR != u16_RedPwm)	||
		(g_st_CurrentPwm.u16_PwmG != u16_GreenPwm) 	||
		(g_st_CurrentPwm.u16_PwmB != u16_BluePwm))
	{    
		// Ensure PWM values do not exceed the PWM_PERIOD
		for (uint32_t u32_Index = 0; u32_Index < 3; u32_Index++) {
            if (*pau16_PwmVal[u32_Index] >= PWM_PERIOD) 
			{
                *pau16_PwmVal[u32_Index] = PWM_PERIOD - 1;
            }
        }

		// Update the current PWM values
		g_st_CurrentPwm.u16_PwmR = u16_RedPwm;
		g_st_CurrentPwm.u16_PwmG = u16_GreenPwm;
		g_st_CurrentPwm.u16_PwmB = u16_BluePwm;
		
		// Apply the new PWM settings to the LEDs
		ModuleLed_SetPwm(&g_st_CurrentPwm);
  }
}


/**
* @brief      Turns off all LEDs by setting their PWM values to zero.
* @note       This function effectively powers down the LED module by disabling all LED outputs.
* @param      void
* @return     void
* @retval     None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleLed_PowerOff(void)
{
	// Set PWM values for all LEDs to zero to turn them off
	ModuleLed_PwmUpdate(0, 0, 0);
}



/**
* @brief      re-initialize LED status and get ready to sleep.
* @note       None
* @param      void
* @return     void
* @retval     None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleLed_Sleep(void)
{
	g_u8_RedStatus = (uint8_t)E_LED_STATUS_INIT;
	g_u8_GreenStatus = (uint8_t)E_LED_STATUS_INIT;
	g_u8_BlueStatus = (uint8_t)E_LED_STATUS_INIT;
}

/**
* @brief      Get one of three led's status.
* @note       This function returns the status of the red, green, or blue LED based on the provided channel parameter.
* @param      channel: The LED channel for which the status is requested (red, green, or blue).
* @return     e_Status:
* @retval     The current status of the requested LED channel.
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
uint8_t ModuleLed_GetStatus(E_LED_CHANNEL e_Channel)
{
	uint8_t u8_Status = (uint8_t)E_LED_STATUS_INIT;
  
	// Determine the status based on the specified channel
	switch (e_Channel)
	{
		
		case E_LED_CHANNEL_RED:
			// Get status of the red LED
			u8_Status = g_u8_RedStatus;
			break;
		case E_LED_CHANNEL_GREEN:
			// Get status of the green LED
			u8_Status = g_u8_GreenStatus;
			break;
		case E_LED_CHANNEL_BLUE:
			// Get status of the blue LED
			u8_Status = g_u8_BlueStatus;
			break;
		
		default:
			// If the channel is not recognized, do nothing
			break;
	}
	return u8_Status;
}


/**
* @brief 		Get one of three led's voltage.
* @note 		None
* @param[in]	e_Channel                          
* @return		s32_VoltageValue
* @retval		LED's voltage
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
int16_t ModuleLed_GetVoltage(E_LED_CHANNEL e_Channel)
{
	int16_t s16_VoltageValue = 2000;
	// Determine the voltage based on the specified channel
	switch (e_Channel)
	{
		// Get filtered voltage for red LED
		case E_LED_CHANNEL_RED:
			s16_VoltageValue = (int16_t)g_s32_FilteredRedVoltage;
			break;
		// Get filtered voltage for green LED
		case E_LED_CHANNEL_GREEN:
			s16_VoltageValue = (int16_t)g_s32_FilteredGreenVoltage;
			break;
		// Get filtered voltage for blue LED
		case E_LED_CHANNEL_BLUE:
			s16_VoltageValue = (int16_t)g_s32_FilteredBlueVoltage;
			break;
		
		default:
			// If the channel is not recognized, do nothing
			break;
	}

	return s16_VoltageValue;
}


/**
* @brief 		Get one of three led's PWM.
* @note 		None
* @param[in]	e_Channel                          
* @return		u16_PwmValue
* @retval		LED's PWM value
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12      Sen Gao
**/
uint16_t ModuleLed_GetPwm(E_LED_CHANNEL e_Channel)
{
	uint16_t u16_Pwm = 0;
	
	// Determine the PWM value based on the specified channel
	switch (e_Channel)
	{
		// Get PWM for red LED
		case E_LED_CHANNEL_RED:
			u16_Pwm = g_st_CurrentPwm.u16_PwmR;
			break;
		// Get PWM for green LED
		case E_LED_CHANNEL_GREEN:
			u16_Pwm = g_st_CurrentPwm.u16_PwmG;
			break;
		// Get PWM for blue LED
		case E_LED_CHANNEL_BLUE:
			u16_Pwm = g_st_CurrentPwm.u16_PwmB;
			break;
		default:
			//do nothing
			break;
	}
	return u16_Pwm;
}
