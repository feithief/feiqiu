/**************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:        main.c
* Project:         Infineon LIN Slave Driver
* Description:     This file contains the main application logic, initializing 
*                  the system and handling the main operational loop. It integrates
*                  various system modules including LIN communication, battery 
*                  management, and system configurations.
* Note(s):         - This application is designed for LIN communication 
*                   based systems.
* Update Record:   
* Version      Date          Modified by     Why & What is modified
* 00.01        2023.02.22    B.Reiss         Initial Version
* 00.02        2024.12.12    Genhua Yang     Code standardization and optimization
***************************************************************************/
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
#include <string.h>
#include "device.h"
#include "Types.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "main.h"
#include "ModuleBattery.h"
#include "lin_hal.h"
#include "main.h"
#include "SystemMain.h"
#include "SystemType.h"
#include "ModuleFlash.h"
#include "FloatLib.h"
#include "ModuleDebug.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/





/**
 * @brief Main entry point for the program, initializes the system and enters the main loop.
 * 
 * This function serves as the primary entry point for the system. It starts by disabling 
 * interrupts with `CMSIS_Irq_Dis`, followed by calling `SystemMain_Init` to initialize the system. 
 * After initialization, it enables interrupts again with `CMSIS_Irq_En` and then enters an 
 * infinite loop where `SystemMain_Loop` is called continuously to maintain system operations.
 * 
 * @return int32_t Return status code, currently unused.
 */
int32_t main(void)
{
		moduleWatchdogFeed();

    // Disable interrupts
    (void)CMSIS_Irq_Dis();   

    // Initialize the system
    SystemMain_Init();
	 
    // Enable interrupts
    CMSIS_Irq_En();  
//	

		// System_Startup_Check();	
		

    // Infinite loop to keep the system running
    while(1)
    {
        // Call the main loop of the system
        SystemMain_Loop();
    }
}




