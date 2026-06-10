/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
 * \file     main.c
 *
 * \brief    Program, verify and erase one page in 1000TP area
 *
 * \version  V1.0.3
 * \date     07. Jul 2025
 *
 * \note
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** VO           Vanessa Ongaro                                                **
** SL           Stefano Lissandron                                            **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2024-03-19, SL:   Initial version                                  **
** V1.0.1: 2025-02-28, VO:   [ATVCOMETLL-173] Add abstract to main.c          **
** V1.0.2: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V1.0.3: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

/*******************************************************************************
**                                  Abstract                                  **
********************************************************************************
** Program, verify and erase one page in 1000TP area                          **
********************************************************************************
** A data string is written into and erased from the 1000TP area by using the **
** startup service routines.                                                  **
** Execute this example in debug mode to set the variable u16_srvReq for      **
** according operation.                                                       **
** Set u16_srvReq to 1 to execute the write opration.                         **
** Set u16_srvReq to 2 to execute the erase operation.                        **
** The LED signals following states:                                          **
** - RED LED: Error of previous startup service routine request               **
** - GREEN LED: Content of 1000TP matches data string                         **
** - BLUE LED: Write or erase operation was successful                        **
*******************************************************************************/

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "device.h"
#include "isr.h"

/*******************************************************************************
**                        Global Constant Declarations                        **
*******************************************************************************/
#define LED_MIN_BRIGHTNESS 0x0000
#define LED_MAX_BRIGHTNESS 0x0FFF

/*******************************************************************************
**                          Global Type Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/

/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/

/*******************************************************************************
** Place your application code here                                           **
*******************************************************************************/
sint32 main(void)
{
	uint8_t u8_return_code;
	uint32_t u32_idx;
	uint32_t u32_quote_xor_checksum;
	uint32_t u32_1000tp_xor_checksum;
	const uint8_t *u8p_1000tp_read_data;
	uint16_t u16_srvReq = 0;

	/* srv_1000tp_write and srv_1000tp_erase are used by the startup service routines managed by the bootROM FW
	 * Since the bootROM FW is using the first 1kB of RAM (0x18000000 - 0x180003FF), these two variables must
	 * be placed outside this RAM area, so in the remaining 0x18000400 - 0x18000BFF
	 */
	srv_1000tp_write_t srv_1000tp_write = {UC_1000TP_START, UC_1000TP_PAGE_SIZE, "** Be yourself; everyone else is already taken. [Oscar Wilde] **"};
	srv_1000tp_erase_t srv_1000tp_erase = {UC_1000TP_START};

	/* Main watchdog service */
	(void)PMU_serviceWatchdog();

	/* Initialization of hardware modules based on Config Wizard configuration */
	u8_return_code = Device_init();

	if (u8_return_code != ERR_LOG_SUCCESS)
	{
		/* Place your code here to handle an initialization error */
	}

	if (PMU->RESETSTS.bit.VDD1V5UV)
	{
		/* Clear the reset status of VDD1V5 under-voltage */
		PMU->RESETSC.bit.VDD1V5UV = 1; 

		/*
		 * Place your code here to handle an under-voltage reset condition (e.g. after a Power-On Reset)
		 * Consider that this bit must be cleared to trigger the startup service routine during the next soft reset;
		 * otherwise the reset type is not interpreted as hot reset
		 */
	}

	if (PMU->RESETSTS.bit.SOFT)
	{
		PMU->RESETSC.bit.SOFT = 1; /* Clear the reset status for the soft reset */

		/* Place your code here to handle a soft reset condition */
	}

	/* Check status of previous startup service routine requests */
	if (MEM->SRVREQ.bit.LASTREQ && MEM->SRVREQ.bit.RESULTSTS)
	{
		/* RED LED on to signal the error condition */
		LDRV->PWM_DC_CFG0.reg = LED_MAX_BRIGHTNESS;
	}

	/* Clear startup service routine configuration to be ready for another request */
	MEM->SRVREQ.reg = 0;
	MEM->SRVREQPTR.reg = 0;

	/* Calculate a classic 32-bit xor checksum over data in the 1000TP */
	u8p_1000tp_read_data = (uint8_t *)UC_1000TP_START;
	u32_1000tp_xor_checksum = 0;
	for (u32_idx = 0; u32_idx < UC_1000TP_PAGE_SIZE; u32_idx++)
	{
		u32_1000tp_xor_checksum ^= (uint32_t)srv_1000tp_write.data[u32_idx] << ((u32_idx & 3) * 8);
	}

	/* Calculate a classic 32-bit xor checksum over data quote */
	u32_quote_xor_checksum = 0;
	for (u32_idx = 0; u32_idx < UC_1000TP_PAGE_SIZE; u32_idx++)
	{
		u32_quote_xor_checksum ^= (uint32_t)u8p_1000tp_read_data[u32_idx] << ((u32_idx & 3) * 8);
	}

	for (;;)
	{
		/* Main watchdog service */
		(void)PMU_serviceWatchdog();

		if (u32_1000tp_xor_checksum == u32_quote_xor_checksum)
		{
			/* 1000TP is containing the expected data, signal this condition with GREED LED  */
			LDRV->PWM_DC_CFG2.reg = LED_MAX_BRIGHTNESS;
		}
		else if (u16_srvReq == 1)
		{
			u16_srvReq = 0;

			/* Prepare the startup service routine request */
			MEM->SRVREQPTR.reg = (uint32_t)&srv_1000tp_write;
			MEM->SRVREQ.bit.WR1000TP = 1;

			/* Signal a pending writing request on BLUE LED */
			LDRV->PWM_DC_CFG1.reg = LED_MAX_BRIGHTNESS;

			/* Soft reset to let the bootROM execute the startup service routine */
			__NVIC_SystemReset();
		}

		if (u16_srvReq == 2)
		{
			u16_srvReq = 0;

			/* Prepare the startup service routine request */
			MEM->SRVREQPTR.reg = (uint32_t)&srv_1000tp_erase;
			MEM->SRVREQ.bit.ER1000TP = 1;

			/* Signal a pending erasing request on BLUE LED */
			LDRV->PWM_DC_CFG1.reg = LED_MAX_BRIGHTNESS;

			/* Soft reset to let the bootROM to execute the startup service routine */
			__NVIC_SystemReset();
		}
	}
}
