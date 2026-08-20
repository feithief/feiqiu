#ifndef LIN_TP_H
#define LIN_TP_H

#include "ComStack_Types.h"

#define LINTP_MAX_MESSAGE_LENGTH       (64U)
#define LINTP_RX_NSDU_ID               ((PduIdType)0U)
#define LINTP_TX_NSDU_ID               ((PduIdType)1U)

void LinTp_Init(void);
void LinTp_MainFunction(void);
void LinTp_RxIndication(const PduInfoType *PduInfoPtr);
Std_ReturnType LinTp_TriggerTransmit(PduInfoType *PduInfoPtr);
void LinTp_TxConfirmation(Std_ReturnType Result);
Std_ReturnType LinTp_Transmit(PduIdType TxSduId,
                              const PduInfoType *PduInfoPtr);

#endif
