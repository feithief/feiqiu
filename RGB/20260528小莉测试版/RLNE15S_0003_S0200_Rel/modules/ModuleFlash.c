/*
************************************************************************
* Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
* All Rights Reserved
* Filename:		ModuleFlash.c
* Project:		RLNE15S
* Description:	This module provides functions for managing flash memory operations,
*               including saving, loading, and validating configuration data.
* Note(s):		None.
* Update Record:	
* Version     	Date     	 Modified by     Why & What is modified
* 00.01      2017.10.24       Leo Song           Initial Version
* 00.02      2024.10.12       Sen Gao            Code standardization 
************************************************************************
*/


/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
#include "Module1000TP_Test.h"
#include "ModuleFlash.h"
#include "flash.h"
#include <string.h>
#include <stdbool.h>


/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/
/**********************************************************************************************************************
  GLOBAL VARIABLES
  **********************************************************************************************************************/
/*!< MAGIC number as well as version NO >*/
#define DMAGIC_NUM  (555U)
/*!< Code flash size >*/
#define CODE_FLASH_SIZE      (0x7600U)
/*!< Data flash start address >*/
#define FLASH_START_ADDRESS     (UC_FLASH_START + CODE_FLASH_SIZE)
/*!< Data flash size, we not used in RLNE15S >*/
//#define DATA_FLASH_SIZE      (UC_FLASH_SIZE - CODE_FLASH_SIZE)
/*!< Data flash size of a page >*/
#define FLASH_PAGE_SIZE (64U)
/*!< Current flash's content >*/
ST_FLASH_CONTENT g_st_SavedConfig;
/*!< Current flash's status >*/
volatile uint8_t g_u8_FlashFlag = 0;;
/*!< Product information >*/
ST_LINE_CONTENT g_st_SavedProduct;
/*!< Last NAD >*/
uint16_t g_u16_LastSavedSingalAdress;
/**********************************************************************************************************************
  GLOBAL VARIABLES
**********************************************************************************************************************/


/**
* @brief 		Read a byte from data flash.
* @note 		void
* @param[in]	Reads a single byte from the data flash memory, starting from a given offset.                         
* @return		void
* @retval		None
* @par  		Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
static uint8_t ModuleFlash_ByteRead(uint16_t u16_AddrOffset)
{
    return *(uint8_t *)(FLASH_START_ADDRESS + u16_AddrOffset);
}


/**
 * @brief   Write configuration data to a specified flash memory address.
 * @param   startAddress: The address to start writing data.
 * @param   configData: Pointer to the configuration data to be written.
 * @param   size: The size of the data to write.
 * @retval  None.
 */
static void ModuleFlash_WriteConfigToFlash(uint32_t startAddress, const void* configData, size_t size) 
{
    // Write data to flash memory
    user_nvm_write(startAddress, (uint8_t *)configData, size, NVM_OPTIONS_RETRY_MASK);
}


/**
* @brief      Load current flash content to RAM.
* @note       This function reads the flash content byte by byte and stores it in the provided structure in RAM. 
* @param[out] st_FlashData Pointer to the structure where the flash data will be loaded.
* @return     None
* @retval     None
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
static void ModuleFlash_Load(ST_FLASH_CONTENT  *st_FlashData)
{
    uint16_t u16_FlashDataSize = sizeof(ST_FLASH_CONTENT);
    uint16_t u16_Addr = 0;
	
	// Read each byte from flash memory and store it in the st_FlashData structure
    for(u16_Addr=0; u16_Addr < u16_FlashDataSize; u16_Addr++)
    {
        *((uint8_t*)st_FlashData + u16_Addr) = ModuleFlash_ByteRead(u16_Addr);
    }
    
    /*ledSupplier & 0x00f0u == 0x0080 means flash content has been locked, */
    /*can not be changed in any circumstances*/
    if ((g_st_SavedConfig.u16_LedSupplier & 0x00f0u) ==  0x0080u)
    {	
		// Set the flash locked flag
		g_u8_FlashFlag |= (uint8_t)FLASH_UPDATE_LOCKED;

    }
    else
    {
		// Clear the flash locked flag
        g_u8_FlashFlag &= ~((uint8_t)FLASH_UPDATE_LOCKED);
    }
}


/**
* @brief      Erase and write data to the flash memory.
* @note       This function erases a page in the data flash memory and writes data in two parts, 
				ensuring that the entire configuration is saved across two flash pages.
* @param      None
* @return     None
* @retval     None
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
		00.03       2024.10.29      Genhua Yang
**/
static void ModuleFlash_MemoryEraseAndWrite(void) 
{   
    uint8_t au8_Page2Data[FLASH_PAGE_SIZE];
    uint16_t u16_ConfigSize = sizeof(ST_FLASH_CONTENT);
    uint16_t u16_Page2Len = 0u;
    uint16_t u16_Index = 0u;

    // Erase the specified flash memory area
    user_nvm_erase(FLASH_START_ADDRESS, NVM_ERASE_PAGE);
    user_nvm_erase(FLASH_START_ADDRESS + FLASH_PAGE_SIZE, NVM_ERASE_PAGE);
    
    // Write the first part of g_st_SavedConfig to the first flash page
    ModuleFlash_WriteConfigToFlash(FLASH_START_ADDRESS, &g_st_SavedConfig, FLASH_PAGE_SIZE);
    
    for (u16_Index = 0u; u16_Index < FLASH_PAGE_SIZE; u16_Index++)
    {
        au8_Page2Data[u16_Index] = 0xFFu;
    }

    if (u16_ConfigSize > FLASH_PAGE_SIZE)
    {
        u16_Page2Len = (uint16_t)(u16_ConfigSize - FLASH_PAGE_SIZE);
        if (u16_Page2Len > FLASH_PAGE_SIZE)
        {
            u16_Page2Len = FLASH_PAGE_SIZE;
        }

        for (u16_Index = 0u; u16_Index < u16_Page2Len; u16_Index++)
        {
            au8_Page2Data[u16_Index] = *((uint8_t *)&g_st_SavedConfig + FLASH_PAGE_SIZE + u16_Index);
        }
    }

    // Write the second part of g_st_SavedConfig to the second flash page
    ModuleFlash_WriteConfigToFlash(FLASH_START_ADDRESS + FLASH_PAGE_SIZE,
                       au8_Page2Data,
                       FLASH_PAGE_SIZE);
}

/**
 * @brief 	Calculates the CRC-16 checksum for a given data frame.
 * @note  	This function computes the CRC-16 checksum using a polynomial algorithm
 * 		  	on the provided data frame and its length.
 *
 * @param 	pucFrame Pointer to the data frame to calculate CRC for.
 * @param 	usLen Length of the data frame.
 * @return  The computed CRC-16 checksum.
 * @retval  The computed CRC-16 checksum.
 * @par  	  Edition History
		Version Number   	Time    	 Author
			00.01		2017.10.24		Leo Song
			00.02       2024.10.12       Sen Gao
 */
static uint16_t ModuleFlash_Crc16Calculate(const uint8_t* pu8_FrameAddr, uint16_t u16_Len)
{
	uint8_t u8_CrcHi = 0xFF;
	uint8_t u8_CrcLo = 0xFF;
	uint16_t u16_Index = 0;
	
	while ((u16_Len--) >0 )
	{
		// Calculate the index for the CRC lookup
		u16_Index = (uint16_t)(u8_CrcLo ^ *(pu8_FrameAddr++));
		// Update the low byte of the CRC
		u8_CrcLo = (uint8_t)(u8_CrcHi ^ g_au8_CrcHi[u16_Index]);
		// Update the high byte of the CRC
		u8_CrcHi = g_au8_CrcLo[u16_Index];
	}
	
	return (uint16_t)(((uint16_t)u8_CrcHi << 8u) | u8_CrcLo);
}


/**
* @brief      Save current RAM content to flash.
* @note       This function first loads the existing flash content into a temporary structure, 
				then erases and writes the updated configuration to flash memory. 
				It also checks if the flash content is locked and updates the flash flag accordingly.
* @param      None
* @return     None
* @retval     None
* @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
**/
void ModuleFlash_Save(void) 
{
	bool_t u8_WriteSuccessful = bfalse;
	uint16_t u16_DataSize = 0;
	ST_FLASH_CONTENT *savedconfigptr = &g_st_SavedConfig;
	ST_FLASH_CONTENT st_FlashReadData;
	u16_DataSize = sizeof(ST_FLASH_CONTENT);

    // Calculate CRC for the configuration, skipping the first 2 bytes
    g_st_SavedConfig.u16_Crc = ModuleFlash_Crc16Calculate((uint8_t*)&g_st_SavedConfig + 2, u16_DataSize - 2);

    (void)moduleWatchdogFeed();

	// Attempt to write to flash until successful
    while (u8_WriteSuccessful != btrue) 
	{
        // Erase and write to flash memory
        ModuleFlash_MemoryEraseAndWrite();
        (void)moduleWatchdogFeed();

        // Load the written data from flash to verify
        ModuleFlash_Load(&st_FlashReadData);

        // Calculate CRC for the configuration, skipping the first 2 bytes
        uint16_t readCrc = ModuleFlash_Crc16Calculate((uint8_t*)&st_FlashReadData + 2, u16_DataSize - 2);

        // Check if the calculated CRC matches the saved CRC
        if (readCrc == g_st_SavedConfig.u16_Crc)
		{
			// Successful write if CRC matches
            u8_WriteSuccessful = btrue; 
        }
    }

    /*u16_LedSupplier & 0x00f0u == 0x0080 means flash content has been locked, */
    /*can not be changed in any circumstances*/
    if ((g_st_SavedConfig.u16_LedSupplier & 0x00f0u) == 0x0080u) {
        g_u8_FlashFlag |= (uint8_t)FLASH_UPDATE_LOCKED;
    } else {
        g_u8_FlashFlag &= ~((uint8_t)FLASH_UPDATE_LOCKED);
    }
}


/**
 * @brief 	  Validates the saved flash data.
 * @note 	  This function loads the saved configuration from flash memory,
 *       	  calculates its CRC, and checks if it matches the stored CRC.
 *       	  If they do not match, it increments the tamper count and returns 0.
 *       	  If they match, it returns 1, indicating the data is valid.
 * @param 	  void
 * @return 	  1 or 0
 * @retval 	  - 1 Data is valid.
 *    		  - 0 Data is invalid.
 * @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
 */
static uint8_t ModuleFlash_DataValidate(void) 
{
	uint16_t	DataSize = sizeof(ST_FLASH_CONTENT);
	uint16_t	u16_ReadCrc = 0;
	bool_t		u8_Ret = 0;
    

    // Calculate the CRC of the saved configuration, skipping the first 2 bytes(CRC in data flash)
    u16_ReadCrc = ModuleFlash_Crc16Calculate((uint8_t*)&g_st_SavedConfig + 2, DataSize - 2);

    // Check if the computed CRC matches the stored CRC
	if (u16_ReadCrc != g_st_SavedConfig.u16_Crc) 
	{
		g_st_SavedConfig.u16_Crc = u16_ReadCrc;
		// Increment tamper count
		g_st_SavedConfig.u16_TamperCount++; 
		// CRC mismatch, return false
		u8_Ret = bfalse; 
	}else
	{
		// CRC match, return true
		u8_Ret = btrue;
	}
    return u8_Ret;
}


/**
 * @brief 	  Initializes the flash module and configures settings.
 * @note 	  This function loads the saved configuration, validates it,
 *       	  and initializes default values if the configuration is invalid
 *       	  or if the magic number does not match.
 * @param 	  void
 * @return 	  void 
 * @retval    None
 * @par  	  Edition History
    Version Number   	Time    	 Author
		00.01		2017.10.24		Leo Song
		00.02       2024.10.12       Sen Gao
 */
void ModuleFlash_Init(void)
{
    uint8_t u8_1000tpInvalid = bfalse;

    Module1000TP_TestInit();
//	ModuleFlash_Load(&g_st_SavedConfig);
	Module1000TP_Load(&g_st_SavedConfig);
	
	// If 1000TP is invalid or half-written, fall back to data flash staging.
	if ( ModuleFlash_DataValidate() == 0)
	{
        u8_1000tpInvalid = btrue;
		ModuleFlash_Load(&g_st_SavedConfig);
        if ( ModuleFlash_DataValidate() == 0)
        {
            g_st_SavedConfig.u16_MagicNO = 0u;
        }
	}
	
	// Check if the magic number is valid; if not, set defaults
    if ( g_st_SavedConfig.u16_MagicNO != 0x5A ) 
    {
        memset(&g_st_SavedConfig, 0, sizeof(g_st_SavedConfig));
        g_st_SavedConfig.u16_MagicNO = 0x5A;
        g_st_SavedConfig.e_Platform = E_PLATFORM_TYPE_MLBEVOVW;
              
		g_st_SavedConfig.u16_LedSupplier	= 0x2;
		g_st_SavedConfig.u16_SingleAddr 	= 0x08;
		g_st_SavedConfig.u16_GroupAddr 		= 0x1;
		g_st_SavedConfig.u16_Factor 		= 0x8000;
        
		// Initialize color calibration values
		g_st_SavedConfig.u16_Redx 			= 6991;		
		g_st_SavedConfig.u16_Redy 			= 2999; 	
		g_st_SavedConfig.u32_RedY 			= 16850; 	
		g_st_SavedConfig.u16_Greenx 		= 1620; 	
		g_st_SavedConfig.u16_Greeny 		= 7300; 	
		g_st_SavedConfig.u32_GreenY 		= 36700; 	
		g_st_SavedConfig.u16_Bluex 			= 1555; 	
		g_st_SavedConfig.u16_Bluey 			= 247; 		
		g_st_SavedConfig.u32_BlueY 			= 5094; 

		// Initialize product details
		strncpy((char *)g_st_SavedConfig.au8_PartNo , "123456789012", DMAX_PART_NUMBER_LENGTH);
		strncpy((char *)g_st_SavedConfig.au8_HardwareVersion , "H01", DHARDWARE_VERSION_LENGTH);
		strncpy((char *)g_st_SavedConfig.au8_SerialNo , "12345678900987654321", DMAX_SERIAL_NUMBER_LENGTH);
        
        g_st_SavedConfig.s16_RThermolRatio 	= 60;
        g_st_SavedConfig.s16_GThermolRatio 	= 56;
        g_st_SavedConfig.s16_BThermolRatio 	= 20;
        
		// Initialize product ID
        g_st_SavedConfig.au8_ProductID[0] 	= 0x84;
        g_st_SavedConfig.au8_ProductID[1] 	= 0x00;
        g_st_SavedConfig.au8_ProductID[2] 	= 0x03;
        g_st_SavedConfig.au8_ProductID[3] 	= 0x00;
        g_st_SavedConfig.au8_ProductID[4] 	= 0x02;
        
        g_st_SavedConfig.s16_McuThermolRatio 	= 16;
        g_st_SavedConfig.s8_RatingLowerVoltage 	= 8;
        g_st_SavedConfig.s8_RatingHigherVoltage = 1;
		g_st_SavedConfig.u16_TamperCount = 0;
		g_st_SavedConfig.u16_Diag2ECount = 0;
		g_st_SavedConfig.u8_DebugSwitch[0] = 0x00;
		g_st_SavedConfig.u8_DebugSwitch[1] = 0x00;
		g_st_SavedConfig.u8_DebugSwitch[2] = 0x00;
        g_st_SavedConfig.u16_Crc = ModuleFlash_Crc16Calculate((uint8_t*)&g_st_SavedConfig + 2, sizeof(ST_FLASH_CONTENT) - 2);
        u8_1000tpInvalid = btrue;
	}

    g_st_SavedConfig.au8_ProductID[4] = 0x02;
    if (u8_1000tpInvalid == btrue)
    {
        g_st_SavedConfig.u16_Crc = ModuleFlash_Crc16Calculate((uint8_t*)&g_st_SavedConfig + 2, sizeof(ST_FLASH_CONTENT) - 2);
    }
    if (u8_1000tpInvalid == btrue)
    {
        Module1000TP_AdaptiveWriteTask();
    }
    g_u16_LastSavedSingalAdress = g_st_SavedConfig.u16_SingleAddr;
}

