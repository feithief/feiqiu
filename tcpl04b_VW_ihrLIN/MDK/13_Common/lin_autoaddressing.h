#ifndef LIN_AUTOADDRESSING_COMPAT_H
#define LIN_AUTOADDRESSING_COMPAT_H

#include <stdint.h>

typedef void (*AA_INITIAL_CALLBACK)(void);

#define D_AUTO_ADDR_STEP1_START_AA    (0x01U)
#define D_AUTO_ADDR_STEP2_ASSIGN_NAD  (0x02U)
#define D_AUTO_ADDR_STEP3_STORE_NAD   (0x03U)
#define D_AUTO_ADDR_STEP4_FINISH_AA   (0x04U)

typedef enum
{
    AddressStatusSuccess = 0x00,
    AddressStatusWatting = 0x01
} E_AddrStatus;

typedef enum
{
    E_StateMachineStatusIdle = 0x00,
    E_StateMachineStatusStart = 0x01,
    E_StateMachineStatusWaitting = 0x02,
    E_StateMachineStatusAddressed = 0x04
} E_StateMachineStatus;

void LIN_AutoAddr_Init(AA_INITIAL_CALLBACK Start, AA_INITIAL_CALLBACK End);
void LIN_AutoAddr_Start(void);
E_AddrStatus LIN_AutoAddr_Assign_NAD(void);
void LIN_AutoAddr_End(void);
E_StateMachineStatus LIN_AutoAddr_Get_Status(void);

#endif
