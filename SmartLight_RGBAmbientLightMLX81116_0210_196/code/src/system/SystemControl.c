/*
 * SystemControl.c
 *
 *  Created on: 2020Äê1ÔÂ1ÈÕ
 *      Author: SH1705016
 */

#include <algorithm.h>
#include "SystemControl.h"
#include "ModuleComm.h"
#include "ModulePwm.h"
#include "ModuleFlash.h"
#include "ModuleAdc.h"
#include "ModuleTemprature.h"
#include <bl_tables.h>

typedef union LedColorData
{
  uint8_t rawData[4];

  struct{
  uint32_t L : 12;
  uint32_t v : 10;
  uint32_t u : 10;
  } color __attribute__((packed));
}SLedColorData_t;

void systemControlInit(void)
{

}

uint16_t ledCalData[15] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		                   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

/*systemStatus includes led opencircuit status¡¢ shortcircuit status¡¢chip temperature and led temperature*/
uint16_t systemStatus[15] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		                     0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

#define RED_OPEN_VOL 4500        //red led opencircuit threshold 4500mV
#define RED_SHORTCUT_VOL 400     //red led shortcircuit threshold 400mV
#define GREEN_OPEN_VOL 4500      //green led opencircuit threshold 4500mV
#define GREEN_SHORTCUT_VOL 400   //green led shortcircuit threshold 400mV
#define BLUE_OPEN_VOL 4500       //blue led opencircuit threshold 4500mV
#define BLUE_SHORTCUT_VOL 400    //blue led shortcircuit threshold 400mV

#define LED_OPEN 1
#define LED_SHORTCUT 2
#define LED_NORMAL 0

uint8_t txData[30] = {0};//data buffer of open/short state¡¢chip temperature and led temperature
SLEDVoltages currentLedVoltage;

uint16_t cntredLedOpen[RGBLEDMAX] = {0};
uint16_t cntredLedShortCut[RGBLEDMAX] = {0};
uint16_t cntgreenLedOpen[RGBLEDMAX] = {0};
uint16_t cntgreenLedShortCut[RGBLEDMAX] = {0};
uint16_t cntblueLedOpen[RGBLEDMAX] = {0};
uint16_t cntblueLedShortCut[RGBLEDMAX] = {0};

#define ERRORCNT 20//It is be considered that the fault has occurred when opencircuit or shortcircuit is continuously detected 20 times.

uint8_t shortCutState = 0;
uint8_t openState = 0;

void LedVoltageInit(SLEDVoltages *LedVoltage)
{
	uint16_t cntLeds = 0;

	for(cntLeds = 0; cntLeds < RGBLEDMAX; cntLeds++)
	{
		LedVoltage->LedVoltage[cntLeds].ledChannelData[1]  = 1745;
		LedVoltage->LedVoltage[cntLeds].ledChannelData[0]  = 2758;
		LedVoltage->LedVoltage[cntLeds].ledChannelData[2]  = 2599;
	}
}

void ShortCutCntInit(void)
{
	uint16_t cntLeds = 0;

	for(cntLeds = 0; cntLeds < RGBLEDMAX; cntLeds++)
	{
		cntredLedShortCut[cntLeds] = 0;
		cntgreenLedShortCut[cntLeds] = 0;
		cntblueLedShortCut[cntLeds] = 0;
	}
}

void OpenCntInit(void)
{
	uint16_t cntLeds = 0;

	for(cntLeds = 0; cntLeds < RGBLEDMAX; cntLeds++)
	{
		cntredLedOpen[cntLeds] = 0;
		cntgreenLedOpen[cntLeds] = 0;
		cntblueLedOpen[cntLeds] = 0;
	}
}

void slaveStatusBufferUpdate(void)
{
	uint16_t cntLeds = 0;
	uint8_t i = 0;

	txData[0] = 0x00;//status circuit ram rom nvrm
	txData[1] = 0x00;
	txData[2] = 0x00;
	txData[3] = 0x00;
	txData[4] = 0x00;
	txData[5] = 0x00;
	txData[6] = 0x00;

	for(cntLeds = 0; cntLeds < RGBLEDMAX; cntLeds++)
	{
		//No detection when the led is turned off
		if((getLedAdcSampleState(cntLeds, 1) == 0)&&(shortCutState == 0)&&(openState == 0))
		{
			txData[cntLeds + 1] = (LED_NORMAL<<0);
		}
		else
		{
			//red led voltage
			if(currentLedVoltage.LedVoltage[cntLeds].ledChannelData[1] > RED_OPEN_VOL)
			{
				cntredLedOpen[cntLeds]++;
				if(cntredLedOpen[cntLeds] >= ERRORCNT)
				{
					cntredLedOpen[cntLeds] = ERRORCNT;
					txData[cntLeds + 1] = (LED_OPEN<<0);
					openState = 1;
				}
			}
			else if(currentLedVoltage.LedVoltage[cntLeds].ledChannelData[1] < RED_SHORTCUT_VOL)
			{
				cntredLedShortCut[cntLeds]++;
				if(cntredLedShortCut[cntLeds] >= ERRORCNT)
				{
					cntredLedShortCut[cntLeds] = ERRORCNT;
					txData[cntLeds + 1] = (LED_SHORTCUT<<0);
					shortCutState = 1;
				}
			}
			else
			{
				cntredLedOpen[cntLeds] = 0;
				cntredLedShortCut[cntLeds] = 0;
				txData[cntLeds + 1] = (LED_NORMAL<<0);
			}
		}

		//No detection when the led is turned off
		if((getLedAdcSampleState(cntLeds, 0) == 0)&&(shortCutState == 0)&&(openState == 0))
		{
			txData[cntLeds + 1] |= (LED_NORMAL<<2);
		}
		else
		{
			//green led voltage
			if(currentLedVoltage.LedVoltage[cntLeds].ledChannelData[0] > GREEN_OPEN_VOL)
			{
				cntgreenLedOpen[cntLeds]++;
				if(cntgreenLedOpen[cntLeds] >= ERRORCNT)
				{
					cntgreenLedOpen[cntLeds] = ERRORCNT;
					txData[cntLeds + 1] |= (LED_OPEN<<2);
					openState = 1;
				}
			}
			else if(currentLedVoltage.LedVoltage[cntLeds].ledChannelData[0] < GREEN_SHORTCUT_VOL)
			{
				cntgreenLedShortCut[cntLeds]++;
				if(cntgreenLedShortCut[cntLeds] >= ERRORCNT)
				{
					cntgreenLedShortCut[cntLeds] = ERRORCNT;
					txData[cntLeds + 1] |= (LED_SHORTCUT<<2);
					shortCutState = 1;
				}
			}
			else
			{
				cntgreenLedOpen[cntLeds] = 0;
				cntgreenLedShortCut[cntLeds] = 0;
				txData[cntLeds + 1] |= (LED_NORMAL<<2);
			}
		}

		//No detection when the led is turned off
		if((getLedAdcSampleState(cntLeds, 2) == 0)&&(shortCutState == 0)&&(openState == 0))
		{
			txData[cntLeds + 1] |= (LED_NORMAL<<4);
		}
		else
		{
			//blue led voltage
			if(currentLedVoltage.LedVoltage[cntLeds].ledChannelData[2] > BLUE_OPEN_VOL)
			{
				cntblueLedOpen[cntLeds]++;
				if(cntblueLedOpen[cntLeds] >= ERRORCNT)
				{
					cntblueLedOpen[cntLeds] = ERRORCNT;
					txData[cntLeds + 1] |= (LED_OPEN<<4);
					openState = 1;
				}
			}
			else if(currentLedVoltage.LedVoltage[cntLeds].ledChannelData[2] < BLUE_SHORTCUT_VOL)
			{
				cntblueLedShortCut[cntLeds]++;
				if(cntblueLedShortCut[cntLeds] >= ERRORCNT)
				{
					cntblueLedShortCut[cntLeds] = ERRORCNT;
					txData[cntLeds + 1] |= (LED_SHORTCUT<<4);
					shortCutState = 1;
				}
			}
			else
			{
				cntblueLedOpen[cntLeds] = 0;
				cntblueLedShortCut[cntLeds] = 0;
				txData[cntLeds + 1] |= (LED_NORMAL<<4);
			}
		}
	}

	txData[7] = moduleTempratureGetMcu() + 80;//adding 80 is to avoid negative numbers
	for(cntLeds = 0; cntLeds < RGBLEDMAX; cntLeds++)
	{
		txData[8 + cntLeds] = moduleTempratureGetLed(cntLeds)/DTEMP_MULTIPLIED_RATIO + 80;//adding 80 is to avoid negative numbers
	}

	if(Get_Transmit_Lock() == 0)//if the transmit state is free
	{
		systemStatus[0] = 0x0200;
		for(i = 1; i < 15; i++)//The data transmission of the chip is in unit of word
		{
			systemStatus[i] = (txData[(i-1)*2]<<8) + txData[(i-1)*2 + 1];
		}
	}
}

void systemProtocolControl(SCommMessage *Message, SColorParams *params)
{
//	SLedColorData_t tempData;
//	uint16_t ledIndex;
//
//	for (ledIndex = 0; ledIndex < RGBLEDMAX; ledIndex++)
//	{
//		params->colorU[RGBLEDMAX - ledIndex - 1] = 203;
//		params->colorV[RGBLEDMAX - ledIndex - 1] = 463;
//		params->intensity[RGBLEDMAX - ledIndex - 1] = 4000;
//	}
//
//	modulePwmLedSpecialMask(0);

#if 1
	uint16_t ledIndex;

	switch (Message->func)
	{
    	case ECommFunctionLedColor:
    	{
#if 1
    		SLedColorData_t tempData;
    		uint16_t step = 0;

    		if (Message->msgLength > 6)
    			step = 4;
    		else
    			step = 0;

    		for (ledIndex = 0; ledIndex < RGBLEDMAX; ledIndex++)
    		{
    			uint8_t* dataPtr = Message->data;
    			tempData.rawData[0] = *(dataPtr+1+step*ledIndex);
    			tempData.rawData[1] = *(dataPtr+0+step*ledIndex);
    			tempData.rawData[2] = *(dataPtr+3+step*ledIndex);
    			tempData.rawData[3] = *(dataPtr+2+step*ledIndex);

    			params->colorU[RGBLEDMAX - ledIndex - 1] = tempData.color.u;
    			params->colorV[RGBLEDMAX - ledIndex - 1] = tempData.color.v;
    			params->intensity[RGBLEDMAX - ledIndex - 1] = tempData.color.L;
    		}

    		if((tempData.rawData[0] == 0)
    				&&(tempData.rawData[1] == 0)
					&&(tempData.rawData[2] == 0)
					&&(tempData.rawData[3] == 0))
    		{
    			if(shortCutState == 1)
				{
					systemStatus[0] = 0x0000;
					systemStatus[1] = 0x0000;
					systemStatus[2] = 0x0000;
					systemStatus[3] &= 0xFF00;
					ShortCutCntInit();
					shortCutState = 0;
				}
				if(openState == 1)
				{
					systemStatus[0] = 0x0000;
					systemStatus[1] = 0x0000;
					systemStatus[2] = 0x0000;
					systemStatus[3] &= 0xFF00;
					OpenCntInit();
					openState = 0;
				}
    		}
    		else
    		{

    		}

    		modulePwmLedSpecialMask(0);
//      ClearMeliubuBuffer();

#endif
    	}
    	break;
		case ECommFunctionCalLed:
		{
			modulePwmLedSpecialMask(Message->data[0]);
		}
		break;
		case ECommFunctionCalData:
		{
			uint8_t ledChannel;
			uint8_t ledCalMask, index;
			uint8_t* dataPtr = &Message->data[2];
			ledCalMask = Message->data[0];
			for (index = 0; index < 6; index++)
			{
				if (ledCalMask & (0x1<<index))
				{
					savedConfig.redx[5-index] = dataPtr[1] | (dataPtr[0]<<8);
					savedConfig.redy[5-index] = dataPtr[3] | (dataPtr[2]<<8);
					savedConfig.redY[5-index] = (uint32_t)dataPtr[5] | ((uint32_t)dataPtr[4]<<8) | ((uint32_t)dataPtr[7]<<16) | ((uint32_t)dataPtr[6]<<24);
					savedConfig.greenx[5-index] = dataPtr[9] | (dataPtr[8]<<8);
					savedConfig.greeny[5-index] = dataPtr[11] | (dataPtr[10]<<8);
					savedConfig.greenY[5-index] = (uint32_t)dataPtr[13] | ((uint32_t)dataPtr[12]<<8) | ((uint32_t)dataPtr[15]<<16) | ((uint32_t)dataPtr[14]<<24);
					savedConfig.bluex[5-index] = dataPtr[17] | (dataPtr[16]<<8);
					savedConfig.bluey[5-index] = dataPtr[19] | (dataPtr[18]<<8);
					savedConfig.blueY[5-index] = (uint32_t)dataPtr[21] | ((uint32_t)dataPtr[20]<<8) | ((uint32_t)dataPtr[23]<<16) | ((uint32_t)dataPtr[22]<<24);

					for(ledChannel = 0; ledChannel < 3; ledChannel++)
					{
						savedConfig.LedVoltage_25Celsius.LedVoltage[index].ledChannelData[ledChannel] = currentLedVoltage.LedVoltage[index].ledChannelData[ledChannel];
					}
				}
				else
				{
				}
			}

			for (index = 0; index < RGBLEDMAX; index++)
			{
				algoUpdateRedCIE(index, savedConfig.redx[index], savedConfig.redy[index], savedConfig.redY[index]);
				algoUpdateGreenCIE(index, savedConfig.greenx[index], savedConfig.greeny[index], savedConfig.greenY[index]);
				algoUpdateBlueCIE(index, savedConfig.bluex[index], savedConfig.bluey[index], savedConfig.blueY[index]);
			}

			if (flashFlag == EFlashUpdateFree)
				flashFlag |= EFlashUpdateProcessing;

//      if (ledCalMask == 0)
//      {
//		  if (flashFlag == EFlashUpdateFree)
//			flashFlag |= EFlashUpdateProcessing;
//      }

//      ClearMeliubuBuffer();
		}
		break;

		case ECommFunctionRequestCalData:
		{
			uint8_t index;
			uint8_t caldata[30] = {0};
			uint8_t i = 0;
			index = Message->data[0];

			caldata[0] = 0x01;
			caldata[1] = index;
			caldata[2] = savedConfig.redx[5-index];
			caldata[3] = savedConfig.redx[5-index]>>8;
			caldata[4] = savedConfig.redy[5-index];
			caldata[5] = savedConfig.redy[5-index]>>8;
			caldata[6] = savedConfig.redY[5-index];
			caldata[7] = savedConfig.redY[5-index]>>8;
			caldata[8] = savedConfig.redY[5-index]>>16;
			caldata[9] = savedConfig.redY[5-index]>>24;
			caldata[10] = savedConfig.greenx[5-index];
			caldata[11] = savedConfig.greenx[5-index]>>8;
			caldata[12] = savedConfig.greeny[5-index];
			caldata[13] = savedConfig.greeny[5-index]>>8;
			caldata[14] = savedConfig.greenY[5-index];
			caldata[15] = savedConfig.greenY[5-index]>>8;
			caldata[16] = savedConfig.greenY[5-index]>>16;
			caldata[17] = savedConfig.greenY[5-index]>>24;
			caldata[18] = savedConfig.bluex[5-index];
			caldata[19] = savedConfig.bluex[5-index]>>8;
			caldata[20] = savedConfig.bluey[5-index];
			caldata[21] = savedConfig.bluey[5-index]>>8;
			caldata[22] = savedConfig.blueY[5-index];
			caldata[23] = savedConfig.blueY[5-index]>>8;
			caldata[24] = savedConfig.blueY[5-index]>>16;
			caldata[25] = savedConfig.blueY[5-index]>>24;

			if(Get_Transmit_Lock() == 0)
			{
				for(i = 0; i < 15; i++)
				{
					ledCalData[i] = (caldata[i*2]<<8) + caldata[i*2 + 1];
				}
			}
			//Change the DMA TX pointer to the led calibration data buffer
			IO_SET(PORT_DMA_TX, DMA_TX, (uint16_t)((uint16_t *)&ledCalData[0]));
		}
		break;

		case ECommFunctionRequestLedStatus:
		{
			//Change the DMA TX pointer to the led status and temperature data buffer
			IO_SET(PORT_DMA_TX, DMA_TX, (uint16_t)((uint16_t *)&systemStatus[0]));
		}
		break;

		case ECommFunctionAutoAddressing:
			break;
		case ECommFunctionEnterProgrammingMode:
			break;
		case ECommFunctionNone:
			break;
		default:
		  break;
	}
#endif
}
