#ifndef DCM_INTERNAL_H
#define DCM_INTERNAL_H

#include "Dcm.h"

#define DCM_SID_READ_DATA_BY_IDENTIFIER  (0x22U)
#define DCM_SID_WRITE_DATA_BY_IDENTIFIER (0x2EU)
#define DCM_SID_ASSIGN_NAD               (0xB0U)
#define DCM_SID_READ_BY_ID               (0xB2U)
#define DCM_SID_COND_CHANGE_NAD          (0xB3U)
#define DCM_SID_SAVE_CONFIG              (0xB6U)
#define DCM_SID_ASSIGN_ID_RANGE          (0xB7U)

#define DCM_POSITIVE_RESPONSE_OFFSET      (0x40U)
#define DCM_NEGATIVE_RESPONSE_SID         (0x7FU)
#define DCM_NRC_SERVICE_NOT_SUPPORTED     (0x11U)
#define DCM_NRC_SUBFUNCTION_NOT_SUPPORTED (0x12U)
#define DCM_NRC_INCORRECT_LENGTH          (0x13U)
#define DCM_NRC_REQUEST_OUT_OF_RANGE      (0x31U)
#define DCM_NAD_BROADCAST                  (0x7FU)

void Dcm_DslInit(void);
void Dcm_DslMainFunction(void);
void Dcm_DslTransmit(const uint8_t *DataPtr, uint16_t Length,
                     uint8_t ResponseNad);
uint8_t *Dcm_DslGetTxBuffer(void);
uint16_t Dcm_DslGetTxBufferCapacity(void);
BufReq_ReturnType Dcm_DslStartOfReception(
    PduIdType RxPduId, const PduInfoType *PduInfoPtr,
    PduLengthType TpSduLength, PduLengthType *BufferSizePtr);
BufReq_ReturnType Dcm_DslCopyRxData(PduIdType RxPduId,
                                    const PduInfoType *PduInfoPtr,
                                    PduLengthType *BufferSizePtr);
void Dcm_DslTpRxIndication(PduIdType RxPduId, Std_ReturnType Result);
BufReq_ReturnType Dcm_DslCopyTxData(PduIdType TxPduId,
                                    const PduInfoType *PduInfoPtr,
                                    PduLengthType *AvailableDataPtr);
void Dcm_DslTpTxConfirmation(PduIdType TxPduId, Std_ReturnType Result);

void Dcm_DsdDispatch(const uint8_t *RequestPtr, uint16_t RequestLength,
                     uint8_t Nad);
void Dcm_DsdSendNegative(uint8_t Sid, uint8_t Nrc, uint8_t Nad);

void Dcm_DspInit(const Dcm_ConfigType *ConfigPtr, uint8_t InitialNad);
void Dcm_DspProcessService(uint8_t Sid, const uint8_t *RequestPtr,
                           uint16_t RequestLength, uint8_t Nad);
bool Dcm_DspIsRequestNadAllowed(uint8_t Nad);
void Dcm_DspSetNad(uint8_t Nad);
uint8_t Dcm_DspGetNad(void);
uint8_t Dcm_DspGetInitialNad(void);
bool Dcm_DspTakeNadChange(uint8_t *NadPtr);

#endif
