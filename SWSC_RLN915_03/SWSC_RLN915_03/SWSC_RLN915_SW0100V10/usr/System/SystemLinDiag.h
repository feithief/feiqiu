#ifndef _SYSTEMLINDIAG_H_
#define _SYSTEMLINDIAG_H_

#include "SystemType.h"
#include "linStackTask.h"
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
}EDiagServiceRW;

/**@brief Defines service NAD attributes*/
typedef enum DiagServiceNad
{
  NAD_NORL  = 0x1,          /**<service has can access in normal NAD.*/
  NAD_CAL   = 0x2,          /**<service has can access in calibration NAD.*/
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

typedef struct Debug
{
  uint16_t        VR;  /**<service ID, must unique.*/
  uint16_t        VG;     /**<read/write attribute.*/
  uint16_t        VB;    /**<NAD attribute.*/
  uint16_t        VS;     /**<service's content length.*/
}Debug;
//extern Debug wkz;
/**
*@details   LIN diagnostic receiving call back function.
*
*@param[in] recvBuffer  Diagnostic received buffer..
*
*@retval    IHR's response code.
*/
//uint16_t systemLinDiagHandleSend(uint8_t* sendBuffer, uint16_t DID, uint16_t requestLength, uint16_t* respLength);
uint16_t systemLinDiag0x2EHandle(const DiagReqInfo_t *const DiagReq, uint8_t * ReqData_buffer, DiagRspInfo_t* diagRsp);
/**
*@details   LIN diagnostic sending call back function.
*
*@param[in] recvBuffer  Diagnostic received buffer..
*
*@retval    IHR's response code.
*/
//uint16_t systemLinDiagHandleRecv(uint8_t* recvBuffer, uint16_t DID, uint16_t requestLength);
uint16_t systemLinDiag0x22Handle(const DiagReqInfo_t *const DiagReq, DiagRspInfo_t* diagRsp);
/** @} */

/**@brief software VERSION section length.*/
#define DSOFTWARE_VERSION_LENGTH  6U
extern const uint8_t softwareVersion[DSOFTWARE_VERSION_LENGTH+1];    /* 7 bytes */

uint16_t systemLinDiag0x27Handle(const DiagReqInfo_t *const DiagReq, uint8_t * ReqData_buffer, DiagRspInfo_t* diagRsp);


#endif
