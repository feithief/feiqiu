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
#include "SystemType.h"
#include "ModuleWatchdog.h"
#include "ModuleClock.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "lin_app.h"
#include "ModuleAdc.h"
#include "SystemStatus.h"
#include "ModuleTemperature.h"
#include "Algorithm.h"
#include "ModuleFlash.h"
#include "ModuleLed.h"
#include "ModuleBattery.h"
#include "SystemControl.h"
#include "SystemLinComm.h"
#include "ModulePower.h"  //sleep related

#include "device.h"
#include "Types.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "LINApplication.h"
#include "main.h"
#include "ModuleBattery.h"
#include "pmu.h"
#include "main.h"
#include "SystemMain.h"
#include "SystemType.h"
#include "lin_autoaddressing.h"
/*Songjm added for auto-addressing*/

/*add end*/

/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */
#define DSYSTEM_LIN_TASK_CATCHUP_MAX  (4u)

/* ============================================================================ */
/* ----------------------------SYSTEM GLOBALE VARIABLES ----------------------- */
/* ============================================================================ */
/*Record last main loop execute tick.*/
static tick_t systemEventTick = 0;
extern volatile uint32_t i;
static tick_t linEventTick = 0;
static tick_t linresEventTick = 0;
static volatile unsigned char tick_1ms;
extern tick_t currentTick;
extern l_u8 lin_hal_rx_data_guc;
extern l_u16 BusIdleTimeOutCounter;
const uint8_t CmdListPID1[17] =   {0xdd, 0x1a, 0x1a, 0x1b, 0x1b, 0x1c, 0x1c, 0x1d, 0x1d, 0x1e, 0x1e, 0x20, 0x20, 0x21, 0x21,0x1a, 0x1a};
const uint8_t WriteListPID1[17] = {0x6a, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,0x2f, 0x30};

static void systemMainRunLinTaskDue(void)
{
	uint8_t catchupCount = 0u;

	while ((DTickAfter(moduleClockGet(), linEventTick)) &&
	       (catchupCount < DSYSTEM_LIN_TASK_CATCHUP_MAX))
	{
		linEventTick += LIN_TASK_CYCLE_MS;
		LIN_Task();
		Reply_message();
		catchupCount++;
	}
}


/*Songjm added for auto-addressing*/
#ifdef LIN_ENABLE_AA_BSM_B5
//#include "lin_autoaddressing.h"
	
bool_t force_lightUp = bfalse;
uint8_t assign_addr = 0;
extern uint8_t AutoAdressFlgLin;
static void startAAProcess(void)
{
	//inputColor.intensity = 0;
	moduleLedPowerOff();  
	systemLinCommChangeNad(16);
	AutoAdressFlgLin = bfalse;
	assign_addr = 0xff;
}

static void endAAProcess(void)
{
   // inputColor.intensity = 0;
   // moduleLedPowerOff();  
	//LINFlag = btrue;

	  /*IHR functions, used for LIN Initial*/
//	InitHw();
	if (assign_addr!=0xff)
	{
		l_sys_init();
		systemLinCommChangeNad(assign_addr);
	}
	
	//force_lightUp = btrue;
}		 

/*add end*/
#endif
/********************************************************************************
*@details   Initialize all system modules.
*
*@retval    None.
********************************************************************************/
void systemInit(void)
{
	Device_init();
   
   /* Systick is used for LIN task */
   /* No start needed */

   InitHw();
	
  /*EEPROM Initial, it may cause watch dog reset if put it behind*/
  moduleFlashInit(); //just for debug
  
  SlaveNodeNAD = savedConfig.singleAddr;
  /*watch-dog initialize and feed once*/
  moduleWatchdogInit();
  
  PMU_initWatchdog();
	moduleWatchdogFeed();
  /*Basic clock initialize(including VIC Initial), should be run at the first of functional initialization*/
  moduleClockInit();
  
  /*ADC initial, should run before Battery/Power initial*/
  moduleAdcInit();
  
  /*Give most of the modules flags initial status value*/
  systemStatusInit();
  
  /*not used, perhaps to be defined*/
//  moduleTempInit();
    
  /*Algorithm initialize, set core color parameters initial values*/
//   savedConfig.whiteY = savedConfig.eolAddr;
  algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
  algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
  algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
	algoUpdateWhiteCIE(savedConfig.whiteY);
  WitheBalanceTempInit();
  /*Update battery status. if other status/flags are initial value, it represents battery initialization*/
  moduleBatInit();
  
  /*In fact, it is PWM initial*/
  moduleLedInit();
  moduleWatchdogFeed();
  
  /*IHR functions, used for LIN Initial*/
  l_sys_init();
	l_set_current_NAD(savedConfig.CurrentNAD);
  /*it provides the mainloop 5ms to initialize*/
  systemEventTick = moduleClockGet() + DSYSTEM_EVENT_LOOP_PERIOD;
//   GPIO->OUT.bit.P0 = (uint32)0;

	systemLinCommChangeNad(savedConfig.singleAddr);
#ifdef LIN_ENABLE_AA_BSM_B5
	LIN_AutoAddr_Init(startAAProcess, endAAProcess);
#endif
}

void goToSleep(void)
{
	
   /* Disable LIN interrupts */
   (void)l_sys_irq_disable();
   
   /* Stop Auto Addressing procedure */
//#if (BUILD_LIN_SNPD_BSM == STD_ON)
//   LinAa_Stop(HDL);
//#endif
   
   /* Disable LEDs */
//   PrepareHwForAutoAddressing();
   
   /* Configure sleep mode and wakeup */
//	 LIN->LIN_CTRL.bit.EN = 0u;
   LIN->WAKE_CNF.bit.WU_DOM_FILT_CFG = 5u;   /* min. 130 us */
   LIN->LIN_CTRL.bit.MODE = 0u;              /* LIN transceiver sleep mode */
   PMU->WAKECTRL.bit.EXT0 = 1u;              /* LIN as external wake source */
//   LIN->LIN_CTRL.bit.TXD = 1;
//	LIN->LIN_CTRL.bit.HV_MODE = 0;
//	LIN->LIN_CTRL.bit.MODE = 2;
   /* Stop watchdog */
   PMU_stopWatchdog();
//   LIN->AA_CTRL.bit.PU_DISC = (uint32_t)1;
//	 LIN->AA_CTRL.bit.ISRC_EN = (uint32_t)0;
//	 LIN->AA_CTRL.bit.ISRC_CONF_CTRL = (uint32_t)0;
   /* Enter device sleep mode */
   SCU_enterSleepMode();
//	 __WFI();
   CMSIS_WFE();
   CMSIS_WFE();
   CMSIS_WFE();
}
uint8_t forceReInitLin = bfalse;
uint8_t SWriteLinFlag = bfalse;
//uint8_t SDiagWriteLinFlag = bfalse;
extern uint8_t LINAA_forceShutdown;
extern uint8_t AutoAdressFlg;
extern volatile S_AAMeasureData measureSuccessData;
extern int16_t maxOffset, minOffset;
extern int16_t maxPre, minPre;
extern int16_t maxSel, minSel;
volatile uint32_t aa[10];
uint8_t SyncUpdatedFlag = 0;
extern uint8_t B0FLAG;
uint8_t IsSignalUpdated(void);
/********************************************************************************
*@details   System main loop function.
*
*@retval    None.
*********************************************************************************/
void systemMainLoop(void)
{
    

		bool_t LINFlag = bfalse, tickFlag = bfalse;
		
		if (flashFlag & (EFlashUpdateProcessing | EFlashAutoAddressSaved))
		{
//			SDiagWriteLinFlag = btrue;
			moduleWatchdogFeed();
			switch(savedConfig.pART)
	    {
	    	case Slave_Address_eol:
	    	{
					if (B0FLAG == 0)
					{
						if(savedConfig.eolAddr < 1 || savedConfig.eolAddr > 14)
						{
								savedConfig.eolAddr = 14u;
						}
					}
	    		savedConfig.singleAddr = savedConfig.eolAddr;
	    		break;
	    	}
	    	case Slave_Address_auto:
	    	{
	    		savedConfig.singleAddr = savedConfig.AutoAddr;
	    		break;
	    	}
	    }
			if (B0FLAG == 0)
			{
				if ((savedConfig.singleAddr <1) || (savedConfig.singleAddr > 14))
				{
					savedConfig.singleAddr = 16;
				}
			}
			/*if single address temporarily change to 0xA0, we do not save it*/
//			if (savedConfig.singleAddr != lastSavedSingalAdress && savedConfig.singleAddr!=0)
//			{
//				systemLinCommChangeNad(savedConfig.singleAddr);
//				lastSavedSingalAdress = savedConfig.singleAddr;
//			}
			// savedConfig.whiteY = savedConfig.eolAddr;
			/*update LED R/G/B light attributes*/
			algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
			algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
			algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
			algoUpdateWhiteCIE(savedConfig.whiteY);
			
			moduleFlashSave();
	//    while( btrue==GetEEPROMBusyState())		//delete
//			{
				moduleWatchdogFeed();
//			}
			if(flashFlag & EFlashUpdateLocked)
	    	flashFlag = EFlashUpdateLocked;
	    else
	    	flashFlag = EFlashUpdateFree;
		}
		else //Flash/EEPROM process is over
		{
			if ((savedConfig.singleAddr != lastSavedSingalAdress || forceReInitLin == btrue) && savedConfig.singleAddr!=0)
			{
				systemLinCommInitColor_app();
				if(forceReInitLin == btrue)
				{
					systemLinCommInitColor_linStk();
				}
	//			systemControl_reInitDimmingStateMachine();
				systemLinCommChangeNad(savedConfig.singleAddr);
				lastSavedSingalAdress = savedConfig.singleAddr;
				forceReInitLin = bfalse;
				/*re-init color*/
		//		MID_list[0] = (0x4000 | CmdListPID1[savedConfig.singleAddr]);
	//			MID_list[7] = (0x4000 | WriteListPID1[savedConfig.singleAddr]);
	//			MID_list[8] = (0x4000 | savedConfig.singleAddr);
			}
			
			
			//GPIO->OUT.bit.P0 = 1;
			/*System Tick enables the system run every 'systemEventTick' time*/
			if (DTickAfter(moduleClockGet(), systemEventTick))
			{
				tickFlag = btrue;
				
			}
			else 
			{
				tickFlag = bfalse;
			}
			/*IHR rules that LIN_Task must run before any other IHR functions or ReadWrite operations within a loop*/
			systemMainRunLinTaskDue();

			if (DTickAfter(moduleClockGet(), linresEventTick))
			{
				linresEventTick += 10;
				systemLinCommRespUpdate();
				
				
			}
					/*update response signals*/
					
//					CMSIS_Irq_Dis();			//delete  	?????????
					
//	        CMSIS_Irq_En();			//delete  	?????????
         if (LIN_AutoAddr_Get_Status() != E_StateMachineStatusIdle)
         {
					 moduleLedPowerOff();  
					 moduleWatchdogFeed();
            return;		 
         }
         
			if (tickFlag == btrue)
			{
				SColorParams inputColor;
				SPWMParams outputPWM;
				SWriteLinFlag = Is_S_Write_SignalUpdated();
				if(SWriteLinFlag == btrue)
	      {
		      ReciveDataCrc();
	      }
         LIN_Sleep_Status();
				
				/*Update LIN signals*/      
	      //CMSIS_Irq_Dis();			
				LINFlag = systemLinCommSignalUpdate();

				
				systemEventTick += DSYSTEM_EVENT_LOOP_PERIOD;
							
				if ((LINFlag == btrue) || (tickFlag == btrue) || (sysStatus.newLinSignal == btrue))
				{      

					/*get current system status*/
					systemStatusUpdate();
					/*get control signal*/
					systemControl(&inputColor);

					   /*if error occurred, turn off led power, otherwise normal control*/
					
					{
						if ((sysStatus.errFlag == ESystemErrorNone && LINAA_forceShutdown == bfalse && (sysLin.RGB.BCM_RGB_Color_u != 0 && sysLin.RGB.BCM_RGB_Color_v != 0)))
						{
							inputColor.factor = savedConfig.factor;
								// uint16_t origin_intensity = inputColor.intensity;
								// inputColor.intensity = (uint16_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL);
								algoGetPWMOutput(&inputColor, &outputPWM);
//								inputColor.intensity = (uint16_t)((uint32_t)100)*DINTENSITY_ADJUST_SHIFT;
								// outputPWM.PWM_R = (uint16_t)((uint32_t)(outputPWM.PWM_R) * (uint32_t)(origin_intensity)/((uint32_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL)));
								// outputPWM.PWM_G = (uint16_t)((uint32_t)(outputPWM.PWM_G) * (uint32_t)(origin_intensity)/((uint32_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL)));
								// outputPWM.PWM_B = (uint16_t)((uint32_t)(outputPWM.PWM_B) * (uint32_t)(origin_intensity)/((uint32_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL)));
								// outputPWM.PWM_W = (uint16_t)((uint32_t)(outputPWM.PWM_W) * (uint32_t)(origin_intensity)/((uint32_t)(((uint32_t)100) << DINTENSITY_ADJUST_SHIFT_LEVEL)));
								// inputColor.intensity = origin_intensity;
							if(AutoAdressFlg == 5)
							{
								if (LDRV->MOD_CFG.bit.LD_ENABLE == 0)
								{
									LDRV->MOD_CFG.bit.LD_ENABLE = 1;
								}
								moduleLedUpdatePWM(outputPWM.PWM_R, outputPWM.PWM_G, outputPWM.PWM_B, outputPWM.PWM_W);
							}
						}
						else  			
						{
							inputColor.intensity = 0;
							if(AutoAdressFlg == 5)
							{
								if (LDRV->MOD_CFG.bit.LD_ENABLE == 0)
								{
									LDRV->MOD_CFG.bit.LD_ENABLE = 1;
								}
								moduleLedPowerOff();  
							}
							  
						} 
					}
					
					// Reply_message();

						if (sysStatus.sleepFlag == ESystemSleepSleep)
						{
							
							systemLinCommSleep();
							systemControlSleep();
							moduleLedSleep();
              moduleLedPowerOff();
							goToSleep();
						}
				} //if(LINFlag == btrue)
				else//if(LINFlag == bfalse)
				{
					moduleWatchdogFeed();
				}
				/*Update tick. */	
			}
				 
			moduleWatchdogFeed();

			
			
		}
	
}



void LIN_Task_1ms(void)
{
	currentTick++;
	BusIdleTimeOutCounter++;
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


l_irqmask l_sys_irq_disable (void)
{
   /* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
    * Reason: HW access through library */
   l_irqmask retVal;
   
   /* Get NVIC LIN IRQ status and disable LIN IRQ */
   retVal = ((LIN_UART_IR_SET_EN) & LIN_IR_SET_EN_MASK); /*lint !e923*/
   LIN_UART_IR_CLR_EN = LIN_IR_SET_EN_MASK; /*lint !e923*/
   return retVal;
}

void l_sys_irq_restore (l_irqmask mask)
{
   /* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
    * Reason: HW access through library */
   /* Check if IRQ was disabled in last irq_disable call */
   if ( 0u != mask )
   {
      /* Re-enable LIN IRQ */
      LIN_UART_IR_SET_EN = (LIN_UART_IR_SET_EN | LIN_IR_SET_EN_MASK); /*lint !e923*/
   }
   else
   {
      /* Do not re-enable LIN IRQ */
   }
}

/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
void LIN_Enable_Transceiver(void)
{
   /* Set LIN normal mode */
   LIN->LIN_CTRL.bit.MODE = 3u; /*lint !e923*/

   /* Disable LIN transceiver wakeup */
   PMU->WAKECTRL.bit.EXT0 = 0u; /*lint !e923*/
}

/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: Part of API, if not used MISRA violation will be issued. */
void LIN_Disable_Transceiver(void)
{
}



/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: System ISR. */
void LIN_COM_Handler(void)
{
   if (LIN->LIN_IS.bit.RI == 1u) /*lint !e923*/
   {
      l_ifc_rx();
      l_cyclic_com_task();
      /* If LIN version is < 2.1 special handling is necessary in the ISR to
       * detect bus disturbances or other communication. Bus activity is
       * signaled by the function l_ifc_read_status only from LIN 2.1 and
       * higher. */
      /* Some UARTs require special handling to detect bus stuck to
       * dominant level as the HW will generate continuously interrupts.
       * This should be prevented by the application engineer as the driver
       * cannot recognize such situations. */
      LIN->LIN_ISC.bit.RICLR = 1u; /*lint !e923*/
   }
#ifdef AUTOBAUD_ENABLED
#ifdef LIN_ENABLE_AA_BSM_B5
   //else if (((LIN->LIN_IS.reg & LINST_MASK_SYNBRK) != 0u)&&(L_RESET == (LinAa_GetActive()))) /*lint !e923*/
   else if (((LIN->LIN_IS.reg & LINST_MASK_SYNBRK) != 0u)&&(E_StateMachineStatusIdle == (LIN_AutoAddr_Get_Status()))) /*lint !e923*/
#else
   else if ((LIN->LIN_IS.reg & LINST_MASK_SYNBRK) != 0u) /*lint !e923*/
#endif
   {
      l_ifc_rx();
      l_cyclic_com_task();
      /* If LIN version is < 2.1 special handling is necessary in the ISR to
       * detect bus disturbances or other communication. Bus activity is
       * signaled by the function l_ifc_read_status only from LIN 2.1 and
       * higher. */
      /* Some UARTs require special handling to detect bus stuck to
       * dominant level as the HW will generate continuously interrupts.
       * This should be prevented by the application engineer as the driver
       * cannot recognize such situations. */
      /* LINST interrupt flags are cleared by Hardware Abstraction Layer */
   }
   else
   {
      /* unknown interrupt source */
   }
#endif /* end #ifdef AUTOBAUD_ENABLED */
	 
	 //LIN->LIN_ISC.reg = interruptStatus;
}

/* Timer ISR */
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: System ISR. */


/* (MISRA-C:2004 11.1/R, e923) Cast from unsigned long to pointer
 * Reason: HW access through library, multiple times in this function */
static void InitHw(void)
{
   /* LED settings are done in Config Wizard */

   /* variables initialization */
   tick_1ms = 0u;
   LED_0 = LED_MIN_BRIGHTNESS; /*lint !e923*/
   LED_1 = LED_MIN_BRIGHTNESS; /*lint !e923*/
   LED_2 = 0u; /*lint !e923*/
 
   /* Set tranceiver into sleep state */
   LIN->LIN_CTRL.bit.MODE = 0u; /*lint !e923*/
   
   /* Wait till state is changed to sleep */
   while (LIN->LIN_CTRL.bit.MODE != 0u) /*lint !e923*/
   {
      /* Do nothing */
   }
   
   LIN->LIN_IEN.bit.OC_IEN = 0u; /*lint !e923*/
   LIN->LIN_IEN.bit.OT_IEN = 0u; /*lint !e923*/
   LIN->LIN_IEN.bit.TXD_TMOUT_IEN = 0u; /*lint !e923*/
   
   /* First set register without changing the mode to normal mode */
   /* EN = 1, TXD = 0, HV_MODE = 0, SM = 0 */
   LIN->LIN_CTRL.reg = (uint32)1u; /*lint !e923*/
   /* Now set transceiver to normal mode (MODE = 3) */
   LIN->LIN_CTRL.reg = (uint32)7u; /*lint !e923*/   
   
   /* TRX->IRQCLR.reg = 0u; */
   /* TRX->IRQS.reg=0u; */
   
   LIN->UART_LINCON.bit.BGSEL = 3u; /*lint !e923*/
   LIN->UART_LINCON.bit.BREN = 1u; /*lint !e923*/
   LIN->LIN_IEN.bit.ERRSYNEN = 0u; /*lint !e923*/
   LIN->LIN_IEN.bit.EOFSYNEN = 0u; /*lint !e923*/
}

