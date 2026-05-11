/***************************************************************************//**
 * @file      mod_swtimer.c
 *
 * @creator   poe
 * @created   2021.05.04 
 *
 * @brief     Example for SW Timer usage.
 *
 * @purpose   Simple implementation example for usage of the swtimer module.
 *            
 * 
 *
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 * $Id: mod_swtimer.c 8848 2021-05-10 14:11:24Z poe $
 *
 * $Revision: 8848 $
 *
 ******************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/

#include "ModuleClock.h"


/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/


/*brief This variant contains current system ticks.*/
tick_t currentTick = 0;


/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

/** \brief Initialize the OSC module
 * 
 * \return sint8 0: success, <0: error codes
 */
bool_t moduleClockInit(void)
{
  /*{E6C4BFC8-8222-4912-8ED9-1229AB097C7D}*/

  sint8 s8_returnCode;
  s8_returnCode = ERR_LOG_SUCCESS; /* config can not be disabled in CW */
  
  OSC->FCLKDIV.reg = (uint32) OSC_FCLKDIV;

  return s8_returnCode;
	
} 


/********************************************************************************
*@details   system Tick interrupt service routing.
*
*@retval    None.
********************************************************************************/
void moduleClockISR(int sno)
{
  currentTick+=2;
//  SWTIMER_IRQ_VNO = 2; //clear interrupt
}



tick_t moduleClockGet(void)
{
  tick_t temp;
  temp = currentTick;
//	if (currentTick > 65000)
//	{
//		currentTick = 0;
//	}
  return temp;
}