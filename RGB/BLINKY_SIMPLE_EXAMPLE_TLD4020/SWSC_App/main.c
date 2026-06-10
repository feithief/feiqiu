 /******************************************************************************
*                _  _               ___             _                         *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*    ihr GmbH                                                                 *
*    Airport Boulevard B210                                                   *
*    77836 Rheinm®πnster - Germany                                             *
*    http://www.ihr.de                                                        *
*    Phone +49(0) 7229-18475-0                                                *
*    Fax   +49(0) 7229-18475-11                                               *
*                                                                             *
*******************************************************************************
*                                                                             *
* (c) Alle Rechte bleiben bei IHR GmbH, auch fuer den Fall von Schutzrechts-  *
* anmeldungen. Jede Verfuegungsbefugnis, wie Kopier- und Weitergaberecht      *
* bleibt bei IHR GmbH.                                                        *
*                                                                             *
* (c) All rights reserved by IHR GmbH including the right to file             *
* industrial property rights. IHR GmbH retains the sole power of              *
* disposition such as reproduction or distribution.                           *
*                                                                             *
********************     Workfile:      main.c           **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  Infineon LIN Slave Driver                            *
*  FILE-DESCRIPTION:     main application file                                *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 1743        $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2023-02-22#$                                 *
*                                                                             *
******************************************************************************/
/**
@file   main.c
@brief   Main application file
*/

#include <string.h>
#include "device.h"
#include "Types.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "LINApplication.h"
#include "main.h"
#include "ModuleBattery.h"
#include "lin_hal.h"
#include "main.h"
#include "SystemMain.h"
#include "SystemType.h"
#include "ModuleFlash.h"
#include "flash.h"
/* Global variables */

//extern void initStackSpace(void);

uint8_t LINAA_forceShutdown = bfalse;
int main(void)
{
	LINAA_forceShutdown = btrue;
//	initStackSpace(); // ≥ı ºªØ’ªø’º‰
  CMSIS_Irq_Dis();	
		
  systemInit();
  CMSIS_Irq_En();	
//  	uint8 u8p_data_quote1[sizeof(savedConfig)];
//	 uint8 u8p_data_quote[UC_FLASH_PAGE_SIZE+1] = 
//  "** Be yourself; everyone else is already taken. [Oscar Wilde] ***";
//		memcpy(u8p_data_quote1, &savedConfig, sizeof(savedConfig));
//	uint8_t i;
//	for (i = 0; i < sizeof(savedConfig)+1; i++)
//	{
//		user_nvm_write(DATA_FLASH_START+i, (uint8_t *)&savedConfig+i, 1, NVM_OPTIONS_RETRY_MASK);
//    user_nvm_erase(DATA_FLASH_START+i, 1);
//	}
//	
//    
//    user_nvm_write(DATA_FLASH_START, &u8p_data_quote1[0], UC_FLASH_PAGE_SIZE, NVM_OPTIONS_RETRY_MASK);
//    user_nvm_write(DATA_FLASH_START+UC_FLASH_PAGE_SIZE, &u8p_data_quote1[0]+UC_FLASH_PAGE_SIZE, UC_FLASH_PAGE_SIZE, NVM_OPTIONS_RETRY_MASK);
//		user_nvm_erase(DATA_FLASH_START, NVM_ERASE_SECTOR);
//    user_nvm_write(DATA_FLASH_START, (uint8_t *)&savedConfig, UC_FLASH_PAGE_SIZE, NVM_OPTIONS_RETRY_MASK);
//    user_nvm_write(DATA_FLASH_START+UC_FLASH_PAGE_SIZE, (uint8_t *)&savedConfig+UC_FLASH_PAGE_SIZE, UC_FLASH_PAGE_SIZE, NVM_OPTIONS_RETRY_MASK);
//    user_nvm_erase(DATA_FLASH_START, NVM_ERASE_SECTOR);
  while(1)
  {
//    if (i <= 4000)
//    {
//      LED_0 = 0XFFFF;
//    }
//    else if (i > 4000 && i <= 8000)
//      LED_0 = 0;
//		else
//			i = 0;
		
      	systemMainLoop();
//        LED_1 =  0x0FFF;
		
  }
	return 0;
}




