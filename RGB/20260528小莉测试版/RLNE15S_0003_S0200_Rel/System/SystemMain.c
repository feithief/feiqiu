/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemMain.c
 * Project:		RLNE15S
 * Description:	This file contains the implementation of the main system 
 *              initialization and loop. It manages hardware and software 
 *              module initialization, system event handling, and key 
 *              operational loops.
 * Note(s):	    - This module is designed for integration in the main system loop.
 * Update Record:	
 * Version     	Date     	 Modified by       Changes Made
 * 00.01      2017.10.24      Leo Song         Initial version
 * 00.02      2024.10.12      Genhua Yang      Code standardization and optimizations
 ************************************************************************
 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
#include "SystemMain.h"
#include "SystemType.h"
#include "ModuleClock.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "lin_app.h"
#include "ModuleAdc.h"
#include "SystemStatus.h"
#include "Algorithm.h"
#include "ModuleFlash.h"
#include "SystemControl.h"
#include "SystemLinComm.h"
#include "device.h"
#include "Types.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "main.h"
#include "ModuleBattery.h"
#include "pmu.h"
#include "main.h"
#include "SystemMain.h"
#include "SystemType.h"
#include "lin.h"
#include "ModuleDebug.h"
#include "Module1000TP_Test.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< System Event Tick value >*/
static tick_t g_u16_SystemEventTick = 0;

/*!< Input Color Struct >*/
static ST_ColorParams g_st_InputColor = {0};
/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/

/**
 * @brief 		Initialize the main system components, including hardware modules, communication protocols, and algorithms.
 * @note 		This function sets up the device hardware, initializes key modules like ADC, battery, and LED control, 
 *               and configures the LIN communication interface. It ensures all system flags and parameters are set to their initial values.
 *               A watchdog is also initialized to monitor system health during operation.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemMain_Init(void)
{
		/* Initialize the device hardware */
		(void)Device_init();

		/* Systick is used for LIN task */
		/* No need to start Systick explicitly here */

		/* Initialize hardware modules */
		InitHw();

		/* Initialize EEPROM (Flash module), must be executed early to avoid watchdog reset */
		ModuleFlash_Init(); // Just for debug purposes

		/* Set the slave node NAD (Node Address) */
		g_u8_SlaveNodeNAD = (uint8_t)g_st_SavedConfig.u16_SingleAddr;

		/* Initialize the watchdog and feed it once to prevent immediate reset */
		(void)moduleWatchdogInit();
		(void)moduleWatchdogFeed();

		/* Initialize the basic system clock, including VIC (Vector Interrupt Controller) */
		(void)ModuleClock_Init();

		/* Initialize the ADC module, must be done before Battery/Power initialization */
		(void)Adc11_Init();

		/* Initialize system status and set initial values for status flags */
		SystemStatus_Init();

		/* Initialize core color parameters for the algorithm using saved configuration values */
		Algorithm_AlgoUpdateRedCIE((int16_t)g_st_SavedConfig.u16_Redx, (int16_t)g_st_SavedConfig.u16_Redy, (int32_t)g_st_SavedConfig.u32_RedY);
		Algorithm_AlgoUpdateGreenCIE((int16_t)g_st_SavedConfig.u16_Greenx, (int16_t)g_st_SavedConfig.u16_Greeny, (int32_t)g_st_SavedConfig.u32_GreenY);
		Algorithm_AlgoUpdateBlueCIE((int16_t)g_st_SavedConfig.u16_Bluex, (int16_t)g_st_SavedConfig.u16_Bluey, (int32_t)g_st_SavedConfig.u32_BlueY);

		/* Initialize the battery module and update battery status */
		ModuleBattery_Init();

		/* Initialize the LED module (PWM control) */
		ModuleLed_Init();

		/* Feed the watchdog again after initializing key modules */
		(void)moduleWatchdogFeed();
//		
		/* Initialize the LIN system */
		(void)l_sys_init();
		
		/* Allow the main loop 5 ms to initialize */
		g_u16_SystemEventTick = ModuleClock_TickGet() + DSYSTEM_EVENT_LOOP_PERIOD;

		/* Change the NAD to the configured single address */
		SystemLinComm_ChangeNad(g_st_SavedConfig.u16_SingleAddr);
}



/**
 * @brief 		Main system loop for managing LIN communication, LED control, and system status.
 * @note 		This function runs continuously, handling flash updates, LIN communication tasks, 
 *               system control, and power management, including transitioning the system into sleep mode when necessary.
 *               It also ensures that system events are processed at regular intervals.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
void SystemMain_Loop(void)
{
		static tick_t u16_LinEventTick = 0; // Tick counter for LIN tasks
		bool_t u8_TickFlag = bfalse; // Flags to track LIN signal updates and tick events

		if (g_u8_FlashFlag == FLASH_UPDATE_PROCESSING)
		{
				(void)moduleWatchdogFeed(); // Feed the watchdog to avoid reset during flash update

				/* If the current single address is different from the last saved address, update the NAD */
				if (g_st_SavedConfig.u16_SingleAddr != g_u16_LastSavedSingalAdress)
				{
						if (g_st_SavedConfig.u16_SingleAddr != 0)
						{
								SystemLinComm_ChangeNad(g_st_SavedConfig.u16_SingleAddr);
								g_u16_LastSavedSingalAdress = g_st_SavedConfig.u16_SingleAddr;
						}
				}

				/* Update the LED's Red, Green, and Blue CIE color attributes using the saved configuration */
				Algorithm_AlgoUpdateRedCIE((int16_t)g_st_SavedConfig.u16_Redx, (int16_t)g_st_SavedConfig.u16_Redy, (int32_t)g_st_SavedConfig.u32_RedY);
				Algorithm_AlgoUpdateGreenCIE((int16_t)g_st_SavedConfig.u16_Greenx, (int16_t)g_st_SavedConfig.u16_Greeny, (int32_t)g_st_SavedConfig.u32_GreenY);
				Algorithm_AlgoUpdateBlueCIE((int16_t)g_st_SavedConfig.u16_Bluex, (int16_t)g_st_SavedConfig.u16_Bluey, (int32_t)g_st_SavedConfig.u32_BlueY);
				ModuleFlash_Save(); // Save complete config before 1000TP page writes
		/* ==================== 封装后的调用接口 ==================== */
				// 直接调用封装好的自适应任务函数
				if (Module1000TP_AdaptiveWriteTask() == true)
				{
						// 只有当内部两页数据现场比对完全正确（第三阶段成功）时，才会走进这�?if
					(void)moduleWatchdogFeed(); // Feed the watchdog again after flash update
					g_u8_FlashFlag = FLASH_UPDATE_FREE; // Reset flash update flag					
					
				}
		/* ========================================================= */
				

		}
		else
		{
				/* Check if the system tick has passed the scheduled event time */
				if (DTickAfter(ModuleClock_TickGet(), g_u16_SystemEventTick) != 0)
				{
						u8_TickFlag = btrue;
				}
				else
				{
						u8_TickFlag = bfalse;
				}

				/* LIN_Task must be executed periodically before any other operations */
				if (DTickAfter(ModuleClock_TickGet(), u16_LinEventTick) != 0)
				{
						u16_LinEventTick += LIN_TASK_CYCLE_MS;
						LIN_Task();
				}



				if (u8_TickFlag == btrue)
				{
						ST_PWM_PARAMS st_OutputPWM;

						LIN_Sleep_Status(); // Check LIN sleep status

						/* Update LIN signals with interrupts disabled for safety */
						(void)CMSIS_Irq_Dis();
						(void)SystemLinComm_SignalUpdate();
						CMSIS_Irq_En();

						g_u16_SystemEventTick += DSYSTEM_EVENT_LOOP_PERIOD; // Schedule the next system event

						
						SystemStatus_Update(); // Update the current system status
						SystemControl_Control(&g_st_InputColor); // Process control logic for the system

						if (g_st_SysStatus.u8_ErrFlag == SYSTEM_ERROR_NONE) // If no error, update LED PWM output
						{
								Algorithm_AlgoGetPWMOutput(&g_st_InputColor, &st_OutputPWM);
								ModuleLed_PwmUpdate(st_OutputPWM.u16_PwmR, st_OutputPWM.u16_PwmG, st_OutputPWM.u16_PwmB);
						}
						else // If an error occurs, turn off the LED power
						{
								g_st_InputColor.u16_Intensity = 0;
								ModuleLed_PowerOff();
						}

						/* Update response signals with interrupts disabled */
						(void)CMSIS_Irq_Dis();
						SystemLinComm_RespUpdate();
						CMSIS_Irq_En();

						/* Enter sleep mode if the sleep flag is set */
						if (g_st_SysStatus.e_SleepFlag == E_SYSTEM_SLEEP_SLEEP)
						{
//								SystemLinComm_Sleep();
//								SystemControl_Sleep();
//								ModuleLed_Sleep();
//								ModuleLed_PowerOff();
//								goToSleep(); // Enter sleep mode
						}
						if (g_st_SavedConfig.u8_DebugSwitch[0] == UDS_CTRL_MAGIC_WORD)
						{						
							Process_UDS_Interface_Control();
	//						LDRV->PWM_DC_CFG1.reg = LED_MAX_BRIGHTNESS;
						}
				}

				(void)moduleWatchdogFeed(); // Ensure the watchdog is fed at the end of the loop
		}
}







