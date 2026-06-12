/*
 ************************************************************************
 * Copyright  (c)   KEBODA TECHNOLOGY CO.,LTD.
 * All Rights Reserved
 * Filename:		SystemLinDiag.h
 * Project:		RLNE15S
 * Description:	This header file declares functions and data structures for 
 *              managing LIN diagnostic services. It supports handling read 
 *              and write requests, diagnostic communication, and security 
 *              access unlocking for LIN protocol-based systems.
 * Note(s):	    - This module integrates with the MELEXIS LIN protocol stack.
 *                Callback functions are registered with the stack for 
 *                handling diagnostic requests and responses.
 * Update Record:	
 * Version     	Date     	 Modified by       Changes Made
 * 00.01      2024.11.28      Genhua Yang      Initial version
 ************************************************************************
 */

#ifndef SYSTEMLINDIAG_H
#define SYSTEMLINDIAG_H
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

#include "SystemType.h"
/**********************************************************************************************************************
  INCLUDE FILES
  **********************************************************************************************************************/

/** @file
*
* @defgroup SYSTEM_LIN_DIAG LIN diagnostic module.
* @{
*
* @ingroup RGB_SYSTEM
*
* @brief This module manage LIN diagnostic functions.
*
* @details This module uses MELEXIS' LIN protocol stack.The two call back functions has already insert to MELEXIS' stack.
*/

/*!< Defines service attributes >*/
typedef enum
{
	E_DS_READ   = 0x1,  /*!< service has write property >*/
	E_DS_WRITE  = 0x2,  /*!< service has read property >*/
	E_DS_READWRITE = 0x3, /*!< service supports both read and write >*/
} E_DIAG_SERVICE_RW;


/*!< Defines service NAD attributes >*/
typedef enum
{
	E_NAD_NORL  = 0x1,  /*!< service has can access in normal NAD >*/
	E_NAD_CAL   = 0x2,  /*!< service has can access in calibration NAD >*/
	E_NAD_NORLCAL = 0x3, /*!< service can access in both normal and calibration NAD >*/
} E_DIAG_SERVICE_NAD;


/*!< Defines diagnostic service's structure >*/
typedef struct
{
	uint16_t        u16_ServiceID;  /*!< service ID, must unique >*/
	E_DIAG_SERVICE_RW  e_RwAttr;     /*!< read/write attribute >*/
	E_DIAG_SERVICE_NAD e_NadAttr;    /*!< NAD attribute >*/
	uint16_t        u16_Length;     /*!< service's content length >*/
	uint8_t         *addr;      /*!< service's content address in RAM >*/
} ST_LinDiagSevice;


/*!< Task control structure >*/
typedef struct 
{
	uint16_t        u16_RequstDid;      /*!< KEBODA DID >*/
	uint16_t        u16_TotalLength;    /*!< total access length, may not be used in reading >*/
	E_DIAG_SERVICE_RW  e_OperationType;  /*!< Access operation type >*/
	E_DIAG_SERVICE_NAD u16_NadType;        /*!< Access NAD type >*/
	uint8_t         u8_Pos;            /*!< operation's start position of array >*/
	uint8_t         u8_Amount;         /*!< In reading, it dictates the read out u16_Length. In writing, it represents the u16_Length of one writing operation >*/
	uint8_t*        pu8_data;           /*!< pointer to pu8_data. This can be both input and output >*/
} ST_RequsetTCB;

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
uint16_t SystemLinDiag_HandleSend(uint8_t* pu8_SendBuffer, uint16_t u16_Did, uint16_t* pu16_RespLength);

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
uint16_t SystemLinDiag_HandleRecv(uint8_t* pu8_RecvBuffer, uint16_t u16_Did, uint16_t u16_RequestLength);

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
uint16_t SystemLinDiag_UnlockHandle(const uint8_t * const pu8_RecvBuffer, uint8_t u8_Did, uint8_t* pu8_SendBuffer, uint16_t* pu8_RespLength);/** @} */

#endif
