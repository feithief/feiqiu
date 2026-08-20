#include "Dcm.h"

#include "SystemLinDiag.h"
#include "LinTp.h"
#include "genLinConfig.h"

#ifdef LIN_COOKED_API

#define DCM_MESSAGE_LENGTH_MAX          (30U)

#define DCM_SID_READ_DID                (0x22U)
#define DCM_SID_SECURITY_ACCESS         (0x27U)
#define DCM_SID_WRITE_DID               (0x2EU)

#define DCM_POSITIVE_RESPONSE_OFFSET    (0x40U)
#define DCM_NEGATIVE_RESPONSE_SID       (0x7FU)

#define DCM_NRC_SERVICE_NOT_SUPPORTED   (0x11U)
#define DCM_NRC_INCORRECT_LENGTH        (0x13U)
#define DCM_NRC_REQUEST_OUT_OF_RANGE    (0x31U)

typedef enum
{
  DCM_STATE_ARM_RX = 0,
  DCM_STATE_WAIT_RX,
  DCM_STATE_WAIT_TX
} Dcm_StateType;

static Dcm_StateType Dcm_State;
static uint16_t Dcm_RequestLength;
static uint16_t Dcm_ResponseLength;
static uint8_t Dcm_Request[DCM_MESSAGE_LENGTH_MAX];
static uint8_t Dcm_Response[DCM_MESSAGE_LENGTH_MAX];

static void Dcm_PrepareNegativeResponse(uint8_t Sid, uint8_t Nrc)
{
  Dcm_Response[0] = DCM_NEGATIVE_RESPONSE_SID;
  Dcm_Response[1] = Sid;
  Dcm_Response[2] = Nrc;
  Dcm_ResponseLength = 3U;
  Dcm_State = DCM_STATE_WAIT_TX;
}

static uint16_t Dcm_GetDid(void)
{
  return (uint16_t)(((uint16_t)Dcm_Request[1] << 8U) |
                    (uint16_t)Dcm_Request[2]);
}

static void Dcm_ProcessReadDid(void)
{
  uint16_t result;

  if (Dcm_RequestLength != 3U)
  {
    Dcm_PrepareNegativeResponse(DCM_SID_READ_DID,
                                DCM_NRC_INCORRECT_LENGTH);
    return;
  }

  Dcm_ResponseLength = 0U;
  result = systemLinDiagHandleSend(Dcm_Response, Dcm_GetDid(),
                                   Dcm_RequestLength,
                                   &Dcm_ResponseLength);
  if (result != 0U)
  {
    Dcm_PrepareNegativeResponse(DCM_SID_READ_DID,
                                DCM_NRC_REQUEST_OUT_OF_RANGE);
    return;
  }

  Dcm_ResponseLength += 3U;
  Dcm_State = DCM_STATE_WAIT_TX;
}

static void Dcm_ProcessWriteDid(void)
{
  uint16_t result;
  uint16_t did;

  if (Dcm_RequestLength < 4U)
  {
    Dcm_PrepareNegativeResponse(DCM_SID_WRITE_DID,
                                DCM_NRC_INCORRECT_LENGTH);
    return;
  }

  did = Dcm_GetDid();
  result = systemLinDiagHandleRecv(Dcm_Request, did, Dcm_RequestLength);
  if (result == 2U)
  {
    Dcm_PrepareNegativeResponse(DCM_SID_WRITE_DID,
                                DCM_NRC_INCORRECT_LENGTH);
  }
  else if (result != 0U)
  {
    Dcm_PrepareNegativeResponse(DCM_SID_WRITE_DID,
                                DCM_NRC_REQUEST_OUT_OF_RANGE);
  }
  else
  {
    Dcm_Response[0] = DCM_SID_WRITE_DID + DCM_POSITIVE_RESPONSE_OFFSET;
    Dcm_Response[1] = (uint8_t)(did >> 8U);
    Dcm_Response[2] = (uint8_t)did;
    Dcm_ResponseLength = 3U;
    Dcm_State = DCM_STATE_WAIT_TX;
  }
}

static void Dcm_ProcessSecurityAccess(void)
{
  uint8_t subFunction;

  if (Dcm_RequestLength < 2U)
  {
    Dcm_PrepareNegativeResponse(DCM_SID_SECURITY_ACCESS,
                                DCM_NRC_INCORRECT_LENGTH);
    return;
  }

  subFunction = Dcm_Request[1];
  if (((subFunction == 0x03U) && (Dcm_RequestLength != 2U)) ||
      ((subFunction == 0x04U) && (Dcm_RequestLength != 6U)))
  {
    Dcm_PrepareNegativeResponse(DCM_SID_SECURITY_ACCESS,
                                DCM_NRC_INCORRECT_LENGTH);
    return;
  }

  (void)systemLinDiag0x27Handle(&Dcm_Request[1], subFunction,
                                Dcm_Response, &Dcm_ResponseLength);
  Dcm_State = DCM_STATE_WAIT_TX;
}

static void Dcm_DispatchRequest(void)
{
  if (Dcm_RequestLength == 0U)
  {
    Dcm_State = DCM_STATE_ARM_RX;
    return;
  }

  switch (Dcm_Request[0])
  {
    case DCM_SID_READ_DID:
      Dcm_ProcessReadDid();
      break;

    case DCM_SID_WRITE_DID:
      Dcm_ProcessWriteDid();
      break;

    case DCM_SID_SECURITY_ACCESS:
      Dcm_ProcessSecurityAccess();
      break;

    default:
      Dcm_PrepareNegativeResponse(Dcm_Request[0],
                                  DCM_NRC_SERVICE_NOT_SUPPORTED);
      break;
  }
}

void Dcm_MainFunction(void)
{
  uint8_t rxStatus = ld_rx_status();
  uint8_t txStatus = ld_tx_status();

  if ((rxStatus > LD_COMPLETED) || (txStatus > LD_COMPLETED))
  {
    (void)ld_init();
    Dcm_State = DCM_STATE_ARM_RX;
    return;
  }

  if (Dcm_State == DCM_STATE_ARM_RX)
  {
    if (rxStatus == LD_COMPLETED)
    {
      Dcm_RequestLength = DCM_MESSAGE_LENGTH_MAX;
      ld_receive_message(&Dcm_RequestLength, Dcm_Request);
      Dcm_State = DCM_STATE_WAIT_RX;
    }
  }
  else if (Dcm_State == DCM_STATE_WAIT_RX)
  {
    if (rxStatus == LD_COMPLETED)
    {
      Dcm_State = DCM_STATE_ARM_RX;
      Dcm_DispatchRequest();
    }
  }
  else
  {
    if (txStatus == LD_COMPLETED)
    {
      ld_send_message(Dcm_ResponseLength, Dcm_Response);
      Dcm_State = DCM_STATE_ARM_RX;
    }
  }
}

#else

void Dcm_MainFunction(void)
{
}

#endif
