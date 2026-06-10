/***************************************************************************//**
 * @file		mod_sleep.c
 *
 * @creator		wca
 * @created		2022.06.23
 *
 * @brief  		Top application layer with main() entry point.
 *
 * @purpose             Hardware and LIN driver intialization, 
 *                      top application layer.
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
 * $Id: $
 *
 * $Revision:  $
 *
 ******************************************************************************/

#include "ModulePower.h"
#include "main.h"
#include "ModuleLed.h"
#include "pmu.h"

//void sleepEnSet(void)
//{
//    GucSleepEn = 1;
//}

void modulePowerInit(void)
{
	SCU->PMCTRL.bit.SLEEP = 0;
}

void modulePowerSleep(void)
{
//	LIN->LIN_CTRL.bit.MODE = LIN_LIN_CTRL_MODE_LINSleepMode; /* in order to configure some bitfields of LIN_CTRL */
//    LIN->LIN_CTRL.reg = (uint32) ((0x1 & ~LIN_LIN_CTRL_MODE_Msk) | (LIN_LIN_CTRL_MODE_LINSleepMode<<LIN_LIN_CTRL_MODE_Pos));
//    LIN->LIN_CTRL.reg = (uint32) 0x1;
//    LIN->LIN_IEN.reg = (uint32) 0xD;
//    LIN->WAKE_CNF.reg = (uint32) 0x0;
	LIN->WAKE_CNF.bit.WU_DOM_FILT_CFG = 5u;   /* min. 130 us */
   LIN->LIN_CTRL.bit.MODE = 0u;              /* LIN transceiver sleep mode */
   PMU->WAKECTRL.bit.EXT0 = 1u;              /* LIN as external wake source */
   
	LIN->LIN_CTRL.bit.TXD = (uint32_t)0;
	LIN->AA_CTRL.bit.PU_DISC = (uint32_t)1;
  LIN->AA_CTRL.bit.ISRC_EN = (uint32_t)0;
  LIN->AA_CTRL.bit.ISRC_CONF_CTRL = (uint32_t)0;
	
	//Ã»ÓÐLIN×´Ì¬·µ»Ø
	int i = 0;
	while (++i != 100);
	CPU->SYST_CSR.bit.ENABLE = 0;
		
	PMU_serviceWatchdogSOW();
    CMSIS_Irq_Dis();

//    LDRV->MOD_CFG.bit.LD_ENABLE = 0;

    SCU->PMCTRL.bit.SLEEP = 1;
//		ADC11->CFG_1.bit.ADC_ENABLE = 0;
		CMSIS_WFE();
		CMSIS_WFE();
		for (;;)
		{
		}
}

