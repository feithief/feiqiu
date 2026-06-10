#ifndef _SYSTEMLINDIAG_H_
#define _SYSTEMLINDIAG_H_

#include "SystemType.h"

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

/**@brief Defines service attributes*/
typedef enum DiagServiceRW
{
  DS_READ   = 0x1,  /**<service has write property.*/
  DS_WRITE  = 0x2,  /**<service has read property.*/
  DS_READWRITE = DS_READ|DS_WRITE,
	DS_Nolock = 0x4,  /**<Because autoaddressing can be written with or without locking*/
  DS_RESERT = 0x8,   /**The LIN write information can only be reset*/
}EDiagServiceRW;

/**@brief Defines service NAD attributes*/
typedef enum DiagServiceNad
{
  NAD_NORL  = 0x1,  /**<service has can access in normal NAD.*/
  NAD_CAL   = 0x2,  /**<service has can access in calibration NAD.*/
  NAD_NORLCAL = NAD_NORL|NAD_CAL,
}EDiagServiceNad;

/**@brief Defines diagnostic service's structure*/
typedef struct LinDiagSevice
{
  uint16_t        serviceID;  /**<service ID, must unique.*/
  EDiagServiceRW  rwAttr;     /**<read/write attribute.*/
  EDiagServiceNad nadAttr;    /**<NAD attribute.*/
  uint16_t        length;     /**<service's content length.*/
  uint8_t         *addr;      /**<service's content address in ram.*/
} SLinDiagSevice;

/**
*@details   LIN diagnostic receiving call back function.
*
*@param[in] recvBuffer  Diagnostic received buffer..
*
*@retval    IHR's response code.
*/
uint16_t systemLinDiagHandleSend(uint8_t* sendBuffer, uint16_t DID, uint16_t requestLength, uint16_t* respLength);

/**
*@details   LIN diagnostic sending call back function.
*
*@param[in] recvBuffer  Diagnostic received buffer..
*
*@retval    IHR's response code.
*/
uint16_t systemLinDiagHandleRecv(uint8_t* recvBuffer, uint16_t DID, uint16_t requestLength);

/** @} */
void systemLinCommRespUpdate(void);

uint16_t systemLinDiag0x27Handle(uint8_t* recvBuffer, uint8_t DID, uint8_t* sendBuffer, uint16_t* respLength);

#endif
