/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    SystemMain.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file maintains system main initialization and loop.
  *Others:      None.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
                     V01: we will watch LIN signal update status out side main loop.
                          modified by Leo, Song. 2018.1.12
**********************************************************************************/


/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "SystemMain.h"
#include "Swc_Lighting.h"
#include "SystemType.h"
#include "IoHwAb_Wdg.h"
#include "IoHwAb_Time.h"
#include "IoHwAb_Adc.h"
#include "SystemStatus.h"
#include "IoHwAb_Temperature.h"
#include "Algorithm.h"
#include "Swc_PersistentData.h"
#include "IoHwAb_Led.h"
#include "IoHwAb_Battery.h"
#include "SystemControl.h"
#include "SystemLinComm.h"
#include "Swc_Diagnostic.h"
#include "Rte_Bsw.h"

/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */

/* ============================================================================ */
/* ----------------------------SYSTEM GLOBALE VARIABLES ----------------------- */
/* ============================================================================ */
/*Record last main loop execute tick.*/
static tick_t systemEventTick = 0;
/********************************************************************************
*@details   Initialize all system modules.
*
*@retval    None.
********************************************************************************/
void systemInit(void)
{
  /*EEPROM Initial, it may cause watch dog reset if put it behind*/
  Swc_PersistentData_Init();

  /*watch-dog initialize and feed once*/
  IoHwAb_Wdg_Init();
  
	IoHwAb_Wdg_Feed();
  /*Basic clock initialize(including VIC Initial), should be run at the first of functional initialization*/
  IoHwAb_Time_Init();
  
  /*ADC initial, should run before Battery/Power initial*/
  IoHwAb_Adc_Init();
  
  /*Give most of the modules flags initial status value*/
  systemStatusInit();
  systemLinCommInit();
  Swc_Diagnostic_Init();
  
  /*not used, perhaps to be defined*/
  IoHwAb_Temperature_Init();
    
  /*Algorithm initialize, set core color parameters initial values*/
  algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
  algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
  algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
  
  /*Update battery status. if other status/flags are initial value, it represents battery initialization*/
  IoHwAb_Battery_Init();
  
  /*In fact, it is PWM initial*/
  IoHwAb_Led_Init();
  IoHwAb_Wdg_Feed();
  
  /*it provides the mainloop 5ms to initialize*/
  systemEventTick = IoHwAb_Time_Get() + DSYSTEM_EVENT_LOOP_PERIOD;
//   GPIO->OUT.bit.P0 = (uint32)0;
	systemLinCommChangeNad(savedConfig.singleAddr);
	
	
}

SColorParams inputColor;
void goToSleep(void)
{
   /* LinIf owns transceiver sleep; physical MCU sleep stays in EcuM/MCAL. */
}
/********************************************************************************
*@details   System main loop function.
*
*@retval    None.
*********************************************************************************/
void systemMainLoop(void)
{
    

		bool_t LINFlag = bfalse, tickFlag = bfalse;
		
		if (flashFlag == EFlashUpdateProcessing)
		{
			IoHwAb_Wdg_Feed();
			/*if single address temporarily change to 0xA0, we do not save it*/
			if (savedConfig.singleAddr != lastSavedSingalAdress)
			{
				if(savedConfig.singleAddr!=0)
				{
					systemLinCommChangeNad(savedConfig.singleAddr);
					Rte_Call_Diagnostic_SetNad((uint8_t)savedConfig.singleAddr);
					lastSavedSingalAdress = savedConfig.singleAddr;
				}
			}
			
			/*update LED R/G/B light attributes*/
			algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
			algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
			algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);

			Swc_PersistentData_Save();
//	    while( btrue==GetEEPROMBusyState())		//delete  	?á???1y
//			{
				IoHwAb_Wdg_Feed();
//			}
			flashFlag = EFlashUpdateFree;
		}
		else //Flash/EEPROM process is over
		{
							
			/*System Tick enables the system run every 'systemEventTick' time*/
			if (DTickAfter(IoHwAb_Time_Get(), systemEventTick))
    {
      tickFlag = btrue;
      
    }
    else 
    {
      tickFlag = bfalse;
    }
			if (tickFlag == btrue)
			{
//				SColorParams inputColor;
				SPWMParams outputPWM;
				
				/* COM/RTE supplies a coherent signal-group snapshot. */
				LINFlag = systemLinCommSignalUpdate();
				
				systemEventTick += DSYSTEM_EVENT_LOOP_PERIOD;
							
				if ((LINFlag == btrue) || (tickFlag == btrue))
				{       
					/*get current system status*/
					systemStatusUpdate();

					/*get control signal*/
					systemControl(&inputColor);
											
					/*if error occurred, turn off led power, otherwise normal control*/
						if (sysStatus.errFlag == ESystemErrorNone)		
						{
								uint16_t origin_intensity = inputColor.intensity;
								inputColor.intensity = (uint16_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL);
								algoGetPWMOutput(&inputColor, &outputPWM);
//								inputColor.intensity = (uint16_t)((uint32_t)100)*DINTENSITY_ADJUST_SHIFT;
								outputPWM.PWM_R = (uint16_t)((uint32_t)(outputPWM.PWM_R) * (uint32_t)(origin_intensity)/((uint32_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL)));
								outputPWM.PWM_G = (uint16_t)((uint32_t)(outputPWM.PWM_G) * (uint32_t)(origin_intensity)/((uint32_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL)));
								outputPWM.PWM_B = (uint16_t)((uint32_t)(outputPWM.PWM_B) * (uint32_t)(origin_intensity)/((uint32_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL)));
								inputColor.intensity = origin_intensity;
								IoHwAb_Led_UpdatePWM(outputPWM.PWM_R, outputPWM.PWM_G, outputPWM.PWM_B);
//									moduleLedUpdatePWM(9291, 6814, 8523);
						}
						else  			
						{
							inputColor.intensity = 0;
							IoHwAb_Led_PowerOff();
						} //if (sysStatus.errFlag == ESystemErrorNone)
					
					
					/*System has malfunction*/
							 
								
					/*update response signals*/
					systemLinCommRespUpdate();

					/*enter into real sleep mode now*/
						if (sysStatus.sleepFlag == ESystemSleepSleep)
						{
							
							systemLinCommSleep();
							systemControlSleep();
							IoHwAb_Led_Sleep();
//              moduleLedPowerOff();
//							modulePowerSleep();	
							
							goToSleep();
						}
					
	 
				} //if(LINFlag == btrue)
				else//if(LINFlag == bfalse)
				{
					IoHwAb_Wdg_Feed();
				}
				/*Update tick. */
				
//				systemEventTick = 0;		
			}//if (tickFlag == btrue)
				 
			IoHwAb_Wdg_Feed();
//			if (currentTick > 65000)
//			{
//				currentTick = 0;
//			}
//			if (systemEventTick > 65530)
//			{
//				systemEventTick = 0;
//			}
//			if (linEventTick > 65530)
//			{
//				linEventTick = 0;
//			}
		}//if (flashFlag != EFlashUpdateProcessing)
	
}


void LIN_Task_1ms(void)
{
	/* Compatibility hook: the AUTOSAR-style BswM owns the 1 ms LIN task. */
}

void Swc_Lighting_Init(void)
{
  systemInit();
}

void Swc_Lighting_MainFunction1ms(void)
{
  systemMainLoop();
}

void Swc_Lighting_MainFunction10ms(void)
{
  /* The preserved RLNE1A loop performs its own 10 ms time check. */
}

void Swc_Lighting_MainFunction100ms(void)
{
  IoHwAb_Adc_MainFunction100ms();
}
///* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
// * Reason: HW access through library, multiple times in this function */
//int main(void)
//{
//	
//	
//   Device_init();
//   
//   /* Systick is used for LIN task */
//   /* No start needed */

//   InitHw();

//   /* Init LIN Driver */
//   LINAPP_Init();
////   LDRV->PWM_ISET_CFG0.bit.PWM_ISET = 0X0; 
////   LDRV->PWM_ISET_CFG1.bit.PWM_ISET = 0X0;
////   LDRV->PWM_ISET_CFG2.bit.PWM_ISET = 0X0;    
//   /* startup done, Turn on LED 0 */
////   LED_0 = LED_MAX_BRIGHTNESS; /*lint !e923*/

//   /* global interrupt enable not available */
//  LED_0 = LED_MAX_BRIGHTNESS / 10;
//  LED_1 = LED_MAX_BRIGHTNESS / 10;       
//  LED_2 = LED_MAX_BRIGHTNESS / 10;
////  LED_0 = 0;
////  LED_1 = 0;
////  LED_2 = 0;
////          
//   /* endless loop */
//   while (1)
//   {  // ADC
//			adcValueForVS[0] = ReturnADCBuffer(ADC_CH_VS) * 20067 / 2047;
//			adcValueForVS[1] = ReturnADCBuffer(ADC_CH_LED0) * 4000 / 2047;
//			adcValueForVS[2] = ReturnADCBuffer(ADC_CH_LED1) * 4000 / 2047;
//			adcValueForVS[3] = ReturnADCBuffer(ADC_CH_LED2) * 4000 / 2047;
//      adcValueForVS[4] = ReturnADCBuffer(ADC_CH_VTEMP) * 4000 / 2047;
//      systemStatusUpdate();
//      /* 1ms Task Cycle */
//      if (tick_1ms > 0u)
//      {          
//         tick_1ms = 0u;
//         /* Main watchdog service */
//				 (void)moduleWatchdogFeed();
//#if ((defined(J2602_SLAVE)) || (defined(LIN_SLAVE)))
//       

////       LinSlave_Task();
//#endif /* end #if ((defined(J2602_SLAVE)) || (defined(LIN_SLAVE))) */
//#if ((defined(LIN_MASTER)) || (defined(J2602_MASTER)))
//         LinMaster_Task();
//#endif /* end #ifdef LIN_MASTER */
//      }
////      if ( i < 1000 && i >= 0)
////			{
////				
//////				LIN->UART_TXBUF.reg |= 0x15;
//////				LIN->UART_TSTART.reg |= 1;

////				LED_1 = 0x0FFF;
//////				LIN->UART_TSTART.reg &= ~(1);
////			}
////			else if (i >= 1000 && i <2000)
////			{
//////				LIN->UART_TXBUF.reg |= 0x1F;
////				LED_1 = 0;
////			}
////			else if ( i >= 2000){
////				i = 0;

////			}
//   }
// }


/* Timer ISR */
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: System ISR. */


/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
