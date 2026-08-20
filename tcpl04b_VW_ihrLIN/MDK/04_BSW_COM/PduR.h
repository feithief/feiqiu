#ifndef PDUR_H
#define PDUR_H

#include "ComStack_Types.h"

void PduR_LinIfRxIndication(PduIdType RxPduId,
                            const PduInfoType *PduInfoPtr);
Std_ReturnType PduR_LinIfTriggerTransmit(PduIdType TxPduId,
                                        PduInfoType *PduInfoPtr);
void PduR_LinIfTxConfirmation(PduIdType TxPduId, Std_ReturnType Result);
void PduR_LinIfErrorIndication(Lin_SlaveErrorType ErrorStatus);

Std_ReturnType PduR_DcmTransmit(PduIdType TxPduId,
                               const PduInfoType *PduInfoPtr);
BufReq_ReturnType PduR_LinTpStartOfReception(
    PduIdType RxPduId, const PduInfoType *PduInfoPtr,
    PduLengthType TpSduLength, PduLengthType *BufferSizePtr);
BufReq_ReturnType PduR_LinTpCopyRxData(PduIdType RxPduId,
                                      const PduInfoType *PduInfoPtr,
                                      PduLengthType *BufferSizePtr);
void PduR_LinTpRxIndication(PduIdType RxPduId, Std_ReturnType Result);
BufReq_ReturnType PduR_LinTpCopyTxData(PduIdType TxPduId,
                                      const PduInfoType *PduInfoPtr,
                                      PduLengthType *AvailableDataPtr);
void PduR_LinTpTxConfirmation(PduIdType TxPduId, Std_ReturnType Result);

#endif
