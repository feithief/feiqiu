/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemLinDiag.c
 * Project:		RLNE15S
 * Description:	This file implements LIN diagnostic services and request 
 *               handling. It includes functionality for read/write operations,
 *               security access unlocking, and diagnostic request/response 
 *               communication. The file also defines service lists and related
 *               configurations for supported diagnostic operations.
 * Note(s):	- This module is designed to operate within the LIN stack
 *             and related diagnostic workflows.
 * Update Record:	
 * Version     	Date     	 Modified by       Changes Made
 * 00.01      2024.11.28      Genhua Yang      Initial version
 ************************************************************************
 */
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/
#include "SystemStatus.h"
#include "ModuleFlash.h"
#include "SystemLinDiag.h"
#include "ModuleTemperature.h"
#include "ModuleLed.h"
#include "ModuleBattery.h"
#include "lin_driver_api.h"
#include "lin_app.h"
/**********************************************************************************************************************
  INCLUDE FILES
 **********************************************************************************************************************/

/**@brief software VERSION section length.*/
#define DSOFTWARE_VERSION_LENGTH  5U

/**@brief module name section length.*/
#define DCONTENT_6C69_LEGNTH      13U
/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/
/*!< Stores the software version, 5 bytes in length */
static const uint8_t g_au8_SoftwareVersion[DSOFTWARE_VERSION_LENGTH + 1] = "S0200";    

/*!< Stores content related to identifier 0x6C69, 13 bytes */
static const uint8_t g_au8_Content0x6c69[DCONTENT_6C69_LEGNTH+1] = "RGBe_Slave   "; 

/*!< List of diagnostic services with associated configurations */
static const ST_LinDiagSevice g_st_ServeceList[] =
{
		{0x0002,  E_DS_READWRITE, E_NAD_CAL, 2u , (uint8_t*)&(g_st_SavedConfig.u16_LedSupplier)},
		{0x0003,  E_DS_READWRITE, E_NAD_CAL, 2u , (uint8_t*)&(g_st_SavedConfig.u16_SingleAddr)},
		{0x0004,  E_DS_READWRITE, E_NAD_CAL, 2u , (uint8_t*)&(g_st_SavedConfig.u16_GroupAddr)},
		{0x0005,  E_DS_READWRITE, E_NAD_CAL, 2u , (uint8_t*)&(g_st_SavedConfig.e_Platform)},
		{0x0006,  E_DS_READWRITE, E_NAD_CAL, 2u , (uint8_t*)&(g_st_SavedConfig.u16_Factor)},
		{0x0007,  E_DS_READWRITE, E_NAD_CAL, 8u , (uint8_t*)&(g_st_SavedConfig.u16_Redx)},
		{0x0008,  E_DS_READWRITE, E_NAD_CAL, 8u , (uint8_t*)&(g_st_SavedConfig.u16_Greenx)},
		{0x0009,  E_DS_READWRITE, E_NAD_CAL, 8u , (uint8_t*)&(g_st_SavedConfig.u16_Bluex)},
		{0x000A,  E_DS_WRITE,      E_NAD_CAL, 1u , (uint8_t*)&(g_e_LedControlFlag)},
		{0x000B,  E_DS_READWRITE, E_NAD_CAL, 17u , (uint8_t*)&(g_st_SavedProduct)}, 
		{0x000C,  E_DS_READWRITE, E_NAD_CAL, 24u , (uint8_t*)&(g_st_SavedConfig.u16_Redx)},
		{0x000D,  E_DS_READWRITE, E_NAD_CAL, 8u , (uint8_t*)&(g_st_SavedConfig.s16_RThermolRatio)},
		{0x6269,  E_DS_READWRITE, E_NAD_NORLCAL, DMAX_PART_NUMBER_LENGTH , (uint8_t*)&(g_st_SavedConfig.au8_PartNo[0])},
		{0x6469,  E_DS_READ,       E_NAD_NORLCAL, DSOFTWARE_VERSION_LENGTH , (uint8_t*)&(g_au8_SoftwareVersion[0])},
		{0x6669,  E_DS_READWRITE, E_NAD_NORLCAL, DMAX_PART_NUMBER_LENGTH , (uint8_t*)&(g_st_SavedConfig.au8_PartNo[0])},
		{0x6869,  E_DS_READWRITE, E_NAD_NORLCAL, DHARDWARE_VERSION_LENGTH , (uint8_t*)&(g_st_SavedConfig.au8_HardwareVersion[0])},
		{0x6A69,  E_DS_READWRITE, E_NAD_NORLCAL, DMAX_SERIAL_NUMBER_LENGTH, (uint8_t*)&(g_st_SavedConfig.au8_SerialNo[0])},
		{0x6B69,  E_DS_READ,       E_NAD_NORLCAL, 6u, (uint8_t*)&(g_au16_QuickCheck)},
		{0x6C69,  E_DS_READ,       E_NAD_NORL, DCONTENT_6C69_LEGNTH , (uint8_t*)&g_au8_Content0x6c69[0]},
		{0x6E69,  E_DS_READWRITE, E_NAD_CAL, 24u, (uint8_t*)&g_st_SavedConfig.u16_Redx},
		{0x6F69,  E_DS_READWRITE, E_NAD_CAL, 22u, (uint8_t*)&g_st_SavedConfig.u16_Factor},
		{0x6F79,  E_DS_READWRITE, E_NAD_CAL, 15u, (uint8_t*)&g_st_SavedConfig.au8_PartNo[0]},
		{0x0922,  E_DS_WRITE, E_NAD_CAL, 3u, (uint8_t*)&g_st_SavedConfig.u8_DebugSwitch[0]},
		
};

/**********************************************************************************************************************
  GLOBAL VARIABLES
 **********************************************************************************************************************/



//Task control block for handling current requests.
static ST_RequsetTCB g_st_CurrentRequestTcb = {0};
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
/**
 * @brief 		Read data from the specified service based on the diagnostic request.
 * @note 		This function reads data from the service address defined in the service list and copies it to the request control block (`pst_Tcb`).
 *               If `pu8_data` is `NULL`, it only updates the total length (`pu16_Length`) of the data available for the service.
 * @param[in,out]	pst_Tcb -- Pointer to the request control block that will store the read data.
 * @param[in]	u16_Index -- Index of the target service in the service list.
 * @param[out]	pu16_Length -- Pointer to the variable that will store the total length of the service data if `pu8_data` is `NULL`.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinDiag_ReadData(ST_RequsetTCB* pst_Tcb, uint16_t u16_Index, uint16_t* pu16_Length)
{
		/* Check if the data pointer in the request control block is not NULL */
		if (pst_Tcb->pu8_data != NULL)
		{
				/* Iterate through the requested data amount */
				for (uint16_t u16_Ccnt = 0; u16_Ccnt < pst_Tcb->u8_Amount; u16_Ccnt++)
				{
						/* Ensure the current position plus the offset is within the valid range of the service data length */
						if ((pst_Tcb->u8_Pos + u16_Ccnt) < g_st_ServeceList[u16_Index].u16_Length)
						{
								/* Copy the data from the service list to the request data buffer */
								pst_Tcb->pu8_data[u16_Ccnt] = g_st_ServeceList[u16_Index].addr[pst_Tcb->u8_Pos + u16_Ccnt];
						}
				}
		}
		else
		{
				/* If the data pointer is NULL, set the length to the service data length at the given index */
				*pu16_Length = g_st_ServeceList[u16_Index].u16_Length;
		}
}




/**
 * @brief 		Write data to the specified service based on the diagnostic request.
 * @note 		This function writes data from the request control block (`pst_Tcb`) to the service address defined in the service list.
 *               The amount of data written is determined by `u8_Amount`, ensuring it does not exceed the service length.
 * @param[in]	pst_Tcb -- Pointer to the request control block containing the data to be written.
 * @param[in]	u16_Index -- Index of the target service in the service list.
 * @return		None.
 * @retval		None.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static void SystemLinDiag_WriteData(const ST_RequsetTCB * const pst_Tcb, uint16_t u16_Index)
{
		/* Check if the data pointer in the request control block is not NULL */
		if (pst_Tcb->pu8_data != NULL)
		{
				/* Iterate through the requested data amount */
				for (uint16_t u16_Cntz = 0; u16_Cntz < pst_Tcb->u8_Amount; u16_Cntz++)
				{
						/* Ensure the current position plus the offset is within the valid range of the service data length */
						if ((pst_Tcb->u8_Pos + u16_Cntz) < g_st_ServeceList[u16_Index].u16_Length)
						{
								/* Write the data from the request data buffer to the service list */
								g_st_ServeceList[u16_Index].addr[pst_Tcb->u8_Pos + u16_Cntz] = *(pst_Tcb->pu8_data + u16_Cntz);
						}
				}
		}
		else
		{
			;
		}
}


/**
 * @brief 		Handle LIN diagnostic request by processing read or write operations.
 * @note 		This function iterates through the service list to find a matching service ID and validates the request based on 
 *               read/write attributes and NAD type. It then delegates the operation to `SystemLinDiag_ReadData` or `SystemLinDiag_WriteData`.
 * @param[in]	pst_Tcb -- Pointer to the request control block containing the request details.
 * @param[out]	pu16_Length -- Pointer to the response length, which will be updated for read operations.
 * @return		Operation result.
 * @retval		Returns 0 for success, 1 if no matching service is found or the request fails.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
static uint16_t SystemLinDiag_DoRequest(ST_RequsetTCB *pst_Tcb, uint16_t *pu16_Length)
{
		uint16_t u16_Ret = 1;  // Default return value indicating failure

		do {
				/* Iterate through the diagnostic service list to find a matching service */
				for (uint16_t u16_Index = 0; u16_Index < (sizeof(g_st_ServeceList) / sizeof(g_st_ServeceList[0])); u16_Index++)
				{
						/* Check if the service ID, read/write attribute, and NAD type match the request */
						if ((g_st_ServeceList[u16_Index].u16_ServiceID == pst_Tcb->u16_RequstDid) &&
										(((uint8_t)(g_st_ServeceList[u16_Index].e_RwAttr) & (uint8_t)(pst_Tcb->e_OperationType)) != 0) &&
										(((uint8_t)(g_st_ServeceList[u16_Index].e_NadAttr) & (uint8_t)(pst_Tcb->u16_NadType)) != 0))
						{
								u16_Ret = 0;  // Set return value to 0 indicating success

								/* Handle read request */
								if ((uint8_t)(pst_Tcb->e_OperationType) == (uint8_t)E_DS_READ)
								{
										SystemLinDiag_ReadData(pst_Tcb, u16_Index, pu16_Length);
								}
								/* Handle write request */
								else if ((uint8_t)(pst_Tcb->e_OperationType) == (uint8_t)E_DS_WRITE)
								{
										/* Special handling for service ID 0x000A */
										if (g_st_ServeceList[u16_Index].u16_ServiceID == 0x000A || g_st_ServeceList[u16_Index].u16_ServiceID == 0x0922)
										{
												SystemLinDiag_WriteData(pst_Tcb, u16_Index);
												break;  // Exit the loop after writing
										}
										/* Handle normal write request if flash is not locked */
										else if (((uint8_t)g_u8_FlashFlag & (uint8_t)FLASH_UPDATE_LOCKED) == 0)
										{
												SystemLinDiag_WriteData(pst_Tcb, u16_Index);

												/* Update flash flag if it is currently free */
												if ((uint8_t)g_u8_FlashFlag == (uint8_t)FLASH_UPDATE_FREE)
												{
														g_u8_FlashFlag = ((uint8_t)g_u8_FlashFlag | (uint8_t)FLASH_UPDATE_PROCESSING);
												}
										}
										else
										{
												/* Flash is locked; no operation performed */
												;
										}
								}
								else
								{
										/* Unsupported operation type; no action performed */
										;
								}

								break;  // Exit the loop once the matching service is processed
						}
				}
		} while (0);

		return u16_Ret;  // Return 0 if successful, otherwise return 1
}



/**
 * @brief 		Handle LIN diagnostic service for writing data (SID 0x2E request).
 * @note 		This function initializes the request control block for a write operation and processes the received data. 
 *               It determines the NAD type and calls `SystemLinDiag_DoRequest` to handle the request.
 * @param[in]	pu8_RecvBuffer -- Pointer to the received data buffer containing the diagnostic request.
 * @param[in]	u16_Did -- Diagnostic Identifier (DID) to be processed.
 * @param[in]	u16_RequestLength -- Length of the received request data.
 * @return		Operation result.
 * @retval		Returns 1 for success, or an error code if the request fails.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
uint16_t SystemLinDiag_HandleRecv(uint8_t *pu8_RecvBuffer, uint16_t u16_Did, uint16_t u16_RequestLength)
{
		uint16_t u16_Ret = 1;  // Default return value indicating failure

		/* Initialize the current request control block */
		g_st_CurrentRequestTcb.e_OperationType = E_DS_WRITE;          // Set the operation type to WRITE
		g_st_CurrentRequestTcb.u16_RequstDid = u16_Did;              // Set the requested DID (Data Identifier)
		g_st_CurrentRequestTcb.u16_TotalLength = 0;                  // Initialize total length to 0
		g_st_CurrentRequestTcb.u8_Amount = (uint8_t)(u16_RequestLength - 3);  // Calculate the amount of data to process (excluding the first 3 bytes)
		g_st_CurrentRequestTcb.u8_Pos = 0;                           // Set the starting position to 0
		g_st_CurrentRequestTcb.pu8_data = &pu8_RecvBuffer[3];        // Set the data pointer to the start of the data portion in the receive buffer (after 3 bytes)

		/* Determine the NAD (Node Address) type */
		if (l_get_current_NAD() == 0xA0u)
		{
				g_st_CurrentRequestTcb.u16_NadType = E_NAD_CAL;  // Set NAD type to calibration if current NAD is 0xA0
		}
		else
		{
				g_st_CurrentRequestTcb.u16_NadType = E_NAD_NORL; // Set NAD type to normal otherwise
		}

		/* Process the request using the current request control block */
		u16_Ret = SystemLinDiag_DoRequest(&g_st_CurrentRequestTcb, NULL);

		/* Return the result of the request processing */
		return u16_Ret;
}


/**
 * @brief 		Handle LIN diagnostic service for reading data (SID 0x22 response).
 * @note 		This function prepares the response buffer with the diagnostic read service identifier (0x62) and DID.
 *               It then initializes the request control block and processes the diagnostic request by calling `SystemLinDiag_DoRequest`.
 * @param[out]	pu8_SendBuffer -- Pointer to the response buffer to store the response data.
 * @param[in]	u16_Did -- Diagnostic Identifier (DID) to be processed.
 * @param[out]	pu16_RespLength -- Pointer to the response length, which will be updated with the number of bytes in the response.
 * @return		Operation result.
 * @retval		Returns 1 for success, or an error code if the request fails.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
uint16_t SystemLinDiag_HandleSend(uint8_t* pu8_SendBuffer, uint16_t u16_Did, uint16_t* pu16_RespLength)
{
		uint16_t u16_Ret = 1;  // Default return value indicating failure

		/* Prepare the response header */
		pu8_SendBuffer[0] = 0x62;  // Set the positive response identifier for read request
		pu8_SendBuffer[1] = (uint8_t)((u16_Did & 0xff00u) >> 8);  // High byte of the DID
		pu8_SendBuffer[2] = (uint8_t)(u16_Did & 0xffu);           // Low byte of the DID

		/* Initialize the current request control block for reading data */
		g_st_CurrentRequestTcb.u16_RequstDid = u16_Did;  // Set the requested DID
		g_st_CurrentRequestTcb.e_OperationType = E_DS_READ;  // Set operation type to READ
		g_st_CurrentRequestTcb.u8_Pos = 0;  // Set position to 0
		g_st_CurrentRequestTcb.u16_TotalLength = 0;  // Initialize total length to 0

		/* Determine the NAD (Node Address) type */
		if (l_get_current_NAD() == 0xA0u)
		{
				g_st_CurrentRequestTcb.u16_NadType = E_NAD_CAL;  // Set NAD type to calibration if current NAD is 0xA0
		}
		else
		{
				g_st_CurrentRequestTcb.u16_NadType = E_NAD_NORL;  // Set NAD type to normal otherwise
		}

		/* Perform the first request to get the response length */
		g_st_CurrentRequestTcb.pu8_data = NULL;  // Set data pointer to NULL for length query
		(void)SystemLinDiag_DoRequest(&g_st_CurrentRequestTcb, pu16_RespLength);

		/* Update the request control block with the response buffer and length */
		g_st_CurrentRequestTcb.pu8_data = &pu8_SendBuffer[3];  // Set the data pointer to start writing after the response header
		g_st_CurrentRequestTcb.u8_Amount = *pu16_RespLength;  // Set the amount to the response length

		/* Perform the actual data transfer */
		u16_Ret = SystemLinDiag_DoRequest(&g_st_CurrentRequestTcb, NULL);

		/* Return the result of the request handling */
		return u16_Ret;
}



/**
 * @brief 		Handle the diagnostic unlock process for LIN communication (SID 0x27).
 * @note 		This function manages seed and key-based unlocking for security access. It generates a seed for the request (0x27 0x03) 
 *               and validates the key in response (0x27 0x04). If the key is correct, it updates the flash flag and unlocks access.
 * @param[in]	pu8_RecvBuffer -- Pointer to the received data buffer.
 * @param[in]	u8_Did -- The sub-function identifier (0x03 for seed request, 0x04 for key verification).
 * @param[out]	pu8_SendBuffer -- Pointer to the response buffer where the response data will be written.
 * @param[out]	pu8_RespLength -- Pointer to the response length, which will be updated with the number of bytes in the response.
 * @return		Unlock process result.
 * @retval		Returns 0 for success, 1 for key mismatch.
 * @par  		Edition History
 Version Number   	Time    	 Author
 00.01		2017.10.24		Leo Song
 00.02       2024.12.12       Genhua Yang
 **/
uint16_t SystemLinDiag_UnlockHandle(const uint8_t * const pu8_RecvBuffer, uint8_t u8_Did, uint8_t* pu8_SendBuffer, uint16_t* pu8_RespLength)
{
		static uint32_t u32_Sid27Seed = 0;  // Seed value for SID 0x27
		static uint32_t u32_Sid27Key = 0;   // Key value for SID 0x27
		uint8_t u8_Ssid = 0, u8_Ret27 = 0;
		u8_Ssid = u8_Did;  // Sub-function identifier

		switch (u8_Ssid)
		{
				case 0x03:  // Request seed
						u32_Sid27Seed = (uint32_t)ModuleClock_TickGet();  // Generate a seed using the current clock tick

						/* Prepare the response with the seed value */
						*pu8_RespLength = 6;  // Response length: 2 bytes for header + 4 bytes for seed
						pu8_SendBuffer[0] = 0x67u;  // Positive response code for SID 0x27
						pu8_SendBuffer[1] = u8_Ssid;
						pu8_SendBuffer[2] = (uint8_t)(u32_Sid27Seed & 0x000000FFu);
						pu8_SendBuffer[3] = (uint8_t)((u32_Sid27Seed & 0x0000FF00u) >> 8);
						pu8_SendBuffer[4] = (uint8_t)((u32_Sid27Seed & 0x00FF0000u) >> 16);
						pu8_SendBuffer[5] = (uint8_t)((u32_Sid27Seed & 0xFF000000u) >> 24);

						u8_Ret27 = 0;  // Indicate success
						break;

				case 0x04:  // Send key
						u32_Sid27Key = (uint32_t)(((uint32_t)(pu8_RecvBuffer[1]) | (((uint32_t)pu8_RecvBuffer[2]) << 8)));

						/* Verify the key using the seed and a predefined offset (0x0C04) */
						if ((u32_Sid27Key & 0xFFFF) == (u32_Sid27Seed + 0x0C04))
						{
								/* Correct key provided, prepare success response */
								pu8_SendBuffer[0] = 0x67u;
								pu8_SendBuffer[1] = u8_Ssid;
								*pu8_RespLength = 2;  // Response length for success: 2 bytes
								u8_Ret27 = 0;
								g_st_SavedConfig.u16_LedSupplier = 0x0002u;  // Update configuration
								g_u8_FlashFlag = FLASH_UPDATE_PROCESSING;  // Set flash update flag to processing
						}
						else
						{
								/* Incorrect key provided, prepare negative response */
								pu8_SendBuffer[0] = 0x7Fu;  // Negative response code
								pu8_SendBuffer[1] = 0x67u;  // Response for SID 0x27
								*pu8_RespLength = 2;  // Response length for failure: 2 bytes
								u8_Ret27 = 1;  // Indicate failure
						}

						/* Reset seed and key after processing */
						u32_Sid27Seed = 0;
						u32_Sid27Key = 0;
						break;

				default:  // Unsupported sub-function
						/* Prepare general negative response for unsupported sub-function */
						pu8_SendBuffer[0] = 0x7Fu;  // Negative response code
						pu8_SendBuffer[1] = 0xB2u;  // Service not supported
						*pu8_RespLength = 2;  // Response length: 2 bytes
						u8_Ret27 = 0;

						/* Reset seed and key */
						u32_Sid27Seed = 0;
						u32_Sid27Key = 0;
						break;
		}

		return u8_Ret27;  // Return 0 for success, 1 for failure
}





