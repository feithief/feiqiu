/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleLed.h
* Project:		RLNE15S
* Description:	The header file of ModuleLed.
* Note(s):	
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/
#ifndef MODULELED_H
#define MODULELED_H

#include "tld40xx.h"
#include "ldrv_defines.h"
#include "SystemType.h"


/*!< Defines PWM period counter >*/
#define PWM_PERIOD  31999


/*!< Current led(R/G/B) status >*/
typedef enum
{
  E_LED_STATUS_INIT   = 0x1, /*!< LED status initialized >*/
  E_LED_STATUS_NORMAL = 0x2, /*!< LED status normal >*/
  E_LED_STATUS_OPEN   = 0x4, /*!< LED open circuit detected >*/
  E_LED_STATUS_SHORT  = 0x8, /*!< LED short circuit detected >*/
}E_LED_STATUS;

/*!< This ENUM shows overwrite status.This will be used when calibration is processing >*/
typedef enum
{
  E_LED_CONTROL_ALL_NORMAL = 0x0, /*!< LED color do not over write >*/
  E_LED_CONTROL_RED_ON     = 0x1, /*!< If status set to this, LED red will be on constantly >*/
  E_LED_CONTROL_GREEN_ON   = 0x2, /*!< If status set to this, LED green will be on constantly >*/
  E_LED_CONTROL_BLUE_ON    = 0x3, /*!< If status set to this, LED blue will be on constantly >*/
}E_LED_CONTROL;

/*!< Inquiry Led status and voltages >*/
typedef enum
{
  E_LED_CHANNEL_RED   = 0x2,	/*!< Red channel of LED >*/
  E_LED_CHANNEL_GREEN = 0x3,	/*!< Green channel of LED >*/
  E_LED_CHANNEL_BLUE  = 0x4,	/*!< Blue channel of LED >*/
}E_LED_CHANNEL;

/*!< The array of fast open and short circuit detection >*/
extern uint16_t g_au16_QuickCheck[3];
/*!< Current LED over write status >*/
extern volatile E_LED_CONTROL g_e_LedControlFlag;


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
void ModuleLed_StatusUpdate(void);


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
void ModuleLed_DetectionReset(void);


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
void ModuleLed_PwmUpdate(uint16_t u16_RedPwm, uint16_t u16_GreenPwm, uint16_t u16_BluePwm);


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
void ModuleLed_PowerOff(void);


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
void ModuleLed_Sleep(void);


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
uint8_t ModuleLed_GetStatus(E_LED_CHANNEL e_Channel);


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
int16_t ModuleLed_GetVoltage(E_LED_CHANNEL e_Channel);


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
uint16_t ModuleLed_GetPwm(E_LED_CHANNEL e_Channel);

#endif
