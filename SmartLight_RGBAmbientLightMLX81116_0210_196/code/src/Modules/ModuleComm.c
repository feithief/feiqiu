/*
 * comm.c
 *
 *  Created on: 2019年6月9日
 *      Author: SH1705016
 */

#include "../Modules/ModuleComm.h"
#include <bl_melibus.h>
#include <bl_tables.h>
#include <bl_bist.h>
#include <bl_tools.h>
#include <atomic.h>
#include <hal_melibus.h>
#include "ModuleFlash.h"
#include "ModulePwm.h"
#include "ModulePower.h"
#include "SystemControl.h"

///** CRC u16_sTable */
//static const uint16_t u16_sTable[256] =
//{
//   0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
//   0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
//   0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52B5U, 0x4294U, 0x72F7U, 0x62D6U,
//   0x9339U, 0x8318U, 0xB37BU, 0xA35AU, 0xD3BDU, 0xC39CU, 0xF3FFU, 0xE3DEU,
//   0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64E6U, 0x74C7U, 0x44A4U, 0x5485U,
//   0xA56AU, 0xB54BU, 0x8528U, 0x9509U, 0xE5EEU, 0xF5CFU, 0xC5ACU, 0xD58DU,
//   0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76D7U, 0x66F6U, 0x5695U, 0x46B4U,
//   0xB75BU, 0xA77AU, 0x9719U, 0x8738U, 0xF7DFU, 0xE7FEU, 0xD79DU, 0xC7BCU,
//   0x48C4U, 0x58E5U, 0x6886U, 0x78A7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
//   0xC9CCU, 0xD9EDU, 0xE98EU, 0xF9AFU, 0x8948U, 0x9969U, 0xA90AU, 0xB92BU,
//   0x5AF5U, 0x4AD4U, 0x7AB7U, 0x6A96U, 0x1A71U, 0x0A50U, 0x3A33U, 0x2A12U,
//   0xDBFDU, 0xCBDCU, 0xFBBFU, 0xEB9EU, 0x9B79U, 0x8B58U, 0xBB3BU, 0xAB1AU,
//   0x6CA6U, 0x7C87U, 0x4CE4U, 0x5CC5U, 0x2C22U, 0x3C03U, 0x0C60U, 0x1C41U,
//   0xEDAEU, 0xFD8FU, 0xCDECU, 0xDDCDU, 0xAD2AU, 0xBD0BU, 0x8D68U, 0x9D49U,
//   0x7E97U, 0x6EB6U, 0x5ED5U, 0x4EF4U, 0x3E13U, 0x2E32U, 0x1E51U, 0x0E70U,
//   0xFF9FU, 0xEFBEU, 0xDFDDU, 0xCFFCU, 0xBF1BU, 0xAF3AU, 0x9F59U, 0x8F78U,
//   0x9188U, 0x81A9U, 0xB1CAU, 0xA1EBU, 0xD10CU, 0xC12DU, 0xF14EU, 0xE16FU,
//   0x1080U, 0x00A1U, 0x30C2U, 0x20E3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
//   0x83B9U, 0x9398U, 0xA3FBU, 0xB3DAU, 0xC33DU, 0xD31CU, 0xE37FU, 0xF35EU,
//   0x02B1U, 0x1290U, 0x22F3U, 0x32D2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
//   0xB5EAU, 0xA5CBU, 0x95A8U, 0x8589U, 0xF56EU, 0xE54FU, 0xD52CU, 0xC50DU,
//   0x34E2U, 0x24C3U, 0x14A0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
//   0xA7DBU, 0xB7FAU, 0x8799U, 0x97B8U, 0xE75FU, 0xF77EU, 0xC71DU, 0xD73CU,
//   0x26D3U, 0x36F2U, 0x0691U, 0x16B0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
//   0xD94CU, 0xC96DU, 0xF90EU, 0xE92FU, 0x99C8U, 0x89E9U, 0xB98AU, 0xA9ABU,
//   0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18C0U, 0x08E1U, 0x3882U, 0x28A3U,
//   0xCB7DU, 0xDB5CU, 0xEB3FU, 0xFB1EU, 0x8BF9U, 0x9BD8U, 0xABBBU, 0xBB9AU,
//   0x4A75U, 0x5A54U, 0x6A37U, 0x7A16U, 0x0AF1U, 0x1AD0U, 0x2AB3U, 0x3A92U,
//   0xFD2EU, 0xED0FU, 0xDD6CU, 0xCD4DU, 0xBDAAU, 0xAD8BU, 0x9DE8U, 0x8DC9U,
//   0x7C26U, 0x6C07U, 0x5C64U, 0x4C45U, 0x3CA2U, 0x2C83U, 0x1CE0U, 0x0CC1U,
//   0xEF1FU, 0xFF3EU, 0xCF5DU, 0xDF7CU, 0xAF9BU, 0xBFBAU, 0x8FD9U, 0x9FF8U,
//   0x6E17U, 0x7E36U, 0x4E55U, 0x5E74U, 0x2E93U, 0x3EB2U, 0x0ED1U, 0x1EF0U
//};

/**
 * @details    CRC16 CCITT calculator
 * @param[in]  *bytes
 * @param[in]  len
 * @return     ccitt value
 */
//uint16_t CRC16_CCITT(const uint8_t *bytes, uint32_t len)
//{
//   uint16_t value_ = 0xFFFF; //初始值，根据CRC类型设定
//
//   while(len--)
//   {
//      value_ = ((value_ << 8) ^ u16_sTable[(uint16_t)((value_ >> 8) ^ *bytes) & 0xFFU]) & 0xFFFFU;
//
//      bytes++;
//   }
//
//   return value_;
//}

typedef struct MeliBuIDStruct {
    uint16_t parity_0 : 1;
    uint16_t parity_1 : 1;
    uint16_t instrExt_SubAdr : 6;
    uint16_t functionType : 1;
    uint16_t readWrite : 1;
    uint16_t nodeAddress : 6;
} MeliBuID_t;

typedef union _MeLiBuKBDData_u {
    uint8_t rawdatabyte[40];
    uint16_t rawdataword[20];

    struct MeliBuDataStruct {
        MeliBuID_t melibuID __attribute__((packed));
        uint8_t protocolBuffer[RGBLEDMAX*6];
        uint16_t S2M_CRC; /* in case of S2M Message, RX buffer will be filled TX Data and CRC - avoid overflow of buffer */
    }MelData __attribute__((packed));
}MeLiBuKBDData_u;

#define MELBUFFERA 1u

#define MASKMELIBUERROR 0x00F8u
#define REQUESTLEDBUFFERUPDATE 1u
#define REQUESTSTATEBUFFERUPDATE 2u

/* Default MeLiBus ID for boot loader mode */
#define MEL_DEFAULT_ADDRESS          0x0Au
#if (FPLL<=16000)
#define MEL_DEFAULT_BAUDRATE_CFG     0u
#else
#define MEL_DEFAULT_BAUDRATE_CFG     1u
#endif

MeLiBuKBDData_u meLiBuBufferA = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
MeLiBuKBDData_u meLiBuBufferB = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

/* melibu transmit buffer for VS-voltage, temperature and eror flag handling */
volatile uint16_t meLiBuStateTransmit[9] = {0x0000,0x0000,0x0000,
                                            0x0000,0x6463,0x6261,
                                            0x605F,0x5e00,0x0000};

/* flag to signalize ongoing melibu transmission */
volatile uint16_t meLiBuLEDTransmitLock = 0;

/* flag to signalize new received data from Melibu */
volatile uint16_t melNewData = 0;

/* frame type */
volatile uint16_t frameType = 0;

/* flag to get auto-addressing state for aaout */
volatile uint8_t aaoutflag = 0;

/* auto-addressing transmit buffer */
volatile uint16_t meLiBuAATransmit[3] = {0xFC00,0x0000,0x0000};

/* store flag for EEPROM write command */
volatile uint16_t eeWriteIDFlag = 0;

/* ----------------------------------------------------------------------------
 * handle MeLiBu auto-addressing
 */
static INLINE void Mel_CmdAutoAddressing(uint8_t slaveAddress){
    if (slaveAddress < 4u) {
        /* If slave address == 0, 1, 2, 3 -> start auto addressing sequence */
        Mel_ClearAAoutPin();
//        Mel_SetNewNAD(BL_MEL_DEFAULT_ADDRESS);
        IO_SET(PORT_ID, ID, BL_MEL_DEFAULT_ADDRESS);
        meLiBuAATransmit[1] = ((uint16_t)(BL_MEL_DEFAULT_ADDRESS)&0x00FF);
    }
    else if (Mel_GetAAinStatus() != 0u) {
        /* if AA input pin is high and (NAD >= 4)  -> Set new address */
//    	Mel_SetNewNAD(slaveAddress);
        meLiBuAATransmit[1] = ((uint16_t)(slaveAddress)&0x00FF);
        IO_SET(PORT_ID, ID, slaveAddress);
        savedConfig.nad = slaveAddress;
	    Set_Last_Address(slaveAddress);
	    Auto_Addressing_Write_NAD_TO_EEPROM(slaveAddress);
	    aaoutflag = 1u;
    }
    else {
        /*  Waiting for high level at AIN pin */
    }
}

/* Melibus hardware configuration */
const MelConfiguration_t MelConfigurationDefault = {
    .melBaudRate = 1,
    .melAddress = MEL_DEFAULT_ADDRESS,       /* Default address for boot loader mode */
    .melAddressMask = 0u,                       /* Address masking is disabled for boot loader mode */
    .melIdDummy = 0u,
    .melDmaRda = (uint16_t)((uint8_t *)&meLiBuBufferA.rawdatabyte[0]),
    .melDmaRdb = (uint16_t)((uint8_t *)&meLiBuBufferB.rawdatabyte[0]),
	.melDmaTx  = (uint16_t)((uint16_t *)&systemStatus[0]),
    .melCtrlEn = 0u,        /* Note: Melibus hardware should be disabled during configuration. Mel_Start function must be called to start MeLiBus HW */
    .melCtrlAckEn = 0u,
    .melCtrlDisParity = 0u,
    .melCtrlDisCrc = 0u,
    .melCtrlSelTxd = MEL_SEL_TX_OUTPUT,
    .melCtrlSelRxIn = MEL_SEL_RX_INPUT,
    .melCtrlDummy = 0u
};

const MelInterruptConfiguration_t MelInterruptConfigurationDefault = {
    .melErrorInterruptEnable = 1u,
    .melFrameTransmittedInterruptEnable = 1u,
    .melFrameHeaderReceivedInterruptEnable = 1u,
    .melFrameReceivedInterruptEnable = 1u,
    .melTxTimeoutInterruptEnable = 1u,
    .melErrorInterruptPriority = 0u,     /*HW increases value with +3 */
    .melFrameTransmittedInterruptPriority = 2u,    /*HW increases value with +3 */
    .melFrameHeaderReceivedInterruptPriority = 1u,    /*HW increases value with +3 */
    .melFrameReceivedInterruptPriority = 0u,    /*HW increases value with +3 */
    .melTxTimeoutInterruptPriority = 3u    /*HW increases value with +3 */
};

void moduleCommInit(void)
{
  /* configure MeLiBus */
  Mel_Init((MelConfiguration_t*)&MelConfigurationDefault);
  Mel_ConfigureInterrupts(MelInterruptConfigurationDefault);
#if defined (__MLX81116xAA__)
  Mel_Start();
#elif defined (__MLX81116xAC__)
  IO_SET(UART, REE, 1, TRE, 1);
  IO_SET(PORT_MELIBUS_CTRL, MELIBUS_EN, 1);
  IO_SET(PORT_MISC2_OUT, ENABLE_TX, 1, AOUT_SUP, 0, EN_IN_AAIN, 1);
#endif

      /* set new node address from EEPROM */
#if defined (__MLX81116xAA__)
      /* set new node address from EEPROM */
      Mel_SetNewNAD(savedConfig.nad);
#elif defined (__MLX81116xAC__)
      /* Stop MeLiBu HW before loading new NAD */
      Mel_Stop();
      /* Load new NAD */
      IO_SET(PORT_ID, ID, savedConfig.nad);
      /* Restart MeLiBu HW after loading new NAD */
      IO_SET(UART, REE, 1, TRE, 1);
      IO_SET(PORT_MELIBUS_CTRL, MELIBUS_EN, 1);
      IO_SET(PORT_MISC2_OUT, ENABLE_TX, 1, AOUT_SUP, 0, EN_IN_AAIN, 1);
#endif
}

bool_t moduleCommFlagUpdated(void)
{
  if (melNewData == 1)
    return btrue;
  else
    return bfalse;
}

void moduleCommFlagClear(void)
{
  ENTER_SECTION(ATOMIC_KEEP_MODE);
  melNewData = 0;
  EXIT_SECTION();
}

/* store flag for EEPROM write command */
//static volatile uint16_t eeWriteIDFlag = 0;

static ECommFunction moduleCommGetFunc(MeLiBuKBDData_u* ptrMelBuffer, SCommMessage *Message)
{
  uint8_t instrCode = 0, counter = 0;
  uint8_t lenIndex = 0;
  ECommFunction result = ECommFunctionNone;

  if (ptrMelBuffer == (MeLiBuKBDData_u*)0)
	  return ECommFunctionNone;

  instrCode = ptrMelBuffer->MelData.melibuID.instrExt_SubAdr;

  if(ptrMelBuffer->MelData.melibuID.functionType == 1)
  {
	  for (lenIndex = 0; lenIndex < 6; lenIndex++)
	  {
		  if (instrCode & (1<<lenIndex))
			  counter++;
		  else
		  {}
	  }
  }
  else if(ptrMelBuffer->MelData.melibuID.functionType == 0)
  {
	  for (lenIndex = 0; lenIndex < 3; lenIndex++)
	  {
		  if (instrCode & (1<<lenIndex))
			  counter++;
		  else
		  {}
	  }
  }

  Message->msgLength = counter*6;

  if (ptrMelBuffer->MelData.melibuID.functionType == 1)
  {
	 switch(ptrMelBuffer->MelData.protocolBuffer[1])
	 {
	     case 0x00:
	     {
	    	 Message->func = ECommFunctionLedColor;
			 Message->data = &ptrMelBuffer->MelData.protocolBuffer[2];
			 return ECommFunctionLedColor;
	     }
	     break;

	     case 0xAA:
	     {
	    	 Message->func = ECommFunctionCalData;
			 Message->data = &ptrMelBuffer->MelData.protocolBuffer[0];
			 return ECommFunctionCalData;
	     }
		 break;

	     default:
	     break;
	 }
  }
  else
  {
    if(((instrCode>>3)&0x07) == 0x00)//KBD Function
    {
      if (ptrMelBuffer->MelData.melibuID.readWrite == 0)//RT == 0 Receive
      {
    	  switch(ptrMelBuffer->MelData.protocolBuffer[1])
    	  {
			  case 0x00:
			  {
				  Message->func = ECommFunctionCalLed;
				  Message->data = &ptrMelBuffer->MelData.protocolBuffer[0];
				  return ECommFunctionCalLed;
			  }
			  break;

			  case 0x01:
			  {
				  Message->func = ECommFunctionRequestCalData;
				  Message->data = &ptrMelBuffer->MelData.protocolBuffer[0];
				  return ECommFunctionRequestCalData;
			  }
			  break;

			  case 0x02:
			  {
				  Message->func = ECommFunctionRequestLedStatus;
				  return ECommFunctionRequestLedStatus;
			  }
			  break;

			  default:
			  break;
    	  }
      }
    }
    else//MELEXIS Function
    {
      if (((instrCode>>3)&0x07) == 0x07)
      {
        /* check if enter programming mode message has been received */
        if((ptrMelBuffer->rawdataword[1]==0x00FEu) && (ptrMelBuffer->rawdataword[2]==0xDEADu) && \
           (ptrMelBuffer->rawdataword[3]==0xBEEFu))
        {
        	result = ECommFunctionEnterProgrammingMode;
			Message->func = ECommFunctionEnterProgrammingMode;

            /* EPM received -> set programming mode state and reset */
            bl_table.NAD = IO_GET(PORT_ID, ID);
            bl_table.key = BL_TABLE_KEY;
            MLX16_RESET_SIGNED(C_CHIP_STATE_CMD_EPM);
        }
        else
        {
            if(ptrMelBuffer->rawdataword[1]==0x00FCu)
            {
            	result = ECommFunctionAutoAddressing;
            	Message->func = ECommFunctionAutoAddressing;
                /* Auto-addressing */
                eeWriteIDFlag = (uint8_t)(ptrMelBuffer->rawdataword[3]&0x00FFu);

                IO_SET(PORT_DMA_TX, DMA_TX, (uint16_t)((uint16_t *)&meLiBuAATransmit[0]));
                Mel_CmdAutoAddressing((uint8_t)(ptrMelBuffer->rawdataword[2]&0x00FFu));
            }
            if(ptrMelBuffer->rawdataword[1]==0x00C0u)
            {
                /* sleep Mode */
            	modulePowerEnterSleep();
            }
        }
      }
      else
      {}
    }
  }

  return result;
}

bool_t moduleCommGetData(SCommMessage *Message)
{
  MeLiBuKBDData_u* ptrMelBuffer;

  /* check for valid dataBuffer */
  if(IO_GET(PORT_MELIBUS_STAT,RD_BUFFER_VALID) == MELBUFFERA)
  {
      ptrMelBuffer = (MeLiBuKBDData_u*)&meLiBuBufferA.rawdataword[0];
  }
  else
  {
      ptrMelBuffer = (MeLiBuKBDData_u*)&meLiBuBufferB.rawdataword[0];
  }

  if (moduleCommGetFunc(ptrMelBuffer, Message) == ECommFunctionNone)
	  return bfalse;
  else
	  return btrue;
}

uint16_t Get_Transmit_Lock(void)
{
	return meLiBuLEDTransmitLock;
}

/* Frame transmitted interrupt */
__attribute__((interrupt)) void  _MFT_INT(void){
    /* copy transmit buffer if requested */

   // uint16_t i = 0; /* local variable for loop */

    /* release buffer lock -> transmit buffer change allowed */
    meLiBuLEDTransmitLock = 0;

//    /* check for Melibu error (CRC or parity) */
//    if(((IO_HOST(PORT_MELIBUS_STAT,RD_BUFFER_VALID) & (MASKMELIBUERROR))==0)) {
//        /* check if error has been reported successfully */
//        if(frameType == 1) {
//            meLiBuStateTransmit[0][2] = 0; /* reset error */
//        }
//    }
//    else {
//        /* get MeLiBu error */
//        meLiBuStateTransmit[0][2] = (IO_HOST(PORT_MELIBUS_STAT,RD_BUFFER_VALID) & (MASKMELIBUERROR));
//    }

    if(aaoutflag == 1)
    {
    	/* reset flag */
    	aaoutflag = 0;

    	/* Set AAOUT pin high */
    	Mel_SetAAoutPin();

    	if(eeWriteIDFlag == 1)
    	{
    		ENTER_SECTION(ATOMIC_KEEP_MODE);
    		/* clear write flag */
    		eeWriteIDFlag = 0;
    		EXIT_SECTION();

    		if (flashFlag == EFlashUpdateFree)
    			flashFlag |= EFlashUpdateProcessing;
    	}
    }
}
/* Frame received interrupt */
__attribute__((interrupt)) void  _MFR_INT(void){
    /* signalize that new data have been received */
    melNewData = 1;
}

/* Header received interrupt */
__attribute__((interrupt)) void  _MHR_INT(void){
    /* signalize to application that transmit buffer must not be changed */
    meLiBuLEDTransmitLock = 1;
}

/* Frame error interrupt */
__attribute__((interrupt)) void  _MER_INT(void){
    /* Melibu error handler */

    /* read 16 Bit status port and mask error flags for crc and parity */
    meLiBuStateTransmit[0] = (IO_HOST(PORT_MELIBUS_STAT,RD_BUFFER_VALID) & (MASKMELIBUERROR));
}

/* txd time out error */
__attribute__((interrupt)) void  _TX_TIMEOUT_INT(void){
    /* provide handler here */
}

//void ClearMeliubuBuffer(void)
//{
//	uint8_t i = 0;
//
//	if(IO_GET(PORT_MELIBUS_STAT,RD_BUFFER_VALID) == MELBUFFERA)
//	{
//	    for(i = 0; i < 40; i++)
//	    {
//	    	meLiBuBufferA.rawdatabyte[i] = 0;
//	    }
//	}
//	else
//	{
//	    for(i = 0; i < 40; i++)
//	    {
//	      	meLiBuBufferB.rawdatabyte[i] = 0;
//	    }
//	}
//}
