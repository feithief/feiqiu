/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    SystemLinDiag.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file manage LIN slave receive and response functions.
  *Others:      None.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
                     V01: systemLinCommSignalUpdate will return LIN signal update status.
                          modified by Leo, Song. 2018.1.12
**********************************************************************************/

#include "SystemLinComm.h"
#include "SystemControl.h"
#include "SystemStatus.h"
#include "SystemDerating.h"
#include "SystemLinDiag.h"
#include "ModuleFlash.h"
#include "ModulePower.h"
#include "ModuleLed.h"
#include "ModuleWatchdog.h"
#include "lin_driver_api.h"
#include "lin_app.h"
#include <string.h>
/* disable colin interrupt */
#define DISABLE_COLIN_INT()               \
    do {                                  \
        Itc_Disable(COLIN_LIN);           \
    } while (0)

/* enable colin interrupt */
#define ENABLE_COLIN_INT()                \
    do {                                  \
        Itc_Clear(COLIN_LIN);             \
        Itc_SetPrio(COLIN_LIN, 5u);       \
        Itc_Enable(COLIN_LIN);            \
    } while (0)
		
		
#define BIT(A,B)      ((A>>B)&0x01)		

#define Sonderfunktion(x) l_u8_rd_BCM_RGB_Sonderfunktion_##x()

uint8_t calculateParityPID(uint8_t PID);
/**
*@details   LIN common communication initialization.
*
*@params[in] newNad new single address.
*
*@retval    None.
*/
const uint8_t CmdListPID[17] =   {0xdd, 0x1a, 0x1a, 0x5b, 0x5b, 0x9c, 0x9c, 0xdd, 0xdd, 0x5e, 0x5e, 0x20, 0x20, 0x61, 0x61, 0x1A, 0x1A};
const uint8_t WriteListPID[17] = {0x6a, 0xa3, 0x64, 0x25, 0xa6, 0xe7, 0xa8, 0xe9, 0x6a, 0x2b, 0xec, 0xad, 0x2e, 0x6f, 0xf0, 0x6f, 0xf0};
const uint8_t WriteListPID00000[17] = {0x80, 0xC1, 0x42, 0x03, 0xC4, 0x85, 0x06, 0x47, 0x08, 0x49, 0xCA, 0x8B, 0x4C, 0x0D, 0x8E, 0xCF, 0x50};
const uint8_t NadSignal_index[17] = {0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 0, 0};
uint8_t Nad_index = 0;
static uint8_t changeNadCalled = bfalse;
void systemLinCommChangeNad(uint8_t newNad)
{

	if (newNad<17 && newNad>0)
	{
		LIN_CHANGE_PID_FRAME_RGBs_Slave_08a(WriteListPID00000[newNad]);
	//	LIN_CHANGE_PID_FRAME_RGBe_Slave_a_07_08(CmdListPID[newNad]);
		LIN_CHANGE_PID_FRAME_RGBe_Write_a_08(WriteListPID[newNad]);
	}
	else
	{
		LIN_CHANGE_PID_FRAME_RGBs_Slave_08a(calculateParityPID(newNad));
//		LIN_CHANGE_PID_FRAME_RGBe_Write_a_08(calculateParityPID(newNad));
	}
	SlaveNodeNAD = savedConfig.singleAddr;
	Nad_index = NadSignal_index[newNad];
	changeNadCalled = btrue;
}


uint8_t calculateParityPID(uint8_t PID)
{
    uint8_t parity, p0,p1;

    parity = PID;

    p0=(BIT(parity,0)^BIT(parity,1)^BIT(parity,2)^BIT(parity,4))<<6;
    p1=(!(BIT(parity,1)^BIT(parity,3)^BIT(parity,4)^BIT(parity,5)))<<7;
    parity|=(p0|p1);

    return parity;
}






/** User defined read-by-identifier request.
 * This callout is used when the master node transmits a read by identifier request with
 * an identifier in the user defined area. The slave node application will be called from
 * the driver when such request is received.
 *
 * This callout function is called by the std LIN API when a Read by identifier request
 * with an identifier in the range of 32 to 63 inclusive is received from the master.
 *
 * The data buffer is preinitialised with recessive values (0xFF), so, it's not needed to be fully filled in
 * by the application.
 *
 * This callout function may also be called by the std LIN API in case an identifier
 * in this range is used in Conditional Change NAD request received from the master.
 * @note The driver will act according to the return values from the application.
 * @note This service is optional in the network and the callout is available in slave node only.
 * In case the user defined read by identifier request is used, the slave node application must implement this callout.
 * @param[in]   iii     ifc handler
 * @param[in]   id      identifier in the user defined area (32 to 63)
 * @param[in]   pci     pointer for setting PCI of response
 * @param[in]   data    pointer to response buffer
 * @return  Status:
 * - LD_NEGATIVE_RESPONSE   - the slave node will respond with a negative response;
 * - LD_POSITIVE_RESPONSE   - the slave node will setup a positive response;
 * - LD_NO_RESPONSE         - the slave node will not answer.
 */


extern bool_t SignalColorFlag ;
static SLinSignals oldLinData;
extern uint8_t LINAA_forceShutdown;
uint8_t IsSignalUpdated(void)
{
	uint8_t ret;

	ret = bfalse;

		if(l_bool_rd_BCM_RGB_GlobalSync_01() == btrue ||
			 l_bool_rd_BCM_RGB_GlobalSync_03() == btrue || 
			 l_bool_rd_BCM_RGB_GlobalSync_05() == btrue ||
			 l_bool_rd_BCM_RGB_GlobalSync_07() == btrue || 
		   l_bool_rd_BCM_RGB_GlobalSync_09() == btrue ||
			 l_bool_rd_BCM_RGB_GlobalSync_11() == btrue || 
			 l_bool_rd_BCM_RGB_GlobalSync_13() == btrue || 
			 l_bool_rd_BCM_RGB_GlobalSync() == btrue)
		{

				ret = btrue;
				LINAA_forceShutdown = bfalse;
//				l_LinData.frames.l_frm_RGBe_Sync_abcd.frame_data[0] = 0;
				l_LinData.frames.l_frm_RGBe_Slave_a_01_02.frame_data[7] &= ~(1 << 7);
				l_LinData.frames.l_frm_RGBe_Slave_a_03_04.frame_data[7] &= ~(1 << 7);
				l_LinData.frames.l_frm_RGBe_Slave_a_05_06.frame_data[7] &= ~(1 << 7);
				l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[7] &= ~(1 << 7);
				l_LinData.frames.l_frm_RGBe_Slave_a_09_10.frame_data[7] &= ~(1 << 7);
				l_LinData.frames.l_frm_RGBe_Slave_a_11_12.frame_data[7] &= ~(1 << 7);
				l_LinData.frames.l_frm_RGBe_Slave_a_13_14.frame_data[7] &= ~(1 << 7);
				SignalColorFlag = btrue;
			
		}
		else
		{
				ret = bfalse;
		}
	return ret;
}
bool_t SignalColorFlag = btrue;

void Updatebuf01_02(void)
{
		if (savedConfig.singleAddr == 1 || savedConfig.singleAddr == 2 || savedConfig.singleAddr == 16)
		{
		 do{
			 if(savedConfig.singleAddr == 2)
			 {
				if (Sonderfunktion(02)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_02() != 0 && l_u8_rd_BCM_RGB_Intensitaet_02() <= 100)||
					sysLin.RGB.BCM_RGB_Intensitaet != 0)
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_02();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_02();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_02();
						SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_02();

						
						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_02();
						
					}
				}
			 }
			 else
			 {
				if (Sonderfunktion(01)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_01() != 0 && l_u8_rd_BCM_RGB_Intensitaet_01() <= 100) ||
					sysLin.RGB.BCM_RGB_Intensitaet != 0 )
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_01();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_01();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_01();
						SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_01();

						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_01();
						
					}
				}
			 }
	//	 }
		 
	 }while(0);
 }
}

void Updatebuf03_04(void)
{

		if (savedConfig.singleAddr == 3 || savedConfig.singleAddr == 4)
		{
		 do{
				 if(savedConfig.singleAddr% 2 == 0 && savedConfig.singleAddr!=16)
				 {
					if (Sonderfunktion(04)==0)
					{
						if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
							l_u8_rd_BCM_RGB_Intensitaet_04() != 0 && l_u8_rd_BCM_RGB_Intensitaet_04() <= 100)||
						sysLin.RGB.BCM_RGB_Intensitaet != 0)
						{
							sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_04();//l_u8_rd_BCM_RGB_Color_u_07();
							sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_04();
							sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_04();
							SignalColorFlag = btrue;

									sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_04();

							
							
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_04();
						
						}
					}
					
				 }
				 else
				 {
					if (Sonderfunktion(03)==0)
					{
						if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
							l_u8_rd_BCM_RGB_Intensitaet_03() != 0 && l_u8_rd_BCM_RGB_Intensitaet_03() <= 100) ||
						sysLin.RGB.BCM_RGB_Intensitaet != 0 )
						{
							sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_03();//l_u8_rd_BCM_RGB_Color_u_07();
							sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_03();
							sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_03();
							SignalColorFlag = btrue;

									sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_03();

							
									sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_03();
							
						}
					}
				 }
			 
	 }while(0);
 }
		
}
void Updatebuf05_06(void)
{

		if (savedConfig.singleAddr == 5 || savedConfig.singleAddr == 6)
		{
		 do{
			 if(savedConfig.singleAddr% 2 == 0 && savedConfig.singleAddr!=16)
			 {
				if (Sonderfunktion(06)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_06() != 0 && l_u8_rd_BCM_RGB_Intensitaet_06() <= 100)||
					sysLin.RGB.BCM_RGB_Intensitaet != 0)
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_06();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_06();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_06();
						SignalColorFlag = btrue;
						
								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_06();

						
						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_06();
						
					}
				}
			 }
			 else
			 {
				if (Sonderfunktion(05)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_05() != 0 && l_u8_rd_BCM_RGB_Intensitaet_05() <= 100) ||
					sysLin.RGB.BCM_RGB_Intensitaet != 0 )
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_05();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_05();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_05();
						SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_05();

						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_05();
						
					}
				}
			 }
	//	 }
		 
	 }while(0);
 }
		
}

void Updatebuf07_08(void)
{
		if (savedConfig.singleAddr == 7 || savedConfig.singleAddr == 8)
		{
		 do{
		 
			 if(savedConfig.singleAddr% 2 == 0 && savedConfig.singleAddr!=16)
			 {
				if (Sonderfunktion(08)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_08() != 0 && l_u8_rd_BCM_RGB_Intensitaet_08() <= 100)||
					sysLin.RGB.BCM_RGB_Intensitaet != 0)
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_08();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_08();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_08();
						SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_08();

						
						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_08();
						
					}
				}
			 }
			 else
			 {
				if (Sonderfunktion(07)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_07() != 0 && l_u8_rd_BCM_RGB_Intensitaet_07() <= 100) ||
					sysLin.RGB.BCM_RGB_Intensitaet != 0 )
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_07();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_07();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_07();
						SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_07();

						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_07();
						
					}
				}
			 }
	//	 }
		 
	 }while(0);
 }
		
}
void Updatebuf09_10(void)
{
		if (savedConfig.singleAddr == 9 || savedConfig.singleAddr == 10)
		{
		 do{
			 if(savedConfig.singleAddr% 2 == 0 && savedConfig.singleAddr!=16)
			 {
				if (Sonderfunktion(10)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_10() != 0 && l_u8_rd_BCM_RGB_Intensitaet_10() <= 100)||
					sysLin.RGB.BCM_RGB_Intensitaet != 0)
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_10();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_10();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_10();
						SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_10();

						
						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_10();
						
					}
				}
			 }
			 else
			 {
				if (Sonderfunktion(09)==0)
				{
					if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_09() != 0 && l_u8_rd_BCM_RGB_Intensitaet_09() <= 100) ||
					sysLin.RGB.BCM_RGB_Intensitaet != 0 )
					{
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_09();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_09();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_09();
						SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_09();

						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_09();
						
					}
				}
			 }

		 
	 }while(0);
 }
		
}
void Updatebuf11_12(void)
{
		if (savedConfig.singleAddr == 11 || savedConfig.singleAddr == 12)
		{
				 do{
				 
					
					 if(savedConfig.singleAddr% 2 == 0 && savedConfig.singleAddr!=16)
					 {
						if (Sonderfunktion(12)==0)
						{
							if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
								l_u8_rd_BCM_RGB_Intensitaet_12() != 0 && l_u8_rd_BCM_RGB_Intensitaet_12() <= 100)||
							sysLin.RGB.BCM_RGB_Intensitaet != 0)
							{
								sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_12();//l_u8_rd_BCM_RGB_Color_u_07();
								sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_12();
								sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_12();
								SignalColorFlag = btrue;

										sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_12();

								
								
										sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_12();
								
							}
						}
					 }
					 else
					 {
						if (Sonderfunktion(11)==0)
				 		{
							 if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
								l_u8_rd_BCM_RGB_Intensitaet_11() != 0 && l_u8_rd_BCM_RGB_Intensitaet_11() <= 100) ||
							 sysLin.RGB.BCM_RGB_Intensitaet != 0 )
							 {
								sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_11();//l_u8_rd_BCM_RGB_Color_u_07();
								sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_11();
								sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_09();
								SignalColorFlag = btrue;

								sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_11();

						
								sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_11();
								
							}
						 }
						 
					 }
			//	 }
				 
			 }while(0);
		}
		else
		{
			
		}
		
}

void Updatebuf13_14(void)
{
		if (savedConfig.singleAddr == 13 || savedConfig.singleAddr == 14)
		{
		 do{
		 
			 if(savedConfig.singleAddr% 2 == 0 && savedConfig.singleAddr!=16)
			 {
				 if (Sonderfunktion(14)==0)
				 {
					 if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_14() != 0 && l_u8_rd_BCM_RGB_Intensitaet_14() <= 100)||
					 sysLin.RGB.BCM_RGB_Intensitaet != 0)
					 {
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_14();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_14();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_14();
						SignalColorFlag = btrue;

						sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_14();

				
				
						sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_14();
						
					}
				}
			 }
			 else
			 {
				 if (Sonderfunktion(13)==0)
				 {
					 if((sysLin.RGB.BCM_RGB_Intensitaet == 0 &&
						l_u8_rd_BCM_RGB_Intensitaet_13() != 0 && l_u8_rd_BCM_RGB_Intensitaet_13() <= 100) ||
					 sysLin.RGB.BCM_RGB_Intensitaet != 0 )
					 {
						sysLin_Stack.RGB.BCM_RGB_Color_u = (uint16_t)l_u8_rd_BCM_RGB_Color_u_13();//l_u8_rd_BCM_RGB_Color_u_07();
						sysLin_Stack.RGB.BCM_RGB_Color_v = (uint16_t)l_u8_rd_BCM_RGB_Color_v_13();
						sysLin_Stack.RGB.BCM_RGB_Dimmrampe = l_u8_rd_BCM_RGB_Dimmrampe_13();
						SignalColorFlag = btrue;
						
						sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = l_u8_rd_BCM_RGB_Sonderfunktion_13();


						
						sysLin_Stack.RGB.BCM_RGB_Intensitaet = l_u8_rd_BCM_RGB_Intensitaet_13();
						
					}
				}
			 }
	 }while(0);
 }
		
}



updatebuf_t updatebuf[7] = {
	[0] = Updatebuf01_02,
	[1] = Updatebuf03_04,
	[2] = Updatebuf05_06,
	[3] = Updatebuf07_08,
	[4] = Updatebuf09_10,
	[5] = Updatebuf11_12,
	[6] = Updatebuf13_14,
};

t_Lin_Frame_Ctrl *updatebuf_flag[7] = {
	&g_lin_frame_ctrl[0],
	&g_lin_frame_ctrl[1],
	&g_lin_frame_ctrl[2],
	&g_lin_frame_ctrl[3],
	&g_lin_frame_ctrl[4],
	&g_lin_frame_ctrl[5],
	&g_lin_frame_ctrl[6],
};

static uint8_t LinbrightnessDiff = 0u;
extern uint16_t Debug_0100[15];
uint8_t GetBrightnessDiffForSpeedBasedDimming(void)
{
	return LinbrightnessDiff;
}
extern uint8_t AutoAdressFlgLin;
extern uint8_t SWriteLinFlag;
//extern uint8_t SDiagWriteLinFlag;

extern uint8_t SyncUpdatedFlag;
/**
*@details   update LIN slave signal.
*
*@retval    if LIN signal has been updated.
*/
bool_t systemLinCommSignalUpdate(void)
{
	static uint8_t First_Lin = 0, Second_Lin = 0;

//	if (AutoAdressFlgLin == btrue)
//	{
//			updatebuf[Nad_index]();
//	}
	if (g_lin_frame_ctrl[Nad_index].frame.frame_type.update_flag == 1)
	{
//		for (uint8_t i = 0; i<7; i++)
//		{
//			g_lin_frame_ctrl[i].frame.frame_type.update_flag = 0;
//		}
		if (AutoAdressFlgLin == btrue)
		{
				updatebuf[Nad_index]();
		}
	}
	if (AutoAdressFlgLin == btrue)
	{
			
		for (uint8_t i = 0; i<8; i++)
		{
			if (g_lin_frame_ctrl[i].frame.frame_type.update_flag == 1)
			{
				g_lin_frame_ctrl[i].frame.frame_type.update_flag = 0;
				SyncUpdatedFlag = IsSignalUpdated();
	//			if (i == Nad_index)
	//			{
	//				break;
	//			}
			}
		}
		if (l_bool_rd_BCM_RGB_GlobalSync() == 1)
		{
			l_LinData.frames.l_frm_RGBe_Sync_abcd.frame_data[0] = 0;
			SyncUpdatedFlag = btrue;
			LINAA_forceShutdown = bfalse;
		}
	}
	
  if ( btrue == SyncUpdatedFlag && AutoAdressFlgLin == btrue)
	{
		SyncUpdatedFlag = bfalse;

			if(	(sysLin_Stack.RGB.BCM_RGB_Color_u != oldLinData.RGB.BCM_RGB_Color_u)
			|| (sysLin_Stack.RGB.BCM_RGB_Color_v != oldLinData.RGB.BCM_RGB_Color_v)
			|| (sysLin_Stack.RGB.BCM_RGB_Dimmrampe != oldLinData.RGB.BCM_RGB_Dimmrampe)
			|| (sysLin_Stack.RGB.BCM_RGB_Sonderfunktion != oldLinData.RGB.BCM_RGB_Sonderfunktion)
			|| (sysLin_Stack.RGB.BCM_RGB_Intensitaet != oldLinData.RGB.BCM_RGB_Intensitaet)
			|| (SWriteLinFlag == btrue)
			)
			{
				
				{
					
					if(sysLin_Stack.RGB.BCM_RGB_Intensitaet >= sysLin.RGB.BCM_RGB_Intensitaet)
					{
						LinbrightnessDiff = sysLin_Stack.RGB.BCM_RGB_Intensitaet - sysLin.RGB.BCM_RGB_Intensitaet;
					}
					else
					{
						LinbrightnessDiff = sysLin.RGB.BCM_RGB_Intensitaet - sysLin_Stack.RGB.BCM_RGB_Intensitaet;
					}
					if ( (sysLin_Stack.RGB.BCM_RGB_Color_u <= 127 && sysLin_Stack.RGB.BCM_RGB_Color_u > 0) &&
							(sysLin_Stack.RGB.BCM_RGB_Color_v <= 127 && sysLin_Stack.RGB.BCM_RGB_Color_v > 0) &&
							sysLin_Stack.RGB.BCM_RGB_Intensitaet <=100)
					{
						sysLin.RGB = sysLin_Stack.RGB;
					}
					else if ((sysLin_Stack.RGB.BCM_RGB_Color_u <= 127 && sysLin_Stack.RGB.BCM_RGB_Color_u > 0) &&
							(sysLin_Stack.RGB.BCM_RGB_Color_v <= 127 && sysLin_Stack.RGB.BCM_RGB_Color_v > 0) )
					{
						sysLin.RGB.BCM_RGB_Color_u = sysLin_Stack.RGB.BCM_RGB_Color_u;
						sysLin.RGB.BCM_RGB_Color_v = sysLin_Stack.RGB.BCM_RGB_Color_v;
						sysLin.RGB.BCM_RGB_Dimmrampe = sysLin_Stack.RGB.BCM_RGB_Dimmrampe;
					}
					else if (sysLin_Stack.RGB.BCM_RGB_Intensitaet <=100)
					{
						if (sysLin.RGB.BCM_RGB_Intensitaet != 0)
						{
							sysLin.RGB.BCM_RGB_Intensitaet = sysLin_Stack.RGB.BCM_RGB_Intensitaet;
							sysLin.RGB.BCM_RGB_Dimmrampe = sysLin_Stack.RGB.BCM_RGB_Dimmrampe;
						}
					}
					
					
					sysStatus.newLinSignal = btrue;
					


					oldLinData.RGB = sysLin_Stack.RGB;
					
		//			memcpy(&oldLinData, &sysLin, sizeof(SLinSignals));
					return btrue;
				}
			}
		
	}
return bfalse;
}


uint8_t Is_S_Write_SignalUpdated(void)
{
	uint8_t newWrite = bfalse;

	if	(		(sysLin.Swrite.SWrite_Dummy != l_u8_rd_SWrite_Dummy_08())
			|| (sysLin.Swrite.SWrite_Len != l_u8_rd_SWrite_Len_08())
			|| (sysLin.Swrite.SWrite_MemAddress != l_u8_rd_SWrite_MemAddress_08())
			|| (sysLin.Swrite.SWrite_Data_B1 != l_u8_rd_SWrite_Data_B1_08())
			|| (sysLin.Swrite.SWrite_Data_B2 != l_u8_rd_SWrite_Data_B2_08())
			|| (sysLin.Swrite.SWrite_Data_B3 != l_u8_rd_SWrite_Data_B3_08())
			|| (sysLin.Swrite.SWrite_Data_B4 != l_u8_rd_SWrite_Data_B4_08())
			|| (sysLin.Swrite.SWrite_Data_B5 != l_u8_rd_SWrite_Data_B5_08())
			|| (sysLin.Swrite.SWrite_CRC != l_u8_rd_SWrite_CRC_08())
		)
	{
		sysLin.Swrite.SWrite_Dummy = l_u8_rd_SWrite_Dummy_08();
		sysLin.Swrite.SWrite_Len = l_u8_rd_SWrite_Len_08();
		sysLin.Swrite.SWrite_MemAddress = l_u8_rd_SWrite_MemAddress_08();
		sysLin.Swrite.SWrite_Data_B1 = l_u8_rd_SWrite_Data_B1_08();
		sysLin.Swrite.SWrite_Data_B2 = l_u8_rd_SWrite_Data_B2_08();
		sysLin.Swrite.SWrite_Data_B3 = l_u8_rd_SWrite_Data_B3_08();
		sysLin.Swrite.SWrite_Data_B4 = l_u8_rd_SWrite_Data_B4_08();
		sysLin.Swrite.SWrite_Data_B5 = l_u8_rd_SWrite_Data_B5_08();
		sysLin.Swrite.SWrite_CRC = l_u8_rd_SWrite_CRC_08();
		newWrite = btrue;
	}
	return newWrite;
}



/**
*@details   system LIN communication module prepare to enter sleep.
*
*@retval    None.
*/
void systemLinCommSleep(void)
{
  oldLinData.RGB.BCM_RGB_Color_u = 0;
  oldLinData.RGB.BCM_RGB_Color_v = 0;
  oldLinData.RGB.BCM_RGB_Intensitaet = 0;
  //oldLinData.BCM_RGB_Normierung_Intensitaet = 0;
  oldLinData.RGB.BCM_RGB_Dimmrampe = 0;
  oldLinData.RGB.BCM_RGB_Sonderfunktion = 0;
 // oldLinData.BCM_RGB_Bewertung_Dimmzeit = 0;
 // oldLinData.BCM_RGB_Gueltigkeit = 0;
}
void resetlinStk_CurrentColor(void)
{
	memset(&sysLin_Stack, 0, sizeof(sysLin_Stack));
}
void systemLinCommInitColor_app(void)
{
	//	systemLinCommSleep();
	sysLin.RGB.BCM_RGB_Dimmrampe = 0;
	sysLin.RGB.BCM_RGB_Color_u = 0;
	sysLin.RGB.BCM_RGB_Color_v = 0;
	sysLin.BCM_RGB_Slave_Adresse = 0;
	sysLin.RGB.BCM_RGB_Sonderfunktion = 0;
	sysLin.RGB.BCM_RGB_Intensitaet = 0;
	
	oldLinData.RGB.BCM_RGB_Dimmrampe = 0;
	oldLinData.RGB.BCM_RGB_Color_u = 0;
	oldLinData.RGB.BCM_RGB_Color_v = 0;
	oldLinData.BCM_RGB_Slave_Adresse = 0;
	oldLinData.RGB.BCM_RGB_Sonderfunktion = 0;
	oldLinData.RGB.BCM_RGB_Intensitaet = 0;
	
	
//	sysStatus.sleepFlag = ESystemSleepAwake;
	sysLin.FristLin = FristDimingstart;
	intensityDimStatus.dimmingStateMachineInit = bfalse;
	resetlinStk_CurrentColor();
	reinitColorDimming();
}

void systemLinCommInitColor_linStk(void)
{
	memset(&l_LinData.frames.l_frm_RGBe_Slave_a_01_02.frame_data, 0, sizeof(l_LinData.frames.l_frm_RGBe_Slave_a_01_02.frame_data));
	memset(&l_LinData.frames.l_frm_RGBe_Slave_a_03_04.frame_data, 0, sizeof(l_LinData.frames.l_frm_RGBe_Slave_a_03_04.frame_data));
	memset(&l_LinData.frames.l_frm_RGBe_Slave_a_05_06.frame_data, 0, sizeof(l_LinData.frames.l_frm_RGBe_Slave_a_05_06.frame_data));
	memset(&l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data, 0, sizeof(l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data));
	memset(&l_LinData.frames.l_frm_RGBe_Slave_a_09_10.frame_data, 0, sizeof(l_LinData.frames.l_frm_RGBe_Slave_a_09_10.frame_data));
	memset(&l_LinData.frames.l_frm_RGBe_Slave_a_11_12.frame_data, 0, sizeof(l_LinData.frames.l_frm_RGBe_Slave_a_11_12.frame_data));
	memset(&l_LinData.frames.l_frm_RGBe_Slave_a_13_14.frame_data, 0, sizeof(l_LinData.frames.l_frm_RGBe_Slave_a_13_14.frame_data));

	
	
	//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[0] = 0;
//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[1] = 0;
//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[2] &= ~(0x0F);
//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[3] &= ~(0x7F);

//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[4] = 0;
//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[5] = 0;
//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[6] &= ~(0x0F);
//	l_LinData.frames.l_frm_RGBe_Slave_a_07_08.frame_data[7] &= ~(0x7F);
	
}






uint8_t calculateCRC(const uint8_t* data, size_t length, uint8_t polynomial, uint8_t initialValue, uint8_t finalXORValue)
{
    uint8_t crc = initialValue;

    for (size_t i = 0; i < length; i++)
    {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ polynomial;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc ^ finalXORValue;
}

/*area judge start*/
#define WRITEAREA_FALSE 1u
#define WRITEAREA_TRUE 0u
uint8_t SwriteJudge_brightnessFactor(uint8_t *dataArray)
{
	uint8_t retVal = WRITEAREA_FALSE;
	if((dataArray[0] & 0x7f) <= (uint8_t)100u)
	{
		retVal = WRITEAREA_TRUE;
	}
	return retVal;
}

uint8_t SwriteJudge_Utemp(uint8_t *dataArray)
{
	uint8_t retVal = WRITEAREA_FALSE;
	if(dataArray[0] <= (uint8_t)100u && dataArray[0] >= (uint8_t)60u)
	{
		retVal = WRITEAREA_TRUE;
	}
	return retVal;
}

uint8_t SwriteJudge_Vtemp(uint8_t *dataArray)
{
	uint8_t retVal = WRITEAREA_FALSE;
	if(dataArray[0] <= (uint8_t)208u && dataArray[0] >= (uint8_t)160u)
	{
		retVal = WRITEAREA_TRUE;
	}
	return retVal;
}

uint8_t SwriteJudge_pDeveloper(uint8_t *dataArray)
{
	uint8_t retVal = WRITEAREA_FALSE;
	if(dataArray[0] == (uint8_t)0u || dataArray[0] == (uint8_t)1u)
	{
		retVal = WRITEAREA_TRUE;
	}
	return retVal;
}

extern uint8_t forceReInitLin;
uint8_t SwriteJudge_pART(uint8_t *dataArray)
{
	/*This is not a write*/
	uint8_t retVal = WRITEAREA_FALSE;
	if(dataArray[0] == 0u || dataArray[0] == 1u)
	{
		retVal = WRITEAREA_TRUE;
		forceReInitLin = btrue;
	}
	return retVal;
}

uint8_t SwriteJudge_eolAddr(uint8_t *dataArray)
{
	uint8_t retVal = WRITEAREA_FALSE;
	if(dataArray[0] >= (uint8_t)1u && dataArray[0] <= (uint8_t)14u)
	{
		retVal = WRITEAREA_TRUE;
		forceReInitLin = btrue;
	}
	return retVal;
}

uint8_t SwriteJudge_AutolAddr(uint8_t *dataArray)
{
	/*read-only, always return true*/
	uint8_t retVal = WRITEAREA_TRUE;
	return retVal;
}

uint8_t SwriteJudge_DimXnYn(uint8_t *dataArray)
{
	uint8_t retVal = WRITEAREA_FALSE;
	uint16_t ytemp = (((uint16_t)dataArray[2]) << 8) | (uint16_t)dataArray[1];
	if(dataArray[0] <= (uint8_t)100u && ytemp <= (uint16_t)1000u)
	{
		retVal = WRITEAREA_TRUE;
	}
	return retVal;
}

uint8_t SwriteJudge_ResetDummy(uint8_t *dataArray)
{
	/*For reset only items, always return true*/
	uint8_t retVal = WRITEAREA_TRUE;
	return retVal;
}
/*area judge end*/

#define DMAX_SERVICE_A 25
static const LINwritedata Serverlist[] =
{
  {0x05,  	DS_READ | DS_WRITE,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.brightness_factor), 		SwriteJudge_brightnessFactor},
  {0x06,  	DS_READ | DS_WRITE,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.Utemp), 					SwriteJudge_Utemp},
  {0x07,  	DS_READ | DS_WRITE,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.Vtemp), 					SwriteJudge_Vtemp},
  {0x80,  	DS_READ | DS_WRITE,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.pDevelor),					SwriteJudge_pDeveloper},
  {0x08,  	DS_READ | DS_WRITE	,       NAD_CAL,		1u,			(uint8_t*)&(savedConfig.pART),						SwriteJudge_pART},
  {0x09,  	DS_READ| DS_WRITE ,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.eolAddr),					SwriteJudge_eolAddr},
  {0x0A,  	DS_READ 			,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.AutoAddr), 					SwriteJudge_AutolAddr},
//  {0x0C,  DS_READ | DS_WRITE,     NAD_CAL,		4u,			(uint8_t*)&(savedConfig.x2)},
//  {0x0D,  DS_READ | DS_WRITE,     NAD_CAL,		4u,			(uint8_t*)&(savedConfig.x4)},
//  {0x0E,  DS_READ | DS_WRITE,     NAD_CAL,		4u,			(uint8_t*)&(savedConfig.x6)},
//  {0x0F,  DS_READ | DS_WRITE,     NAD_CAL,		4u,			(uint8_t*)&(savedConfig.x8)},
  {0x0C, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x1),						SwriteJudge_DimXnYn},
  {0x0D, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x2),						SwriteJudge_DimXnYn},
  {0x0E, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x3),						SwriteJudge_DimXnYn},
  {0x0F, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x4),						SwriteJudge_DimXnYn},
  {0x10, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x5),						SwriteJudge_DimXnYn},
  {0x11, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x6),						SwriteJudge_DimXnYn},
  {0x12, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x7),						SwriteJudge_DimXnYn},
  {0x13, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x8),						SwriteJudge_DimXnYn},
  {0x14, 	DS_READ | DS_WRITE,     	NAD_CAL,		3u,			(uint8_t*)&(savedConfig.x9),						SwriteJudge_DimXnYn},
  {0x20,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.shortRed),					SwriteJudge_ResetDummy},
  {0x21,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.shortGreen),				SwriteJudge_ResetDummy},
  {0x22,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.shortBule),					SwriteJudge_ResetDummy},
  {0x23,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.openRed),					SwriteJudge_ResetDummy},
  {0x24,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.openGreen),					SwriteJudge_ResetDummy},
  {0x25,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.openBule),					SwriteJudge_ResetDummy},
  {0x26,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.error_ram),					SwriteJudge_ResetDummy},
  {0x27,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.error_rom),					SwriteJudge_ResetDummy},
  {0x28,  	DS_READ | DS_RESERT,     	NAD_CAL,		1u,			(uint8_t*)&(savedConfig.error_NVM),					SwriteJudge_ResetDummy},

};
void ReciveDataCrc(void)
{
	static uint8_t flag_reset = 0;
	uint8_t* dataP[7];
	uint8_t data[7],data_splicing[2];
	static uint8_t old_data[7];
	static uint8_t old_data_Reset[7];
	uint8_t crc;
    uint8_t polynomial = 0x97;
    uint8_t initialValue = 0xFF;
    uint8_t finalXORValue = 0x00;
    size_t dataLength;

	sysLin.Swrite.SWrite_Dummy = l_u8_rd_SWrite_Dummy_08();
	sysLin.Swrite.SWrite_Len = l_u8_rd_SWrite_Len_08();
	sysLin.Swrite.SWrite_MemAddress = l_u8_rd_SWrite_MemAddress_08();
	sysLin.Swrite.SWrite_Data_B1 = l_u8_rd_SWrite_Data_B1_08();
	sysLin.Swrite.SWrite_Data_B2 = l_u8_rd_SWrite_Data_B2_08();
	sysLin.Swrite.SWrite_Data_B3 = l_u8_rd_SWrite_Data_B3_08();
	sysLin.Swrite.SWrite_Data_B4 = l_u8_rd_SWrite_Data_B4_08();
	sysLin.Swrite.SWrite_Data_B5 = l_u8_rd_SWrite_Data_B5_08();
	sysLin.Swrite.SWrite_CRC = l_u8_rd_SWrite_CRC_08();  //Acceptance of data

	data_splicing[0] = sysLin.Swrite.SWrite_Dummy;
	data_splicing[1] = sysLin.Swrite.SWrite_Len;
	data_splicing[1] = data_splicing[1]<<5;
	data_splicing[1] = data_splicing[1]|data_splicing[0];//The data in the first two frames of the concatenation is one byte

	dataP[0] = &data_splicing[1];
	dataP[1] = &sysLin.Swrite.SWrite_MemAddress;
	dataP[2] = &sysLin.Swrite.SWrite_Data_B1;
	dataP[3] = &sysLin.Swrite.SWrite_Data_B2;
	dataP[4] = &sysLin.Swrite.SWrite_Data_B3;
	dataP[5] = &sysLin.Swrite.SWrite_Data_B4;
	dataP[6] = &sysLin.Swrite.SWrite_Data_B5;

	data[0] = data_splicing[1];
	data[1] = sysLin.Swrite.SWrite_MemAddress;
	data[2] = sysLin.Swrite.SWrite_Data_B1;
	data[3] = sysLin.Swrite.SWrite_Data_B2;
	data[4] = sysLin.Swrite.SWrite_Data_B3;
	data[5] = sysLin.Swrite.SWrite_Data_B4;
	data[6] = sysLin.Swrite.SWrite_Data_B5;

	dataLength = sizeof(data) / sizeof(data[0]);
	crc = calculateCRC(data, dataLength, polynomial, initialValue, finalXORValue);//Calculate the CRC

	if(crc == sysLin.Swrite.SWrite_CRC)
	{
		extern uint8_t B0FLAG;
		B0FLAG = 0;
		for (uint8_t index = 0; index < DMAX_SERVICE_A; index++)
		{
		    if ((Serverlist[index].ADDRESS == data[1] ) &&
		        (Serverlist[index].length  == sysLin.Swrite.SWrite_Len))
		    {
		    	if(WRITEAREA_TRUE == Serverlist[index].judge(&data[2]))
		    	{
					if(Serverlist[index].ADDRESS == 0x80)
					{
							for(uint8_t j=0;j<Serverlist[index].length;j++)
							{
								Serverlist[index].addr[j] = *(dataP[j+2]);
							}
					         if (/*(flashFlag == EFlashUpdateFree)&&*/(memcmp(&old_data, &data, sizeof(data)) != 0))
						         {
						            flashFlag = EFlashUpdateProcessing;
						            memcpy(&old_data, &data, sizeof(data));
						         }
					}
					else if(Serverlist[index].ADDRESS == 0x09)
					{
							for(uint8_t j=0;j<Serverlist[index].length;j++)
							{
								Serverlist[index].addr[j] = *(dataP[j+2]);
							}
							if ((memcmp(&old_data, &data, sizeof(data)) != 0))
							{

								flashFlag = EFlashUpdateProcessing;
//								savedConfig.pART = Slave_Address_eol;
								memcpy(&old_data, &data, sizeof(data));
							}
					}//Serverlist[index].rwAttr  == DS_WRITE
					else if(Serverlist[index].rwAttr & DS_WRITE)
					{
							for(uint8_t j=0;j<Serverlist[index].length;j++)
							{
								Serverlist[index].addr[j] = *(dataP[j+2]);
							}
							if (memcmp(&old_data, &data, sizeof(data)) != 0)
							{
								if(data[1] == 0x06u)
								{
									savedConfig.u++;
									savedConfig.U[(savedConfig.u)%10] = data[2];
								}
								if(data[1] == 0x07)
								{
									savedConfig.v++;
									savedConfig.V[(savedConfig.v)%10] = data[2];
								}
								flashFlag = EFlashUpdateProcessing;
//								if(Serverlist[index].ADDRESS == 0x05)
//								{
//									Serverlist[index].addr[0] = Serverlist[index].addr[0]>(uint8_t)100u?(uint8_t)100u:Serverlist[index].addr[0];
//								}
								memcpy(&old_data, &data, sizeof(data));
							}
					}//Serverlist[index].rwAttr  == DS_WRITE
					else if((((Serverlist[index].rwAttr) &DS_RESERT)== DS_RESERT) && (data[2] == 0))
					{
						if(savedConfig.pDevelor	==	btrue)
						{
							Serverlist[index].addr[0] = 0;//*init;
							
						 if (/*(flashFlag == EFlashUpdateFree)&&*/(memcmp(&old_data_Reset, &data, sizeof(data)) != 0))
						 {
								flashFlag = EFlashUpdateProcessing;
								memcpy(&old_data_Reset, &data, sizeof(data));
						 }
						}
					}
		    	}
					else if(data[1] == 0x06u || data[1] == 0x07)
		    	{
		    		/*in case of write data UV out of range, fallback to default values*/
		    		savedConfig.Utemp = 80;
		    		savedConfig.Vtemp = 188;
						if (memcmp(&old_data, &data, sizeof(data)) != 0)
						{
							flashFlag = EFlashUpdateProcessing;

							memcpy(&old_data, &data, sizeof(data));
						}
		    		
		    	}
				break;
		    }//Serverlist[index].ADDRESS
		}//for
	}//crc == sysLin.SWrite_CRC

}

SLINREQESTNVM recive_buffer;
SLINANSNVM send_buffer;
#define replyJudge_UNINIT ((uint8_t)0xffu)
void Reply_message(void)
{
	uint8_t replyJudge = false;
	static uint8_t lastreplyJudge = replyJudge_UNINIT;
	uint8_t index = 0;
	uint8_t send_11frame_data[5]={0};

	recive_buffer.Requst_NAD 	= l_u8_rd_RGBe_Req_NAD();
	recive_buffer.Requst_Adress = l_u8_rd_RGBe_Req_lin_nvmadresse_rsp();
	recive_buffer.Requst_Length = l_u8_rd_RGBe_Req_lin_number_bytes_rsp();
	if(savedConfig.pDevelor == btrue && savedConfig.singleAddr == recive_buffer.Requst_NAD)
	{
		replyJudge = btrue;
	}
	else
	{
		replyJudge = bfalse;
	}
	if(replyJudge == btrue)
	{
		for (index = 0; index < DMAX_SERVICE_A; index++)
		{
			 if ((Serverlist[index].ADDRESS == recive_buffer.Requst_Adress ) &&
				(Serverlist[index].length  == recive_buffer.Requst_Length))
			 {
				 send_buffer.Anwser_NAD = savedConfig.singleAddr;
				 send_buffer.Anwser_Adress = Serverlist[index].ADDRESS;
				 send_buffer.Anwser_Length = Serverlist[index].length;
				  for(uint16_t j = 0; j < send_buffer.Anwser_Length; j++)
				  {
					  if(j >= 5)
					  {
						  /*prevent overflow*/
						  break;
					  }
					  send_11frame_data[j] = Serverlist[index].addr[j];

				  }

				  send_buffer.Anwser_data01 = send_11frame_data[0];
				  send_buffer.Anwser_data02 = send_11frame_data[1];
				  send_buffer.Anwser_data03 = send_11frame_data[2];
				  send_buffer.Anwser_data04 = send_11frame_data[3];

//				  send_buffer.Anwser_RSID = 0x39u;
//				  l_u8_wr_RGBe_Ans_RSID(send_buffer.Anwser_RSID);
				  l_u8_wr_RGBe_Ans_NAD(send_buffer.Anwser_NAD);
				  l_u8_wr_RGBe_Ans_lin_nvmadress_read_rsp(send_buffer.Anwser_Adress);
				  l_u8_wr_RGBe_Ans_lin_number_bytes_rsp(send_buffer.Anwser_Length);
				  l_u8_wr_RGBe_Ans_databyte01(send_buffer.Anwser_data01);
				  l_u8_wr_RGBe_Ans_databyte02(send_buffer.Anwser_data02);
				  l_u8_wr_RGBe_Ans_databyte03(send_buffer.Anwser_data03);
				  l_u8_wr_RGBe_Ans_databyte04(send_buffer.Anwser_data04);
				  l_u8_wr_RGBe_Ans_databyte05(send_buffer.Anwser_data05);
				  break;
			 }
		}
		if(index == DMAX_SERVICE_A)
		{
			replyJudge = bfalse;
		}
	}
	if(lastreplyJudge != replyJudge || changeNadCalled == btrue)
	{
		if(replyJudge == btrue)
		{
			lin_set_frame_enable(10);
//			lin_set_frame_enable(5);
		}
		else
		{
			lin_set_frame_disable(10);
//			lin_set_frame_disable(5);
		}
		lastreplyJudge = replyJudge;
		changeNadCalled = bfalse;
	}
}
